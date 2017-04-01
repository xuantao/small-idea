/*
 * scoped buffer
 * xuantao, 2017
*/

#include "allocator_base.h"

#pragma once

namespace zh
{
    class scoped_buffer
    {
    public:
        scoped_buffer(scoped_buffer&& other)
        {
            capture(other);
        }

        scoped_buffer(allocator_base* allocator, void* buffer, size_t size)
            : _allocator(allocator), _buffer(buffer), _size(size)
        {
        }

        ~scoped_buffer()
        {
            if (_allocator)
                _allocator->deallocate(_buffer, _size);
        }

    protected:
        /*
         * ‘ –Ì±ªºÃ≥–¿©’π
        */
        scoped_buffer(scoped_buffer& other)
        {
            capture(other);
        }

        scoped_buffer& operator = (const scoped_buffer&);

    public:
        inline void* get() const { return _buffer; }
        inline size_t size() const { return _size; }

    protected:
        void capture(scoped_buffer& other)
        {
            _allocator = other._allocator;
            _buffer = other._buffer;
            _size = other._size;

            other._allocator = nullptr;
            other._buffer = nullptr;
            other._size = 0;
        }

    protected:
        allocator_base* _allocator;
        void* _buffer;
        size_t _size;
    };
}
