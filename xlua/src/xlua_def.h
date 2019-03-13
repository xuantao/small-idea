#pragma once
#include <lua.hpp>

#define XLUA_NAMESPACE_BEGIN    namespace xlua {
#define XLUA_NAMESPACE_END      } // namespace xlua

XLUA_NAMESPACE_BEGIN

struct xLuaState;
struct TypeInfo;

typedef int(*LuaFunction)(xLuaState*);
typedef int(*LuaIndexer)(xLuaState*, void*);

template <typename Ty, typename... Bys>
struct Declare
{
    static_assert(sizeof...(Bys) > 1, "not support multy inherit");
};

template <typename Ty>
struct Declare<Ty>
{
    typedef void    SuperType;
    typedef Ty      SelfType;
};

template <typename Ty, typename By>
struct Declare<Ty, By>
{
    typedef By      SuperType;
    typedef Ty      SelfType;
};

struct ITypeDesc
{
    virtual ~ITypeDesc() { }
    virtual void AddFunc(const char* name, LuaFunction func, bool member) = 0;
    virtual void AddVar(const char* name, LuaIndexer getter, LuaIndexer setter, bool member) = 0;
    virtual int Finalize() = 0;
};

ITypeDesc* AllocTypeInfo(const char* name, const TypeInfo* super);
const TypeInfo* GetTypeInfo(int index);

XLUA_NAMESPACE_END

/* 声明导出Lua类 */
#define XLUA_DECLARE(ClassName, ...)                            \
    typedef xlua::Declare<ClassName, _VAR_ARGS_> LuaDeclare;    \
    static const xlua::TypeInfo* LuaGetTypeInfo()

