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
        TypeDesc(GlobalVar* mgr, const char* name, const TypeInfo* super)
            : mgr_(mgr)
            , name_(name)
            , super_(super) {
        }

        ~TypeDesc() { }

    public:
        void SetConverter(LuaConverter up, LuaConverter down) override {
            convert_up_ = up;
            convert_down_ = down;
        }

        void AddMember(TypeMember member, bool global) override {
            if (global)
                globals_.push_back(member);
            else
                members_.push_back(member);
        }

        TypeKey Finalize() override;

    private:
        GlobalVar* mgr_;
        const char* name_;
        const TypeInfo* super_;
        LuaConverter convert_up_;
        LuaConverter convert_down_;
        std::vector<TypeMember> members_;
        std::vector<TypeMember> globals_;
    };
} // namespace detail

XLUA_NAMESPACE_END
