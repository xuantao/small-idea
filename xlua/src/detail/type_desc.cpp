#pragma once
#include "type_desc.h"
#include "global.h"
#include <memory>

XLUA_NAMESPACE_BEGIN

namespace detail
{
    //static const char* PerifyTypeName(const char* name, char* buff, size_t size)
    //{
    //    if (name == nullptr || *name == 0)
    //        return name;

    //    while (*name == ':')
    //        ++name;

    //    const char* sub = ::strstr(name, "::");
    //    if (sub == nullptr)
    //        return name;

    //    char* dst = buff;
    //    do
    //    {
    //        size_t len = sub - name + 1;
    //        memcpy_s(dst, size, name, len);
    //        dst += len;
    //        size -= len;
    //        name = sub + 2;
    //        dst[-1] = '.';

    //        sub = ::strstr(name, "::");
    //    } while (sub);
    //    return buff;
    //}

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

    static void ZeroMember(TypeMember& mem) {
        mem.type = MemberType::kInvalid;
        mem.name = nullptr;
        mem.getter = nullptr;
        mem.setter = nullptr;
    }

    void TypeDesc::AddMember(const char* name, LuaFunction func, bool global)
    {
        auto& vec = global ? globals_ : members_;
        vec.push_back(TypeMember{MemberType::kFunction, PerifyMemberName(name), func});
    }

    void TypeDesc::AddMember(const char* name, LuaIndexer getter, LuaIndexer setter, bool global)
    {
        auto& vec = global ? globals_ : members_;
        TypeMember mem{MemberType::kVariate, PerifyMemberName(name), nullptr};
        mem.getter = getter;
        mem.setter = setter;
        vec.push_back(mem);
    }

    const TypeInfo* TypeDesc::Finalize() {
        std::unique_ptr<TypeDesc> ptr(this);  // auto free
        TypeInfo* info = (TypeInfo*)mgr_->SerialAlloc(sizeof(TypeInfo));

        PerifyTypeName(info);
        info->category = category_;
        info->super = super_;
        info->is_weak_obj = is_weak_obj_;
        info->caster = caster_;
        info->members = (TypeMember*)mgr_->SerialAlloc(sizeof(TypeMember) * (members_.size() + 1));
        info->globals = (TypeMember*)mgr_->SerialAlloc(sizeof(TypeMember) * (globals_.size() + 1));

        for (size_t i = 0; i < members_.size(); ++i)
            info->members[i] = members_[i];
        ZeroMember(info->members[members_.size()]);

        for (size_t i = 0; i < globals_.size(); ++i)
            info->globals[i] = globals_[i];
        ZeroMember(info->globals[globals_.size()]);

        mgr_->AddTypeInfo(info);
        return info;
    }

    void TypeDesc::PerifyTypeName(TypeInfo* info) {
        const char* name = name_;
        while (*name == ':')
            ++name;

        size_t len = strlen(name);
        const char* sub = ::strstr(name, "::");
        if (sub == nullptr)
        {
            info->type_name = name;
        }
        else
        {
            char* dst = (char*)mgr_->SerialAlloc(len + 1);
            info->type_name = dst;
            do
            {
                size_t len = sub - name;
                memcpy(dst, name, len);
                dst[len] = '.';
                dst += len + 1;
                name = sub + 2;

                sub = ::strstr(name, "::");
            } while (sub);
        }

        //int buf_len = snprintf(nullptr, 0, "std::shared_ptr<%s>", info->type_name);
        char* buf = (char*)mgr_->SerialAlloc(128);
        info->shared_ptr_name = buf;
        snprintf(buf, 128, "std::shared_ptr<%s>", info->type_name);

        //buf_len = snprintf(nullptr, 0, "std::unique_ptr<%s>", info->type_name);
        buf = (char*)mgr_->SerialAlloc(128);
        info->unique_ptr_name = buf;
        snprintf(buf, 128, "std::unique_ptr<%s>", info->type_name);
    }

} // namespace detail

XLUA_NAMESPACE_END
