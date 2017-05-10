/*
 * fixed buffer allocator
 * xuantao, 2017
*/
#pragma once

#include <cstdint>
#include "def.h"

NAMESPACE_ZH_BEGIN

/*
 * 指定内存大小的对象
*/
template <class Ty>
struct block_size_type
{
    int8_t _[sizeof(Ty)];
};

template <>
struct block_size_type<void>
{};

/*
 * 固定大小对象内存分配器
 * 分配器持有一块内存, 将这块大的内存分割为固定大小的小块
 * 一次分配一个块内存, 当没有空间分配时返回nullptr
 * 分配和释放的时间复杂度都是O(1)
*/
template <class Ty>
class fixed_allocator
{
protected:
    union _node
    {
        _node* next;
        block_size_type<Ty> _;
    };
    typedef _node* _node_ptr;
public:
    typedef Ty value_type;
    typedef Ty* pointer;
    typedef Ty& reference;
    typedef const Ty* const_pointer;
    typedef const Ty& const_reference;
    typedef size_t size_type;

public:
    static const size_type element_size = sizeof(_node);

public:
    fixed_allocator(void* pool, size_type size)
        : _size(size), _root(static_cast<_node_ptr>(pool))
    {
        // build single direction list
        for (size_type i = 0; i < max_size(); ++i)
            _root[i].next = &_root[i + 1];

        if (max_size())
            _root[max_size() - 1].next = nullptr;
    }

    fixed_allocator(fixed_allocator&& other)
        : _size(other._size), _root(other._root)
    {
        other._size = 0;
        other._root = nullptr;
    }

    fixed_allocator(const fixed_allocator&)
    {
        static_assert(false, "this allocator can not been copy");
    }

    ~fixed_allocator() {}

    fixed_allocator& operator = (const fixed_allocator&)
    {
        static_assert(false, "this allocator can not been assign copy");
        return *this;
    }

public:
    pointer address(reference ref) const
    {
        return (std::addressof(ref));
    }

    const_pointer address(const_reference ref) const
    {
        return (std::addressof(ref));
    }

    pointer allocate()
    {
        if (_root == nullptr)
            return nullptr;

        pointer node = reinterpret_cast<pointer>(_root);
        _root = _root->next;

        return node;
    }

    void deallocate(Ty* ptr)
    {
        _node_ptr node = reinterpret_cast<_node_ptr>(ptr);
        node->next = _root;
        _root = node;
    }

    size_type max_size() const
    {
        return _size / element_size;
    }

    template<class _Ty, class... Args>
    void construct(_Ty* ptr, Args&&... args)
    {
        ::new ((void *)ptr) _Ty(std::forward<Args>(args)...);
    }

    template<class _Ty>
    void destroy(_Ty* ptr)
    {
        ptr->~_Ty();
    }

protected:
    _node* _root;
    size_type _size;
};

NAMESPACE_ZH_END
