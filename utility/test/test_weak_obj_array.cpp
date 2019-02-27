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

void test_weak_obj_array()
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

    printf("ptr1=0x%p, 0x%p\n", &t1, ptr3.Get());
    printf("ptr2=0x%p, 0x%p\n", &t2, ptr4.Get());
    printf("test_weak_obj_array\n");
}
