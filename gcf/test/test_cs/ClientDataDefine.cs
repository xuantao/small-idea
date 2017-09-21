using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace DummyJX3MClient
{
    [StructLayoutAttribute(LayoutKind.Sequential, Pack = 4)]
    public struct SkillData
    {
        const int MAX_SKILL_COUNT = 16;

        [StructLayoutAttribute(LayoutKind.Sequential, Pack = 4)]
        public struct SkillInfo
        {
            public long skillID;
            public int skillLevel;

            public SkillInfo(long id, int level)
            {
                skillID = id;
                skillLevel = level;
            }
        }

        [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = MAX_SKILL_COUNT, ArraySubType = UnmanagedType.Struct)]
        public SkillInfo[] skillInfo;

        public SkillData(int _)
        {
            skillInfo = new SkillInfo[MAX_SKILL_COUNT];
        }
    }

    [StructLayoutAttribute(LayoutKind.Sequential, Pack = 4)]
    public struct TalentData
    {
        const int MAX_TALENT_COUNT = 128;

        [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = MAX_TALENT_COUNT, ArraySubType = UnmanagedType.I4)]
        public int[] talent;

        public TalentData(int _)
        {
            talent = new int[MAX_TALENT_COUNT];
        }
    }

    [StructLayoutAttribute(LayoutKind.Sequential, Pack = 4)]
    public struct EquipmentSlotData
    {
        const int MAX_MOUNT_SLOT_COUNT = 3;

        public int level;

        [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = MAX_MOUNT_SLOT_COUNT, ArraySubType = UnmanagedType.I4)]
        public int[] mountItemTypeIds;

        public EquipmentSlotData(int _)
        {
            level = 1;
            mountItemTypeIds = new int[MAX_MOUNT_SLOT_COUNT];
        }
    }

    [StructLayoutAttribute(LayoutKind.Sequential, Pack = 4)]
    public struct EquipmentItemData
    {
        const int MAX_MOUNT_SLOT_COUNT = 3;

        public int typeID;
        public int qualityLevel;
        public int needPlayerLevel;

        [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = MAX_MOUNT_SLOT_COUNT, ArraySubType = UnmanagedType.I4)]
        public int[] mountAttrIds;

        public int randSeed;

        public EquipmentItemData(int _)
        {
            typeID = 0;
            qualityLevel = 0;
            needPlayerLevel = 0;
            mountAttrIds = new int[MAX_MOUNT_SLOT_COUNT];
            randSeed = 0;
        }
    }

    [StructLayoutAttribute(LayoutKind.Sequential, Pack = 4)]
    public struct EquipmentData
    {
        const int MAX_EQUIPMENT_COUNT   = 12;

        [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = MAX_EQUIPMENT_COUNT, ArraySubType = UnmanagedType.Struct)]
        public EquipmentItemData[] equipments;

        [MarshalAsAttribute(UnmanagedType.ByValArray, SizeConst = MAX_EQUIPMENT_COUNT, ArraySubType = UnmanagedType.Struct)]
        public EquipmentSlotData[] slots;

        public EquipmentData(int _)
        {
            equipments = new EquipmentItemData[MAX_EQUIPMENT_COUNT];
            slots = new EquipmentSlotData[MAX_EQUIPMENT_COUNT];
        }
    }

    // 所有的结构大小必须是4的整数倍。
    // 如果不填充满，对齐补足的内存上可能是未知的数据，导致C++那里计算的MD5是未知的
    [StructLayoutAttribute(LayoutKind.Sequential, Pack = 4)]
    public struct PlayerData
    {
        const int MAX_SKILL_COUNT = 16;
        const int MAX_TALENT_COUNT = 16;

        public int forceID;
        public int star;
        public int quality;
        public int level;
        public int exteriorID;
        public int weaponRepresentID;

        [MarshalAsAttribute(UnmanagedType.Struct)]
        public SkillData skill;

        [MarshalAsAttribute(UnmanagedType.Struct)]
        public TalentData talent;

        [MarshalAsAttribute(UnmanagedType.Struct)]
        public EquipmentData equipment;

        public PlayerData(int _)
        {
            forceID = 101;
            star = 102;
            quality = 103;
            level = 104;
            exteriorID = 105;
            weaponRepresentID = 106;

            skill = new SkillData(1);
            talent = new TalentData(1);
            equipment = new EquipmentData(1);
        }
    }
}
