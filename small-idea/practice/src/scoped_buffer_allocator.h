/*
 * scoped buffer allocator
 * xuantao, 2017
*/
#pragma once

#include <cassert>
#include "scoped_obj_buffer.h"

NAMESPACE_ZH_BEGIN

template <size_t N, size_t A>
class scoped_buffer_allocator
{
    static const uint8_t _CHECK_VALUE = 0xAA;
public:
    static const size_t capacity_size = N;
    static const size_t align_size = A;
protected:
    class adaptor : public allocator_base
    {
    public:
        adaptor(scoped_buffer_allocator& allocator) : _allocator(allocator) {}
    public:
        void* allocate(size_t size) { return nullptr; }
        void deallocate(void* buffer, size_t size) { _allocator.deallocate(buffer, size); }
    protected:
        scoped_buffer_allocator& _allocator;
    };

public:
    scoped_buffer_allocator()
        : _alloced(0), _adaptor(*this)
    {
    }

    scoped_buffer_allocator(scoped_buffer_allocator&& other)
        : _adaptor(0), _adaptor(*this)
    {
    }

protected:
    scoped_buffer_allocator(const scoped_buffer_allocator&);
    scoped_buffer_allocator& operator = (const scoped_buffer_allocator&);

public:
    scoped_buffer allocate(size_t size)
    {
        uint8_t* buffer = nullptr;
        size_t align_size = align(size);

#ifdef _DEBUG
        if (size && align_size == size)
            align_size += A;
#endif // _DEBUG

        if (align_size > N - _alloced)
            return scoped_buffer(&_adaptor, new uint8_t[size], size);

        buffer = &_pool[_alloced];

#ifdef _DEBUG
        write_bytes(buffer + size, align_size - size);
#endif // _DEBUG

        _alloced += align_size;

        return scoped_buffer(&_adaptor, buffer, size);
    }

    template<class Ty>
    scoped_obj_buffer<Ty> allocate(size_t count)
    {
        return scoped_obj_buffer<Ty>(allocate(sizeof(Ty) * count), count);
    }

    template<class Ty, typename ...Args>
    scoped_obj_buffer<Ty> allocate(size_t count, Args&& ...args)
    {
        return scoped_obj_buffer<Ty>(allocate(sizeof(Ty) * count), count, args...);
    }

    size_t size() const
    {
        return capacity_size - _alloced;
    }

    size_t capacity() const
    {
        return N;
    }
protected:
    void deallocate(void* buffer, size_t size)
    {
        size_t align_size = align(size);
        uint8_t* addres = (uint8_t*)buffer;
#ifdef _DEBUG
        if (size && align_size == size)
            align_size += A;
#endif // _DEBUG

        if (addres < _pool || (addres - _pool) > N)
        {
            delete buffer;
        }
        else
        {
            assert(addres - _pool == _alloced - align_size);
            _alloced -= align_size;
#ifdef _DEBUG
            check_bytes(&_pool[_alloced] + size, align_size - size);
#endif // _DEBUG
        }
    }

private:
#ifdef _DEBUG
    /*
    * 通过在每段Buff的后面写入一些特定字节, 当释放Buff的时候判断是否被修改来检测是否被写越界了.
    */
#define _CHECK_VALUE 0xAA
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
        assert(over == 0);
    }
#endif
    size_t align(size_t size)
    {
        size_t rem = size % A;
        if (rem) size += A - rem;
        return size;
    }

protected:
    uint8_t _pool[N];
    size_t _alloced;
    adaptor _adaptor;
};

NAMESPACE_ZH_END
