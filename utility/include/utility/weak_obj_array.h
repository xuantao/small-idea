#pragma once

#include "common.h"

UTILITY_NAMESPACE_BEGIN

class WeakObjIndex
{
    friend class WeakObjArray;
public:
    WeakObjIndex() = default;
    ~WeakObjIndex();

    WeakObjIndex(const WeakObjIndex&) = delete;
    WeakObjIndex& operator = (const WeakObjIndex&) = delete;

private:
    int index_ = 0;
};

UTILITY_NAMESPACE_END
