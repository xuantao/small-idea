#pragma once
#include <type_traits>
#include "common.h"
#include "allocator_adapter.h"
#include "detail/alloc_internal.h"

UTILITY_NAMESPACE_BEGIN

/* serial allocator
 * only alloc memory but do not dealloc
*/
template <size_t A = sizeof(void*)>
class SerialAllocator : Alloc_Internal::AllocCtrlChain<Alloc_Internal::SerialAllocCtrl<A>>
{
public:
    using AllocCtrl = Alloc_Internal::SerialAllocCtrl<A>;
    using BaseType = Alloc_Internal::AllocCtrlChain<AllocCtrl>;
    static constexpr size_t DefaultBlock = 4096;

public:
    SerialAllocator() : BaseType(DefaultBlock)
    {
    }

    SerialAllocator(size_t block) : BaseType(block)
    {
    }

    /* build allocator with default pool, the default pool will not free */
    SerialAllocator(void* defaut_pool, size_t default_size, size_t block = DefaultBlock)
        : BaseType(defaut_pool, default_size, block)
    {
    }

    SerialAllocator(SerialAllocator&& other)
    {
        Swap(other);
    }

    ~SerialAllocator()
    {
    }

    SerialAllocator& operator = (SerialAllocator&& other)
    {
        this->Dissolve();
        Swap(other);
    }

public:
    void* Alloc(size_t size)
    {
        return BaseType::Alloc(size);
    }

    void Dealloc(void*, size_t)
    {
        // not support
    }

    /* Reset the allocator without free alloc node */
    void Reset()
    {
        BaseType::Reset();
    }

    /* Reset the allocator and free alloc node */
    void Dissolve()
    {
        BaseType::Dissolve();
    }

    void Swap(SerialAllocator& other)
    {
        BaseType::Swap(other);
    }

    template <typename Ty>
    inline AllocatorAdapter<Ty, SerialAllocator<A>> GetAdapter()
    {
        return AllocatorAdapter<Ty, SerialAllocator<A>>(this);
    }
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

/* fixed stack memory allocator */
template <size_t N, size_t A = sizeof(void*)>
class StackMemoryAllocator
{
    using AllocCtrl = Alloc_Internal::SerialAllocCtrl<A>;
public:
    StackMemoryAllocator() : alloc_(pool_, N)
    { }

    StackMemoryAllocator(const StackMemoryAllocator&) = delete;
    StackMemoryAllocator& operator = (const StackMemoryAllocator&) = delete;

public:
    inline void* Alloc(size_t size) { return alloc_.Alloc(size); }
    inline void Dealloc(void*, size_t) { }

    inline bool Empty() const { return alloc_.Empty(); }
    inline size_t Size() const { return alloc_.Size(); }
    inline size_t Capacity() const { return alloc_.Capacity(); }
    inline void Reset() { alloc_.Reset(); }

    template <typename Ty>
    inline AllocatorAdapter<Ty, StackMemoryAllocator<N, A>> GetAdapter()
    {
        return AllocatorAdapter<Ty, StackMemoryAllocator<N, A>>(this);
    }

private:
    AllocCtrl alloc_;
    int8_t pool_[N];
}; // class fixed_stack_allocator

UTILITY_NAMESPACE_END
