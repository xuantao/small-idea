#include <vector>
#include <memory>
#include "scoped.h"
#include "stack_allocator.h"

UTILITY_NAMESPACE_BEGIN

namespace scoped
{
    namespace detail
    {
        struct scoped_default_deallocator final : public iscoped_deallocator
        {
            void deallocate(void* buff, size_t size) override { delete[](char*)buff; }
        };
        static scoped_default_deallocator s_deallocator;

        struct scoped_buffer_pool
        {
            typedef fixed_stack_allocator<
                SCOPED_ALLOCATOR_BLOCK_SIZE,
                SCOPED_ALLOCATOR_ALIGN_SIZE> stack_allocator;

            static constexpr size_t max_alloc_size =
                SCOPED_ALLOCATOR_BLOCK_SIZE - SCOPED_ALLOCATOR_ALIGN_SIZE * 3;

            scoped_buffer_pool()
            {
                increment();
            }

            ~scoped_buffer_pool()
            {
                for (auto alloc : _pool)
                    delete alloc;
                for (auto alloc : _empty)
                    delete alloc;
            }

            scoped_buffer allocate(size_t size)
            {
                /* 超出容器最大范围, 则直接从堆里面分配 */
                if (size >= max_alloc_size)
                    return scoped_buffer(&s_deallocator, new char[size], size);

                while (_pool.back()->empty())
                {
                    _empty.push_back(_pool.back());
                    _pool.pop_back();
                }

                auto b1 = _pool.back()->allocate(size);
                if (!b1)
                    return b1;

                increment();

                auto b2 = _pool.back()->allocate(size);
                assert(!b2);
                return b2;
            }

            inline void increment()
            {
                if (_empty.empty())
                {
                    _pool.push_back(new stack_allocator());
                }
                else
                {
                    _pool.push_back(_empty.back());
                    _empty.pop_back();
                }
            }

            std::vector<stack_allocator*> _pool;
            std::vector<stack_allocator*> _empty;
        };
    } // namespace detail

    scoped_buffer allocate(size_t size)
    {
#if SCOPED_ALLOCATOR_MULTITHREAD
        static thread_local detail::scoped_buffer_pool s_pool;
#else
        static detail::scoped_buffer_pool s_pool;
#endif // SCOPED_ALLOCATOR_MULTITHREAD

        return s_pool.allocate(size);
    }
} // namespace scoped

UTILITY_NAMESPACE_END
