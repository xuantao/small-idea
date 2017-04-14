#include "scoped_vector.h"
#include "scoped_test_base.h"

using namespace zh;

static void test_normal()
{
    scoped_vector<int> vec(test::allocate(100 * sizeof(int)));
    vec.push_back(1);
}

void test_scoped_vector()
{

}
