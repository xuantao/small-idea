#include "to_string.h"

namespace
{
#pragma warning( push )
#pragma warning( disable : 4200)
    struct test_obj {
        char data[2];
        char data1[0];
    };
#pragma warning( pop )

    struct test_obj2 {
        int get() { return 1; }
        int get_const(int) const { return 1; }

        static int s_get() { return 3; }

        int a;
        const int ca = 10;
        static int sa;

        friend int to_string(char* buff, size_t len, const test_obj2& obj)
        {
            return snprintf(buff, len, "test_obj2_1: %p", &obj);
        }
    };

    int test_obj2::sa = 1;

    enum TestEnum
    {
        teType1,
        teType2,
    };

    enum class TestEnum2
    {
        te2Type1,
        te2Type2,
    };
}

#define TEST_TO_STR(val)            \
    to_string(buff, BUFF_LEN, val); \
    printf("to_string, %s: %s\n", #val, buff)

#define TEST_TO_STR_T(val)                          \
    UTILITY_NAMESPACE to_str_t(buff, BUFF_LEN, val); \
    printf("to_string, %s: %s\n", #val, buff)

static void test_to_string_normal()
{
    constexpr size_t BUFF_LEN = 64;
    char buff[BUFF_LEN];

    TEST_TO_STR("hello world");
    TEST_TO_STR(std::string("hello world"));
    TEST_TO_STR(true);
    TEST_TO_STR(false);
    TEST_TO_STR('C');
    TEST_TO_STR(short(1));
    TEST_TO_STR(unsigned short(2));
    TEST_TO_STR(int(3));
    TEST_TO_STR(unsigned int(4));
    TEST_TO_STR(unsigned(5));
    TEST_TO_STR(long(6));
    TEST_TO_STR(unsigned long(7));
    TEST_TO_STR(long long(8));
    TEST_TO_STR(unsigned long long(9));
    TEST_TO_STR(float(10));
    TEST_TO_STR(double(11));
    TEST_TO_STR(nullptr);
    TEST_TO_STR((void*)nullptr);

    test_obj* pObj = nullptr;
    TEST_TO_STR(pObj);  // => const void*

    test_obj obj;
    TEST_TO_STR(obj.data);  // const char*
    TEST_TO_STR(obj.data1); // unknown type auto cast to void*

    char str[] = "hello world";
    TEST_TO_STR(str);       // const char*

    char sz1[10] ={'0','1','2','3','4','5','6','7','8','9'};
    TEST_TO_STR(sz1);       // const char*

    const char sz2[10] ={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    TEST_TO_STR(sz2);       // const char*
}

static void test_to_str_t_1()
{
    constexpr size_t BUFF_LEN = 64;
    char buff[BUFF_LEN];

    TEST_TO_STR_T("hello world");
    TEST_TO_STR_T(std::string("hello world"));
    TEST_TO_STR_T(true);
    TEST_TO_STR_T(false);
    TEST_TO_STR_T('C');
    TEST_TO_STR_T(short(1));
    TEST_TO_STR_T(unsigned short(2));
    TEST_TO_STR_T(int(3));
    TEST_TO_STR_T(unsigned int(4));
    TEST_TO_STR_T(unsigned(5));
    TEST_TO_STR_T(long(6));
    TEST_TO_STR_T(unsigned long(7));
    TEST_TO_STR_T(long long(8));
    TEST_TO_STR_T(unsigned long long(9));
    TEST_TO_STR_T(float(10));
    TEST_TO_STR_T(double(11));
    TEST_TO_STR_T(nullptr);
    TEST_TO_STR_T((void*)nullptr);

    test_obj* pObj = nullptr;
    TEST_TO_STR_T(pObj);  // => const void*

    test_obj obj;
    TEST_TO_STR_T(obj.data);  // const char[2]
    TEST_TO_STR_T(obj.data1); // const void*

    char str[] = "hello world";
    TEST_TO_STR_T(str);       // const char[N]

    char sz1[10] ={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    TEST_TO_STR_T(sz1);       // const char[N]

    const char sz2[10] ={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    TEST_TO_STR_T(sz2);       // const char[N]
}

static void test_to_str_t_2()
{
    constexpr size_t BUFF_LEN = 128;
    char buff[BUFF_LEN];

    test_obj2 obj;
    TEST_TO_STR_T(&test_obj2::get);
    TEST_TO_STR_T(&test_obj2::get_const);
    TEST_TO_STR_T(&test_obj2::s_get);
    TEST_TO_STR_T(&test_obj2::a);
    TEST_TO_STR_T(&test_obj2::ca);
    TEST_TO_STR_T(&test_obj2::sa);
    TEST_TO_STR_T(test_obj2::sa);

    TEST_TO_STR_T(obj.a);
    TEST_TO_STR_T(obj.ca);
    TEST_TO_STR_T(obj.sa);

    TEST_TO_STR_T(TestEnum::teType1);
    TEST_TO_STR_T(TestEnum2::te2Type1);

    TEST_TO_STR_T(obj);     // to_string(obj&)
    TEST_TO_STR_T(&obj);    // to_string(obj&)

    std::map<int, std::string> map;
    map[1] = "hello";
    map[2] = "world";
    map[3] = "hello every body, nice to meet you";
    TEST_TO_STR_T(map);
    TEST_TO_STR_T(*map.begin());

    std::list<int> list;
    list.push_back(1);
    list.push_back(2);
    TEST_TO_STR_T(list);

    TEST_TO_STR_T(std::make_pair(1, "utility"));

    std::array<int, 3> ary;
    ary[0] = 1;
    ary[1] = 2;
    ary[2] = 3;
    TEST_TO_STR_T(ary);

    std::vector<std::string> vec;
    vec.push_back("hello");
    vec.push_back("world");
    vec.push_back("c plus plus");
    TEST_TO_STR_T(vec);
    TEST_TO_STR_T(&vec);

    utility::to_str_sep(buff, BUFF_LEN, ", ", vec, 1, 2.0, true, "hello", ary, obj);
    printf("utility::to_str_sep: %s\n", buff);

    char buff2[1024];
    utility::to_str_format(buff2, 1024, "true:{0} hello:{1} ary:{2} obj:{3}", true, "hello", ary, obj);
    printf("utility::to_str_format: %s\n", buff2);
}

void test_to_string()
{
    printf("test_to_string_normal...\n");
    test_to_string_normal();
    printf("test_to_str_t_normal...\n");
    test_to_str_t_1();
    printf("test_to_str_t_complex...\n");
    test_to_str_t_2();
}
