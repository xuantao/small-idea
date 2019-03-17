#pragma once
#include "type_desc.h"
#include "global.h"
#include <memory>

XLUA_NAMESPACE_BEGIN

namespace detail
{
    TypeKey TypeDesc::Finalize() {
        std::unique_ptr<TypeDesc> ptr(this);  // auto free
                                              //if (s_global == nullptr)
        return TypeKey();
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
