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

template <class Ty>
void print(std::vector<Ty>& vec)
{
    printf("std::vector\n");
}

template <class Ty, size_t N>
void print(std::array<Ty, N>& arr)
{
    printf("std::array\n");
}

enum class TestConvert
{
    A,
    B,
};

int main(int argc, char* argv[])
{
    std::vector<int> vec;
    std::array<int, 10> arr;

    print(vec);
    print(arr);

    TestConvert tc;
    int& temp = (int&)tc;

    temp = (int)TestConvert::B;

    return 1;
}
