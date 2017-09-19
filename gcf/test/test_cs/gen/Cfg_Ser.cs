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
        public static bool Read(IReader reader, ref GlobalType val, string name = null)
        {
            int tmp = 0;
            if (!reader.Read(ref tmp, name))
                return false;

            val = (GlobalType)tmp;
            return true;
        }

        public static bool Write(IWriter writer, GlobalType val, string name = null)
        {
            return writer.Write((int)val, name);
        }

        public static bool Read(IReader reader, ref TabTest_1 val, string name = null)
        {
            if (!reader.StructBegin(TabTest_1.HASH_CODE, name))
                return false;

            if (!Read(reader, ref val._bool, "_bool")) return false;
            if (!Read(reader, ref val._byte, "_byte")) return false;
            if (!Read(reader, ref val._int, "_int")) return false;
            if (!Read(reader, ref val._long, "_long")) return false;
            if (!Read(reader, ref val._float, "_float")) return false;
            if (!Read(reader, ref val._double, "_double")) return false;
            if (!Read(reader, ref val._string, "_string")) return false;

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, TabTest_1 val, string name = null)
        {
            if (!writer.StructBegin(TabTest_1.HASH_CODE, name))
                return false;

            if (!Write(writer, val._bool, "_bool")) return false;
            if (!Write(writer, val._byte, "_byte")) return false;
            if (!Write(writer, val._int, "_int")) return false;
            if (!Write(writer, val._long, "_long")) return false;
            if (!Write(writer, val._float, "_float")) return false;
            if (!Write(writer, val._double, "_double")) return false;
            if (!Write(writer, val._string, "_string")) return false;

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref Enum1 val, string name = null)
        {
            int tmp = 0;
            if (!reader.Read(ref tmp, name))
                return false;

            val = (Enum1)tmp;
            return true;
        }

        public static bool Write(IWriter writer, Enum1 val, string name = null)
        {
            return writer.Write((int)val, name);
        }

        public static bool Read(IReader reader, ref Detail.Enum2 val, string name = null)
        {
            int tmp = 0;
            if (!reader.Read(ref tmp, name))
                return false;

            val = (Detail.Enum2)tmp;
            return true;
        }

        public static bool Write(IWriter writer, Detail.Enum2 val, string name = null)
        {
            return writer.Write((int)val, name);
        }

        public static bool Read(IReader reader, ref Struct0.Innser val, string name = null)
        {
            if (!reader.StructBegin(Struct0.Innser.HASH_CODE, name))
                return false;

            if (!Read(reader, ref val.a, "a")) return false;
            if (!Read(reader, ref val.b, "b")) return false;

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, Struct0.Innser val, string name = null)
        {
            if (!writer.StructBegin(Struct0.Innser.HASH_CODE, name))
                return false;

            if (!Write(writer, val.a, "a")) return false;
            if (!Write(writer, val.b, "b")) return false;

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref Struct0 val, string name = null)
        {
            if (!reader.StructBegin(Struct0.HASH_CODE, name))
                return false;

            if (!Read(reader, ref val.s0a, "s0a")) return false;
            if (!Read(reader, ref val.s0b, "s0b")) return false;
            if (!Read(reader, ref val.s0c, "s0c")) return false;

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, Struct0 val, string name = null)
        {
            if (!writer.StructBegin(Struct0.HASH_CODE, name))
                return false;

            if (!Write(writer, val.s0a, "s0a")) return false;
            if (!Write(writer, val.s0b, "s0b")) return false;
            if (!Write(writer, val.s0c, "s0c")) return false;

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref Struct1 val, string name = null)
        {
            if (!reader.StructBegin(Struct1.HASH_CODE, name))
                return false;

            if (!Read(reader, ref val.s0a, "s0a")) return false;
            if (!Read(reader, ref val.s0b, "s0b")) return false;
            if (!Read(reader, ref val.s0c, "s0c")) return false;
            if (!Read(reader, ref val.s1a, "s1a")) return false;
            if (!Read(reader, ref val.f1, "f1")) return false;
            if (!Read(reader, ref val.str2, "str2")) return false;
            if (!Read(reader, ref val.str3, "str3")) return false;
            if (!Read(reader, ref val.s1, "s1")) return false;
            if (!Read(reader, ref val.s2, "s2")) return false;
            if (!Read(reader, ref val.s3, "s3")) return false;
            if (!Read(reader, ref val.s1arEnum, "s1arEnum")) return false;
            if (!Read(reader, ref val.s1arEnum2, "s1arEnum2")) return false;

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, Struct1 val, string name = null)
        {
            if (!writer.StructBegin(Struct1.HASH_CODE, name))
                return false;

            if (!Write(writer, val.s0a, "s0a")) return false;
            if (!Write(writer, val.s0b, "s0b")) return false;
            if (!Write(writer, val.s0c, "s0c")) return false;
            if (!Write(writer, val.s1a, "s1a")) return false;
            if (!Write(writer, val.f1, "f1")) return false;
            if (!Write(writer, val.str2, "str2")) return false;
            if (!Write(writer, val.str3, "str3")) return false;
            if (!Write(writer, val.s1, "s1")) return false;
            if (!Write(writer, val.s2, "s2")) return false;
            if (!Write(writer, val.s3, "s3")) return false;
            if (!Write(writer, val.s1arEnum, "s1arEnum")) return false;
            if (!Write(writer, val.s1arEnum2, "s1arEnum2")) return false;

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref Struct2 val, string name = null)
        {
            if (!reader.StructBegin(Struct2.HASH_CODE, name))
                return false;

            if (!Read(reader, ref val.a, "a")) return false;
            if (!Read(reader, ref val.b, "b")) return false;
            if (!Read(reader, ref val.c, "c")) return false;
            if (!Read(reader, ref val.d, "d")) return false;
            if (!Read(reader, ref val.s, "s")) return false;
            if (!Read(reader, ref val.s1, "s1")) return false;
            if (!Read(reader, ref val.s2, "s2")) return false;
            if (!Read(reader, ref val.s3, "s3")) return false;

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, Struct2 val, string name = null)
        {
            if (!writer.StructBegin(Struct2.HASH_CODE, name))
                return false;

            if (!Write(writer, val.a, "a")) return false;
            if (!Write(writer, val.b, "b")) return false;
            if (!Write(writer, val.c, "c")) return false;
            if (!Write(writer, val.d, "d")) return false;
            if (!Write(writer, val.s, "s")) return false;
            if (!Write(writer, val.s1, "s1")) return false;
            if (!Write(writer, val.s2, "s2")) return false;
            if (!Write(writer, val.s3, "s3")) return false;

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref Inner val, string name = null)
        {
            if (!reader.StructBegin(Inner.HASH_CODE, name))
                return false;

            if (!Read(reader, ref val.ii, "ii")) return false;
            if (!Read(reader, ref val.name, "name")) return false;

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, Inner val, string name = null)
        {
            if (!writer.StructBegin(Inner.HASH_CODE, name))
                return false;

            if (!Write(writer, val.ii, "ii")) return false;
            if (!Write(writer, val.name, "name")) return false;

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref Msg.Inner val, string name = null)
        {
            if (!reader.StructBegin(Msg.Inner.HASH_CODE, name))
                return false;

            if (!Read(reader, ref val.idx, "idx")) return false;
            if (!Read(reader, ref val.name, "name")) return false;

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, Msg.Inner val, string name = null)
        {
            if (!writer.StructBegin(Msg.Inner.HASH_CODE, name))
                return false;

            if (!Write(writer, val.idx, "idx")) return false;
            if (!Write(writer, val.name, "name")) return false;

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref Msg val, string name = null)
        {
            if (!reader.StructBegin(Msg.HASH_CODE, name))
                return false;

            if (!Read(reader, ref val._bool, "_bool")) return false;
            if (!Read(reader, ref val._byte, "_byte")) return false;
            if (!Read(reader, ref val._int, "_int")) return false;
            if (!Read(reader, ref val._long, "_long")) return false;
            if (!Read(reader, ref val._float, "_float")) return false;
            if (!Read(reader, ref val._double, "_double")) return false;
            if (!Read(reader, ref val._string, "_string")) return false;
            if (!Read(reader, ref val._int_ary, "_int_ary")) return false;
            if (!Read(reader, ref val._int_ary_2, "_int_ary_2")) return false;
            if (!Read(reader, ref val._inner, "_inner")) return false;
            if (!Read(reader, ref val._inner_ary, "_inner_ary")) return false;
            if (!Read(reader, ref val._inner_ary_2, "_inner_ary_2")) return false;

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, Msg val, string name = null)
        {
            if (!writer.StructBegin(Msg.HASH_CODE, name))
                return false;

            if (!Write(writer, val._bool, "_bool")) return false;
            if (!Write(writer, val._byte, "_byte")) return false;
            if (!Write(writer, val._int, "_int")) return false;
            if (!Write(writer, val._long, "_long")) return false;
            if (!Write(writer, val._float, "_float")) return false;
            if (!Write(writer, val._double, "_double")) return false;
            if (!Write(writer, val._string, "_string")) return false;
            if (!Write(writer, val._int_ary, "_int_ary")) return false;
            if (!Write(writer, val._int_ary_2, "_int_ary_2")) return false;
            if (!Write(writer, val._inner, "_inner")) return false;
            if (!Write(writer, val._inner_ary, "_inner_ary")) return false;
            if (!Write(writer, val._inner_ary_2, "_inner_ary_2")) return false;

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref Wtf val, string name = null)
        {
            int tmp = 0;
            if (!reader.Read(ref tmp, name))
                return false;

            val = (Wtf)tmp;
            return true;
        }

        public static bool Write(IWriter writer, Wtf val, string name = null)
        {
            return writer.Write((int)val, name);
        }

        public static bool Read(IReader reader, ref Sr3 val, string name = null)
        {
            if (!reader.StructBegin(Sr3.HASH_CODE, name))
                return false;

            if (!Read(reader, ref val.ID, "ID")) return false;
            if (!Read(reader, ref val.StageID, "StageID")) return false;
            if (!Read(reader, ref val.Name, "Name")) return false;

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, Sr3 val, string name = null)
        {
            if (!writer.StructBegin(Sr3.HASH_CODE, name))
                return false;

            if (!Write(writer, val.ID, "ID")) return false;
            if (!Write(writer, val.StageID, "StageID")) return false;
            if (!Write(writer, val.Name, "Name")) return false;

            return writer.StructEnd();
        }
    }
}

