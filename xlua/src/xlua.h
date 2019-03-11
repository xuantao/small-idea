#pragma once
#include <lua.hpp>
#include "xlua_def.h"

XLUA_NAMESPACE_BEGIN

namespace xlua_internal
{

} // namespace xlua_internal

typedef int(*LuaFunction)(lua_State*);
typedef int(*LuaIndexer)(lua_State*, void*);

struct MemberFunc
{
    const char* name;
    LuaFunction func;
};

struct MemberVar
{
    const char* name;
    LuaIndexer getter;
    LuaIndexer setter;
};

struct TypeIndex
{
    int serial;
    int index;
};

struct TypeInfo
{
    int index;
    const char* name;
    MemberVar* vars;
    MemberVar* static_vars;
    MemberFunc* funcs;
    MemberFunc* static_funcs;
};

XLUA_NAMESPACE_END
