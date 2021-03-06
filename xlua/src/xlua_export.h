﻿#pragma once
#include "xlua_def.h"
#include "detail/traits.h"
#include "detail/export.h"
#include "detail/extractor.h"

/* 匿名 */
#define _XLUA_ANONYMOUS_2(line) Anonymous##line
#define _XLUA_ANONYMOUS_1(line) _XLUA_ANONYMOUS_2(line)
#define _XLUA_ANONYMOUS         _XLUA_ANONYMOUS_1(__LINE__)

/* 获取基类 */
#define _XLUA_SUPER_CLASS(...)      typename xlua::detail::BaseType<__VA_ARGS__>::type
/* 提取指定版本的成员函数 */
#define _EXTRACT_METHOD(Func, ...)  xlua::detail::Extractor<__VA_ARGS__>::extract(xlua::detail::ConstTag(), Func)

// 导出实现
#define _XLUA_EXPORT_FUNC_(Name, Func, Meta, IsGlobal)                                   \
    static_assert(!std::is_null_pointer<decltype(Func)>::value,                         \
        "can not export func:"#Name" with null pointer"                                 \
    );                                                                                  \
    struct _XLUA_ANONYMOUS {                                                            \
        static int call(lua_State* l) {                                                 \
            auto* xl = (xlua::xLuaState*)(lua_touserdata(l, lua_upvalueindex(1)));      \
            assert(xl);                                                                 \
            return xlua::detail::Meta<class_type>::Call(xl, s_type_info, Func);         \
        }                                                                               \
    };                                                                                  \
    desc->AddMember(#Name, &_XLUA_ANONYMOUS::call, IsGlobal);

#define _XLUA_EXPORT_FUNC(Name, Func, Meta)   _XLUA_EXPORT_FUNC_(Name, Func, Meta, !std::is_member_function_pointer<decltype(Func)>::value)

#define _XLUA_EXPORT_VAR_(Name, GetOp, SetOp, Meta, IsGlobal)                           \
    static_assert(                                                                      \
        xlua::detail::IndexerTrait<decltype(GetOp), decltype(SetOp)>::is_allow,         \
        "can not export var:"#Name" to lua"                                             \
    );                                                                                  \
    struct _XLUA_ANONYMOUS {                                                            \
        static void Get(xlua::xLuaState* l, void* obj, const xlua::TypeInfo* info) {    \
            xlua::detail::Meta<class_type>::Get(l, obj, info, s_type_info, GetOp);      \
        }                                                                               \
        static void Set(xlua::xLuaState* l, void* obj, const xlua::TypeInfo* info) {    \
            xlua::detail::Meta<class_type>::Set(l, obj, info, s_type_info, SetOp);      \
        }                                                                               \
    };                                                                                  \
    desc->AddMember(#Name,                                                              \
        std::is_null_pointer<decltype(GetOp)>::value ? nullptr : &_XLUA_ANONYMOUS::Get, \
        std::is_null_pointer<decltype(SetOp)>::value ? nullptr : &_XLUA_ANONYMOUS::Set, \
        IsGlobal                                                                        \
    );
#define _XLUA_IS_STATIC_VAR(GetOp, SetOp)           !xlua::detail::IndexerTrait<decltype(GetOp), decltype(SetOp)>::is_member
#define _XLUA_EXPORT_VAR(Name, GetOp, SetOp, Meta)  _XLUA_EXPORT_VAR_(Name, GetOp, SetOp, Meta, _XLUA_IS_STATIC_VAR(GetOp, SetOp))

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
        xlua::detail::TypeNode _XLUA_ANONYMOUS(&ClassName::xLuaGetTypeInfo);            \
    } /* end namespace */                                                               \
    const xlua::TypeInfo* ClassName::xLuaGetTypeInfo() {                                \
        using class_type = ClassName;                                                   \
        static const xlua::TypeInfo* s_type_info = nullptr;                             \
        if (s_type_info)                                                                \
            return s_type_info;                                                         \
        xlua::detail::GlobalVar* global = xlua::detail::GlobalVar::GetInstance();       \
        if (global == nullptr)                                                          \
            return nullptr;                                                             \
        ITypeDesc* desc = global->AllocType(                                            \
            xlua::TypeCategory::kInternal, false,                                       \
            #ClassName,                                                                 \
            xlua::detail::GetTypeInfoImpl<LuaDeclare::super>()                          \
        );                                                                              \
        if (desc == nullptr)                                                            \
            return nullptr;                                                             \
        desc->SetCaster(                                                                \
            xlua::detail::MakePtrCaster<ClassName, LuaDeclare::SuperType>()             \
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
        xlua::detail::TypeNode _XLUA_ANONYMOUS([]() -> const xlua::TypeInfo* {          \
            return xLuaGetTypeInfo(xlua::Identity<ClassName>());                        \
        });                                                                             \
    } /* end namespace */                                                               \
    const xlua::TypeInfo* xLuaGetTypeInfo(xlua::Identity<ClassName>) {                  \
        using class_type = ClassName;                                                   \
        using super_type = _XLUA_SUPER_CLASS(__VA_ARGS__);                              \
        static const xlua::TypeInfo* s_type_info = nullptr;                             \
        if (s_type_info)                                                                \
            return s_type_info;                                                         \
        auto* global = xlua::detail::GlobalVar::GetInstance();                          \
        if (global == nullptr)                                                          \
            return nullptr;                                                             \
        auto* desc = global->AllocType(xlua::TypeCategory::kExternal,                   \
            xlua::detail::IsWeakObjPtr<ClassName>::value, #ClassName,                   \
            xlua::detail::GetTypeInfoImpl<super_type>()                                 \
        );                                                                              \
        if (desc == nullptr)                                                            \
            return nullptr;                                                             \
        desc->SetCaster(                                                                \
            xlua::detail::MakePtrCaster<ClassName, super_type>()                        \
        );

#define XLUA_EXPORT_EXTERNAL_CLASS_END()                                                \
    XLUA_EXPORT_CLASS_END()

/* 导出全局表 */
#define XLUA_EXPORT_GLOBAL_BEGIN(Name)                                                  \
    namespace {                                                                         \
        xlua::detail::TypeNode _XLUA_ANONYMOUS([]() -> const xlua::TypeInfo* {          \
            using class_type = void;                                                    \
            static const xlua::TypeInfo* s_type_info = nullptr;                         \
            if (s_type_info)                                                            \
                return s_type_info;                                                     \
            auto* global = xlua::detail::GlobalVar::GetInstance();                      \
            if (global == nullptr)                                                      \
                return nullptr;                                                         \
            auto* desc = global->AllocType(xlua::TypeCategory::kGlobal,                 \
                false, #Name, nullptr);                                                 \
            if (desc == nullptr)                                                        \
                return nullptr;                                                         \

#define XLUA_EXPORT_GLOBAL_END()                                                        \
            s_type_info = desc->Finalize();                                             \
            return s_type_info;                                                         \
        });                                                                             \
    }   /* end namespace*/

/* 成员函数, 支持静态成员函数 */
#define XLUA_EXPORT_MEMBER_FUNC(Func)                   _XLUA_EXPORT_FUNC(Func, _EXTRACT_METHOD(&class_type::Func), MetaFunc)
#define XLUA_EXPORT_MEMBER_FUNC_AS(Name, Func, ...)     _XLUA_EXPORT_FUNC(Name, _EXTRACT_METHOD(&class_type::Func, __VA_ARGS__), MetaFunc)
/* 将外部函数包装为成员函数 */
#define XLUA_EXPORT_MEMBER_FUNC_EXTEND(Name, Func)      _XLUA_EXPORT_FUNC_(Name, &Func, MetaFuncEx, false)

/* 成员变量, 支持静态成员变量 */
#define XLUA_EXPORT_MEMBER_VAR(Var)                     _XLUA_EXPORT_VAR(Var, &class_type::Var, &class_type::Var, MetaVar)
#define XLUA_EXPORT_MEMBER_VAR_R(Var, ...)              _XLUA_EXPORT_VAR(Var, _EXTRACT_METHOD(&class_type::Var, __VA_ARGS__)), nullptr, MetaVar)
#define XLUA_EXPORT_MEMBER_VAR_AS(Name, Var)            _XLUA_EXPORT_VAR(Name, &class_type::Var, &class_type::Var, MetaVar)
#define XLUA_EXPORT_MEMBER_VAR_AS_R(Name, Var, ...)     _XLUA_EXPORT_VAR(Name, _EXTRACT_METHOD(&class_type::Var, __VA_ARGS__)), nullptr, MetaVar)
#define XLUA_EXPORT_MEMBER_VAR_AS_W(Name, Var)          _XLUA_EXPORT_VAR(Name, nullptr, &class_type::Var, MetaVar)
#define XLUA_EXPORT_MEMBER_VAR_WRAP(Name, Get, Set)     _XLUA_EXPORT_VAR(Name, &class_type::Get, &class_type::Set, MetaVar)
/* 将外部函数导出为成员变量(非静态) */
#define XLUA_EXPORT_MEMBER_VAR_EXTEND(Name, Get, Set)   _XLUA_EXPORT_VAR_(Name, &Get, &Set, MetaVarEx, false)
#define XLUA_EXPORT_MEMBER_VAR_EXTEND_R(Name, Get)      _XLUA_EXPORT_VAR_(Name, &Get, nullptr, MetaVarEx, false)
#define XLUA_EXPORT_MEMBER_VAR_EXTEND_W(Name, Set)      _XLUA_EXPORT_VAR_(Name, nullptr, &Set, MetaVarEx, false)

/* 全局函数 */
#define XLUA_EXPORT_GLOBAL_FUNC(Func)                   _XLUA_EXPORT_FUNC(Func, &Func, MetaFunc)
#define XLUA_EXPORT_GLOBAL_FUNC_AS(Name, Func)          _XLUA_EXPORT_FUNC(Name, &Func, MetaFunc)
/* 全局变量 */
#define XLUA_EXPORT_GLOBAL_VAR(Var)                     _XLUA_EXPORT_VAR(Var, &Var, &Var, MetaVar)
#define XLUA_EXPORT_GLOBAL_VAR_R(Var)                   _XLUA_EXPORT_VAR(Var, &Var, nullptr, MetaVar)
#define XLUA_EXPORT_GLOBAL_VAR_AS(Name, Var)            _XLUA_EXPORT_VAR(Name, &Var, &Var, MetaVar)
#define XLUA_EXPORT_GLOBAL_VAR_AS_R(Name, Var)          _XLUA_EXPORT_VAR(Name, &Var, nullptr, MetaVar)
#define XLUA_EXPORT_GLOBAL_VAR_AS_W(Name, Var)          _XLUA_EXPORT_VAR(Name, nullptr, &Var, MetaVar)
#define XLUA_EXPORT_GLOBAL_VAR_WARP(Name, Get, Set)     _XLUA_EXPORT_VAR(Name, &Get, &Set, MetaVar)
#define XLUA_EXPORT_GLOBAL_VAR_WARP_R(Name, Get)        _XLUA_EXPORT_VAR(Name, &Get, nullptr, MetaVar)
#define XLUA_EXPORT_GLOBAL_VAR_WARP_W(Name, Set)        _XLUA_EXPORT_VAR(Name, nullptr, &Set, MetaVar)


/* 导出常量表 */
#define _XLUA_EXPORT_CONST_BEGIN(Name)                                                  \
    namespace {                                                                         \
        xlua::detail::ConstNode _XLUA_ANONYMOUS([]() -> const xlua::ConstInfo* {        \
            static xlua::ConstInfo info;                                                \
            info.name = #Name;

#define XLUA_EXPORT_CONST_BEGIN(Name)                                                   \
    _XLUA_EXPORT_CONST_BEGIN(Name)                                                      \
            static xlua::ConstValue values[] = {

#define XLUA_EXPORT_CONST_END()                                                         \
                xlua::detail::MakeConstValue()                                          \
            };                                                                          \
            info.values = values;                                                       \
            return &info;                                                               \
        }); /* end function */                                                          \
    } /* end namespace*/

#define XLUA_EXPORT_CONST_VAR(Name, Val) xlua::detail::MakeConstValue(#Name, Val),

/* 导出枚举表 */
#define XLUA_EXPORT_ENUM_BEGIN_AS(Name, EnumType)                                       \
    _XLUA_EXPORT_CONST_BEGIN(Name)                                                      \
            using enum_type = EnumType;                                                 \
            static xlua::ConstValue values[] = {

#define XLUA_EXPORT_ENUM_BEGIN(EnumType)    XLUA_EXPORT_ENUM_BEGIN_AS(EnumType, EnumType)
#define XLUA_EXPORT_ENUM_END()              XLUA_EXPORT_CONST_END()

#define XLUA_EXPORT_ENUM_VAR_AS(Name, Val)  XLUA_EXPORT_CONST_VAR(Name, (int)enum_type::Val)
#define XLUA_EXPORT_ENUM_VAR(Val)           XLUA_EXPORT_ENUM_VAR_AS(Var, Val)

/* 导出预设脚本 */
#define XLUA_EXPORT_SCRIPT(Str)                                                         \
    namespace {                                                                         \
        xlua::detail::ScriptNode _XLUA_ANONYMOUS(Str);                                  \
    }

