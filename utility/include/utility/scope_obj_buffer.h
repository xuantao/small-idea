/*
 * scoped objects buffer
 * xuantao, 2017
*/
#pragma once

#include "scope_buffer.h"

UTILITY_NAMESPACE_BEGIN
/*
 * 用scoped_buffer构造的Obj数组
*/
template <class Ty>
class scope_obj_buffer : private ScopeBuffer
{
public:
    typedef std::ptrdiff_t difference_type;

public:
    scope_obj_buffer(scope_obj_buffer&& other)
        : ScopeBuffer(std::forward<ScopeBuffer>(other))
    {
    }

    scope_obj_buffer(ScopeBuffer&& buffer, size_t count)
        : ScopeBuffer(std::forward<ScopeBuffer>(buffer))
    {
        construct();
    }

    template <typename ...Args>
    scope_obj_buffer(ScopeBuffer&& buffer, size_t count, Args&& ...args)
        : ScopeBuffer(std::forward<ScopeBuffer>(buffer))
    {
        construct(std::forward<Args>(args)...);
    }

    ~scope_obj_buffer()
    {
        destruct();
    }

    scope_obj_buffer(const scope_obj_buffer&) = delete;
    scope_obj_buffer& operator = (const scope_obj_buffer&) = delete;

public:
    inline Ty* get() const { return (Ty*)ScopeBuffer::get(); }
    inline size_t count() const { return ScopeBuffer::size() / sizeof(Ty); }
    inline bool empty() const { return count() == 0; }

    const Ty& operator [] (difference_type idx) const
    {
        return const_cast<scope_obj_buffer*>(this)->operator[] (idx);
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
