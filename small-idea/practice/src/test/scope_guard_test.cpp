#include "../scope_guard.h"
#include <memory>
#include <functional>

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
    //guard.append(std::ref(op1));

    guard.append(op);
    guard.append(static_cast<const MyOp&>(op));
    guard.append(std::ref(op));

    guard.append(MyOp());

    guard.append([&] {printf("a=%d\n", a); });
    guard.append([=] {printf("a=%d\n", a); });

    guard.append(test_scope_func_1);
    guard.append(std::bind(test_scope_func_2, 1));
}


void test_scope_guard()
{
    test_scope_guard_normal();
}