#pragma once

#include <memory>
#include <cassert>
#include "KGSerialAllocator.h"

namespace ScopeGuard_Internal
{
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

    template <size_t N, size_t A = sizeof(void*)>
    class Allocator
    {
    public:
        Allocator() : m_Alloc(N)
        {
        }

        ~Allocator()
        {
        }

        Allocator(const Allocator&) = delete;
        Allocator& operator = (const Allocator&) = delete;

    public:
        void* Alloc(size_t size)
        {
            return m_Alloc.Alloc(size);
        }

        void Dealloc(void* p, size_t size)
        {
        }

    private:
        KGPoolSerialAlloc<N, A> m_Alloc;
    };

} // namespace ScopeGuard_Internal

/*
 * 作用域卫士
 * 守护作用域内的资源，错误回滚等
 * 回滚操作的是逆序的（栈，先入后出）
 * WARN: 回滚函数使用lambda时要注意引用对象的生命期
 * Alloc: 内存分配器
*/
template <typename Alloc = ScopeGuard_Internal::Allocator<512>>
class KGScopeGuardImpl
{
public:
    typedef Alloc Allocator;
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

/*
 * 作用域守护
 * 方便反初始化、异常资源释放
 * 默认512字节大小是个经验值, 在64位程序中，像 [this] { //TODO: } 这样持有一个指针变量的lambda,
 * 在内部构建的回滚对象大小是24Byte, 默认栈上的分配器可以分配21个回滚对象, 能够满足大多数情况了。
*/
typedef KGScopeGuardImpl<> KGScopeGuard;
