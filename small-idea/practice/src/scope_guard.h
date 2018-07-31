/*
 * scope guard
 * guard resources or roll back initialize operators
 * xuantao, 2018
*/
#pragma once

#include "common.h"
#include "serial_allocator.h"

UTILITY_NAMESPACE_BEGIN

namespace scope_guard_detail
{
    struct caller
    {
        caller(size_t s) : size(size) {}

        virtual ~caller() {}
        virtual void call() = 0;

        caller* next = nullptr;
        size_t size;
    };

    template <typename Fy>
    struct caller_impl final : caller
    {
        typedef caller_impl<Fy> _MyType;
        typedef Fy FuncType;

        caller_impl(const FuncType& fn)
            : caller(sizeof(caller_impl)), _func(fn)
        { }

        caller_impl(FuncType&& fn)
            : caller(sizeof(caller_impl)), _func(std::forward<FuncType>(fn))
        { }

        virtual ~caller_impl() {}

        void call() override { _func(); }

    private:
        FuncType _func;
    };

    template <size_t _Bsize>
    class scope_guard_allocator
    {
    public:
        enum { BlockSize = _Bsize, Align = sizeof(void*) };

        typedef fixed_serial_allocator<BlockSize, Align> serial_allocator;
        typedef singly_node<serial_allocator> node_allocator;

    public:
        scope_guard_allocator() : _alloc(&_default)
        { }

        ~scope_guard_allocator()
        {
            size_t inc_size = 0;
            while (_alloc->next)
            {
                auto tmp = _alloc;
                _alloc = _alloc->next;
                inc_size += BlockSize;
                delete tmp;
            }

#ifdef LOG_DEBUG_INFO
            if (inc_size)
                std::cerr << "scope_guard_allocator should inc size to:" << (BlockSize + inc_size) << std::endl;
#else
            (void)inc_size;
#endif // LOG_DEBUG_INFO
        }

        void* allocate(size_t s)
        {
            if (need_raw_alloc(s))
                return new char[s];

            void* p = _alloc->value.allocate(s);
            if (p != nullptr)
                return p;

            auto tmp = new node_allocator();
            tmp->next = _alloc;
            _alloc = tmp;

            p = _alloc->value.allocate(s);
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
        node_allocator* _alloc = nullptr;
        node_allocator _default;
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
} // namespace detail

template <size_t _BlockSize = 512>
class scope_guard
{
public:
    enum { BlockSize = _BlockSize };
    typedef scope_guard_detail::scope_guard_allocator<_BlockSize> allocator;

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
        typedef scope_guard_detail::caller_impl<decay> caller_impl;

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
            auto size = node->size;
            node = node->next;

            tmp->~caller();
            _alloc.deallocate(tmp, size);
        }
    }

private:
    bool _dismiss = false;
    scope_guard_detail::caller* _head = nullptr;
    allocator _alloc;
};

UTILITY_NAMESPACE_END
