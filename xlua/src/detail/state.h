#pragma once
#include "../xlua_def.h"
#include "traits.h"
#include <memory>
#include <unordered_map>

XLUA_NAMESPACE_BEGIN

namespace detail {
    enum class LuaUserDataType {
        kValue,
        kRawPtr,
        kUniquePtr,
        kSharedPtr,
    };

    struct LuaUserData {
        LuaUserData(LuaUserDataType type, void* obj, const TypeInfo* info)
            : type_(type)
            , obj_(obj)
            , info_(info) {
        }

        virtual ~LuaUserData() {
        }

        virtual bool ConvetSharedPtr(void* dst, const TypeInfo* dst_type) {
            return false;
        }

        LuaUserDataType type_;
        void* obj_;
        const TypeInfo* info_;
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
        LuaUserDataImpl(const std::unique_ptr<Ty>& val, const TypeInfo* info)
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

    struct State
    {
        int meta_table_index_;
        int lua_obj_table_index_;
        int user_data_table_index_;
        std::unordered_map<void*, int> user_datas_; // user_data_ptr -> user_data_ref_index
    };
} // namespace detail

XLUA_NAMESPACE_END
