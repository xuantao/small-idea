#pragma once
#include "../common.h"
#include <algorithm>
#include <cassert>

UTILITY_NAMESPACE_BEGIN

namespace Alloc_Internal
{
    /* 序列式分配器, 顺序从一块缓存中分配一段内存
     * 只分配不负责释放。当不够空间分配时返回nullptr
    */
    template <size_t A = sizeof(void*)>
    struct SerialAllocCtrl
    {
        static constexpr size_t Align = A;

        SerialAllocCtrl(void* pool, size_t cap_size)
        {
            std_ext::align(Align, 0, pool, cap_size);
            pool_ = static_cast<int8_t*>(pool);
            capacity_ = cap_size;
        }

        SerialAllocCtrl(SerialAllocCtrl&& other) : pool_(nullptr), capacity_(0)
        {
            Swap(other);
        }

        inline bool Empty() const
        {
            return alloced_ == 0;
        }

        inline size_t Size() const
        {
            return alloced_;
        }

        inline size_t Capacity() const
        {
            return capacity_;
        }

        inline void Reset()
        {
            alloced_ = 0;
        }

        void* Alloc(size_t sz)
        {
            size_t al_sz = AlignSize(sz, Align);
            if (al_sz > capacity_ - alloced_)
                return nullptr;

            int8_t* buf = pool_ + alloced_;
            alloced_ += al_sz;
            return buf;
        }

        void Swap(SerialAllocCtrl& other)
        {
            std::swap(other.alloced_, alloced_);
            std::swap(other.capacity_, capacity_);
            std::swap(other.pool_, pool_);
        }

        size_t alloced_ = 0;
        size_t capacity_;
        int8_t* pool_;
    }; // class AllocCtrl

    /* stacked allocator controler
     * first alloc last dealloc, first in lat out
    */
    template <size_t A = sizeof(void*)>
    struct StackedAllocCtrl : SerialAllocCtrl<A>
    {
        StackedAllocCtrl(void* pool, size_t capacity)
            : SerialAllocCtrl<A>(pool, capacity)
        {
        }

        void Dealloc(void* p, size_t size)
        {
            auto addr = static_cast<int8_t*>(p);
            auto align_size = AlignSize(size, SerialAllocCtrl<A>::Align);

            assert(addr >= this->pool_ && "not allocate from this object");
            assert((addr - this->pool_ == this->alloced_ - align_size) && "deallocate order not as allocated");

            this->alloced_ -= align_size;
        }
    };

    template <typename AllocCtrl>
    struct AllocCtrlChain
    {
        static constexpr size_t Align = AllocCtrl::Align;
        using AllocNode = SinglyNode<AllocCtrl>;

        AllocCtrlChain(size_t block_size)
            : alloc_node_(nullptr, 0)
            , block_size_(block_size)
        {
        }

        AllocCtrlChain(void* default_pool, size_t default_size, size_t block_size)
            : alloc_node_(default_pool, default_size)
            , block_size_(block_size)
        {
        }

        ~AllocCtrlChain()
        {
            Dissolve();
        }

        void* Alloc(size_t size)
        {
            void* buff = alloc_node_.value.Alloc(size);
            if (buff)
                return buff;

            CreateNode(std::max(AlignSize(size + Align, Align), block_size_));

            buff = alloc_node_.value.Alloc(size);
            assert(buff);
            return buff;
        }

        void Swap(AllocCtrlChain<AllocCtrl>& other)
        {
            std::swap(block_size_, other.block_size_);
            std::swap(empty_head_, other.empty_head_);
            std::swap(alloc_node_.next, other.alloc_node_.next_node);
            alloc_node_.value.Swap(other.alloc_node_.Value);
        }

        /* Reset the allocator without free alloc node */
        void Reset()
        {
            AllocNode* cur = nullptr;
            AllocNode* node = alloc_node_.next;

            while (node)
            {
                cur = node;
                node = node->next;

                cur->next = empty_head_;
                cur->value.Reset();
                empty_head_ = cur;
            }

            alloc_node_.next = nullptr;
            alloc_node_.value.Reset();

            if (cur)
                alloc_node_.value.Swap(cur->value);
        }

        /* Reset the allocator and free alloc node */
        void Dissolve()
        {
            AllocNode* node = alloc_node_.next;
            alloc_node_.next = nullptr;
            while (node)
            {
                AllocNode* cur = node;
                node = node->next;

                if (node == nullptr)
                    alloc_node_.value.Swap(cur->value);

                cur->~AllocNode();
                delete reinterpret_cast<int8_t*>(cur);
            }

            while (empty_head_)
            {
                AllocNode* pCur = empty_head_;
                empty_head_ = empty_head_->next;

                pCur->~AllocNode();
                delete reinterpret_cast<int8_t*>(pCur);
            }
        }

        void CreateNode(size_t size)
        {
            AllocNode* node = FindSuitableNode(size);
            if (node == nullptr)
            {
                constexpr size_t NodeObjSize = AlignSize(sizeof(AllocNode), Align);
                int8_t* mem = new int8_t[NodeObjSize + size];
                assert(mem);

                node = new (mem) AllocNode(mem + NodeObjSize, size);
            }

            alloc_node_.value.Swap(node->value);
            node->next = alloc_node_.next;
            alloc_node_.next = node;
        }

        AllocNode* FindSuitableNode(size_t size)
        {
            AllocNode* prev = nullptr;
            AllocNode* node = empty_head_;

            while (node)
            {
                if (node->value.Capacity() < size)
                {
                    prev = node;
                    node = node->next;
                }
                else
                {
                    if (prev)
                        prev->next = node->next;
                    else
                        empty_head_ = node->next;
                    break;
                }
            }

            return node;
        }

        AllocNode* empty_head_ = nullptr;
        AllocNode alloc_node_;
        size_t block_size_;
    };
} // Alloc_Internal

UTILITY_NAMESPACE_END
