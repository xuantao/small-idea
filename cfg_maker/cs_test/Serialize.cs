using System;
using System.Collections.Generic;

namespace Serialize
{
    public interface IReader
    {
        bool StructBegin(uint code);
        bool StructEnd();
        bool Read(ref bool val);
        bool Read(ref int val);
        bool Read(ref uint val);
        bool Read(ref float val);
        bool Read(ref string val);
    }

    public interface IWriter
    {
        bool StructBegin(uint code);
        bool StructEnd();
        bool Write(bool val);
        bool Write(int val);
        bool Write(uint val);
        bool Write(float val);
        bool Write(string val);
    }

    public static partial class Utility
    {
        public static bool Read(IReader reader, ref bool val)
        {
            return reader.Read(ref val);
        }

        public static bool Read(IReader reader, ref int val)
        {
            return reader.Read(ref val);
        }

        public static bool Read(IReader reader, ref uint val)
        {
            return reader.Read(ref val);
        }

        public static bool Read(IReader reader, ref float val)
        {
            return reader.Read(ref val);
        }

        public static bool Read(IReader reader, ref string val)
        {
            return reader.Read(ref val);
        }

        private static bool Read<T>(IReader reader, ref T tmp)
        {
            throw new NotImplementedException();
        }

        public static bool Read<T>(IReader reader, ref List<T> lst)
        {
            int size = 0;
            if (!Read(reader, ref size)) return false;
            if (size < 0) return false;

            if (lst == null)
                lst = new List<T>(size);

            for (int i = lst.Count; i < size; ++i)
                lst.Add(default(T));

            for (int i = 0; i < size; ++i)
            {
                T tmp = default(T);
                if (!Read(reader, ref tmp))
                    return false;

                lst[i] = tmp;
            }

            return true;
        }

        public static bool Read<T>(IReader reader, ref T[] arr)
        {
            int size = 0;
            if (!Read(reader, ref size)) return false;
            if (size < 0) return false;

            if (arr == null || arr.Length != size)
                arr = new T[size];

            for (int i = 0; i < size; ++i)
            {
                if (!Read(reader, ref arr[i]))
                    return false;
            }

            return true;
        }
    }
}
