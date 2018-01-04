/*
 * scope guard
 * guard resources or roll back initialize operaters
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

        roll_back_node* next = nullptr;
    };

    template <typename Fy>
    struct roll_back_executor : roll_back_node
    {
        typedef roll_back_executor<Fy> _MyType;

        roll_back_executor(const Fy& f) : func(f) {}
        roll_back_executor(Fy&& f) : func(std::forward<Fy>(f)) {}
        ~roll_back_executor() { }

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

    ~scope_guard() { if (!_dismiss) roll_back(); }

public:
    template <typename Fy>
    void Append(const Fy& func)
    {
        auto* p = (detail::roll_back_executor<Fy>*)_alloc.allocate(sizeof(detail::roll_back_executor<Fy>));
        assert(p);

        new p detail::roll_back_executor(func);
        p->next = _node;
        _node = p;
    }

    template <typename Fy>
    void Append(Fy&& func)
    {
        auto* p = (detail::roll_back_executor<Fy>*)_alloc.allocate(sizeof(detail::roll_back_executor<Fy>));
        assert(p);

        new p detail::roll_back_executor(func);
        p->next = _node;
        _node = p;
    }

    void Dissmis()
    {
        _dismiss = true;
    }

private:
    void roll_back()
    {
        auto node = _node;
        _node = nullptr;

        while (node)
        {
            auto cur = node;
            node = node->next;

            cur->execute();
            cur->~roll_back_node();
            _alloc.dealloc(cur, cur->size());
        }
    }

private:
    bool _dismiss = false;
    detail::roll_back_node* _node = nullptr;
    allocator _alloc;
};
