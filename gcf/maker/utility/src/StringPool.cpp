#include "StringPool.h"
#include <string>
#include <set>

CFG_NAMESPACE_BEGIN

namespace utility
{
    namespace detail
    {
        class StringPoolImpl
        {
        public:
            std::set<std::string> _strs;
        };
    }

    StringPool::StringPool()
    {
        _pool = new detail::StringPoolImpl();
    }

    StringPool::~StringPool()
    {
        delete _pool;
    }

    const char* StringPool::Record(const char* str)
    {
        if (str == nullptr)
            return nullptr;

        auto pir = _pool->_strs.insert(std::string(str));
        return pir.first->c_str();
    }
}

CFG_NAMESPACE_END
