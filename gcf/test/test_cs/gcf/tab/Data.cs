using System.Collections.Generic;

namespace Tab
{
    public abstract class DataBase<T> where T : new()
    {
        public bool LoadText(Info info, Serialize.ReadCallback<T> fn, string file, int startLine, bool hasDefault = true)
        {
            var lines = System.IO.File.ReadAllLines(file);
            if (lines == null || lines.Length == 0)
                return false;

            return LoadText(info, fn, lines, startLine, hasDefault);
        }

        public bool LoadText(Info info, Serialize.ReadCallback<T> fn, string[] lines, int startLine, bool hasDefault = true)
        {
            if (info == null)
                return false;

            var loader = new TextLoader<T>();
            if (!loader.Setup(fn, info.Titles, lines, startLine, hasDefault))
                return false;

            return Load(loader);
        }

        public bool LoadBinary(Serialize.ReadCallback<T> fn, string file, int hashCode = 0)
        {
            var bytes = System.IO.File.ReadAllBytes(file);
            if (bytes == null || bytes.Length == 0)
                return false;

            return LoadBinary(fn, bytes);
        }

        public bool LoadBinary(Serialize.ReadCallback<T> fn, byte[] data, int hashCode = 0)
        {
            var loader = new BinaryLoader<T>();
            if (!loader.Setup(fn, data))
                return false;

            if (hashCode != 0 && loader.Code != hashCode)
                return false;

            return Load(loader);
        }

        protected bool Load(ILoader<T> loader)
        {
            while (loader.Load(out T val))
            {
                if (!OnLoad(val))
                    return false;
            }

            return true;
        }

        protected abstract bool OnLoad(T val);
    }


    public class DataList<T> : DataBase<T> where T : new()
    {
        protected List<T> _datas = new List<T>();

        public List<T> Datas { get { return _datas; } }

        protected override bool OnLoad(T val)
        {
            if (Filter(val))
                _datas.Add(val);
            return true;
        }

        protected virtual bool Filter(T val) { return true; }
    }

    public abstract class DataMap<K, T> : DataBase<T> where T : new()
    {
        protected Dictionary<K, T> _datas = new Dictionary<K, T>();

        public Dictionary<K, T> Datas { get { return _datas; } }

        public T Get(K key)
        {
            _datas.TryGetValue(key, out T val);
            return val;
        }

        protected override bool OnLoad(T val)
        {
            if (Filter(val))
                _datas.Add(Key(val), val);
            return true;
        }

        protected virtual bool Filter(T val) { return true; }
        protected abstract K Key(T Val);
    }
}
