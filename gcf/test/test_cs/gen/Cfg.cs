/*
 * this file is auto generated.
 * please does not edit it manual!
*/
using System;
using System.Collections.Generic;

public static class GlobalVar
{
    public const int g_1 = 1;
    public const float f_1 = 2.000000f;
    public const string s_1 = "xuantao";
    public const bool b_1 = true;
    public const int MAX_TALENT_COUNT = 128;
    public const int MAX_SKILL_COUNT = 16;
    public const int MAX_MOUNT_SLOT_COUNT = 3;
    public const int MAX_PLAYER_DATA_SHELL_COUNT = 3;
    public const int MAX_EQUIPMENT_COUNT = 12;

}
public enum GlobalType
{
    Invalid = -1,
    Bool = 0,
    Int = 1,
    Float = 2,
    String = 3,
}
public partial class TabTest_1
{
    public const int HASH_CODE = -296353828;

    public bool _bool = true;
    public byte _byte = 1;
    public int _int = 1;
    public long _long = 1;
    public float _float = 1.000000f;
    public double _double = 1.000000;
    public string _string = "string";
}
public enum Enum1
{
    a = 0,
    b = a,
}
public class Struct0
{
    public class Innser
    {
        public const int HASH_CODE = 1686872738;

        public int a = 0;
        public int b = 0;
    }

    public const int HASH_CODE = -1900661952;

    public List<int> s0a = new List<int>();
    public FixedArray<int, ArrayLength_10> s0b = new FixedArray<int, ArrayLength_10>();
    public FixedArray<int, ArrayLength_10> s0c = new FixedArray<int, ArrayLength_10>();
}
public class Struct1 : Struct0
{
    public new const int HASH_CODE = -487629328;

    public int s1a = 1;
    public float f1 = 0.0f;
    public string str2 = GlobalVar.s_1;
    public string str3 = "zouhui";
    public Struct0 s1 = new Struct0();
    public FixedArray<Struct0, ArrayLength_1> s2 = new FixedArray<Struct0, ArrayLength_1>();
    public FixedArray<Struct0, ArrayLength_2> s3 = new FixedArray<Struct0, ArrayLength_2>();
    public List<Detail.Enum2> s1arEnum = new List<Detail.Enum2>();
    public FixedArray<Detail.Enum2, ArrayLength_2> s1arEnum2 = new FixedArray<Detail.Enum2, ArrayLength_2>();
}
public class Struct2
{
    public const int HASH_CODE = 726836598;
    public const int MAX = 100;

    public int a = 1;
    public int b = (int)Detail.Enum2.c;
    public int c = 0;
    public int d = 0;
    public string s = GlobalVar.s_1;
    public Struct1 s1 = new Struct1();
    public FixedArray<Struct1, ArrayLength_1> s2 = new FixedArray<Struct1, ArrayLength_1>();
    public FixedArray<Struct1, ArrayLength_2> s3 = new FixedArray<Struct1, ArrayLength_2>();
}
public class Inner
{
    public const int HASH_CODE = -2071995248;

    public int ii = 0;
    public string name = "inner";
}
public class SkillLevel
{
    public const int HASH_CODE = -1463516307;

    public long m_nSkillID = 0;
    public int m_nSkillLevel = 0;
}
public class KGSkillData
{
    public const int HASH_CODE = 51293418;

    public FixedArray<SkillLevel, ArrayLength_16> m_SkillInfo = new FixedArray<SkillLevel, ArrayLength_16>();
}
public class KGTalentData
{
    public const int HASH_CODE = -2098121292;

    public FixedArray<int, ArrayLength_128> m_nTalent = new FixedArray<int, ArrayLength_128>();
}
public class KGEquipmentItemData
{
    public const int HASH_CODE = -1048349466;

    public int m_nTypeID = 0;
    public int m_nQualityLevel = 0;
    public int m_nNeedPlayerLevel = 0;
    public FixedArray<int, ArrayLength_3> m_MountAttrIds = new FixedArray<int, ArrayLength_3>();
    public int m_nRandSeed = 0;
}
public class KGEquipmentSlotData
{
    public const int HASH_CODE = -2111042310;

    public int m_nLevel = 0;
    public FixedArray<int, ArrayLength_3> m_MountItemTypeIds = new FixedArray<int, ArrayLength_3>();
}
public class KGEquipmentData
{
    public const int HASH_CODE = -1047003838;

    public FixedArray<KGEquipmentItemData, ArrayLength_12> m_EquipmentItemData = new FixedArray<KGEquipmentItemData, ArrayLength_12>();
    public FixedArray<KGEquipmentSlotData, ArrayLength_12> m_EquipmentSlotData = new FixedArray<KGEquipmentSlotData, ArrayLength_12>();
}
public class KGPlayerData
{
    public const int HASH_CODE = 668747640;

    public int m_nForceID = 0;
    public int m_nStar = 0;
    public int m_nQuality = 0;
    public int m_nLevel = 0;
    public int m_nExteriorID = 0;
    public int m_nWeaponRepresentID = 0;
    public KGSkillData m_SkillData = new KGSkillData();
    public KGTalentData m_TalentData = new KGTalentData();
    public KGEquipmentData m_EquipmentData = new KGEquipmentData();
}
public class Msg
{
    public class Inner
    {
        public const int HASH_CODE = -2071995248;

        public int idx = 101;
        public string name = NAME;
    }

    public const int HASH_CODE = 1039095060;
    public const string NAME = "MSG";

    public bool _bool = true;
    public byte _byte = 1;
    public int _int = 2;
    public long _long = 3;
    public float _float = 4.000000f;
    public double _double = 5.000000;
    public string _string = "msg";
    public List<int> _int_ary = new List<int>();
    public FixedArray<int, ArrayLength_2> _int_ary_2 = new FixedArray<int, ArrayLength_2>();
    public Msg.Inner _inner = new Msg.Inner();
    public List<Msg.Inner> _inner_ary = new List<Msg.Inner>();
    public FixedArray<Msg.Inner, ArrayLength_2> _inner_ary_2 = new FixedArray<Msg.Inner, ArrayLength_2>();
}
public enum Wtf
{
    Invalid = -1,
    Bool = 0,
    Int = 1,
    Float = 2,
    String = 3,
}
public partial class Sr3
{
    public const int HASH_CODE = 1645663838;

    public int ID = 0;
    public int StageID = 0;
    public string Name = "";
}
namespace Detail
{
    public enum Enum2
    {
        c = 100,
        d = 101,
    }
}
