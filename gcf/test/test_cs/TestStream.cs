using System;
using System.IO;
using System.Runtime.InteropServices;

public unsafe class TestStream
{
    const int SIZE = 128;

    byte[] data = new byte[SIZE];
    IntPtr ptr;
    UnmanagedMemoryStream readStream;
    UnmanagedMemoryStream writeStream;
    BinaryReader reader;
    BinaryWriter writer;

    public void Init()
    {
        ptr = Marshal.AllocHGlobal(SIZE);

        readStream = new UnmanagedMemoryStream((byte*)ptr.ToPointer(), SIZE, SIZE, FileAccess.Read);
        writeStream = new UnmanagedMemoryStream((byte*)ptr.ToPointer(), SIZE, SIZE, FileAccess.Write);

        reader = new BinaryReader(readStream);
        writer = new BinaryWriter(writeStream);
    }

    bool _bool = false;
    byte _byte = 1;
    short _short = 2;
    int _int = 3;
    long _long = 4;
    float _float = 5.0f;
    double _double = 6.0f;
    string _string = "xuantao";

    public void TestWrite()
    {
        writer.Write((int)0);

        writer.Write(_bool);
        writer.Write(_byte);
        writer.Write(_short);
        writer.Write(_int);
        writer.Write(_long);
        writer.Write(_float);
        writer.Write(_double);
        Write(_string);

        long length = writeStream.Seek(0, SeekOrigin.Current);
        writeStream.Seek(0, SeekOrigin.Begin);
        writer.Write((int)length);
        writeStream.Seek(0, SeekOrigin.Begin);
        Console.WriteLine("write length:{0}", length);
    }

    public void TestRead()
    {
        int length = reader.ReadInt32();
        bool _bo = reader.ReadBoolean();
        byte _bt = reader.ReadByte();
        short _sh = reader.ReadInt16();
        int _in = reader.ReadInt32();
        long _lo = reader.ReadInt64();
        float _fl = reader.ReadSingle();
        double _do = reader.ReadDouble();
        string _st = ReadStr();

        long rdLen = readStream.Seek(0, SeekOrigin.Current);
        readStream.Seek(0, SeekOrigin.Begin);

        Console.WriteLine("Read length:{0} total:{1}", length, rdLen);
    }

    public void Reset()
    {
        writeStream.Seek(0, SeekOrigin.Begin);
        readStream.Seek(0, SeekOrigin.Begin);
    }

    public void Test2()
    {
        byte[] buf = { 0, 0, 0, 0 };
        for (int i = 0; i < 65537; ++i)
        {
            Encode(i, buf);
            if (i != Decode(buf))
                Console.WriteLine("encode length error i:{0} value:{1}", i, Decode(buf));
        }
    }

    void Write(string str)
    {
        byte[] bytes = System.Text.Encoding.UTF8.GetBytes(str);
        writer.Write(bytes.Length);
        writer.Write(bytes);
    }

    string ReadStr()
    {
        int length = reader.ReadInt32();
        byte[] bytes = reader.ReadBytes(length);
        return System.Text.Encoding.UTF8.GetString(bytes);
    }

    int Encode(int len, byte[] data)
    {
        int c = 0;
        do
        {
            byte by = (byte)(len & 0x7F);
            len >>= 7;
            if (len != 0) by |= 0x80;
            data[c++] = by;
        } while (len != 0);

        return c;
    }

    int Decode(byte[] data)
    {
        int val = 0;
        int idx = 0;
        int shift = 0;
        byte by = 0;
        do
        {
            by = data[idx++];
            val |= (by & 0x7f) << shift;
            shift += 7;
        } while ((by & 0x80) != 0);
        return val;
    }
}
