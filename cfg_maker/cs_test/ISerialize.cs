namespace Serialize
{
    public interface IReader
    {
        bool StructBegin(uint code, string name = null);
        bool StructEnd();
        bool ArrayBegin(ref int length, string name = null);
        bool ArrayEnd();
        bool Read(ref bool val, string name = null);
        bool Read(ref int val, string name = null);
        bool Read(ref uint val, string name = null);
        bool Read(ref float val, string name = null);
        bool Read(ref string val, string name = null);
    }

    public interface IWriter
    {
        bool StructBegin(uint code, string name = null);
        bool StructEnd();
        bool ArrayBegin(int length, string name = null);
        bool ArrayEnd();
        bool Write(bool val, string name = null);
        bool Write(int val, string name = null);
        bool Write(uint val, string name = null);
        bool Write(float val, string name = null);
        bool Write(string val, string name = null);
    }
}
