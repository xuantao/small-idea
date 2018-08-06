#include "utility/scope_map.h"
#include "utility/scope.h"
#include "test_util.h"
#include "test.h"

UTILITY_USING_NAMESPACE;

static void test_normal()
{
    scope_map<int, int> map = scoped::map<int, int>(10);

    map.insert(std::make_pair(1, 1));
    map.insert(std::make_pair(2, 2));
    map.insert(std::make_pair(3, 3));
    map[1] = 2;
    assert(map[1] == 2);

    map.erase(2);
    map.erase(map.find(3));
    map.erase(map.begin(), map.end());

    assert(map.size() == 0);
}

static void test_reference()
{
    int count = 1000;
    scope_map<int, int> map = scoped::map<int, int>(count);
    std::map<int, int> stdMap;
    for (int i = 0; i < count; ++i)
    {
        map[i] = i + 1;
        stdMap[i] = i + 1;
    }

    for (int i = 0; i < count; ++i)
        assert(map[i] == stdMap[i]);
}

void scoped_map_test()
{
    test_normal();
    test_reference();
}
