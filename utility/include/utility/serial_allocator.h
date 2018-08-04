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

    void* allocate(size_t sz)
    {
        size_t align_size = UTILITY_NAMESPCE align(sz, align_byte);
        if (align_size > _capacity - _alloced)
            return nullptr;

        int8_t* buf = _pool + _alloced;
        _alloced += align_size;

        return buf;
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
    int8_t _pool[N];
}; // class fixed_serial_allocator

/* chain serial allocator */
template <size_t B, size_t A = sizeof(void*)>
class chain_serial_allocator
{
public:
    static constexpr size_t block_size = align(B, A);
    static constexpr size_t align_byte = A;
    typedef singly_node<fixed_serial_allocator<block_size, align_byte>> alloc_node;
    static_assert(align_byte != 0 && (block_size % align_byte) == 0, "block size must be aligned");

public:
    chain_serial_allocator() : _cur(&_head) { }
    ~chain_serial_allocator() { dissolve(); }

    chain_serial_allocator(const chain_serial_allocator&) = delete;
    chain_serial_allocator& operator = (const chain_serial_allocator&) = delete;

public:
    inline void* allocate(size_t s)
    {
        if (s > block_size)
            return nullptr;

        void* buf = _cur->value.allocate(s);
        if (buf == nullptr)
        {
            if (_cur->next = nullptr)
                _cur->next = new alloc_node();
            _cur = _cur->next;

            buf = _cur->value.allocate(s);
            assert(buf);
        }

        return buf;
    }

    void reset()
    {
        _cur = &_head;

        alloc_node* node = _cur;
        while (node)
        {
            node->value.reset();
            node = node->next;
        }
    }

    void dissolve()
    {
        _cur = &_head;
        _cur->value.reset();

        alloc_node* node = _cur;
        while (node)
        {
            alloc_node* tmp = node;
            node = node->next;

            tmp->next = nullptr;
            delete tmp;
        }
    }

protected:
    alloc_node* _cur;
    alloc_node _head;
}; // chain_serial_allocator

UTILITY_NAMESPACE_END
