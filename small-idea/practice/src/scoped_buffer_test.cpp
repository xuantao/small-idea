#include <memory>
#include "scoped_buffer_allocator.h"

using namespace zh;

typedef scoped_buffer_allocator<1024, 8> scoped_allocator;

static scoped_allocator& GetAllocator()
{
    // 1M size, 8 bytes alignment
    static scoped_allocator s_Allocator;
    return s_Allocator;
}

static void Test_FullAllocate()
{
    scoped_buffer buff1 = GetAllocator().allocate(512);
    scoped_buffer buff2 = GetAllocator().allocate(256);
    // this must be allocate by system pool in debug mode
    scoped_buffer buff3 = GetAllocator().allocate(256);
    // 
    scoped_buffer buff4 = GetAllocator().allocate(128);

    memset(buff1.get(), 0, buff1.size());
    memset(buff2.get(), 1, buff2.size());
    memset(buff3.get(), 2, buff3.size());
    memset(buff4.get(), 4, buff4.size());
}

static void Test_StackBuffer_1()
{
    // test align
    scoped_buffer buff0 = GetAllocator().allocate(0);
    scoped_buffer buff1 = GetAllocator().allocate(7);
    scoped_buffer buff2 = GetAllocator().allocate(9);
}

static void Test_StackBuffer()
{
    scoped_buffer buff1(GetAllocator().allocate(4));
    Test_StackBuffer_1();
    scoped_buffer buff2 = GetAllocator().allocate(14);
}

static void Test_Overwrite()
{
    scoped_buffer buff1(GetAllocator().allocate(4));
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
    scoped_obj_buffer<TestObj> pObjs1 = GetAllocator().allocate<TestObj>(10, 1);
    auto pObjs2 = GetAllocator().allocate<TestObj>(10, Param());
    auto pObjs3 = GetAllocator().allocate<int>(100, 0);

    int* pInt = pObjs3.get();

    const TestObj& p1 = ((const scoped_obj_buffer<TestObj>*)&pObjs1)->operator[] (9);
    TestObj& p2 = pObjs2[3];

    printf("Test_StackObj completed\n");

    // scoped_bufferObj<TestObj> Obj3 = pObjs1;
}

void TestStackBufferAllocatoer()
{
    Test_FullAllocate();
    Test_StackBuffer();
    Test_StackObj();

    //Test_Overwrite();
}
