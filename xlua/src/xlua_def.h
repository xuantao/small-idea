#pragma once
#include <lua.hpp>
#include <type_traits>

#define XLUA_NAMESPACE_BEGIN    namespace xlua {
#define XLUA_NAMESPACE_END      } // namespace xlua

XLUA_NAMESPACE_BEGIN

namespace detail {
    class GlobalVar;

    template <typename... Tys> struct BaseType;
    template <> struct BaseType<> { typedef void type; };
    template <> struct BaseType<void> { typedef void type; };
    template <typename Ty> struct BaseType<Ty> { typedef Ty type; };
}

struct xLuaState;
struct TypeInfo;

typedef int (*LuaFunction)(xLuaState* L);
typedef int (*LuaIndexer)(xLuaState* L, void* obj);
typedef bool (*LuaCastCheck)(void* obj, const TypeInfo* obj_info, const TypeInfo* target_info);

template <typename Ty, typename By>
struct Declare {
    typedef By  super;
    typedef Ty  self;
};

// std::indetity
template <typename Ty>
struct Identity {
    typedef Ty type;
};

class TypeKey {
    friend class detail::GlobalVar;
public:
    bool IsValid() const;

private:
    int index_ = -1;
    int serial_ = 0;
};

struct ITypeDesc {
    virtual ~ITypeDesc() { }
    virtual void SetCastCheck(LuaCastCheck cast_checker) = 0;
    virtual void AddFunc(const char* name, LuaFunction func, bool member) = 0;
    virtual void AddVar(const char* name, LuaIndexer getter, LuaIndexer setter, bool member) = 0;
    virtual TypeKey Finalize() = 0;
};

ITypeDesc* AllocTypeInfo(const char* name, const TypeInfo* super);
const TypeInfo* GetTypeInfo(const TypeKey& key);

class ObjIndex
{
    friend class detail::GlobalVar;
public:
    ObjIndex() : index_(-1) { }
    ~ObjIndex();

public:
    ObjIndex(const ObjIndex&) { }
    ObjIndex& operator = (const ObjIndex&) { }

private:
    int index_;
};

XLUA_NAMESPACE_END

/* 声明导出Lua类 */
#define XLUA_DECLARE(ClassName, ...)                                    \
    typedef xlua::Declare<ClassName,                                    \
        typename xlua::detail::BaseType<_VAR_ARGS_>::type> LuaDeclare;  \
    xlua::ObjIndex xlua_obj_index_;                                     \
    static const xlua::TypeInfo* xLuaGetTypeInfo()

/* 声明导出外部类 */
#define XLUA_DECLARE_EXPORT_EXTERNAL_CLASS(ClassName)                   \
    const xlua::TypeInfo* xLuaGetTypeInfo(xlua::Identity<ClassName>)
