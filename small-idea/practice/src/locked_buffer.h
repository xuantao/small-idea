#pragma once
#include "common.h"

UTILITY_NAMESPACE_BEGIN

struct ibuffer_unlocker
{
    virtual void unlock(void* buf, size_t sz) = 0;
};

class locked_buffer
{
    locked_buffer(const locked_buffer& other) = delete;
    locked_buffer& operator = (const locked_buffer& other) = delete;

public:
    locked_buffer() : _unlocker(nullptr), _buff(nullptr) { }

    locked_buffer(ibuffer_unlocker* unlocker, void* buf, size_t sz)
        : _allocator(allocator)
        , _buff(buf)
        , _size(sz)
    { }

    locked_buffer(locked_buffer&& other)
        : _unlocker(other._unlocker)
        , _buff(other._buff)
        , _size(other._size)
    {
        other._unlocker = nullptr;
    }

    ~locked_buffer()
    {
        if (_unlocker)
            _unlocker->unlock(_buff, _size);
    }

public:
    inline void* get() const { return _buff; }
    inline size_t size() const { return _size; }
    inline explicit operator bool() const { return _buff != nullptr; }

private:
    ibuffer_unlocker* _unlocker;
    void* _buff;
    size_t _size;
};

UTILITY_NAMESPACE_END
