/*
 * scope guard
 * guard resources or roll back initialize operators
 * xuantao, 2018
*/
#pragma once

#include <cassert>

namespace detail
{
    struct roll_back_node
    {
        virtual ~roll_back_node() {}
        virtual void execute() = 0;
        virtual size_t size() const = 0;

        roll_back_node* prev = nullptr;
    };

    template <typename Fy>
    struct roll_back_executor : roll_back_node
    {
        typedef roll_back_executor<Fy> _MyType;

        roll_back_executor(const Fy& f) : func(f) {}
        roll_back_executor(Fy&& f) : func(std::forward<Fy>(f)) {}
        ~roll_back_executor() {}

        void execute() { func(); }
        size_t size() const { return sizeof(_MyType); }

        Fy func;
    };

    template <size_t _Capacity>
    class _allocator
    {
    public:
        constexpr size_t capacity() const { return _Capacity; }

        void* allocate(size_t s)
        {
            if (_pos + s > capacity())
                return nullptr;

            void* p = &_data[_pos];
            _pos += s;
            return p;
        }

        void deallocate(void* p, size_t s)
        {
        }

    protected:
        char _data[_Capacity];
        int _pos = 0;
    };

    template <size_t _Bsize>
    class guard_allocator
    {
        enum { BlockSize = _Bsize };

        struct alloc_node
        {
            void* allocate(size_t s)
            {
                if (_pos + s > BlockSize)
                    return nullptr;

                void* p = &_data[_pos];
                _pos += s;
                return p;
            }

            void deallocate(void* p, size_t s)
            {
            }

            alloc_node* prev = nullptr;
        private:
            char _data[BlockSize];
            int _pos = 0;
        };

    public:
        guard_allocator()
        {
            _alloc = &_default;
        }

        ~guard_allocator()
        {
            while (_alloc->prev)
            {
                auto tmp = _alloc;
                _alloc = _alloc->prev;
                delete tmp;
            }
        }

        void* allocate(size_t s)
        {
            if (s >= BlockSize)
                return new char[s];

            void* p = nullptr;
            do
            {
                p = _alloc->allocate(s);
                if (p == nullptr)
                {
                    auto tmp = new alloc_node();
                    tmp->prev = _alloc;
                    _alloc = tmp;
                }
            } while (p == nullptr);

            return p;
        }

        void deallocate(void* p, size_t s)
        {
            if (s >= _Bsize)
                delete p;
        }

    private:
        alloc_node _default;
        alloc_node* _alloc = nullptr;
    };

    template <>
    class guard_allocator<0>
    {
    public:
        enum { BlockSize = 0 };
        typedef guard_allocator<0> _MyType;

    public:
        guard_allocator() {}
        guard_allocator(const _MyType&) = delete;
        _MyType& operator = (const _MyType&) = delete;

    public:
        void* allocate(size_t s)
        {
            return new char[s];
        }

        void deallocate(void* p, size_t s)
        {
            delete p;
        }
    };
}

template <size_t _BlockSize = 512>
class scope_guard
{
public:
    enum { BlockSize = _BlockSize };
    typedef detail::guard_allocator<_BlockSize> allocator;

public:
    scope_guard() {}
    ~scope_guard()
    {
        if (!_dismiss)
            roll_back();
        clear();
    }

    scope_guard(const scope_guard&) = delete;
    scope_guard& operator = (const scope_guard&) = delete;

public:
    template <typename Fy>
    void append(const Fy& func)
    {
        typedef detail::roll_back_executor<typedef std::remove_const<Fy>::type> executor;
        auto p = (executor*)_alloc.allocate(sizeof(executor));
        assert(p);

        new (p) executor(func);
        push(p);
    }

    template <typename Fy>
    void append(Fy&& func)
    {
        typedef detail::roll_back_executor<Fy> executor;
        auto p = (executor*)_alloc.allocate(sizeof(executor));
        assert(p);

        new (p) executor(std::forward<Fy>(func));
        push(p);
    }

    void dissmis()
    {
        _dismiss = true;
    }

private:
    void push(detail::roll_back_node* node)
    {
        node->prev = _tail;
        _tail = node;
    }

    void roll_back()
    {
        auto node = _tail;
        while (node)
        {
            node->execute();
            node = node->prev;
        }
    }

    void clear()
    {
        auto node = _tail;
        _tail = nullptr;

        while (node)
        {
            auto cur = node;
            auto size = node->size();
            node = cur->prev;

            cur->~roll_back_node();
            _alloc.deallocate(cur, size);
        }
    }

private:
    bool _dismiss = false;
    detail::roll_back_node* _tail = nullptr;
    allocator _alloc;
};
