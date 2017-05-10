/*
 * allocator base, for hide the implement
*/
#pragma once

#include <cstdint>
#include "def.h"

NAMESPACE_ZH_BEGIN

class deallocator
{
public:
    virtual void deallocate(void* buff, size_t size) = 0;
};

NAMESPACE_ZH_END
