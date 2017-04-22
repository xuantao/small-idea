/*
 * scoped vector, copy from std::vector
 * xuantao, 2017
*/
#pragma once

#include "scoped_buffer.h"
#include "detail/_vector.h"

NAMESPACE_ZH_BEGIN

template <class Ty>
class scoped_vector
{
public:
    typedef scoped_vector<Ty> _my_type;
    typedef Ty value_type;
    typedef Ty* pointer;
    typedef Ty& reference;
    typedef const Ty* const_pointer;
    typedef const Ty& const_reference;
    typedef size_t size_type;
    typedef std::ptrdiff_t difference_type;

    typedef detail::_vector_iterator<Ty> iterator;
    typedef detail::_vector_const_iterator<Ty> const_iterator;
    typedef typename iterator::_val_type _val_type;

public:
    static const size_type element_size = sizeof(Ty);
    static size_type buffer_size(size_type c) { return element_size * c; }

public:
    scoped_vector(scoped_buffer&& buffer)
        : _buff(std::forward<scoped_buffer>(buffer))
        , _val(static_cast<pointer>(_buff.get()))
    {
    }

    scoped_vector(_my_type&& other)
        : _buff(std::move(other._buff))
        , _val(std::move(other._val))
    {
    }

    ~scoped_vector()
    {
        difference_type idx = Next();
        while (idx-- > 0)
            destruct(&Base()[idx]);
        Next() = 0;
    }

protected:
    _my_type(const _my_type& other)
    {
        static_assert(false, "not allow copy");
    }

    _my_type& operator = (const _my_type& other)
    {
        static_assert(false, "not allow copy");
    }

public:
    reference operator [] (difference_type idx)
    {
        assert((size_type)idx < size() && idx >= 0);
        return Base()[idx];
    }

    const_reference operator [] (difference_type idx) const
    {
        return const_cast<scoped_vector*>(this)->operator [](idx);
    }

    reference front()
    {
        assert(!empty());
        return Base()[0];
    }

    const_reference front() const
    {
        return const_cast<scoped_vector*>(this)->front();
    }

    reference back()
    {
        assert(!empty());
        return Base()[Next() - 1];
    }

    const_reference back() const
    {
        return const_cast<scoped_vector*>(this)->back();
    }

    iterator begin()
    {
        return iterator(&_val, 0);
    }

    iterator end()
    {
        return iterator(&_val, Next());
    }

    const_iterator cbegin() const
    {
        return const_iterator(&_val, 0);
    }

    const_iterator cend() const
    {
        return const_iterator(&_val, _val._next);
    }

public:
    template <class _Ty>
    void push_back(_Ty&& val)
    {
        assert(capacity() > size() && "not any more space");

        construct(&Base()[Next()], std::forward<_Ty>(val));
        ++Next();
    }

    void pop_back()
    {
        assert(!empty() && "vector is empty");

        --Next();
        destruct(&Base()[Next()]);
    }

    template <class _Ty>
    iterator insert(const_iterator position, _Ty&& val)
    {
        return insert(position, 1, std::forward<_Ty>(val));
    }

    template <class _Ty>
    iterator insert(const_iterator position, size_type n, _Ty&& val)
    {
        difference_type idx = position - begin();
        assert(n <= capacity() - size() && "not any more space");
        assert(idx >= 0 && idx <= Next() && "invalid param in insert");

        // move
        for (difference_type i = Next() - idx - 1; i >= 0; --i)
        {
            construct(&Base()[idx + i], Base()[idx + n + i]);
            destruct(&Base()[idx + i]);
        }

        // copy construct
        for (size_type i = 0; i < n; ++i)
            construct(&Base()[idx + i], std::forward<_Ty>(val));

        Next() += (difference_type)n;
        return iterator(&_val, idx);
    }

    iterator erase(const_iterator position)
    {
        return erase(position, position + 1);
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        difference_type begIdx = first - begin();
        difference_type endIdx = last - begin();
        assert(begIdx <= Next() && "invalid param, first iterator is valid");
        assert(endIdx >= begIdx && endIdx <= Next() && "invalid param");

        // destruct
        for (difference_type i = begIdx; i < endIdx; i++)
            destruct(&Base()[i]);

        // move
        for (difference_type i = 0; i < Next() - endIdx; ++i)
        {
            construct(&Base()[begIdx + i], Base()[endIdx + i]);
            destruct(&Base()[endIdx + i]);
        }

        Next() -= (endIdx - begIdx);
        return iterator(&_val, begIdx);
    }

    bool empty() const
    {
        return _val.empty();
    }

    size_type size() const
    {
        return _val.size();
    }

    size_type capacity() const
    {
        return _buff.size() / sizeof(Ty);
    }

    size_type max_size() const
    {
        return capacity();
    }
protected:
    template <class _Ty>
    inline void construct(Ty* pTy, _Ty&& ty)
    {
        ::new (pTy) Ty(std::forward<_Ty>(ty));
    }

    inline void destruct(Ty* pTy)
    {
        pTy->~Ty();
    }

    inline Ty* Base() { return _val._base; }
    inline difference_type& Next() { return _val._next; }
protected:
    scoped_buffer _buff;
    _val_type _val;
};

NAMESPACE_ZH_END
