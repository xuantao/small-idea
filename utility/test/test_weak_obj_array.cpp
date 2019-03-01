#include "test.h"
#include "utility/weak_obj_array.h"

using namespace utility;

namespace
{
    struct TestDrive : WeakObjIndex
    {
    };

    struct TestMember
    {
        WeakObjIndex weak_obj_index;
    };

    struct TestMember2 : TestMember
    {

    };
}

static void test_1()
{
    WeakObjPtr<TestDrive> ptr1 = nullptr;
    WeakObjPtr<TestMember> ptr2 = nullptr;

    TestDrive t1;
    TestMember t2;
    TestMember2 t3;

    WeakObjPtr<TestDrive> ptr3 = &t1;
    WeakObjPtr<TestMember> ptr4 = &t2;
    WeakObjPtr<TestMember2> ptr5 = &t3;

    ptr4 = ptr5;
    WeakObjPtr<TestMember> ptr6 = ptr5;
    WeakObjPtr<TestMember> ptr7 = &t3;

    bool b1 = (ptr5 == ptr7);
    bool b2 = ptr5 == &t3;
    bool b3 = &t3 == ptr5;
    bool b4 = ptr5 == &t2;
    bool b5 = &t2 == ptr5;

    printf("ptr1=0x%p, 0x%p\n", &t1, ptr3.Get());
    printf("ptr2=0x%p, 0x%p\n", &t2, ptr4.Get());
    printf("test_weak_obj_array\n");
}

void test_weak_obj_array()
{
    WeakObjArray::Startup(2);
    test_1();
    WeakObjArray::GetInstance()->Purge();
}
