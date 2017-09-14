#include "TestTmp.h"
#include <stdio.h>

int main(int argc, char* argv[])
{

    for (int i = 0; TabInfo<TmpTy>::title[i]; ++i)
    {
        printf("title[%d] = %s\n", i, TabInfo<TmpTy>::title[i]);
    }

    const char* n1 = Name(TestEnum::a);
    const char* n2 = Name<TestEnum>(0);
    int idx1 = Index<TestEnum>(TestEnum::a);
    int idx2 = Index<TestEnum>("xuantao");

    TestEnum val1 = Value<TestEnum>(0);
    TestEnum val2 = Value<TestEnum>("xuantao");

    return 1;
}
