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

        //private static bool Write(IWriter writer, object obj, string name = null)
        //{
        //    if (obj is bool)
        //        return Write(writer, (bool)obj, name);
        //    else if (obj is byte)
        //        return Write(writer, (byte)obj, name);
        //    else if (obj is int)
        //        return Write(writer, (int)obj, name);
        //    else if (obj is long)
        //        return Write(writer, (long)obj, name);
        //    else if (obj is float)
        //        return Write(writer, (float)obj, name);
        //    else if (obj is double)
        //        return Write(writer, (double)obj, name);
        //    else if (obj is string)
        //        return Write(writer, (string)obj, name);
        //    else
        //        throw new NotImplementedException();
        //    return false;
        //}

        public static bool Write<T>(IWriter writer, List<T> lst, string name = null)
        {
            if (!writer.ArrayBegin(lst.Count, name))
                return false;

            for (int i = 0; i < lst.Count; ++i)
            {
                if (!SWrite(writer, lst[i], null))
                    return false;
            }

            return writer.ArrayEnd();
        }

        public static bool Write<T, L>(IWriter writer, FixedArray<T, L> arr, string name = null)
            where T : new()
            where L : IArrayLength, new()
        {
            for (int i = 0; i < arr.Length; ++i)
                if (!SWrite(writer, arr[i], null))
                    return false;

            return true;
        }

        private static bool SWrite<T>(IWriter writer, T obj, string name = null)
        {
            return Write(writer, obj, name);
        }

        private static bool Write<T>(IWriter writer, T obj, string name = null)
        {
            //throw new NotImplementedException();
            //if (obj is IList<>)
            //    ;
            return true;
        }

        //private static bool SWrite(IWriter writer, object obj, string name = null)
        //{
        //    //object obj = tmp;
        //    if (obj is bool)
        //        return Write(writer, (bool)obj, name);
        //    else if (obj is byte)
        //        return Write(writer, (byte)obj, name);
        //    else if (obj is int)
        //        return Write(writer, (int)obj, name);
        //    else if (obj is long)
        //        return Write(writer, (long)obj, name);
        //    else if (obj is float)
        //        return Write(writer, (float)obj, name);
        //    else if (obj is double)
        //        return Write(writer, (double)obj, name);
        //    else if (obj is string)
        //        return Write(writer, (string)obj, name);
        //    //else if (obj is List<T>)
        //    //    return Write<T>(writer, obj as List<T>, name);
        //    else
        //        throw new NotImplementedException();
        //}

        //private static bool SWrite<T>(IWriter writer, List<T> lst, string name = null)
        //{
        //    return Write(writer, lst, name);
        //}

        //private static bool SWrite<T, L>(IWriter writer, FixedArray<T, L> arr, string name = null)
        //    where T : new()
        //    where L : IArrayLength, new()
        //{
        //    return Write(writer, arr, name);
        //}
    }
}
