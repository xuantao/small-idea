/*
 * scope guard
 * guard resources or roll back initialize operators
 * xuantao, 2018
*/
#pragma once

#include "common.h"
#include "serial_allocator.h"
#include <utility>

UTILITY_NAMESPACE_BEGIN

namespace ScopeGuard_Internal
{
    struct Caller
    {
        Caller(size_t s) : size(size) {}

        virtual ~Caller() {}
        virtual void Do() = 0;

        Caller* next = nullptr;
        size_t size;
    };

    template <typename Fty>
    struct CallerImpl final : Caller
    {
        CallerImpl(Fty&& fn) : Caller(sizeof(CallerImpl)), func(std::forward<Fty>(fn))
        { }

        virtual ~CallerImpl() { }
        void Do() override { func(); }

        Fty func;
    };

    template <typename Ty, size_t N, size_t A = sizeof(void*)>
    class Allocator
    {
    public:
        typedef Ty                  value_type;
        typedef value_type*         pointer;
        typedef const value_type*   const_pointer;
        typedef value_type&         reference;
        typedef const value_type&   const_reference;
        typedef std::size_t         size_type;
        typedef std::ptrdiff_t      difference_type;

        template <typename U>
        struct rebind {
            typedef Allocator<U, N, A> other;
        };

    public:
        Allocator() : alloc_(N)
        {
        }

        ~Allocator()
        {
        }

        Allocator(const Allocator&) = delete;
        Allocator& operator = (const Allocator&) = delete;

    public:
        pointer allocate(size_t size)
        {
            return reinterpret_cast<pointer>(alloc_.Alloc(sizeof(value_type) * size));
        }

        void deallocate(pointer p, size_t size)
        {
            alloc_.Dealloc(p, size);
        }

    private:
        PooledSerialAlloc<N, A> alloc_;
    };
} // namespace detail

/* Guard Stack */
template <class Alloc = ScopeGuard_Internal::Allocator<int8_t, 512>>
class ScopeGuard
{
public:
    typedef typename std::allocator_traits<Alloc>::template rebind_alloc<int8_t> Allocator;
    typedef ScopeGuard_Internal::Caller Caller;

public:
    ScopeGuard()
    {
    }

    template <typename _Alloc>
    ScopeGuard(_Alloc&& alloc) : alloc_(std::forward<_Alloc>(alloc))
    {
    }

    ~ScopeGuard()
    {
        Done();
    }

    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator = (const ScopeGuard&) = delete;

public:
    template <typename Fy>
    void Push(Fy&& func)
    {
        typedef ScopeGuard_Internal::CallerImpl<Fy> CallerImpl;

        auto buff = alloc_.allocate(sizeof(CallerImpl));
        assert(buff);

        auto node = new (buff) CallerImpl(std::forward<Fy>(func));
        node->next = head_;
        head_ = node;
    }

    inline bool IsDissmiss() const
    {
        return dismiss_;
    }

    inline void Dismiss()
    {
        dismiss_ = true;
    }

    bool Done()
    {
        if (!IsDissmiss())
            Rollback();

        Clear();
        return IsDissmiss();
    }

private:
    void Rollback()
    {
        auto node = head_;
        while (node)
        {
            node->Do();
            node = node->next;
        }
    }

    void Clear()
    {
        auto node = head_;
        head_ = nullptr;

        while (node)
        {
            auto tmp = node;
            auto size = node->size;
            node = node->next;

            tmp->~Caller();
            alloc_.deallocate(reinterpret_cast<int8_t*>(tmp), size);
        }
    }

private:
    bool dismiss_ = false;
    Caller* head_ = nullptr;
    Allocator alloc_;
};

UTILITY_NAMESPACE_END
