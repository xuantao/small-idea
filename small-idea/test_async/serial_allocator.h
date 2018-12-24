#pragma once
#include "common.h"

UTILITY_NAMESPACE_BEGIN

/*
 * 序列式分配器, 顺序从一块缓存中分配一段内存
 * 只分配不负责释放。当不够空间分配时返回nullptr
*/
template <size_t A = sizeof(void*)>
class serial_allocator
{
public:
    static constexpr size_t align_byte = A;

public:
    serial_allocator(void* pool, size_t cap_size)
        : _pool((int8_t*)pool), _capacity(cap_size)
    { }

    serial_allocator(const serial_allocator&) = delete;
    serial_allocator& operator = (const serial_allocator&) = delete;

public:
    inline bool empty() const { return _alloced == 0; }
    inline size_t size() const { return _alloced; }
    inline size_t capacity() const { return _capacity; }
    inline void reset() { _alloced = 0; }

    inline void* allocate(size_t sz)
    {
        size_t al_sz = UTILITY_NAMESPCE align_size(sz, align_byte);
        if (al_sz > _capacity - _alloced)
            return nullptr;

        int8_t* buf = _pool + _alloced;
        _alloced += al_sz;
        return buf;
    }

    void swap(serial_allocator& other)
    {
        std::swap(other._alloced, _alloced);
        std::swap(other._capacity, _capacity);
        std::swap(other._pool, _pool);
    }

protected:
    size_t _alloced = 0;
    size_t _capacity;
    int8_t* _pool;
}; // class serial_allocator

/* fixed serial allocator */
template <size_t N, size_t A = sizeof(void*)>
class fixed_serial_allocator : public serial_allocator<A>
{
public:
    typedef serial_allocator<A> base_type;

public:
    fixed_serial_allocator() : base_type(_pool, N)
    { }

private:
    alignas (A) int8_t _pool[N];
}; // class fixed_serial_allocator

/* chain serial allocator */
template <size_t B, size_t A = sizeof(void*)>
class chain_serial_allocator
{
public:
    static constexpr size_t block_size = align_size(B, A);
    static constexpr size_t align_byte = A;
    typedef singly_node<fixed_serial_allocator<block_size, align_byte>> alloc_node;

public:
    chain_serial_allocator() : _cur(&_head) { }
    ~chain_serial_allocator() { reset(true); }

    chain_serial_allocator(const chain_serial_allocator&) = delete;
    chain_serial_allocator& operator = (const chain_serial_allocator&) = delete;

public:
    void* allocate(size_t s)
    {
        if (s > block_size)
            return nullptr;

        void* buf = _cur->value.allocate(s);
        if (buf == nullptr)
        {
            if (_cur->next == nullptr)
                _cur->next = new alloc_node();
            _cur = _cur->next;

            buf = _cur->value.allocate(s);
            assert(buf);
        }

        return buf;
    }

    inline void reset() { reset(false); }

    void reset(bool dissolve)
    {
        _cur = &_head;

        if (dissolve)
        {
            alloc_node* node = _cur->next;
            _cur->value.reset();
            _cur->next = nullptr;

            while (node)
            {
                alloc_node* tmp = node;
                node = node->next;
                delete tmp; // remove all extra node
            }
        }
        else
        {
            alloc_node* node = _cur;
            while (node)
            {
                node->value.reset();
                node = node->next;
            }
        }
    }

    size_t node_size() const
    {
        size_t ns = 0;
        const alloc_node* node = &_head;

        while (node)
        {
            node = node->next;
            ++ns;
        }
        return ns;
    }

protected:
    alloc_node* _cur;
    alloc_node _head;
}; // chain_serial_allocator

UTILITY_NAMESPACE_END
