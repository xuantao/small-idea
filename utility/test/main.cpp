#include <stdio.h>
#include <type_traits>
#include "test.h"
#include <map>

int main(int argc, char* argv[])
{
    test_serial_allocator();
    test_stack_allocator();
    test_scope_guard();
    test_to_string();
    test_logger();
    system("pause");
    return 0;
}
