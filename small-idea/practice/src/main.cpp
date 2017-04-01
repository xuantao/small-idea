#include <stdio.h>
#include <cstdint>
#include <type_traits>
#include <iostream>
#include <functional>
#include <cassert>

extern void TestStackBufferAllocatoer();

int main(int argc, char* argv[])
{
    TestStackBufferAllocatoer();
    return 0;
}
