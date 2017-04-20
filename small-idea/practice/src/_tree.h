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

        static const key_type& kfn(const value_type& val) { return val; }
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
                return (_cmp(_Left.first, _Right.first));
            }

            value_compare(key_compare _Pred) : _cmp(_Pred)
            {
            }

        protected:
            key_compare _cmp;
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

        static _node_ptr Max(_node_ptr node)
        {
            while (!node->right->nil)
                node = node->right;
            return node;
        }

        static _node_ptr Min(_node_ptr node)
        {
            while (!node->left->nil)
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
        iterator end()
        {
            return iterator(Head());
        }

        const_iterator cbegin() const
        {
            return const_iterator();
        }

        const_iterator cend() const
        {
            return const_iterator();
        }
    public:
        // capacity
        bool empty() const { return size() == 0 };
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

        iterator erase(const_iterator it)
        {
            //TODO: check it is valid
            _node_ptr eraseNode = it._node;
            ++it;

            _node_ptr fixNode = nullptr;
            _node_ptr fixParent = nullptr;
            _node_ptr node = eraseNode;

            if (node->left->nil)
            {
                fixNode = node->right;
            }
            else if (node->right->nil)
            {
                fixNode = node->left;
            }
            else
            {
                node = it._node;
                fixNode = node->right;
            }

            if (node == eraseNode)
            {
                fixParent = eraseNode->parent;
                if (fixNode->nil)
                    fixNode->parent = fixParent;

                if (Root() == eraseNode)
                    Root() = fixNode;
                else if (fixParent->left == eraseNode)
                    fixParent->left = fixNode;
                else
                    fixParent->right = fixNode;

                if (Lmost() == eraseNode)
                    Lmost() = fixNode->nil ? fixParent : Min(fixNode);
                if (Rmost() == eraseNode)
                    Rmost() = fixNode->nil ? fixParent : Max(fixNode);
            }
            else
            {
                eraseNode->left->parent = node;
                node->left = eraseNode->left;

                if (node == eraseNode->right)
                {
                    fixParent = node;
                }
                else
                {
                    fixParent = node->parent;
                    if (!fixNode->nil)
                        fixNode->parent = fixParent;
                    fixParent->left = fixNode;
                    node->right = eraseNode->right;
                    eraseNode->right->parent = node;
                }

                if (Root() == eraseNode)
                    Root() = node;
                else if (eraseNode->parent->left == eraseNode)
                    eraseNode->parent->left = node;
                else
                    eraseNode->parent->right = node;

                node->parent = eraseNode->parent;
                std::swap(node->color, eraseNode->color);
            }

            // balance
            if (eraseNode->color == _rb_color::black)
            {
                for (; fixNode != Root() && fixNode->color == _rb_color::black;
                    fixParent = fixNode->parent)
                {
                    if (fixNode == fixParent->left)
                    {
                        node = fixParent->right;
                        if (node->color == _rb_color::red)
                        {
                            node->color = _rb_color::black;
                            fixParent->color = _rb_color::red;
                            Lrotate(fixParent);
                            node = fixParent->right;
                        }

                        if (node->nil)
                        {
                            fixNode = fixParent;
                        }
                        else if (node->left->color == _rb_color::black)
                        {
                            node->color = _rb_color::red;
                            fixNode = fixParent;
                        }
                        else
                        {
                            if (node->right->color == _rb_color::black)
                            {
                                node->left->color = _rb_color::black;
                                node->color = _rb_color::red;
                                Rrotate(node);
                                node = fixParent->right;
                            }

                            node->color = fixParent->color;
                            fixParent->color = _rb_color::black;
                            node->right->color = _rb_color::black;
                            Lrotate(fixParent);
                            break;
                        }
                    }
                    else
                    {
                        node = fixParent->left;
                        if (node->color == _rb_color::red)
                        {
                            node->color = _rb_color::black;
                            fixParent->color = _rb_color::red;
                            Rrotate(fixParent);
                            node = fixParent->left;
                        }

                        if (node->nil)
                        {
                            fixNode = fixParent;
                        }
                        else if (node->right->color == _rb_color::black &&
                            node->left->color == _rb_color::black)
                        {
                            node->color = _rb_color::red;
                            fixNode = fixParent;
                        }
                        else
                        {
                            if (node->left->color == _rb_color::black)
                            {
                                node->right->color = _rb_color::black;
                                node->color = _rb_color::red;
                                Lrotate(node);
                                node = fixParent->left;
                            }

                            node->color = fixParent->color;
                            fixParent->color = _rb_color::black;
                            node->left->color = _rb_color::black;
                            Rrotate(fixParent);

                            break;
                        }
                    }
                }

                fixNode->color = _rb_color::black;
            }

            _alloc.destroy(&eraseNode->val);
            _alloc.deallocate(eraseNode);

            assert(_val._size);
            --_val._size;

            return iterator(it._node);
        }

        iterator erase(const_iterator first, const_iterator last)
        {
            if (first == begin() && last == end())
            {
                clear();
                return begin();
            }
            else
            {
                while (first != last)
                    erase(first++);
                return iterator(first._node);
            }
        }

        size_type erase(const key_type& key)
        {
            iterator it = find(key);
            if (it == end())
                return 0;

            erase(it);
            return 1;
        }

        void clear()
        {
            Erase(Root());
            Root() = Head();
            Lmost() = Head();
            Rmost() = Head();
            _val._size = 0;
        }

        iterator find(const key_type& key)
        {
            iterator it(Lbound(key));
            return (it == end()) || _camp(key, Key(it._node)) ?
                end() : it;
        }

        const_iterator find(const key_type& key) const
        {
            const_iterator it(Lbound(key));
            return (it == cend()) || _camp(key, Key(it._node)) ?
                cend() : it;
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
            for (_node_ptr balance = node; balance->parent->color == _rb_color::red;)
            {
                if (balance->parent == balance->parent->parent->left)
                {
                    _node_ptr n_parent = balance->parent->parent->right;
                    if (n_parent->color == _rb_color::red)
                    {
                        n_parent->color = _rb_color::black;
                        balance->parent->color = _rb_color::black;
                        balance->parent->parent->color = _rb_color::red;
                        balance = balance->parent->parent;
                    }
                    else
                    {
                        if (balance == balance->parent->right)
                        {
                            balance = balance->parent;
                            Lrotate(balance);
                        }

                        balance->parent->color = _rb_color::black;
                        balance->parent->parent->color = _rb_color::red;
                        Rrotate(balance->parent->parent);
                    }
                }
                else
                {
                    _node_ptr n_parent = balance->parent->parent->left;
                    if (n_parent->color == _rb_color::red)
                    {
                        n_parent->color = _rb_color::black;
                        balance->parent->color = _rb_color::black;
                        balance->parent->parent->color = _rb_color::red;
                        balance = balance->parent->parent;
                    }
                    else
                    {
                        if (balance == balance->parent->left)
                        {
                            balance = balance->parent;
                            Rrotate(balance);
                        }

                        balance->parent->color = _rb_color::black;
                        balance->parent->parent->color = _rb_color::red;
                        Lrotate(balance->parent->parent);
                    }
                }
            }

            Root()->color = _rb_color::black;
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
        _node_ptr Buynode_if_nil(_node_ptr node, _Ty&&) { return node; }

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
            node->color = _rb_color::red;
            node->nil = false;

            _alloc.construct(&node->val, std::forward<_Ty>(val)...);
            return node;
        }

        _node_ptr Max(_node_ptr node)
        {
            return _tree_val::Max(node);
        }

        _node_ptr Min(_node_ptr node)
        {
            return _tree_val::Min(node);
        }

        void Erase(_node_ptr root)
        {
            for (_node_ptr node = root; !node->nil; root = node)
            {
                Erase(node->right);
                node = node->left;

                _alloc.destroy(&root->val);
                _alloc.deallocate(root);
            }
        }

        template <class _Key>
        _node_ptr Lbound(const _Key& key) const
        {
            _node_ptr node = Root();
            _node_ptr head = Head();

            while (!node->nil)
            {
                if (_camp(Key(node), key))
                {
                    node = node->right;
                }
                else
                {
                    head = node;
                    node = node->left;
                }
            }
            return head;
        }
    protected:
        scoped_buffer _buffer;
        allocator _alloc;
        _tree_val _val;
        key_compare _camp;
    };
}

NAMESPACE_ZH_END
