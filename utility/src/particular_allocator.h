﻿/*
 * particular allocator
 * xuantao, 2017
*/
#pragma once

#include "common.h"

UTILITY_NAMESPACE_BEGIN

namespace detail
{
    /* 指定内存大小的对象 */
    template <class Ty>
    struct block_size_type
    {
        int8_t _[sizeof(Ty)];
    };

    template <>
    struct block_size_type<void>
    {};
}

/*
 * 固定大小对象内存分配器
 * 分配器持有一块内存, 将这块大的内存分割为固定大小的小块
 * 一次分配一个块内存, 当没有空间分配时返回nullptr
 * 分配和释放的时间复杂度都是O(1)
*/
template <class Ty>
class particular_allocator
{
protected:
    union _node
    {
        _node* next;
        detail::block_size_type<Ty> _;
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
    particular_allocator(void* pool, size_type pool_size)
        : _root(static_cast<_node_ptr>(pool))
        , _size(pool_size)
    {
        // build single direction list
        for (size_type i = 0, ms = max_size(); i < ms; ++i)
            _root[i].next = &_root[i + 1];

        if (max_size())
            _root[max_size() - 1].next = nullptr;
    }

    particular_allocator(particular_allocator&& other)
        : _root(other._root)
        , _size(other._size)
    {
        other._root = nullptr;
        other._size = 0;
    }

    ~particular_allocator() {}

    particular_allocator(const particular_allocator&) = delete;
    particular_allocator& operator = (const particular_allocator&) = delete;

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

        pointer p = reinterpret_cast<pointer>(_root);
        _root = _root->next;

        return p;
    }

    void deallocate(pointer ptr)
    {
        _node_ptr node = reinterpret_cast<_node_ptr>(ptr);
        node->next = _root;
        _root = node;
    }

    inline size_type max_size() const
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
}; // class particular_allocator

/* fixed particular allocator */
template <typename Ty, size_t N>
class fixed_particular_allocator : public particular_allocator<Ty>
{
public:
    typedef particular_allocator<Ty> base_type;

public:
    fixed_particular_allocator() : base_type(_pool, N)
    { }

private:
    int8_t _pool[N];
}; // class fixed_particular_allocator


UTILITY_NAMESPACE_END
