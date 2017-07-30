/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#include "Cfg.h"
#include "CfgUtility.h"
#include "ITabFile.h"

using namespace cfg;

namespace Enum
{
    static const int s_Enum1_val[] = {
        0, 0, CFG_INVALID_ENUM }; // end of enum Enum1
    static const char* const  s_Enum1_str[] = {
        "a", "b", nullptr }; // end of enum Enum1

    static const int s_Enum2_val[] = {
        100, 101, CFG_INVALID_ENUM }; // end of enum Enum2
    static const char* const  s_Enum2_str[] = {
        "c", "d", nullptr }; // end of enum Enum2

    static const int s_GlobalType_val[] = {
        -1, 0, 1, 2, 3, CFG_INVALID_ENUM }; // end of enum GlobalType
    static const char* const  s_GlobalType_str[] = {
        "Invalid", "Bool", "Int", "Float", "String", nullptr }; // end of enum GlobalType

    const char* const * Names(Enum1)
    {
        return s_Enum1_str;
    }

    const char* ToString(Enum1 value)
    {
        for (int i = 0; s_Enum1_str[i]; ++i)
        {
            if (s_Enum1_val[i] == value)
                return s_Enum1_str[i];
        }

        return nullptr;
    }

    bool ToEnum(const char* name, Enum1& out)
    {
        for (int i = 0; s_Enum1_str[i]; ++i)
        {
            if (std::strcmp(s_Enum1_str[i], name) == 0)
            {
                out = (Enum1)s_Enum1_val[i];
                return true;
            }
        }

        return false;
    }

    const char* const * Names(Enum2)
    {
        return s_Enum2_str;
    }

    const char* ToString(Enum2 value)
    {
        for (int i = 0; s_Enum2_str[i]; ++i)
        {
            if (s_Enum2_val[i] == value)
                return s_Enum2_str[i];
        }

        return nullptr;
    }

    bool ToEnum(const char* name, Enum2& out)
    {
        for (int i = 0; s_Enum2_str[i]; ++i)
        {
            if (std::strcmp(s_Enum2_str[i], name) == 0)
            {
                out = (Enum2)s_Enum2_val[i];
                return true;
            }
        }

        return false;
    }

    const char* const * Names(GlobalType)
    {
        return s_GlobalType_str;
    }

    const char* ToString(GlobalType value)
    {
        for (int i = 0; s_GlobalType_str[i]; ++i)
        {
            if (s_GlobalType_val[i] == value)
                return s_GlobalType_str[i];
        }

        return nullptr;
    }

    bool ToEnum(const char* name, GlobalType& out)
    {
        for (int i = 0; s_GlobalType_str[i]; ++i)
        {
            if (std::strcmp(s_GlobalType_str[i], name) == 0)
            {
                out = (GlobalType)s_GlobalType_val[i];
                return true;
            }
        }

        return false;
    }
} // end of namespace Enum

namespace Tab
{
    static bool Load(ITabFile* pTab, int line, Struct0& out)
    {
        char cBuffer[CFG_STR_BUFFER_SIZE] = {0};
        std::vector<std::string> vecStr;

        pTab->GetString(line, "s0a" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        out.s0a.resize(vecStr.size());
        for (size_t i = 0; i < vecStr.size(); ++i)
        {
            if (!utility::Convert(vecStr[i].c_str(), out.s0a[i]))
                ; //TODO: log error
        }
        pTab->GetString(line, "s0b" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        for (size_t i = 0; i < std::min(vecStr.size(), out.s0b.size()); ++i)
        {
            if (!utility::Convert(vecStr[i].c_str(), out.s0b[i]))
                ; //TODO: log error
        }
        pTab->GetString(line, "s0c" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        for (size_t i = 0; i < std::min(vecStr.size(), out.s0c.size()); ++i)
        {
            if (!utility::Convert(vecStr[i].c_str(), out.s0c[i]))
                ; //TODO: log error
        }
        return true;
    }

    int Load(ITabFile* pTab, std::vector<Struct0>& out)
    {
        size_t prev = out.size();
        if (pTab == nullptr || pTab->GetHeight() <= 4) 
            return 0;

        // load default data
        Struct0 def;
        Load(pTab, 4, def);

        for (int i = 5; i < pTab->GetHeight(); ++i)
        {
            Struct0 data = def;
            if (Load(pTab, i, data))
                out.push_back(data);
        }

        return out.size() - prev;
    }

    static bool Load(ITabFile* pTab, int line, Struct1& out)
    {
        int nTemp = 0;
        char cBuffer[CFG_STR_BUFFER_SIZE] = {0};
        std::vector<std::string> vecStr;

        pTab->GetString(line, "s0a" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        out.s0a.resize(vecStr.size());
        for (size_t i = 0; i < vecStr.size(); ++i)
        {
            if (!utility::Convert(vecStr[i].c_str(), out.s0a[i]))
                ; //TODO: log error
        }
        pTab->GetString(line, "s0b" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        for (size_t i = 0; i < std::min(vecStr.size(), out.s0b.size()); ++i)
        {
            if (!utility::Convert(vecStr[i].c_str(), out.s0b[i]))
                ; //TODO: log error
        }
        pTab->GetString(line, "s0c" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        for (size_t i = 0; i < std::min(vecStr.size(), out.s0c.size()); ++i)
        {
            if (!utility::Convert(vecStr[i].c_str(), out.s0c[i]))
                ; //TODO: log error
        }
        pTab->GetInteger(line, "s1a" , out.s1a, &out.s1a);
        pTab->GetInteger(line, "s1b" , out.s1b, &out.s1b);
        pTab->GetInteger(line, "s1c" , out.s1c, &out.s1c);
        pTab->GetString(line, "s1arEnum" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        out.s1arEnum.resize(vecStr.size());
        for (size_t i = 0; i < vecStr.size(); ++i)
        {
            if (!Enum::ToEnum(vecStr[i].c_str(), out.s1arEnum[i]))
            {
                if (utility::Convert(vecStr[i].c_str(), nTemp) && Enum::ToString((Enum2)nTemp))
                    out.s1arEnum[i] = (Enum2)nTemp;
                else
                    ; //TODO: log error
            }
        }
        pTab->GetString(line, "s1arEnum2" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        for (size_t i = 0; i < std::min(vecStr.size(), out.s1arEnum2.size()); ++i)
        {
            if (!Enum::ToEnum(vecStr[i].c_str(), out.s1arEnum2[i]))
            {
                if (utility::Convert(vecStr[i].c_str(), nTemp) && Enum::ToString((Enum2)nTemp))
                    out.s1arEnum2[i] = (Enum2)nTemp;
                else
                    ; //TODO: log error
            }
        }
        return true;
    }

    int Load(ITabFile* pTab, std::vector<Struct1>& out)
    {
        size_t prev = out.size();
        if (pTab == nullptr || pTab->GetHeight() <= 4) 
            return 0;

        // load default data
        Struct1 def;
        Load(pTab, 4, def);

        for (int i = 5; i < pTab->GetHeight(); ++i)
        {
            Struct1 data = def;
            if (Load(pTab, i, data))
                out.push_back(data);
        }

        return out.size() - prev;
    }

    static bool Load(ITabFile* pTab, int line, Struct2& out)
    {
        int nTemp = 0;
        char cBuffer[CFG_STR_BUFFER_SIZE] = {0};
        std::vector<std::string> vecStr;

        pTab->GetString(line, "s0a" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        out.s0a.resize(vecStr.size());
        for (size_t i = 0; i < vecStr.size(); ++i)
        {
            if (!utility::Convert(vecStr[i].c_str(), out.s0a[i]))
                ; //TODO: log error
        }
        pTab->GetString(line, "s0b" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        for (size_t i = 0; i < std::min(vecStr.size(), out.s0b.size()); ++i)
        {
            if (!utility::Convert(vecStr[i].c_str(), out.s0b[i]))
                ; //TODO: log error
        }
        pTab->GetString(line, "s0c" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        for (size_t i = 0; i < std::min(vecStr.size(), out.s0c.size()); ++i)
        {
            if (!utility::Convert(vecStr[i].c_str(), out.s0c[i]))
                ; //TODO: log error
        }
        pTab->GetInteger(line, "s1a" , out.s1a, &out.s1a);
        pTab->GetInteger(line, "s1b" , out.s1b, &out.s1b);
        pTab->GetInteger(line, "s1c" , out.s1c, &out.s1c);
        pTab->GetString(line, "s1arEnum" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        out.s1arEnum.resize(vecStr.size());
        for (size_t i = 0; i < vecStr.size(); ++i)
        {
            if (!Enum::ToEnum(vecStr[i].c_str(), out.s1arEnum[i]))
            {
                if (utility::Convert(vecStr[i].c_str(), nTemp) && Enum::ToString((Enum2)nTemp))
                    out.s1arEnum[i] = (Enum2)nTemp;
                else
                    ; //TODO: log error
            }
        }
        pTab->GetString(line, "s1arEnum2" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        for (size_t i = 0; i < std::min(vecStr.size(), out.s1arEnum2.size()); ++i)
        {
            if (!Enum::ToEnum(vecStr[i].c_str(), out.s1arEnum2[i]))
            {
                if (utility::Convert(vecStr[i].c_str(), nTemp) && Enum::ToString((Enum2)nTemp))
                    out.s1arEnum2[i] = (Enum2)nTemp;
                else
                    ; //TODO: log error
            }
        }
        pTab->GetInteger(line, "a" , out.a, &out.a);
        pTab->GetInteger(line, "b" , out.b, &out.b);
        pTab->GetInteger(line, "c" , out.c, &out.c);
        pTab->GetString(line, "s" , out.s.c_str(), cBuffer, CFG_STR_BUFFER_SIZE);
        out.s = cBuffer;
        pTab->GetString(line, "s1.s0a" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        out.s1.s0a.resize(vecStr.size());
        for (size_t i = 0; i < vecStr.size(); ++i)
        {
            if (!utility::Convert(vecStr[i].c_str(), out.s1.s0a[i]))
                ; //TODO: log error
        }
        pTab->GetString(line, "s1.s0b" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        for (size_t i = 0; i < std::min(vecStr.size(), out.s1.s0b.size()); ++i)
        {
            if (!utility::Convert(vecStr[i].c_str(), out.s1.s0b[i]))
                ; //TODO: log error
        }
        pTab->GetString(line, "s1.s0c" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        for (size_t i = 0; i < std::min(vecStr.size(), out.s1.s0c.size()); ++i)
        {
            if (!utility::Convert(vecStr[i].c_str(), out.s1.s0c[i]))
                ; //TODO: log error
        }
        pTab->GetInteger(line, "s1.s1a" , out.s1.s1a, &out.s1.s1a);
        pTab->GetInteger(line, "s1.s1b" , out.s1.s1b, &out.s1.s1b);
        pTab->GetInteger(line, "s1.s1c" , out.s1.s1c, &out.s1.s1c);
        pTab->GetString(line, "s1.s1arEnum" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        out.s1.s1arEnum.resize(vecStr.size());
        for (size_t i = 0; i < vecStr.size(); ++i)
        {
            if (!Enum::ToEnum(vecStr[i].c_str(), out.s1.s1arEnum[i]))
            {
                if (utility::Convert(vecStr[i].c_str(), nTemp) && Enum::ToString((Enum2)nTemp))
                    out.s1.s1arEnum[i] = (Enum2)nTemp;
                else
                    ; //TODO: log error
            }
        }
        pTab->GetString(line, "s1.s1arEnum2" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        for (size_t i = 0; i < std::min(vecStr.size(), out.s1.s1arEnum2.size()); ++i)
        {
            if (!Enum::ToEnum(vecStr[i].c_str(), out.s1.s1arEnum2[i]))
            {
                if (utility::Convert(vecStr[i].c_str(), nTemp) && Enum::ToString((Enum2)nTemp))
                    out.s1.s1arEnum2[i] = (Enum2)nTemp;
                else
                    ; //TODO: log error
            }
        }
        pTab->GetString(line, "s2.s0a" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        out.s2[0].s0a.resize(vecStr.size());
        for (size_t i = 0; i < vecStr.size(); ++i)
        {
            if (!utility::Convert(vecStr[i].c_str(), out.s2[0].s0a[i]))
                ; //TODO: log error
        }
        pTab->GetString(line, "s2.s0b" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        for (size_t i = 0; i < std::min(vecStr.size(), out.s2[0].s0b.size()); ++i)
        {
            if (!utility::Convert(vecStr[i].c_str(), out.s2[0].s0b[i]))
                ; //TODO: log error
        }
        pTab->GetString(line, "s2.s0c" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        for (size_t i = 0; i < std::min(vecStr.size(), out.s2[0].s0c.size()); ++i)
        {
            if (!utility::Convert(vecStr[i].c_str(), out.s2[0].s0c[i]))
                ; //TODO: log error
        }
        pTab->GetInteger(line, "s2.s1a" , out.s2[0].s1a, &out.s2[0].s1a);
        pTab->GetInteger(line, "s2.s1b" , out.s2[0].s1b, &out.s2[0].s1b);
        pTab->GetInteger(line, "s2.s1c" , out.s2[0].s1c, &out.s2[0].s1c);
        pTab->GetString(line, "s2.s1arEnum" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        out.s2[0].s1arEnum.resize(vecStr.size());
        for (size_t i = 0; i < vecStr.size(); ++i)
        {
            if (!Enum::ToEnum(vecStr[i].c_str(), out.s2[0].s1arEnum[i]))
            {
                if (utility::Convert(vecStr[i].c_str(), nTemp) && Enum::ToString((Enum2)nTemp))
                    out.s2[0].s1arEnum[i] = (Enum2)nTemp;
                else
                    ; //TODO: log error
            }
        }
        pTab->GetString(line, "s2.s1arEnum2" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        for (size_t i = 0; i < std::min(vecStr.size(), out.s2[0].s1arEnum2.size()); ++i)
        {
            if (!Enum::ToEnum(vecStr[i].c_str(), out.s2[0].s1arEnum2[i]))
            {
                if (utility::Convert(vecStr[i].c_str(), nTemp) && Enum::ToString((Enum2)nTemp))
                    out.s2[0].s1arEnum2[i] = (Enum2)nTemp;
                else
                    ; //TODO: log error
            }
        }
        pTab->GetString(line, "s2.s0a" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        out.s2[1].s0a.resize(vecStr.size());
        for (size_t i = 0; i < vecStr.size(); ++i)
        {
            if (!utility::Convert(vecStr[i].c_str(), out.s2[1].s0a[i]))
                ; //TODO: log error
        }
        pTab->GetString(line, "s2.s0b" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        for (size_t i = 0; i < std::min(vecStr.size(), out.s2[1].s0b.size()); ++i)
        {
            if (!utility::Convert(vecStr[i].c_str(), out.s2[1].s0b[i]))
                ; //TODO: log error
        }
        pTab->GetString(line, "s2.s0c" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        for (size_t i = 0; i < std::min(vecStr.size(), out.s2[1].s0c.size()); ++i)
        {
            if (!utility::Convert(vecStr[i].c_str(), out.s2[1].s0c[i]))
                ; //TODO: log error
        }
        pTab->GetInteger(line, "s2.s1a" , out.s2[1].s1a, &out.s2[1].s1a);
        pTab->GetInteger(line, "s2.s1b" , out.s2[1].s1b, &out.s2[1].s1b);
        pTab->GetInteger(line, "s2.s1c" , out.s2[1].s1c, &out.s2[1].s1c);
        pTab->GetString(line, "s2.s1arEnum" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        out.s2[1].s1arEnum.resize(vecStr.size());
        for (size_t i = 0; i < vecStr.size(); ++i)
        {
            if (!Enum::ToEnum(vecStr[i].c_str(), out.s2[1].s1arEnum[i]))
            {
                if (utility::Convert(vecStr[i].c_str(), nTemp) && Enum::ToString((Enum2)nTemp))
                    out.s2[1].s1arEnum[i] = (Enum2)nTemp;
                else
                    ; //TODO: log error
            }
        }
        pTab->GetString(line, "s2.s1arEnum2" , "", cBuffer, CFG_STR_BUFFER_SIZE);
        vecStr = utility::Split(cBuffer, ",");
        for (size_t i = 0; i < std::min(vecStr.size(), out.s2[1].s1arEnum2.size()); ++i)
        {
            if (!Enum::ToEnum(vecStr[i].c_str(), out.s2[1].s1arEnum2[i]))
            {
                if (utility::Convert(vecStr[i].c_str(), nTemp) && Enum::ToString((Enum2)nTemp))
                    out.s2[1].s1arEnum2[i] = (Enum2)nTemp;
                else
                    ; //TODO: log error
            }
        }
        return true;
    }

    int Load(ITabFile* pTab, std::vector<Struct2>& out)
    {
        size_t prev = out.size();
        if (pTab == nullptr || pTab->GetHeight() <= 4) 
            return 0;

        // load default data
        Struct2 def;
        Load(pTab, 4, def);

        for (int i = 5; i < pTab->GetHeight(); ++i)
        {
            Struct2 data = def;
            if (Load(pTab, i, data))
                out.push_back(data);
        }

        return out.size() - prev;
    }
} // end of namespace Tab

namespace Json
{
} // end of namespace Json
