#include <memory>
#include "../stack_allocator.h"
#include "../scoped.h"
#include "test_util.h"
#include "test.h"

UTILITY_USING_NAMESPACE;

namespace
{
    typedef fixed_stack_allocator<56, 8> scoped_allocator;
    scoped_allocator _allocator;
}

static char* getPointer()
{
    return (char*)((void*)&_allocator);
}

static void test_fullallocate()
{
    scoped_buffer buff1 = _allocator.allocate(32);
    scoped_buffer buff2 = _allocator.allocate(8);
    // this must be allocate by system pool in debug mode
    scoped_buffer buff3 = _allocator.allocate(1);

    assert((char*)buff1.get() > getPointer() && buff1.get() < getPointer() + sizeof(_allocator));
    assert((char*)buff2.get() > getPointer() && buff2.get() < getPointer() + sizeof(_allocator));
    // system pool
    assert(buff3.empty());
}

static void Test_StackBuffer_1()
{
    // test align
    scoped_buffer buff0 = _allocator.allocate(0);
    scoped_buffer buff1 = _allocator.allocate(7);
    scoped_buffer buff2 = _allocator.allocate(9);
}

static void test_stackbuffer()
{
    scoped_buffer buff1(_allocator.allocate(4));
    Test_StackBuffer_1();
    scoped_buffer buff2 = _allocator.allocate(14);
}

static void test_overwrite()
{
    scoped_buffer buff1(_allocator.allocate(4));
    //memset(buff1.get(), 0, buff1.size() + 2);
}

static void test_order()
{
    /*
    scoped_buffer* pBuff1 = new scoped_buffer(_allocator.allocate(10));
    scoped_buffer buff2 = _allocator.allocate(10);
    delete pBuff1;  // assert(false), deallocate order is wrong
    */
}

void scoped_buffer_test()
{
    test_fullallocate();
    test_stackbuffer();
    test_overwrite();
    test_order();
}
