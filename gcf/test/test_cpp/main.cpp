﻿#include "TestTmp.h"
#include <stdio.h>

int main(int argc, char* argv[])
{

    for (int i = 0; TabInfo<TmpTy>::title[i]; ++i)
    {
        printf("title[%d] = %s\n", i, TabInfo<TmpTy>::title[i]);
    }

    return 1;
}
