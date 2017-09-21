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

            Read(reader, ref val._bool, "_bool");
            Read(reader, ref val._byte, "_byte");
            Read(reader, ref val._int, "_int");
            Read(reader, ref val._long, "_long");
            Read(reader, ref val._float, "_float");
            Read(reader, ref val._double, "_double");
            Read(reader, ref val._string, "_string");

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, TabTest_1 val, string name = null)
        {
            if (!writer.StructBegin(TabTest_1.HASH_CODE, name))
                return false;

            Write(writer, val._bool, "_bool");
            Write(writer, val._byte, "_byte");
            Write(writer, val._int, "_int");
            Write(writer, val._long, "_long");
            Write(writer, val._float, "_float");
            Write(writer, val._double, "_double");
            Write(writer, val._string, "_string");

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

            Read(reader, ref val.a, "a");
            Read(reader, ref val.b, "b");

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, Struct0.Innser val, string name = null)
        {
            if (!writer.StructBegin(Struct0.Innser.HASH_CODE, name))
                return false;

            Write(writer, val.a, "a");
            Write(writer, val.b, "b");

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref Struct0 val, string name = null)
        {
            if (!reader.StructBegin(Struct0.HASH_CODE, name))
                return false;

            DoRead(reader, ref val.s0a, "s0a", Read);
            DoRead(reader, ref val.s0b, "s0b", Read);
            DoRead(reader, ref val.s0c, "s0c", Read);

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, Struct0 val, string name = null)
        {
            if (!writer.StructBegin(Struct0.HASH_CODE, name))
                return false;

            DoWrite(writer, val.s0a, "s0a", Write);
            DoWrite(writer, val.s0b, "s0b", Write);
            DoWrite(writer, val.s0c, "s0c", Write);

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref Struct1 val, string name = null)
        {
            if (!reader.StructBegin(Struct1.HASH_CODE, name))
                return false;

            DoRead(reader, ref val.s0a, "s0a", Read);
            DoRead(reader, ref val.s0b, "s0b", Read);
            DoRead(reader, ref val.s0c, "s0c", Read);
            Read(reader, ref val.s1a, "s1a");
            Read(reader, ref val.f1, "f1");
            Read(reader, ref val.str2, "str2");
            Read(reader, ref val.str3, "str3");
            Read(reader, ref val.s1, "s1");
            DoRead(reader, ref val.s2, "s2", Read);
            DoRead(reader, ref val.s3, "s3", Read);
            DoRead(reader, ref val.s1arEnum, "s1arEnum", Read);
            DoRead(reader, ref val.s1arEnum2, "s1arEnum2", Read);

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, Struct1 val, string name = null)
        {
            if (!writer.StructBegin(Struct1.HASH_CODE, name))
                return false;

            DoWrite(writer, val.s0a, "s0a", Write);
            DoWrite(writer, val.s0b, "s0b", Write);
            DoWrite(writer, val.s0c, "s0c", Write);
            Write(writer, val.s1a, "s1a");
            Write(writer, val.f1, "f1");
            Write(writer, val.str2, "str2");
            Write(writer, val.str3, "str3");
            Write(writer, val.s1, "s1");
            DoWrite(writer, val.s2, "s2", Write);
            DoWrite(writer, val.s3, "s3", Write);
            DoWrite(writer, val.s1arEnum, "s1arEnum", Write);
            DoWrite(writer, val.s1arEnum2, "s1arEnum2", Write);

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref Struct2 val, string name = null)
        {
            if (!reader.StructBegin(Struct2.HASH_CODE, name))
                return false;

            Read(reader, ref val.a, "a");
            Read(reader, ref val.b, "b");
            Read(reader, ref val.c, "c");
            Read(reader, ref val.d, "d");
            Read(reader, ref val.s, "s");
            Read(reader, ref val.s1, "s1");
            DoRead(reader, ref val.s2, "s2", Read);
            DoRead(reader, ref val.s3, "s3", Read);

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, Struct2 val, string name = null)
        {
            if (!writer.StructBegin(Struct2.HASH_CODE, name))
                return false;

            Write(writer, val.a, "a");
            Write(writer, val.b, "b");
            Write(writer, val.c, "c");
            Write(writer, val.d, "d");
            Write(writer, val.s, "s");
            Write(writer, val.s1, "s1");
            DoWrite(writer, val.s2, "s2", Write);
            DoWrite(writer, val.s3, "s3", Write);

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref Inner val, string name = null)
        {
            if (!reader.StructBegin(Inner.HASH_CODE, name))
                return false;

            Read(reader, ref val.ii, "ii");
            Read(reader, ref val.name, "name");

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, Inner val, string name = null)
        {
            if (!writer.StructBegin(Inner.HASH_CODE, name))
                return false;

            Write(writer, val.ii, "ii");
            Write(writer, val.name, "name");

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref SkillLevel val, string name = null)
        {
            if (!reader.StructBegin(SkillLevel.HASH_CODE, name))
                return false;

            Read(reader, ref val.m_nSkillID, "m_nSkillID");
            Read(reader, ref val.m_nSkillLevel, "m_nSkillLevel");

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, SkillLevel val, string name = null)
        {
            if (!writer.StructBegin(SkillLevel.HASH_CODE, name))
                return false;

            Write(writer, val.m_nSkillID, "m_nSkillID");
            Write(writer, val.m_nSkillLevel, "m_nSkillLevel");

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref KGSkillData val, string name = null)
        {
            if (!reader.StructBegin(KGSkillData.HASH_CODE, name))
                return false;

            DoRead(reader, ref val.m_SkillInfo, "m_SkillInfo", Read);

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, KGSkillData val, string name = null)
        {
            if (!writer.StructBegin(KGSkillData.HASH_CODE, name))
                return false;

            DoWrite(writer, val.m_SkillInfo, "m_SkillInfo", Write);

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref KGTalentData val, string name = null)
        {
            if (!reader.StructBegin(KGTalentData.HASH_CODE, name))
                return false;

            DoRead(reader, ref val.m_nTalent, "m_nTalent", Read);

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, KGTalentData val, string name = null)
        {
            if (!writer.StructBegin(KGTalentData.HASH_CODE, name))
                return false;

            DoWrite(writer, val.m_nTalent, "m_nTalent", Write);

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref KGEquipmentItemData val, string name = null)
        {
            if (!reader.StructBegin(KGEquipmentItemData.HASH_CODE, name))
                return false;

            Read(reader, ref val.m_nTypeID, "m_nTypeID");
            Read(reader, ref val.m_nQualityLevel, "m_nQualityLevel");
            Read(reader, ref val.m_nNeedPlayerLevel, "m_nNeedPlayerLevel");
            DoRead(reader, ref val.m_MountAttrIds, "m_MountAttrIds", Read);
            Read(reader, ref val.m_nRandSeed, "m_nRandSeed");

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, KGEquipmentItemData val, string name = null)
        {
            if (!writer.StructBegin(KGEquipmentItemData.HASH_CODE, name))
                return false;

            Write(writer, val.m_nTypeID, "m_nTypeID");
            Write(writer, val.m_nQualityLevel, "m_nQualityLevel");
            Write(writer, val.m_nNeedPlayerLevel, "m_nNeedPlayerLevel");
            DoWrite(writer, val.m_MountAttrIds, "m_MountAttrIds", Write);
            Write(writer, val.m_nRandSeed, "m_nRandSeed");

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref KGEquipmentSlotData val, string name = null)
        {
            if (!reader.StructBegin(KGEquipmentSlotData.HASH_CODE, name))
                return false;

            Read(reader, ref val.m_nLevel, "m_nLevel");
            DoRead(reader, ref val.m_MountItemTypeIds, "m_MountItemTypeIds", Read);

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, KGEquipmentSlotData val, string name = null)
        {
            if (!writer.StructBegin(KGEquipmentSlotData.HASH_CODE, name))
                return false;

            Write(writer, val.m_nLevel, "m_nLevel");
            DoWrite(writer, val.m_MountItemTypeIds, "m_MountItemTypeIds", Write);

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref KGEquipmentData val, string name = null)
        {
            if (!reader.StructBegin(KGEquipmentData.HASH_CODE, name))
                return false;

            DoRead(reader, ref val.m_EquipmentItemData, "m_EquipmentItemData", Read);
            DoRead(reader, ref val.m_EquipmentSlotData, "m_EquipmentSlotData", Read);

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, KGEquipmentData val, string name = null)
        {
            if (!writer.StructBegin(KGEquipmentData.HASH_CODE, name))
                return false;

            DoWrite(writer, val.m_EquipmentItemData, "m_EquipmentItemData", Write);
            DoWrite(writer, val.m_EquipmentSlotData, "m_EquipmentSlotData", Write);

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref KGPlayerData val, string name = null)
        {
            if (!reader.StructBegin(KGPlayerData.HASH_CODE, name))
                return false;

            Read(reader, ref val.m_nForceID, "m_nForceID");
            Read(reader, ref val.m_nStar, "m_nStar");
            Read(reader, ref val.m_nQuality, "m_nQuality");
            Read(reader, ref val.m_nLevel, "m_nLevel");
            Read(reader, ref val.m_nExteriorID, "m_nExteriorID");
            Read(reader, ref val.m_nWeaponRepresentID, "m_nWeaponRepresentID");
            Read(reader, ref val.m_SkillData, "m_SkillData");
            Read(reader, ref val.m_TalentData, "m_TalentData");
            Read(reader, ref val.m_EquipmentData, "m_EquipmentData");

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, KGPlayerData val, string name = null)
        {
            if (!writer.StructBegin(KGPlayerData.HASH_CODE, name))
                return false;

            Write(writer, val.m_nForceID, "m_nForceID");
            Write(writer, val.m_nStar, "m_nStar");
            Write(writer, val.m_nQuality, "m_nQuality");
            Write(writer, val.m_nLevel, "m_nLevel");
            Write(writer, val.m_nExteriorID, "m_nExteriorID");
            Write(writer, val.m_nWeaponRepresentID, "m_nWeaponRepresentID");
            Write(writer, val.m_SkillData, "m_SkillData");
            Write(writer, val.m_TalentData, "m_TalentData");
            Write(writer, val.m_EquipmentData, "m_EquipmentData");

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref Msg.Inner val, string name = null)
        {
            if (!reader.StructBegin(Msg.Inner.HASH_CODE, name))
                return false;

            Read(reader, ref val.idx, "idx");
            Read(reader, ref val.name, "name");

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, Msg.Inner val, string name = null)
        {
            if (!writer.StructBegin(Msg.Inner.HASH_CODE, name))
                return false;

            Write(writer, val.idx, "idx");
            Write(writer, val.name, "name");

            return writer.StructEnd();
        }

        public static bool Read(IReader reader, ref Msg val, string name = null)
        {
            if (!reader.StructBegin(Msg.HASH_CODE, name))
                return false;

            Read(reader, ref val._bool, "_bool");
            Read(reader, ref val._byte, "_byte");
            Read(reader, ref val._int, "_int");
            Read(reader, ref val._long, "_long");
            Read(reader, ref val._float, "_float");
            Read(reader, ref val._double, "_double");
            Read(reader, ref val._string, "_string");
            DoRead(reader, ref val._int_ary, "_int_ary", Read);
            DoRead(reader, ref val._int_ary_2, "_int_ary_2", Read);
            Read(reader, ref val._inner, "_inner");
            DoRead(reader, ref val._inner_ary, "_inner_ary", Read);
            DoRead(reader, ref val._inner_ary_2, "_inner_ary_2", Read);

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, Msg val, string name = null)
        {
            if (!writer.StructBegin(Msg.HASH_CODE, name))
                return false;

            Write(writer, val._bool, "_bool");
            Write(writer, val._byte, "_byte");
            Write(writer, val._int, "_int");
            Write(writer, val._long, "_long");
            Write(writer, val._float, "_float");
            Write(writer, val._double, "_double");
            Write(writer, val._string, "_string");
            DoWrite(writer, val._int_ary, "_int_ary", Write);
            DoWrite(writer, val._int_ary_2, "_int_ary_2", Write);
            Write(writer, val._inner, "_inner");
            DoWrite(writer, val._inner_ary, "_inner_ary", Write);
            DoWrite(writer, val._inner_ary_2, "_inner_ary_2", Write);

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

            Read(reader, ref val.ID, "ID");
            Read(reader, ref val.StageID, "StageID");
            Read(reader, ref val.Name, "Name");

            return reader.StructEnd();
        }

        public static bool Write(IWriter writer, Sr3 val, string name = null)
        {
            if (!writer.StructBegin(Sr3.HASH_CODE, name))
                return false;

            Write(writer, val.ID, "ID");
            Write(writer, val.StageID, "StageID");
            Write(writer, val.Name, "Name");

            return writer.StructEnd();
        }
    }
}

