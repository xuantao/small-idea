#include <stdio.h>
#include <cstdint>
#include <type_traits>
#include <iostream>
#include <functional>
#include <cassert>

//extern void TestStackBufferAllocatoer();

struct Test
{
    //template <>
    static bool Traverse(const std::function<bool(int)>& Visitor)
    {
        printf("bool Traverse(const std::function<bool(int)>& Visitor)\n");
        return true;
    }

    //static bool Traverse(std::function<bool(int)>& Visitor)
    //{
    //    printf("bool Traverse(std::function<bool(int)>& Visitor)\n");
    //    return Traverse((const std::function<bool(int)>&)Visitor);
    //}

    template <class Func>
    static bool Traverse(Func&& Visitor)
    {
        printf("bool Traverse(Func&& Visitor)\n");
        const std::function<bool(int)> fn = std::ref(Visitor);
        //return Traverse(std::function<bool(int)>(std::ref(Visitor)));
        return Traverse(fn);
    }
};

#include "fixed_allocator.h"
#include <vector>
#include <set>

void test_in(Test&& t)
{

}

void test_out(Test&& t)
{
    test_in(std::forward<Test>(t));
}

custom_allocator_adapter<int> get_allocator()
{
    return custom_allocator_adapter<int>(nullptr, 0);
}

int main(int argc, char* argv[])
{
    const size_t size = 256;
    char buff[size] = {0};

    fixed_allocator<int> allocator(buff, size);
    fixed_allocator<int> a2(buff, size);
    std::vector<char*> vec;

    //a2 = allocator;

    for (int i = 0; i < 10; ++i)
    {
        char* p = (char*)allocator.allocate();
        vec.push_back(p);

        printf("i:%2d %lld\n", i, p - buff);
    }

    allocator.deallocate((int*)vec[7]);

    allocator.deallocate((int*)vec[2]);

    printf("i:%2d %lld\n", 99, (char*)allocator.allocate() - buff);
    printf("i:%2d %lld\n", 99, (char*)allocator.allocate() - buff);
    printf("i:%2d %lld\n", 99, (char*)allocator.allocate() - buff);

    printf("max_size:%lld\n", allocator.max_size());

    //custom_allocator_adapter<int> c_a = custom_allocator_adapter<int>(buff, size);
    custom_allocator_adapter<int> c_a = get_allocator();
    //std::set<int, std::less<int>, custom_allocator_adapter<int> > ss(c_a);
    c_a.allocate(1);
    test_out(Test());

    return 0;
}
