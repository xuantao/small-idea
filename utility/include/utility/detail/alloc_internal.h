#pragma once
#include "../common.h"
#include <algorithm>
#include <cassert>

UTILITY_NAMESPACE_BEGIN

namespace Alloc_Internal
{
    /*
     * ����ʽ������, ˳���һ�黺���з���һ���ڴ�
     * ֻ���䲻�����ͷš��������ռ����ʱ����nullptr
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

        size_t capacity_;
        size_t alloced_ = 0;
        int8_t* pool_;
    }; // class AllocCtrl

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
            void* buff = alloc_node_.val.Alloc(size);
            if (buff)
                return buff;

            CreateNode(std::max(AlignSize(size + Align, Align), block_size_));

            buff = alloc_node_.val.Alloc(size);
            assert(buff);
            return buff;
        }

        void Swap(AllocCtrlChain<AllocCtrl>& other)
        {
            std::swap(block_, other.block_);
            std::swap(empty_head_, other.empty_head_);
            std::swap(alloc_node_.next_node, other.alloc_node_.next_node);
            alloc_node_.val.Swap(other.alloc_node_.Value);
        }

        /* Reset the allocator without free alloc node */
        void Reset()
        {
            AllocNode* cur = nullptr;
            AllocNode* node = alloc_node_.next_node;

            while (node)
            {
                cur = node;
                node = node->next_node;

                cur->next_node = empty_node_;
                cur->val.Reset();
                empty_node_ = cur;
            }

            alloc_node_.next_node = nullptr;
            alloc_node_.val.Reset();

            if (cur)
                alloc_node_.val.Swap(cur->val);
        }

        /* Reset the allocator and free alloc node */
        void Dissolve()
        {
            AllocNode* node = alloc_node_.next_node;
            alloc_node_.next_node = nullptr;
            while (node)
            {
                AllocNode* cur = node;
                node = node->next_node;

                if (node == nullptr)
                    alloc_node_.val.Swap(cur->val);

                cur->~AllocNode();
                delete reinterpret_cast<int8_t*>(cur);
            }

            while (empty_head_)
            {
                AllocNode* pCur = empty_head_;
                empty_head_ = empty_head_->next_node;

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

            alloc_node_.val.Swap(node->val);
            node->next_node = alloc_node_.next_node;
            alloc_node_.next_node = node;
        }

        AllocNode* FindSuitableNode(size_t size)
        {
            AllocNode* prev = nullptr;
            AllocNode* node = empty_head_;

            while (node)
            {
                if (node->val.Capacity() < size)
                {
                    prev = node;
                    node = node->next_node;
                }
                else
                {
                    if (prev)
                        prev->next_node = node->next_node;
                    else
                        empty_head_ = node->next_node;
                    break;
                }
            }

            return node;
        }

        size_t block_size_;
        AllocNode* empty_head_ = nullptr;
        AllocNode alloc_node_;
    };
} // Alloc_Internal

UTILITY_NAMESPACE_END