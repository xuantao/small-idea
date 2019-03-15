#pragma once
#include "xlua_def.h"
#include "xlua_traits.h"

XLUA_NAMESPACE_BEGIN

struct MemberFunc {
    const char* name;
    LuaFunction func;
};

struct MemberVar {
    const char* name;
    LuaIndexer getter;
    LuaIndexer setter;
};

struct TypeInfo {
    int index;
    const char* name;
    const TypeInfo* super;
    MemberVar* vars;
    MemberFunc* funcs;
    MemberVar* static_vars;
    MemberFunc* static_funcs;
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

    template <typename Ty>
    struct TypeInfoFunc {
        static const TypeInfo* GetInfo() {
            return xLuaGetTypeInfo(xlua::Indetity<Ty>());
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
        ConstNode(const ConstInfo* info)
            : NodeBase(NodeType::kConst)
            , info_(info) {
        }
        const ConstInfo* const info_;
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

    /* 获取基类导出lua类 */
    template <typename Ty> inline const TypeInfo* GetSuper() { return Ty::LuaGetTypeInfo(); }
    template <> inline const TypeInfo* GetSuper<void>() { return nullptr; }

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
} // namespace detail

XLUA_NAMESPACE_END

/* 导出Lua类开始 */
#define XLUA_EXPORT_BEGIN(ClassName)                                                            \
    namespace {                                                                                 \
        static int lua_##ClassName##_index_ = -1;                                               \
        struct Lua##ClassName##Node : private xlua::detail::TypeNode {                          \
            Lua##ClassName##Node()                                                              \
                : xlua::detail::TypeNode(&ClassName::LuaGetTypeInfo) { }                        \
            ~Lua##ClassName##Node() { lua_##ClassName##_index_ = -1; }                          \
        } lua_##ClassName##_node_;                                                              \
    }                                                                                           \
    const xlua::TypeInfo* ClassName::LuaGetTypeInfo() {                                         \
        if (lua_##ClassName##_index_ != -1)                                                     \
            return xlua::GetTypeInfo(lua_##ClassName##_index_);                                 \
        ITypeDesc* desc = xlua::AllocTypeInfo(                                                  \
            #ClassName, xlua::detail::GetSupper<LuaDeclare::SuperType>());                      \
        if (desc == nullptr)                                                                    \
            return nullptr;                                                                     \
        using ClassType = ClassName;


/* 导出lua类结束 */
#define XLUA_EXPORT_END()                                                                       \
        lua_##ClassName##_index_ = desc->Finalize();                                            \
        return xlua::GetTypeInfo(lua_##ClassName##_index_);                                     \
    }

// 导出实现
#define _XLUA_EXPORT_FUNCTION(Name, Func)                                               \
    static_assert(!std::is_null_pointer<decltype(Func)>::value,                         \
        "can not export func:"##Name##" with null pointer"                              \
    );                                                                                  \
    struct /* export */ Name {                                                          \
        static int call(xLuaState* L, void* obj) {                                      \
            return xlua::detail::MetaCall(L, obj Func);                                 \
        }                                                                               \
    };                                                                                  \
    desc->AddFunc(#Name, &Name::call, std::is_member_function<decltype(Func)>::value);

#define _XLUA_EXPORT_VAR(Name, GetOp, SetOp)                                            \
    static_assert(                                                                      \
        xlua::detail::IndexerTrait<decltype(GetOp), decltype(SetOp)>::accept,           \
        "can not export var:"##Name##" to lua"                                          \
    );                                                                                  \
    struct /* export */ Name {                                                          \
        static void get(lua_State* L, void* obj) {                                      \
            xlua::detail::MetaGet(L, obj, GetOp);                                       \
        }                                                                               \
        static void set(lua_State* L, void* obj) {                                      \
            xlua::detail::MetaSet(L, obj, SetOp);                                       \
        }                                                                               \
    };                                                                                  \
    desc->AddVar(#Name,                                                                 \
        std::is_null_pointer<decltype(GetOp)>::value ? nullptr : &Name::get,            \
        std::is_null_pointer<decltype(SetOp)>::value ? nullptr : &Name::set,            \
        xlua::detail::IndexerTrait<decltype(GetOp), decltype(SetOp)>::member            \
    );

#define XLUA_EXPORT_FUNC()
#define XLUA_EXPORT_VAR()
//#define XLUA_EXPORT_

/* 导出常量表 */
#define XLUA_EXPORT_CONST_BEGIN(Name)                           \
    namespace {                                                 \
        static const xlua::ConstInfo* sGetConstInfo##Name();    \
        xlua::detail::ConstNode const_node_##__line__(          \
            sGetConstInfo##Name());                             \
        const xlua::ConstInfo* sGetConstInfo##Name() {          \
        static xlua::ConstInfo info;                            \
        info.name = #Name;                                      \
        static xlua::ConstValue values[] =  {

#define XLUA_EXPORT_CONST_END()                                 \
                xlua::detail::MakeConstValue()                  \
            }                                                   \
            info.values = values;                               \
            return &info;                                       \
        } /* end function */                                    \
    } /* end namespace*/

#define XLUA_EXPORT_CONST_VAR(Name, Val) xlua::detail::MakeConstValue(#Name, Val),

/* 导出枚举表 */
#define XLUA_EXPORT_ENUM_BEGIN_AS(Name, EnumType)               \
    XLUA_EXPORT_CONST_BEGIN(Name)                               \
    typedef EnumType Type;

#define XLUA_EXPORT_ENUM_BEGIN(EnumType)    XLUA_EXPORT_ENUM_BEGIN_AS(EnumType, EnumType)
#define XLUA_EXPORT_ENUM_END()              XLUA_EXPORT_CONST_END()

#define XLUA_EXPORT_ENUM_VAR_AS(Name, Val)  XLUA_EXPORT_CONST_VAR(Name, (int)Type::Val)
#define XLUA_EXPORT_ENUM_VAR(Val)           XLUA_EXPORT_ENUM_VAR_AS(Var, Val)

/* 导出预设脚本 */
#define XLUA_EXPORT_SCRIPT(Str)                                 \
    namespace {                                                 \
        xlua::detail::ScriptNode script_node_##__line__(Str);   \
    }

