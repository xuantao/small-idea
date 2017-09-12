#pragma once
#include "CfgDef.h"

CFG_NAMESPACE_BEGIN

namespace utility
{
    namespace detail
    {
        class StringPoolImpl;
    }

    /*
     * 字符串池，存储全局字符串
    */
    class StringPool
    {
    public:
        StringPool();
        ~StringPool();

        StringPool(const StringPool&) = delete;
        StringPool& operator = (const StringPool&) = delete;
    public:
        const char* Record(const char* str);

    protected:
        detail::StringPoolImpl* _pool = nullptr;
    };
}

CFG_NAMESPACE_END
