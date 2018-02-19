#include "../scope_guard.h"
#include "test.h"
#include <memory>
#include <functional>

USING_NAMESPACE;

namespace {
    struct MyOp
    {
        MyOp() { printf("construction\n"); }
        MyOp(const MyOp&) { printf("construction by copy\n"); }
        MyOp(MyOp&&) { printf("construction by right value\n"); }
        MyOp& operator = (const MyOp&&) { printf("copy operator = \n"); return *this; }

        void operator ()() { printf("void operator ()\n"); }
    };

    void test_scope_func_1()
    {
        printf("test_scope_func_1\n");
    }

    void test_scope_func_2(int a)
    {
        printf("test_scope_func_1 a=%d\n", a);
    }
}

static void test_scope_guard_normal()
{
    int a = 0;
    bool b = 1;
    MyOp op;
    scope_guard guard;

    //MyOp op1;                         // not allow, the op1 lifetime is not allow
    //guard.push(std::ref(op1));

    guard.push(op);
    guard.push(static_cast<const MyOp&>(op));
    guard.push(std::ref(op));

    guard.push(MyOp());

    guard.push([&] {printf("a=%d\n", a); });
    guard.push([=] {printf("a=%d\n", a); });

    guard.push(test_scope_func_1);
    guard.push(std::bind(test_scope_func_2, 1));
}


static void test_scope_guard_raw_alloc()
{
    typedef scope_guard_detail::scope_guard<0> scope_guard;
    int a = 0;
    bool b = true;
    scope_guard guarder;

    guarder.push([&] {printf("a=%2d b=%s\n", a, b ? "true" : "false"); });
    guarder.push([=] {printf("a=%2d b=%s\n", a, b ? "true" : "false"); });
    guarder.push([&a, b] {printf("a=%2d b=%s\n", a, b ? "true" : "false"); });
    guarder.push([a, &b] {printf("a=%2d b=%s\n", a, b ? "true" : "false"); });
}

void scope_guard_test()
{
    test_scope_guard_raw_alloc();
    test_scope_guard_normal();
}
