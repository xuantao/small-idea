/*
 *
*/

#include <cstdint>

#pragma once

namespace zh
{
    class allocator_base
    {
    public:
        virtual void* allocate(size_t size) = 0;
        virtual void deallocate(void* buff, size_t size) = 0;
    };
}
