/*
 * vector val & vector iterator
 * xuantao, 2017
*/
#pragma once

#include "..\common.h"
#include <iterator>

NAMESPACE_ZH_BEGIN

namespace detail
{
    /*
     * vector inner value type, hold base pointer and index
    */
    template <class Ty>
    class _vector_val
    {
    public:
        typedef Ty value_type;
        typedef Ty* pointer;
        typedef Ty& reference;

        typedef size_t size_type;
        typedef std::ptrdiff_t difference_type;

        _vector_val(value_type* base)
            : _base(base), _next(0)
        {
        }

        _vector_val(_vector_val&& other)
            : _base(other._base), _next(other._next)
        {
            other._base = nullptr;;
            other._next = 0;
        }

        size_type size() const { return _next; }
        bool empty() const { return size() == 0; }

    public:
        value_type* _base;
        difference_type _next;
    };

    /*
     * vector const iterator
    */
    template <class Ty>
    class _vector_const_iterator : public std::iterator<std::random_access_iterator_tag, Ty>
    {
    public:
        typedef _vector_const_iterator<Ty> _my_iter;
        typedef std::iterator<std::random_access_iterator_tag, Ty> _base_type;
        typedef typename _base_type::difference_type difference_type;
        typedef size_t size_type;

        typedef Ty value_type;
        typedef const Ty* pointer;
        typedef const Ty& reference;

        typedef _vector_val<value_type> _val_type;
        typedef const _val_type* _val_ptr;
    public:
        _vector_const_iterator() : _val(nullptr), _idx(0) {}

        _vector_const_iterator(_val_ptr val, difference_type idx) : _val(val), _idx(idx)
        {
        }

        reference operator * () const
        {
            assert(_val && _idx < _val->_next);
            return _val->_base[_idx];
        }

        pointer operator -> () const
        {
            return &(**this);
        }

        _my_iter& operator ++ ()
        {
            if (_val && _idx < _val->_next)
                ++_idx;
            return *this;
        }

        _my_iter operator ++ (int)
        {
            _my_iter tmp = *this;
            ++*this;
            return tmp;
        }

        _my_iter& operator -- ()
        {
            if (_idx > 0)
                --_idx;
            return *this;
        }

        _my_iter operator -- (int)
        {
            _my_iter tmp = *this;
            --*this;
            return tmp;
        }

        _my_iter& operator += (difference_type off)
        {
            assert(_val && _idx + off < _val->_next && _idx + off >= 0);
            if (_val && _idx + off < _val->_next && _idx + off >= 0)
                _idx += off;
            return *this;
        }

        _my_iter operator + (difference_type off)
        {
            _my_iter tmp = *this;
            return (tmp += off);
        }

        _my_iter& operator -= (difference_type off)
        {
            return (*this += -off);
        }

        _my_iter operator - (difference_type off)
        {
            _my_iter tmp = *this;
            return (tmp -= off);
        }

        difference_type operator - (const _my_iter& other) const
        {
            assert(_val && _val == other._val);
            return _idx - other._idx;
        }

        reference operator [] (difference_type off) const
        {
            return (*(*this + off));
        }

        bool operator == (const _my_iter& other) const
        {
            return (*this - other) == 0;
        }

        bool operator != (const _my_iter& other) const
        {
            return !(*this == other);
        }

        bool operator < (const _my_iter& other) const
        {
            return (*this - other) < 0;
        }

        bool operator > (const _my_iter& other) const
        {
            return other < *this;
        }

        bool operator <= (const _my_iter& other) const
        {
            return !(other < *this);
        }

        bool operator >= (const _my_iter& other) const
        {
            return !(*this < other);
        }

    protected:
        difference_type _idx;
        _val_ptr _val;
    };

    /*
    * vector iterator
    */
    template <class Ty>
    class _vector_iterator : public _vector_const_iterator<Ty>
    {
    public:
        typedef _vector_iterator<Ty> _my_iter;
        typedef _vector_const_iterator<Ty> _my_base;

        typedef typename _my_base::difference_type difference_type;
        typedef typename _my_base::_val_ptr _val_ptr;
        typedef typename _my_base::value_type value_type;
        typedef Ty* pointer;
        typedef Ty& reference;
    public:
        _vector_iterator() {}

        _vector_iterator(_val_ptr val, difference_type idx) : _my_base(val, idx)
        {
        }

        reference operator * () const
        {
            return ((reference)**(_my_base *)this);
        }

        pointer operator -> () const
        {
            return (std::pointer_traits<pointer>::pointer_to(**this));
        }

        _my_iter& operator ++ ()
        {
            ++*(_my_base *)this;
            return (*this);
        }

        _my_iter operator ++ (int)
        {
            _my_iter tmp = *this;
            ++*this;
            return tmp;
        }

        _my_iter& operator -- ()
        {
            --*(_my_base *)this;
            return (*this);
        }

        _my_iter operator -- (int)
        {
            _my_iter tmp = *this;
            --*this;
            return tmp;
        }

        _my_iter& operator += (difference_type _Off)
        {
            *(_my_base *)this += _Off;
            return (*this);
        }

        _my_iter operator + (difference_type _Off) const
        {
            _my_iter _Tmp = *this;
            return (_Tmp += _Off);
        }

        _my_iter& operator -= (difference_type _Off)
        {
            return (*this += -_Off);
        }

        _my_iter operator - (difference_type _Off) const
        {
            _my_iter tmp = *this;
            return (tmp -= _Off);
        }

        difference_type operator - (const _my_base& _Right) const
        {
            return (*(_my_base *)this - _Right);
        }

        reference operator [] (difference_type _Off) const
        {
            return (*(*this + _Off));
        }
    };
}

NAMESPACE_ZH_END
