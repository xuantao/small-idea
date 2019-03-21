#pragma once
#include "xlua_def.h"
#include "detail/traits.h"
#include "detail/export.h"

#define _XLUA_SUPER_CLASS(...)  typename xlua::detail::BaseType<__VA_ARGS__>::type

struct Func_XX : xlua::detail::MetaType<Func_XX>
{
    Func_XX(xlua::ITypeDesc* desc) {
        // desc->AddMember()
    }
    static int Call(xlua::xLuaState* l) {
        return 0;
    }
};

// 导出实现
#define _XLUA_EXPORT_FUNC(Name, Func, Meta)                                             \
    static_assert(!std::is_null_pointer<decltype(Func)>::value,                         \
        "can not export func:"#Name" with null pointer"                                 \
    );                                                                                  \
    struct Func_##__LINE__ {                                                            \
        static int call(xlua::xLuaState* l) {                                           \
            return xlua::detail::Meta<class_type>::Call(l, s_type_info, Func);          \
        }                                                                               \
    };                                                                                  \
    desc->AddMember(xlua::detail::MakeMember(#Name, &Func_##__LINE__::call),            \
        !std::is_member_function_pointer<decltype(Func)>::value);

#define _XLUA_EXPORT_VAR(Name, GetOp, SetOp, Meta)                                      \
    static_assert(                                                                      \
        xlua::detail::IndexerTrait<decltype(GetOp), decltype(SetOp)>::is_allow,         \
        "can not export var:"#Name" to lua"                                             \
    );                                                                                  \
    struct Var_##__LINE__ {                                                             \
        static void Get(xlua::xLuaState* l, void* obj) {                                \
            xlua::detail::Meta<class_type>::Get(l, (class_type*)obj, GetOp);            \
        }                                                                               \
        static void Set(xlua::xLuaState* l, void* obj) {                                \
            xlua::detail::Meta<class_type>::Set(l, (class_type*)obj, SetOp);            \
        }                                                                               \
    };                                                                                  \
    desc->AddMember(xlua::detail::MakeMember(#Name,                                     \
        std::is_null_pointer<decltype(GetOp)>::value ? nullptr : &Var_##__LINE__::Get,  \
        std::is_null_pointer<decltype(SetOp)>::value ? nullptr : &Var_##__LINE__::Set), \
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
        using class_type = ClassName;                                                   \
        static const xlua::TypeInfo* s_type_info = nullptr;                             \
        if (s_type_info)                                                                \
            return s_type_info;                                                         \
        ITypeDesc* desc = xlua::AllocTypeInfo(xlua::TypeCategory::kInternal,            \
            #ClassName,                                                                 \
            xlua::detail::GetTypeInfoImpl<LuaDeclare::super>()                          \
        );                                                                              \
        if (desc == nullptr)                                                            \
            return nullptr;                                                             \
        desc->SetConverter(                                                             \
            &xlua::detail::PtrCast<ClassName, LuaDeclare::SuperType>::CastUp,           \
            &xlua::detail::PtrCast<ClassName, LuaDeclare::SuperType>::CastDown,         \
            &xlua::detail::SharedPtrCast<ClassName, LuaDeclare::SuperType>::Cast        \
        );

/* 导出lua类结束 */
#define XLUA_EXPORT_CLASS_END()                                                         \
        s_type_info = desc->Finalize();                                                 \
        return s_type_info;                                                             \
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
        using super_type = _XLUA_SUPER_CLASS(__VA_ARGS__);                              \
        static const xlua::TypeInfo* s_type_info = nullptr;                             \
        if (s_type_info)                                                                \
            return s_type_info;                                                         \
        xlua::ITypeDesc* desc = xlua::AllocTypeInfo(xlua::TypeCategory::kExternal,      \
            false, #ClassName,                                                          \
            xlua::detail::GetTypeInfoImpl<super_type>()                                 \
        );                                                                              \
        if (desc == nullptr)                                                            \
            return nullptr;                                                             \
        desc->SetConverter(                                                             \
            &xlua::detail::PtrCast<ClassName, super_type>::CastUp,                      \
            &xlua::detail::PtrCast<ClassName, super_type>::CastDown,                    \
            &xlua::detail::SharedPtrCast<ClassName, super_type>::Cast                   \
        );

#define XLUA_EXPORT_EXTERNAL_CLASS_END()                                                \
    XLUA_EXPORT_CLASS_END()

/* 导出全局表 */
#define XLUA_EXPORT_GLOBAL_BEGIN(Name)                                                  \
    namespace {                                                                         \
        xlua::detail::TypeNode type_node_##__LINE__([]() -> const xlua::TypeInfo* {     \
            static const xlua::TypeInfo* s_type_info = nullptr;                         \
            if (s_type_info)                                                            \
                return s_type_info;                                                     \
            xlua::ITypeDesc* desc = xlua::AllocTypeInfo(xlua::TypeCategory::kGlobal,    \
                false, #Name, nullptr);                                                 \
            if (desc == nullptr)                                                        \
                return nullptr;                                                         \

#define XLUA_EXPORT_GLOBAL_END()                                                        \
            s_type_info = desc->Finalize();                                             \
            return s_type_info;                                                         \
        });                                                                             \
    }   /* end namespace*/

#define XLUA_EXPORT_MEMBER_FUNC(Func)   _XLUA_EXPORT_FUNC(Func, &class_type::Func, MetaFunc)
#define XLUA_EXPORT_MEMBER_VAR(Var)     _XLUA_EXPORT_VAR(Var, &class_type::Var, &class_type::Var, MetaVar)
#define XLUA_EXPORT_GLOBAL_FUNC(Func)   _XLUA_EXPORT_FUNC(Func, &Func, MetaFunc)
#define XLUA_EXPORT_GLOBAL_VAR(Var)     _XLUA_EXPORT_VAR(Var, &Var, &Var, MetaVar)
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

