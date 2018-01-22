#include <vector>
#include <memory>
#include "scoped.h"
#include "scoped_buffer_allocator.h"

NAMESPACE_BEGIN

namespace scoped
{
    class default_deallocator : public deallocator
    {
    public:
        virtual void deallocate(void* buff, size_t size)
        {
            delete buff;
        }
    };

    typedef scoped_buffer_allocator<
        SCOPED_ALLOCATOR_BLOCK_SIZE,
        SCOPED_ALLOCATOR_ALIGN_SIZE> scoped_allocator;
    typedef std::auto_ptr<scoped_allocator> scoped_allocator_ptr;

#if SCOPED_ALLOCATOR_MULTITHREAD
    //TODO: 在这里添加多线程支持
#else
    scoped_buffer allocate_impl(size_t size)
    {
        static std::vector<scoped_allocator_ptr> s_alloc;

        for (auto it = s_alloc.begin(); it != s_alloc.end(); ++it)
        {
            scoped_allocator_ptr& allocator = *it;
            if (allocator->capacity() - allocator->size() > size)
                return allocator->allocate(size);
        }

        s_alloc.push_back(scoped_allocator_ptr(new scoped_allocator()));
        return s_alloc.back()->allocate(size);
    }
#endif // SCOPED_ALLOCATOR_MULTITHREAD

    scoped_buffer allocate(size_t size)
    {
        static default_deallocator s_default;
        /*
         * 超出容器最大范围, 则直接从堆里面分配
        */
        if (size > scoped_allocator::capacity_size)
            return scoped_buffer(&s_default, new char[size], size);

        return allocate_impl(size);
    }
}

NAMESPACE_END
