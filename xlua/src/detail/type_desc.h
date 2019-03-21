#pragma once
#include "../xlua_def.h"
#include <vector>

XLUA_NAMESPACE_BEGIN

namespace detail
{
    struct TypeDesc : public ITypeDesc
    {
        friend class GlobalVar;
    public:
        TypeDesc(GlobalVar* mgr, TypeCategory category, bool is_wak_obj, const char* name, const TypeInfo* super)
            : mgr_(mgr)
            , category_(category)
            , is_weak_obj_(is_wak_obj)
            , name_(name)
            , super_(super) {
        }

        ~TypeDesc() { }

    public:
        static void Free(TypeInfo* info);

    public:
        void SetConverter(LuaPointerConvert up, LuaPointerConvert down, LuaSharedPtrConvert shared_ptr) override {
            convert_up_ = up;
            convert_down_ = down;
            convert_shared_ptr_ = shared_ptr;
        }

        void AddMember(TypeMember member, bool global) override {
            if (global)
                globals_.push_back(member);
            else
                members_.push_back(member);
        }

        const TypeInfo* Finalize() override;

    private:
        GlobalVar* mgr_;
        TypeCategory category_;
        bool is_weak_obj_;
        const char* name_;
        const TypeInfo* super_;
        LuaPointerConvert convert_up_;
        LuaPointerConvert convert_down_;
        LuaSharedPtrConvert convert_shared_ptr_;
        std::vector<TypeMember> members_;
        std::vector<TypeMember> globals_;
    };
} // namespace detail

XLUA_NAMESPACE_END
