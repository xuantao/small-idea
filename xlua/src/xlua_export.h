#pragma once
#include "xlua_def.h"
#include "detail/traits.h"
#include "detail/export.h"

#define _XLUA_SUPER_CLASS(...)  typename xlua::detail::BaseType<__VA_ARGS__>::type

// 导出实现
#define _XLUA_EXPORT_FUNC(Name, Func)                                                   \
    static_assert(!std::is_null_pointer<decltype(Func)>::value,                         \
        "can not export func:"##Name##" with null pointer"                              \
    );                                                                                  \
    int export_to_xlua_##Name = 0;                                                      \
    (export_to_xlua_##Name);    /* used for check rename */                             \
    struct Func_##Name {                                                                \
        static int call(xlua::xLuaState* L) {                                           \
            return xlua::detail::MetaCall(L, Func);                                     \
        }                                                                               \
    };                                                                                  \
    desc->AddMember(xlua::detail::MakeMember(#Name, &Func_##Name::call),                \
        !std::is_member_function_pointer<decltype(Func)>::value);

#define _XLUA_EXPORT_VAR(Name, GetOp, SetOp)                                            \
    static_assert(                                                                      \
        xlua::detail::IndexerTrait<decltype(GetOp), decltype(SetOp)>::is_allow,         \
        "can not export var:"#Name" to lua"                                             \
    );                                                                                  \
    int export_to_xlua_##Name = 0;                                                      \
    (export_to_xlua_##Name);    /* used for check rename problem */                     \
    struct Var_##Name {                                                                 \
        static void get(xlua::xLuaState* L, void* obj) {                                \
            xlua::detail::MetaGet(L, obj, GetOp);                                       \
        }                                                                               \
        static void set(xlua::xLuaState* L, void* obj) {                                \
            xlua::detail::MetaSet(L, obj, SetOp);                                       \
        }                                                                               \
    };                                                                                  \
    desc->AddMember(xlua::detail::MakeMember(#Name,                                     \
        std::is_null_pointer<decltype(GetOp)>::value ? nullptr : &Var_##Name::get,      \
        std::is_null_pointer<decltype(SetOp)>::value ? nullptr : &Var_##Name::set),     \
        !xlua::detail::IndexerTrait<decltype(GetOp), decltype(SetOp)>::is_member        \
    );

/* 导出Lua类开始 */
#define XLUA_EXPORT_CLASS_BEGIN(ClassName)                                              \
    static_assert(std::is_void<ClassName::LuaDeclare::super>::type                      \
        || std::is_base_of<ClassName::LuaDeclare::super, ClassName>::value,             \
        "internal class is not inherited"                                               \
    );                                                                                  \
    static_assert(std::is_void<ClassName::LuaDeclare::super>::type                      \
        || xlua::detail::IsInternal<ClassName::LuaDeclare::super>::value                \
        || xlua::detail::IsExternal<ClassName::LuaDeclare::super>::value                \
        "base type is not declare to export to lua"                                     \
    );                                                                                  \
    namespace {                                                                         \
        xlua::detail::TypeNode type_node_##__LINE__(&ClassName::xLuaGetTypeInfo);       \
    } /* end namespace */                                                               \
    const xlua::TypeInfo* ClassName::xLuaGetTypeInfo() {                                \
        static xlua::TypeKey s_key;                                                     \
        if (s_key.IsValid())                                                            \
            return xlua::GetTypeInfo(s_key);                                            \
        ITypeDesc* desc = xlua::AllocTypeInfo(                                          \
            #ClassName,                                                                 \
            xlua::detail::GetTypeInfo<LuaDeclare::super>()                              \
        );                                                                              \
        if (desc == nullptr)                                                            \
            return nullptr;                                                             \
        desc->SetConverter(                                                             \
            &xlua::detail::CastCheck<ClassName, LuaDeclare::SuperType>::CastUp,         \
            &xlua::detail::CastCheck<ClassName, LuaDeclare::SuperType>::CastDown        \
        );                                                                              \
        using class_type = ClassName;


/* 导出lua类结束 */
#define XLUA_EXPORT_CLASS_END()                                                         \
        s_key = desc->Finalize();                                                       \
        return xlua::GetTypeInfo(s_key);                                                \
    }

/* 导出外部类 */
#define XLUA_EXPORT_EXTERNAL_CLASS_BEGIN(ClassName, ...)                                \
    static_assert(std::is_void<_XLUA_SUPER_CLASS(__VA_ARGS__)>::value                   \
        || std::is_base_of<_XLUA_SUPER_CLASS(__VA_ARGS__), ClassName>::value,           \
        "external class is not inherited"                                               \
    );                                                                                  \
    static_assert(std::is_void<_XLUA_SUPER_CLASS(__VA_ARGS__)>::value                   \
        || xlua::detail::IsInternal<_XLUA_SUPER_CLASS(__VA_ARGS__)>::value              \
        || xlua::detail::IsExternal<_XLUA_SUPER_CLASS(__VA_ARGS__)>::value,             \
        "base type is not declare to export to lua"                                     \
    );                                                                                  \
    namespace {                                                                         \
        xlua::detail::TypeNode type_node_##__LINE__([]() -> const xlua::TypeInfo* {     \
            return xLuaGetTypeInfo(xlua::Identity<ClassName>());                        \
        });                                                                             \
    } /* end namespace */                                                               \
    const xlua::TypeInfo* xLuaGetTypeInfo(xlua::Identity<ClassName>) {                  \
        using class_type = ClassName;                                                   \
        static xlua::TypeKey s_key;                                                     \
        if (s_key.IsValid())                                                            \
            return xlua::GetTypeInfo(s_key);                                            \
        xlua::ITypeDesc* desc = xlua::AllocTypeInfo(                                    \
            #ClassName,                                                                 \
            xlua::detail::GetTypeInfo<_XLUA_SUPER_CLASS(__VA_ARGS__)>()                 \
        );                                                                              \
        if (desc == nullptr)                                                            \
            return nullptr;                                                             \
        desc->SetConverter(                                                             \
            &xlua::detail::CastCheck<ClassName, _XLUA_SUPER_CLASS(__VA_ARGS__)>::CastUp,   \
            &xlua::detail::CastCheck<ClassName, _XLUA_SUPER_CLASS(__VA_ARGS__)>::CastDown  \
        );                                                                              \
        using class_type = ClassName;

#define XLUA_EXPORT_EXTERNAL_CLASS_END()                                                \
    XLUA_EXPORT_CLASS_END()

/* 导出全局表 */
#define XLUA_EXPORT_GLOBAL_BEGIN(Name)                                                  \
    namespace {                                                                         \
        xlua::detail::TypeNode type_node_##__LINE__([]() -> const xlua::TypeInfo* {     \
            static xlua::TypeKey s_key;                                                 \
            if (s_key.IsValid())                                                        \
                return xlua::GetTypeInfo(s_key);                                        \
            xlua::ITypeDesc* desc = xlua::AllocTypeInfo(#Name, nullptr);                \
            if (desc == nullptr)                                                        \
                return nullptr;                                                         \

#define XLUA_EXPORT_GLOBAL_END()                                                        \
            s_key = desc->Finalize();                                                   \
            return xlua::GetTypeInfo(s_key);                                            \
        });                                                                             \
    }   /* end namespace*/

#define XLUA_EXPORT_MEMBER_FUNC(Func)   _XLUA_EXPORT_FUNC(Func, &class_type::Func)
#define XLUA_EXPORT_MEMBER_VAR(Var)     _XLUA_EXPORT_VAR(Var, &class_type::Var, &class_type::Var)
#define XLUA_EXPORT_GLOBAL_FUNC(Func)   _XLUA_EXPORT_FUNC(Func, &Func)
#define XLUA_EXPORT_GLOBAL_VAR(Var)     _XLUA_EXPORT_VAR(Var, &Var, &Var)
//#define XLUA_EXPORT_

/* 导出常量表 */

#define _XLUA_EXPORT_CONST_BEGIN(Name)                          \
    namespace {                                                 \
        xlua::detail::ConstNode const_node_##__LINE__(          \
        []() -> const xlua::ConstInfo* {                        \
            static xlua::ConstInfo info;                        \
            info.name = #Name;

#define XLUA_EXPORT_CONST_BEGIN(Name)                           \
    _XLUA_EXPORT_CONST_BEGIN(Name)                              \
            static xlua::ConstValue values[] = {

#define XLUA_EXPORT_CONST_END()                                 \
                xlua::detail::MakeConstValue()                  \
            };                                                  \
            info.values = values;                               \
            return &info;                                       \
        }); /* end function */                                  \
    } /* end namespace*/

#define XLUA_EXPORT_CONST_VAR(Name, Val) xlua::detail::MakeConstValue(#Name, Val),

/* 导出枚举表 */
#define XLUA_EXPORT_ENUM_BEGIN_AS(Name, EnumType)               \
    _XLUA_EXPORT_CONST_BEGIN(Name)                              \
            using enum_type = EnumType;                         \
            static xlua::ConstValue values[] = {

#define XLUA_EXPORT_ENUM_BEGIN(EnumType)    XLUA_EXPORT_ENUM_BEGIN_AS(EnumType, EnumType)
#define XLUA_EXPORT_ENUM_END()              XLUA_EXPORT_CONST_END()

#define XLUA_EXPORT_ENUM_VAR_AS(Name, Val)  XLUA_EXPORT_CONST_VAR(Name, (int)enum_type::Val)
#define XLUA_EXPORT_ENUM_VAR(Val)           XLUA_EXPORT_ENUM_VAR_AS(Var, Val)

/* 导出预设脚本 */
#define XLUA_EXPORT_SCRIPT(Str)                                 \
    namespace {                                                 \
        xlua::detail::ScriptNode script_node_##__LINE__(Str);   \
    }

