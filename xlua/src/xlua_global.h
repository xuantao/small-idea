#pragma once
#include "xlua_def.h"
#include <vector>
#include <mutex>

XLUA_NAMESPACE_BEGIN

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
};

namespace detail
{
    struct TypeDesc;

    class GlobalVar
    {
        friend struct TypeDesc;
    public:
        static GlobalVar* GetInstance() { return instance_; }
    public:
        ITypeDesc* AllocType(const char* name, const TypeInfo* super);
        const TypeInfo* GetTypeInfo(int index) const;

    private:
        int AddTypeInfo(TypeDesc* desc);

    private:
        //std::mutex mutex_;
        std::vector<TypeInfo*> types_;

    private:
        static GlobalVar* instance_;
    };
} // namespace detail

XLUA_NAMESPACE_END
