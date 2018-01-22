/*
 * allocator base, for hide the implement
*/
#pragma once


#include "common.h"

NAMESPACE_BEGIN

class deallocator
{
public:
    virtual void deallocate(void* buff, size_t size) = 0;
};

NAMESPACE_END
