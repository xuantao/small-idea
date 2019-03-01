#include <stdio.h>
#include "ref_count_set.h"
#include "test.h"

UTILITY_USING_NAMESPACE;

void ref_count_set_test()
{
    int a = 11;
    ref_count_set<int> rfs;
    rfs.insert(a);
    rfs.insert(11);
    rfs.insert(11);

    printf("a ref count: %lld\n", rfs.ref_count(a));
    printf("a ref count: %lld\n", rfs.erase(a));

    rfs.insert(12);
    rfs.insert(13);
    rfs.insert(13);

    printf("first visit\n");
    rfs.traverse([ ](const int& v, std::ptrdiff_t c) {
        printf("\ton visitor v:%2d, c:%2d\n", v, (int)c);
        return true;
    });

    rfs.clear();
    printf("second visit\n");
    rfs.traverse([ ](const int& v, std::ptrdiff_t c) {
        printf("\ton visitor v:%2d, c:%2d\n", v, (int)c);
        return true;
    });

    //printf("a ref count: %d\n", rfs.erase(a));
}
