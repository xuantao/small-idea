#pragma once
#include "type_desc.h"
#include "global.h"
#include <memory>

XLUA_NAMESPACE_BEGIN

namespace detail
{
    static void* DummyPtrCast(void*, const TypeInfo*, const TypeInfo*) { return nullptr; }
    static bool DummySharedPtrCast(void*, const TypeInfo*, void*, const TypeInfo*) { return false; }

    void TypeDesc::AddMember(const char* name, LuaFunction func, bool glboal)
    {

    }

    void TypeDesc::AddMember(const char* name, LuaIndexer getter, LuaIndexer setter, bool glboal)
    {

    }

    const TypeInfo* TypeDesc::Finalize() {
        std::unique_ptr<TypeDesc> ptr(this);  // auto free
        TypeInfo* info = nullptr;// static_cast<TypeInfo*>(buff);


        return info;
        //return GlobalVar::GetInstance()->AddTypeInfo(info);

        //convert_up_ = convert_down_ ?

        /*
        void* buff = new char[sizeof(TypeInfo)
        + sizeof(TypeFunc) * (member_func_.size() + static_member_func_.size() + 2)
        + sizeof(TypeVar) * (member_var_.size() + static_var_func_.size() + 2)
        ];
        TypeInfo* info = static_cast<TypeInfo*>(buff);

        info->name = PerifyTypeName(name_);
        info->super = super_;
        info->convert_up = (convert_up_ == nullptr) ? &DummyCast : convert_up_;
        info->convert_down = (convert_down_ == nullptr) ? &DummyCast : convert_down_;

        TypeFunc* func = reinterpret_cast<TypeFunc*>(&info[1]);

        info->funcs = func;
        for (const auto& f : member_func_)
        {
        *func = f;
        ++func;
        }
        func->func = nullptr;
        func->name = nullptr;
        ++func;

        info->static_funcs = func;
        for (const auto& f : static_member_func_)
        {
        *func = f;
        ++func;
        }
        func->func = nullptr;
        func->name = nullptr;
        ++func;

        TypeVar* var = reinterpret_cast<TypeVar*>(func);
        info->vars = var;
        for (const auto& v : member_var_)
        {
        *var = v;
        ++var;
        }
        var->name = nullptr;
        var->getter = nullptr;
        var->setter = nullptr;
        ++var;

        info->static_vars = var;
        for (const auto& v : static_var_func_)
        {
        *var = v;
        ++var;
        }
        var->name = nullptr;
        var->getter = nullptr;
        var->setter = nullptr;

        TypeKey key = s_global->AddTypeInfo(info);
        if (!key.IsValid())
        delete[] reinterpret_cast<char*>(info);
        return key;*/
    }
} // namespace detail

XLUA_NAMESPACE_END
