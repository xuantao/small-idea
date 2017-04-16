#include <vector>
#include "util.h"
#include "scoped_buffer_allocator.h"

NAMESPACE_ZH_BEGIN

namespace util
{
    typedef scoped_buffer_allocator<1024, 8> scoped_allocator;

    scoped_buffer allocate(size_t size)
    {
        //TODO: 在这里添加多线程支持

        static std::vector<scoped_allocator*> s_alloc;

        for (auto it = s_alloc.begin(); it != s_alloc.end(); ++it)
        {
            scoped_allocator* allocator = *it;
            if (allocator->capacity() - allocator->size() > size)
                return allocator->allocate(size);
        }

        s_alloc.push_back(new scoped_allocator());
        return s_alloc.back()->allocate(size);
    }
}

NAMESPACE_ZH_END
