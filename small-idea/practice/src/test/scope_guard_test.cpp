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
}

static void test_scope_guard_normal()
{
    int a = 0;
    scope_guard<1> guard;

    MyOp op;
    guard.append(op);
    guard.append(static_cast<const MyOp&>(op));
    guard.append(std::ref(op));

    guard.append(MyOp());

    guard.append([&] {printf("a=%d\n", a); });
    guard.append([=] {printf("a=%d\n", a); });
}


void test_scope_guard()
{
    test_scope_guard_normal();
}