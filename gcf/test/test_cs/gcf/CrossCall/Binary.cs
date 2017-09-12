/*
 * 二进制序列化
 */

using System;

namespace CrossCall
{
    /*
     * 二进制数据Reader 
     */
    public class BinaryReader : Serialize.IReader
    {
        protected IBinaryStream _stream = null;

        public BinaryReader(IBinaryStream stream)
        {
            _stream = stream;
        }

        public bool StructBegin(uint code, string name = null)
        {
            uint check = 0;
            if (!Read(ref check, null))
                return false;

            return check == code;
        }

        public bool StructEnd()
        {
            return true;
        }

        public bool ArrayBegin(ref int length, string name = null)
        {
            byte[] data = { 0, 0, 0, 0 };
            if (!_stream.Read(data)) return false;

            length = BitConverter.ToInt32(data, 0);
            return true;
        }

        public bool ArrayEnd()
        {
            return true;
        }

        public bool Read(ref bool val, string name = null)
        {
            byte[] data = { 0 };
            if (!_stream.Read(data)) return false;

            val = BitConverter.ToBoolean(data, 0);
            return true;
        }

        public bool Read(ref int val, string name = null)
        {
            byte[] data = { 0, 0, 0, 0 };
            if (!_stream.Read(data)) return false;

            val = BitConverter.ToInt32(data, 0);
            return true;
        }

        public bool Read(ref uint val, string name = null)
        {
            byte[] data = { 0, 0, 0, 0 };
            if (!_stream.Read(data)) return false;

            val = BitConverter.ToUInt32(data, 0);
            return true;
        }

        public bool Read(ref float val, string name = null)
        {
            byte[] data = { 0, 0, 0, 0 };
            if (!_stream.Read(data)) return false;

            val = BitConverter.ToSingle(data, 0);
            return true;
        }

        public bool Read(ref string val, string name = null)
        {
            int len = 0;
            if (!Read(ref len, null))
                return false;

            byte[] data = new byte[len];
            if (!_stream.Read(data))
                return false;

            val = System.Text.Encoding.UTF8.GetString(data);
            return true;
        }
    }

    /*
     * 二进制数据Writer
     */
    public class BinaryWriter : Serialize.IWriter
    {
        protected IBinaryStream _stream = null;

        public BinaryWriter(IBinaryStream stream)
        {
            _stream = stream;
        }

        public bool StructBegin(uint code, string name = null)
        {
            return _stream.Write(BitConverter.GetBytes(code));
        }

        public bool StructEnd()
        {
            return true;
        }

        public bool ArrayBegin(int length, string name = null)
        {
            return _stream.Write(BitConverter.GetBytes(length));
        }

        public bool ArrayEnd()
        {
            return true;
        }

        public bool Write(bool val, string name = null)
        {
            return _stream.Write(BitConverter.GetBytes(val));
        }

        public bool Write(int val, string name = null)
        {
            return _stream.Write(BitConverter.GetBytes(val));
        }

        public bool Write(uint val, string name = null)
        {
            return _stream.Write(BitConverter.GetBytes(val));
        }

        public bool Write(float val, string name = null)
        {
            return _stream.Write(BitConverter.GetBytes(val));
        }

        public bool Write(string val, string name = null)
        {
            return _stream.Write(BitConverter.GetBytes(val.Length)) &&
                _stream.Write(System.Text.Encoding.UTF8.GetBytes(val));
        }
    }
}
