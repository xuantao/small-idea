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

struct TabTest_1
{
    static const int32_t HASH_CODE = -296353828;

    bool _bool = true; //< 测试bool
    int8_t _byte = 1; //< 测试byte
    int _int = 1; //< 测试int
    int64_t _long = 1; //< 测试long
    float _float = 1; //< 测试float
    double _double = 1; //< 测试double
    std::string _string = "string"; //< 测试string
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
        static const int32_t HASH_CODE = 1686872738;

        int a = 0;
        int b = 0;
    };

    static const int32_t HASH_CODE = -1900661952;

    std::vector<int> s0a;
    std::array<int, 10> s0b;
    std::array<int, 10> s0c;
};

struct Struct1 : public Struct0
{
    static const int32_t HASH_CODE = -487629328;

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
    static const int32_t HASH_CODE = 726836598;
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

struct Inner
{
    static const int32_t HASH_CODE = -2071995248;

    int ii = 0;
    std::string name = "inner";
};

extern const int MAX_TALENT_COUNT;
extern const int MAX_SKILL_COUNT;
extern const int MAX_MOUNT_SLOT_COUNT;
extern const int MAX_PLAYER_DATA_SHELL_COUNT;
extern const int MAX_EQUIPMENT_COUNT;

struct SkillLevel
{
    static const int32_t HASH_CODE = -1463516307;

    int64_t m_nSkillID = 0;
    int m_nSkillLevel = 0;
};

struct KGSkillData
{
    static const int32_t HASH_CODE = 51293418;

    std::array<SkillLevel, 16> m_SkillInfo;
};

struct KGTalentData
{
    static const int32_t HASH_CODE = -2098121292;

    std::array<int, 128> m_nTalent;
};

struct KGEquipmentItemData
{
    static const int32_t HASH_CODE = -1048349466;

    int m_nTypeID = 0;
    int m_nQualityLevel = 0;
    int m_nNeedPlayerLevel = 0;
    std::array<int, 3> m_MountAttrIds;
    int m_nRandSeed = 0;
};

struct KGEquipmentSlotData
{
    static const int32_t HASH_CODE = -2111042310;

    int m_nLevel = 0;
    std::array<int, 3> m_MountItemTypeIds;
};

struct KGEquipmentData
{
    static const int32_t HASH_CODE = -1047003838;

    std::array<KGEquipmentItemData, 12> m_EquipmentItemData;
    std::array<KGEquipmentSlotData, 12> m_EquipmentSlotData;
};

struct KGPlayerData
{
    static const int32_t HASH_CODE = 668747640;

    int m_nForceID = 0;
    int m_nStar = 0;
    int m_nQuality = 0;
    int m_nLevel = 0;
    int m_nExteriorID = 0;
    int m_nWeaponRepresentID = 0;
    KGSkillData m_SkillData;
    KGTalentData m_TalentData;
    KGEquipmentData m_EquipmentData;
};

struct Msg
{
    struct Inner
    {
        static const int32_t HASH_CODE = -2071995248;

        int idx = 101;
        std::string name = NAME;
    };

    static const int32_t HASH_CODE = 1039095060;
    static const std::string NAME;

    bool _bool = true;
    int8_t _byte = 1;
    int _int = 2;
    int64_t _long = 3;
    float _float = 4.000000;
    double _double = 5.000000;
    std::string _string = "msg";
    std::vector<int> _int_ary;
    std::array<int, 2> _int_ary_2;
    Inner _inner;
    std::vector<Inner> _inner_ary;
    std::array<Inner, 2> _inner_ary_2;
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
    static const int32_t HASH_CODE = 1645663838;

    int ID = 0; //< 模板ID
    int StageID = 0; //< 关卡ID
    std::string Name; //< 名称
};


