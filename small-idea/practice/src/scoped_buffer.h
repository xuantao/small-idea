/*
 * scoped buffer
 * xuantao, 2017
*/
#pragma once

#include <type_traits>
#include "deallocator.h"

NAMESPACE_ZH_BEGIN

class scoped_buffer
{
public:
    scoped_buffer(scoped_buffer&& other)
    {
        capture(std::forward<scoped_buffer>(other));
    }

    scoped_buffer(deallocator* dealloc, void* buffer, size_t size)
        : _deallocator(dealloc), _buffer(buffer), _size(size)
    {
    }

    ~scoped_buffer()
    {
        if (_deallocator)
            _deallocator->deallocate(_buffer, _size);
    }

protected:
    scoped_buffer(const scoped_buffer& other);
    scoped_buffer& operator = (const scoped_buffer&);

public:
    inline void* get() const { return _buffer; }
    inline size_t size() const { return _size; }

protected:
    void capture(scoped_buffer&& other)
    {
        _deallocator = other._deallocator;
        _buffer = other._buffer;
        _size = other._size;

        other._deallocator = nullptr;
        other._buffer = nullptr;
        other._size = 0;
    }

protected:
    deallocator* _deallocator;
    void* _buffer;
    size_t _size;
};

NAMESPACE_ZH_END
