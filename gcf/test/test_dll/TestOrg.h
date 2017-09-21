#pragma once

namespace test
{
    const int MAX_TALENT_COUNT = 128;
    const int MAX_SKILL_COUNT = 16;
    const int MAX_MOUNT_SLOT_COUNT = 3;
    const int MAX_PLAYER_DATA_SHELL_COUNT = 3;
    const int MAX_EQUIPMENT_COUNT = 12;

#pragma pack(push, 4)

    struct SkillLevel
    {
        long m_nSkillID;
        int m_nSkillLevel;
    };

    struct KGSkillData
    {
        SkillLevel m_SkillInfo[MAX_SKILL_COUNT];
    };

    struct KGTalentData
    {
        int m_nTalent[MAX_TALENT_COUNT];
    };

    struct KGEquipmentItemData
    {
        int m_nTypeID;
        int m_nQualityLevel;
        int m_nNeedPlayerLevel;
        int m_MountAttrIds[MAX_MOUNT_SLOT_COUNT];
        int m_nRandSeed;
    };

    struct KGEquipmentSlotData
    {
        int m_nLevel;
        int m_MountItemTypeIds[MAX_MOUNT_SLOT_COUNT];
    };

    struct KGEquipmentData
    {
        KGEquipmentItemData m_EquipmentItemData[MAX_EQUIPMENT_COUNT];
        KGEquipmentSlotData m_EquipmentSlotData[MAX_EQUIPMENT_COUNT];
    };

    struct KGPlayerData
    {
        int m_nForceID;
        int m_nStar;
        int m_nQuality;
        int m_nLevel;
        int m_nExteriorID;
        int m_nWeaponRepresentID;

        KGSkillData     m_SkillData;
        KGTalentData    m_TalentData;
        KGEquipmentData m_EquipmentData;
    };

#pragma pack(pop)
}
