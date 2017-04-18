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
    typedef detail::_scoped_vecoter_iter<Ty> iterator;
    typedef detail::_const_scoped_vecoter_iter<Ty> const_iterator;

    typedef size_t size_type;
    typedef std::ptrdiff_t difference_type;

public:
    static size_type buffer_size(size_type c) { return sizeof(Ty) * c; }

public:
    scoped_vector(scoped_buffer&& buffer)
        : _buff(std::forward<scoped_buffer>(buffer))
        , _idx(0)
    {
    }

    scoped_vector(scoped_vector&& other)
        : _buff(std::forward<scoped_buffer>(other._buff))
        , _idx(other._idx)
    {
    }

    ~scoped_vector()
    {
        while (_idx > 0)
        {
            --_idx;
            destruct(&base()[_idx]);
        }
    }

public:
    Ty& operator [] (difference_type idx)
    {
        assert((size_type)idx < size() && idx >= 0);
        return base()[idx];
    }

    const Ty& operator [] (difference_type idx) const
    {
        return const_cast<scoped_vector*>(this)->operator [](idx);
    }

    Ty& front()
    {
        assert(!empty());
        return base()[0];
    }

    const Ty& front() const
    {
        return const_cast<scoped_vector*>(this)->front();
    }

    Ty& back()
    {
        assert(!empty());
        return base()[_idx - 1];
    }

    const Ty& back() const
    {
        return const_cast<scoped_vector*>(this)->back();
    }

    iterator begin()
    {
        return iterator(*this, 0);
    }

    iterator end()
    {
        return iterator(*this, _idx);
    }

    const_iterator cbegin() const
    {
        return const_iterator(*this, 0);
    }

    const_iterator cend() const
    {
        return const_iterator(*this, _idx);
    }

public:
    template <class _Ty>
    void push_back(_Ty&& val)
    {
        assert(capacity() > size());

        construct(&base()[_idx], std::forward<_Ty>(val));
        ++_idx;
    }

    void pop_back()
    {
        assert(!empty());

        --idx;
        destruct(&base()[_idx]);
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
        assert(idx >= 0 && idx <= _idx);

        // move
        for (size_type i = _idx - idx + n; i > 0; --i)
        {
            construct(base()[idx + i], base()[idx + i]);
            destruct(&base()[idx + i]);
        }

        // copy construct
        for (size_type i = 0; i < n; ++i)
        {
            construct(base()[idx + i], std::forward<_Ty>(val));
        }

        _idx += (difference_type)n;
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
        assert(begIdx <= _idx);
        assert(endIdx >= begIdx && endIdx <= _idx);

        // destruct
        for (size_t i = begIdx; i < endIdx; i++)
        {
            destruct(&base()[i]);
        }

        // move
        for (size_t i = 0; i < _idx - endIdx; ++i)
        {
            construct(&base()[begIdx + i], base()[endIdx + i]);
            destruct(&base()[endIdx + i])
        }

        _idx -= (endIdx - begIdx);
        return iterator(*this, begIdx);
    }

    bool empty() const
    {
        return _idx == 0;
    }

    size_type size() const
    {
        return _idx;
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
    void construct(Ty* pTy, _Ty&& ty)
    {
        ::new (pTy) Ty(std::forward<_Ty>(ty));
    }

    void destruct(Ty* pTy)
    {
        pTy->~Ty();
    }

    Ty* base() const
    {
        return (Ty*)_buff.get();
    }
protected:
    scoped_buffer _buff;
    difference_type _idx;
};

NAMESPACE_ZH_END
