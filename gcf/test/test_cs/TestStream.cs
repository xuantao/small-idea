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

    public void TestWrite()
    {
        string text = "xuantao";
        writer.Write(text);

        Console.WriteLine("write length:{0}", writeStream.Seek(0, SeekOrigin.Current));
    }

    public void TestRead()
    {
        string data = reader.ReadString();
        if (data != "xuantao")
            Console.WriteLine("read error");

        Console.WriteLine("Read length:{0}", readStream.Seek(0, SeekOrigin.Current));
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
