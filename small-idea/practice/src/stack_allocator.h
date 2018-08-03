/*
 * scoped buffer allocator
 * xuantao, 2017
*/
#pragma once

#include "scoped_buffer.h"
#include "serial_allocator.h"

UTILITY_NAMESPACE_BEGIN

template <size_t A = sizeof(void*)>
class stack_allocator : private serial_allocator<A>
{
public:
    typedef serial_allocator<A> base_type;
    using base_type::align_byte;

public:
    stack_allocator(void* pool, size_t cap_size) : base_type(pool, cap_size)
    { }

public:
    inline scoped_buffer allocate(size_t sz)
    {
        return scoped_buffer(&_dealloc, base_type::allocate(sz), sz);
    }

    inline bool empty() const { return base_type::empty(); }
    inline size_t size() const { return base_type::size(); }
    inline size_t capacity() const { return base_type::capacity(); }

protected:
    void deallocate(void* buffer, size_t sz)
    {
        size_t align_size = UTILITY_NAMESPCE align(sz, align_byte);
        int8_t* addres = (int8_t*)buffer;

        assert((addres >= _pool && (addres - _pool) <= align_byte) && "not allocate from this object");
        assert((addres - _pool == _alloced - align_size) && "deallocate order not as allocated");

        _alloced -= align_size;
    }

    struct deallocator : public iscoped_deallocator
    {
        deallocator(stack_allocator* allocator) : _allocator(allocator) {}
        void deallocate(void* buf, size_t sz) override { _allocator->deallocate(buf, sz); }

        stack_allocator* _allocator;
    };

protected:
    deallocator _dealloc = { this };
}; // class stack_allocator

template <size_t N, size_t A = sizeof(void*)>
class fixed_stack_allocator : public stack_allocator<A>
{
public:
    typedef stack_allocator<A> base_type;

public:
    fixed_stack_allocator() : base_type(_pool, N)
    { }

private:
    int8_t _pool[N];
}; // class fixed_stack_allocator


template <size_t B, size_t A = sizeof(void*)>
class chain_stack_allocator
{
public:
    static constexpr size_t block_size = B;
    static constexpr size_t align_byte = A;
    typedef singly_node<fixed_stack_allocator<B, A>> alloc_node;
    static_assert(align_byte != 0 && (block_size % align_byte) == 0, "block size must be aligned");

protected:
    struct heap_deallocator : iscoped_deallocator
    {
        void deallocate(void* buf, size_t s) override { delete [] (int8_t*)buf; }
    };


};
UTILITY_NAMESPACE_END
