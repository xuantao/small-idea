/*
 * stack allocator
 * xuantao, 2017
*/
#pragma once

#include "scope_buffer.h"
#include "detail/alloc_internal.h"

UTILITY_NAMESPACE_BEGIN

/* stack allocator */
template <size_t A = sizeof(void*)>
class StackedAllocator : private Alloc_Internal::AllocCtrlChain<Alloc_Internal::StackedAllocCtrl<A>>
{
public:
    using AllocCtrl = Alloc_Internal::StackedAllocCtrl<A>;
    using BaseType = Alloc_Internal::AllocCtrlChain<AllocCtrl>;
    static constexpr size_t DefaultBlock = 4096;

public:
    StackedAllocator() : BaseType(DefaultBlock)
    {
    }

    StackedAllocator(size_t block_size) : BaseType(block_size)
    {
    }

    ~StackedAllocator()
    {
    }

    StackedAllocator(const StackedAllocator&) = delete;
    StackedAllocator& operator = (const StackedAllocator&) = delete;

public:
    ScopeBuffer Alloc(size_t size)
    {
        void* buff = BaseType::Alloc(size);
        return ScopeBuffer(buff, size, Deallocator{ this });
    }

private:
    void Dealloc(void* p, size_t size)
    {
        auto& alloc_node = this->alloc_node_;
        auto& ctrl = alloc_node.val;
        ctrl.Dealloc(p, size);

        if (ctrl.Empty() && alloc_node.next_node)
        {
            auto* empty_node = alloc_node.next_node;
            alloc_node.next_node = empty_node->next_node;
            ctrl.Swap(empty_node->val);

            empty_node->next_node = this->empty_head_;
            this->empty_head_ = empty_node;
        }
    }

    struct Deallocator
    {
        StackedAllocator<A>* allocator_;
        void operator() (void* p, size_t size) { allocator_->Dealloc(p, size); }
    };
};

UTILITY_NAMESPACE_END
