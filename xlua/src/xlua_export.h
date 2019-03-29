#pragma once
#include "xlua_def.h"
#include "detail/traits.h"
#include "detail/export.h"

/* 匿名*/
#define _XLUA_ANONYMOUS_2(line) Anonymous##line
#define _XLUA_ANONYMOUS_1(line) _XLUA_ANONYMOUS_2(line)
#define _XLUA_ANONYMOUS         _XLUA_ANONYMOUS_1(__LINE__)

#define _XLUA_SUPER_CLASS(...)  typename XLUA_USE_NAMESPCE detail::BaseType<__VA_ARGS__>::type

// 导出实现
#define _XLUA_EXPORT_FUNC(Name, Func, Meta)                                                     \
    static_assert(!std::is_null_pointer<decltype(Func)>::value,                                 \
        "can not export func:"#Name" with null pointer"                                         \
    );                                                                                          \
    struct _XLUA_ANONYMOUS {                                                                    \
        static int call(lua_State* l) {                                                         \
            XLUA_USE_NAMESPCE xLuaState* xl =                                                   \
                XLUA_USE_NAMESPCE detail::GlobalVar::GetInstance()->GetState(l);                \
            assert(xl);                                                                         \
            return XLUA_USE_NAMESPCE detail::Meta<class_type>::Call(xl, s_type_info, Func);     \
        }                                                                                       \
    };                                                                                          \
    desc->AddMember(#Name, &_XLUA_ANONYMOUS::call,                                              \
        !std::is_member_function_pointer<decltype(Func)>::value);

#define _XLUA_EXPORT_VAR(Name, GetOp, SetOp, Meta)                                              \
    static_assert(                                                                              \
        XLUA_USE_NAMESPCE detail::IndexerTrait<decltype(GetOp), decltype(SetOp)>::is_allow,     \
        "can not export var:"#Name" to lua"                                                     \
    );                                                                                          \
    struct _XLUA_ANONYMOUS {                                                                    \
        static void Get(XLUA_USE_NAMESPCE xLuaState* l, void* obj, const XLUA_USE_NAMESPCE TypeInfo* info) {      \
            XLUA_USE_NAMESPCE detail::Meta<class_type>::Get(l,                                  \
                (class_type*)info->caster.to_super(obj, info, s_type_info), GetOp);                  \
        }                                                                                       \
        static void Set(XLUA_USE_NAMESPCE xLuaState* l, void* obj, const XLUA_USE_NAMESPCE TypeInfo* info) {      \
            XLUA_USE_NAMESPCE detail::Meta<class_type>::Set(l,                                  \
                (class_type*)info->caster.to_super(obj, info, s_type_info), SetOp);                  \
        }                                                                                       \
    };                                                                                          \
    desc->AddMember(#Name,                                                                      \
        std::is_null_pointer<decltype(GetOp)>::value ? nullptr : &_XLUA_ANONYMOUS::Get,         \
        std::is_null_pointer<decltype(SetOp)>::value ? nullptr : &_XLUA_ANONYMOUS::Set,         \
        !XLUA_USE_NAMESPCE detail::IndexerTrait<decltype(GetOp), decltype(SetOp)>::is_member    \
    );

/* 导出Lua类开始 */
#define XLUA_EXPORT_CLASS_BEGIN(ClassName)                                                      \
    static_assert(std::is_void<ClassName::LuaDeclare::super>::type                              \
        || std::is_base_of<ClassName::LuaDeclare::super, ClassName>::value,                     \
        "internal class is not inherited"                                                       \
    );                                                                                          \
    static_assert(std::is_void<ClassName::LuaDeclare::super>::type                              \
        || XLUA_USE_NAMESPCE detail::IsInternal<ClassName::LuaDeclare::super>::value            \
        || XLUA_USE_NAMESPCE detail::IsExternal<ClassName::LuaDeclare::super>::value            \
        "base type is not declare to export to lua"                                             \
    );                                                                                          \
    namespace {                                                                                 \
        XLUA_USE_NAMESPCE detail::TypeNode _XLUA_ANONYMOUS(&ClassName::xLuaGetTypeInfo);        \
    } /* end namespace */                                                                       \
    const XLUA_USE_NAMESPCE TypeInfo* ClassName::xLuaGetTypeInfo() {                            \
        using class_type = ClassName;                                                           \
        static const XLUA_USE_NAMESPCE TypeInfo* s_type_info = nullptr;                         \
        if (s_type_info)                                                                        \
            return s_type_info;                                                                 \
        XLUA_USE_NAMESPCE detail::GlobalVar* global =                                           \
            XLUA_USE_NAMESPCE detail::GlobalVar::GetInstance();                                 \
        if (global == nullptr)                                                                  \
            return nullptr;                                                                     \
        ITypeDesc* desc = global->AllocType(                                                    \
            XLUA_USE_NAMESPCE TypeCategory::kInternal, false,                                   \
            #ClassName,                                                                         \
            XLUA_USE_NAMESPCE detail::GetTypeInfoImpl<LuaDeclare::super>()                      \
        );                                                                                      \
        if (desc == nullptr)                                                                    \
            return nullptr;                                                                     \
        desc->SetCaster(                                                                        \
            XLUA_USE_NAMESPCE detail::MakePtrCaster<ClassName, LuaDeclare::SuperType>()         \
        );

/* 导出lua类结束 */
#define XLUA_EXPORT_CLASS_END()                                                                 \
        s_type_info = desc->Finalize();                                                         \
        return s_type_info;                                                                     \
    }

/* 导出外部类 */
#define XLUA_EXPORT_EXTERNAL_CLASS_BEGIN(ClassName, ...)                                        \
    static_assert(std::is_void<_XLUA_SUPER_CLASS(__VA_ARGS__)>::value                           \
        || std::is_base_of<_XLUA_SUPER_CLASS(__VA_ARGS__), ClassName>::value,                   \
        "external class is not inherited"                                                       \
    );                                                                                          \
    static_assert(std::is_void<_XLUA_SUPER_CLASS(__VA_ARGS__)>::value                           \
        || XLUA_USE_NAMESPCE detail::IsInternal<_XLUA_SUPER_CLASS(__VA_ARGS__)>::value          \
        || XLUA_USE_NAMESPCE detail::IsExternal<_XLUA_SUPER_CLASS(__VA_ARGS__)>::value,         \
        "base type is not declare to export to lua"                                             \
    );                                                                                          \
    namespace {                                                                                 \
        XLUA_USE_NAMESPCE detail::TypeNode _XLUA_ANONYMOUS(                                     \
            []() -> const XLUA_USE_NAMESPCE TypeInfo* {                                         \
                return xLuaGetTypeInfo(XLUA_USE_NAMESPCE Identity<ClassName>());                \
        });                                                                                     \
    } /* end namespace */                                                                       \
    const XLUA_USE_NAMESPCE TypeInfo* xLuaGetTypeInfo(XLUA_USE_NAMESPCE Identity<ClassName>) {  \
        using class_type = ClassName;                                                           \
        using super_type = _XLUA_SUPER_CLASS(__VA_ARGS__);                                      \
        static const XLUA_USE_NAMESPCE TypeInfo* s_type_info = nullptr;                         \
        if (s_type_info)                                                                        \
            return s_type_info;                                                                 \
        auto* global = XLUA_USE_NAMESPCE detail::GlobalVar::GetInstance();                      \
        if (global == nullptr)                                                                  \
            return nullptr;                                                                     \
        auto* desc = global->AllocType(XLUA_USE_NAMESPCE TypeCategory::kExternal,               \
            XLUA_USE_NAMESPCE detail::IsWeakObjPtr<ClassName>::value, #ClassName,               \
            XLUA_USE_NAMESPCE detail::GetTypeInfoImpl<super_type>()                             \
        );                                                                                      \
        if (desc == nullptr)                                                                    \
            return nullptr;                                                                     \
        desc->SetCaster(                                                                        \
            XLUA_USE_NAMESPCE detail::MakePtrCaster<ClassName, super_type>()                    \
        );

#define XLUA_EXPORT_EXTERNAL_CLASS_END()                                                        \
    XLUA_EXPORT_CLASS_END()

/* 导出全局表 */
#define XLUA_EXPORT_GLOBAL_BEGIN(Name)                                                          \
    namespace {                                                                                 \
        XLUA_USE_NAMESPCE detail::TypeNode _XLUA_ANONYMOUS(                                     \
        []() -> const XLUA_USE_NAMESPCE TypeInfo* {                                             \
            static const XLUA_USE_NAMESPCE TypeInfo* s_type_info = nullptr;                     \
            if (s_type_info)                                                                    \
                return s_type_info;                                                             \
            auto* global = XLUA_USE_NAMESPCE detail::GlobalVar::GetInstance();                  \
            if (global == nullptr)                                                              \
                return nullptr;                                                                 \
            auto* desc = global->AllocType(XLUA_USE_NAMESPCE TypeCategory::kGlobal,             \
                false, #Name, nullptr);                                                         \
            if (desc == nullptr)                                                                \
                return nullptr;                                                                 \

#define XLUA_EXPORT_GLOBAL_END()                                                                \
            s_type_info = desc->Finalize();                                                     \
            return s_type_info;                                                                 \
        });                                                                                     \
    }   /* end namespace*/

#define XLUA_EXPORT_MEMBER_FUNC(Func)   _XLUA_EXPORT_FUNC(Func, &class_type::Func, MetaFunc)
#define XLUA_EXPORT_MEMBER_VAR(Var)     _XLUA_EXPORT_VAR(Var, &class_type::Var, &class_type::Var, MetaVar)
#define XLUA_EXPORT_GLOBAL_FUNC(Func)   _XLUA_EXPORT_FUNC(Func, &Func, MetaFunc)
#define XLUA_EXPORT_GLOBAL_VAR(Var)     _XLUA_EXPORT_VAR(Var, &Var, &Var, MetaVar)
//#define XLUA_EXPORT_

/* 导出常量表 */

#define _XLUA_EXPORT_CONST_BEGIN(Name)                          \
    namespace {                                                 \
        XLUA_USE_NAMESPCE detail::ConstNode _XLUA_ANONYMOUS(    \
        []() -> const XLUA_USE_NAMESPCE ConstInfo* {            \
            static XLUA_USE_NAMESPCE ConstInfo info;            \
            info.name = #Name;

#define XLUA_EXPORT_CONST_BEGIN(Name)                           \
    _XLUA_EXPORT_CONST_BEGIN(Name)                              \
            static XLUA_USE_NAMESPCE ConstValue values[] = {

#define XLUA_EXPORT_CONST_END()                                 \
                XLUA_USE_NAMESPCE detail::MakeConstValue()      \
            };                                                  \
            info.values = values;                               \
            return &info;                                       \
        }); /* end function */                                  \
    } /* end namespace*/

#define XLUA_EXPORT_CONST_VAR(Name, Val) XLUA_USE_NAMESPCE detail::MakeConstValue(#Name, Val),

/* 导出枚举表 */
#define XLUA_EXPORT_ENUM_BEGIN_AS(Name, EnumType)               \
    _XLUA_EXPORT_CONST_BEGIN(Name)                              \
            using enum_type = EnumType;                         \
            static XLUA_USE_NAMESPCE ConstValue values[] = {

#define XLUA_EXPORT_ENUM_BEGIN(EnumType)    XLUA_EXPORT_ENUM_BEGIN_AS(EnumType, EnumType)
#define XLUA_EXPORT_ENUM_END()              XLUA_EXPORT_CONST_END()

#define XLUA_EXPORT_ENUM_VAR_AS(Name, Val)  XLUA_EXPORT_CONST_VAR(Name, (int)enum_type::Val)
#define XLUA_EXPORT_ENUM_VAR(Val)           XLUA_EXPORT_ENUM_VAR_AS(Var, Val)

/* 导出预设脚本 */
#define XLUA_EXPORT_SCRIPT(Str)                                     \
    namespace {                                                     \
        XLUA_USE_NAMESPCE detail::ScriptNode _XLUA_ANONYMOUS(Str);  \
    }

