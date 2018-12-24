#pragma once
#include <type_traits>
#include <algorithm>
#include <cassert>
#include "KGTemplate.h"
#include "KGAlloctorAdapter.h"

template <size_t A>
class KGSerialAllocator;

template <typename Ty, size_t A = sizeof(void*)>
class KGSerialAllocatorAdapter : public KGAlloctorAdapter<Ty, KGSerialAllocator<A>>
{
    friend class KGSerialAllocator<A>;
    typedef KGAlloctorAdapter<Ty, KGSerialAllocator<A>> BaseType;

public:
    KGSerialAllocatorAdapter(const KGSerialAllocatorAdapter& o) : BaseType(o.m_pAlloc)
    {
    }

    template <typename U>
    KGSerialAllocatorAdapter(const KGSerialAllocatorAdapter<U, A>& o) : BaseType(o.m_pAlloc)
    {
    }

private:
    KGSerialAllocatorAdapter() = default;

    KGSerialAllocatorAdapter(KGSerialAllocator<A>* pAlloc) : BaseType(pAlloc)
    {
    }
};

/*
 * 序列式分配器, 顺序从一块缓存中分配一段内存
 * 只分配不负责释放。当不够空间分配时返回nullptr
*/
template <size_t A = sizeof(void*)>
class KGSerialAllocatorImpl
{
public:
    static constexpr size_t AlignByte = A;

public:
    KGSerialAllocatorImpl(void* pool, size_t cap_size)
        : m_pPool((int8_t*)pool), m_nCapacity(cap_size)
    { }

    KGSerialAllocatorImpl(KGSerialAllocatorImpl&& other)
    {
        MoveFrom(other);
    }

    ~KGSerialAllocatorImpl()
    {
    }

    KGSerialAllocatorImpl& operator = (KGSerialAllocatorImpl&& other)
    {
        MoveFrom(other);
    }

public:
    inline bool Empty() const { return m_nAlloced == 0; }
    inline size_t Size() const { return m_nAlloced; }
    inline size_t Capacity() const { return m_nCapacity; }
    inline void Reset() { m_nAlloced = 0; }

    inline void* Alloc(size_t sz)
    {
        size_t al_sz = KGAlignSize(sz, AlignByte);
        if (al_sz > m_nCapacity - m_nAlloced)
            return nullptr;

        int8_t* buf = m_pPool + m_nAlloced;
        m_nAlloced += al_sz;
        return buf;
    }

    void Dealloc(void*, size_t)
    {
        //
    }

    void Swap(KGSerialAllocatorImpl& other)
    {
        std::swap(other.m_nAlloced, m_nAlloced);
        std::swap(other.m_nCapacity, m_nCapacity);
        std::swap(other.m_pPool, m_pPool);
    }

protected:
    void MoveFrom(KGSerialAllocatorImpl& other)
    {
        m_nAlloced = other.m_nAlloced;
        m_nCapacity = other.m_nCapacity;
        m_pPool = other.m_pPool;

        other.m_nAlloced = 0;
        other.m_nCapacity = 0;
        other.m_pPool = nullptr;
    }

private:
    size_t m_nAlloced = 0;
    size_t m_nCapacity;
    int8_t* m_pPool;
}; // class KGSerialAllocatorImpl


/* serial allocator
 * only alloc memory but do not dealloc
*/
template <size_t A = sizeof(void*)>
class KGSerialAllocator
{
public:
    typedef KGSerialAllocatorImpl<A> AllocImpl;
    typedef KGSinglyNode<AllocImpl> AllocNode;
    //using AllocImpl::AlignByte;
    static constexpr size_t DefaultBlock = 4096;

public:
    KGSerialAllocator()
        : m_nBlock(DefaultBlock)
        , m_AllocNode(0, 0)
    {
    }

    KGSerialAllocator(size_t block)
        : m_nBlock(block)
        , m_AllocNode(0, 0)
    {
    }

    ~KGSerialAllocator()
    {
        Dissolve();
    }

    KGSerialAllocator(const KGSerialAllocator&) = delete;
    KGSerialAllocator& operator = (const KGSerialAllocator&) = delete;
    //TODO: add move semantic

protected:
    /* build allocator with default pool, the default pool will node free */
    KGSerialAllocator(void* pDefautPool, size_t nDefaultSize, size_t block = DefaultBlock)
        : m_bMoveable(false)
        , m_nBlock(block)
        , m_AllocNode(pDefautPool, nDefaultSize)
    {
    }

public:
    inline bool IsMoveable() const
    {
        return m_bMoveable;
    }

    void* Alloc(size_t nSize)
    {
        void* pBuff = m_AllocNode.Value.Alloc(nSize);
        if (pBuff)
            return pBuff;

        CreateNode(std::max(KGAlignSize(nSize, AllocImpl::AlignByte), m_nBlock));

        pBuff = m_AllocNode.Value.Alloc(nSize);
        assert(pBuff);
        return pBuff;
    }

    void Dealloc(void*, size_t)
    {
        //
    }

    /* Reset the allocator without free alloc node */
    void Reset()
    {
        AllocNode* pCur = nullptr;
        AllocNode* pNode = m_AllocNode.pNext;

        while (pNode)
        {
            pCur = pNode;
            pNode = pNode->pNext;

            pCur->pNext = m_pEmptyNode;
            pCur->Value.Reset();
            m_pEmptyNode = pCur;
        }

        m_AllocNode.pNext = nullptr;
        m_AllocNode.Value.Reset();
        if (pCur)
        {
            m_AllocNode.Value.Swap(pCur->Value);
        }
    }

    /* Reset the allocator and free alloc node */
    void Dissolve()
    {
        AllocNode* pNode = m_AllocNode.pNext;
        m_AllocNode.pNext = nullptr;
        while (pNode)
        {
            AllocNode* pCur = pNode;
            pNode = pNode->pNext;

            if (pNode == nullptr)
                m_AllocNode.Value.Swap(pCur->Value);

            pCur->~AllocNode();
            delete reinterpret_cast<int8_t*>(pCur);
        }

        while (m_pEmptyNode)
        {
            AllocNode* pCur = m_pEmptyNode;
            m_pEmptyNode = m_pEmptyNode->pNext;

            pCur->~AllocNode();
            delete reinterpret_cast<int8_t*>(pCur);
        }
    }

    template <typename Ty>
    KGSerialAllocatorAdapter<Ty, A> GetAdapter()
    {
        return KGSerialAllocatorAdapter<Ty, A>(this);
    }

private:
    void CreateNode(size_t nSize)
    {
        AllocNode* pNode = FindSuitableNode(nSize);
        if (pNode == nullptr)
        {
            constexpr size_t NodeObjSize = KGAlignSize(sizeof(AllocNode), AllocImpl::AlignByte);
            int8_t* pMem = new int8_t[NodeObjSize + nSize];
            assert(pMem);

            pNode = new (pMem) AllocNode(pMem + NodeObjSize, nSize);
        }

        m_AllocNode.Value.Swap(pNode->Value);
        pNode->pNext = m_AllocNode.pNext;
        m_AllocNode.pNext = pNode;
    }

    AllocNode* FindSuitableNode(size_t nSize)
    {
        AllocNode* pPrev = nullptr;
        AllocNode* pNode = m_pEmptyNode;

        while (pNode)
        {
            if (pNode->Value.Capacity() < nSize)
            {
                pPrev = pNode;
                pNode = pNode->pNext;
            }
            else
            {
                if (pPrev)
                    pPrev->pNext = pNode->pNext;
                else
                    m_pEmptyNode = pNode->pNext;
                break;
            }
        }

        return pNode;
    }

private:
    bool        m_bMoveable = true;
    size_t      m_nBlock;
    AllocNode*  m_pEmptyNode;
    AllocNode   m_AllocNode;
};

/* serial allocator with an default memory pool */
template <size_t N, size_t A = sizeof(void*)>
class KGPoolSerialAlloc : public KGSerialAllocator<A>
{
public:
    KGPoolSerialAlloc() : KGSerialAllocator<A>(m_Pool, N)
    {
    }

    KGPoolSerialAlloc(size_t nBlock) : KGSerialAllocator<A>(m_Pool, N, nBlock)
    {
    }

    ~KGPoolSerialAlloc()
    {
    }

    KGPoolSerialAlloc(const KGPoolSerialAlloc&) = delete;
    KGPoolSerialAlloc(KGPoolSerialAlloc&&) = delete;
    KGPoolSerialAlloc& operator = (KGPoolSerialAlloc&&) = delete;
    KGPoolSerialAlloc& operator = (const KGPoolSerialAlloc&) = delete;

private:
    alignas (A) int8_t m_Pool[N];
};
