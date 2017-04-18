/*
 * tree, a simple red black tree
 * xuantao, 2017
*/
#pragma once

#include <utility>
#include "fixed_allocator.h"

NAMESPACE_ZH_BEGIN

namespace detail
{
    template <class Ty, class Pr, bool Mfl>
    class _set_traits
    {
    public:
        typedef Ty key_type;
        typedef Ty value_type;
        typedef Pr key_compare;
        typedef key_compare value_compare;

        enum { _multi = Mfl };

        static cosnt key_type& kfn(const value_type& val) { return val; }
    };

    template <class Kty, class Ty, class Pr, bool Mfl>
    class _map_traits
    {
    public:
        typedef Kty key_type;
        typedef std::pair<const Kty, Ty> value_type;
        typedef Pr key_compare;

        enum { multi = Mfl };

        class value_compare
        {
            friend class _map_traits<Kty, Ty, Pr, Mfl>;
        public:
            bool operator()(const value_type& _Left, const value_type& _Right) const
            {
                return (comp(_Left.first, _Right.first));
            }

            value_compare(key_compare _Pred) : comp(_Pred)
            {
            }

        protected:
            key_compare comp;
        };

        template<class Ty1, class Ty2>
        static const Kty& kfn(const std::pair<Ty1, Ty2>& Val)
        {
            return (Val.first);
        }
    };

    template <class Ty>
    struct _tree_node
    {
        _tree_node* left;
        _tree_node* right;
        _tree_node* parent;
        char color;
        bool isNil;
        Ty val;
    };

    template <class Traits>
    class _tree
    {
    public:
        typedef _tree<Traits> _MyTy;
        typedef _tree_node<Traits::value_type> _node;
        typedef Traits::key_compare key_compare;
        typedef Traits::value_compare value_compare;
        typedef size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef fixed_allocator<_node> allocator;

    public:
        static size_type buffer_size(size_type count) { return sizeof(_node) * (count + 1); }

    public:
        _tree(scoped_buffer&& buffer)
            : _buffer(std::forward<scoped_buffer>(buffer))
            , _alloc(buffer.get(), buffer.size())
            , _root(nullptr)
            , _size(0)
        {
        }

        _tree(_MyTy&& other)
            : _buffer(std::forward<scoped_buffer>(other._buffer))
            , _alloc(std::forward<allocator>(other._alloc))
            , _root(other._root)
            , _size(other._size)
        {
            other._root = nullptr;
            other._size = 0;
        }

    public:

    public:
        // capacity
        bool empty() const { return _size == 0 };
        bool size() const { return _size; }
        bool max_size() const { return _alloc.max_size(); }

    public:
        bool insert(value_type&& val)
        {
            return false;
        }

    protected:
        _node* _root;
        size_type _size;
        scoped_buffer _buffer;
        allocator _alloc;
    };
}

NAMESPACE_ZH_END
