#include "test.h"
#include "test_util.h"
#include "utility/to_string.h"
#include "utility/logger.h"

UTILITY_USING_NAMESPACE

namespace
{
    struct TestToStringObj
    {
        friend int to_string(char* buff, size_t size, const TestToStringObj&)
        {
            return snprintf(buff, size, "TestToStringObj");
        }

        void print() {}
        int a;
    };
}

void test_logger()
{
    //std::function
    TestToStringObj obj;
    printf("test logger ...\n");
    LOG_INFO(1, 1, "xuantao", true, obj, obj.a, &TestToStringObj::print);
    LOG_VALS(1, 1, "xuantao", true, obj, obj.a, &TestToStringObj::print);
    LOG_FORMAT(1,"1:{0}, 2:{1}, 3:{2}, 4:{3}", 1, "xuantao", true, obj);
}
