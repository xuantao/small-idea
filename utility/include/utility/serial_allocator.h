#pragma once
#include <type_traits>
#include <algorithm>
#include <cassert>
#include "common.h"
#include "allocator_adapter.h"

UTILITY_NAMESPACE_BEGIN
/*
 * 序列式分配器, 顺序从一块缓存中分配一段内存
 * 只分配不负责释放。当不够空间分配时返回nullptr
*/
template <size_t A = sizeof(void*)>
class SerialAllocatorImpl
{
public:
    static constexpr size_t AlignBytes = A;

public:
    SerialAllocatorImpl(void* pool, size_t cap_size)
        : pool_((int8_t*)pool), capacity_(cap_size)
    { }

    SerialAllocatorImpl(SerialAllocatorImpl&& other)
    {
        MoveFrom(other);
    }

    ~SerialAllocatorImpl()
    {
    }

    SerialAllocatorImpl& operator = (SerialAllocatorImpl&& other)
    {
        MoveFrom(other);
        return *this;
    }

public:
    inline bool Empty() const { return alloced_ == 0; }
    inline size_t Size() const { return alloced_; }
    inline size_t Capacity() const { return capacity_; }
    inline void Reset() { alloced_ = 0; }

    inline void* Alloc(size_t sz)
    {
        size_t al_sz = KGAlignSize(sz, AlignBytes);
        if (al_sz > capacity_ - alloced_)
            return nullptr;

        int8_t* buf = pool_ + alloced_;
        alloced_ += al_sz;
        return buf;
    }

    void Dealloc(void*, size_t)
    {
        // not support
    }

    void Swap(SerialAllocatorImpl& other)
    {
        std::swap(other.alloced_, alloced_);
        std::swap(other.capacity_, capacity_);
        std::swap(other.pool_, pool_);
    }

protected:
    void MoveFrom(SerialAllocatorImpl& other)
    {
        alloced_ = other.alloced_;
        capacity_ = other.capacity_;
        pool_ = other.pool_;

        other.alloced_ = 0;
        other.capacity_ = 0;
        other.pool_ = nullptr;
    }

protected:
    size_t alloced_ = 0;
    size_t capacity_;
    int8_t* pool_;
}; // class SerialAllocatorImpl

/* serial allocator
 * only alloc memory but do not dealloc
*/
template <size_t A = sizeof(void*)>
class SerialAllocator
{
public:
    typedef SerialAllocator<A> MyType;
    typedef SerialAllocatorImpl<A> AllocImpl;
    typedef SinglyNode<AllocImpl> AllocNode;

    static constexpr size_t AlignBytes = AllocImpl::AlignBytes;
    static constexpr size_t DefaultBlock = 4096;

public:
    SerialAllocator()
        : block_(DefaultBlock)
        , alloc_node_(nullptr, 0)
    {
    }

    SerialAllocator(size_t block)
        : block_(block)
        , alloc_node_(nullptr, 0)
    {
    }

    /* build allocator with default pool, the default pool will not free */
    SerialAllocator(void* defaut_pool, size_t default_size, size_t block = DefaultBlock)
        : block_(block)
        , alloc_node_(defaut_pool, default_size)
    {
    }

    SerialAllocator(SerialAllocator&& other)
    {
        MoveFrom(other);
    }

    ~SerialAllocator()
    {
        Dissolve();
    }

    SerialAllocator& operator = (SerialAllocator&& other)
    {
        Dissolve();
        MoveFrom(other);
    }

public:
    void* Alloc(size_t size)
    {
        void* buff = alloc_node_.Value.Alloc(size);
        if (buff)
            return buff;

        CreateNode(std::max(KGAlignSize(size, AlignBytes), block_));

        buff = alloc_node_.Value.Alloc(size);
        assert(buff);
        return buff;
    }

    void Dealloc(void*, size_t)
    {
        // not support
    }

    /* Reset the allocator without free alloc node */
    void Reset()
    {
        AllocNode* cur = nullptr;
        AllocNode* node = alloc_node_.pNext;

        while (node)
        {
            cur = node;
            node = node->pNext;

            cur->pNext = empty_node_;
            cur->Value.Reset();
            empty_node_ = cur;
        }

        alloc_node_.pNext = nullptr;
        alloc_node_.Value.Reset();

        if (cur)
            alloc_node_.Value.Swap(cur->Value);
    }

    /* Reset the allocator and free alloc node */
    void Dissolve()
    {
        AllocNode* node = alloc_node_.pNext;
        alloc_node_.pNext = nullptr;
        while (node)
        {
            AllocNode* cur = node;
            node = node->pNext;

            if (node == nullptr)
                alloc_node_.Value.Swap(cur->Value);

            cur->~AllocNode();
            delete reinterpret_cast<int8_t*>(cur);
        }

        while (empty_node_)
        {
            AllocNode* pCur = empty_node_;
            empty_node_ = empty_node_->pNext;

            pCur->~AllocNode();
            delete reinterpret_cast<int8_t*>(pCur);
        }
    }

    void Swap(MyType& other)
    {
        std::swap(block_, other.block_);
        std::swap(empty_node_, other.empty_node_);
        std::swap(alloc_node_.pNext, other.alloc_node_.pNext);
        alloc_node_.Value.Swap(other.alloc_node_.Value);
    }

    template <typename Ty>
    inline AllocatorAdapter<Ty, MyType> GetAdapter()
    {
        return AllocatorAdapter<Ty, MyType>(this);
    }

private:
    void CreateNode(size_t size)
    {
        AllocNode* node = FindSuitableNode(size);
        if (node == nullptr)
        {
            constexpr size_t NodeObjSize = KGAlignSize(sizeof(AllocNode), AlignBytes);
            int8_t* mem = new int8_t[NodeObjSize + size];
            assert(mem);

            node = new (mem) AllocNode(mem + NodeObjSize, size);
        }

        alloc_node_.Value.Swap(node->Value);
        node->pNext = alloc_node_.pNext;
        alloc_node_.pNext = node;
    }

    AllocNode* FindSuitableNode(size_t size)
    {
        AllocNode* prev = nullptr;
        AllocNode* node = empty_node_;

        while (node)
        {
            if (node->Value.Capacity() < size)
            {
                prev = node;
                node = node->pNext;
            }
            else
            {
                if (prev)
                    prev->pNext = node->pNext;
                else
                    empty_node_ = node->pNext;
                break;
            }
        }

        return node;
    }

    void MoveFrom(MyType& other)
    {
        block_ = other.block_;
        empty_node_ = other.empty_node_;
        alloc_node_.pNext = other.alloc_node_.pNext;
        alloc_node_.Value.Swap(other.alloc_node_.Value);

        other.empty_node_ = nullptr;
        other.alloc_node_.pNext = nullptr;
    }

private:
    size_t      block_;
    AllocNode*  empty_node_ = nullptr;
    AllocNode   alloc_node_;
};

/* serial allocator with an default memory pool */
template <size_t N, size_t A = sizeof(void*)>
class PoolSerialAlloc
{
public:
    PoolSerialAlloc() : alloc_(pool_, N, N)
    {
    }

    PoolSerialAlloc(size_t block) : alloc_(pool_, N, block)
    {
    }

    ~PoolSerialAlloc()
    {
    }

    PoolSerialAlloc(const PoolSerialAlloc&) = delete;
    PoolSerialAlloc& operator = (const PoolSerialAlloc&) = delete;

public:
    inline void* Alloc(size_t s)
    {
        return alloc_.Alloc(s);
    }

    inline void Dealloc(void* p, size_t s)
    {
        alloc_.Dealloc(p, s);
    }

    /* Reset the allocator without free alloc node */
    inline void Reset()
    {
        alloc_.Reset();
    }

    /* Reset the allocator and free alloc node */
    inline void Dissolve()
    {
        alloc_.Dissolve();
    }

    inline SerialAllocator<A>* GetAlloc()
    {
        return &alloc_;
    }

    template <typename Ty>
    inline AllocatorAdapter<Ty, SerialAllocator<A>> GetAdapter()
    {
        return alloc_.template GetAdapter<Ty>();
    }

private:
    SerialAllocator<A> alloc_;
    alignas (A) int8_t pool_[N];
};

UTILITY_NAMESPACE_END
