#include <memory>
#include "KGStackBuffer.h"
#include "KGStackBufferAllocator.h"

static KGStackBufferAllocator& GetAllocator()
{
    // 1M size, 8 bytes alignment
    static KGStackBufferAllocator s_Allocator(1024, 8);
    return s_Allocator;
}

static void Test_FullAllocate()
{
    KGStackBuffer buff1 = GetAllocator().Allocate(512);
    KGStackBuffer buff2 = GetAllocator().Allocate(256);
    // this must be allocate by system pool in debug mode
    KGStackBuffer buff3 = GetAllocator().Allocate(256);
    // 
    KGStackBuffer buff4 = GetAllocator().Allocate(128);

    memset(buff1.GetBuffer(), 0, buff1.GetSize());
    memset(buff2.GetBuffer(), 1, buff2.GetSize());
    memset(buff3.GetBuffer(), 2, buff3.GetSize());
    memset(buff4.GetBuffer(), 4, buff4.GetSize());
}

static void Test_StackBuffer_1()
{
    // test align
    KGStackBuffer buff0 = GetAllocator().Allocate(0);
    KGStackBuffer buff1 = GetAllocator().Allocate(7);
    KGStackBuffer buff2 = GetAllocator().Allocate(9);
}

static void Test_StackBuffer()
{
    KGStackBuffer buff1(GetAllocator().Allocate(4));
    Test_StackBuffer_1();
    KGStackBuffer buff2 = GetAllocator().Allocate(14);
}

static void Test_Overwrite()
{
    KGStackBuffer buff1(GetAllocator().Allocate(4));
    memset(buff1.GetBuffer(), 0, buff1.GetSize() + 2);
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
    auto pObjs1 = GetAllocator().Allocate<TestObj>(10, 1);
    auto pObjs2 = GetAllocator().Allocate<TestObj>(10, Param());
    auto pObjs3 = GetAllocator().Allocate<int>(100, 0);

    int* pInt = pObjs3.GetBuffer();

    printf("Test_StackObj completed\n");

   // KGStackBufferObj<TestObj> Obj3 = pObjs1;
}

void TestStackBufferAllocatoer()
{
    Test_FullAllocate();
    Test_StackBuffer();
    Test_StackObj();

    //Test_Overwrite();
}
