#include <stdio.h>
#include <type_traits>
#include "test.h"

int main(int argc, char* argv[])
{
    test_serial_allocator();
    test_stack_allocator();
    test_to_string();
    system("pause");
    return 0;
}
