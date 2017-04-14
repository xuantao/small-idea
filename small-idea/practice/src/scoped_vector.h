/*
 * scoped vector, copy from std::vector
 * xuantao, 2017
*/
#pragma once

#include <iterator>
#include <cassert>
#include <type_traits>
#include "scoped_buffer.h"

NAMESPACE_ZH_BEGIN

namespace detail
{
    template <class Ty>
    class _scoped_vecoter_iter : public std::iterator<std::random_access_iterator_tag, Ty>
    {
    public:
        _scoped_vecoter_iter(Ty* base, size_t size, size_t idx)
            : _base(base), _size(size), _idx(idx)
        {
        }
    public:
        Ty& operator* ()
        {
            return _base[_idx];
        }

        _scoped_vecoter_iter operator + (size_t n)
        {
            assert(_idx + n <= _size);
            return _scoped_vecoter_iter(_base, _size, _idx + n);
        }

        _scoped_vecoter_iter operator - (size_t n)
        {
            assert(_idx >= n);
            return _scoped_vecoter_iter(_base, _size, _idx - n);
        }

        _scoped_vecoter_iter& operator ++ ()
        {
            assert(_idx < _size);
            ++_idx;
            return *this;
        }

        _scoped_vecoter_iter operator ++ (int)
        {
            assert(_idx < _size);
            _scoped_vecoter_iter temp(*this);
            ++_idx;
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
            assert(_idx > 0);
            _scoped_vecoter_iter temp(*this);
            --_idx;
            return temp;
        }

        bool operator == (const _scoped_vecoter_iter& other) const
        {
            assert(_base == other->_base);
            return _idx == other->_idx;
        }
    public:
        friend size_t operator - (const _scoped_vecoter_iter& last, const _scoped_vecoter_iter& first);

    protected:
        Ty* _base;
        size_t _size;
        size_t _idx;
    };

    template <class Ty>
    size_t operator - (const _scoped_vecoter_iter<Ty>& last, const _scoped_vecoter_iter<Ty>& first)
    {
        assert(last._base == first._base);
        assert(last._idx >= first._idx);
        return last._idx = first._idx;
    }

    template <class Ty>
    class _const_scoped_vecoter_iter : public std::iterator<std::random_access_iterator_tag, Ty>
    {
    public:
        _const_scoped_vecoter_iter(Ty* base, size_t size, size_t idx)
            : _base(base), _size(size), _idx(idx)
        {
        }
    public:
        const Ty& operator* () const
        {
            return _base[_idx];
        }

        _const_scoped_vecoter_iter operator + (size_t n)
        {
            assert(_idx + n <= _size);
            return _scoped_vecoter_iter(_base, _size, _idx + n);
        }

        _const_scoped_vecoter_iter operator - (size_t n)
        {
            assert(_idx >= n);
            return _const_scoped_vecoter_iter(_base, _size, _idx - n);
        }

        _const_scoped_vecoter_iter& operator ++ ()
        {
            assert(_idx < _size);
            ++_idx;
            return *this;
        }

        _const_scoped_vecoter_iter operator ++ (int)
        {
            assert(_idx < _size);
            _scoped_vecoter_iter temp(*this);
            ++_idx;
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
            assert(_idx > 0);
            _const_scoped_vecoter_iter temp(*this);
            --_idx;
            return temp;
        }

        bool operator == (const _const_scoped_vecoter_iter& other) const
        {
            assert(_base == other->_base);
            return _idx == other->_idx;
        }

    public:
        friend size_t operator - (const _const_scoped_vecoter_iter& last, const _const_scoped_vecoter_iter& first);

    protected:
        Ty* _base;
        size_t _size;
        size_t _idx;
    };

    template <class Ty>
    size_t operator - (const _const_scoped_vecoter_iter<Ty>& last, const _const_scoped_vecoter_iter<Ty>& first)
    {
        assert(last._base == first._base);
        assert(last._idx >= first._idx);
        return last._idx = first._idx;
    }
}

template <class Ty>
class scoped_vector
{
public:
    typedef detail::_scoped_vecoter_iter<Ty>        iterator;
    typedef detail::_const_scoped_vecoter_iter<Ty>  const_iterator;
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
            destruct(&get_base()[_idx]);
        }
    }

public:
    Ty& operator [] (size_t idx)
    {
        assert(idx < _idx);
        return get_base()[idx];
    }

    const Ty& operator [] (size_t idx) const
    {
        return const_cast<scoped_vector*>(this)->operator [](idx);
    }

    Ty& front()
    {
        assert(!empty());
        return get_base()[0];
    }

    const Ty& front() const
    {
        return const_cast<scoped_vector*>(this)->front();
    }

    Ty& back()
    {
        assert(!empty());
        return get_base()[_idx - 1];
    }

    const Ty&  back() const
    {
        return const_cast<scoped_vector*>(this)->back();
    }

    iterator begin()
    {
        return iterator(get_base(), size(), 0);
    }

    iterator end()
    {
        return iterator(get_base(), size(), _idx);
    }

    const_iterator cbegin() const
    {
        return const_iterator(get_base(), size(), 0);
    }

    const_iterator cend() const
    {
        return const_iterator(get_base(), size(), _idx);
    }
public:
    void push_back(const Ty& val)
    {
        assert(_idx < capacity());

        construct(&get_base()[_idx], val);
        ++_idx;
    }

    void pop_back()
    {
        assert(!empty());

        --idx;
        destruct(&get_base()[_idx]);
    }

    iterator insert(const_iterator position, const Ty& val)
    {
        return insert(position, 1, val);
    }

    iterator insert(const_iterator position, size_t n, const Ty& val)
    {
        assert(n < capacity() - size());

        Ty* base = get_base();
        size_t idx = &(*position) - base;
        assert(idx >= 0 && idx <= _idx);

        // move
        for (size_t i = _idx - idx + n; i > 0; --i)
        {
            construct(base[idx + i], base[idx + i]);
            destruct(&base[idx + i]);
        }

        // copy construct
        for (size_t i = 0; i < n; ++i)
        {
            construct(base[idx + i], val);
        }

        _idx += n;
        return iterator(get_base(), size(), idx);
    }

    iterator erase(const_iterator position)
    {
        return erase(position, position + 1);
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        Ty* base = get_base();
        size_t begIdx = &(*first) - base;
        size_t endIdx = &(*last) - base;
        assert(begIdx <= _idx);
        assert(endIdx >= begIdx && endIdx <= _idx);

        // destruct
        for (size_t i = begIdx; i < endIdx; i++)
        {
            destruct(&base[begIdx + i]);
        }

        // move
        for (size_t i = 0; i < _idx - endIdx; ++i)
        {
            construct(&base[begIdx + i], base[endIdx + i]);
            destruct(&base[endIdx + i])
        }

        _idx -= endIdx - begIdx;
        return iterator(get_base(), size(), begIdx);
    }

    bool empty() const
    {
        return _idx == 0;
    }

    size_t size() const
    {
        return _idx;
    }

    size_t capacity() const
    {
        return _buff.size() / sizeof(Ty);
    }

    size_t max_size() const
    {
        return capacity();
    }
protected:
    void construct(Ty* pTy, const Ty& ty)
    {
        ::new (pTy) Ty(ty);
    }

    void destruct(Ty* pTy)
    {
        pTy->~Ty();
    }

    Ty* get_base() const
    {
        return (Ty*)_buff.get();
    }
protected:
    scoped_buffer _buff;
    size_t _idx;
};

NAMESPACE_ZH_END
