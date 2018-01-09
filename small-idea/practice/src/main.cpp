#include <stdio.h>
#include <map>
#include <set>
#include <vector>
#include <iterator>
#include <allocators>

extern void scoped_buffer_test();
extern void scoped_vector_test();
extern void scoped_set_test();
extern void scoped_map_test();
extern void test_scope_guard();


int main(int argc, char* argv[])
{
    //scoped_buffer_test();
    //scoped_vector_test();
    //scoped_set_test();
    //scoped_map_test();
    test_scope_guard();
    system("pause");
    return 0;
}
