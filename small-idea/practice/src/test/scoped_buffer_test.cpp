#include <memory>
#include "../scoped_buffer_allocator.h"
#include "../scoped.h"
#include "test_util.h"

USING_NAMESPACE_ZH;

namespace
{
    typedef scoped_buffer_allocator<56, 8> scoped_allocator;
    scoped_allocator _allocator;
}

static char* getPointer()
{
    return (char*)((void*)&_allocator);
}

static void Test_FullAllocate()
{
    scoped_buffer buff1 = _allocator.allocate(32);
    scoped_buffer buff2 = _allocator.allocate(8);
    // this must be allocate by system pool in debug mode
    scoped_buffer buff3 = _allocator.allocate(1);

    assert((char*)buff1.get() > getPointer() && buff1.get() < getPointer() + sizeof(_allocator));
    assert((char*)buff2.get() > getPointer() && buff2.get() < getPointer() + sizeof(_allocator));
    // system pool
    assert(!((char*)buff3.get() > getPointer() && buff3.get() < getPointer() + sizeof(_allocator)));
}

static void Test_StackBuffer_1()
{
    // test align
    scoped_buffer buff0 = _allocator.allocate(0);
    scoped_buffer buff1 = _allocator.allocate(7);
    scoped_buffer buff2 = _allocator.allocate(9);
}

static void Test_StackBuffer()
{
    scoped_buffer buff1(_allocator.allocate(4));
    Test_StackBuffer_1();
    scoped_buffer buff2 = _allocator.allocate(14);
}

static void Test_Overwrite()
{
    scoped_buffer buff1(_allocator.allocate(4));
    //memset(buff1.get(), 0, buff1.size() + 2);
}

void scoped_buffer_test()
{
    Test_FullAllocate();
    Test_StackBuffer();
    Test_Overwrite();
}
