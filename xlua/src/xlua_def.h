#pragma once
#include <lua.hpp>

#define XLUA_NAMESPACE_BEGIN    namespace xlua {
#define XLUA_NAMESPACE_END      } // namespace xlua

XLUA_NAMESPACE_BEGIN

typedef int(*LuaFunction)(lua_State*);
typedef int(*LuaIndexer)(lua_State*, void*);

struct TypeInfo;

struct ITypeDesc
{
    virtual ~ITypeDesc() { }
    virtual void SetName(const char* name) = 0;
    virtual void SetSupper(const TypeInfo* info) = 0;
    virtual void AddFunc(const char* name, LuaFunction func, bool is_static) = 0;
    virtual void AddVar(const char* name, LuaIndexer get, LuaIndexer set, bool is_static) = 0;
    virtual int Finalize() = 0;
};

struct LuaDeclare
{
    typedef int self;
    typedef int super;
};

struct LuaExpType
{
public:
    typedef LuaDeclare LuaDeclare;
    static const TypeInfo* LuaGetTypeInfo();
};

XLUA_NAMESPACE_END
