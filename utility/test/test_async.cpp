#include "test_util.h"
#include "utility/async.h"
#include <thread>

using namespace utility;

void test_async_expected()
{
    Async::Run([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        printf("async Run 1\n");
    });

    {
        Async::ExpectRun([] {
            printf("this ExpectedRun will not be printed\n");
            assert(false);
        });
    }

    auto f = Async::ExpectRun([] {
        printf("this ExpectedRun must be printed\n");
    });

    Async::Run([] {
        printf("async Run 2\n");
    });

    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void test_async()
{
    Async::Startup(1);

    test_async_expected();

    Async::Shutdown();
}
