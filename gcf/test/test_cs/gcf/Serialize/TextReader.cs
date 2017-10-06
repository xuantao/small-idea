using System;

namespace Serialize
{
    /*
     * 序列化文本
     * 对数组元素使用','分割的，由此需要注意
     * 1. 不支持多维数组，无法解析数据
     * 2. 不支持自定义数据结构的数组
     * 3. 如果是字符串数组，单个元素中不能存在','
    */
    public class TextReader : IReader
    {
        int _index = 0;
        bool _isArray = false;
        string[] _array = null;
        ITokenReadStream _stream = null;

        public TextReader(ITokenReadStream stream)
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
            if (_isArray) throw new Exception("tab not support multi array");

            string text = _stream.Read();
            if (string.IsNullOrEmpty(text))
                return false;

            _index = 0;
            _isArray = true;
            _array = text.Split(',');

            length = _array.Length;
            return true;
        }

        public bool ArrayEnd()
        {
            if (!_isArray)
                return false;

            _isArray = false;
            _index = 0;
            _array = null;
            return true;
        }

        public bool Read(ref bool val, string name = null)
        {
            string text = Pop();
            if (string.IsNullOrEmpty(text))
                return false;

            text = text.ToLower();
            if (text == "true" || text == "1")
                val = true;
            else if (text == "false" || text == "0")
                val = false;
            else
                throw new Exception(string.Format("can not parse bool value with:{0}", text));

            return true;
        }

        public bool Read(ref byte val, string name = null)
        {
            string text = Pop();
            if (string.IsNullOrEmpty(text))
                return false;

            val = byte.Parse(text);
            return true;
        }

        public bool Read(ref int val, string name = null)
        {
            string text = Pop();
            if (string.IsNullOrEmpty(text))
                return false;

            val = int.Parse(text);
            return true;
        }

        public bool Read(ref long val, string name = null)
        {
            string text = Pop();
            if (string.IsNullOrEmpty(text))
                return false;

            val = long.Parse(text);
            return true;
        }

        public bool Read(ref float val, string name = null)
        {
            string text = Pop();
            if (string.IsNullOrEmpty(text))
                return false;

            val = float.Parse(text);
            return true;
        }

        public bool Read(ref double val, string name = null)
        {
            string text = Pop();
            if (string.IsNullOrEmpty(text))
                return false;

            val = double.Parse(text);
            return true;
        }

        public bool Read(ref string val, string name = null)
        {
            string text = Pop();
            if (string.IsNullOrEmpty(text))
                return false;

            val = text;
            return true;
        }

        protected string Pop()
        {
            if (_isArray)
            {
                if (_array == null || _index >= _array.Length)
                    return null;
                return _array[_index];
            }

            return _stream.Read();
        }
    }
}
