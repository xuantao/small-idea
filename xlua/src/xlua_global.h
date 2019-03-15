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
        GlobalVar();
        ~GlobalVar();

    public:
        static bool Startup();
        static GlobalVar* GetInstance();

        void Purge();

    public:
        ITypeDesc* AllocType(const char* name, const TypeInfo* super);
        const TypeInfo* GetTypeInfo(int index) const;

    private:
        TypeKey AddTypeInfo(TypeDesc* desc);

    private:
        //std::mutex mutex_;
        std::vector<TypeInfo*> types_;
    };
} // namespace detail

XLUA_NAMESPACE_END
