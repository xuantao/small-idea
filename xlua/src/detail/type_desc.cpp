#pragma once
#include "type_desc.h"
#include "global.h"
#include <memory>

XLUA_NAMESPACE_BEGIN

namespace detail
{
    static const char* PerifyMemberName(const char* name) {
        while (const char* sub = ::strstr(name, "::"))
            name = sub + 2;

        // remove prefix: "m_"
        if (name[0] == 'm' && name[1] == '_')
            name += 2;

        // remove prefix: "lua_"
        if ((name[0] == 'l' || name[0] == 'L')
            && (name[0] == 'u' || name[0] == 'U')
            && (name[0] == 'a' || name[0] == 'A')
            ) {
            name += 3;
            if (name[0] == '_')
                ++name;
        }
        return name;
    }

    void TypeDesc::AddMember(const char* name, LuaFunction func, bool global) {
        auto& vec = global ? global_funcs_ : funcs_;
        vec.push_back(MemberFunc{PerifyMemberName(name), func});
    }

    void TypeDesc::AddMember(const char* name, LuaIndexer getter, LuaIndexer setter, bool global) {
        auto& vec = global ? global_vars_ : vars_;
        vec.push_back(MemberVar{PerifyMemberName(name), getter, setter});
    }

    const TypeInfo* TypeDesc::Finalize() {
        std::unique_ptr<TypeDesc> ptr(this);  // auto free
        TypeInfo* info = (TypeInfo*)mgr_->SerialAlloc(sizeof(TypeInfo));

        PerifyTypeName(info);
        info->category = category_;
        info->super = super_;
        info->is_weak_obj = is_weak_obj_;
        info->caster = caster_;

        vars_.push_back(MemberVar{nullptr, nullptr, nullptr});
        info->vars = (MemberVar*)mgr_->SerialAlloc(sizeof(MemberVar) * vars_.size());
        for (size_t i = 0; i < vars_.size(); ++i)
            info->vars[i] = vars_[i];

        funcs_.push_back(MemberFunc{nullptr, nullptr});
        info->funcs = (MemberFunc*)mgr_->SerialAlloc(sizeof(MemberFunc) * funcs_.size());
        for (size_t i = 0; i < funcs_.size(); ++i)
            info->funcs[i] = funcs_[i];

        global_vars_.push_back(MemberVar{nullptr, nullptr, nullptr});
        info->global_vars = (MemberVar*)mgr_->SerialAlloc(sizeof(MemberVar) * global_vars_.size());
        for (size_t i = 0; i < global_vars_.size(); ++i)
            info->global_vars[i] = global_vars_[i];

        global_funcs_.push_back(MemberFunc{nullptr, nullptr});
        info->global_funcs = (MemberFunc*)mgr_->SerialAlloc(sizeof(MemberFunc) * global_funcs_.size());
        for (size_t i = 0; i < global_funcs_.size(); ++i)
            info->global_funcs[i] = global_funcs_[i];

        mgr_->AddTypeInfo(info);
        return info;
    }

    void TypeDesc::PerifyTypeName(TypeInfo* info) {
        const char* name = name_;
        while (*name == ':')
            ++name;

        size_t len = strlen(name);
        const char* sub = ::strstr(name, "::");
        if (sub == nullptr) {
            info->type_name = name;
        } else {
            char* dst = (char*)mgr_->SerialAlloc(len + 1);
            info->type_name = dst;
            do {
                size_t len = sub - name;
                memcpy(dst, name, len);
                dst[len] = '.';
                dst += len + 1;
                name = sub + 2;

                sub = ::strstr(name, "::");
            } while (sub);
        }
    }
} // namespace detail

XLUA_NAMESPACE_END
