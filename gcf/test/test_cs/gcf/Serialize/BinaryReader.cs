using System;

namespace Serialize
{
    public class BinaryReader : IReader
    {
        protected IBinaryStream _stream = null;

        public BinaryReader(IBinaryStream stream)
        {
            _stream = stream;
        }

        public bool StructBegin(int code, string name = null)
        {
            return true;
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

        public bool Read(ref byte val, string name = null)
        {
            byte[] data = { 0 };
            if (!_stream.Read(data)) return false;

            val = data[0];
            return true;
        }

        public bool Read(ref int val, string name = null)
        {
            byte[] data = { 0, 0, 0, 0 };
            if (!_stream.Read(data)) return false;

            val = BitConverter.ToInt32(data, 0);
            return true;
        }

        public bool Read(ref long val, string name = null)
        {
            byte[] data = { 0, 0, 0, 0, 0, 0, 0, 0 };
            if (!_stream.Read(data)) return false;

            val = BitConverter.ToInt64(data, 0);
            return true;
        }

        public bool Read(ref float val, string name = null)
        {
            byte[] data = { 0, 0, 0, 0 };
            if (!_stream.Read(data)) return false;

            val = BitConverter.ToSingle(data, 0);
            return true;
        }

        public bool Read(ref double val, string name = null)
        {
            byte[] data = { 0, 0, 0, 0, 0, 0, 0, 0 };
            if (!_stream.Read(data)) return false;

            val = BitConverter.ToDouble(data, 0);
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
}
