/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#pragma once

#include <vector>
#include <string>
#include <array>
#include <ostream>

enum class GlobalType
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

enum class Enum1
{
    a = 0,
    b = a,
};

namespace Detail
{
    enum class Enum2
    {
        c = 100,
        d = 101,
    };

}
struct Struct0
{
    std::vector<int> s0a;
    std::array<int, 10> s0b;
    std::array<int, 10> s0c;
};

struct Struct1 : public Struct0
{
    Struct1()
        : s1a(1)
        , f1(0.0f)
        , str2(s_1)
        , str3("zouhui")
    {
    }

    int s1a;
    float f1;
    std::string str2;
    std::string str3;
    Struct0 s1;
    std::array<Struct0, 1> s2;
    std::array<Struct0, 2> s3;
    std::vector<Detail::Enum2> s1arEnum;
    std::array<Detail::Enum2, 2> s1arEnum2;
};

struct Struct2
{
    static const int MAX = 100;

    Struct2()
        : a(1)
        , b((int)Detail::Enum2::c)
        , c(0)
        , d(0)
        , s(s_1)
    {
    }

    int a;
    int b;
    int c;
    int d;
    std::string s;
    Struct1 s1;
    std::array<Struct1, 1> s2;
    std::array<Struct1, 2> s3;
};

enum class Wtf
{
    Invalid = -1,
    Bool = 0,
    Int = 1,
    Float = 2,
    String = 3,
};

struct Sr3
{
    Sr3()
        : ID(0)
        , StageID(0)
    {
    }

    int ID;
    int StageID;
    std::string Name;
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

    /* Detail::Enum2 */
    const char* const * Names(Detail::Enum2);
    const char* ToString(Detail::Enum2 value);
    bool ToEnum(const char* name, Detail::Enum2& out);

    /* Wtf */
    const char* const * Names(Wtf);
    const char* ToString(Wtf value);
    bool ToEnum(const char* name, Wtf& out);
} // end of namespace Enum

namespace Tab
{
    void WriteHeader(std::ostream& stream, const Struct2& def);
    void Write(std::ostream& stream, const Struct2& data);
    bool Load(const char* data, size_t size, std::vector<Struct2>& out, const char* chunk = nullptr);
    void WriteHeader(std::ostream& stream, const Sr3& def);
    void Write(std::ostream& stream, const Sr3& data);
    bool Load(const char* data, size_t size, std::vector<Sr3>& out, const char* chunk = nullptr);
} // end of namespace Tab

namespace Json
{
    void Write(std::ostream& stream, const Struct1& data);
    bool Load(const char* data, size_t size, Struct1& out);
} // end of namespace Json
