/*
 * stack allocator
 * xuantao, 2017
*/
#pragma once

#include "scope_buffer.h"
#include "serial_allocator.h"

UTILITY_NAMESPACE_BEGIN

/* stack allocator (first allocate buffer last deallocate, like a stack structure) */
template <size_t A = sizeof(void*)>
class stack_allocator : private SerialAllocatorImpl<A>
{
public:
    typedef SerialAllocatorImpl<A> base_type;
    using base_type::AlignBytes;

public:
    stack_allocator(void* pool, size_t cap_size) : base_type(pool, cap_size)
    { }

public:
    inline scoped_buffer allocate(size_t sz)
    {
        return scoped_buffer(&_dealloc, base_type::Alloc(sz), sz);
    }

    inline bool empty() const { return base_type::Empty(); }
    inline size_t size() const { return base_type::Size(); }
    inline size_t capacity() const { return base_type::Capacity(); }

protected:
    using base_type::pool_;
    using base_type::alloced_;

    void deallocate(void* buffer, size_t sz)
    {
        int8_t* addr = (int8_t*)buffer;
        size_t al_sz = UTILITY_NAMESPCE align_size(sz, AlignBytes);

        assert(addr >= pool_ && "not allocate from this object");
        assert((addr - pool_ == alloced_ - al_sz) && "deallocate order not as allocated");

        alloced_ -= al_sz;
    }

    struct deallocator : public iscope_deallocator
    {
        deallocator(stack_allocator* allocator) : _allocator(allocator) {}
        void deallocate(void* buf, size_t sz) override { _allocator->deallocate(buf, sz); }

        stack_allocator* _allocator;
    };

protected:
    deallocator _dealloc = { this };
}; // class stack_allocator


/* fixed stack allocator */
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


/* chain stack allocator */
template <size_t B, size_t A = sizeof(void*)>
class chain_stack_allocator
{
public:
    static constexpr size_t block_size = align_size(B, A);
    static constexpr size_t align_byte = A;
    typedef singly_node<fixed_stack_allocator<block_size, align_byte>> alloc_node;

public:
    chain_stack_allocator() : _top(&_bottom) { }
    ~chain_stack_allocator() { destory(); }

    chain_stack_allocator(const chain_stack_allocator&) = delete;
    chain_stack_allocator& operator = (const chain_stack_allocator&) = delete;

public:
    scoped_buffer allocate(size_t s)
    {
        if (s > block_size)
            return scoped_buffer();

        // pop empty alloc node
        while (_top != &_bottom && _top->value.empty())
        {
            alloc_node* node = _top;
            _top = _top->next;

            node->next = _empty;
            _empty = node;
        }

        auto buf = _top->value.allocate(s);
        if (!buf)
        {
            if (_empty)
            {
                alloc_node* node = _empty;
                _empty = _empty->next;

                node->next = _top;
                _top = node;
            }
            else
            {
                alloc_node* node = new alloc_node();
                node->next = _top;
                _top = node;
            }

            return _top->value.allocate(s);
        }

        return buf;
    }

private:
    void destory()
    {
        while (_empty)
        {
            alloc_node* node = _empty;
            _empty = _empty->next;
            delete node;
        }

        while (_top->next)
        {
            alloc_node* node = _top;
            _top = _top->next;
            delete node;
        }
    }

private:
    alloc_node* _empty = nullptr;
    alloc_node* _top;
    alloc_node _bottom;
};

UTILITY_NAMESPACE_END
