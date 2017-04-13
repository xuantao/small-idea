/*
 * scoped vector, copy from std::vector
 * xuantao, 2017
*/
#pragma once

#include "scoped_buffer.h"

//std::vector

namespace zh
{
    template <class Ty>
    class _scoped_vecoter_iter
    {
    public:
        Ty& operator* ()
        {
            return _objs[_idx];
        }

        _scoped_vecoter_iter operator + (size_t n)
        {
            return _scoped_vecoter_iter();
        }

    protected:
        Ty* _objs;
        size_t _idx;
    };

    template <class Ty>
    class scoped_vector
    {
    public:
        typedef _scoped_vecoter_iter<Ty> iterator;
        typedef _scoped_vecoter_iter<Ty> const_iterator;
    public:
        scoped_vector(scoped_buffer&& buffer)
            : _buff(std::forward(buffer))
            , _idx(0)
            , _pObj((Ty*)buffer.get())
        {
        }

        ~scoped_vector()
        {
            while (_idx > 0)
            {
                --_idx;
                destruct(&_pObj[_idx]);
            }
        }

    public:
        Ty& operator [] (size_t idx)
        {
            assert(idx < _idx);
            return _pObj[idx];
        }

        const Ty& operator [] (size_t idx) const
        {
            assert(idx < _idx);
            return _pObj[idx];
        }

        Ty& front()
        {
            assert(!empty());
            return _pObj[0];
        }

        Ty& back()
        {
            assert(!empty());
            return _pObj[_idx - 1];
        }

    public:
        void push_back(const Ty& val)
        {
            assert(_idx < capacity());

            construct(&_pObj[_idx++], val);
        }

        void pop_back()
        {
            assert(!empty());

            --idx;
            destruct(&_pObj[_idx]);
        }

        iterator intert(const_iterator position, const Ty& val)
        {
            return insert(position, 1, val);
        }

        iterator intert(const_iterator position, size_t n, const Ty& val)
        {
            assert(n < capacity() - size());

            size_t idx = &(*position) - _pObj;
            assert(idx >= 0 && idx <= _idx);

            // move
            for (size_t i = _idx - idx + n; i > 0; --i)
            {
                _pObj[_idx + i] = _pObj[idx + i];
                destruct(&_pObj[idx + i]);
            }

            // copy construct
            for (size_t i = 0; i < n; ++i)
            {
                construct(_pObj[idx + i], val);
            }

            _idx += n;
            return iterator();
        }

        iterator erase(const_iterator position)
        {
            return iterator();
        }

        iterator erase(const_iterator first, const_iterator last)
        {
            return iterator();
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

    protected:
        void construct(Ty* pTy, const Ty& ty)
        {
            ::new pTy Ty(ty);
        }

        void destruct(Ty* pTy)
        {
            pTy->~Ty();
        }

    protected:
        scoped_buffer _buff;
        size_t _idx;
        Ty* _pObj;
    };

}
