#include "Tesh.h"
#include "KGSerialAllocator.h"

static void TestAllocImpl()
{
    constexpr size_t POOL_SIZE = 64;
    int8_t Pool[POOL_SIZE];

    void* pBuff = nullptr;
    KGSerialAllocatorImpl<> alloc_1(Pool, POOL_SIZE);
    pBuff = alloc_1.Alloc(10);  //X64: align(10) = 16
    assert(pBuff);
    pBuff = alloc_1.Alloc(24);  //X64: align(24) = 24
    assert(pBuff);
    pBuff = alloc_1.Alloc(20);  //X64: align(20) = 24
    assert(pBuff);
    pBuff = alloc_1.Alloc(4);   //X64: align(4)  = 8
    assert(pBuff == nullptr);

    KGSerialAllocatorImpl<> alloc_2(std::move(alloc_1));
    pBuff = alloc_1.Alloc(4);   // alloc_1 has cleared
    assert(pBuff == nullptr);

    pBuff = alloc_2.Alloc(4);   // alloc_2 is empty
    assert(pBuff == nullptr);

    KGSerialAllocatorImpl<> alloc_3(nullptr, 0);
    pBuff = alloc_3.Alloc(4);   // alloc_3 is empty
    assert(pBuff == nullptr);

    alloc_3 = std::move(alloc_2);
    pBuff = alloc_2.Alloc(4);   // alloc_3 is empty
    assert(pBuff == nullptr);

    alloc_3.Reset();            // reset alloator
    pBuff = alloc_3.Alloc(10);  //X64: align(10) = 16
    assert(pBuff);
    pBuff = alloc_3.Alloc(24);  //X64: align(24) = 24
    assert(pBuff);
    pBuff = alloc_3.Alloc(20);  //X64: align(20) = 24
    assert(pBuff);
    pBuff = alloc_3.Alloc(4);   //X64: align(4)  = 8
    assert(pBuff == nullptr);

    KGSerialAllocatorImpl<> alloc_4(nullptr, 0);
    alloc_3.Reset();
    alloc_4.Swap(alloc_3);

    pBuff = alloc_3.Alloc(4);   // alloc_3 is empty
    assert(pBuff == nullptr);

    pBuff = alloc_4.Alloc(10);  //X64: align(10) = 16
    assert(pBuff);
    pBuff = alloc_4.Alloc(24);  //X64: align(24) = 24
    assert(pBuff);
    pBuff = alloc_4.Alloc(20);  //X64: align(20) = 24
    assert(pBuff);
    pBuff = alloc_4.Alloc(4);   //X64: align(4)  = 8
    assert(pBuff == nullptr);

    printf("KGSerialAllocatorImpl pass...\n");
}

static void TestSerialAlloc()
{
    //TODO:
}

void TestSerialAllocator()
{
    TestAllocImpl();
    TestSerialAlloc();
}
