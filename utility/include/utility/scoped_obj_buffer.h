﻿/*
 * scoped objects buffer
 * xuantao, 2017
*/
#pragma once

#include "scoped_buffer.h"

UTILITY_NAMESPACE_BEGIN
/*
 * 用scoped_buffer构造的Obj数组
*/
template <class Ty>
class scoped_obj_buffer : private scoped_buffer
{
public:
    typedef std::ptrdiff_t difference_type;

public:
    scoped_obj_buffer(scoped_obj_buffer&& other)
        : scoped_buffer(std::forward<scoped_buffer>(other))
    {
    }

    scoped_obj_buffer(scoped_buffer&& buffer, size_t count)
        : scoped_buffer(std::forward<scoped_buffer>(buffer))
    {
        construct();
    }

    template <typename ...Args>
    scoped_obj_buffer(scoped_buffer&& buffer, size_t count, Args&& ...args)
        : scoped_buffer(std::forward<scoped_buffer>(buffer))
    {
        construct(std::forward<Args>(args)...);
    }

    ~scoped_obj_buffer()
    {
        destruct();
    }

    scoped_obj_buffer(const scoped_obj_buffer&) = delete;
    scoped_obj_buffer& operator = (const scoped_obj_buffer&) = delete;

public:
    inline Ty* get() const { return (Ty*)scoped_buffer::get(); }
    inline size_t count() const { return scoped_buffer::size() / sizeof(Ty); }
    inline bool empty() const { return count() == 0; }

    const Ty& operator [] (difference_type idx) const
    {
        return const_cast<scoped_obj_buffer*>(this)->operator[] (idx);
    }

    Ty& operator [] (difference_type idx)
    {
        assert(idx > 0 && count() > (size_t)idx);
        return get()[idx];
    }

protected:
    void construct()
    {
        size_t c = count();
        for (size_t i = 0; i < c; ++i)
            new (get() + i) Ty();
    }

    template <typename ...Args>
    void construct(Args&& ...args)
    {
        size_t c = count();
        for (size_t i = 0; i < c; ++i)
            new (get() + i) Ty(std::forward<Args>(args)...);
    }

    void destruct()
    {
        size_t idx = count();
        while (idx)
            (get()[--idx]).~Ty();
    }
};
UTILITY_NAMESPACE_END