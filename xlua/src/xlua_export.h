#pragma once
#include "xlua_def.h"
#include "xlua_traits.h"

XLUA_NAMESPACE_BEGIN

struct TypeFunc {
    const char* name;
    LuaFunction func;
};

struct TypeVar {
    const char* name;
    LuaIndexer getter;
    LuaIndexer setter;
};

struct TypeInfo {
    const char* name;
    const TypeInfo* super;
    TypeVar* vars;
    TypeVar* static_vars;
    TypeFunc* funcs;
    TypeFunc* static_funcs;
    LuaCastCheck fnCast;
};

enum class ConstValueType {
    kNone,
    kInteger,
    kFloat,
    kString,
};

struct ConstValue {
    ConstValueType type;
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

namespace detail {
    class GlobalVar;
    typedef const TypeInfo* (*fnTypeInfo)();
    typedef const ConstInfo* (*fnConstInfo)();

    template <typename Ty>
    struct TypeInfoFunc {
        static const TypeInfo* GetInfo() {
            return xLuaGetTypeInfo(Identity<Ty>());
        }
    };

    enum class NodeType {
        kType,
        kConst,
        kScript,
    };

    struct NodeBase {
        NodeBase(NodeType type);
        ~NodeBase();

        NodeType type_;
        NodeBase* next_;
    };

    struct TypeNode : NodeBase {
        TypeNode(fnTypeInfo func)
            : NodeBase(NodeType::kType)
            , func_(func) {
        }
        const fnTypeInfo func_;
    };

    struct ConstNode : NodeBase {
        ConstNode(fnConstInfo func)
            : NodeBase(NodeType::kConst)
            , func_(func) {
        }
        const fnConstInfo func_;
    };

    struct ScriptNode : NodeBase {
        ScriptNode(const char* script)
            : NodeBase(NodeType::kScript)
            , script_(script) {
        }
        const char* const script_;
    };

    inline ConstValue MakeConstValue() {
        ConstValue cv;
        cv.type = ConstValueType::kNone;
        cv.name = nullptr;
        cv.int_val = 0;
        return cv;
    }

    inline ConstValue MakeConstValue(const char* name, int val) {
        ConstValue cv;
        cv.type = ConstValueType::kInteger;
        cv.name = name;
        cv.int_val = val;
        return cv;
    }

    inline ConstValue MakeConstValue(const char* name, float val) {
        ConstValue cv;
        cv.type = ConstValueType::kFloat;
        cv.name = name;
        cv.float_val = val;
        return cv;
    }

    inline ConstValue MakeConstValue(const char* name, const char* val) {
        ConstValue cv;
        cv.type = ConstValueType::kString;
        cv.name = name;
        cv.string_val = val;
        return cv;
    }

    /* 导出属性萃取 */
    template <typename Ly, typename Ry>
    struct IndexerTrait
    {
        static constexpr bool accept =
            (std::is_null_pointer<Ly>::value != std::is_null_pointer<Ry>::value) &&
            ((std::is_null_pointer<Ly>::value || std::is_null_pointer<Ry>::value)
                ||
                ((std::is_member_pointer<Ly>::value || std::is_member_function_pointer<Ly>::value)
                    ==
                    (std::is_member_pointer<Ry>::value || std::is_member_function_pointer<Ry>::value)
                    ));

        static constexpr bool member = (
            std::is_member_pointer<Ly>::value || std::is_member_function_pointer<Ly>::value
            || std::is_member_pointer<Ry>::value || std::is_member_function_pointer<Ry>::value
                );
    };

    template <typename Ty, typename By>
    struct CastCheck {
        static bool Check(void* obj, const TypeInfo* obj_info, const TypeInfo* target_info) {
            if (obj_info == target_info)
                return true;

            const TypeInfo* super = target_info->super;
            return super && super->fnCast
                && super->fnCast(obj, obj_info, super)
                && dynamic_cast<Ty*>(static_cast<By*>(obj)) != nullptr;
        }
    };

    template <typename Ty>
    struct CastCheck<Ty, void> {
        static bool Check(void*, const TypeInfo* obj_info, const TypeInfo* target_info) {
            return obj_info == target_info;
        }
    };
} // namespace detail

XLUA_NAMESPACE_END


#define _XLUA_SUPER_CLASS(...)  typename xlua::detail::BaseType<_VAR_ARGS_>::type

// 导出实现
#define _XLUA_EXPORT_FUNCTION(Name, Func)                                               \
    static_assert(!std::is_null_pointer<decltype(Func)>::value,                         \
        "can not export func:"##Name##" with null pointer"                              \
    );                                                                                  \
    int export_to_xlua_##Name = 0;                                                      \
    (export_to_xlua_##Name);    /* used for check rename */                             \
    struct Func_##Name {                                                                  \
        static int call(xLuaState* L) {                                                 \
            return xlua::detail::MetaCall(L, Func);                                     \
        }                                                                               \
    };                                                                                  \
    desc->AddFunc(#Name, &Func_##Name::call, std::is_member_function<decltype(Func)>::value);

#define _XLUA_EXPORT_VAR(Name, GetOp, SetOp)                                            \
    static_assert(                                                                      \
        xlua::detail::IndexerTrait<decltype(GetOp), decltype(SetOp)>::accept,           \
        "can not export var:"##Name##" to lua"                                          \
    );                                                                                  \
    int export_to_xlua_##Name = 0;                                                      \
    (export_to_xlua_##Name);    /* used for check rename */                             \
    struct Var_##Name {                                                                 \
        static void get(lua_State* L, void* obj) {                                      \
            xlua::detail::MetaGet(L, obj, GetOp);                                       \
        }                                                                               \
        static void set(lua_State* L, void* obj) {                                      \
            xlua::detail::MetaSet(L, obj, SetOp);                                       \
        }                                                                               \
    };                                                                                  \
    desc->AddVar(#Name,                                                                 \
        std::is_null_pointer<decltype(GetOp)>::value ? nullptr : &Var_##Name::get,      \
        std::is_null_pointer<decltype(SetOp)>::value ? nullptr : &Var_##Name::set,      \
        xlua::detail::IndexerTrait<decltype(GetOp), decltype(SetOp)>::member            \
    );

/* 导出Lua类开始 */
#define XLUA_EXPORT_CLASS_BEGIN(ClassName)                                              \
    static_assert(std::is_void<ClassName::LuaDeclare::super>::type                      \
        || std::is_base_of<ClassName::LuaDeclare::super, ClassName>::value,             \
        "external class is not inherited"                                               \
    );                                                                                  \
    static_assert(std::is_void<ClassName::LuaDeclare::super>::type                      \
        || xlua::detail::IsInternal<ClassName::LuaDeclare::super, ClassName>::value     \
        || xlua::detail::IsExternal<ClassName::LuaDeclare::super, ClassName>::value     \
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
        desc->SetCastCheck(                                                             \
            &xlua::detail::CastCheck<ClassName, LuaDeclare::super>::Check               \
        );                                                                              \
        using class_type = ClassName;


/* 导出lua类结束 */
#define XLUA_EXPORT_CLASS_END()                                                         \
        s_key = desc->Finalize();                                                       \
        return xlua::GetTypeInfo(s_key);                                                \
    }

/* 导出外部类 */
#define XLUA_EXPORT_EXTERNAL_CLASS_BEGIN(ClassName, ...)                                \
    static_assert(std::is_void<typename xlua::BaseType<_VAR_ARGS_>::type                \
        || std::is_base_of<_XLUA_SUPER_CLASS(_VAR_ARGS_), ClassName>::value,            \
        "external class is not inherited"                                               \
    );                                                                                  \
    static_assert(std::is_void<typename xlua::BaseType<_VAR_ARGS_>::type                \
        || xlua::detail::IsInternal<_XLUA_SUPER_CLASS(_VAR_ARGS_), ClassName>::value    \
        || xlua::detail::IsExternal<_XLUA_SUPER_CLASS(_VAR_ARGS_), ClassName>::value    \
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
        ITypeDesc* desc = xlua::AllocTypeInfo(                                          \
            #ClassName,                                                                 \
            xlua::detail::GetTypeInfo<typename BaseType<_VAR_ARGS_>::type>()            \
        );                                                                              \
        if (desc == nullptr)                                                            \
            return nullptr;                                                             \
        desc->SetCastCheck(                                                             \
            &xlua::detail::CastCheck<ClassName, LuaDeclare::SuperType>::Check           \
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
            ITypeDesc* desc = xlua::AllocTypeInfo(#Name, nullptr);                      \
            if (desc == nullptr)                                                        \
                return nullptr;                                                         \

#define XLUA_EXPORT_GLOBAL_END()                                                        \
            s_key = desc->Finalize();                                                   \
            return xlua::GetTypeInfo(s_key);                                            \
        });                                                                             \
    }   /* end namespace*/

#define XLUA_EXPORT_FUNC()
#define XLUA_EXPORT_VAR()
//#define XLUA_EXPORT_

/* 导出常量表 */

#define _XLUA_EXPORT_CONST_BEGIN(Name)                          \
    namespace {                                                 \
        xlua::detail::ConstNode const_node_##__LINE__(          \
        []() -> const ConstInfo* {                              \
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

