/*
 * scoped vector, copy from std::vector
 * xuantao, 2017
*/
#pragma once

#include <iterator>
#include <cassert>
#include "scoped_buffer.h"

NAMESPACE_ZH_BEGIN

template <class Ty>
class scoped_vector;

namespace detail
{
    template <class Ty>
    class _vector_val
    {
    public:
        typedef Ty value_type;
        typedef size_t size_type;
        typedef std::ptrdiff_t difference_type;

        _vector_val(value_type* base, difference_type idx)
            : _base(base), _idx(idx)
        {
        }

        _vector_val(const _vector_val& other)
            : _base(other._base), _idx(other._idx)
        {
        }

        size_type size() const { return _idx; }
        bool empty() const { return size() == 0; }

    public:
        value_type* _base;
        difference_type _idx;
    };

    template <class Ty>
    class _vector_iterator_base : public std::iterator<std::random_access_iterator_tag, Ty>
    {
    public:
        typedef std::iterator<std::random_access_iterator_tag, Ty> _base_type;
        typedef typename _base_type::difference_type difference_type;
        typedef size_t size_type;

        typedef Ty value_type;
        typedef value_type& reference;
        typedef value_type* pointer;

        typedef _vector_val<value_type> _val_type;


        _val_type _val;
    };

    template <class Ty>
    class _scoped_vecoter_iter : public std::iterator<std::random_access_iterator_tag, Ty>
    {
    public:
        typedef std::iterator<std::random_access_iterator_tag, Ty> _MyBase;
        typedef typename _MyBase::difference_type difference_type;
        typedef size_t size_type;
        typedef scoped_vector<Ty> scoped_vector;
    public:
        _scoped_vecoter_iter(scoped_vector& src, difference_type idx)
            : _src(src), _idx(idx)
        {
        }
    public:
        Ty& operator* ()
        {
            return _src.operator[](_idx);
        }

        _scoped_vecoter_iter& operator ++ ()
        {
            assert((size_type)_idx < _src.size());
            ++_idx;
            return *this;
        }

        _scoped_vecoter_iter operator ++ (int)
        {
            _scoped_vecoter_iter temp(*this);
            ++*this;
            return temp;
        }

        _scoped_vecoter_iter& operator -- ()
        {
            assert(_idx > 0);
            --_idx;
            return *this;
        }

        _scoped_vecoter_iter operator -- (int)
        {
            _scoped_vecoter_iter temp(*this);
            --*this;
            return temp;
        }

        _scoped_vecoter_iter& operator += (difference_type n)
        {
            assert((size_type)(_idx + n) <= _src.size());
            _idx += n;
            return *this;
        }


        _scoped_vecoter_iter& operator -= (difference_type n)
        {
            assert(_idx >= n);
            _idx -= n;
            return *this;
        }

        _scoped_vecoter_iter operator + (difference_type n) const
        {
            assert((size_type)(_idx + n) <= _src.size());
            return _scoped_vecoter_iter(_src, _idx + n);
        }

        _scoped_vecoter_iter operator - (difference_type n) const
        {
            assert(_idx >= n);
            return _scoped_vecoter_iter(_src, _idx - n);
        }

        difference_type operator - (const _scoped_vecoter_iter& prev) const
        {
            assert(&_src == &prev._src);
            return _idx - prev._idx;
        }

        bool operator == (const _scoped_vecoter_iter& other) const
        {
            assert(&_src == &other._src);
            return _idx == other._idx;
        }

        bool operator != (const _scoped_vecoter_iter& other) const
        {
            return !this->operator == (other);
        }

    protected:
        scoped_vector&  _src;
        difference_type _idx;
    };


    template <class Ty>
    class _const_scoped_vecoter_iter : public std::iterator<std::random_access_iterator_tag, Ty>
    {
    public:
        typedef std::iterator<std::random_access_iterator_tag, Ty> _MyBase;
        typedef typename _MyBase::difference_type difference_type;
        typedef size_t size_type;
        typedef scoped_vector<Ty> scoped_vector;
    public:
        _const_scoped_vecoter_iter(const scoped_vector& src, difference_type idx)
            : _src(src), _idx(idx)
        {
        }
    public:
        const Ty& operator* () const
        {
            return _src.operator[](_idx);
        }

        _const_scoped_vecoter_iter& operator ++ ()
        {
            assert((size_type)_idx < _src.size());
            ++_idx;
            return *this;
        }

        _const_scoped_vecoter_iter operator ++ (int)
        {
            _const_scoped_vecoter_iter temp(*this);
            ++*this;
            return temp;
        }

        _const_scoped_vecoter_iter& operator -- ()
        {
            assert(_idx > 0);
            --_idx;
            return *this;
        }

        _const_scoped_vecoter_iter operator -- (int)
        {
            _scoped_vecoter_iter temp(*this);
            --*this;
            return temp;
        }

        _const_scoped_vecoter_iter& operator += (difference_type n)
        {
            assert((size_type)(_idx + n) <= _src.size());
            _idx += n;
            return *this;
        }

        _const_scoped_vecoter_iter& operator -= (difference_type n)
        {
            assert(_idx >= n);
            _idx -= n;
            return *this;
        }

        _const_scoped_vecoter_iter operator + (difference_type n) const
        {
            assert((size_type)(_idx + n) <= _src.size());
            return _scoped_vecoter_iter(_src, _idx + n);
        }

        _const_scoped_vecoter_iter operator - (difference_type n) const
        {
            assert(_idx >= n);
            return _scoped_vecoter_iter(_src, _idx - n);
        }

        difference_type operator - (const _const_scoped_vecoter_iter& prev) const
        {
            assert(&_src == &prev._src);
            return _idx - prev._idx;
        }

        bool operator == (const _const_scoped_vecoter_iter& other) const
        {
            assert(&_src == &other._src);
            return _idx == other._idx;
        }

        bool operator != (const _const_scoped_vecoter_iter& other) const
        {
            return !this->operator == (other);
        }

    protected:
        const scoped_vector&    _src;
        difference_type         _idx;
    };
}

template <class Ty>
class scoped_vector
{
public:
    typedef Ty value_type;
    typedef Ty* pointer;
    typedef Ty& reference;
    typedef const Ty* const_pointer;
    typedef const Ty& const_reference;
    typedef detail::_scoped_vecoter_iter<Ty> iterator;
    typedef detail::_const_scoped_vecoter_iter<Ty> const_iterator;
    typedef detail::_vector_val<value_type> _val_type;
    typedef size_t size_type;
    typedef std::ptrdiff_t difference_type;

public:
    static size_type buffer_size(size_type c) { return sizeof(Ty) * c; }

public:
    scoped_vector(scoped_buffer&& buffer)
        : _buff(std::forward<scoped_buffer>(buffer))
        , _val(static_cast<pointer>(buffer.get()), 0)
    {
    }

    scoped_vector(scoped_vector&& other)
        : _buff(std::forward<scoped_buffer>(other._buff))
        , _val(other._val)
    {
    }

    ~scoped_vector()
    {
        while (!empty())
            pop_back();
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
        return Base()[Idx() - 1];
    }

    const_reference back() const
    {
        return const_cast<scoped_vector*>(this)->back();
    }

    iterator begin()
    {
        return iterator(*this, 0);
    }

    iterator end()
    {
        return iterator(*this, Idx());
    }

    const_iterator cbegin() const
    {
        return const_iterator(*this, 0);
    }

    const_iterator cend() const
    {
        return const_iterator(*this, Idx());
    }

public:
    template <class _Ty>
    void push_back(_Ty&& val)
    {
        assert(capacity() > size());

        construct(&Base()[Idx()], std::forward<_Ty>(val));
        ++Idx();
    }

    void pop_back()
    {
        assert(!empty());

        --Idx();
        destruct(&Base()[Idx()]);
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

        assert(n < capacity() - size());
        assert(idx >= 0 && idx <= Idx());

        // move
        for (size_type i = Idx() - idx + n; i > 0; --i)
        {
            construct(Base()[idx + i], Base()[idx + i]);
            destruct(&Base()[idx + i]);
        }

        // copy construct
        for (size_type i = 0; i < n; ++i)
            construct(Base()[idx + i], std::forward<_Ty>(val));

        Idx() += (difference_type)n;
        return iterator(*this, idx);
    }

    iterator erase(const_iterator position)
    {
        return erase(position, position + 1);
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        difference_type begIdx = first - begin();
        difference_type endIdx = last - begin();
        assert(begIdx <= Idx());
        assert(endIdx >= begIdx && endIdx <= Idx());

        // destruct
        for (size_t i = begIdx; i < endIdx; i++)
            destruct(&Base()[i]);

        // move
        for (size_t i = 0; i < Idx() - endIdx; ++i)
        {
            construct(&Base()[begIdx + i], Base()[endIdx + i]);
            destruct(&Base()[endIdx + i])
        }

        Idx() -= (endIdx - begIdx);
        return iterator(*this, begIdx);
    }

    bool empty() const
    {
        return _val.empty();
    }

    size_type size() const
    {
        return _val.empty();
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
    inline difference_type& Idx() { return _val._idx; }
protected:
    scoped_buffer _buff;
    _val_type _val;
};

NAMESPACE_ZH_END
