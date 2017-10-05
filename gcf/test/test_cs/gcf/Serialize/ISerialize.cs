namespace Serialize
{
    public interface IReader
    {
        bool StructBegin(int code, string name = null);
        bool StructEnd();
        bool ArrayBegin(ref int length, string name = null);
        bool ArrayEnd();
        bool Read(ref bool val, string name = null);
        bool Read(ref byte val, string name = null);
        bool Read(ref int val, string name = null);
        bool Read(ref long val, string name = null);
        bool Read(ref float val, string name = null);
        bool Read(ref double val, string name = null);
        bool Read(ref string val, string name = null);
    }

    public interface IWriter
    {
        bool StructBegin(int code, string name = null);
        bool StructEnd();
        bool ArrayBegin(int length, string name = null);
        bool ArrayEnd();
        bool Write(bool val, string name = null);
        bool Write(byte val, string name = null);
        bool Write(int val, string name = null);
        bool Write(long val, string name = null);
        bool Write(float val, string name = null);
        bool Write(double val, string name = null);
        bool Write(string val, string name = null);
    }

    public interface IBinaryStream
    {
        bool Read(byte[] data);
        bool Write(byte[] data);
    }

    public interface ITokenStream
    {
        string Read();
        bool Write(string token);
    }
}
