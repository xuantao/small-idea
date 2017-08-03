/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#include "xxx.h"

namespace Enum
{
    static const int s_Enum1_val[] = {
        0, 0, -111111 }; // end of enum Enum1
    static const char* const  s_Enum1_str[] = {
        "a", "b", nullptr }; // end of enum Enum1
    static const int s_Enum2_val[] = {
        100, 101, -111111 }; // end of enum Enum2
    static const char* const  s_Enum2_str[] = {
        "c", "d", nullptr }; // end of enum Enum2
    static const int s_GlobalType_val[] = {
        -1, 0, 1, 2, 3, -111111 }; // end of enum GlobalType
    static const char* const  s_GlobalType_str[] = {
        "Invalid", "Bool", "Int", "Float", "String", nullptr }; // end of enum GlobalType

    const char*[] Names(Enum1)
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
    bool ToEnum(Enum1& out, const char* name)
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
    const char*[] Names(Enum2)
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
    bool ToEnum(Enum2& out, const char* name)
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
    const char*[] Names(GlobalType)
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
    bool ToEnum(GlobalType& out, const char* name)
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
    static bool Load(ITabFile* pTab, int line, Struct0& data)
    {
        {
            std::vector<std::string> vecStr;
            char buffer[1024] = {0};
            pTab->GetString(line, "data.a" , "", buffer, 1024);
            vecStr = KGTool::Split(buffer, ',');
        }
        return true;
    }

    int Load(ITabFile* pTab, std::vector<Struct0>& out)
    {
        if (pTab == nullptr || pTab->GetHeight() <= 4) 
            return 0;

        Struct0 def;
        Load(pTab, 4, def);

        out.resize(pTab->GetHeight() - 4, def);
        for (int i = 5; i < pTab->GetHeight(); ++i)
        {
            Load(pTab, i, def)
        }
    }
    static bool Load(ITabFile* pTab, int line, Struct1& data)
    {
        {
            std::vector<std::string> vecStr;
            char buffer[1024] = {0};
            pTab->GetString(line, "data.a" , "", buffer, 1024);
            vecStr = KGTool::Split(buffer, ',');
        }
        pTab->GetInteger(line, "data.a" , data.a, &data.a);
        pTab->GetInteger(line, "data.b" , data.b, &data.b);
        pTab->GetInteger(line, "data.c" , data.c, &data.c);
        return true;
    }

    int Load(ITabFile* pTab, std::vector<Struct1>& out)
    {
        if (pTab == nullptr || pTab->GetHeight() <= 4) 
            return 0;

        Struct1 def;
        Load(pTab, 4, def);

        out.resize(pTab->GetHeight() - 4, def);
        for (int i = 5; i < pTab->GetHeight(); ++i)
        {
            Load(pTab, i, def)
        }
    }
    static bool Load(ITabFile* pTab, int line, Struct2& data)
    {
        {
            std::vector<std::string> vecStr;
            char buffer[1024] = {0};
            pTab->GetString(line, "data.a" , "", buffer, 1024);
            vecStr = KGTool::Split(buffer, ',');
        }
        pTab->GetInteger(line, "data.a" , data.a, &data.a);
        pTab->GetInteger(line, "data.b" , data.b, &data.b);
        pTab->GetInteger(line, "data.c" , data.c, &data.c);
        pTab->GetInteger(line, "data.MAX" , data.MAX, &data.MAX);
        pTab->GetInteger(line, "data.a" , data.a, &data.a);
        pTab->GetInteger(line, "data.b" , data.b, &data.b);
        pTab->GetInteger(line, "data.c" , data.c, &data.c);
                {
            char buffer[1024] = {0};
            pTab->GetString(line, "data.s" , data.s.c_str(), buffer, 1024);
            data.s = buffer;
        }
        {
            std::vector<std::string> vecStr;
            char buffer[1024] = {0};
            pTab->GetString(line, "data.s1.a" , "", buffer, 1024);
            vecStr = KGTool::Split(buffer, ',');
        }
        pTab->GetInteger(line, "data.s1.a" , data.s1.a, &data.s1.a);
        pTab->GetInteger(line, "data.s1.b" , data.s1.b, &data.s1.b);
        pTab->GetInteger(line, "data.s1.c" , data.s1.c, &data.s1.c);
        {
            std::vector<std::string> vecStr;
            char buffer[1024] = {0};
            pTab->GetString(line, "data.s2_1.a" , "", buffer, 1024);
            vecStr = KGTool::Split(buffer, ',');
        }
        pTab->GetInteger(line, "data.s2_1.a" , data.s2_1.a, &data.s2_1.a);
        pTab->GetInteger(line, "data.s2_1.b" , data.s2_1.b, &data.s2_1.b);
        pTab->GetInteger(line, "data.s2_1.c" , data.s2_1.c, &data.s2_1.c);
        {
            std::vector<std::string> vecStr;
            char buffer[1024] = {0};
            pTab->GetString(line, "data.s2_2.a" , "", buffer, 1024);
            vecStr = KGTool::Split(buffer, ',');
        }
        pTab->GetInteger(line, "data.s2_2.a" , data.s2_2.a, &data.s2_2.a);
        pTab->GetInteger(line, "data.s2_2.b" , data.s2_2.b, &data.s2_2.b);
        pTab->GetInteger(line, "data.s2_2.c" , data.s2_2.c, &data.s2_2.c);
        return true;
    }

    int Load(ITabFile* pTab, std::vector<Struct2>& out)
    {
        if (pTab == nullptr || pTab->GetHeight() <= 4) 
            return 0;

        Struct2 def;
        Load(pTab, 4, def);

        out.resize(pTab->GetHeight() - 4, def);
        for (int i = 5; i < pTab->GetHeight(); ++i)
        {
            Load(pTab, i, def)
        }
    }
} // end of namespace Tab

namespace Json
{
} // end of namespace Json
