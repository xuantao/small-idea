namespace Serialize
{
    /*
     * 二进制数据写入
     * NOTE:
     * 针对System.IO.BinaryWriter的字符串处理做了转换
    */
    public class BinaryWriter : IWriter
    {
        protected System.IO.BinaryWriter _writer;

        public BinaryWriter(System.IO.BinaryWriter writer)
        {
            _writer = writer;
        }

        public virtual bool StructBegin(int code, string name = null)
        {
            return true;
        }

        public virtual bool StructEnd()
        {
            return true;
        }

        public bool ArrayBegin(int length, string name = null)
        {
            _writer.Write(length);
            return true;
        }

        public bool ArrayEnd()
        {
            return true;
        }

        public bool Write(bool val, string name = null)
        {
            _writer.Write(val);
            return true;
        }

        public bool Write(byte val, string name = null)
        {
            _writer.Write(val);
            return true;
        }

        public bool Write(int val, string name = null)
        {
            _writer.Write(val);
            return true;
        }

        public bool Write(long val, string name = null)
        {
            _writer.Write(val);
            return true;
        }

        public bool Write(float val, string name = null)
        {
            _writer.Write(val);
            return true;
        }

        public bool Write(double val, string name = null)
        {
            _writer.Write(val);
            return true;
        }

        public bool Write(string val, string name = null)
        {
            var bytes = System.Text.Encoding.UTF8.GetBytes(val);
            _writer.Write(bytes.Length);
            _writer.Write(bytes);
            return true;
        }
    }
}
