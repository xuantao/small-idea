/*
 * scope guard
 * guard resources or roll back initialize operators
 * xuantao, 2018
*/
#pragma once

namespace detail
{
    struct roll_back_node
    {
        virtual ~roll_back_node() {}
        virtual void execute() = 0;
        virtual size_t size() = 0;

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
}

template <typename Alloc>
class scope_guard
{
public:
    typename Alloc allocator;

public:
    scope_guard() {}
    scope_guard(allocator& alloc) : _alloc(alloc) {}
    scope_guard(allocator&& alloc) : _alloc(std::forward<allocator>(alloc)) {}

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
        typedef detail::roll_back_executor<Fy> executor;
        auto p = (executor*)_alloc.allocate(sizeof(executor));
        assert(p);

        new p executor(func);
        push(p);
    }

    template <typename Fy>
    void append(Fy&& func)
    {
        typedef detail::roll_back_executor<Fy> executor;
        auto p = (executor*)_alloc.allocate(sizeof(executor));
        assert(p);

        new p executor(std::forward<Fy>(func));
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
        while (node)
        {
            auto cur = node;
            node = cur->prev;

            cur->~roll_back_node();
            _alloc.deallocate(cur, cur->size());
        }
    }

private:
    bool _dismiss = false;
    detail::roll_back_node* _tail = nullptr;
    allocator _alloc;
};
