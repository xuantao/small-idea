using System.IO;

namespace Tab
{
    public delegate bool ReadFn<T>(Serialize.IReader rader, ref T val, string name);

    public interface ILoader<T>
    {
        bool Load(out T val);
    }

    public class TextLoader<T> : ILoader<T> where T : new()
    {
        IndexMap _idxMap;
        Parser _parser;
        int _index = 0;
        string[] _lines;
        bool _hasDefault = false;
        T _default;
        Serialize.ReadCallback<T> _fn;

        public bool Setup(Serialize.ReadCallback<T> fn, string[] titles, string[] lines, int lineBeg, bool hasDefault)
        {
            if (lineBeg < 1)
                return false;   // first line must be titles
            if (lines.Length <= lineBeg)
                return false;   // error parameter

            _idxMap = new IndexMap();
            if (!_idxMap.Setup(titles, lines[0]))
                return false;

            _parser = new Parser(_idxMap);
            _hasDefault = hasDefault;
            _index = lineBeg;
            _lines = lines;
            _fn = fn;

            if (_hasDefault)
            {
                if (!Load(out _default))
                    return false;
            }

            return true;
        }

        public bool Load(out T val)
        {
            val = default(T);
            while (_index < _lines.Length && string.IsNullOrEmpty(_lines[_index]))
                ++_index;   // skip empty line

            if (_index >= _lines.Length)
                return false;

            //TODO: clone default
            val = new T();

            var reader = _parser.Parse(_lines[_index]);
            ++_index;
            if (reader == null)
                return false;

            return _fn(reader, ref val, null);
        }
    }


    public class BinaryLoader<T> : ILoader<T> where T : new()
    {
        int _count = 0;
        int _code = 0;
        Serialize.IReader _reader;
        Serialize.ReadCallback<T> _fn;
        public int Code { get { return _code; } }

        public bool Setup(Serialize.ReadCallback<T> fn, byte[] data)
        {
            var stream = new MemoryStream(data);
            var reader = new BinaryReader(stream);

            _fn = fn;
            _code = reader.ReadInt32();
            _count = reader.ReadInt32();
            _reader = new Serialize.BinaryReader(reader);
            return true;
        }

        public bool Load(out T val)
        {
            val = default(T);
            if (_count == 0)
                return false;

            val = new T();
            --_count;
            return Serialize.Utility.Read(_reader, ref val);
        }
    }
}
