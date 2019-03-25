#pragma once
#include "xlua_def.h"
#include "detail/traits.h"
#include "detail/state.h"
#include <lua.hpp>
#include <string>
#include <unordered_map>

/* 扩展类型
 * void xLuaPush(xlua::xLuaState* l, const Type& val)
 * Type xLuaLoad(xlua::xLuaState* L, int index, xlua::Identity<Type>);
 * const char* xLuaTypeName(xlua::Identity<Type>);
*/

XLUA_NAMESPACE_BEGIN

namespace detail {
    template <typename Ty> struct Pusher;
    template <typename Ty> struct Loader;
    struct MetaFuncs;
}

bool Startup();
void Shutdown();
xLuaState* Create();
xLuaState* Attach(lua_State* l);

class xLuaTable { };
class xLuaFunction { };

struct StackGuard {
    StackGuard(xLuaState* l) {}
    StackGuard(xLuaState* l, int off) {}
};

enum class LuaValueType : int {
    kNill = 0,
    kBoolean = 1 << 0,
    kNumber = 1 << 1,
    kString = 1 << 2,

    kLuaFunction = 1 << 3,
    kCFunction = 1 << 4,
    kFunction = kLuaFunction | kCFunction,

    kLightUserData = 1 << 5,
    kFullUserData = 1 << 6,
    kUserData = kLightUserData | kFullUserData,
};

class xLuaState {
    template <typename Ty> friend struct detail::Loader;
    template <typename Ty> friend struct detail::Pusher;
    friend struct detail::MetaFuncs;
    friend class detail::GlobalVar;

    xLuaState(lua_State* l, bool attach);
    ~xLuaState();
public:
    void Release();

public:
    inline bool IsAttach() const { return attach_; }
    inline lua_State* GetState() const { return state_; }

public:
    int GetTopIndex() const;

    void LogCallStack() const;
    void GetCallStack(char* buf, size_t size) const;

    const char* GetTypeName(int index) const;
    LuaValueType GetValueType(int index) const { return LuaValueType::kFullUserData; }

    bool DoString(const char* stream, const char* chunk = nullptr) { return false; }

    //xLuaTable CreateTable() { return }

    LuaValueType LoadGlobal(const char* path);
    void SetGlobal(const char* path);

    LuaValueType LoadTableField(int index, int field);
    LuaValueType LoadTableField(int index, const char* field);
    void SetTableField(int index, int field);
    void SetTableField(int index, const char* field);

    template <typename Ty>
    inline Ty GetGlobal(const char* path) {
        StackGuard(this);
        LoadGlobal(path);
        return Load<Ty>(-1);
    }

    template <typename Ty>
    inline void SetGlobal(const char* path, const Ty& val) {
        StackGuard(this);
        Push(val);
        SetGlobal(path);
    }

    template <typename Ty>
    inline void SetTableField(int index, int field, const Ty& val) {
        Push(val);
        SetTableField(index, field);
    }

    template <typename Ty>
    inline void SetTableField(int index, const char* field, const Ty& val) {
        Push(val);
        SetTableField(index, field);
    }

    template <typename Ty>
    inline Ty GetTableField(xLuaTable table, int field) {
        StackGuard guard(this);
        Push(table);
        LoadTableField(-1, field);
        return Load<Ty>(-1);
    }

    template <typename Ty>
    inline Ty GetTableField(xLuaTable table, const char* field) {
        StackGuard guard(this);
        Push(table);
        LoadTableField(-1, field);
        return Load<Ty>(-1);
    }

    template <typename Ty>
    inline void SetTableField(xLuaTable table, int field, const Ty& val) {
        StackGuard guard(this);
        Push(table);
        Push(val);
        SetTableField(-2, field);
    }

    template <typename Ty>
    inline void SetTableField(xLuaTable table, const char* field, const Ty& val) {
        StackGuard guard(this);
        Push(table);
        Push(val);
        SetTableField(-2, field);
    }

    template <typename Ty>
    inline void Push(const Ty& val) {
        detail::Pusher<typename std::decay<Ty>::type>::Do(this, val);
    }

    template <typename Ty>
    inline void Push(std::unique_ptr<Ty>&& ptr) {
        static_assert(IsInternal<Ty>::value || IsExternal<Ty>::value, "");
        //_PushUniquePtr(MakeUserData(std::move(ptr), detail::GetTypeInfoImpl<Ty>()));
        // do not support unique_ptr
    }

    template <typename Ty>
    inline Ty Load(int index) {
        return detail::Loader<typename std::decay<Ty>::type>::Do(this, index);
    }

    template<typename... Ty>
    inline void PushMul(Ty&&... vals) {
        using ints = int[];
        (void)ints {0, (Push(std::forward<Ty>(vals)), 0)...};
    }

    template <typename... Ty>
    inline void LoadMul(std::tuple<Ty&...> tp, int index) {
        DoLoadMul(tp, index, detail::make_index_sequence_t<sizeof...(Ty)>());
    }

    inline void Push(bool val) { }
    inline void Push(char val) { }
    inline void Push(unsigned char val) { }
    inline void Push(short val) { }
    inline void Push(unsigned short val) { }
    inline void Push(int val) { }
    inline void Push(unsigned int val) { }
    inline void Push(long val) { }
    inline void Push(unsigned long val) { }
    inline void Push(long long val) { }
    inline void Push(unsigned long long val) { }
    inline void Push(float val) { }
    inline void Push(double val) { }
    inline void Push(char* val) { }
    inline void Push(const char* val) { }
    inline void Push(const std::string& val) { }
    inline void Push(std::nullptr_t) { }
    void Push(xLuaTable val) { }
    void Push(xLuaFunction val) { }

    template<> inline bool Load<bool>(int index) { return false; }
    template<> inline char Load<char>(int index) { return false; }
    template<> inline unsigned char Load<unsigned char>(int index) { return false; }
    template<> inline short Load<short>(int index) { return false; }
    template<> inline unsigned short Load<unsigned short>(int index) { return false; }
    template<> inline int Load<int>(int index) { return false; }
    template<> inline unsigned int Load<unsigned int>(int index) { return false; }
    template<> inline long Load<long>(int index) { return false; }
    template<> inline unsigned long Load<unsigned long>(int index) { return false; }
    template<> inline long long Load<long long>(int index) { return false; }
    template<> inline unsigned long long Load<unsigned long long>(int index) { return false; }
    template<> inline float Load<float>(int index) { return false; }
    template<> inline double Load<double>(int index) { return false; }
    template<> inline const char* Load<const char*>(int index) { return false; }
    template<> inline std::string Load<std::string>(int index) { return false; }
    //template<> inline const std::string Load<const std::string>(int index) { return false; }

    template<typename... Rys, typename... Args>
    inline bool Call(std::tuple<Rys&...> ret, Args&&... args) {
        StackGuard guard(this, -1);
        return DoCall(ret, std::forward<Args>(args)...);
    }

    template <typename... Rys, typename... Args>
    inline bool Call(const char* global, std::tuple<Rys&...> ret, Args&&... args) {
        StackGuard guard(this);
        if (((int)LoadGlobal(global) & (int)LuaValueType::kFunction) == 0)
            return false;
        return DoCall(ret, std::forward<Args>(args)...);
    }

    template <typename... Rys, typename... Args>
    inline bool Call(xLuaFunction func, std::tuple<Rys&...> ret, Args&&... args) {
        StackGuard guard(this);
        Push(func);
        return DoCall(ret, std::forward<Args>(args)...);
    }

    /* table call */
    template <typename... Rys, typename... Args>
    inline bool Call(xLuaTable table, const char* func , std::tuple<Rys&...> ret, Args&&... args) {
        StackGuard guard(this);
        //GetTableField(table, func);
        return DoCall(ret, table, std::forward<Args>(args)...);
    }

public:
    template <typename... Ty, size_t... Idxs>
    inline void DoLoadMul(std::tuple<Ty&...>& ret, int index, detail::index_sequence<Idxs...>)
    {
        using ints = int[];
        (void)ints { 0, (std::get<Idxs>(ret) = Load<Ty>(index++), 0)...};
    }

    template<typename... Rys, typename... Args>
    inline bool DoCall(std::tuple<Rys&...>& ret, Args&&... args) {
        int top = GetTopIndex();
        if (!((int)GetValueType(top) & (int)LuaValueType::kFunction))
            return false;

        PushMul(std::forward<Args>(args)...);
        //TODO: DoCall
        DoLoadMul(ret, top, detail::make_index_sequence_t<sizeof...(Rys)>());
        return true;
    }

    bool _TryPushSharedPtr(void* root, void* ptr, const TypeInfo* info);

    template <typename Ty>
    detail::LuaUserData* PushUserData(const Ty& val, const TypeInfo* info) {
        using DataType = detail::LuaUserDataImpl<Ty>;
        void* mem = lua_newuserdata(state_, sizeof(DataType));
        auto* data = new (mem)DataType(ptr, info);

        lua_rawgeti(state_, LUA_REGISTRYINDEX, meta_table_ref_);    // type metatable table
        lua_rawgeti(state_, -1, info->index);   // metatable
        lua_remove(state_, -2);     // remove type table
        lua_setmetatable(state_, 2);

        return data;
    }

    //template <typename Ty>
    //void PushUserData(void* root, const Ty& val, const TypeInfo* info) {
    //    auto it = raw_ptrs_.find(root);
    //    if (it != raw_ptrs_.cend()) {
    //        PushCacheUd(it->second, ptr, info);
    //    }
    //    else {
    //        UserDataCache ud;
    //        ud.user_data_ = PushUserData(ptr, info);

    //        lua_pushvalue(state_, -2);
    //        ud.lua_ref_ = luaL_ref(state_, -2);
    //        lua_pop(state_, 1);

    //        raw_ptrs_.insert(std::make_pair(root, ud));
    //    }
    //}

    template <typename Ty>
    void PushSharedPtr(void* root, const std::shared_ptr<Ty>& ptr, const TypeInfo* info) {
        void* root = GetRootPtr(ptr.get(), info);
        auto it = shared_ptrs_.find(root);
        if (it != shared_ptrs_.cend()) {
            PushCacheUd(it->second, ptr, info);
        } else {
            UserDataCache ud;
            ud.user_data_ = PushUserData(ptr, info);

            lua_pushvalue(state_, -2);
            ud.lua_ref_ = luaL_ref(state_, -2);
            lua_pop(state_, 1);

            shared_ptrs_.insert(std::make_pair(root, ud));
        }
    }

    template <typename Ty>
    void PushUserData(void* root, const Ty& ptr, const TypeInfo* info) {
        using DataType = detail::LuaUserDataImpl<Ty>;
        //if ()
    }


    void _PushSharedPtr(void* root, detail::LuaUserData* user_data);
    void _PushUniquePtr(detail::LuaUserData* user_data);
    void _PushValue(detail::LuaUserData* user_data);

    bool _TryPushRawPtr(void* root, void* ptr, const TypeInfo* info);
    void _PushRawPtr(void* root, detail::LuaUserData* user_data);

    bool _IsLightUserData(int index) const { return false; }
    bool _IsFullUserData(int index) const { return false; }

#if XLUA_USE_LIGHT_USER_DATA
#else
    void _PushWeakObjPtr(const detail::LuaUserData user_data);
#endif // XLUA_USE_LIGHT_USER_DATA

    //template <typename Ty>
    //inline detail::LuaUserData* MakeUserData(const Ty& val, const TypeInfo* info) {
    //    using DataType = detail::LuaUserDataImpl<Ty>;
    //    void* mem = AllocUserData(sizeof(DataType));
    //    return new (mem) DataType(val, info);
    //}

    //template <typename Ty>
    //inline detail::LuaUserData* MakeUserData(std::unique_ptr<Ty>&& val, const TypeInfo* info) {
    //    using DataType = detail::LuaUserDataImpl<std::unique_ptr<Ty>>;
    //    void* mem = AllocUserData(sizeof(DataType));
    //    return new (mem) DataType (std::move(val), info);
    //}

    void* GetUserDataPtr(int index) { return nullptr; }
private:
    struct UserDataCache {
        int lua_ref_;
        detail::LuaUserData* user_data_;
    };

    void* AllocUserData(size_t size);
    void PushCacheUd(UserDataCache& cache, void* ptr, const TypeInfo* info);
    void PushUd(UserDataCache& cache);
    TypeMember* GetMetaMember();
    void Gc(detail::LuaUserData* user_data);

    bool InitEnv(const std::vector<TypeInfo*>& types,
        const std::vector<const ConstInfo*>& consts,
        const std::vector<const char*>& scripts
    );
    void AddConsts(const std::vector<const ConstInfo*>& consts);
    void CreateMeta(const TypeInfo* info);
    void PushClosure(lua_CFunction func);

private:
    bool attach_;
    lua_State* state_;
    int meta_table_ref_;        // 导出元表索引
    int user_data_table_ref_;   // user data table
    std::vector<int> type_meta_ref_;
    std::unordered_map<void*, UserDataCache> raw_ptrs_;
    std::unordered_map<void*, UserDataCache> shared_ptrs_;
};

namespace detail {
    /* push operator */
#if XLUA_USE_LIGHT_USER_DATA
#else // XLUA_USE_LIGHT_USER_DATA
    template <typename Ty>
    inline void PushPointer(xLuaState* l, Ty* val, tag_weakobj) {
        int index = xLuaAllocWeakObjIndex(val);
        int serial_num = xLuaGetWeakObjSerialNum(index);
        const TypeInfo* info = GetTypeInfoImpl<Ty>();
        l->_PushWeakObjPtr(LuaUserData(LuaUserDataType::kWeakObjPtr, index, serial_num, info));
    }

    template <typename Ty>
    inline void PushPointer(xLuaState* l, Ty* val, tag_internal) {
        int index = AllocInternalIndex(val);
        int serial_num = GetInternalSerialNum(index);
        const TypeInfo* info = GetTypeInfoImpl<Ty>();
        l->_PushWeakObjPtr(LuaUserData(LuaUserDataType::kWeakObjPtr, index, serial_num, info));
    }

    template <typename Ty>
    inline void PushPointer(xLuaState* l, Ty* val, tag_external) {
        const TypeInfo* info = GetTypeInfoImpl<Ty>();
        void* root = GetRootPtr(val, info);
        if (!l->_TryPushRawPtr(root))
            l->_PushRawPtr(root, l->MakeUserData(val, info));
    }
#endif // XLUA_USE_LIGHT_USER_DATA

    template <typename Ty>
    struct Pusher {
        static_assert(IsExternal<Ty>::value || IsExtendLoad<Ty>::value, "");
        static inline void Do(xLuaState* l, const Ty& val) {
            using tag = typename std::conditional<IsExternal<Ty>::value, tag_external, tag_extend>::type;
            PushValue(l, val, tag());
        }

        template <typename Ty>
        static inline void PushValue(xLuaState* l, const Ty& val, tag_extend) {
            ::xLuaPush(l, val);
        }

        template <typename Ty>
        static inline void PushValue(xLuaState* l, const Ty& val, tag_external) {
            static_assert(!IsWeakObjPtr<Ty>::value, "not allow push weak obj value");
            l->PushUserData(val, GetTypeInfoImpl<Ty>());
        }
    };

    template <typename Ty>
    struct Pusher<Ty*> {
        static_assert(IsInternal<Ty>::value || IsExternal<Ty>::value, "");
        static inline void Do(xLuaState* l, Ty* val) {
            using tag = typename std::conditional<IsWeakObjPtr<Ty>::value, tag_weakobj,
                typename std::conditional<IsInternal<Ty>::value, tag_internal, tag_external>::type>::type;
            if (val == nullptr)
                l->Push(nullptr);
            else
                PushPointer(l, val, tag());
        }

#if XLUA_USE_LIGHT_USER_DATA
        static inline void PushPointer(xLuaState* l, Ty* val, tag_weakobj) {
            const TypeInfo* info = GetTypeInfoImpl<Ty>();
            auto ptr = MakeLightPtr(info->external_type_index,
                xLuaAllocWeakObjIndex(val),
                xLuaGetWeakObjSerialNum(index)
            );
            lua_pushlightuserdata(l->GetState(), ptr.ptr_);
        }

        static inline void PushPointer(xLuaState* l, Ty* val, tag_internal) {
            const TypeInfo* info = GetTypeInfoImpl<Ty>();
            auto* global = GlobalVar::GetInstance();
            int index = global->AllocObjIndex(GetRootObjIndex(val), val, info);
            int serial_num = global->GetSerialNum(index);
            auto ptr = MakeLightPtr(0, index, serial_num);
            lua_pushlightuserdata(l->GetState(), ptr.ptr_);
        }

        static inline void PushPointer(xLuaState* l, Ty* val, tag_external) {
            const TypeInfo* info = GetTypeInfoImpl<Ty>();
            auto ptr = MakeLightPtr(info->external_type_index, val);
            lua_pushlightuserdata(l->GetState(), ptr.ptr_);
        }
#else // XLUA_USE_LIGHT_USER_DATA
        //TODO:
#endif // XLUA_USE_LIGHT_USER_DATA
    };

    template <typename Ty>
    struct Pusher<std::shared_ptr<Ty>> {
        static_assert(IsInternal<Ty>::value || IsExternal<Ty>::value, "");
        static inline void Do(xLuaState* l, const std::shared_ptr<Ty>& ptr) {
            if (!ptr) {
                l->Push(nullptr);
            } else {
                l->PushSharedPtr(ptr, GetTypeInfoImpl<Ty>());
            }
        }
    };

    template <typename Ty>
    struct Loader {
        static_assert(IsExternal<Ty>::value || IsExtendLoad<Ty>::value, "");
        using tag = typename std::conditional<IsExternal<Ty>::value, tag_external, tag_extend>::type;
        static inline Ty Do(xLuaState* l, int index) {
            return LoadValue<Ty>(l, index, tag());
        }

        template <typename U>
        static inline U LoadValue(xLuaState* l, int index, tag_extend) {
            return ::xLuaLoad(l, index, Identity<U>());
        }

        template <typename U>
        static inline U LoadValue(xLuaState* l, int index, tag_external) {
            static_assert(!IsWeakObjPtr<U>::value, "not allow weak obj");
            do {
                if (lua_type(xl->GetState(), index) != LUA_TUSERDATA)
                    break;

                void* p = lua_touserdata(xl->GetState(), index);
                if (p == nullptr)
                    break;

                LuaUserData* ud = static_cast<LuaUserData*>(ptr);
                const TypeInfo* info = GetTypeInfoImpl<Ty>();
                if (!IsBaseOf(info, ud->info_))
                    break;

                // copy construct
                return Ty(*static_cast<Ty*>(ud->info_->caster.to_super(ud->obj_, ud->info_, info)));
            } while (false);

            return Ty();
        }
    };

    template <typename Ty>
    struct Loader<Ty*> {
        static_assert(IsInternal<Ty>::value || IsExternal<Ty>::value, "");
        static Ty* Do(xLuaState* xl, int index) {
            const TypeInfo* info = GetTypeInfoImpl<Ty>();
            void* user_data = lua_touserdata(xl->GetState(), index);
            if (user_data == nullptr)
                return nullptr;

            int ty = lua_type(xl->GetState(), index);
            if (ty == LUA_TLIGHTUSERDATA) {
                return GetLightUserDataPtr<Ty>(user_data, info);
            } else if (ty == LUA_TUSERDATA) {
                return GetFullUserDataPtr<Ty>(user_data, info);
            } else {
                return nullptr;
            }
        }
    };

    template <typename Ty>
    struct Loader<std::shared_ptr<Ty>> {
        static_assert(IsInternal<Ty>::value || IsExternal<Ty>::value, "");
        static inline std::shared_ptr<Ty> Do(xLuaState* l, int index) {
            if (lua_type(xl->GetState(), index) != LUA_TUSERDATA)
                return nullptr;

            void* p = lua_touserdata(xl->GetState(), index);
            if (p == nullptr)
                return nullptr;

            LuaUserData* ud = static_cast<LuaUserData*>(p);
            if (ud->type_ != LuaUserDataType::kSharedPtr)
                return nullptr;

            const TypeInfo* info = GetTypeInfoImpl<Ty>();
            if (!IsBaseOf(info, ud->info_))
                return nullptr;

            return std::shared_ptr<Ty>(*static_cast<std::shared_ptr<Ty>*>(ud->GetDataPtr()),
                (Ty*)ud->info_->caster.to_super(ud->obj_, ud->info_, info));
        }
    };

    template <typename Ty>
    struct Loader<std::unique_ptr<Ty>> {
        static_assert(IsInternal<Ty>::value || IsExternal<Ty>::value, "");
        static_assert(std::is_base_of<std::nullptr_t, Ty>::value, "can not load unique_ptr");
        static inline std::unique_ptr<Ty> Do(xLuaState* l, int index) {
            return nullptr;
        }
    };
} // namespace detail

XLUA_NAMESPACE_END
