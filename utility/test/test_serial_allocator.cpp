#include "serial_allocator.h"
#include "test.h"
#include <vector>

UTILITY_USING_NAMESPACE

static void test_fixed_serial_allocator()
{
    //fixed_serial_allocator<1024> allocator;
    //void* buf = nullptr;

    //buf = allocator.allocate(512);
    //assert(buf);
    //buf = allocator.allocate(512);
    //assert(buf);
    //assert(allocator.size() == allocator.capacity());

    //buf = allocator.allocate(512);
    //assert(buf == nullptr);    // fall

    //allocator.reset();

    //buf = allocator.allocate(512);
    //assert(buf);
    //buf = allocator.allocate(500);
    //assert(buf);
    //assert(allocator.size() == align_size(512) + align_size(500));
    //buf = allocator.allocate(12);
    //assert(buf == nullptr);    // not enough (because of size align)

    //printf("fixed_serial_allocator pass...\n");
}

static void test_chain_serial_allocator()
{
    //chain_serial_allocator<512> allocator;
    //void* buf = nullptr;

    //buf = allocator.allocate(256);
    //assert(buf);

    //buf = allocator.allocate(256);
    //assert(buf);
    //assert(allocator.node_size() == 1);

    //buf = allocator.allocate(256);
    //assert(buf);
    //assert(allocator.node_size() == 2); // increament a node

    //allocator.reset();   // reset all
    //assert(allocator.node_size() == 2);

    //allocator.reset(true);  // reset with dissolve
    //assert(allocator.node_size() == 1);

    //buf = allocator.allocate(1024);
    //assert(buf == nullptr); // greater than block size

    //buf = allocator.allocate(256);
    //assert(buf);

    //buf = allocator.allocate(256);
    //assert(buf);
    //assert(allocator.node_size() == 1);

    //buf = allocator.allocate(256);
    //assert(buf);
    //assert(allocator.node_size() == 2); // increament a node
    //printf("chain_serial_allocator pass...\n");
}

void test_serial_allocator()
{
    test_fixed_serial_allocator();
    test_chain_serial_allocator();
}
