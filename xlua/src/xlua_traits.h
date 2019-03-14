#pragma once
#include "xlua_def.h"

XLUA_NAMESPACE_BEGIN

// std::indetity
template <typename Ty>
struct Indetity
{
    typedef Ty type;
};

XLUA_NAMESPACE_END
