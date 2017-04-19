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

    enum class _rb_color : bool
    {
        red, black,
    };

    struct _tree_nil
    {
    };

    template <class Ty>
    struct _tree_node
    {
        _tree_node* left;
        _tree_node* right;
        _tree_node* parent;
        _rb_color color;
        bool nil;
        Ty val;
    };

    template <class Ty>
    class _tree_val
    {
    public:
        typedef _tree_val<Ty> _my_type;
        typedef Ty value_type;
        typedef _tree_node<Ty> _node;
        typedef _node* _node_ptr;
        typedef size_t size_type;
        typedef std::ptrdiff_t difference_type;

        static _node_ptr _max(_node_ptr node)
        {
            while (!node->right->isNil)
                node = node->right;
            return node;
        }

        static _node_ptr _min(_node_ptr node)
        {
            while (!node->left->isNil)
                node = node->left;
            return node;
        }

        _node_ptr _head;
        size_type _size;
    };

    template <class TreeVal>
    class _tree_iterator
    {
    public:
        typedef TreeVal* _val_ptr;
        typedef typename _tree_node <TreeVal::value_type> _node;
        typedef _node* _node_ptr;
    public:
        _tree_iterator(_node_ptr node)
        {
        }

        _node_ptr _node;
    };

    template <class TreeVal>
    class _tree_const_iterator
    {
    public:
        typedef TreeVal* _val_ptr;
        typedef typename _tree_node <TreeVal::value_type> _node;
        typedef _node* _node_ptr;
    public:
        _tree_const_iterator(_node_ptr node)
        {
        }

        const _node_ptr _node;
    };

    template <class Traits>
    class _tree
    {
    public:
        typedef _tree<Traits> _MyTy;

        typedef typename Traits::value_type value_type;
        typedef typename Traits::key_compare key_compare;
        typedef typename Traits::value_compare value_compare;

        typedef _tree_val<value_type> _tree_val;
        typedef _tree_val::_node _node;
        typedef _tree_val::_node_ptr _node_ptr;

        typedef size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef fixed_allocator<_node> allocator;

        typedef _tree_iterator<_tree_val> iterator;
        typedef _tree_const_iterator<_tree_val> const_iterator;
        typedef std::pair<iterator, bool> pairib;
    public:
        static size_type buffer_size(size_type count) { return sizeof(_node) * (count + 1); }

    public:
        _tree(scoped_buffer&& buffer)
            : _buffer(std::forward<scoped_buffer>(buffer))
            , _alloc(buffer.get(), buffer.size())
            , Root(nullptr)
            , _size(0)
        {
        }

        _tree(_MyTy&& other)
            : _buffer(std::forward<scoped_buffer>(other._buffer))
            , _alloc(std::forward<allocator>(other._alloc))
            , Root(other._root)
            , _size(other._size)
        {
            other._root = nullptr;
            other._size = 0;
        }

    public:
        iterator begin()
        {
            return iterator(Lmost());
        }
        iterator end();


    public:
        // capacity
        bool empty() const { return _size == 0 };
        bool size() const { return _size; }
        bool max_size() const { return _alloc.max_size(); }

    public:
        pairib insert(const value_type& val)
        {
            return Insert(false, val, _tree_nil())
        }

        pairib insert(value_type&& val)
        {
            return Insert(false, std::forward<value_type>(val), _tree_nil())
        }



    protected:
        template <class _Ty, class _Node>
        pairib Insert(bool leftish, _Ty&& val, _Node newNode)
        {
            _node_ptr root = Root();
            _node_ptr node = Head();
            bool addLeft = true;

            while (!root->nil)
            {
                node = root;
                if (leftish)
                    addLeft = _camp(Key(root), Kfn(val));
                else
                    addLeft = _camp(Kfn(val), Key(root));

                root = addLeft ? root->left : root->right;
            }

            iterator it = iterator(node);
            if (!addLeft)
                ;
            else if (it == begin())
                return pairib(Insert_at(true, node, std::forward<_Ty>(val), newNode), true);
            else
                --it;

            if (_camp(Key(it._node), Kfn(val)))
            {
                return pairib(Insert_at(addLeft, node, std::forward<_Ty>(val), newNode), true);
            }
            else
            {
                Destroy_if_not_nil(newNode);
                return pairib(it, false);
            }
        }

        template <class _Ty, class _Node>
        iterator Insert_at(bool addLeft, _node_ptr parent, _Ty&& val, _Node newNode)
        {
            assert(max_size() - 1 > size());

            _node_ptr node = Buynode_if_nil(newNode, std::forward<_Ty>(val));

            ++_val._size;
            node->parent = parent;

            if (parent == Head())
            {
                Root() = node;
                Lmost() = node;
                Rmost() = node;
            }
            else if (addLeft)
            {
                parent->left = node;
                if (parent == Lmost())
                    Lmost() = node;
            }
            else
            {
                parent->right = node;
                if (parent == Rmost())
                    Rmost() = node;
            }

            //balance
            for (_node_ptr balance = node; balance->parent->color == _rb_color.red;)
            {
                if (balance->parent == balance->parent->parent->left)
                {
                    _node_ptr n_parent = balance->parent->parent->right;
                    if (n_parent->color == _rb_color.red)
                    {
                        n_parent->color = _rb_color.black;
                        balance->parent->color = _rb_color.black;
                        balance->parent->parent->color = _rb_color.red;
                        balance = balance->parent->parent;
                    }
                    else
                    {
                        if (balance == balance->parent->right)
                        {
                            balance = balance->parent;
                            Lrotate(balance);
                        }

                        balance->parent->color = _rb_color.black;
                        balance->parent->parent->color = _rb_color.red;
                        Rrotate(balance->parent->parent);
                    }
                }
                else
                {
                    _node_ptr n_parent = balance->parent->parent->left;
                    if (n_parent->color == _rb_color.red)
                    {
                        n_parent->color = _rb_color::black;
                        balance->parent->color = _rb_color.black;
                        balance->parent->parent->color = _rb_color.red;
                        balance = balance->parent->parent;
                    }
                    else
                    {
                        if (balance == balance->parent->left)
                        {
                            balance = balance->parent;
                            Rrotate(balance);
                        }

                        balance->parent->color = _rb_color.black;
                        balance->parent->parent->color = _rb_color.red;
                        Lrotate(balance->parent->parent);
                    }
                }
            }

            Root()->color = _rb_color.black;
            return iterator(node);
        }
    protected:
        const key_type& Kfn(const value_type& _Val) const { return (Traits::_kfn(_Val)); }
        const key_type& Key(_node_ptr node) const { return (const key_type&)Kfn(node->val); }

        _node_ptr& Root() { return _val._head->parent; }
        _node_ptr& Head() { return _val._head; }
        _node_ptr& Lmost() { return _val._head->left; }
        _node_ptr& Rmost() { return _val._head->right; }

        void Lrotate(_node_ptr node)
        {
            _node_ptr right = node->right;
            node->right = right->left;

            if (!right->left->nil)
                right->left->parent = node;
            right->parent = node->parent;

            if (node == Root())
                Root() = right;
            else if (node == node->parent->left)
                node->parent->left = right;
            else
                node->parent->right = right;

            right->left = node;
            node->parent = right;
        }

        void Rrotate(_node_ptr node)
        {
            _node_ptr left = node->left;
            node->left = left->right;

            if (!left->right->nil)
                left->right->parent = node;
            left->parent = node->parent;

            if (node == Root())
                Root() = left;
            else if (node == node->parent->right)
                node->parent->right = left;
            else
                node->parent->left = left;

            left->right = node;
            node->parent = left;
        }

        void Destroy_if_not_nil(_node_ptr node)
        {
            _alloc.destroy(&node->val);
            _alloc.deallocate(node);
        }

        void Destroy_if_not_nil(_tree_nil) {}

        template<class _Ty>
        _node_ptr Buynode_if_nil(_node_ptr node, _Ty&&)
        {
            return node;
        }

        template<class _Ty>
        _node_ptr Buynode_if_nil(_tree_nil, _Ty&& val)
        {
            return (Buynode(std::forward<_Ty>(val)));
        }

        _node_ptr Buynode0()
        {
            _node_ptr node = _alloc.allocate();
            node->left = Head();
            node->right = Head();
            node->parent = Head();
            return node;
        }

        template<class... _Ty>
        _node_ptr Buynode(_Ty&&... val)
        {
            _node_ptr node = Buynode0();
            node->color = _rb_color.red;
            node->nil = false;

            _alloc.construct(&node->val, std::forward<_Ty>(val)...);
            return node;
        }

    protected:
        scoped_buffer _buffer;
        allocator _alloc;
        _tree_val _val;
        key_compare _camp;
    };
}

NAMESPACE_ZH_END
