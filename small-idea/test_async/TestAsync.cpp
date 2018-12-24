#include "KGAsync.h"
#include "Tesh.h"

namespace
{
    struct Caller
    {
        void operator() ()
        {
            printf("void operator()\n");
        }

        int operator() (int v)
        {
            printf("void operator(int)\n");
            return v;
        }
    };
}

static void TestCall()
{
    printf("void TestCall()\n");
}

static int TestCall_Int(int v)
{
    printf("void TestCall_Int(int)\n");
    return v;
}

void TestAsync()
{
    Caller callObj;
    KGAsync::Startup(2);

    auto cFuture1 = KGAsync::Run(callObj);
    auto cFuture2 = KGAsync::Run(callObj, 1);

    auto cFuture3 = KGAsync::Run(&TestCall);
    //auto cFuture3_1 = KGAsync::Run(&TestCall, 1);
    auto cFuture4 = KGAsync::Run(&TestCall_Int, 1);

    //KGAsync::Run(&callObj);

    while (!cFuture1.IsReady())
    {
    }

    while (!cFuture2.IsReady())
    {
    }

    while (!cFuture3.IsReady())
    {
    }

    while (!cFuture4.IsReady())
    {
    }

    KGAsync::Shutdown();
}
