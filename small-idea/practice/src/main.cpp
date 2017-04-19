#include <stdio.h>
#include <map>
#include <set>
#include <vector>
#include <iterator>
#include <allocators>

extern void test_scoped_vector();

template<class Ty>
void test_i()
{
    printf("");
}

int main(int argc, char* argv[])
{
    //test_scoped_vector();

    //test_i<std::vector<int>::difference_type>();
    int a = 1;
    int b = a << 8;

    return 0;
}
