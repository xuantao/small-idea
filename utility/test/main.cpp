#include <stdio.h>
#include <type_traits>
#include "test.h"
#include "utility/common.h"
#include <map>
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

int main(int argc, char* argv[])
{
    //test_serial_allocator();
    //test_stack_allocator();
    //test_scope_guard();
    //test_to_string();
    //test_logger();
    //test_step_excutor();
    //test_async();
    //test_com_obj_ptr();
    test_shared_obj_array();
    test_weak_obj_array();
    system("pause");

    _CrtDumpMemoryLeaks();
    return 0;
}
