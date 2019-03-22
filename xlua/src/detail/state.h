#pragma once
#include "../xlua_def.h"
#include "traits.h"
#include "global.h"
#include <memory>
#include <unordered_map>

XLUA_NAMESPACE_BEGIN

namespace detail {
    inline const TypeInfo* GetRootType(const TypeInfo* info) {
        while (info->super)
            info = info->super;
        return info;
    }

    inline void* GetRootPtr(void* ptr, const TypeInfo* info) {
        const TypeInfo* root = GetRootType(info);
        if (info == root)
            return ptr;
        return info->converter.convert_up(ptr, info, root);
    }

    inline bool IsBaseOf(const TypeInfo* base, const TypeInfo* info) {
        while (info && info != base)
            info = info->super;
        return info == base;
    }

    template <typename Ty>
    inline int AllocInternalIndex(Ty* ptr) {
        return 0;
    }

    inline int GetInternalSerialNum(int index) {
        return 0;
    }

    template <typename Ty>
    inline ObjIndex& GetObjIndex(Ty* ptr) {
        using declare = typename Ty::LuaDeclare;
        return ObjIndexDetect<typename declare::self, typename declare::super>::Detect(ptr);
    }

#ifdef XLUA_USE_LIGHT_USER_DATA
    struct LightDataPtr {
        union {
            struct {
                void* ptr_;
            };
            struct {
                struct {
                    uint32_t serial_;
                };
                struct {
                    uint32_t index_ : 24;
                    uint32_t type_ : 8;
                };
            };
        };

        inline void* ToRawPtr() const {
            return (void*)(reinterpret_cast<uint64_t>(ptr_) & 0x00ffffffffffffff);
        }
    };

    inline LightDataPtr MakeLightPtr(void* ptr) {
        LightDataPtr ud;
        ud.ptr_ = ptr;
        return ud;
    }

    inline LightDataPtr MakeLightPtr(int type, int index, int serial_num) {
        assert((0xff000000 & index) == 0);
        LightDataPtr ptr;
        ptr.serial_ = serial_num;
        ptr.index_ = index;
        ptr.type_ = (unsigned char)type;
        return ptr;
    }

    inline LightDataPtr MakeLightPtr(int type, void* p) {
        assert((0xff00000000000000 & reinterpret_cast<uint64_t>(p)) == 0);
        LightDataPtr ptr;
        ptr.ptr_ = p;
        ptr.type_ = (unsigned char)type;
        return ptr;
    }

#endif // XLUA_USE_LIGHT_USER_DATA

    enum class LuaUserDataType {
        kValue,
        kRawPtr,
        kSharedPtr,
        kUniquePtr,
        kLuaObjPtr,
        kWeakObjPtr,
    };

    struct LuaUserData {
        LuaUserData(LuaUserDataType type, void* obj, const TypeInfo* info)
            : type_(type)
            , info_(info)
            , obj_(obj) {
        }

        LuaUserData(LuaUserDataType type, int32_t index, int32_t serial, const TypeInfo* info)
            : type_(type)
            , info_(info)
            , index_(index)
            , serial_(serial) {
        }

        virtual ~LuaUserData() {}

        virtual void* GetDataPtr() { return nullptr; }

        LuaUserDataType type_;
        const TypeInfo* info_;
        union {
            void* obj_;
            struct {
                int32_t index_;
                int32_t serial_;
            };
        };
    };

    template <typename Ty>
    struct LuaUserDataImpl : LuaUserData {
        LuaUserDataImpl(const Ty& val, const TypeInfo* info)
            : LuaUserData(LuaUserDataType::kValue, nullptr, info)
            , val_(val) {
            obj_ = &val_;
        }

        virtual ~LuaUserDataImpl() { }

        void* GetDataPtr() override { return &val_; }

        Ty val_;
    };

    template <typename Ty>
    struct LuaUserDataImpl<Ty*> : LuaUserData {
        LuaUserDataImpl(Ty* val, const TypeInfo* info)
            : LuaUserData(LuaUserDataType::kRawPtr, val, info) {
        }
    };

    template <typename Ty>
    struct LuaUserDataImpl<std::unique_ptr<Ty>> : LuaUserData {
        LuaUserDataImpl(std::unique_ptr<Ty>&& val, const TypeInfo* info)
            : LuaUserData(LuaUserDataType::kUniquePtr, nullptr, info)
            , val_(std::move(val)) {
            obj_ = val_.get();
        }

        virtual ~LuaUserDataImpl() {}

        void* GetDataPtr() override { return &val_; }

        std::unique_ptr<Ty> val_;
    };

    template <typename Ty>
    struct LuaUserDataImpl<std::shared_ptr<Ty>> : LuaUserData {
        LuaUserDataImpl(const std::shared_ptr<Ty>& val, const TypeInfo* info)
            : LuaUserData(LuaUserDataType::kSharedPtr, nullptr, info)
            , val_(val) {
            obj_ = val_.get();
        }

        virtual ~LuaUserDataImpl() {}

        void* GetDataPtr() override { return &val_; }

        std::shared_ptr<Ty> val_;
    };

    template <typename Ty>
    inline LuaUserData* MakeUserData(const Ty& val, const TypeInfo* info) {
        return new LuaUserDataImpl<Ty>(val, info);
    }

    template <typename Ty>
    inline LuaUserData* MakeUserData(std::unique_ptr<Ty>&& val, const TypeInfo* info) {
        return new LuaUserDataImpl<std::unique_ptr<Ty>>(std::move(val), info);
    }

    template <typename Ty>
    Ty* GetLightUserDataPtr(void* user_data, const TypeInfo* info) {
#if XLUA_USE_LIGHT_USER_DATA
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

            return (Ty*)obj->info_->converter.convert_up(obj->obj_, obj->info_, info);
        }
        else {
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
            }
            else {
                return (Ty*)dst_type->converter.convert_up(ud.ToRawPtr(), dst_type, info);
            }
        }
#endif // XLUA_USE_LIGHT_USER_DATA
        return nullptr;
    }
} // namespace detail

XLUA_NAMESPACE_END
