using System;

namespace Serialize
{
    public class BinaryWriter : IWriter
    {
        protected IBinaryStream _stream = null;

        public BinaryWriter(IBinaryStream stream)
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

        public bool Write(byte val, string name = null)
        {
            byte[] by = { val };
            return _stream.Write(by);
        }

        public bool Write(int val, string name = null)
        {
            return _stream.Write(BitConverter.GetBytes(val));
        }

        public bool Write(long val, string name = null)
        {
            return _stream.Write(BitConverter.GetBytes(val));
        }

        public bool Write(float val, string name = null)
        {
            return _stream.Write(BitConverter.GetBytes(val));
        }

        public bool Write(double val, string name = null)
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
