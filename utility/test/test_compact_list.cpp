#include "test_util.h"
#include "compact_list.h"

using namespace utility;

void test_compact_list() {
    CompactList<int> lst;

    for (auto beg = lst.begin(), end = lst.end(); beg != end; ++beg) {
        printf("impossable\n");
    }
    lst.erase(lst.end());

    lst.push_back(1);
    assert(lst.front() == 1);
    assert(lst.back() == 1);

    lst.erase(lst.cbegin());
    assert(lst.begin() == lst.end());

    lst.push_back(1);
    lst.push_back(2);
    assert(lst.front() == 1);
    assert(lst.back() == 2);

    lst.erase(lst.begin());
    assert(lst.front() == 2);
    assert(lst.back() == 2);

    lst.erase(lst.begin());
    assert(lst.begin() == lst.end());
}
