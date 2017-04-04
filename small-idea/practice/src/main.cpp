#include <stdio.h>
#include <cstdint>
#include <type_traits>
#include <iostream>
#include <functional>
#include <cassert>

extern void TestStackBufferAllocatoer();
extern void TestRefCount();

int main(int argc, char* argv[])
{
    TestStackBufferAllocatoer();
    TestRefCount();
    return 0;
}
