using System;
using System.Collections.Generic;

namespace Serialize
{
    public delegate bool ReadCallback<T>(IReader reader, ref T val, string name);
    public delegate bool WritCallback<T>(IWriter writer, T val, string name);

    public static partial class Utility
    {
        public static bool Read(IReader reader, ref bool val, string name = null)
        {
            return reader.Read(ref val, name);
        }

        public static bool Read(IReader reader, ref byte val, string name = null)
        {
            return reader.Read(ref val, name);
        }

        public static bool Read(IReader reader, ref int val, string name = null)
        {
            return reader.Read(ref val, name);
        }

        public static bool Read(IReader reader, ref long val, string name = null)
        {
            return reader.Read(ref val, name);
        }

        public static bool Read(IReader reader, ref float val, string name = null)
        {
            return reader.Read(ref val, name);
        }

        public static bool Read(IReader reader, ref double val, string name = null)
        {
            return reader.Read(ref val, name);
        }

        public static bool Read(IReader reader, ref string val, string name = null)
        {
            return reader.Read(ref val, name);
        }

        public static bool Read<T>(IReader reader, ref T tmp, string name = null)
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
            for (int i = 0; i < arr.Length; ++i)
                if (!Read(reader, ref ((T[])arr)[i]))
                    return false;

            return true;
        }

        public static bool Write(IWriter writer, bool val, string name = null)
        {
            return writer.Write(val, name);
        }

        public static bool Write(IWriter writer, byte val, string name = null)
        {
            return writer.Write(val, name);
        }

        public static bool Write(IWriter writer, int val, string name = null)
        {
            return writer.Write(val, name);
        }

        public static bool Write(IWriter writer, long val, string name = null)
        {
            return writer.Write(val, name);
        }

        public static bool Write(IWriter writer, float val, string name = null)
        {
            return writer.Write(val, name);
        }

        public static bool Write(IWriter writer, double val, string name = null)
        {
            return writer.Write(val, name);
        }

        public static bool Write(IWriter writer, string val, string name = null)
        {
            return writer.Write(val, name);
        }

        public static bool Write<T>(IWriter writer, T val, string name = null)
        {
            throw new NotFiniteNumberException();
        }

        public static bool DoRead<T>(IReader reader, ref List<T> val, string name, ReadCallback<T> call)
            where T : new()
        {
            int size = 0;
            if (!reader.ArrayBegin(ref size, name))
                return false;

            for (int i = 0; i < size; ++i)
            {
                T tmp = new T();
                call(reader, ref tmp, null);
                val.Add(tmp);
            }

            return reader.ArrayEnd();
        }

        public static bool DoRead<T, L>(IReader reader, ref FixedArray<T, L> val, string name, ReadCallback<T> call)
            where T : new()
            where L : IArrayLength, new()
        {
            for (int i = 0; i < val.Length; ++i)
            {
                string valName = string.Format("{0}_{1}", string.IsNullOrEmpty(name) ? "" : name, i);
                call(reader, ref ((T[])val)[i], valName);
            }

            return true;
        }

        public static bool DoWrite<T>(IWriter writer, List<T> val, string name, WritCallback<T> call)
        {
            if (!writer.ArrayBegin(val.Count, name))
                return false;

            for (int i = 0; i < val.Count; ++i)
            {
                call(writer, val[i], null);
            }

            return writer.ArrayEnd();
        }

        public static bool DoWrite<T, L>(IWriter writer, FixedArray<T, L> val, string name, WritCallback<T> call)
            where T : new()
            where L : IArrayLength, new()
        {
            for (int i = 0; i < val.Length; ++i)
            {
                //string valName = string.Format("{0}_{1}", string.IsNullOrEmpty(name) ? "" : name, i);
                call(writer, val[i], null);
            }

            return true;
        }

        public static ReadCallback<T> GetReader<T>()
        {
            return (ReadCallback<T>)Read;
        }
    }
}
