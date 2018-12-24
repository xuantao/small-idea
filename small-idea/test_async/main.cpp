#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <memory>
#include <type_traits>
#include <iostream>
#include <future>
//#include <experimental/future>
#include "KGStepExcutor.h"
#include "Tesh.h"
#include "serial_allocator.h"

using namespace utility;

template <size_t A = sizeof(void*)>
struct xxx_allocator
{
    typedef singly_node<serial_allocator<A>> node_type;

    xxx_allocator(size_t inc)
        : _increment(inc)
        ,_alloc_node(0, 0)
    {
    }

    xxx_allocator(size_t inc, void* defPool, size_t defSize)
        : _increment(inc)
        , _alloc_node(defPool, defSize)
    {
    }

    ~xxx_allocator()
    {
        node_type* pRoot = _alloc_node.next;
        while (pRoot)
        {
            node_type* pCur = pRoot;
            pRoot = pCur->next;

            pCur->~node_type();
            delete reinterpret_cast<int8_t*>(pCur);
        }
    }

public:
    void* allocate(size_t sz)
    {
        void* pBuff = _alloc_node.value.allocate(sz);
        if (pBuff)
            return pBuff;

        create_node(std::max(align_size(sz), _increment));
        pBuff = _alloc_node.value.allocate(sz);
        assert(pBuff);
        return pBuff;
    }

    void deallocate(void* p, size_t) { }

private:
    void create_node(size_t s)
    {
        int8_t* p = new int8_t[align_size(sizeof(node_type)) + s];
        assert(p);
        node_type* pNode = new (p) node_type(p + align_size(sizeof(node_type)), s);
        _alloc_node.value.swap(pNode->value);
        pNode->next = _alloc_node.next;
        _alloc_node.next = pNode;
    }

    size_t _increment;
    node_type _alloc_node;
};

template <size_t DefSize, size_t A = sizeof(void*)>
struct special_allocator : xxx_allocator<A>
{
    special_allocator(size_t extra_size) : xxx_allocator<A>(extra_size, _pool, DefSize)
    {
    }

private:
    alignas (A) int8_t _pool[DefSize];
};

//void test_ref(int)
//{
//    printf("22222222");
//}

void TestScopeGuard()
{
    KGScopeGuard guard;
    guard.Push([] { printf("11111111\n"); });
}

template <typename Ty>
void test_callable(Ty&& fn)
{
    static_assert(utility::is_callable<Ty, int>::value, "222");
}

void test_ref(int&)
{
    printf("22222222");
}

void test_spe_allocator()
{
    int i = 0;
    test_ref(i);
    //test_callable(&test_ref);
    //test_ref(1);

    special_allocator<16> alloc(64);

    alloc.allocate(6);
    alloc.allocate(10);
    alloc.allocate(1024);
    alloc.allocate(10);

}

void TestStep()
{
    //rebind_alloc
    MakeStepExcutor([] {
        printf("111111111\n");
    });

    MakeStepExcutor([] {
        printf("111111111\n");
        return true;
    });

    MakeStepExcutor([] {
        printf("111111111\n");
        return KGSTEP_RET::Completed;
    });

    //MakeStepExcutor([](KGStepGuard) {
    //    printf("111111111\n");
    //});

    //MakeStepExcutor([](KGStepGuard&&) {
    //    printf("111111111\n");
    //    return true;
    //});

    //MakeStepExcutor([](const KGStepGuard&) {
    //    printf("111111111\n");
    //    return true;
    //});

    MakeStepExcutor([](KGStepGuard&) {
        printf("111111111\n");
        return KGSTEP_RET::Completed;
    });
}


struct Allocator;
extern void TestAsync();

struct CallObj : std::enable_shared_from_this<CallObj>
{
    void operator () () {
        printf("CallObj()\n");
    }

    void operator () (int) {
        printf("CallObj()\n");
    }
};

struct Alloc : std::allocator<int>
{

};

int main(int argc, char* argv[])
{
    std::function<void()> fn;
    CallObj obj;

    //std::allocate_shared()
    std::allocator<void> a1;
    std::allocator<int> a2(a1);

    std::allocator<void> a3 = a1;

    TestStep();

    //TestCallable(&obj);

    //KGAsync::Run(obj);
    //TestAsync();

    //std::allocate_shared()

    //test_spe_allocator();

    system("pause");
    return 0;
}
