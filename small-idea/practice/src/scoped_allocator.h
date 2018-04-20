﻿/*
 * scoped buffer allocator
 * xuantao, 2017
*/
#pragma once

#include "scoped_buffer.h"

UTILITY_NAMESPACE_BEGIN

template <size_t N, size_t A = sizeof(void*)>
class scoped_allocator
{
private:
    struct deallocator : public iscoped_deallocator
    {
        deallocator(scoped_allocator& allocator) : _allocator(allocator) {}
        void deallocate(void* buffer, size_t size) override { _allocator.deallocate(buffer, size); }

        scoped_allocator& _allocator;
    };

public:
    scoped_allocator()
        : _alloced(0), _dealloc(*this)
    {
        void* p = _pool;
        size_t s = N;
        std::align(A, 0, p, s);
        _alloced = N - s;   // align base size;
    }

    scoped_allocator(const scoped_allocator&) = delete;
    scoped_allocator& operator = (const scoped_allocator&) = delete;

public:
    scoped_buffer allocate(size_t size)
    {
        uint8_t* buffer = nullptr;
        size_t align_size = align(size);

#if SCOPED_ALLOCATOR_BOUNDARY_CHECK
        if (size && align_size == size)
            align_size += A;
#endif // SCOPED_ALLOCATOR_BOUNDARY_CHECK

        if (align_size > N - _alloced)
            return scoped_buffer(nullptr, nullptr, 0);

        buffer = _pool + _alloced;

#if SCOPED_ALLOCATOR_BOUNDARY_CHECK
        write_bytes(buffer + size, align_size - size);
#endif // SCOPED_ALLOCATOR_BOUNDARY_CHECK

        _alloced += align_size;

        return scoped_buffer(&_dealloc, buffer, size);
    }

    inline size_t size() const { return _alloced; }
    inline size_t capacity() const { return N; }

protected:
    void deallocate(void* buffer, size_t size)
    {
        size_t align_size = align(size);
        uint8_t* addres = (uint8_t*)buffer;

#if SCOPED_ALLOCATOR_BOUNDARY_CHECK
        if (size && align_size == size)
            align_size += A;
#endif // SCOPED_ALLOCATOR_BOUNDARY_CHECK

        assert((addres >= _pool && (addres - _pool) <= N) && "not allocate from this object");
        assert((addres - _pool == _alloced - align_size) && "deallocate order not as allocated");

        _alloced -= align_size;
#if SCOPED_ALLOCATOR_BOUNDARY_CHECK
        check_bytes(_pool + _alloced + size, align_size - size);
#endif // SCOPED_ALLOCATOR_BOUNDARY_CHECK
    }

private:
#if SCOPED_ALLOCATOR_BOUNDARY_CHECK
    static const uint8_t _CHECK_VALUE = 0xAA;
    /*
     * 通过在每段Buff的后面写入一些特定字节, 当释放Buff的时候判断是否被修改来检测是否被写越界了.
    */
    void write_bytes(uint8_t* buffer, size_t size)
    {
        for (size_t i = 0; i < size; ++i)
            buffer[i] = _CHECK_VALUE;
    }

    void check_bytes(uint8_t* buffer, size_t size)
    {
        size_t over = 0;
        for (size_t i = 0; i < size; ++i)
            over += (buffer[i] != _CHECK_VALUE) ? 1 : 0;
        // over write bytes
        assert(over == 0 && "buffer has been overrides");
    }
#endif // SCOPED_ALLOCATOR_BOUNDARY_CHECK

    size_t align(size_t size)
    {
        size_t rem = size % A;
        if (rem) size += A - rem;
        return size;
    }

protected:
    size_t _alloced;
    deallocator _dealloc;
    uint8_t _pool[N];
};

UTILITY_NAMESPACE_END