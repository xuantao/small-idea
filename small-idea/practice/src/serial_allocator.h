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

UTILITY_NAMESPACE_END
