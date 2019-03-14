#pragma once
#include "xlua_def.h"
#include "xlua_traits.h"

XLUA_NAMESPACE_BEGIN

namespace detail {
    typedef const TypeInfo* (*fnGetInfo)();
    class GlobalVar;

    template <typename Ty>
    struct TypeInfoFunc {
        static const TypeInfo* GetInfo() {
            return xLuaGetTypeInfo(xlua::Indetity<Ty>());
        }
    };

    class TypeNode
    {
        friend class GlobalVar;
    public:
        TypeNode(fnGetInfo func);
        ~TypeNode();

        TypeNode(const TypeNode&) = delete;
        TypeNode& operator = (const TypeNode&) = delete;

    private:
        TypeNode* next_;
        fnGetInfo func_;
    };

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
