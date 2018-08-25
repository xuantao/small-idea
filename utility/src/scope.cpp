#include "utility/scope.h"
#include "utility/stack_allocator.h"
#include <vector>
#include <memory>

UTILITY_NAMESPACE_BEGIN

namespace scoped
{
    namespace
    {
        struct heap_deallocator final : public iscope_deallocator
        {
            void deallocate(void* buff, size_t size) override { delete[](char*)buff; }
        };
    } // namespace detail

    scoped_buffer allocate(size_t size)
    {
        typedef chain_stack_allocator<
            SCOPED_ALLOCATOR_BLOCK_SIZE,
            SCOPED_ALLOCATOR_ALIGN_SIZE> scoped_allocator;

#if SCOPED_ALLOCATOR_MULTITHREAD
        static thread_local scoped_allocator* s_alloc = new scoped_allocator();
#else
        static scoped_allocator* s_alloc = new scoped_allocator();
#endif // SCOPED_ALLOCATOR_MULTITHREAD

        static heap_deallocator s_deallocator;

        if (size > SCOPED_ALLOCATOR_BLOCK_SIZE)
            return scoped_buffer(&s_deallocator, new char[size], size);
        return s_alloc->allocate(size);
    }
} // namespace scoped

UTILITY_NAMESPACE_END
