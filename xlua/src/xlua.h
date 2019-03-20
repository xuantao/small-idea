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
}

bool Startup();
void Shutdown();

class xLuaTable { };
class xLuaFunction { };

enum class ValueType
{
    kLightUserData,
    kFullUserData,
};

class xLuaState {
public:
    friend class detail::GlobalVar;
    xLuaState(lua_State* l, bool attach);
    ~xLuaState();

public:
    inline bool IsAttach() const { return attach_; }
    inline lua_State* GetState() const { return state_; }

public:
    template <typename Ty>
    inline void Push(const Ty& val) {
        detail::Pusher<typename std::decay<Ty>::type>::Do(this, val);
    }

    template <typename Ty>
    inline void Push(std::unique_ptr<Ty> ptr) {
        static_assert(IsInternal<Ty>::value || IsExternal<Ty>::value, "");
        _PushUniquePtr(MakeUserData(ptr, detail::GetTypeInfoImpl<Ty>()));
    }

    template <typename Ty>
    inline Ty Load(int index) {
        return detail::Loader<typename std::decay<Ty>::type>::Do(this, index);
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
    //void Push(const xLuaTable& val) { }
    //void Push(const xLuaFunction& val) { }

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
    template<> inline const std::string Load<const std::string>(int index) { return false; }

    const char* GetTypeName(int index) const;

    ValueType GetType(int index) const;

public:
    bool _TryPushSharedPtr(void* ptr) const;
    void _PushSharedPtr(void* root, detail::LuaUserData* user_data);
    void _PushUniquePtr(detail::LuaUserData* user_data);
    void _PushValue(detail::LuaUserData* user_data);

    bool _TryPushRawPtr(void* root) const;
    void _PushRawPtr(void* root, detail::LuaUserData* user_data);

    bool _IsLightUserData(int index) const { return false; }
    bool _IsFullUserData(int index) const { return false; }

#if XLUA_USE_LIGHT_USER_DATA
    void _PushLightPtr(detail::LightDataPtr ptr);
#else
    void _PushWeakObjPtr(const detail::LuaUserData user_data);
#endif // XLUA_USE_LIGHT_USER_DATA

    template <typename Ty>
    inline detail::LuaUserData* MakeUserData(const Ty& val, const TypeInfo* info) {
        using DataType = detail::LuaUserDataImpl<Ty>;
        void* mem = AllocUserData(sizeof(DataType));
        return new (mem) DataType(val, info);
    }

    template <typename Ty>
    inline detail::LuaUserData* MakeUserData(std::unique_ptr<Ty> val, const TypeInfo* info) {
        using DataType = detail::LuaUserDataImpl<std::unique_ptr<Ty>>;
        void* mem = AllocUserData(sizeof(DataType));
        return new (mem) DataType (val, info);
    }

    void* GetUserDataPtr(int index);
private:

    void* AllocUserData(size_t size) { return nullptr; }


private:
    bool attach_;
    lua_State* state_;
    int meta_table_index_;
    int lua_obj_table_index_;
    int user_data_table_index_;
    std::unordered_map<void*, int> raw_ptrs_;
    std::unordered_map<void*, int> shared_ptrs_;
};

namespace detail {
    /* push operator */
    template <typename Ty>
    inline void PushValue(xLuaState* l, const Ty& val, tag_extend) {
        ::xLuaPush(l, val);
    }

    template <typename Ty>
    inline void PushValue(xLuaState* l, const Ty& val, tag_external) {
        static_assert(!IsWeakObjPtr<Ty>::value, "not allow push weak obj value");
        l->_PushValue(MakeUserData(val, GetTypeInfoImpl<Ty>()));
    }

#if XLUA_USE_LIGHT_USER_DATA
    template <typename Ty>
    inline void PushPointer(xLuaState* l, Ty* val, tag_weakobj) {
        int index = xLuaAllocWeakObjIndex(val);
        int serial_num = xLuaGetWeakObjSerialNum(index);
        const TypeInfo* info = GetTypeInfoImpl<Ty>();
        l->_PushLightPtr(MakeLightPtr(info->light_index, index, serial_num));
    }

    template <typename Ty>
    inline void PushPointer(xLuaState* l, Ty* val, tag_internal) {
        const TypeInfo* info = GetTypeInfoImpl<Ty>();
        int index = GlobalVar::GetInstance()->AllocObjIndex(GetObjIndex(val), val, info);
        int serial_num = GlobalVar::GetInstance()->GetObjSerialNum(index);
        l->_PushLightPtr(MakeLightPtr(0, index, serial_num));
    }

    template <typename Ty>
    inline void PushPointer(xLuaState* l, Ty* val, tag_external) {
        const TypeInfo* info = GetTypeInfoImpl<Ty>();
        l->_PushLightPtr(MakeLightPtr(info->light_index, val));
    }
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
            l->_PushRawPtr(root, MakeUserData(val, info));
    }
#endif // XLUA_USE_LIGHT_USER_DATA

    template <typename Ty>
    struct Pusher {
        static_assert(IsExternal<Ty>::value || IsExtendLoad<Ty>::value, "");
        static inline void Do(xLuaState* l, const Ty& val) {
            using tag = typename std::conditional<IsExternal<Ty>::value, tag_external, tag_extend>::type;
            PushValue(l, val, tag());
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
    };

    template <typename Ty>
    struct Pusher<std::shared_ptr<Ty>> {
        static_assert(IsInternal<Ty>::value || IsExternal<Ty>::value, "");
        static inline void Do(xLuaState* l, const std::shared_ptr<Ty>& ptr) {
            if (!ptr) {
                l->Push(nullptr);
            } else {
                const TypeInfo* info = GetTypeInfoImpl<Ty>();
                void* root = GetRootPtr(ptr.get(), info);
                if (!l->_TryPushSharedPtr(root))
                    l->_PushSharedPtr(root, l->MakeUserData(ptr, info));
            }
        }
    };

    /* load operator */
    template <typename Ty>
    inline Ty LoadValue(xLuaState* l, int index, tag_extend) {
        return ::xLuaLoad(l, index, Identity<Ty>());
    }

    template <typename Ty>
    inline Ty LoadValue(xLuaState* l, int index, tag_external) {
        static_assert(!IsWeakObjPtr<Ty>::value, "not allow weak obj");
        //LuaUserData* data = nullptr;
        //switch (data->type_)
        //{
        //default:
        //    break;
        //}
        return Ty();
    }

    template <typename Ty>
    inline Ty* LoadPointer(xLuaState* l, int index, tag_weakobj) {
        return nullptr;
    }

    template <typename Ty>
    inline Ty* LoadPointer(xLuaState* l, int index, tag_external) {
        return nullptr;
    }

    template <typename Ty>
    inline Ty* LoadPointer(xLuaState* l, int index, tag_internal) {
        return nullptr;
    }

    template <typename Ty>
    struct Loader {
        static_assert(IsExternal<Ty>::value || IsExtendLoad<Ty>::value, "");
        using tag = typename std::conditional<IsExternal<Ty>::value, tag_external, tag_extend>::type;
        static inline Ty Do(xLuaState* l, int index) {
            return LoadValue<Ty>(l, index, tag());
        }
    };

    template <typename Ty>
    struct Loader<Ty*> {
        static_assert(IsInternal<Ty>::value || IsExternal<Ty>::value, "");
        static inline Ty* Do(xLuaState* l, int index) {
            const TypeInfo* info = GetTypeInfoImpl<Ty>();
            ValueType value_type = l->GetType(index);
            void* user_data = l->GetUserDataPtr(index);
            Ty* ret = nullptr;
            if (user_data == nullptr)
                return nullptr;

            if (value_type == ValueType::kLightUserData) {
                LightDataPtr ud = MakeLightPtr(user_data);
                if (ud.type_ == 0) {
                    ArrayObj* obj = GlobalVar::GetInstance()->GetArrayObj(ud.index_);
                    if (obj == nullptr || obj->serial_num_ != ud.serial_ || obj->obj_ == nullptr) {
                        //TODO: log type info not equal
                        return nullptr;
                    }

                    if (!IsBaseOf(info, obj->info_)) {
                        //TODO: log type info not equal
                        return nullptr;
                    }

                    return obj->info_->converter.convert_up(obj->obj_, obj->info_, info);
                } else {
                    const TypeInfo* dst_type = GlobalVar::GetInstance()->GetExternalInfo(ud.type_);
                    if (dst_type == nullptr || !IsBaseOf(info, dst_type)) {
                        //TODO: log type info not equal
                        return nullptr;
                    }

                    if (dst_type->is_weak_obj) {
                        void* obj = xLuaGetWeakObjPtr(ud.index_);
                        if (obj == nullptr || ud.serial_ != xLuaGetWeakObjSerialNum(ud.index_)) {
                            //TODO:
                            return nullptr;
                        }
                        return static_cast<Ty*>(static_cast<XLUA_WEAK_OBJ_BASE_TYPE*>(obj));
                    } else {
                        return dst_type->converter.convert_up(ud.ToRawPtr(), dst_type, info);
                    }
                }
            } else if (value_type == ValueType::kFullUserData) {
                LuaUserData* ud = static_cast<LuaUserData*>(user_data);
                if (ud->obj_ == nullptr) {
                    return nullptr;
                }
                if (!IsBaseOf(info, ud->info_)) {
                    return nullptr;
                }

                if (ud->type_ == LuaUserDataType::kRawPtr) {
                    return ud->info_->converter.convert_up(ud->obj_, ud->info_, info);
                } else if (ud->type_ == LuaUserDataType::kValue) {

                } else if (ud->type_ == LuaUserDataType::kSharedPtr) {

                } else if (ud->type_ == LuaUserDataType::kUniquePtr) {

                } else if (ud->type_ == LuaUserDataType::kLuaObjPtr) {

                } else if (ud->type_ == LuaUserDataType::kWeakObjPtr) {

                } else {
                    //TODO: log unknown obj type
                    return nullptr;
                }
            }

            return ret;
        }
    };

    template <typename Ty>
    struct Loader<std::shared_ptr<Ty>> {
        static_assert(IsInternal<Ty>::value || IsExternal<Ty>::value, "");
        static inline std::shared_ptr<Ty> Do(xLuaState* l, int index) {
            return nullptr;
        }
    };

    template <typename Ty>
    struct Loader<std::unique_ptr<Ty>> {
        static_assert(std::is_same<Ty, std::nullptr_t>::value, "not allow to load unique_ptr");
        static inline std::unique_ptr<Ty> Do(xLuaState* l, int index) {
            /* can not load unique_ptr from xlua */
            return nullptr;
        }
    };
} // namespace detail

XLUA_NAMESPACE_END
