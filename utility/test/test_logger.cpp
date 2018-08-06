#include "test.h"
#include "test_util.h"
#include "utility/to_string.h"
#include "utility/logger.h"

UTILITY_USING_NAMESPACE

struct TestToStringObj
{
    friend int to_string(char* buff, size_t size, const TestToStringObj&)
    {
        return snprintf(buff, size, "TestToStringObj");
    }

    void print() {}
    int a;
};

//int to_string(char* buff, size_t size, const TestToStringObj&)
//{
//    return snprintf(buff, size, "TestToStringObj");
//}

void test_logger()
{
    constexpr size_t BUFF_SIZE = 1024;

    char buff[BUFF_SIZE];
    to_str_sep(buff, BUFF_SIZE, ", ", false, 'c', "string");
    printf("test_logger %s\n", buff);

    to_str_sep(buff, BUFF_SIZE, ", ", TestToStringObj());
    printf("test_logger %s\n", buff);

    to_str_mul(buff, BUFF_SIZE, ", ", false, 'c', "string");
    printf("test_logger %s\n", buff);

    to_str_mul(buff, BUFF_SIZE, ", ", TestToStringObj());
    printf("test_logger %s\n", buff);

    to_str_sep(buff, BUFF_SIZE, ", ", &TestToStringObj::print, &TestToStringObj::a);
    printf("test_logger %s\n", buff);

    to_str_format(buff, BUFF_SIZE, "{0}, {1}", &TestToStringObj::print, &TestToStringObj::a);
    printf("test_logger %s\n", buff);

    LOG_INFO(1, 1, "xuantao", true);
    LOG_VALS(1, 1, "xuantao", true);
    LOG_FORMAT(1,"1:{0}, 2:{1}, 3:{2} ", 1, "xuantao", true);
}
