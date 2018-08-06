#include <algorithm>
#include "utility/scope.h"
#include "test_util.h"
#include "test.h"

UTILITY_USING_NAMESPACE;

static void test_normal()
{
    scope_vector<int> vec = scoped::vector<int>(5);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);
    //vec.push_back(6);   // assert, max_size

    std::for_each(vec.cbegin(), vec.cend(), [ ](int v) {
        test::log("test for_each v:%d", v);
    });

    std::for_each(vec.begin(), vec.end(), [ ](int v) {
        test::log("test for_each v:%d", v);
    });

    // test pop back
    vec.pop_back();
    assert(vec.size() == 4);

    // test erase iterator
    vec.erase(vec.begin() + 1);
    assert(vec[1] == 3);
    assert(vec.size() == 3);

    // test erase const_iterator
    vec.erase(vec.cbegin());
    assert(vec.front() == 3);
    assert(vec.size() == 2);

    // test_insert
    vec.insert(vec.cbegin() + 1, 10);
    assert(vec[1] == 10);
    assert(vec.size() == 3);
    assert(vec.back() == 4);
}

static void test_obj_move()
{
    test::log(test::Tab::tab++, "init startup data");
    test::Obj obj;                  // test::Obj()
    scope_vector<test::Obj> vec = scoped::vector<test::Obj>(3);

    vec.push_back(obj);             // test::Obj(const test::Obj&)
    vec.push_back(1);               // test::Obj(int)
    vec.push_back(test::Obj(2));    // test::Obj(int)
                                    // test::Obj(test::Obj&&)
                                    // ~test::Obj()
    test::log(--test::Tab::tab, "init complete");

    assert(vec[0] == 0);
    assert(vec[1] == 1);
    assert(vec[2] == 2);


    test::log(test::Tab::tab++, "test erase begin");
    vec.erase(vec.begin() + 1);     // ~Test()
                                    // Test(const test::Obj&)
                                    // ~Test()
    assert(vec[0] == 0);
    assert(vec[1] == 2);
    test::log(--test::Tab::tab, "test erase end");

    test::log(test::Tab::tab++, "test insert begin");
    vec.insert(vec.begin() + 1, test::Obj(1));          // test::Obj(int)
                                                        // test::Obj(const test::Obj&)
                                                        // ~test::Obj()
                                                        // test::Obj(test::Obj&&)
                                                        // ~test::Obj()

    assert(vec[0] == 0);
    assert(vec[1] == 1);
    assert(vec[2] == 2);
    test::log(--test::Tab::tab, "test insert end");
    // ~test::Obj()
    // ~test::Obj()
    // ~test::Obj()
    // ~test::Obj()
}

static void test_iterator()
{
    scope_vector<int> vec = scoped::vector<int>(5);
    vec.push_back(1);
    vec.push_back(3);
    vec.push_back(4);

    scope_vector<int>::iterator it_1;

    //assert(*it_1 == 0);  // assert(false) empty it;

    it_1 = vec.begin();
    auto it_2 = vec.begin() + 2;

    *it_1 = 1;
    //it_2 += 1;  // assert(false); end() can not add

    assert(*it_1 == 1);
    assert(it_1[1] == 3);
    assert(it_1 < it_2);
    assert(it_2 >= it_1);

    assert(it_2 - it_1 == 2);
    assert(it_1 - it_2 == -2);

    it_1++;
    assert(it_2 - it_1 == 1);
    assert(it_2 == ++it_1);

    --it_2;
    assert(it_1 - it_2 == 1);
    it_2--;
    assert(it_1 - it_2 == 2);

    it_1 -= 1;
    it_2 += 1;
    assert(it_1 == it_2);
    assert(it_1 >= it_2);
    assert(it_1 <= it_2);
}

static void test_const_iterator()
{
    scope_vector<int> vec = scoped::vector<int>(5);
    vec.push_back(1);
    vec.push_back(3);
    vec.push_back(4);

    scope_vector<int>::iterator cIt0;
    scope_vector<int>::const_iterator cIt1;
    scope_vector<int>::const_iterator cIt2;

    cIt0 = vec.begin();
    cIt1 = vec.begin();
    cIt2 = vec.cbegin();

    assert(cIt0 == cIt1);
    assert(cIt1 == cIt0);
    assert(cIt1 == cIt2);

    //*cIt1 = 2; static_assert(false, "const l value reference")
}

void scoped_vector_test()
{
    test::log(test::Tab::tab++, "scoped_vector_test begin");
    test_normal();
    //test_iterator();
    test_obj_move();
    test::log(--test::Tab::tab, "scoped_vector_test end");
}
