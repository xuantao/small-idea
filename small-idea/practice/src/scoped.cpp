#include <vector>
#include <memory>
#include "scoped.h"
#include "scoped_allocator.h"

UTILITY_NAMESPACE_BEGIN

namespace scoped
{
    namespace detail
    {
        struct ScopedBufferPool
        {
            typedef scoped_allocator<
                SCOPED_ALLOCATOR_BLOCK_SIZE,
                SCOPED_ALLOCATOR_ALIGN_SIZE> scoped_allocator;
            typedef std::auto_ptr<scoped_allocator> scoped_allocator_ptr;

            struct default_deallocator : public iscoped_deallocator
            {
                void deallocate(void* buff, size_t size) override { delete buff; }
            };

            static constexpr size_t max_alloc_size =
                SCOPED_ALLOCATOR_BLOCK_SIZE - SCOPED_ALLOCATOR_ALIGN_SIZE * 3;

            ScopedBufferPool()
            {
                increment();
            }

            scoped_buffer allocate(size_t size)
            {
                /* 超出容器最大范围, 则直接从堆里面分配 */
                if (size >= max_alloc_size)
                    return scoped_buffer(&_dealloc, new char[size], size);

                auto b1 = _pool.back()->allocate(size);
                if (!b1.empty())
                    return b1;

                increment();

                auto b2 = _pool.back()->allocate(size);
                assert(!b2.empty());
                return b2;
            }

            inline void increment()
            {
                _pool.push_back(scoped_allocator_ptr(new scoped_allocator()));
            }

            default_deallocator _dealloc;
            std::vector<scoped_allocator_ptr> _pool;
        };
    } // namespace detail

    scoped_buffer allocate(size_t size)
    {
#if SCOPED_ALLOCATOR_MULTITHREAD
        static thread_local detail::ScopedBufferPool sPool;
#else
        static detail::ScopedBufferPool sPool;
#endif // SCOPED_ALLOCATOR_MULTITHREAD

        return sPool.allocate(size);
    }
} // namespace scoped

UTILITY_NAMESPACE_END
