#pragma once
#include "common.h"

UTILITY_NAMESPACE_BEGIN

/* 循环缓冲区 */
template <size_t A = sizeof(void*)>
class ring_buffer
{
public:
    static constexpr size_t align_byte = A;
    static constexpr size_t size_bytes = sizeof(size_t);

public:
    ring_buffer(void* pool, size_t cap_size) : _pool((int8_t*)pool), _capacity(cap_size)
    { }

    ring_buffer(const ring_buffer&) = delete;
    ring_buffer& operator = (const ring_buffer&) = delete;

public:
    inline bool empty() const { return _count == 0; }
    inline size_t count() const { return _count; }
    inline size_t capacity() const { return _capacity; }

    void* read_begin(size_t* buff_size)
    {
        if (empty())
            return nullptr;

        size_t sz = *((size_t*)(_pool + _r_pos));
        if (_capacity - _r_pos < size_bytes || sz == 0)
        {
            _r_pos = 0; // jump to front
            sz = *((size_t*)_pool);
        }

        if (buff_size)
            *buff_size = sz;
        return _pool + _r_pos + size_bytes;
    }

    void read_end()
    {
        size_t sz = *((size_t*)(_pool + _r_pos));
        _r_pos += AlignSize(sz + size_bytes, align_byte);
        -- _count;
    }

    void* write_begin(size_t sz)
    {
        size_t rp = _r_pos;
        if (_w_pos == rp && !empty())
            return nullptr;  // full

        size_t cap = AlignSize(sz + size_bytes);
        if (_w_pos < rp)
        {
            if ((rp - _w_pos) > cap) return nullptr;
        }
        else if (_capacity - _w_pos < cap)
        {
            if (rp < cap) return nullptr;

            if (_capacity - _w_pos >= size_bytes)
                *((size_t*)(_pool + _w_pos)) = 0;   // mark
            _w_pos = 0; // jump to front
        }

        int8_t* data = _pool + _w_pos;
        *((size_t*)data) = sz;
        data += size_bytes;
        return data;
    }

    void write_end()
    {
        size_t sz = *((size_t*)(_pool + _w_pos));
        _w_pos += AlignSize(sz + size_bytes, align_byte);
        ++ _count;
    }

    void reset()
    {
        _w_pos = 0;
        _r_pos = 0;
        _count = 0;
    }

private:
    size_t _w_pos = 0;
    size_t _r_pos = 0;
    size_t _count = 0;
    size_t _capacity;
    int8_t* _pool;
}; // class ring_buffer

/* fixed ring buffer */
template <size_t N, size_t A = sizeof(void*)>
class fixed_ring_buffer : public ring_buffer<A>
{
public:
    fixed_ring_buffer() : ring_buffer<A>(_mem, N)
    { }

private:
    int8_t _mem[N];
}; // class fixed_ring_buffer

UTILITY_NAMESPACE_END
