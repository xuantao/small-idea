#include "scope.h"
#include "stacked_allocator.h"
#include <vector>
#include <memory>

UTILITY_NAMESPACE_BEGIN

namespace scoped
{
    ScopeBuffer allocate(size_t size)
    {
        typedef StackedAllocator<> scoped_allocator;

#if SCOPED_ALLOCATOR_MULTITHREAD
        static thread_local scoped_allocator* s_alloc = new scoped_allocator();
#else
        static scoped_allocator* s_alloc = new scoped_allocator();
#endif // SCOPED_ALLOCATOR_MULTITHREAD

        return s_alloc->Alloc(size);
    }
} // namespace scoped

UTILITY_NAMESPACE_END
