#include "logger.h"

UTILITY_NAMESPACE_BEGIN
//
//namespace {
//    struct default_proxy final : ilog_proxy
//    {
//        void log(int level, const char* msg) override
//        {
//            printf("%s", msg);
//        }
//    };
//
//    static default_proxy s_default;
//    static ilog_proxy* s_proxy = &s_default;
//}
//
//void set_log_proxy(ilog_proxy* proxy)
//{
//    if (proxy == nullptr)
//        s_proxy = &s_default;
//    else
//        s_proxy = proxy;
//}
//
//ilog_proxy* get_log_proxy()
//{
//    return s_proxy;
//}

UTILITY_NAMESPACE_END
