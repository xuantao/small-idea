#include "util.h"
#include "scoped_vector.h"

USING_NAMESPACE_ZH;

static void test_normal()
{
    scoped_vector<int> vec(util::allocate(5 * sizeof(int)));
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);
    //vec.push_back(6);   // assert, max_size
}

static void test_iterator()
{
    scoped_vector<int> vec(util::allocate(5 * sizeof(int)));
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

    it_1 -= 1;
    it_2 += 1;
    assert(it_1 == it_2);
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
}
