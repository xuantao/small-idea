/*
 * scoped buffer
 * xuantao, 2017
*/
#pragma once
#include "common.h"

UTILITY_NAMESPACE_BEGIN

struct iscope_deallocator
{
    virtual ~iscope_deallocator() { }
    virtual void deallocate(void* buff, size_t size) = 0;
};

/*
 * scoped buffer, 作用域范围内有效的Buffer
 * 对象用来管理和控制缓存的生命期
 * 不可以被复制/持有
*/
class scoped_buffer
{
public:
    scoped_buffer() = default;

    scoped_buffer(iscope_deallocator* dealloc, void* buffer, size_t sz)
        : _deallocator(dealloc), _buff(buffer), _size(sz)
    {
    }

    scoped_buffer(scoped_buffer&& other)
        : _deallocator(other._deallocator)
        , _buff(other._buff)
        , _size(other._size)
    {
        other._deallocator = nullptr;
        other._buff = nullptr;
        other._size = 0;
    }

    ~scoped_buffer()
    {
        if (_deallocator && _buff)
            _deallocator->deallocate(_buff, _size);
    }

    scoped_buffer(const scoped_buffer& other) = delete;
    scoped_buffer& operator = (const scoped_buffer&) = delete;

public:
    inline void* get() const { return _buff; }
    inline size_t size() const { return _size; }
    inline bool empty() const { return get() == nullptr; }
    inline explicit operator bool() const { return _buff != nullptr; }

    template <typename Ty>
    inline Ty& as() { *((Ty*)_buff); }

protected:
    iscope_deallocator* _deallocator = nullptr;
    void* _buff = nullptr;
    size_t _size = 0;
};

UTILITY_NAMESPACE_END
