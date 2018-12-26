#include "Async.h"
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
    Async::Startup(2);

    auto cFuture1 = Async::Run(callObj);
    auto cFuture2 = Async::Run(callObj, 1);

    auto cFuture3 = Async::Run(&TestCall);
    //auto cFuture3_1 = KGAsync::Run(&TestCall, 1);
    auto cFuture4 = Async::Run(&TestCall_Int, 1);

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

    Async::Shutdown();
}
