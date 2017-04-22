#include <stdio.h>
#include <map>
#include <set>
#include <vector>
#include <iterator>
#include <allocators>

extern void scoped_vector_test();
extern void scoped_set_test();

int main(int argc, char* argv[])
{
    scoped_vector_test();
    scoped_set_test();
    return 0;
}
