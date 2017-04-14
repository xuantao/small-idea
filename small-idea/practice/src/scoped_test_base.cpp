#include <vector>
#include "scoped_test_base.h"
#include "scoped_buffer_allocator.h"

NAMESPACE_ZH_BEGIN
NAMESPACE_TEST_BEGIN

typedef scoped_buffer_allocator<1024, 8> scoped_allocator;

scoped_buffer allocate(size_t size)
{
    static std::vector<scoped_allocator*> s_Allocator;

    for (auto it = s_Allocator.begin(); it != s_Allocator.end(); ++it)
    {
        scoped_allocator* pAllocator = *it;
        if (pAllocator->capacity() - pAllocator->size() > size)
            return pAllocator->allocate(size);
    }

    s_Allocator.push_back(new scoped_allocator());
    return s_Allocator.back()->allocate(size);
}

NAMESPACE_TEST_END
NAMESPACE_ZH_END
