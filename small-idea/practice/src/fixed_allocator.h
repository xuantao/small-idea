/*
 * fixed buffer allocator
 * xuantao, 2017
*/
#pragma once

#include <cstdint>
#include "def.h"

NAMESPACE_ZH_BEGIN

/*
 * �̶���С�����ڴ������
 * ����������һ���ڴ�, ��������ڴ�ָ�Ϊ�̶���С��С��
 * һ�η���һ�����ڴ�, ��û�пռ����ʱ����nullptr
 * ������ͷŵ�ʱ�临�Ӷȶ���O(1)
*/
template <class Ty>
class fixed_allocator
{
protected:
    union _node
    {
        _node* next;
        uint8_t _[sizeof(Ty)];
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
