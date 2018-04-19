/*
 * scoped buffer
 * xuantao, 2017
*/
#pragma once

#include "iscoped_deallocator.h"

UTILITY_NAMESPACE_BEGIN

/*
 * scoped buffer, 作用域范围内有效的Buffer
 * 对象用来管理和控制缓存的生命期
 * 不可以被复制/持有
*/
class scoped_buffer
{
public:
    scoped_buffer(scoped_buffer&& other)
    {
        capture(std::forward<scoped_buffer>(other));
    }

    scoped_buffer(iscoped_deallocator* dealloc, void* buffer, size_t size)
        : _deallocator(dealloc), _buffer(buffer), _size(size)
    {
    }

    ~scoped_buffer()
    {
        if (_deallocator)
            _deallocator->deallocate(_buffer, _size);
    }

protected:
    scoped_buffer(const scoped_buffer& other) = delete;
    scoped_buffer& operator = (const scoped_buffer&) = delete;

public:
    inline void* get() const { return _buffer; }
    inline size_t size() const { return _size; }
    inline bool empty() const { return get() == nullptr; }

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
    iscoped_deallocator* _deallocator;
    void* _buffer;
    size_t _size;
};

UTILITY_NAMESPACE_END
