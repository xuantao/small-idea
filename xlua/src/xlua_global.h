﻿#pragma once
#include "xlua_def.h"
#include <vector>
#include <mutex>

XLUA_NAMESPACE_BEGIN

typedef bool fnCastCheck(void* obj, const TypeInfo* type);

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

struct TypeInfo
{
    int index;
    const char* name;
    MemberVar* vars;
    MemberVar* static_vars;
    MemberFunc* funcs;
    MemberFunc* static_funcs;
    fnCastCheck fnCast;
};

namespace detail
{
    struct TypeDesc;

    class GlobalVar
    {
        friend struct TypeDesc;
    private:
        GlobalVar();
        ~GlobalVar();

    public:
        static bool Startup();
        static GlobalVar* GetInstance();

        void Purge();

    public:
        ITypeDesc* AllocType(const char* name, const TypeInfo* super);
        const TypeInfo* GetTypeInfo(int index) const;

    private:
        int AddTypeInfo(TypeDesc* desc);

    private:
        //std::mutex mutex_;
        std::vector<TypeInfo*> types_;
    };
} // namespace detail

XLUA_NAMESPACE_END
