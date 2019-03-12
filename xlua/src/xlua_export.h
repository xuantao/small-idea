#pragma once

#include "xlua_def.h"

XLUA_NAMESPACE_BEGIN

namespace detail {
    typedef const TypeInfo* (*GetInfoFunc)();
    class GlobalVar;

    class TypeNode
    {
        friend class GlobalVar;
    protected:
        TypeNode(GetInfoFunc f);
        ~TypeNode();

        TypeNode(const TypeNode&) = delete;
        TypeNode& operator = (const TypeNode&) = delete;

    private:
        TypeNode* next;
        GetInfoFunc func;
    };

    /* 获取基类导出lua类 */
    template <typename Ty> inline const TypeInfo* GetSuper() { return Ty::LuaGetTypeInfo(); }
    template <> inline const TypeInfo* GetSuper<void>() { return nullptr; }
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


#define XLUA_EXPORT_FUNC()
#define XLUA_EXPORT_VAR()
//#define XLUA_EXPORT_