﻿#pragma once
#include "xlua_config.h"
#include <lua.hpp>
#include <type_traits>
#include <cstdint>
#include <cassert>

/* 名字空间 */
#define XLUA_NAMESPACE_BEGIN    namespace xlua {
#define XLUA_NAMESPACE_END      } // namespace xlua

XLUA_NAMESPACE_BEGIN

namespace detail {
    template <typename... Tys> struct BaseType { static_assert(sizeof...(Tys) > 1, "not allow multy inherit"); };
    template <> struct BaseType<> { typedef void type; };
    template <typename Ty> struct BaseType<Ty> { typedef Ty type; };

    /* 全局数据 */
    class GlobalVar;
    /* 日志输出 */
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

/* 常量字段类型 */
enum class ConstCategory {
    kNone,
    kInteger,
    kFloat,
    kString,
};

/* 常量数据 */
struct ConstValue {
    ConstCategory category;
    const char* name;

    union {
        int int_val;
        float float_val;
        const char* string_val;
    };
};

/* 常量信息 */
struct ConstInfo {
    const char* name;
    const ConstValue* values;
};

struct MemberFunc {
    const char* name;
    LuaFunction func;
};

struct MemberVar {
    const char* name;
    LuaIndexer getter;
    LuaIndexer setter;
};

/* 类型转换器
 * 基础类型可以子类->基类，基类->子类
*/
struct TypeCaster {
    PtrCast to_super;       // 转为基类指针, static_cast
    PtrCast to_derived;     // 转为派生类指针, dynamic_cast
    ToWeakPtr to_weak_ptr;  // 弱对象指针
};

/* 类型枚举 */
enum class TypeCategory {
    kInternal,
    kExternal,
    kGlobal,
};

/* 导出类型信息 */
struct TypeInfo {
    int index;
    TypeCategory category;
    const char* type_name;
    bool is_weak_obj;
    int8_t external_type_index; // 外部类型编号, 用于lightuserdata索引类型
    const TypeInfo* super;      // 父类信息
    TypeCaster caster;          // 类型转换器
    MemberVar* vars;            // 成员变量
    MemberFunc* funcs;          // 成员函数
    MemberVar* global_vars;     // 全局(静态)变量
    MemberFunc* global_funcs;   // 全局(静态)函数
};

/* 类型描述, 用于构建导出类型信息 */
struct ITypeDesc {
    virtual ~ITypeDesc() { }
    virtual void SetCaster(TypeCaster caster) = 0;
    virtual void AddMember(const char* name, LuaFunction func, bool global) = 0;
    virtual void AddMember(const char* name, LuaIndexer getter, LuaIndexer setter, bool global) = 0;
    virtual const TypeInfo* Finalize() = 0;
};

/* xlua弱引用编号
 * 管理导出对象生命周期
*/
class xLuaIndex {
    friend class detail::GlobalVar;
public:
    xLuaIndex() : index_(-1) { }
    ~xLuaIndex();

public:
    xLuaIndex(const xLuaIndex&) { }
    xLuaIndex& operator = (const xLuaIndex&) { }

private:
    int index_;
};

XLUA_NAMESPACE_END

/* 声明导出Lua类, 在类中插入xlua相关信息
 * 可变宏参数用于设置基类类型(不支持多基类)
 * 1. 关联父、子类关系
 * 2. 添加导出引用编号成员变量
 * 3. 添加获取类型信息的静态成员函数
*/
#define XLUA_DECLARE_CLASS(ClassName, ...)                              \
    typedef xlua::Declare<ClassName,                                    \
        typename xlua::detail::BaseType<_VAR_ARGS_>::type> LuaDeclare;  \
    xlua::xLuaIndex xlua_obj_index_;                                    \
    static const xlua::TypeInfo* xLuaGetTypeInfo()

/* 声明导出外部类 */
#define XLUA_DECLARE_EXTERNAL_CLASS(ClassName)                          \
    const xlua::TypeInfo* xLuaGetTypeInfo(xlua::Identity<ClassName>)
