#pragma once
#include "xlua_def.h"
#include <vector>
#include <mutex>

XLUA_NAMESPACE_BEGIN

namespace detail
{
    struct TypeDesc;

    class GlobalVar
    {
        friend struct TypeDesc;
    private:
        GlobalVar(int version);
        ~GlobalVar();

    public:
        static bool Startup();
        static GlobalVar* GetInstance();

        void Purge();

    public:
        bool IsValid(const TypeKey& key) const;
        ITypeDesc* AllocType(const char* name, const TypeInfo* super);
        const TypeInfo* GetTypeInfo(const TypeKey& key) const;

    private:
        TypeKey AddTypeInfo(TypeInfo* desc);

    private:
        //std::mutex mutex_;
        int version_;
        std::vector<TypeInfo*> types_;
    };
} // namespace detail

XLUA_NAMESPACE_END
