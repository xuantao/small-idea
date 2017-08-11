#include <stdio.h>
#include "CfgTabParser.h"
#include "Cfg.h"
#include "CfgUtility.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <map>
#include <sstream>

using namespace cfg;

struct Keyfn
{
    int Key(Struct2& test) { return test.a; }
};

int main(int argc, char* argv[])
{
    //TabDataMap<int, Struct2, Keyfn> data;
    //if (data.Load("../work/out/Struct2.tab"))
    //{
    //    data.Find(1);
    //    printf("xxxx\n");
    //}

    //ks.find(key);

    //Struct1 s1;
    //Json::Write(std::cout, s1);

    //Struct2 s2;
    //Tab::Write(std::cout, s2);

    //TestTab();
    //TestJson();

    system("pause");

    return 1;
}
