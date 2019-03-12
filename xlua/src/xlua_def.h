#pragma once
#include <lua.hpp>

#define XLUA_NAMESPACE_BEGIN    namespace xlua {
#define XLUA_NAMESPACE_END      } // namespace xlua

XLUA_NAMESPACE_BEGIN

typedef int(*LuaFunction)(lua_State*);
typedef int(*LuaIndexer)(lua_State*, void*);

struct TypeInfo;

template <typename... Tys>
struct Declare;

template <typename Ty>
struct Declare<Ty>
{
    typedef Ty      SelfType;
    typedef void    SuperType;
};

template <typename Ty, typename By>
struct Declare<Ty>
{
    typedef By  SuperType;
    typedef Ty  SelfType;
};

struct ITypeDesc
{
    virtual ~ITypeDesc() { }
    virtual void AddFunc(const char* name, LuaFunction func, bool global) = 0;
    virtual void AddVar(const char* name, LuaIndexer getter, LuaIndexer setter, bool global) = 0;
    virtual int Finalize() = 0;
};

ITypeDesc* AllocTypeInfo(const char* name, const TypeInfo* super);
const TypeInfo* GetTypeInfo(int index);

XLUA_NAMESPACE_END

/* 声明导出Lua类 */
#define XLUA_DECLARE(ClassName, ...)                            \
    typedef xlua::Declare<ClassName, _VAR_ARGS_> LuaDeclare;    \
    static const xlua::TypeInfo* LuaGetTypeInfo()

