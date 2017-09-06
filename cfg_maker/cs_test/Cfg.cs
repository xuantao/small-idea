/*
 * this file is auto generated.
 * please does not edit it manual!
*/
using System;
using System.Collections;
using System.Collections.Generic;

public static class GlobalVar
{
    public const int g_1 = 1;
    public const float f_1 = 0.0f;
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
public enum Enum1
{
    a = 0,
    b = a,
}
public class Struct0
{
    public class Innser
    {
        public static uint HASH_CODE = 0;

        public int a = 0;
        public int b = 0;
    }

    public static uint HASH_CODE = 0;

    public List<int> s0a;
    public int[] s0b;
    public int[] s0c;
}
public class Struct1 : Struct0
{
    public new static uint HASH_CODE = 0;

    public int s1a = 1;
    public float f1 = 0.0f;
    public string str2 = GlobalVar.s_1;
    public string str3 = "zouhui";
    public Struct0 s1 = new Struct0();
    public Struct0[] s2;
    public Struct0[] s3;
    public List<Detail.Enum2> s1arEnum;
    public Detail.Enum2[] s1arEnum2;
}
public class Struct2
{
    public static uint HASH_CODE = 0;

    public const int MAX = 100;
    public int a = 1;
    public int b = (int)Detail.Enum2.c;
    public int c = 0;
    public int d = 0;
    public string s = GlobalVar.s_1;
    public Struct1 s1 = new Struct1();
    public Struct1[] s2;
    public Struct1[] s3;
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
    public static uint HASH_CODE = 0;

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
