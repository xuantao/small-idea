using System;
using System.Collections.Generic;

namespace Serialize
{
    public static partial class Utility
    {
        public static bool Read(IReader reader, ref bool val, string name = null)
        {
            return reader.Read(ref val, name);
        }

        public static bool Read(IReader reader, ref int val, string name = null)
        {
            return reader.Read(ref val, name);
        }

        public static bool Read(IReader reader, ref uint val, string name = null)
        {
            return reader.Read(ref val, name);
        }

        public static bool Read(IReader reader, ref float val, string name = null)
        {
            return reader.Read(ref val, name);
        }

        public static bool Read(IReader reader, ref string val, string name = null)
        {
            return reader.Read(ref val, name);
        }

        private static bool Read<T>(IReader reader, ref T tmp, string name = null)
        {
            throw new NotImplementedException();
        }

        public static bool Read<T>(IReader reader, ref List<T> lst, string name = null)
        {
            int size = 0;
            if (!reader.ArrayBegin(ref size, name))
                return false;

            for (int i = 0; i < size; ++i)
            {
                T tmp = default(T);
                if (!Read(reader, ref tmp))
                    return false;

                lst.Add(tmp);
            }

            return reader.ArrayEnd();
        }

        public static bool Read<T, L>(IReader reader, ref FixedArray<T, L> arr, string name = null)
            where T : new()
            where L : IArrayLength, new()
        {
            int size = 0;
            if (!reader.ArrayBegin(ref size, name))
                return false;

            for (int i = 0; i < Math.Min(size, arr.Length); ++i)
            {
                if (!Read(reader, ref ((T[])arr)[i]))
                    return false;
            }

            return reader.ArrayEnd();
        }

        public static bool Write(IWriter writer, bool val, string name = null)
        {
            return writer.Write(val, name);
        }

        public static bool Write(IWriter writer, int val, string name = null)
        {
            return writer.Write(val, name);
        }

        public static bool Write(IWriter writer, uint val, string name = null)
        {
            return writer.Write(val, name);
        }

        public static bool Write(IWriter writer, float val, string name = null)
        {
            return writer.Write(val, name);
        }

        public static bool Write(IWriter writer, string val, string name = null)
        {
            return writer.Write(val, name);
        }

        private static bool Write<T>(IWriter writer, T tmp, string name = null)
        {
            throw new NotImplementedException();
        }

        public static bool Write<T>(IWriter writer, List<T> lst, string name = null)
        {
            if (!writer.ArrayBegin(lst.Count, name))
                return false;

            for (int i = 0; i < lst.Count; ++i)
            {
                if (!Write(writer, lst[i]))
                    return false;
            }

            return writer.ArrayEnd();
        }

        public static bool Write<T, L>(IWriter writer, FixedArray<T, L> arr, string name = null)
            where T : new()
            where L : IArrayLength, new()
        {
            if (!writer.ArrayBegin(arr.Length, name))
                return false;

            for (int i = 0; i < arr.Length; ++i)
            {
                if (!Write(writer, arr[i]))
                    return false;
            }

            return writer.ArrayEnd();
        }
    }
}
