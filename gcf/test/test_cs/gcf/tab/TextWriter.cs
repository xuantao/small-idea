using System;
using System.Text;

namespace Tab
{
    public class TextWriter : Serialize.IWriter
    {
        StringBuilder _array = null;
        ITokenStream _stream = null;

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
            if (_array != null) throw new Exception("tab does not support multi array");

            _array = new StringBuilder();
            return true;
        }

        public bool ArrayEnd()
        {
            if (_array == null)
                return false;

            _stream.Push(_array.ToString());
            _array = null;
            return true;
        }

        public bool Write(bool val, string name = null)
        {
            return Push(val ? "true" : "false");
        }

        public bool Write(byte val, string name = null)
        {
            return Push(val.ToString());
        }

        public bool Write(int val, string name = null)
        {
            return Push(val.ToString());
        }

        public bool Write(long val, string name = null)
        {
            return Push(val.ToString());
        }

        public bool Write(float val, string name = null)
        {
            return Push(val.ToString());
        }

        public bool Write(double val, string name = null)
        {
            return Push(val.ToString());
        }

        public bool Write(string val, string name = null)
        {
            return Push(val);
        }

        protected bool Push(string val)
        {
            if (_array != null)
            {
                if (_array.Length > 0)
                    _array.Append(",");
                _array.Append(val);
                return true;
            }
            else
            {
                return _stream.Push(val);
            }
        }
    }
}
