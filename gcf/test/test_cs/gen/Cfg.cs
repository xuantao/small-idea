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

}
public enum GlobalType
{
    Invalid = -1,
    Bool = 0,
    Int = 1,
    Float = 2,
    String = 3,
}
public class TabTest_1
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
public class Msg
{
    public const int HASH_CODE = -1854228442;

    public bool b = false;
    public int i = 0;
    public float f = 0.0f;
    public string s = "msg";
    public Inner inner = new Inner();
}
public enum Wtf
{
    Invalid = -1,
    Bool = 0,
    Int = 1,
    Float = 2,
    String = 3,
}
public class Sr3
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
