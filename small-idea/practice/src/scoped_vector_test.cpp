#include "scoped_vector.h"
#include "scoped_test_base.h"

using namespace zh;

static void test_normal()
{
    scoped_vector<int> vec(test::allocate(5 * sizeof(int)));
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);
    //vec.push_back(6);   // assert, max_size
}

static void test_iterator()
{
    scoped_vector<int> vec(test::allocate(5 * sizeof(int)));
    vec.push_back(1);
    vec.push_back(3);
    vec.push_back(4);

    auto it_1 = vec.begin();
    auto it_2 = vec.begin() + 2;

    assert(it_2 - it_1 == 2);
    //size_t c = it_1 - it_2; // assert(false)

    it_1++;
    assert(it_2 - it_1 == 1);
    assert(it_2 == ++it_1);

    --it_2;
    assert(it_1 - it_2 == 1);
    it_2--;
    assert(it_1 - it_2 == 2);

    //it_1 -= 1;
    //it_2 += 1;
    //assert(it_1 == it_2);

}

void test_scoped_vector()
{
    test_normal();
    test_iterator();
}
