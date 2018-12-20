#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <memory>
#include <type_traits>
#include <iostream>
#include "KGStepExcutor.h"
#include "KGAsyncTask.h"

void TestStep()
{
    KGQueueStepExcutor list;

    list.Add(MakeStepExcutor([] {
        printf("return void\n");
    }));

    list.Add(MakeStepExcutor([] {
        printf("return true\n");
        return true;
    }));

    list.Add(MakeStepExcutor([] {
        printf("return false\n");
        return false;
    }));

    int idx = 0;
    list.Add(MakeStepExcutor([=]() mutable {
        ++idx;
        printf("return idx=%d\n", idx);
        return idx < 10 ? KGSTEP_RET::Continue : KGSTEP_RET::Completed;
    }));

    list.Add([](auto& rGuarder) {
        printf("11111111111111\n");
        //rGuarder.Push([] { printf("22222222222\n"); });
    });

    while (list.Step() == KGSTEP_RET::Continue)
    {
    }
}

//template <typename Fty>
//auto TestAsync(KGAsyncTaskPool& pool, Fty&& call) -> KGFuture<typename std::result_of<Fty()>::type>
//{
//    auto pTask = new KGAsyncTask<typename std::result_of<Fty()>::type, Fty>(std::forward<Fty>(call));
//    auto future = pTask->GetFuture();
//    pool.AddTask(pTask);
//    return future;
//}

//int TestAsyncFunc()
//{
//    std::this_thread::sleep_for(std::chrono::milliseconds(4));
//
//    std::cout << "11111111111   " << std::this_thread::get_id() << "\n";
//    return 1;
//}

void TestAsyncPool()
{
    //KGAsyncTaskPool pool;
    //pool.Create(4);

    ////for (int i = 0; i < 100; ++i)
    //auto cFuture = TestAsync(pool, TestAsyncFunc);

    ////auto cFuture2 = TestAsync(pool, [](auto& rGuarder) {

    ////});

    ////pool.Destory();
    //std::this_thread::sleep_for(std::chrono::seconds(5));

    //[cFuture] {
    //    if (cFuture.IsReady())
    //        printf("%d\n", cFuture.GetResult());
    //}();
}

//#include <type_traits>
//#include <stdio.h>
//
//struct KGScopeGuard
//{};

template <typename Ty>
struct HasDeclare
{
    static Ty GetFn();
    static KGScopeGuard& GetGuard();
    template<typename U> static auto Check(int) -> decltype(GetFn()(GetGuard()), std::true_type());
    template<typename U> static std::false_type Check(...);

    static constexpr bool value =
        std::is_same<decltype(Check<Ty>(0)), std::true_type>::value;
};

struct testObj
{
    void operator() (KGScopeGuard&) {}
};

void testGuard(KGScopeGuard& guard) {
}

template <typename Fty>
void checkType(Fty&& fty)
{
    bool value = HasDeclare<testObj>::value;
    printf("value:%s\n", value ? "true" : "false");
}

int main(int argc, char* argv[])
{
    //checkType(testObj());
    //checkType(testGuard);
    //checkType([](KGScopeGuard&) {});
    TestAsyncPool();

    system("pause");
    return 0;
}
