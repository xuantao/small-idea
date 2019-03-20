#pragma once
#include "../xlua_def.h"
#include "../xlua.h"
#include "state.h"
#include "global.h"
#include "traits.h"

XLUA_NAMESPACE_BEGIN

namespace detail
{
    class GlobalVar;
    typedef const TypeInfo* (*fnTypeInfo)();
    typedef const ConstInfo* (*fnConstInfo)();

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

    inline TypeMember MakeMember() {
        TypeMember mem;
        mem.type = MemberType::kInvalid;
        mem.name = nullptr;
        return mem;
    }

    inline TypeMember MakeMember(const char* name, LuaFunction func) {
        TypeMember mem;
        mem.type = MemberType::kFunction;
        mem.name = name;
        mem.func = func;
        return mem;
    }

    inline TypeMember MakeMember(const char* name, LuaIndexer getter, LuaIndexer setter) {
        TypeMember mem;
        mem.type = MemberType::kVariate;
        mem.name = name;
        mem.getter = getter;
        mem.setter = setter;
        return mem;
    }

    /* 导出属性萃取 */
    template <typename GetTy, typename SetTy>
    struct IndexerTrait
    {
    private:
        static constexpr bool all_nullptr = (std::is_null_pointer<GetTy>::value && std::is_null_pointer<SetTy>::value);
        static constexpr bool one_nullptr = (std::is_null_pointer<GetTy>::value || std::is_null_pointer<SetTy>::value);
        static constexpr bool is_get_member = (std::is_member_pointer<GetTy>::value || std::is_member_function_pointer<GetTy>::value);
        static constexpr bool is_set_member = (std::is_member_pointer<SetTy>::value || std::is_member_function_pointer<SetTy>::value);

    public:
        static constexpr bool is_allow = !all_nullptr && (one_nullptr || (is_get_member == is_set_member));
        static constexpr bool is_member = (is_get_member || is_set_member);
    };

    /* 多继承指针偏移, 这里一步步的执行转换 */
    template <typename Ty, typename By>
    struct PtrCast {
        static void* CastUp(void* obj, const TypeInfo* src, const TypeInfo* dst) {
            if (src == dst)
                return obj;
            return src->super->converter.convert_up(static_cast<By*>((Ty*)obj), src->super, dst);
        }

        static void* CastDown(void* obj, const TypeInfo* src, const TypeInfo* dst) {
            if (src == dst)
                return obj;
            obj = dst->super->converter.convert_down(obj, src, dst->super);
            return obj ? dynamic_cast<Ty*>(static_cast<By*>(obj)) : nullptr;
        }
    };

    template <typename Ty>
    struct PtrCast<Ty, void> {
        static void* CastUp(void* obj, const TypeInfo* src, const TypeInfo* dst) {
            return src == dst ? obj : nullptr;
        }

        static void* CastDown(void* obj, const TypeInfo* src, const TypeInfo* dst) {
            return src == dst ? obj : nullptr;
        }
    };

    template <typename Ty, typename By>
    struct SharedPtrCast {
        static bool Cast(void* src, const TypeInfo* src_type, void* dst, const TypeInfo* dst_type) {
            if (src_type == dst_type) {
                *(std::shared_ptr<Ty>*)dst = *(std::shared_ptr<Ty>*)src;
                return true;
            }

            std::shared_ptr<By> tmp = std::static_pointer_cast<By>(*(std::shared_ptr<Ty>*)src);
            return src_type->super->converter.convert_shared_ptr(&tmp, src_type->super, dst, dst_type);
        }
    };

    template <typename Ty>
    struct SharedPtrCast<Ty, void> {
        static bool Cast(void* src, const TypeInfo* src_type, void* dst, const TypeInfo* dst_type) {
            if (src_type != dst_type)
                return false;
            *(std::shared_ptr<Ty>*)dst = *(std::shared_ptr<Ty>*)src;
            return true;
        }
    };

    template <typename Ty>
    inline Ty* GetMetaCallFullUserDataPtr(void* user_data, const TypeInfo* info) {
        LuaUserData* ud = static_cast<LuaUserData*>(user_data);
        if (ud->obj_ == nullptr) {
            return nullptr;
        } else if (!IsBaseOf(info, ud->info_)) {
            return nullptr;
        }
        return static_assert<Ty*>(ud->info_->converter.convert_up(ud->obj_, ud->info_, info));
    }

    template <typename Ty>
    inline Ty* GetMetaCallObj(xLuaState* l, const TypeInfo* info) {
        void* user_data = l->GetUserDataPtr(1);
        if (user_data == nullptr)
            return nullptr;

        switch (l->GetValueType(1))
        {
        case xlua::LuaValueType::kLightUserData:
            return GetLightUserDataPtr<Ty>(l->GetUserDataPtr(1), info);
        case xlua::LuaValueType::kFullUserData:
            return GetMetaCallFullUserDataPtr(user_data, info);
        default:
            break;
        }
        //TODO: log
        return nullptr;
    }

    template <typename Ty>
    inline int MetaCall(xLuaState* L, Ty f) {
        return 0;
    }

    template <typename Ty>
    inline int MetaGet(xLuaState* L, void* obj, Ty f) {
        return 0;
    }

    template <typename Ty>
    inline int MetaSet(xLuaState* L, void* obj, Ty f) {
        return 0;
    }

    template<typename Rty, typename Cty, typename... Args, size_t... Idxs>
    inline int _MetaCall(xLuaState* l, Cty* obj, Rty(Cty::*func)(Args...), index_sequence<Idxs...>)
    {
        l->Push(
            (obj->*func)(l->Load<typename std::decay<Args>::type>(L, Idxs + 1)...)
        );
        return 1;
    }

    template<typename Rty, typename Cty, typename... Args, size_t... Idxs>
    inline int _MetaCall(xLuaState* l, Cty* obj, Rty(Cty::*func)(Args...) const, index_sequence<Idxs...>)
    {
        l->Push(L,
            (obj->*func)(l->Load<typename std::decay<Args>::type>(L, Idxs + 1)...)
        );
        return 1;
    }

    template <typename Cty, typename... Args, size_t... Idxs>
    inline int _MetaCall(xLuaState* l, Cty* obj, void(Cty::*func)(Args...), index_sequence<Idxs...>)
    {
        (obj->*func)(l->Load<typename std::decay<Args>::type>(L, Idxs + 1)...);
        return 0;
    }

    template <typename Cty, typename... Args, size_t... Idxs>
    inline int _MetaCall(xLuaState* l, void* obj, void(Cty::*func)(Args...) const, index_sequence<Idxs...>)
    {
        (obj->*func)(l->Load<typename std::decay<Args>::type>(L, Idxs + 1)...);
        return 0;
    }

    template <typename Cty, typename... Args, size_t... Idxs>
    inline int _MetaCallEx(xLuaState* l, Cty* obj, void(*func)(Args...), index_sequence<Idxs...>)
    {
        //TODO:
        using arg_tuple = std::tuple<Args...>;
        func(obj, l->Load<typename std::decay<Args>::type>(L, Idxs + 2)...);
        return 0;
    }

    template<typename Rty, typename Cty, typename... Args, size_t... Idxs>
    inline int _MetaCallEx(xLuaState* l, Cty* obj, Rty(*func)(Args...) const, index_sequence<Idxs...>)
    {
        //TODO:
        using arg_tuple = std::tuple<Args...>;
        l->Push(func(obj, l->Load<typename std::decay<Args>::type>(L, Idxs + 2)...));
        return 1;
    }

    template <typename Rty, typename Cty, typename... Args>
    inline int MetaCall(xLuaState* l, Cty* obj, Rty(Cty::*func)(Args...))
    {
#ifdef _DEBUG
        //MetaParamCheck<Args...>(L, 1);
#endif // _DEBUG
        return _MetaCall(l, obj, func, make_index_sequence_t<sizeof...(Args)>());
    }

    template <typename Rty, typename Cty, typename... Args>
    inline int MetaCall(xLuaState* l, Cty* obj, Rty(Cty::*func)(Args...)const)
    {
#ifdef _DEBUG
        //MetaParamCheck<Args...>(L, 1);
#endif // _DEBUG
        return _MetaCall(L, obj, func, make_index_sequence_t<sizeof...(Args)>());
    }

    template <typename Cty>
    inline int MetaCall(xLuaState* l, Cty* obj, int(Cty::*func)(xLuaState*))
    {
        return (pObj->*func)(l);
    }

    template <typename Cty>
    inline int MetaCall(xLuaState* l, Cty* obj, int(Cty::*func)(xLuaState*) const)
    {
        return (obj->*func)(l);
    }

    template <typename Cty>
    inline int MetaCall(xLuaState* l, Cty* obj, void(Cty::*func)(xLuaState*))
    {
        (obj->*func)(l);
        return 0;
    }

    template <typename Cty>
    inline int MetaCall(xLuaState* l, void(Cty::*func)(xLuaState*) const) {
        (obj->*func)(l);
        return 0;
    }

    template <typename Cty>
    inline int MetaCall(xLuaState* l, Cty* obj, int(Cty::*func)(lua_State*)) {
        return (pObj->*func)(l->GetState());
    }

    template <typename Cty>
    inline int MetaCall(xLuaState* l, Cty* obj, int(Cty::*func)(lua_State*) const) {
        return (obj->*func)(l->GetState());
    }

    template <typename Cty>
    inline int MetaCall(xLuaState* l, Cty* obj, void(Cty::*func)(lua_State*)) {
        (obj->*func)(l->GetState());
        return 0;
    }

    template <typename Cty>
    inline int MetaCall(xLuaState* l, void(Cty::*func)(lua_State*) const) {
        (obj->*func)(l->GetState());
        return 0;
    }

    template <typename Rty, typename Cty, typename... Args>
    inline int MetaCall(xLuaState* l, Cty* obj, Rty(*func)(Args...))
    {
#ifdef _DEBUG
        //MetaParamCheck<Args...>(L, 1);
#endif // _DEBUG
        return _MetaCallEx(l, obj, func, make_index_sequence_t<sizeof...(Args) - 1>());
    }
} // namespace detail

XLUA_NAMESPACE_END
