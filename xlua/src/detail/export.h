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

    enum class NodeCategory {
        kType,
        kConst,
        kScript,
    };

    struct NodeBase {
        NodeBase(NodeCategory type);
        ~NodeBase();

        NodeCategory category;
        NodeBase* next;
    };

    struct TypeNode : NodeBase {
        TypeNode(fnTypeInfo fn)
            : NodeBase(NodeCategory::kType)
            , func(fn) {
        }
        const fnTypeInfo func;
    };

    struct ConstNode : NodeBase {
        ConstNode(fnConstInfo fn)
            : NodeBase(NodeCategory::kConst)
            , func_(fn) {
        }
        const fnConstInfo func_;
    };

    struct ScriptNode : NodeBase {
        ScriptNode(const char* s)
            : NodeBase(NodeCategory::kScript)
            , script(s) {
        }
        const char* const script;
    };

    inline ConstValue MakeConstValue() {
        ConstValue cv;
        cv.category = ConstCategory::kNone;
        cv.name = nullptr;
        cv.int_val = 0;
        return cv;
    }

    inline ConstValue MakeConstValue(const char* name, int val) {
        ConstValue cv;
        cv.category = ConstCategory::kInteger;
        cv.name = name;
        cv.int_val = val;
        return cv;
    }

    inline ConstValue MakeConstValue(const char* name, float val) {
        ConstValue cv;
        cv.category = ConstCategory::kFloat;
        cv.name = name;
        cv.float_val = val;
        return cv;
    }

    inline ConstValue MakeConstValue(const char* name, const char* val) {
        ConstValue cv;
        cv.category = ConstCategory::kString;
        cv.name = name;
        cv.string_val = val;
        return cv;
    }

    inline TypeMember MakeMember() {
        TypeMember mem;
        mem.category = MemberCategory::kInvalid;
        mem.name = nullptr;
        return mem;
    }

    inline TypeMember MakeMember(const char* name, LuaFunction func) {
        TypeMember mem;
        mem.category = MemberCategory::kFunction;
        mem.name = name;
        mem.func = func;
        return mem;
    }

    inline TypeMember MakeMember(const char* name, LuaIndexer getter, LuaIndexer setter) {
        TypeMember mem;
        mem.category = MemberCategory::kVariate;
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
        FullUserData* ud = static_cast<FullUserData*>(user_data);
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

    namespace param {
        struct UdInfo {
            bool is_ud;
            bool is_nil;
            bool is_ptr;
            const TypeInfo* info;
        };

        inline UdInfo GetUdInfo(xLuaState* l, int index, bool check_nil) {
            UdInfo  ud_info{true, false, true, nullptr};
            int l_ty = l->GetType(index);

            if (l_ty == LUA_TNIL) {
                ud_info.is_nil = true;
            } else if (l_ty == LUA_TUSERDATA) {
                FullUserData* ud = static_cast<FullUserData*>(lua_touserdata(l->GetState(), index));
                ud_info.info = ud->info_;
                if (!check_nil) {
                    if (ud->type_ == UserDataCategory::kObjPtr) {
                        if (ud->info_->is_weak_obj) {
                            ud_info.is_nil = (ud->serial_ == xLuaGetWeakObjSerialNum(ud->index_));
                        } else {
                            ArrayObj* obj = GlobalVar::GetInstance()->GetArrayObj(ud->index_);
                            ud_info.is_nil = (obj == nullptr || obj->serial_num_ != obj->serial_num_);
                        }
                    } else if (ud->type_ == UserDataCategory::kValue) {
                        ud_info.is_ptr = false;
                    }
                }
            } else if (l_ty == LUA_TLIGHTUSERDATA) {
#if XLUA_USE_LIGHT_USER_DATA
                LightUserData ud = MakeLightPtr(lua_touserdata(l->GetState(), index));
                if (ud.type_ == 0) {
                    ArrayObj* obj = GlobalVar::GetInstance()->GetArrayObj(ud.index_);
                    if (obj)
                        ud_info.info = obj->info_;
                    if (check_nil)
                        ud_info.is_nil = (obj == nullptr || obj->serial_num_ != ud.serial_);
                } else {
                    ud_info.info = GlobalVar::GetInstance()->GetExternalTypeInfo(ud.type_);
                    if (check_nil)
                        ud_info.is_nil = (ud.serial_ == xLuaGetWeakObjSerialNum(ud.index_));
                }
#endif // XLUA_USE_LIGHT_USER_DATA
            } else {
                ud_info.is_ud = false;
                ud_info.is_ptr = false;
            }

            return ud_info;
        }

        template <typename Ty>
        struct Checker {
            static bool Do(xLuaState* l, int index, int param) {
                using tag = typename std::conditional<IsInternal<Ty>::value || IsExternal<Ty>::value, tag_declared,
                    typename std::conditional<IsExtendLoad<Ty>::value, tag_extend,
                    typename std::conditional<std::is_enum<Ty>::value, tag_enum, tag_unknown>::type>::type>::type;
                return Do(l, index, param, tag());
            }

            static bool Do(xLuaState* l, int index, int param, tag_unknown) {
                return false;
            }

            static bool Do(xLuaState* l, int index, int param, tag_enum) {
                if (l->GetType(index) != LUA_TNUMBER) {
                    LogError("param(%d) error, need:enum(number) got:%s\n", param, l->GetTypeName(index));
                    return false;
                }
                return true;
            }

            static bool Do(xLuaState* l, int index, int param, tag_extend) {
                return true;    // extend value type
            }

            static bool Do(xLuaState* l, int index, int param, tag_declared) {
                const TypeInfo* info = GetTypeInfoImpl<Ty>();
                UdInfo ud = GetUdInfo(l, index, true);
                if (ud.is_nil) {
                    LogError("param(%d) error, need:%s got:nil\n", param, info->type_name);
                    return false;
                }
                if (!IsBaseOf(info, ud.info)) {
                    LogError("param(%d) error, need:%s got:%s\n", param, info->type_name, l->GetTypeName(index));
                    return false;
                }
                return true;
            }
        };

        template <typename Ty>
        struct Checker<Ty*> {
            static_assert(IsInternal<Ty>::value || IsExternal<Ty>::value, "only declare export to lua types");
            static bool Do(xLuaState* l, int index, int param) {
                const TypeInfo* info = GetTypeInfoImpl<Ty>();
                UdInfo ud = GetUdInfo(l, index, false);
                if (ud.is_nil || IsBaseOf(ud.info, info))
                    return true;
                LogError("param(%d) error, need:%s* got:%s\n", param, info->type_name, l->GetTypeName(index));
                return false;
            }
        };

        template <typename Ty>
        struct Checker<std::shared_ptr<Ty>> {
            static_assert(IsInternal<Ty>::value || IsExternal<Ty>::value, "only declare export to lua types");
            static inline bool Do(xLuaState* l, int index, int param) {
                int l_ty = l->GetType(index);
                const TypeInfo* info = GetTypeInfoImpl<Ty>();
                if (l_ty == LUA_TUSERDATA) {
                    FullUserData* ud = static_cast<FullUserData*>(lua_touserdata(l->GetState(), index));
                    if (ud->type_ == UserDataCategory::kSharedPtr && IsBaseOf(info, ud->info_)) {
                        return true;
                    }
                } else if (l_ty == LUA_TNIL) {
                    return true;
                }

                LogError("param(%d) error, need:std::shared_ptr<%s> got:%s\n", param, info->type_name, l->GetTypeName(index));
                return false;
            }
        };

        template <typename Ty>
        struct Checker<xLuaWeakObjPtr<Ty>> {
            static bool Do(xLuaState* l, int index, int param) {
                return Checker<Ty*>::Do(l, index, param);
            }
        };

        inline bool IsType_1(xLuaState* l, int index, int param, int ty, const char* need) {
            if (l->GetType(index) == ty)
                return true;
            LogError("param(%d) error, need:%s got:%s\n", param, need, l->GetTypeName(index));
            return false;
        }

        inline bool IsType_2(xLuaState* l, int index, int param, int ty, const char* need) {
            int l_ty = l->GetType(index);
            if (l_ty == ty || l_ty == LUA_TNIL)
                return true;
            LogError("param(%d) error, need:%s got:%s\n", param, need, l->GetTypeName(index));
            return false;
        }

        template <typename Ty>
        inline bool IsType(xLuaState* l, int index, int param) {
            return Checker<Ty>::Do(l, index, param);
        }

        template <> inline bool IsType<bool>(xLuaState* l, int index, int param) {
            return IsType_2(l, index, param, LUA_TBOOLEAN, "boolean");
        }

        template <> inline bool IsType<char>(xLuaState* l, int index, int param) {
            return IsType_1(l, index, param, LUA_TNUMBER, "char(number)");
        }

        template <> inline bool IsType<unsigned char>(xLuaState* l, int index, int param) {
            return IsType_1(l, index, param, LUA_TNUMBER, "unsigned char(number)");
        }

        template <> inline bool IsType<short>(xLuaState* l, int index, int param) {
            return IsType_1(l, index, param, LUA_TNUMBER, "short(number)");
        }

        template <> inline bool IsType<unsigned short>(xLuaState* l, int index, int param) {
            return IsType_1(l, index, param, LUA_TNUMBER, "unsigned short(number)");
        }

        template <> inline bool IsType<int>(xLuaState* l, int index, int param) {
            return IsType_1(l, index, param, LUA_TNUMBER, "int(number)");
        }

        template <> inline bool IsType<unsigned int>(xLuaState* l, int index, int param) {
            return IsType_1(l, index, param, LUA_TNUMBER, "unsigned int(number)");
        }

        template <> inline bool IsType<long>(xLuaState* l, int index, int param) {
            return IsType_1(l, index, param, LUA_TNUMBER, "long(number)");
        }

        template <> inline bool IsType<unsigned long>(xLuaState* l, int index, int param) {
            return IsType_1(l, index, param, LUA_TNUMBER, "unsigned long(number)");
        }

        template <> inline bool IsType<long long>(xLuaState* l, int index, int param) {
            return IsType_1(l, index, param, LUA_TNUMBER, "short(number)");
        }

        template <> inline bool IsType<unsigned long long>(xLuaState* l, int index, int param) {
            return IsType_1(l, index, param, LUA_TNUMBER, "unsigned long long(number)");
        }

        template <> inline bool IsType<float>(xLuaState* l, int index, int param) {
            return IsType_1(l, index, param, LUA_TNUMBER, "float(number)");
        }

        template <> inline bool IsType<double>(xLuaState* l, int index, int param) {
            return IsType_1(l, index, param, LUA_TNUMBER, "double(number)");
        }

        template <> inline bool IsType<char*>(xLuaState* l, int index, int param) {
            return IsType_2(l, index, param, LUA_TSTRING, "char*(number)");
        }

        template <> inline bool IsType<std::string>(xLuaState* l, int index, int param) {
            return IsType_2(l, index, param, LUA_TSTRING, "std::string");
        }

        template <> inline bool IsType<xLuaTable>(xLuaState* l, int index, int param) {
            return IsType_2(l, index, param, LUA_TTABLE, "xLuaTable");
        }

        template <> inline bool IsType<xLuaFunction>(xLuaState* l, int index, int param) {
            return IsType_2(l, index, param, LUA_TFUNCTION, "xLuaFunction");
        }

        inline void* GetUdPtr(xLuaState* l, int index, const TypeInfo* info) {
            int l_ty = l->GetType(index);
            if (l_ty == LUA_TUSERDATA) {
                FullUserData* ud = static_cast<FullUserData*>(lua_touserdata(l->GetState(), index));
                if (ud->type_ == UserDataCategory::kObjPtr) {
                    if (ud->info_->is_weak_obj) {
                        if (ud->serial_ == xLuaGetWeakObjSerialNum(ud->index_))
                            return info->caster.to_weak_ptr(xLuaGetWeakObjPtr(ud->index_));
                    } else {
                        ArrayObj* obj = GlobalVar::GetInstance()->GetArrayObj(ud->index_);
                        if (obj && obj->serial_num_ == ud->serial_)
                            return obj->info_->caster.to_super(obj->obj_, obj->info_, info);
                    }
                } else if (ud->type_ == UserDataCategory::kValue) {
                    return ud->info_->caster.to_super(ud->obj_, ud->info_, info);
                }
            } else if (l_ty == LUA_TLIGHTUSERDATA) {
#if XLUA_USE_LIGHT_USER_DATA
                LightUserData ud = MakeLightPtr(lua_touserdata(l->GetState(), index));
                if (ud.type_ == 0) {
                    ArrayObj* obj = GlobalVar::GetInstance()->GetArrayObj(ud.index_);
                    if (obj && obj->serial_num_ == ud.serial_)
                        return obj->info_->caster.to_super(obj->obj_, obj->info_, info);
                } else {
                    const TypeInfo* src = GlobalVar::GetInstance()->GetExternalTypeInfo(ud.type_);
                    if (src->is_weak_obj) {
                        if (ud.serial_ == xLuaGetWeakObjSerialNum(ud.index_))
                            return info->caster.to_weak_ptr(xLuaGetWeakObjPtr(ud.index_));
                    } else {
                        return src->caster.to_super(ud.ToRawPtr(), src, info);
                    }
                }
#endif // XLUA_USE_LIGHT_USER_DATA
            }
            return nullptr;
        }

        template <typename Ty, typename Ry>
        struct Laoder {
            static Ry Do(xLuaState* l, int index) {
                Ty* ptr = GetUdPtr(l, index, GetTypeInfoImpl<Ty>());
                assert(ptr);
                return *ptr;
            }
        };

        template <typename Ty, typename Ry>
        struct Laoder<Ty*, Ry> {
            static Ry Do(xLuaState* l, int index) {
                return GetUdPtr(l, index, GetTypeInfoImpl<Ty>());
            }
        };

        template <typename Ty, typename Ry>
        struct Laoder<std::shared_ptr<Ty>, Ry> {
            static Ry Do(xLuaState* l, int index) {
                return l->Load<std::shared_ptr<Ty>>(index);
            }
        };

        template <typename Ty, typename Ry>
        struct Laoder<xLuaWeakObjPtr<Ty>, Ry> {
            static Ry Do(xLuaState* l, int index) {
                Ty* ptr = GetUdPtr(l, index, GetTypeInfoImpl<Ty>());
                return xLuaWeakObjPtr<Ty>(ptr);
            }
        };
    } // namespace param

    template <typename Ty>
    inline bool CheckParam(xLuaState* l, int index, int param) {
         using decay_ty = typename std::decay<Ty>::type;
        static_assert(!std::is_same<decay_ty, char*>::value || std::is_const<Ty>::value,
            "only accept const char* paramenter");
        return param::IsType<decay_ty>(l, index, param);
    }

    template <typename... Ty>
    inline bool CheckParams(xLuaState* l, int index) {
        size_t count = 0;
        int param = 0;
        using els = int[];
        (void)els { 0, (count += CheckParam<Ty>(l, index++, param++) ? 1 : 0, 0)... };
        if (count == sizeof...(Ty))
            return true;
        // log error
        return false;
    }

    template <typename Ty>
    inline Ty LoadParam_Un(xLuaState* l, int index) {
        return l->Load<typename std::decay<Ty>::type>(index);
    }

    template <>
    inline const char* LoadParam_Un<const char*>(xLuaState* l, int index) {
        return l->Load<const char*>(index); // const char* is expecial
    }

    template <typename Ty>
    inline Ty LoadParam(xLuaState* l, int index, tag_unknown) {
        return LoadParam_Un<typename std::remove_reference<Ty>::type>(l, index);
    }

    template <typename Ty>
    inline Ty LoadParam(xLuaState* l, int index, tag_declared) {
        return param::Loader<typename std::decay<Ty>::type, Ty>::Do(l, index);
    }

    template <typename Ty>
    inline Ty LoadParam(xLuaState* l, int index, tag_extend) {
        return ::xLuaLoad(l, index, Identity<typename std::decay<Ty>::type>());
    }

    template <typename Ty>
    inline Ty LoadParam(xLuaState* l, int index) {
        using decay_ty = typename std::decay<Ty>::type;
        using tag = typename std::conditional<IsInternal<decay_ty>::value || IsExternal<decay_ty>::value, tag_declared,
            typename std::conditional<IsExtendLoad<decay_ty>::value, tag_extend, tag_unknown>::type>::type;
        return LoadParam<Ty>(l, index, tag());
    }

    template <bool value, typename Ty = int>
    using EnableIfT = typename std::enable_if<value, int>::type;

    /* global function */
    template <typename Ry, typename... Args>
    inline int MetaCall(xLuaState* l, Ry(*func)(Args...)) {
        if (!CheckParams<Args...>(l, 1))
            return 0;
        int index = 0;
        l->Push(func(LoadParam<Args>(l, ++index)...));
        return 1;
    }

    template <typename... Args>
    inline int MetaCall(xLuaState* l, void (*func)(Args...)) {
        if (!CheckParams<Args...>(l, 1))
            return 0;
        int index = 0;
        func(LoadParam<Args>(l, ++index)...);
        return 0;
    }

    /* extend member function */
    template <typename Ty, typename Ry, typename... Args>
    inline int MetaCall(xLuaState* l, Ty* obj, Ry(*func)(Ty*, Args...)) {
        if (!CheckParams<Args...>(l, 2))
            return 0;
        int index = 1;
        l->Push(func(obj, LoadParam<Args>(l, ++index)...));
        return 1;
    }

    template <typename Ty, typename... Args>
    inline int MetaCall(xLuaState* l, Ty* obj, void (*func)(Ty*, Args...)) {
        if (!CheckParams<Args...>(l, 2))
            return 0;
        int index = 1;
        func(obj, LoadParam<Args>(l, ++index)...);
        return 0;
    }

    /* normal member function */
    template <typename Ty, typename Ry, typename... Args>
    inline int MetaCall(xLuaState* l, Ty* obj, Ry(Ty::*func)(Args...)) {
        if (!CheckParams<Args...>(l, 2))
            return 0;
        int index = 1;
        l->Push((obj->*func)(LoadParam<Args>(l, ++index)...));
        return 1;
    }

    template <typename Ty, typename... Args>
    inline int MetaCall(xLuaState* l, Ty* obj, void (Ty::*func)(Args...)) {
        if (!CheckParams<Args...>(l, 2))
            return 0;
        int index = 1;
        (obj->*func)(LoadParam<Args>(l, ++index)...);
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
        static_assert(N > 0);
        const char* s = l->Load<const char*>(3);    // 1:obj, 2:name, 3:value
        if (s)
            snprintf(data, N, s);
        else
            data[0] = 0;
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

    template <typename Ty, size_t N>
    inline void MetaGet(xLuaState* l, Ty* obj, char Ty::* data [N]) {
        //l->Push(static_cast<Ry>(obj->*data));
        //TODO:
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
