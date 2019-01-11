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
class SerialAllocator : private Alloc_Internal::AllocCtrlChain<Alloc_Internal::SerialAllocCtrl<A>>
{
public:
    using AllocCtrl = Alloc_Internal::SerialAllocCtrl<A>;
    using BaseType = Alloc_Internal::AllocCtrlChain<AllocCtrl>;
    static constexpr size_t DefaultBlock = 4096;

public:
    SerialAllocator() : BaseType(DefaultBlock) { }
    SerialAllocator(size_t block) : BaseType(block) { }

    /* build allocator with default pool, the default pool will not free */
    SerialAllocator(void* defaut_pool, size_t default_size, size_t block = DefaultBlock)
        : BaseType(defaut_pool, default_size, block)
    {
        assert(block >= A);
    }

    SerialAllocator(SerialAllocator&& other)
    {
        Swap(other);
    }

    ~SerialAllocator() { }

    inline SerialAllocator& operator = (SerialAllocator&& other)
    {
        this->Dissolve();
        Swap(other);
    }

public:
    inline void* Alloc(size_t size) { return BaseType::Alloc(size); }
    inline void Dealloc(void*, size_t) { /* not support */ }

    /* Reset the allocator without free alloc node */
    inline void Reset() { BaseType::Reset(); }
    /* Reset the allocator and free alloc node */
    inline void Dissolve() { BaseType::Dissolve(); }

    inline void Swap(SerialAllocator& other) { BaseType::Swap(other); }

    template <typename Ty>
    inline AllocatorAdapter<Ty, SerialAllocator<A>> GetAdapter()
    {
        return AllocatorAdapter<Ty, SerialAllocator<A>>(this);
    }

    template <typename Ty, typename... Args>
    inline Ty* Construct(Args&&... args)
    {
        void* mem = Alloc(sizeof(Ty));
        assert(mem);
        return new (mem) Ty(std::forward<Args>(args)...);
    }

    template <typename Ty>
    inline void Destruct(Ty* obj) { obj->~Ty(); }
};

/* serial allocator with an default memory pool */
template <size_t N, size_t A = sizeof(void*)>
class PooledSerialAlloc : public SerialAllocator<A>
{
public:
    PooledSerialAlloc() : SerialAllocator<A>(pool_, N, N) { }
    PooledSerialAlloc(size_t block) : SerialAllocator<A>(pool_, N, block) { }

    ~PooledSerialAlloc() = default;

    PooledSerialAlloc(const PooledSerialAlloc&) = delete;
    PooledSerialAlloc& operator = (const PooledSerialAlloc&) = delete;

private:
    alignas (A) int8_t pool_[N];
};

template <size_t A/* = sizeof(void*)*/>
class PooledSerialAlloc<0, A> : public SerialAllocator<A>
{
public:
    PooledSerialAlloc() : SerialAllocator<A>(SerialAllocator<A>::DefaultBlock) { }
    PooledSerialAlloc(size_t block) : SerialAllocator<A>(block) { }

    ~PooledSerialAlloc() = default;
};

/* stack memory allocator
 * the memory is in stack
 * only alloc but not dealloc, when pool is empty alloc will retur nullptr
*/
template <size_t N, size_t A = sizeof(void*)>
class StackMemoryAllocator
{
    using AllocCtrl = Alloc_Internal::SerialAllocCtrl<A>;
public:
    StackMemoryAllocator() : alloc_(pool_, N) { }

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
    alignas (A) int8_t pool_[N];
}; // class fixed_stack_allocator

UTILITY_NAMESPACE_END
