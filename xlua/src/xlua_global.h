#pragma once
#include "xlua_def.h"

XLUA_NAMESPACE_BEGIN

namespace xlua_internal
{
    typedef void (*TypeRegisterFunc)();

    class TypeInitNode
    {
    protected:
        TypeInitNode(TypeRegisterFunc f);
        ~TypeInitNode();

        TypeInitNode(const TypeInitNode&) = delete;
        TypeInitNode& operator = (const TypeInitNode&) = delete;

    public:
        TypeInitNode* next;
        TypeRegisterFunc func;
    };

    template <typename Ty>
    struct TypeInitializer : private TypeInitNode
    {
        TypeInitializer(TypeRegisterFunc func) : TypeInitNode(func) { }
    };

    class GlobalVar
    {

    };
} // namespace xlua_internal

XLUA_NAMESPACE_END
