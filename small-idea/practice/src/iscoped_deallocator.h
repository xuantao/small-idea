/*
 * deallocator base, for hide the implement
*/
#pragma once

#include "common.h"

UTILITY_NAMESPACE_BEGIN

struct iscoped_deallocator
{
    virtual ~iscoped_deallocator() { }
    virtual void deallocate(void* buff, size_t size) = 0;
};

UTILITY_NAMESPACE_END
