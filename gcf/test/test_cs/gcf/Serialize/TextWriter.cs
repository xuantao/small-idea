using System;
using System.Text;

namespace Serialize
{
    /*
     * 序列化文本
     * 对数组元素使用','分割的，由此需要注意
     * 1. 不支持多维数组，无法解析数据
     * 2. 不支持自定义数据结构的数组
     * 3. 如果是字符串数组，单个元素中不能存在','
    */
    public class TextWriter : IWriter
    {
        StringBuilder _array = null;
        ITokenWriteStream _stream = null;

        public TextWriter(ITokenWriteStream stream)
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
            if (_array != null) throw new Exception("tab does not support multi array");

            _array = new StringBuilder();
            return true;
        }

        public bool ArrayEnd()
        {
            if (_array == null)
                return false;

            _stream.Write(_array.ToString());
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
                return _stream.Write(val);
            }
        }
    }
}
