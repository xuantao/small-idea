#include <memory>
#include "scoped_buffer_allocator.h"
#include "util.h"

USING_NAMESPACE_ZH;

static void Test_FullAllocate()
{
    scoped_buffer buff1 = util::allocate(512);
    scoped_buffer buff2 = util::allocate(256);
    // this must be allocate by system pool in debug mode
    scoped_buffer buff3 = util::allocate(256);
    // 
    scoped_buffer buff4 = util::allocate(128);

    memset(buff1.get(), 0, buff1.size());
    memset(buff2.get(), 1, buff2.size());
    memset(buff3.get(), 2, buff3.size());
    memset(buff4.get(), 4, buff4.size());
}

static void Test_StackBuffer_1()
{
    // test align
    scoped_buffer buff0 = util::allocate(0);
    scoped_buffer buff1 = util::allocate(7);
    scoped_buffer buff2 = util::allocate(9);
}

static void Test_StackBuffer()
{
    scoped_buffer buff1(util::allocate(4));
    Test_StackBuffer_1();
    scoped_buffer buff2 = util::allocate(14);
}

static void Test_Overwrite()
{
    scoped_buffer buff1(util::allocate(4));
    memset(buff1.get(), 0, buff1.size() + 2);
}

namespace
{
    struct Param
    {
        Param()
        {
            printf("Param Construction\n");
        }

        Param(const Param& cd)
        {
            printf("Param Copy Construct\n");
        }

        ~Param()
        {
            printf("Param Destruction\n");
        }

        Param& operator = (const Param& cd)
        {
            printf("Param Operator = \n");
        }
    };

    struct TestObj
    {
        TestObj() : i(++idx)
        {
            printf("idx:%2d TestObj Construction\n", i);
        }

        TestObj(int p) : i(++idx)
        {
            printf("idx:%2d TestObj Construction\n", i);
        }

        TestObj(Param& param) : i(++idx)
        {
            printf("idx:%2d TestObj Construction\n", i);
        }

        TestObj(const TestObj& cd) : i(++idx)
        {
            printf("idx:%2d TestObj Copy Construct\n", i);
        }

        ~TestObj()
        {
            printf("idx:%2d TestObj Destruction\n", i);
        }

        TestObj& operator = (const TestObj& cd)
        {
            printf("idx:%2d TestObj Operator = \n", i);
        }

        int     i;
        char    c;
        float   f;

        static int idx;
    };

    int TestObj::idx = 0;
}

void Test_StackObj()
{
    scoped_obj_buffer<TestObj> pObjs1 = util::allocate<TestObj>(10, 1);
    auto pObjs2 = util::allocate<TestObj>(10, Param());
    auto pObjs3 = util::allocate<int>(100, 0);

    int* pInt = pObjs3.get();

    const TestObj& p1 = ((const scoped_obj_buffer<TestObj>*)&pObjs1)->operator[] (9);
    TestObj& p2 = pObjs2[3];

    printf("Test_StackObj completed\n");

    // not allow copy
    //pObjs2 = pObjs1;
    //scoped_obj_buffer<TestObj> pboj4 = pObjs1;
}

void TestStackBufferAllocatoer()
{
    Test_FullAllocate();
    Test_StackBuffer();
    Test_StackObj();

    //Test_Overwrite();
}
