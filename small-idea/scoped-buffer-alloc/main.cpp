#include <stdio.h>
#include <cstdint>
#include <type_traits>
#include <iostream>
#include <functional>
#include <cassert>

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

    bool operator () (int i)
    {
        return true;
    }
};

struct A { int i; };            // C-struct (POD)
class B : public A {};          // still POD (no data members added)
struct C : B { void fn() {} };   // still POD (member function)
struct D : C { D() {} };         // no POD (custom default constructor)
struct E
{
    E(int i) : j(i)
    {
    }
    E(const TestObj& cd)
    {
    }
    int j;
};

template <class Ty, bool isPod>
struct Constructor
{
};

template <class Ty>
struct Constructor<Ty, true>
{
    static void Custruct(void* pBuffer, uint32_t nCount)
    {
    }

    static void Destruct(void* pBuffer, uint32_t nCount)
    {
    }
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

template <class Func>
bool Traverse(Func&& Visitor)
{
    printf("bool Traverse(Func&& Visitor)\n");
    std::function<bool(int)> fn = std::ref(Visitor);
    return Traverse(fn);
}

//template <>
bool Traverse(std::function<bool(int)>& Visitor)
{
    printf("bool Traverse(std::function<bool(int)>& Visitor)\n");
    return true;
}

#include <valarray>
#include <array>
#include "scoped_buffer_allocator.h"

using namespace zh;

int main()
{
    scoped_buffer_allocator<1024, 8> allocator;
    scoped_obj_buffer<int> buffer0 = allocator.allocate<int>(10);
    scoped_obj_buffer<int> buffer1 = allocator.allocate<int>(10, 1);
    std::unique_ptr
        return 0;
}
