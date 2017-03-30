#include <stdio.h>
#include <cstdint>
#include <type_traits>
#include <iostream>
#include <functional>

extern void TestStackBufferAllocatoer();


struct TestObj
{
    TestObj()
    {
        printf("Construction\n");
    }

    TestObj(const TestObj& cd)
    {
        printf("Copy Construct\n");
    }

    ~TestObj()
    {
        printf("Destruction\n");
    }

    TestObj& operator = (const TestObj& cd)
    {
        printf("Operator = \n");
    }
};

struct A { int i; };            // C-struct (POD)
class B : public A {};          // still POD (no data members added)
struct C : B { void fn() {} };   // still POD (member function)
struct D : C { D() {} };         // no POD (custom default constructor)
struct E
{
    E(int i) : j(i)
    {}
    E(const TestObj& cd)
    {}
    int j;
};

template <class Ty, bool isPod>
struct Constructor
{};

template <class Ty>
struct Constructor<Ty, true>
{
    static void Custruct(void* pBuffer, uint32_t nCount)
    {}

    static void Destruct(void* pBuffer, uint32_t nCount)
    {}
};

template <class Ty>
struct Constructor<Ty, false>
{
    static void Custruct(void* pBuffer, uint32_t nCount)
    {
        Ty* pObj = (Ty*)pBuffer;
        for (uint32_t i = 0; i < nCount; ++i)
            new (&pObj[i]) Ty();
    }

    template <typename ...Args>
    static void Custruct(void* pBuffer, uint32_t nCount, Args&& ...args)
    {
        Ty* pObj = (Ty*)pBuffer;
        for (uint32_t i = 0; i < nCount; ++i)
            new (&pObj[i]) Ty(args...);
    }

    static void Destruct(void* pBuffer, uint32_t nCount)
    {
        Ty* pObj = (Ty*)pBuffer;
        for (uint32_t i = 0; i < nCount; ++i)
            (&pObj[i])->~Ty();
    }
};

template <class Ty>
void alloc(int n)
{
    void* p = new int8_t[sizeof(Ty) * 2];
    Constructor<Ty, std::is_pod<Ty>::value>::Custruct(p, n);
    Constructor < Ty, std::is_pod<Ty>::value>::Destruct(p, n);
    delete p;
}

template <class Ty, typename ...Args>
void alloc(int n, Args&& ...args)
{
    void* p = new int8_t[sizeof(Ty) * 2];
    Constructor<Ty, false>::Custruct(p, n, args...);
    Constructor < Ty, false>::Destruct(p, n);
    delete p;
}

int main() {

    TestStackBufferAllocatoer();

    return 0;
}
