#pragma once
#include "../xlua_def.h"
#include "traits.h"
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

    template <typename Ty>
    inline int AllocInternalIndex(Ty* ptr) {
        return 0;
    }

    inline int GetInternalSerialNum(int index) {
        return 0;
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
    };

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
        kUniquePtr,
        kSharedPtr,
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

        virtual ~LuaUserData() {
        }

        virtual bool ConvetSharedPtr(void* dst, const TypeInfo* dst_type) {
            return false;
        }

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

        virtual ~LuaUserDataImpl() {
        }

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
        LuaUserDataImpl(std::unique_ptr<Ty>& val, const TypeInfo* info)
            : LuaUserData(LuaUserDataType::kUniquePtr, nullptr, info)
            , val_(val) {
            obj_ = val_.get();
        }

        virtual ~LuaUserDataImpl() {
        }

        std::unique_ptr<Ty> val_;
    };

    template <typename Ty>
    struct LuaUserDataImpl<std::shared_ptr<Ty>> : LuaUserData {
        LuaUserDataImpl(const std::shared_ptr<Ty>& val, const TypeInfo* info)
            : LuaUserData(LuaUserDataType::kSharedPtr, nullptr, info)
            , val_(val) {
            obj_ = val_.get();
        }

        virtual ~LuaUserDataImpl() {
        }

        virtual bool ConvetSharedPtr(void* dst, const TypeInfo* dst_type) {
            return info_->converter.convert_shared_ptr(&val_, info_, dst, dst_type);
        }

        std::shared_ptr<Ty> val_;
    };

    template <typename Ty>
    inline LuaUserData* MakeUserData(const Ty& val, const TypeInfo* info) {
        return new LuaUserDataImpl<Ty>(val, info);
    }

    template <typename Ty>
    inline LuaUserData* MakeUserData(std::unique_ptr<Ty> val, const TypeInfo* info) {
        return new LuaUserDataImpl<std::unique_ptr<Ty>>(val, info);
    }
} // namespace detail

XLUA_NAMESPACE_END
