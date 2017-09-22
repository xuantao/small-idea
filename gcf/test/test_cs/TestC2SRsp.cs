using System;
using System.Collections.Generic;

public class TestC2SRsp : TestC2S.IResponder
{
    public bool Test(bool v)
    {
        Console.WriteLine("cs Test(bool v:{0})", v);
        return !v;
    }

    public byte Test(byte v)
    {
        Console.WriteLine("cs Test(byte v:{0})", v);
        return (byte)(v + 1);
    }

    public int Test(int v)
    {
        Console.WriteLine("cs Test(int v:{0})", v);
        return v + 1;
    }

    public long Test(long v)
    {
        Console.WriteLine("cs Test(long v:{0})", v);
        return v + 1;
    }

    public float Test(float v)
    {
        Console.WriteLine("cs Test(float v:{0})", v);
        return v + 1;
    }

    public double Test(double v)
    {
        Console.WriteLine("cs Test(double v:{0})", v);
        return v + 1;
    }

    public string Test(string v)
    {
        Console.WriteLine("cs Test(string v:{0})", v);
        return "csharp";
    }

    public void Test(List<bool> v)
    {
        Console.WriteLine("cs Test(List<bool> v:{0})", v);
    }

    public void Test(FixedArray<byte, ArrayLength_2> v)
    {
        Console.WriteLine("cs FixedArray<byte, ArrayLength_2> v:{0})", v);
    }

    public void Test(List<List<int>> v)
    {
        Console.WriteLine("cs Test(List<List<int>> v:{0})", v);
    }

    public void Test(List<FixedArray<long, ArrayLength_2>> v)
    {
        Console.WriteLine("cs Test(FixedArray<long, ArrayLength_2>> v:{0})", v);
    }

    public void Test(FixedArray<List<float>, ArrayLength_2> v)
    {
        Console.WriteLine("cs Test(FixedArray<List<float>, ArrayLength_2> v:{0})", v);
    }

    public void Test()
    {
        Console.WriteLine("cs Test)");
    }

    public void Test(Msg msg)
    {
        Console.WriteLine("cs Test(Msg msg:{0})", msg);
    }

    public void Test(FixedArray<Msg, ArrayLength_2> msg)
    {
        Console.WriteLine("cs Test(FixedArray<Msg, ArrayLength_2> msg:{0})", msg);
    }

    public Msg Test(int a, int b)
    {
        Console.WriteLine("cs Test(int a:{0}, int b:{1})", a, b);
        return new Msg();
    }

    public List<Msg> Test(int a, int b, int c)
    {
        Console.WriteLine("cs Test(int a:{0}, int b:{1}, int c:{2})", a, b, c);
        return new List<Msg>();
    }

    public FixedArray<Msg, ArrayLength_2> Test(int a, int b, int c, int d)
    {
        Console.WriteLine("cs Test(int a:{0}, int b:{1}, int c:{2}, int d:{3})", a, b, c, d);
        return new FixedArray<Msg, ArrayLength_2>();
    }

    public KGPlayerData GetPlayerData()
    {
        return new KGPlayerData();
    }

    public void SetPlayerData(KGPlayerData data)
    {
        Console.WriteLine("SetPlayerData(KGPlayerData data:{0})", data);
    }

    public KGPlayerData TranslatePlayerData(KGPlayerData data)
    {
        Console.WriteLine("SetPlayerData(KGPlayerData data:{0})", data);
        return new KGPlayerData();
    }
}
