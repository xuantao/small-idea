#pragma once

#include <memory>
#include <cassert>

namespace detail
{
    /*
     * 从VC标准库拷贝出来的
     * 经测试clang 3.4和gcc 4.8 不支持std::align
    */
    inline void* Align(size_t _Bound, size_t _Size,
        void*& _Ptr, size_t& _Space)
    {
        // try to carve out _Size bytes on boundary _Bound
        size_t _Off = (size_t)((uintptr_t)_Ptr & (_Bound - 1));
        if (0 < _Off)
            _Off = _Bound - _Off;	// number of bytes to skip
        if (_Space < _Off || _Space - _Off < _Size)
            return (0);
        else
        {   // enough room, update
            _Ptr = (char *)_Ptr + _Off;
            _Space -= _Off;
            return (_Ptr);
        }
    }

    // 回滚操作链表的节点基类
    struct KGRollbackNode
    {
        virtual ~KGRollbackNode() {}
        virtual void Work() = 0;
        virtual size_t Size() const = 0;

        KGRollbackNode* m_pNext = nullptr;
    };

    // 存储回滚操作函数对象
    template <typename Fty>
    struct KGRollbackNodeImpl : KGRollbackNode
    {
        typedef Fty FuncType;
        typedef KGRollbackNodeImpl<Fty> _MyType;

        KGRollbackNodeImpl(const FuncType& fn) : m_Func(fn)
        { }

        KGRollbackNodeImpl(FuncType&& fn) : m_Func(std::forward<FuncType>(fn))
        { }

        virtual ~KGRollbackNodeImpl() {}

        virtual void Work() { m_Func(); }
        virtual size_t Size() const { return sizeof(_MyType); }

        FuncType m_Func;
    };

    /*
     * 块分配器节点
     * 专用分配器，只分配不释放
     * _Bs:     块大小
     * _Align:  内存地址对齐
    */
    template <size_t _BlockSize, size_t _Align>
    struct KGBlockAllocNode
    {
        enum { BlockSize = _BlockSize, Alignment = _Align };
        typedef KGBlockAllocNode<_BlockSize, _Align> _MyType;

        void* Alloc(size_t nSize)
        {
            void* p = &m_Data[BlockSize - m_Size];
            if (!Align(Alignment, nSize, p, m_Size))
                return nullptr;

            m_Size -= nSize;
            return p;
        }

        _MyType*    m_pNext = nullptr;
        size_t      m_Size = BlockSize;
        char        m_Data[BlockSize];
    };

    /*
     * KGScopeGuard 专用分配器
     * 根据应用环境定制的分配器，减少/避免系统内存分配次数。
     * 默认在对象内部持有一个分配器节点（栈上），当不够用时再从堆上分配一个节点。
     * 如果需要分配的大小超过限制，则直接从堆上分配。
    */
    template <size_t _BlockSize>
    class KGScopeGuardAllocator
    {
    public:
        enum { BlockSize = _BlockSize, Alignment = sizeof(void*) };
        typedef KGBlockAllocNode<BlockSize, Alignment> AllocNode;
        typedef KGScopeGuardAllocator<_BlockSize> _MyType;

        KGScopeGuardAllocator() : m_pAlloc(&m_DefAlloc)
        { }

        ~KGScopeGuardAllocator()
        {
#ifdef _DEBUG
            if (m_pAlloc->m_pNext)
            {
                size_t nTotal = 0;
                auto pTmp = m_pAlloc;
                while (pTmp)
                {
                    nTotal += BlockSize;
                    pTmp = pTmp->m_pNext;
                }
            }
#endif // _DEBUG

            auto pNode = m_pAlloc;
            m_pAlloc = nullptr;

            while (pNode->m_pNext)
            {
                auto pTmp = pNode;
                pNode = pNode->m_pNext;
                delete pTmp;
            }
        }

        void* Alloc(size_t nSize)
        {
            if (NeedRawAlloc(nSize))
                return new char[nSize];

            void* p = m_pAlloc->Alloc(nSize);
            if (p != nullptr)
                return p;

            auto pTmp = new AllocNode();
            pTmp->m_pNext = m_pAlloc;
            m_pAlloc = pTmp;

            p = m_pAlloc->Alloc(nSize);
            assert(p);
            return p;
        }

        void Dealloc(void* p, size_t nSize)
        {
            if (NeedRawAlloc(nSize))
                delete[] static_cast<char*>(p);
        }

    private:
        inline bool NeedRawAlloc(size_t nSize) const
        {
            // ignore alignment cause alloc error
            // when s is closer BlockSize,
            // align operate may be not alloc successfully
            return (nSize + Alignment > BlockSize);
        }

        AllocNode*  m_pAlloc = nullptr;
        AllocNode   m_DefAlloc;     // default alloc node, reduce system alloc call
    };

    template <>
    struct KGScopeGuardAllocator<0>
    {
    public:
        enum { BlockSize = 0, Alignment = sizeof(void*) };

    public:
        void* Alloc(size_t size)
        {
            return new char[size];
        }

        void Dealloc(void* p, size_t size)
        {
            delete[] static_cast<char*>(p);
        }
    };

    /*
     * 作用域卫士
     * 守护作用域内的资源，错误回滚等
     * 回滚操作的是逆序的（栈，先入后出）
     * WARN: 回滚函数使用lambda时要注意引用对象的生命期
     * _BlockSize: 内存块大小，每次增量分配大小（默认持有一块），若传0表示直接在堆上分配
    */
    template <size_t _BlockSize>
    class KGScopeGuard
    {
    public:
        enum { BlockSize = _BlockSize };
        typedef KGScopeGuardAllocator<_BlockSize> Allocator;
        typedef KGRollbackNode Node;

    public:
        KGScopeGuard() {}
        ~KGScopeGuard() { Done(); }

        KGScopeGuard(const KGScopeGuard&) = delete;
        KGScopeGuard& operator = (const KGScopeGuard&) = delete;

    public:
        /*
         * 压入一个回滚函数对象
         * func() 形式的函数
        */
        template <typename Fty>
        void Push(Fty&& func)
        {
            typedef typename std::decay<Fty>::type decay;
            typedef KGRollbackNodeImpl<decay> NodeImpl;

            auto pBuff = m_Alloc.Alloc(sizeof(NodeImpl));
            assert(pBuff);

            auto pNode = new (pBuff) NodeImpl(std::forward<Fty>(func));
            pNode->m_pNext = m_pTop;
            m_pTop = pNode;
        }

        inline bool IsDismissed() const { return m_bDismiss; }

        inline void Dismiss() { m_bDismiss = true; }

        /*
         * 结束守卫工作, 如果没有Dismiss则会执行回滚操作
         * 建议: 使用ScopeGuard的时候都手动调用Done来结束,
         *       可以避免部分lambda引用局部变量的生命期问题
         * 返回值：IsDismissed()
        */
        bool Done()
        {
            if (!IsDismissed())
                Rollback();
            Clear();
            return IsDismissed();
        }

    private:
        void Rollback()
        {
            auto pNode = m_pTop;
            while (pNode)
            {
                pNode->Work();
                pNode = pNode->m_pNext;
            }
        }

        void Clear()
        {
            auto pNode = m_pTop;
            m_pTop = nullptr;

            while (pNode)
            {
                auto pTmp = pNode;
                auto nSize = pNode->Size();
                pNode = pNode->m_pNext;

                pTmp->~Node();
                m_Alloc.Dealloc(pTmp, nSize);
            }
        }

    private:
        bool        m_bDismiss = false;
        Node*       m_pTop = nullptr;
        Allocator   m_Alloc;
    };
}

/*
 * 作用域守护
 * 方便反初始化、异常资源释放
 * 默认512字节大小是个经验值, 在64位程序中，像 [this] { //TODO: } 这样持有一个指针变量的lambda,
 * 在内部构建的回滚对象大小是24Byte, 默认栈上的分配器可以分配21个回滚对象, 能够满足大多数情况了。
*/
typedef detail::KGScopeGuard<512> KGScopeGuard;
