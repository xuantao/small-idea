#include "utility/stack_allocator.h"
#include "test.h"
#include <vector>

UTILITY_USING_NAMESPACE

static void test_fixed_stack_allocator()
{
    fixed_stack_allocator<128> allocator;
    assert(allocator.capacity() == 128);
    {
        auto s_1 = allocator.allocate(64);
        assert(s_1);
        auto s_2 = allocator.allocate(64);
        assert(s_2);
        auto s_3 = allocator.allocate(64);
        assert(!s_3);   // not enough
    }

    auto s = allocator.allocate(64);
    assert(s);
    {
        {
            auto s_1 = allocator.allocate(64);
            assert(s_1);
        }

        auto s_2 = allocator.allocate(60);
        assert(s_2);
        assert(allocator.size() == allocator.capacity());   // align size, 60->64

        auto s_3 = allocator.allocate(1);
        assert(!s_3);
    }

    printf("fixed_stack_alloator pass...\n");
}

static void test_chain_stack_allocator()
{
    {
        chain_stack_allocator<128> allocator;
        {
            auto s1 = allocator.allocate(64);
            assert(s1);
            auto s2 = allocator.allocate(64);
            assert(s2);
            auto s3 = allocator.allocate(64);
            assert(s3);
        }

        {
            auto s1 = allocator.allocate(128);
            assert(s1);
            auto s2 = allocator.allocate(128);
            assert(s2);
            auto s3 = allocator.allocate(128);
            assert(s3);
        }
    }

    {
        chain_stack_allocator<128> allocator;
        {
            auto s1 = allocator.allocate(64);
            assert(s1);
            auto s2 = allocator.allocate(64);
            assert(s2);
            auto s3 = allocator.allocate(64);
            assert(s3);
        }

        auto sa1 = allocator.allocate(12);
        assert(sa1);
    }

    printf("chain_stack_allocator pass...\n");
}

void test_stack_allocator()
{
    test_fixed_stack_allocator();
    test_chain_stack_allocator();
}
