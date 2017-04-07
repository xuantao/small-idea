/*
 * fixed buffer allocator
 * xuantao, 2017
*/

#pragma once

#include <cstdint>

template <class Ty>
class fixed_allocator
{
protected:
    union node
    {
        node* next;
        uint8_t _[sizeof(Ty)];
    };
public:
    typedef Ty value_type;
public:
    fixed_allocator(void* pool, size_t size)
        : _size(size), _root(static_cast<node*>(pool))
    {
        node* pnode = _root;
        size_t step = sizeof(node);

        for (size_t pos = step; pos < _size; pos += step)
        {
            void* p = &static_cast<uint8_t*>(pool)[pos];
            pnode->next = static_cast<node*>(p);
            pnode = pnode->next;
        }

        if (pnode)
            pnode->next = nullptr;
    }

    fixed_allocator(fixed_allocator&& other)
        : _size(other._size), _root(other._root)
    {
    }

    fixed_allocator(const fixed_allocator&)
    {
        static_assert(false, "this allocator can not been copy");
    }

    ~fixed_allocator()
    {
    }

    fixed_allocator& operator = (const fixed_allocator&)
    {
        static_assert(false, "this allocator can not been assigned");
        return *this;
    }

public:
    Ty* address(Ty& ref) const
    {
        return (std::addressof(ref));
    }

    const Ty* address(const Ty& ref) const
    {
        return (std::addressof(ref));
    }

    Ty* allocate()
    {
        if (_root == nullptr)
            return nullptr;

        Ty* alloc = reinterpret_cast<Ty*>(_root);
        _root = _root->next;

        return alloc;
    }

    void deallocate(Ty* ptr)
    {
        node* pnode = reinterpret_cast<node*>(ptr);
        pnode->next = _root;
        _root = pnode;
    }

    size_t max_size() const
    {
        return _size / sizeof(node);
    }

    template<class Obj, class... Args>
    void construct(Obj *ptr, Args&&... args)
    {
        ::new ((void *)ptr) Obj(std::forward<Args>(args)...);
    }

    template<class Obj>
    void destroy(Obj* ptr)
    {
        ptr->~Obj();
    }

protected:
    size_t  _size;
    node*   _root;
};

template <class Ty>
class custom_allocator_adapter : public fixed_allocator<Ty>
{
public:
    typedef fixed_allocator<Ty> my_base;
public:
    custom_allocator_adapter(void* pool, size_t size)
        : fixed_allocator(pool, size)
    {
    }

    custom_allocator_adapter(custom_allocator_adapter&& other)
        : fixed_allocator(std::forward<my_base>(other))
    {
    }

    //custom_allocator_adapter(custom_allocator_adapter& other)
    //    : fixed_allocator(nullptr, 0)
    //{
    //}

    custom_allocator_adapter(const custom_allocator_adapter&)
        : fixed_allocator(nullptr, 0)
    {
        //static_assert(false, "this allocator can not been copy");
    }

    custom_allocator_adapter& operator = (const custom_allocator_adapter&)
    {
        //static_assert(false, "this allocator can not been assigned");
    }

public:
    Ty* allocate(size_t count)
    {
        assert(count == 1);
        return my_base::allocate();
    }

    Ty* allocate(size_t count, const void*)
    {
        return allocate(count);
    }

    void deallocate(Ty* ptr, size_t count)
    {
        assert(count == 1);
        my_base::deallocate(ptr);
    }
};
