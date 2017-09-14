#include "TestTmp.h"
#include <stdio.h>

void TestFunc()
{
    for (int i = 0; TabInfo<TmpTy>::title[i]; ++i)
    {
        printf("title[%d] = %s\n", i, TabInfo<TmpTy>::title[i]);
    }
}
