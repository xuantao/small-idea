namespace Serialize
{
    /*
     * 二进制数据读取
     * NOTE:
     * 针对System.IO.BinaryReader的字符串处理做了转换
    */
    public class BinaryReader : IReader
    {
        protected System.IO.BinaryReader _reader;

        public BinaryReader(System.IO.BinaryReader reader)
        {
            _reader = reader;
        }

        public virtual bool StructBegin(int code, string name = null)
        {
            return true;
        }

        public virtual bool StructEnd()
        {
            return true;
        }

        public bool ArrayBegin(ref int length, string name = null)
        {
            length = _reader.ReadInt32();
            return true;
        }

        public bool ArrayEnd()
        {
            return true;
        }

        public bool Read(ref bool val, string name = null)
        {
            val = _reader.ReadBoolean();
            return true;
        }

        public bool Read(ref byte val, string name = null)
        {
            val = _reader.ReadByte();
            return true;
        }

        public bool Read(ref int val, string name = null)
        {
            val = _reader.ReadInt32();
            return true;
        }

        public bool Read(ref long val, string name = null)
        {
            val = _reader.ReadInt64();
            return true;
        }

        public bool Read(ref float val, string name = null)
        {
            val = _reader.ReadSingle();
            return true;
        }

        public bool Read(ref double val, string name = null)
        {
            val = _reader.ReadDouble();
            return true;
        }

        public bool Read(ref string val, string name = null)
        {
            int len = _reader.ReadInt32();
            byte[] data = _reader.ReadBytes(len);
            val = System.Text.Encoding.UTF8.GetString(data);
            return true;
        }
    }
}
