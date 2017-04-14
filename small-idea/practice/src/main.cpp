#include <stdio.h>
#include <map>
#include <set>
#include <vector>

extern void test_scoped_vector();

template<class Ty>
void test_i()
{
    printf("");
}

int main(int argc, char* argv[])
{
    test_scoped_vector();

    test_i<std::vector<int>::difference_type>();

    return 0;
}
