#include "util.h"
#include "scoped_vector.h"

USING_NAMESPACE_ZH;

static void test_normal()
{
    scoped_vector<int> vec = util::vector<int>(5);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);
    //vec.push_back(6);   // assert, max_size
}

static void test_iterator()
{
    scoped_vector<int> vec = util::vector<int>(5);
    vec.push_back(1);
    vec.push_back(3);
    vec.push_back(4);

    scoped_vector<int>::iterator it_1;

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
    scoped_vector<int> vec = util::vector<int>(5);
    vec.push_back(1);
    vec.push_back(3);
    vec.push_back(4);

    scoped_vector<int>::iterator cIt0;
    scoped_vector<int>::const_iterator cIt1;
    scoped_vector<int>::const_iterator cIt2;

    cIt0 = vec.begin();
    cIt1 = vec.begin();
    cIt2 = vec.cbegin();

    assert(cIt0 == cIt1);
    assert(cIt1 == cIt0);
    assert(cIt1 == cIt2);

    //*cIt1 = 2; static_assert(false, "const l value reference")
}

static void test_build_by_vector()
{
    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);
    vi.push_back(4);

    auto dst = util::vector(vi, 2);
    for (size_t i = 0; i < dst.size(); ++i)
    {
        printf("idx=%d val=%d\n", (int)i, dst[i]);
    }
}

void test_scoped_vector()
{
    test_normal();
    test_iterator();
    test_const_iterator();
}
