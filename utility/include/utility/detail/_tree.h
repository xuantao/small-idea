/*
 * tree, a simple red black tree
 * xuantao, 2017
*/
#pragma once

#include "../particular_allocator.h"
#include "../scope_buffer.h"
#include <utility>
#include <iterator>

UTILITY_NAMESPACE_BEGIN

namespace detail
{
    enum class _rb_color : bool
    {
        red, black,
    };

    struct _tree_nil {};

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
    struct _tree_val
    {
        typedef _tree_val<Ty> _my_type;
        typedef Ty value_type;
        typedef Ty* pointer;
        typedef const Ty* const_pointer;
        typedef Ty& reference;
        typedef const Ty& const_reference;
        typedef _tree_node<Ty> _node;
        typedef _node* _node_ptr;
        typedef size_t size_type;
        typedef std::ptrdiff_t difference_type;

        _tree_val() : head(nullptr), size(0)
        {
        }

        _tree_val(_my_type&& other) : head(other._head), size(other._size)
        {
            other._head = nullptr;
            other._size = 0;
        }

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

        _node_ptr head;
        size_type size;
    };

    template <class Val>
    class _tree_const_iterator
        : public std::iterator<std::bidirectional_iterator_tag, typename Val::value_type>
    {
    public:
        typedef _tree_const_iterator<Val> _my_iter;
        typedef const Val* _val_ptr;
        typedef typename Val::_node_ptr _node_ptr;
        typedef typename Val::const_pointer pointer;
        typedef typename Val::const_reference reference;

    public:
        _tree_const_iterator() : _val(nullptr), _node(nullptr) {}
        _tree_const_iterator(_val_ptr val, _node_ptr node) : _val(val), _node(node)
        {
        }

        reference operator * () const
        {
            assert(_val && _node && !_node->nil);
            return _node->val;
        }

        pointer operator -> () const
        {
            return &(**this);
        }

        _my_iter& operator ++ ()
        {
            assert(_val && _node);

            if (_node->nil)
                return *this;

            if (!_node->right->nil)
            {
                _node = Val::Min(_node->right);
            }
            else
            {
                _node_ptr tmp = _node->parent;
                while (!tmp->nil && _node == tmp->right)
                {
                    _node = tmp;
                    tmp = _node->parent;
                }

                _node = tmp;
            }
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
            assert(_val && _node);

            if (_node->nil)
            {
                _node = _node->right;
            }
            else if (!_node->left->nil)
            {
                _node = Val::Max(_node->left);
            }
            else
            {
                _node_ptr tmp = _node->parent;
                while (!tmp->nil && _node == tmp->left)
                {
                    _node = tmp;
                    tmp = _node->parent;
                }

                if (!_node->nil)
                    _node = tmp;
            }
            return *this;
        }

        _my_iter operator -- (int)
        {
            _my_iter tmp = *this;
            --*this;
            return tmp;
        }

        bool operator == (const _my_iter& other) const
        {
            assert(_val == other._val && _node && other._node);
            return _node == other._node;
        }

        bool operator != (const _my_iter& other) const
        {
            return !(*this == other);
        }

    public:
        const _val_ptr _Val() const { return _val; }
        const _node_ptr _Node() const { return _node; }

    protected:
        _val_ptr _val;
        _node_ptr _node;
    };

    template <class Val>
    class _tree_iterator : public _tree_const_iterator<Val>
    {
    public:
        typedef _tree_iterator<Val> _my_iter;
        typedef _tree_const_iterator<Val> _my_base;
        typedef typename _my_base::_val_ptr _val_ptr;
        typedef typename _my_base::_node_ptr _node_ptr;

        typedef typename Val::pointer pointer;
        typedef typename Val::reference reference;

    public:
        _tree_iterator() {}
        _tree_iterator(_val_ptr val, _node_ptr node) : _my_base(val, node) {}

        reference operator*() const
        {
            return ((reference)**(_my_base *)this);
        }

        pointer operator->() const
        {
            return (std::pointer_traits<pointer>::pointer_to(**this));
        }

        _my_iter& operator++()
        {
            ++static_cast<_my_base&>(*this);
            return (*this);
        }

        _my_iter operator++(int)
        {
            _my_iter tmp = *this;
            ++*this;
            return tmp;
        }

        _my_iter& operator--()
        {
            --static_cast<_my_base&>(*this);
            return (*this);
        }

        _my_iter operator--(int)
        {
            _my_iter tmp = *this;
            --*this;
            return tmp;
        }
    };

    template <class Traits>
    class _tree
    {
    public:
        typedef _tree<Traits> _my_tree;

        typedef typename Traits::key_type key_type;
        typedef typename Traits::value_type value_type;
        typedef typename Traits::key_compare key_compare;
        typedef typename Traits::value_compare value_compare;

        typedef _tree_val<value_type> _val_type;
        typedef typename _val_type::_node _node;
        typedef typename _val_type::_node_ptr _node_ptr;
        typedef typename _val_type::pointer pointer;
        typedef typename _val_type::const_pointer const_pointer;
        typedef typename _val_type::reference reference;
        typedef typename _val_type::const_reference const_reference;
        typedef typename _val_type::size_type size_type;
        typedef typename _val_type::difference_type difference_type;

        typedef particular_allocator<_node> allocator;
        typedef _tree_iterator<_val_type> iterator;
        typedef _tree_const_iterator<_val_type> const_iterator;
        typedef std::pair<iterator, bool> pairib;

    public:
        static const size_type element_size = sizeof(_node);
        static size_type buffer_size(size_type count) { return allocator::element_size * (count + 1); }

    public:
        _tree(scoped_buffer&& buffer)
            : _buffer(std::forward<scoped_buffer>(buffer))
            , _alloc(_buffer.get(), _buffer.size())
        {
            _node_ptr node = _alloc.allocate();
            if (node)
            {
                node->left = node;
                node->right = node;
                node->parent = node;
                node->color = _rb_color::black;
                node->nil = true;
            }

            Head() = node;
        }

        _tree(_my_tree&& other)
            : _buffer(std::move(other._buffer))
            , _alloc(std::move(other._alloc))
            , _val(std::move(other._val))
        {
        }

        ~_tree()
        {
            if (_val.head)
                erase(begin(), end());
        }

    protected:
        _tree(const _my_tree& other)
        {
            static_assert(false, "not allow copy");
        }

        _my_tree& operator = (const _my_tree& other)
        {
            static_assert(false, "not allow copy");
        }

    public:
        iterator begin()
        {
            return iterator(&_val, Lmost());
        }
        iterator end()
        {
            return iterator(&_val, Head());
        }

        const_iterator cbegin() const
        {
            return const_iterator(&_val, Lmost());
        }

        const_iterator cend() const
        {
            return const_iterator(&_val, Head());
        }
    public:
        // capacity
        bool empty() const { return size() == 0; }
        size_type size() const { return _val.size; }
        size_type max_size() const { return _alloc.max_size() - 1; }

    public:
        pairib insert(const value_type& val)
        {
            return Insert_Nohint(false, val, _tree_nil());
        }

        pairib insert(value_type&& val)
        {
            return Insert_Nohint(false, std::forward<value_type>(val), _tree_nil());
        }

        iterator erase(const_iterator it)
        {
            assert(it._Val() == &_val);
            assert(it._Node());
            assert(it != end());

            _node_ptr eraseNode = it._Node();
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
                node = it._Node();
                fixNode = node->right;
            }

            if (node == eraseNode)
            {
                fixParent = eraseNode->parent;
                if (!fixNode->nil)
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
                        else if (node->left->color == _rb_color::black &&
                            node->right->color == _rb_color::black)
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

            if (_val.size)
                --_val.size;

            return iterator(&_val, it._Node());
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
                return iterator(&_val, first._Node());
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
            _val.size = 0;
        }

        iterator find(const key_type& key)
        {
            iterator it(&_val, Lbound(key));
            return (it == end()) || _camp(key, Key(it._Node())) ?
                end() : it;
        }

        const_iterator find(const key_type& key) const
        {
            const_iterator it(&_val, Lbound(key));
            return (it == cend()) || _camp(key, Key(it._Node())) ?
                cend() : it;
        }

    protected:
        template <class _Ty, class _Node>
        pairib Insert_Nohint(bool leftish, _Ty&& val, _Node newNode)
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

            iterator it = iterator(&_val, node);
            if (!addLeft)
                ;
            else if (it == begin())
                return pairib(Insert_at(true, node, std::forward<_Ty>(val), newNode), true);
            else
                --it;

            if (_camp(Key(it._Node()), Kfn(val)))
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
        iterator Insert_Hint(const_iterator it, _Ty&& val, _Node newNode)
        {
            const_iterator next;
            bool leftish = false;
            assert(it._Val() == &_val);

            if (empty())
                return Insert_at(true, Head(), std::forward<_Ty>(val), newNode);

            if (it == begin())
            {
                if (_camp(Kfn(val), Key(it._Node())))
                    return Insert_at(true, it._Node(), std::forward<_Ty>(val), newNode);
            }
            else if (it == end())
            {
                if (_camp(Key(Rmost()), Kfn(val)))
                    return Insert_at(false, Rmost(), std::forward<_Ty>(val), newNode);
            }
            else if (_camp(Kfn(val), Key(it._Node())) &&
                _camp(Key((--(next = it))._Node()), Kfn(val)))
            {
                if (next._Node()->right->nil)
                    return Insert_at(false, next._Node(), std::forward<_Ty>(val), newNode);
                else
                    return Insert_at(true, it._Node(), std::forward<_Ty>(val), newNode);
            }
            else if (_camp(Key(it._Node()), Kfn(val)) &&
                ((++(next = it)) == end() || _camp(Kfn(val), Key(next._Node()))))
            {
                if (it._Node()->right->nil)
                    return Insert_at(false, it._Node(), std::forward<_Ty>(val), newNode);
                else
                    return Insert_at(true, next._Node(), std::forward<_Ty>(val), newNode);
            }

            return Insert_Nohint(leftish, std::forward<_Ty>(val), newNode).first;
        }

        template <class _Ty, class _Node>
        iterator Insert_at(bool addLeft, _node_ptr parent, _Ty&& val, _Node newNode)
        {
            assert(max_size() - 1 >= size());

            _node_ptr node = Buynode_if_nil(newNode, std::forward<_Ty>(val));

            ++_val.size;
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
            return iterator(&_val, node);
        }

        template <class... _Ty>
        iterator Emplace_Hint(const_iterator it, _Ty&&... val)
        {
            _node_ptr node = Buynode(std::forward<_Ty>(val)...);
            return Insert_Hint(it, node->val, node);
        }

    protected:
        const key_type& Kfn(const value_type& _Val) const { return (Traits::Kfn(_Val)); }
        const key_type& Key(_node_ptr node) const { return (const key_type&)Kfn(node->val); }

        _node_ptr& Root() { return _val.head->parent; }
        const _node_ptr& Root() const { return _val.head->parent; }
        _node_ptr& Head() { return _val.head; }
        const _node_ptr& Head() const { return _val.head; }
        _node_ptr& Lmost() { return _val.head->left; }
        const _node_ptr& Lmost() const { return _val.head->left; }
        _node_ptr& Rmost() { return _val.head->right; }
        const _node_ptr& Rmost() const { return _val.head->right; }

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

        template<class... _Ty>
        _node_ptr Buynode(_Ty&&... val)
        {
            _node_ptr node = _alloc.allocate();
            node->left = Head();
            node->right = Head();
            node->parent = Head();
            node->color = _rb_color::red;
            node->nil = false;

            _alloc.construct(&node->val, std::forward<_Ty>(val)...);
            return node;
        }

        _node_ptr Max(_node_ptr node)
        {
            return _val_type::Max(node);
        }

        _node_ptr Min(_node_ptr node)
        {
            return _val_type::Min(node);
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
        key_compare _camp;
        _val_type _val;
    };
}

UTILITY_NAMESPACE_END
