/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#pragma once

#include <vector>
#include <string>
#include <array>
#include <ostream>

enum GlobalType
{
    Invalid = -1,
    Bool = 0,
    Int = 1,
    Float = 2,
    String = 3,
};

static const int g_1 = 1;
static const float f_1 = 2.000000;
static const std::string s_1 = "xuantao";
static const bool b_1 = true;

enum Enum1
{
    a = 0,
    b = a,
};

enum Enum2
{
    c = 100,
    d = 101,
};

struct Struct0
{
    Struct0()
    {
        s0b.fill(0);
        s0c.fill(0);
    }

    std::vector<int> s0a;
    std::array<int, 10> s0b;
    std::array<int, 10> s0c;
};

struct Struct1 : public Struct0
{
    Struct1()
        : s1a(1)
        , s1b(2)
        , s1c(0)
        , b1(false)
        , f1(0.000000)
        , str2("s_1")
        , str3("zouhui")
    {
        s1arEnum2.fill(Enum2::c);
    }

    int s1a;
    int s1b;
    int s1c;
    bool b1;
    float f1;
    std::string str1;
    std::string str2;
    std::string str3;
    Struct0 s1;
    std::array<Struct0, 2> s11;
    std::vector<Enum2> s1arEnum;
    std::array<Enum2, 2> s1arEnum2;
};

struct Struct2
{
    static const int MAX = 100;

    Struct2()
        : a(1)
        , b((int)Enum2::c)
        , c(0)
        , d(0)
        , s("s_1")
    {
    }

    int a;
    int b;
    int c;
    int d;
    std::string s;
    Struct1 s1;
    std::array<Struct1, 2> s2;
};

namespace Enum
{
    /* GlobalType */
    const char* const * Names(GlobalType);
    const char* ToString(GlobalType value);
    bool ToEnum(const char* name, GlobalType& out);

    /* Enum1 */
    const char* const * Names(Enum1);
    const char* ToString(Enum1 value);
    bool ToEnum(const char* name, Enum1& out);

    /* Enum2 */
    const char* const * Names(Enum2);
    const char* ToString(Enum2 value);
    bool ToEnum(const char* name, Enum2& out);
} // end of namespace Enum

namespace Tab
{
    void WriteHeader(std::ostream& stream, const Struct2& def);
    void Write(std::ostream& stream, const Struct2& data);
    bool Load(const char* data, size_t size, std::vector<Struct2>& out);
} // end of namespace Tab

namespace Json
{
    void Write(std::ostream& stream, const Struct1& data);
    bool Load(const char* data, size_t size, Struct1& out);
} // end of namespace Json
