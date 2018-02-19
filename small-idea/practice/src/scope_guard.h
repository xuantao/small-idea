/*
 * scope guard
 * guard resources or roll back initialize operators
 * xuantao, 2018
*/
#pragma once

#include "common.h"

NAMESPACE_BEGIN

namespace scope_guard_detail
{
    template <typename Ty>
    struct node_base
    {
        Ty* next = nullptr;
    };

    struct caller : node_base<caller>
    {
        virtual ~caller() {}
        virtual void call() = 0;
        virtual size_t size() const = 0;
    };

    template <typename Fy>
    struct caller_impl final : caller
    {
        typedef caller_impl<Fy> _MyType;
        typedef Fy FuncType;

        caller_impl(const FuncType& fn) : _func(fn)
        { }

        caller_impl(FuncType&& fn) : _func(std::forward<FuncType>(fn))
        { }

        virtual ~caller_impl() {}

        void call() override { _func(); }
        size_t size() const override { return sizeof(_MyType); }

    private:
        FuncType _func;
    };

    template <size_t _Capacity, size_t _Align>
    struct allocator : node_base<allocator<_Capacity, _Align>>
    {
        void* allocate(size_t s)
        {
            void* p = &_data[_Capacity - _size];
            if (!std::align(_Align, s, p, _size))
                return nullptr;

            _size -= s;
            return p;
        }

        void deallocate(void* p, size_t s) { }

    protected:
        size_t _size = _Capacity;
        char _data[_Capacity];
    };

    template <size_t _Bsize>
    class scope_guard_allocator
    {
    public:
        enum { BlockSize = _Bsize, Align = sizeof(void*) };
        typedef allocator<BlockSize, Align> allocator;

    public:
        scope_guard_allocator() : _alloc(&_default)
        { }

        ~scope_guard_allocator()
        {
            while (_alloc->next)
            {
                auto tmp = _alloc;
                _alloc = _alloc->next;

                delete tmp;
            }
#ifdef LOG_DEBUG_INFO
            if (_inc_size)
                std::cerr << "scope_guard_allocator should inc size to:" << (BlockSize + _inc_size) << std::endl;
#endif // LOG_DEBUG_INFO
        }

        void* allocate(size_t s)
        {
            if (need_raw_alloc(s))
            {
#if LOG_DEBUG_INFO
                _inc_size += s;
#endif // LOG_DEBUG_INFO
                return new char[s];
            }

            void* p = _alloc->allocate(s);
            if (p != nullptr)
                return p;

            auto tmp = new allocator();
            tmp->next = _alloc;
            _alloc = tmp;

            p = _alloc->allocate(s);
            assert(p);
            return p;
        }

        void deallocate(void* p, size_t s)
        {
            if (need_raw_alloc(s))
                delete[] static_cast<char*>(p);
        }

    private:
        inline bool need_raw_alloc(size_t s) const
        {
            // ignore alignment cause alloc error
            // when s is closer BlockSize,
            // align operate may be not alloc successfully
            return (s + Align > BlockSize);
        }

    private:
        allocator* _alloc = nullptr;
        allocator _default;
#if LOG_DEBUG_INFO
        size_t _inc_size = 0;
#endif // LOG_DEBUG_INFO
    };

    template <>
    class scope_guard_allocator<0>
    {
    public:
        enum { BlockSize = 0 };

    public:
        void* allocate(size_t s)
        {
            printf("alloc   size:%2d\n", (int)s);
            return new char[s];
        }

        void deallocate(void* p, size_t s)
        {
            printf("dealloc size:%2d\n", (int)s);
            delete[] static_cast<char*>(p);
        }
    };

    template <size_t _BlockSize>
    class scope_guard
    {
    public:
        enum { BlockSize = _BlockSize };
        typedef scope_guard_allocator<_BlockSize> allocator;

    public:
        scope_guard() {}
        ~scope_guard() { done(); }

        scope_guard(const scope_guard&) = delete;
        scope_guard& operator = (const scope_guard&) = delete;

    public:
        template <typename Fy>
        void push(Fy&& func)
        {
            typedef typename std::decay<Fy>::type decay;
            typedef caller_impl<decay> caller_impl;

            auto buff = _alloc.allocate(sizeof(caller_impl));
            assert(buff);

            auto node = new (buff) caller_impl(std::forward<Fy>(func));

            node->next = _head;
            _head = node;
        }

        inline bool is_dissmissed() const { return _dismiss; }

        inline void dismiss() { _dismiss = true; }

        bool done()
        {
            if (!is_dissmissed())
                roll_back();
            clear();
            return is_dissmissed();
        }

    private:
        void roll_back()
        {
            auto node = _head;
            while (node)
            {
                node->call();
                node = node->next;
            }
        }

        void clear()
        {
            auto node = _head;
            _head = nullptr;

            while (node)
            {
                auto tmp = node;
                auto size = node->size();
                node = node->next;

                tmp->~caller();
                _alloc.deallocate(tmp, size);
            }
        }

    private:
        bool _dismiss = false;
        caller* _head = nullptr;
        allocator _alloc;
    };
}

typedef scope_guard_detail::scope_guard<512> scope_guard;

NAMESPACE_END
