/*
 * allocator base, for hide the implement
*/
#pragma once

#include <cstdint>
#include "def.h"

NAMESPACE_ZH_BEGIN

class allocator_base
{
public:
    virtual void* allocate(size_t size) = 0;
    virtual void deallocate(void* buff, size_t size) = 0;
};

NAMESPACE_ZH_END
