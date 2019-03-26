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

    template <typename Ty>
    struct IsMember {
        static constexpr bool value = std::is_member_pointer<Ty>::value || std::is_member_function_pointer<Ty>::value;
    };

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

    template <typename Ty>
    struct WeakObjPtrCast
    {
        static void* ToWeakObjPtr(void* obj) {
            return static_cast<Ty*>(static_cast<XLUA_WEAK_OBJ_BASE_TYPE*>(obj));
        }
    };


    /* 多继承指针偏移, 这里一步步的执行转换 */
    template <typename Ty, typename By>
    struct RawPtrCast {
        static_assert(sizeof(std::shared_ptr<Ty>) == sizeof(std::shared_ptr<By>), "must be same size");
        static_assert(sizeof(std::unique_ptr<Ty>) == sizeof(std::unique_ptr<By>), "must be same size");

        static void* ToBase(void* obj, const TypeInfo* src, const TypeInfo* dst) {
            if (src == dst)
                return obj;
            return src->super->caster.to_super(static_cast<By*>((Ty*)obj), src->super, dst);
        }

        static void* ToDerived(void* obj, const TypeInfo* src, const TypeInfo* dst) {
            if (src == dst)
                return obj;
            obj = dst->super->caster.to_derived(obj, src, dst->super);
            return obj ? dynamic_cast<Ty*>(static_cast<By*>(obj)) : nullptr;
        }
    };

    template <typename Ty>
    struct RawPtrCast<Ty, void> {
        static void* ToBase(void* obj, const TypeInfo* src, const TypeInfo* dst) {
            return obj;
        }

        static void* ToDerived(void* obj, const TypeInfo* src, const TypeInfo* dst) {
            return obj;
        }
    };

    template <typename Ty, typename By>
    inline TypeCaster MakePtrCaster() {
        return TypeCaster{
            &RawPtrCast<Ty, By>::ToBase,
            &RawPtrCast<Ty, By>::ToDerived,
            &WeakObjPtrCast<Ty>::ToWeakObjPtr,
        };
    }

    template <typename Ty>
    inline Ty* GetMetaCallFullUserDataPtr(void* user_data, const TypeInfo* info) {
        LuaUserData* ud = static_cast<LuaUserData*>(user_data);
        if (ud->obj_ == nullptr) {
            return nullptr;
        } else if (!IsBaseOf(info, ud->info_)) {
            return nullptr;
        }
        return static_cast<Ty*>(ud->info_->caster.to_super(ud->obj_, ud->info_, info));
    }

    template <typename Ty>
    inline Ty* GetMetaCallObj(xLuaState* l, const TypeInfo* info) {
        void* p = lua_touserdata(l->GetState(), 1);
        if (p == nullptr)
            return nullptr;

        switch (l->GetType(1))
        {
        case LUA_TLIGHTUSERDATA:
            return GetLightUserDataPtr<Ty>(p, info);
        case LUA_TUSERDATA:
            return GetMetaCallFullUserDataPtr<Ty>(p, info);
        }
        //TODO: log
        return nullptr;
    }

    template <typename Ty>
    bool CheckMetaParam(xLuaState* l, int index, tag_unknown) {
    }

    template <typename Ty>
    bool CheckMetaParam(xLuaState* l, int index, tag_internal) {
    }

    template <typename Ty>
    bool CheckMetaParam(xLuaState* l, int index, tag_external) {
    }

    template <typename Ty>
    bool CheckMetaParam(xLuaState* l, int index, tag_extend) {
    }

    template <bool value, typename Ty = int>
    using EnableIfT = typename std::enable_if<value, int>::type;

    /* global function */
    template <typename Ry, typename... Args>
    inline int MetaCall(xLuaState* l, Ry(*func)(Args...)) {
        return 0;
    }

    /* extend member function */
    template <typename Ty, typename Ry, typename... Args>
    inline int MetaCall(xLuaState* l, Ty* obj, Ry(*func)(Ty*, Args...)) {
        return 0;
    }

    template <typename Ty, typename Ry, typename... Args>
    inline int MetaCall(xLuaState* l, Ty* obj, Ry(Ty::*func)(Args...)) {
        return 0;
    }

    template <typename Ry>
    inline void MetaGet(xLuaState* l, Ry* data) {
        l->Push(*data);
    }

    template <typename Ry>
    inline void MetaSet(xLuaState* l, Ry* data) {
        *data = l->Load<typename std::decay<Ry>::type>(1);
    }

    template <size_t N>
    inline void MetaSet(xLuaState* l, char data[N]) {
        //*data = l->Load<typename std::decay<Ry>::type>(1);
        //TODO:
    }

    template <typename Ry>
    inline void MetaGet(xLuaState* l, Ry (*data)()) {
        l->Push(data());
    }

    template <typename Ry>
    inline void MetaSet(xLuaState* l, void (*data)(Ry)) {
        data(l->Load<typename std::decay<Ry>::type>(1));
    }

    template <typename Ty, typename Ry>
    inline void MetaGet(xLuaState* l, Ty* obj, Ry Ty::* data) {
        l->Push(static_cast<Ry>(obj->*data));
    }

    template <typename Ty, typename Ry>
    inline void MetaSet(xLuaState* l, Ty* obj, Ry Ty::*data) {
        obj->*data = l->Load<typename std::decay<Ry>::type>(1);
    }

    template <typename Ty, typename Ry>
    inline void MetaGet(xLuaState* l, Ty* obj, Ry(Ty::*func)()) {
        l->Push((obj->*func)());
    }

    template <typename Ty, typename Ry>
    inline void MetaSet(xLuaState* l, Ty* obj, void(Ty::*func)(Ry)) {
        (obj->*func)(l->Load<typename std::decay<Ry>::type>(1));
    }

    template <typename Ty, typename Ry>
    inline void MetaGet(xLuaState* l, Ty* obj, Ry (*func)(Ty*)) {
        l->Push(func(obj));
    }

    template <typename Ty, typename Ry>
    inline void MetaSet(xLuaState* l, Ty* obj, void (*func)(Ty*, Ry)) {
        func(obj, l->Load<typename std::decay<Ry>::type>(1));
    }

    /* 支持类的成员函数, 静态函数 */
    template <typename Ty>
    struct MetaFunc {
        template <typename Fy, typename EnableIfT<std::is_member_function_pointer<Fy>::value> = 0>
        static inline int Call(xLuaState* l, const TypeInfo* info, Fy f) {
            Ty* obj = GetMetaCallObj<Ty>(l, info);
            if (obj)
                return MetaCall(l, obj, f);
            //TODO: log err
            return 0;
        }

        template <typename Fy, typename EnableIfT<!std::is_member_function_pointer<Fy>::value> = 0>
        static inline int Call(xLuaState* l, const TypeInfo* info, Fy f) {
            return MetaCall(l, f);
        }
    };

    /* 全局静态函数 */
    //template <>
    //struct MetaFunc<void> {
    //    template <typename Fy>
    //    static inline int Call(xLuaState* l, const TypeInfo* info, Fy f) {
    //        return MetaCall(l, f);
    //    }
    //};

    template <typename Ty>
    struct MetaFuncEx {
        template <typename Fy>
        static inline int Call(xLuaState* l, const TypeInfo* info, Fy f) {
            Ty* obj = GetMetaCallObj<Ty>(l, info);
            if (obj)
                return MetaCall(l, obj, f);
            //TODO: log err
            return 0;
        }
    };

    template <typename Ty>
    struct MetaVar {
        template <typename Fy, typename EnableIfT<IsMember<Fy>::value, int> = 0>
        static inline void Get(xLuaState* l, Ty* obj, Fy f) {
            MetaGet(l, obj, f);
        }

        template <typename Fy, typename EnableIfT<IsMember<Fy>::value, int> = 0>
        static inline void Set(xLuaState* l, Ty* obj, Fy f) {
            MetaSet(l, obj, f);
        }

        template <typename Fy, typename EnableIfT<!IsMember<Fy>::value, int> = 0>
        static inline void Get(xLuaState* l, Ty* obj, Fy f) {
            MetaGet(l, f);
        }

        template <typename Fy, typename EnableIfT<!IsMember<Fy>::value, int> = 0>
        static inline void Set(xLuaState* l, Ty* obj, Fy f) {
            MetaSet(l, f);
        }
    };

    template <typename Ty>
    struct MetaVarEx {
        template <typename Fy>
        static inline  void Get(xLuaState* l, Ty* obj, Fy f) {
            MetaGet(l, obj, f);
        }

        template <typename Fy>
        static inline void Set(xLuaState* l, Ty* obj, Fy f) {
            MetaSet(l, obj, f);
        }
    };

    //template <typename Ty>
    //inline auto MetaCall(xLuaState* L, Ty f) -> typename std::enable_if<std::is_member_function_pointer<Ty>::value, int> {
    //    return 0;
    //}

    //template <typename Ty>
    //inline int MetaGet(xLuaState* L, void* obj, Ty f) {
    //    return 0;
    //}

    //template <typename Ty>
    //inline int MetaSet(xLuaState* L, void* obj, Ty f) {
    //    return 0;
    //}
//
//    template<typename Rty, typename Cty, typename... Args, size_t... Idxs>
//    inline int _MetaCall(xLuaState* l, Cty* obj, Rty(Cty::*func)(Args...), index_sequence<Idxs...>)
//    {
//        l->Push(
//            (obj->*func)(l->Load<typename std::decay<Args>::type>(L, Idxs + 1)...)
//        );
//        return 1;
//    }
//
//    template<typename Rty, typename Cty, typename... Args, size_t... Idxs>
//    inline int _MetaCall(xLuaState* l, Cty* obj, Rty(Cty::*func)(Args...) const, index_sequence<Idxs...>)
//    {
//        l->Push(L,
//            (obj->*func)(l->Load<typename std::decay<Args>::type>(L, Idxs + 1)...)
//        );
//        return 1;
//    }
//
//    template <typename Cty, typename... Args, size_t... Idxs>
//    inline int _MetaCall(xLuaState* l, Cty* obj, void(Cty::*func)(Args...), index_sequence<Idxs...>)
//    {
//        (obj->*func)(l->Load<typename std::decay<Args>::type>(L, Idxs + 1)...);
//        return 0;
//    }
//
//    template <typename Cty, typename... Args, size_t... Idxs>
//    inline int _MetaCall(xLuaState* l, void* obj, void(Cty::*func)(Args...) const, index_sequence<Idxs...>)
//    {
//        (obj->*func)(l->Load<typename std::decay<Args>::type>(L, Idxs + 1)...);
//        return 0;
//    }
//
//    template <typename Cty, typename... Args, size_t... Idxs>
//    inline int _MetaCallEx(xLuaState* l, Cty* obj, void(*func)(Args...), index_sequence<Idxs...>)
//    {
//        //TODO:
//        using arg_tuple = std::tuple<Args...>;
//        func(obj, l->Load<typename std::decay<Args>::type>(L, Idxs + 2)...);
//        return 0;
//    }
//
//    template<typename Rty, typename Cty, typename... Args, size_t... Idxs>
//    inline int _MetaCallEx(xLuaState* l, Cty* obj, Rty(*func)(Args...) const, index_sequence<Idxs...>)
//    {
//        //TODO:
//        using arg_tuple = std::tuple<Args...>;
//        l->Push(func(obj, l->Load<typename std::decay<Args>::type>(L, Idxs + 2)...));
//        return 1;
//    }
//
//    template <typename Rty, typename Cty, typename... Args>
//    inline int MetaCall(xLuaState* l, Cty* obj, Rty(Cty::*func)(Args...))
//    {
//#ifdef _DEBUG
//        //MetaParamCheck<Args...>(L, 1);
//#endif // _DEBUG
//        return _MetaCall(l, obj, func, make_index_sequence_t<sizeof...(Args)>());
//    }
//
//    template <typename Rty, typename Cty, typename... Args>
//    inline int MetaCall(xLuaState* l, Cty* obj, Rty(Cty::*func)(Args...)const)
//    {
//#ifdef _DEBUG
//        //MetaParamCheck<Args...>(L, 1);
//#endif // _DEBUG
//        return _MetaCall(L, obj, func, make_index_sequence_t<sizeof...(Args)>());
//    }
//
//    template <typename Cty>
//    inline int MetaCall(xLuaState* l, Cty* obj, int(Cty::*func)(xLuaState*))
//    {
//        return (pObj->*func)(l);
//    }
//
//    template <typename Cty>
//    inline int MetaCall(xLuaState* l, Cty* obj, int(Cty::*func)(xLuaState*) const)
//    {
//        return (obj->*func)(l);
//    }
//
//    template <typename Cty>
//    inline int MetaCall(xLuaState* l, Cty* obj, void(Cty::*func)(xLuaState*))
//    {
//        (obj->*func)(l);
//        return 0;
//    }
//
//    template <typename Cty>
//    inline int MetaCall(xLuaState* l, void(Cty::*func)(xLuaState*) const) {
//        (obj->*func)(l);
//        return 0;
//    }
//
//    template <typename Cty>
//    inline int MetaCall(xLuaState* l, Cty* obj, int(Cty::*func)(lua_State*)) {
//        return (pObj->*func)(l->GetState());
//    }
//
//    template <typename Cty>
//    inline int MetaCall(xLuaState* l, Cty* obj, int(Cty::*func)(lua_State*) const) {
//        return (obj->*func)(l->GetState());
//    }
//
//    template <typename Cty>
//    inline int MetaCall(xLuaState* l, Cty* obj, void(Cty::*func)(lua_State*)) {
//        (obj->*func)(l->GetState());
//        return 0;
//    }
//
//    template <typename Cty>
//    inline int MetaCall(xLuaState* l, void(Cty::*func)(lua_State*) const) {
//        (obj->*func)(l->GetState());
//        return 0;
//    }
//
//    template <typename Rty, typename Cty, typename... Args>
//    inline int MetaCall(xLuaState* l, Cty* obj, Rty(*func)(Args...))
//    {
//#ifdef _DEBUG
//        //MetaParamCheck<Args...>(L, 1);
//#endif // _DEBUG
//        return _MetaCallEx(l, obj, func, make_index_sequence_t<sizeof...(Args) - 1>());
//    }
} // namespace detail

XLUA_NAMESPACE_END
