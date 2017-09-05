/*
 * this file is auto generated.
 * please does not edit it manual!
*/
using System;
using System.Collections;
using System.Collections.Generic;

namespace Serialize
{
    public static partial class Utility
    {
        public static bool Read(IReader reader, ref GlobalType val)
        {
            int tmp = 0;
            if (!reader.Read(ref tmp))
                return false;

            val = (GlobalType)tmp;
            return true;
        }

        public static bool Write(IWriter writer, GlobalType val)
        {
            return writer.Write((int)val);
        }

        public static bool Read(IReader reader, ref Enum1 val)
        {
            int tmp = 0;
            if (!reader.Read(ref tmp))
                return false;

            val = (Enum1)tmp;
            return true;
        }

        public static bool Write(IWriter writer, Enum1 val)
        {
            return writer.Write((int)val);
        }

        public static bool Read(IReader reader, ref Detail.Enum2 val)
        {
            int tmp = 0;
            if (!reader.Read(ref tmp))
                return false;

            val = (Detail.Enum2)tmp;
            return true;
        }

        public static bool Write(IWriter writer, Detail.Enum2 val)
        {
            return writer.Write((int)val);
        }

        public static bool Read(IReader reader, ref Struct0 val)
        {
            if (!reader.StructBegin(Struct0.HASH_CODE))
                return false;

            if (val == null)
                val = new Struct0();

            if (!Read(reader, ref val.s0a)) return false;
            if (!Read(reader, ref val.s0b)) return false;
            if (!Read(reader, ref val.s0c)) return false;

            if (!reader.StructEnd())
                return false;

            return true;
        }

        public static bool Write(IWriter writer, Struct0 val)
        {
            if (!writer.StructBegin(Struct0.HASH_CODE))
                return false;

            if (!Write(writer, val.s0a)) return false;
            if (!Write(writer, val.s0b)) return false;
            if (!Write(writer, val.s0c)) return false;

            if (!writer.StructEnd())
                return false;

            return true;
        }

        public static bool Read(IReader reader, ref Struct1 val)
        {
            if (!reader.StructBegin(Struct1.HASH_CODE))
                return false;

            if (val == null)
                val = new Struct1();

            if (!Read(reader, ref val.s0a)) return false;
            if (!Read(reader, ref val.s0b)) return false;
            if (!Read(reader, ref val.s0c)) return false;
            if (!Read(reader, ref val.s1a)) return false;
            if (!Read(reader, ref val.f1)) return false;
            if (!Read(reader, ref val.str2)) return false;
            if (!Read(reader, ref val.str3)) return false;
            if (!Read(reader, ref val.s1)) return false;
            if (!Read(reader, ref val.s2)) return false;
            if (!Read(reader, ref val.s3)) return false;
            if (!Read(reader, ref val.s1arEnum)) return false;
            if (!Read(reader, ref val.s1arEnum2)) return false;

            if (!reader.StructEnd())
                return false;

            return true;
        }

        public static bool Write(IWriter writer, Struct1 val)
        {
            if (!writer.StructBegin(Struct1.HASH_CODE))
                return false;

            if (!Write(writer, val.s0a)) return false;
            if (!Write(writer, val.s0b)) return false;
            if (!Write(writer, val.s0c)) return false;
            if (!Write(writer, val.s1a)) return false;
            if (!Write(writer, val.f1)) return false;
            if (!Write(writer, val.str2)) return false;
            if (!Write(writer, val.str3)) return false;
            if (!Write(writer, val.s1)) return false;
            if (!Write(writer, val.s2)) return false;
            if (!Write(writer, val.s3)) return false;
            if (!Write(writer, val.s1arEnum)) return false;
            if (!Write(writer, val.s1arEnum2)) return false;

            if (!writer.StructEnd())
                return false;

            return true;
        }

        public static bool Read(IReader reader, ref Struct2 val)
        {
            if (!reader.StructBegin(Struct2.HASH_CODE))
                return false;

            if (val == null)
                val = new Struct2();

            if (!Read(reader, ref val.a)) return false;
            if (!Read(reader, ref val.b)) return false;
            if (!Read(reader, ref val.c)) return false;
            if (!Read(reader, ref val.d)) return false;
            if (!Read(reader, ref val.s)) return false;
            if (!Read(reader, ref val.s1)) return false;
            if (!Read(reader, ref val.s2)) return false;
            if (!Read(reader, ref val.s3)) return false;

            if (!reader.StructEnd())
                return false;

            return true;
        }

        public static bool Write(IWriter writer, Struct2 val)
        {
            if (!writer.StructBegin(Struct2.HASH_CODE))
                return false;

            if (!Write(writer, val.a)) return false;
            if (!Write(writer, val.b)) return false;
            if (!Write(writer, val.c)) return false;
            if (!Write(writer, val.d)) return false;
            if (!Write(writer, val.s)) return false;
            if (!Write(writer, val.s1)) return false;
            if (!Write(writer, val.s2)) return false;
            if (!Write(writer, val.s3)) return false;

            if (!writer.StructEnd())
                return false;

            return true;
        }

        public static bool Read(IReader reader, ref Wtf val)
        {
            int tmp = 0;
            if (!reader.Read(ref tmp))
                return false;

            val = (Wtf)tmp;
            return true;
        }

        public static bool Write(IWriter writer, Wtf val)
        {
            return writer.Write((int)val);
        }

        public static bool Read(IReader reader, ref Sr3 val)
        {
            if (!reader.StructBegin(Sr3.HASH_CODE))
                return false;

            if (val == null)
                val = new Sr3();

            if (!Read(reader, ref val.ID)) return false;
            if (!Read(reader, ref val.StageID)) return false;
            if (!Read(reader, ref val.Name)) return false;

            if (!reader.StructEnd())
                return false;

            return true;
        }

        public static bool Write(IWriter writer, Sr3 val)
        {
            if (!writer.StructBegin(Sr3.HASH_CODE))
                return false;

            if (!Write(writer, val.ID)) return false;
            if (!Write(writer, val.StageID)) return false;
            if (!Write(writer, val.Name)) return false;

            if (!writer.StructEnd())
                return false;

            return true;
        }
    }
}

