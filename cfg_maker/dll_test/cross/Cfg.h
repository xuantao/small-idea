/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#pragma once

#include <vector>
#include <string>
#include <array>

enum class GlobalType
{
    Invalid = -1,
    Bool = 0,
    Int = 1,
    Float = 2,
    String = 3,
};

extern const int g_1;
extern const float f_1;
extern const std::string s_1;
extern const bool b_1;

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
    struct Innser
    {
        static const uint32_t HASH_CODE = 3334548982;

        int a = 0;
        int b = 0;
    };

    static const uint32_t HASH_CODE = 778842033;

    std::vector<int> s0a;
    std::array<int, 10> s0b;
    std::array<int, 10> s0c;
};

struct Struct1 : public Struct0
{
    static const uint32_t HASH_CODE = 70717381;

    int s1a = 1;
    float f1 = 0.0f;
    std::string str2 = s_1;
    std::string str3 = "zouhui";
    Struct0 s1;
    std::array<Struct0, 1> s2;
    std::array<Struct0, 2> s3;
    std::vector<Detail::Enum2> s1arEnum;
    std::array<Detail::Enum2, 2> s1arEnum2;
};

struct Struct2
{
    static const uint32_t HASH_CODE = 1416237547;
    static const int MAX = 100;

    int a = 1;
    int b = (int)Detail::Enum2::c;
    int c = 0;
    int d = 0;
    std::string s = s_1;
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
    static const uint32_t HASH_CODE = 133125830;

    int ID = 0; //< 模板ID
    int StageID = 0; //< 关卡ID
    std::string Name; //< 名称
};


