#pragma once
#include "xlua_def.h"

XLUA_NAMESPACE_BEGIN

namespace xlua_internal
{
    typedef const TypeInfo* (*GetTypeInfoFunc)();

    class TypeNode
    {
        friend class GlobalVar;
    protected:
        TypeNode(GetTypeInfoFunc f);
        ~TypeNode();

        TypeNode(const TypeNode&) = delete;
        TypeNode& operator = (const TypeNode&) = delete;

    private:
        TypeNode* next;
        GetTypeInfoFunc func;
    };

    ITypeDesc* AllocTypeInfo();
    const TypeInfo* GetTypeInfo(int index);

    class GlobalVar
    {


    public:
        static const TypeInfo* GetTypeInfo(const TypeNode& node);
    };
} // namespace xlua_internal

XLUA_NAMESPACE_END
