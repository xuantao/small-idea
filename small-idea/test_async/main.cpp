#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <memory>
#include <type_traits>
#include <iostream>
#include <future>
#include "StepExcutor.h"
#include "Tesh.h"

using namespace utility;


//void test_ref(int)
//{
//    printf("22222222");
//}

void TestScopeGuard()
{
    ScopeGuard guard;
    guard.Push([] { printf("11111111\n"); });
}

template <typename Ty>
void test_callable(Ty&& fn)
{
    static_assert(utility::is_callable<Ty, int>::value, "222");
}

void test_ref(int&)
{
    printf("22222222");
}

void test_spe_allocator()
{
    int i = 0;
    test_ref(i);
    //test_callable(&test_ref);
    //test_ref(1);
}

void TestStep()
{
    //rebind_alloc
    MakeStepExcutor([] {
        printf("111111111\n");
    });

    MakeStepExcutor([] {
        printf("111111111\n");
        return true;
    });

    MakeStepExcutor([] {
        printf("111111111\n");
        return STEP_STATUS::Completed;
    });

    MakeStepExcutor([](StepGuard&) {
        printf("111111111\n");
    });

    MakeStepExcutor([](StepGuard&) {
        printf("111111111\n");
        return true;
    });

    MakeStepExcutor([](StepGuard&) {
        printf("111111111\n");
        return true;
    });

    MakeStepExcutor([](StepGuard&) {
        printf("111111111\n");
        return STEP_STATUS::Completed;
    });

    QueueStepExcutor steper;
    steper.Add([](StepGuard& guard) {
        printf("222222222222\n");
        guard.Push([] {
            printf("33333333333\n");
        });
    });

    steper.Add([](StepGuard& guard) {
        printf("444444444444\n");
        guard.Push([] {
            printf("5555555555\n");
        });
    });

    steper.Step();
    steper.Add([] {
        printf("11111111111");
        return true;
    });

    steper.Add([] {
        printf("11111111111");
        return STEP_STATUS::Completed;
    });

    steper.Add([] (StepGuard&) {
        printf("11111111111");
        return STEP_STATUS::Completed;
    });

    Future<int> future;
    steper.Add(std::move(future), [] {
        printf("11111111111");
        return STEP_STATUS::Completed;
    });

    steper.Add(std::move(future), [] (StepGuard& guard) {
        printf("11111111111");
        return STEP_STATUS::Completed;
    });

    steper.Add(future.Share(), [] {
        printf("11111111111");
        return STEP_STATUS::Completed;
    });

    steper.Add(future.Share(), [](StepGuard& guard) {
        printf("11111111111");
        return STEP_STATUS::Completed;
    });
}


struct Allocator;
extern void TestAsync();

struct CallObj : std::enable_shared_from_this<CallObj>
{
    void operator () () {
        printf("CallObj()\n");
    }

    void operator () (int) {
        printf("CallObj()\n");
    }
};

struct Alloc : std::allocator<int>
{

};

template <typename Ty>
auto test_enable_if(bool) -> typename std::enable_if<std::is_same<Ty, int>::value>::type
{
    printf("auto test_enable_if(bool) -> typename std::enable_if<std::is_same<Ty, int>::value>::type\n");
}

template <typename Ty>
auto test_enable_if(bool) -> typename std::enable_if<!std::is_same<Ty, int>::value>::type
{
    printf("auto test_enable_if(bool) -> typename std::enable_if<!std::is_same<Ty, int>::value>::type\n");
}

struct A
{
    A() { printf("A()\n"); }
    ~A() { printf("~A()\n"); }
};

struct B
{
    B() { printf("B()\n"); }
    ~B() { printf("~B()\n"); }
};

struct C
{
    C() { printf("C()\n"); }
    ~C() { printf("~C()\n"); }
};

struct D
{
    A a;
    B b;
    C c;
};

struct E
{
    E() : c(), b(), a() {}

    //E(int) : b(), c(), a() {}

    //E(bool) : b(), a(), c() {}

    A a;
    B b;
    C c;
};

void test_order()
{
    {
        D d;
    }
    {
        E e;
    }
    //{
    //    E e(1);
    //}
    //{
    //    E e(true);
    //}
}

int main(int argc, char* argv[])
{

    test_order();
    //KGPoolSerialAlloc<128> alloc;
    ////KGSerialAllocator<>* pAlloc = &alloc;


    //test_enable_if<void>(true);
    //test_enable_if<int>(false);
    //std::function<void()> fn;
    //CallObj obj;

    ////std::allocate_shared()
    //std::allocator<void> a1;
    //std::allocator<int> a2(a1);
    //std::allocator<void> a3 = a1;

    //TestStep();
    TestSerialAllocator();

    //TestCallable(&obj);

    //KGAsync::Run(obj);
    //TestAsync();

    //std::allocate_shared()

    //test_spe_allocator();

    system("pause");
    return 0;
}
