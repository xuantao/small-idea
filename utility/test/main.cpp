#include <stdio.h>
#include <type_traits>
#include "test.h"
#include "utility/common.h"
#include <map>
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

using namespace utility;

template <typename Fty, typename... Args>
void test_call_object(Fty&& fn, Args&&... args)
{
    CallableObject<Fty, Args...> obj(std::forward<Fty>(fn));
    obj.Call(std::forward<Args>(args)...);
}

int main(int argc, char* argv[])
{
    test_call_object([]() {
        printf("1111111\n");
    });

    test_call_object([](int) {
        printf("1111111\n");
    }, 1);
    //test_serial_allocator();
    //test_stack_allocator();
    //test_scope_guard();
    //test_to_string();
    //test_logger();
    test_step_excutor();
    system("pause");

    _CrtDumpMemoryLeaks();
    return 0;
}
