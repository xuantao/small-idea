/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#pragma once

const int g_1 = 1;
const float f_1 = 2.000000;
const std::string s_1 = "";
const bool b_1 = true;
#include "pre.h" // "prev.txt"
enum Enum1
{
    a = 0
    b = a
};

enum Enum2
{
    c = 100
    d = 101
};

struct Struct0
{
    std::vector<int> a;
    std::vector<std::array<int, 10> > b;
    std::array<std::vector<std::array<int, 10> >, 10> c;
};

struct Struct1 : public Struct0
{
    Struct1()
        : a(1)
        , b(2)
        , c(0)
    { }

    int a;
    int b;
    int c;
};

struct Struct2 : public Struct1
{
    static const int MAX = 100;

    Struct2()
        : a(1)
        , b((int)Enum2::c)
        , c(0)
        , s(s_1)
    { }

    int a;
    int b;
    int c;
    std::string s;
    Struct1 s1;
    std::array<Struct1, 2> s2;
};

enum GlobalType
{
    Invalid = -1
    Bool = 0
    Int = 1
    Float = 2
    String = 3
};

namespace Enum
{
    const char* const * Names(Enum1);
    const char* ToString(Enum1 value);
    bool ToEnum(Enum1& out, const char* name);
    const char* const * Names(Enum2);
    const char* ToString(Enum2 value);
    bool ToEnum(Enum2& out, const char* name);
    const char* const * Names(GlobalType);
    const char* ToString(GlobalType value);
    bool ToEnum(GlobalType& out, const char* name);
} // end of namespace Enum

namespace Tab
{
    int Load(ITabFile* pTab, std::vector<Struct0>& out);
    int Load(ITabFile* pTab, std::vector<Struct1>& out);
    int Load(ITabFile* pTab, std::vector<Struct2>& out);
} // end of namespace Tab

namespace Json
{
} // end of namespace Json
