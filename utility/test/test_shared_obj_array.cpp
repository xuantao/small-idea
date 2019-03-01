#include "test.h"
#include "utility/shared_obj_array.h"
#include "utility/scope_guard.h"

namespace
{
    struct TestObj
    {
        int index_;
        static int s_index_;
    };

    struct TestObjConstructor
    {
        void Construct(TestObj* pObj) { pObj->index_ = ++TestObj::s_index_; printf("Construct index:%d\n", pObj->index_); }
        void Construct(TestObj* pObj, int) { pObj->index_ = ++TestObj::s_index_; printf("Construct index:%d\n", pObj->index_); }
        void Destruct(TestObj* pObj) { printf("Destruct  index:%d\n", pObj->index_); }
    };

    int TestObj::s_index_ = 0;

    struct TestObjEx : TestObj
    {
        TestObjEx() { }
        TestObjEx(int) { }
        TestObjEx(const TestObjEx& other)
        {
            index_ = other.index_;
        }
    };
}

static void test_trivial()
{
    using ObjArray = utility::SharedObjArray<TestObj>;
    using SharedObj = typename ObjArray::obj_type;
    ObjArray::Startup(16);
    utility::ScopeGuard<> guard;
    guard.Push([] { ObjArray::GetInstance()->Purge(); });

    auto o1 = ObjArray::GetInstance()->AllocObj();
    o1 = ObjArray::GetInstance()->AllocObj();

    for (int i = 0; i < 16;++ i)
        o1 = ObjArray::GetInstance()->AllocObj();
}

static void test_none_trivial()
{
    TestObjEx::s_index_ = 0;

    using ObjArray = utility::SharedObjArray<TestObjEx>;
    using SharedObj = typename ObjArray::obj_type;
    ObjArray::Startup(16);
    utility::ScopeGuard<> guard;
    guard.Push([] { ObjArray::GetInstance()->Purge(); });

    auto o1 = ObjArray::GetInstance()->AllocObj();
    o1 = ObjArray::GetInstance()->AllocObj(2);

    std::vector<SharedObj> vecs;
    for (int i = 0; i < 16; ++ i)
        vecs.push_back(ObjArray::GetInstance()->AllocObj(i));
}

void test_shared_obj_array()
{
    test_trivial();
    test_none_trivial();
}
