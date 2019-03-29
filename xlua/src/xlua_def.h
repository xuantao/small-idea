#pragma once
#include "xlua_config.h"
#include <lua.hpp>
#include <type_traits>
#include <cstdint>
#include <cassert>

XLUA_NAMESPACE_BEGIN

namespace detail {
    template <typename... Tys> struct BaseType { static_assert(sizeof...(Tys) > 1, "not allow multy inherit"); };
    template <> struct BaseType<> { typedef void type; };
    template <typename Ty> struct BaseType<Ty> { typedef Ty type; };

    class GlobalVar;
    void LogError(const char* fmt, ...);
}

class xLuaState;
struct TypeInfo;

/* 导出到Lua类型 */
typedef int (*LuaFunction)(lua_State* L);
typedef void (*LuaIndexer)(xLuaState* L, void* obj, const TypeInfo* info);

/* convert to weak_obj_ptr */
typedef void* (*ToWeakPtr)(void* obj);
typedef void* (*PtrCast)(void* obj, const TypeInfo* src, const TypeInfo* dst);

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

enum class ConstCategory {
    kNone,
    kInteger,
    kFloat,
    kString,
};

struct ConstValue {
    ConstCategory category;
    const char* name;

    union {
        int int_val;
        float float_val;
        const char* string_val;
    };
};

struct ConstInfo {
    const char* name;
    const ConstValue* values;
};

enum class MemberCategory {
    kInvalid = 0,
    kVariate,
    kFunction,
};

struct TypeMember {
    MemberCategory category;
    const char* name;
    union {
        struct {
            LuaFunction func;
        };
        struct {
            LuaIndexer getter;
            LuaIndexer setter;
        };
    };
};

/* 类型转换器
 * 基础类型可以子类->基类，基类->子类
*/
struct TypeCaster {
    PtrCast to_super;       // 转为基类指针, static_cast
    PtrCast to_derived;     // 转为派生类指针, dynamic_cast
    ToWeakPtr to_weak_ptr;  // 弱对象指针
};

enum class TypeCategory
{
    kInternal,
    kExternal,
    kGlobal,
};

struct TypeInfo {
    int index;
    TypeCategory category;
    const char* type_name;
    const char* shared_ptr_name;
    const char* unique_ptr_name;
    bool is_weak_obj;
    unsigned char external_type_index;  // 外部类型编号, 用于lightuserdata索引类型
    const TypeInfo* super;
    TypeCaster caster;
    TypeMember* members;
    TypeMember* globals;
};

struct ITypeDesc {
    virtual ~ITypeDesc() { }
    virtual void SetCaster(TypeCaster caster) = 0;
    virtual void AddMember(const char* name, LuaFunction func, bool glboal) = 0;
    virtual void AddMember(const char* name, LuaIndexer getter, LuaIndexer setter, bool glboal) = 0;
    virtual const TypeInfo* Finalize() = 0;
};

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
#define XLUA_DECLARE_CLASS(ClassName, ...)                                          \
    typedef XLUA_USE_NAMESPCE Declare<ClassName,                                    \
        typename XLUA_USE_NAMESPCE detail::BaseType<_VAR_ARGS_>::type> LuaDeclare;  \
    XLUA_USE_NAMESPCE ObjIndex xlua_obj_index_;                                     \
    static const XLUA_USE_NAMESPCE TypeInfo* xLuaGetTypeInfo()

/* 声明导出外部类 */
#define XLUA_DECLARE_EXTERNAL_CLASS(ClassName)                                      \
    const XLUA_USE_NAMESPCE TypeInfo* xLuaGetTypeInfo(XLUA_USE_NAMESPCE Identity<ClassName>)
