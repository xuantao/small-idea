/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#include "Cfg.h"
#include "CfgUtility.h"
#include "CfgTabParser.h"
#include <json/reader.h>

using namespace cfg;

namespace Enum
{
    static const int s_GlobalType_val[] = {
        -1, 0, 1, 2, 3, CFG_INVALID_ENUM }; // end of enum GlobalType
    static const char* const  s_GlobalType_str[] = {
        "Invalid", "Bool", "Int", "Float", "String", nullptr }; // end of enum GlobalType

    static const int s_Enum1_val[] = {
        0, 0, CFG_INVALID_ENUM }; // end of enum Enum1
    static const char* const  s_Enum1_str[] = {
        "a", "b", nullptr }; // end of enum Enum1

    static const int s_Enum2_val[] = {
        100, 101, CFG_INVALID_ENUM }; // end of enum Enum2
    static const char* const  s_Enum2_str[] = {
        "c", "d", nullptr }; // end of enum Enum2

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
} // end of namespace Enum

namespace Tab
{
    static bool sLoad(TabLineIter& iter, Struct0& out)
    {
        std::vector<std::string> vec;

        iter.MoveNext();
        vec = utility::Split(iter.Value(), ",");
        out.s0a.resize(vec.size());
        for (size_t i = 0; i < vec.size(); ++i)
        {
            if (!utility::Convert(vec[i].c_str(), out.s0a[i]))
                ; //TODO: log error
        }

        iter.MoveNext();
        vec = utility::Split(iter.Value(), ",");
        for (size_t i = 0; i < std::min(vec.size(), out.s0b.size()); ++i)
        {
            if (!utility::Convert(vec[i].c_str(), out.s0b[i]))
                ; //TODO: log error
        }

        iter.MoveNext();
        vec = utility::Split(iter.Value(), ",");
        for (size_t i = 0; i < std::min(vec.size(), out.s0c.size()); ++i)
        {
            if (!utility::Convert(vec[i].c_str(), out.s0c[i]))
                ; //TODO: log error
        }
        return true;
    }

    static bool sLoad(TabLineIter& iter, Struct1& out)
    {
        int val = 0;
        std::vector<std::string> vec;

        sLoad(iter, (Struct0&)out);
        iter.MoveNext();
        utility::Convert(iter.Value(), out.f1);
        iter.MoveNext();
        if (iter.Value() && iter.Value()[0])
            out.str1 = iter.Value();
        iter.MoveNext();
        if (iter.Value() && iter.Value()[0])
            out.str2 = iter.Value();
        iter.MoveNext();
        if (iter.Value() && iter.Value()[0])
            out.str3 = iter.Value();
        sLoad(iter, out.s1);

        for (size_t i = 0; i < out.s11.size(); ++i)
            sLoad(iter, out.s11[i]);

        iter.MoveNext();
        vec = utility::Split(iter.Value(), ",");
        out.s1arEnum.resize(vec.size());
        for (size_t i = 0; i < vec.size(); ++i)
        {
            if (!Enum::ToEnum(vec[i].c_str(), out.s1arEnum[i]))
            {
                if (utility::Convert(vec[i].c_str(), val) && Enum::ToString((Enum2)val))
                    out.s1arEnum[i] = (Enum2)val;
                else
                    ; //TODO: log error
            }
        }

        iter.MoveNext();
        vec = utility::Split(iter.Value(), ",");
        for (size_t i = 0; i < std::min(vec.size(), out.s1arEnum2.size()); ++i)
        {
            if (!Enum::ToEnum(vec[i].c_str(), out.s1arEnum2[i]))
            {
                if (utility::Convert(vec[i].c_str(), val) && Enum::ToString((Enum2)val))
                    out.s1arEnum2[i] = (Enum2)val;
                else
                    ; //TODO: log error
            }
        }
        return true;
    }

    static bool sLoad(TabLineIter& iter, Struct2& out)
    {
        std::vector<std::string> vec;

        iter.MoveNext();
        if (iter.Value() && iter.Value()[0])
            out.s = iter.Value();
        sLoad(iter, out.s1);

        for (size_t i = 0; i < out.s2.size(); ++i)
            sLoad(iter, out.s2[i]);
        return true;
    }

    bool Load(const char* data, size_t size, std::vector<Struct2>& out)
    {
        static const std::array<const char*, 71> titles = {
            "a", "b", "c", "d", "s",
            "s1.s0a", "s1.s0b", "s1.s0c", "s1.s1a", "s1.s1b",
            "s1.s1c", "s1.b1", "s1.f1", "s1.str1", "s1.str2",
            "s1.str3", "s1.s1.s0a", "s1.s1.s0b", "s1.s1.s0c", "s1.s11.s0a",
            "s1.s11.s0b", "s1.s11.s0c", "s1.s11.s0a", "s1.s11.s0b", "s1.s11.s0c",
            "s1.s1arEnum", "s1.s1arEnum2", "s2.s0a", "s2.s0b", "s2.s0c",
            "s2.s1a", "s2.s1b", "s2.s1c", "s2.b1", "s2.f1",
            "s2.str1", "s2.str2", "s2.str3", "s2.s1.s0a", "s2.s1.s0b",
            "s2.s1.s0c", "s2.s11.s0a", "s2.s11.s0b", "s2.s11.s0c", "s2.s11.s0a",
            "s2.s11.s0b", "s2.s11.s0c", "s2.s1arEnum", "s2.s1arEnum2", "s2.s0a",
            "s2.s0b", "s2.s0c", "s2.s1a", "s2.s1b", "s2.s1c",
            "s2.b1", "s2.f1", "s2.str1", "s2.str2", "s2.str3",
            "s2.s1.s0a", "s2.s1.s0b", "s2.s1.s0c", "s2.s11.s0a", "s2.s11.s0b",
            "s2.s11.s0c", "s2.s11.s0a", "s2.s11.s0b", "s2.s11.s0c", "s2.s1arEnum",
            "s2.s1arEnum2"
        };

        std::vector<std::string> vec;

        TabParser<71> parser(titles);
        if (!parser.Parse(data, size))
            return false;

        // load default data
        Struct2 def;
        if (!parser.NextLine() || !sLoad(parser.LineIter(), def))
            return false;

        while (parser.NextLine())
        {
            Struct2 data = def;
            if (sLoad(parser.LineIter(), data))
                out.push_back(data);
        }
        return true;
    }
} // end of namespace Tab

namespace Json
{
    bool sLoad(const Json::Value& node, Struct0& out)
    {
        if (node.isMember("s0a"))
        {
            const Json::Value& mem = node["s0a"];
            if (mem.type() == Json::ValueType::arrayValue)
            {
                out.s0a.resize(mem.size());
                for (int i = 0; i < mem.size(); ++i)
                {
                    if (mem[i].type() == Json::ValueType::intValue || mem[i].type() == Json::ValueType::uintValue)
                        out.s0a[i]  = mem[i].asInt();
                }
            }
        }

        if (node.isMember("s0b"))
        {
            const Json::Value& mem = node["s0b"];
            if (mem.type() == Json::ValueType::arrayValue)
            {
                for (int i = 0; i < std::min((int)mem.size(), (int)out.s0b.size()); ++i)
                {
                    if (mem[i].type() == Json::ValueType::intValue || mem[i].type() == Json::ValueType::uintValue)
                        out.s0b[i]  = mem[i].asInt();
                }
            }
        }

        if (node.isMember("s0c"))
        {
            const Json::Value& mem = node["s0c"];
            if (mem.type() == Json::ValueType::arrayValue)
            {
                for (int i = 0; i < std::min((int)mem.size(), (int)out.s0c.size()); ++i)
                {
                    if (mem[i].type() == Json::ValueType::intValue || mem[i].type() == Json::ValueType::uintValue)
                        out.s0c[i]  = mem[i].asInt();
                }
            }
        }
        return true;
    }

    bool sLoad(const Json::Value& node, Struct1& out)
    {
        sLoad(node, (Struct0&)out);

        if (node.isMember("s1a"))
        {
            const Json::Value& mem = node["s1a"];
            if (mem.type() == Json::ValueType::intValue || mem.type() == Json::ValueType::uintValue)
                out.s1a = mem.asInt();
        }
        if (node.isMember("s1b"))
        {
            const Json::Value& mem = node["s1b"];
            if (mem.type() == Json::ValueType::intValue || mem.type() == Json::ValueType::uintValue)
                out.s1b = mem.asInt();
        }
        if (node.isMember("s1c"))
        {
            const Json::Value& mem = node["s1c"];
            if (mem.type() == Json::ValueType::intValue || mem.type() == Json::ValueType::uintValue)
                out.s1c = mem.asInt();
        }
        if (node.isMember("b1"))
        {
            const Json::Value& mem = node["b1"];
            if (mem.type() == Json::ValueType::booleanValue)
                out.b1 = mem.asBool();
        }
        if (node.isMember("f1"))
        {
            const Json::Value& mem = node["f1"];
            if (mem.type() == Json::ValueType::intValue || mem.type() == Json::ValueType::uintValue)
                out.f1 = (float)mem.asInt();
            else if (mem.type() == Json::ValueType::realValue)
                out.f1 = (float)mem.asDouble();
        }
        if (node.isMember("str1"))
        {
            const Json::Value& mem = node["str1"];
            if (mem.type() == Json::ValueType::stringValue)
                out.str1 = mem.asString();
        }
        if (node.isMember("str2"))
        {
            const Json::Value& mem = node["str2"];
            if (mem.type() == Json::ValueType::stringValue)
                out.str2 = mem.asString();
        }
        if (node.isMember("str3"))
        {
            const Json::Value& mem = node["str3"];
            if (mem.type() == Json::ValueType::stringValue)
                out.str3 = mem.asString();
        }
        if (node.isMember("s1"))
        {
            const Json::Value& mem = node["s1"];
            if (mem.type() == Json::ValueType::objectValue)
                sLoad(mem, out.s1);
        }

        if (node.isMember("s11"))
        {
            const Json::Value& mem = node["s11"];
            if (mem.type() == Json::ValueType::arrayValue)
            {
                for (int i = 0; i < std::min((int)mem.size(), (int)out.s11.size()); ++i)
                {
                    if (mem[i].type() == Json::ValueType::objectValue)
                        sLoad(mem[i], out.s11[i]);
                }
            }
        }

        if (node.isMember("s1arEnum"))
        {
            const Json::Value& mem = node["s1arEnum"];
            if (mem.type() == Json::ValueType::arrayValue)
            {
                out.s1arEnum.resize(mem.size());
                for (int i = 0; i < (int)mem.size(); ++i)
                {
                    if (mem[i].type() == Json::ValueType::intValue || mem[i].type() == Json::ValueType::uintValue)
                        out.s1arEnum[i] = (Enum2)mem[i].asInt();
                }
            }
        }

        if (node.isMember("s1arEnum2"))
        {
            const Json::Value& mem = node["s1arEnum2"];
            if (mem.type() == Json::ValueType::arrayValue)
            {
                for (int i = 0; i < std::min((int)mem.size(), (int)out.s1arEnum2.size()); ++i)
                {
                    if (mem[i].type() == Json::ValueType::intValue || mem[i].type() == Json::ValueType::uintValue)
                        out.s1arEnum2[i] = (Enum2)mem[i].asInt();
                }
            }
        }
        return true;
    }

    bool Load(const char* data, size_t size, Struct1& out)
    {
        if (data == nullptr || data[0] == 0)
            return false;

        Json::Value node;
        Json::Reader reader;
        if (!reader.parse(std::string(data, size), node, false))
            return false;

        if (node.type() != Json::ValueType::objectValue)
            return false;

         return sLoad(node, out);
    }
} // end of namespace Json
