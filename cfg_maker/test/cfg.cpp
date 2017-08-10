/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#include "Cfg.h"
#include "CfgUtility.h"
#include "CfgTabParser.h"
#include <json/reader.h>

#undef min
using namespace cfg;

namespace Enum
{
    static const int s_GlobalType_val[] = {
        -1, 0, 1, 2, 3, CFG_INVALID_ENUM }; // end of enum GlobalType
    static const char* const  s_GlobalType_str[] = {
        "Invalid", "Bool", "Int", "Float", "String", nullptr }; // end of enum GlobalType

    static const int s_Wtf_val[] = {
        -1, 0, 1, 2, 3, CFG_INVALID_ENUM }; // end of enum Wtf
    static const char* const  s_Wtf_str[] = {
        "Invalid", "Bool", "Int", "Float", "String", nullptr }; // end of enum Wtf

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
            if (s_GlobalType_val[i] == (int)value)
                return s_GlobalType_str[i];
        }

        return "";
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

    const char* const * Names(Wtf)
    {
        return s_Wtf_str;
    }

    const char* ToString(Wtf value)
    {
        for (int i = 0; s_Wtf_str[i]; ++i)
        {
            if (s_Wtf_val[i] == (int)value)
                return s_Wtf_str[i];
        }

        return "";
    }

    bool ToEnum(const char* name, Wtf& out)
    {
        for (int i = 0; s_Wtf_str[i]; ++i)
        {
            if (std::strcmp(s_Wtf_str[i], name) == 0)
            {
                out = (Wtf)s_Wtf_val[i];
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
            if (s_Enum1_val[i] == (int)value)
                return s_Enum1_str[i];
        }

        return "";
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
            if (s_Enum2_val[i] == (int)value)
                return s_Enum2_str[i];
        }

        return "";
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
    void sWrite(std::ostream& stream, const Struct0& data)
    {
        for (size_t i = 0; i < data.s0a.size(); ++i)
        {
            if (i) stream << "\t";
            stream << data.s0a[i];
        }
        stream << "\t";

        for (size_t i = 0; i < data.s0b.size(); ++i)
        {
            if (i) stream << "\t";
            stream << data.s0b[i];
        }
        stream << "\t";

        for (size_t i = 0; i < data.s0c.size(); ++i)
        {
            if (i) stream << "\t";
            stream << data.s0c[i];
        }
    }

    void sWrite(std::ostream& stream, const Struct1& data)
    {
        sWrite(stream, (const Struct0&)data);
        stream << "\t";
        stream << data.s1a;
        stream << "\t";
        stream << data.s1b;
        stream << "\t";
        stream << data.s1c;
        stream << "\t";
        stream << data.b1;
        stream << "\t";
        stream << data.f1;
        stream << "\t";
        stream << data.str1;
        stream << "\t";
        stream << data.str2;
        stream << "\t";
        stream << data.str3;
        stream << "\t";
        sWrite(stream, data.s1);
        stream << "\t";

        for (size_t i = 0; i < data.s11.size(); ++i)
        {
            if (i) stream << "\t";
            sWrite(stream, data.s11[i]);
        }
        stream << "\t";

        for (size_t i = 0; i < data.s1arEnum.size(); ++i)
        {
            if (i) stream << "\t";
            stream << Enum::ToString(data.s1arEnum[i]);
        }
        stream << "\t";

        for (size_t i = 0; i < data.s1arEnum2.size(); ++i)
        {
            if (i) stream << "\t";
            stream << Enum::ToString(data.s1arEnum2[i]);
        }
    }

    void sWrite(std::ostream& stream, const Struct2& data)
    {
        stream << data.a;
        stream << "\t";
        stream << data.b;
        stream << "\t";
        stream << data.c;
        stream << "\t";
        stream << data.d;
        stream << "\t";
        stream << data.s;
        stream << "\t";
        sWrite(stream, data.s1);
        stream << "\t";

        for (size_t i = 0; i < data.s2.size(); ++i)
        {
            if (i) stream << "\t";
            sWrite(stream, data.s2[i]);
        }
    }

    static bool sLoad(TabLineIter& iter, Struct0& out)
    {
        std::vector<std::string> vec;

        iter.MoveNext();
        vec = utility::Split(iter.Value(), ",");
        out.s0a.resize(vec.size());
        for (size_t i = 0; i < vec.size(); ++i)
            utility::Convert(vec[i].c_str(), out.s0a[i]);

        iter.MoveNext();
        vec = utility::Split(iter.Value(), ",");
        for (size_t i = 0; i < std::min(vec.size(), out.s0b.size()); ++i)
            utility::Convert(vec[i].c_str(), out.s0b[i]);

        iter.MoveNext();
        vec = utility::Split(iter.Value(), ",");
        for (size_t i = 0; i < std::min(vec.size(), out.s0c.size()); ++i)
            utility::Convert(vec[i].c_str(), out.s0c[i]);
        return true;
    }

    static bool sLoad(TabLineIter& iter, Struct1& out)
    {
        int val = 0;
        std::vector<std::string> vec;

        sLoad(iter, (Struct0&)out);
        iter.MoveNext();
        utility::Convert(iter.Value(), out.s1a);
        iter.MoveNext();
        utility::Convert(iter.Value(), out.s1b);
        iter.MoveNext();
        utility::Convert(iter.Value(), out.s1c);
        iter.MoveNext();
        utility::Convert(iter.Value(), out.b1);
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
                    utility::Log("chunk:%s line:%d title:%s Convert failed from type:%s value:%s", iter.Chunk(), iter.LineNO(), iter.Title(), "Enum2", vec[i].c_str());
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
                    utility::Log("chunk:%s line:%d title:%s Convert failed from type:%s value:%s", iter.Chunk(), iter.LineNO(), iter.Title(), "Enum2", vec[i].c_str());
            }
        }
        return true;
    }

    static bool sLoad(TabLineIter& iter, Struct2& out)
    {
        std::vector<std::string> vec;

        iter.MoveNext();
        utility::Convert(iter.Value(), out.a);
        iter.MoveNext();
        utility::Convert(iter.Value(), out.b);
        iter.MoveNext();
        utility::Convert(iter.Value(), out.c);
        iter.MoveNext();
        utility::Convert(iter.Value(), out.d);
        iter.MoveNext();
        if (iter.Value() && iter.Value()[0])
            out.s = iter.Value();
        sLoad(iter, out.s1);

        for (size_t i = 0; i < out.s2.size(); ++i)
            sLoad(iter, out.s2[i]);
        return true;
    }

    void WriteHeader(std::ostream& stream, const Struct2& def)
    {
        static const char* title = "a\tb\tc\td\ts\ts1.s0a\ts1.s0b\ts1.s0c\ts1.s1a\ts1.s1b\ts1.s1c\ts1.b1\ts1.f1\ts1.str1\ts1.str2\ts1.str3\ts1.s1.s0a\ts1.s1.s0b\ts1.s1.s0c\ts1.s11.s0a\ts1.s11.s0b\ts1.s11.s0c\ts1.s11.s0a\ts1.s11.s0b\ts1.s11.s0c\ts1.s1arEnum\ts1.s1arEnum2\ts2.s0a\ts2.s0b\ts2.s0c\ts2.s1a\ts2.s1b\ts2.s1c\ts2.b1\ts2.f1\ts2.str1\ts2.str2\ts2.str3\ts2.s1.s0a\ts2.s1.s0b\ts2.s1.s0c\ts2.s11.s0a\ts2.s11.s0b\ts2.s11.s0c\ts2.s11.s0a\ts2.s11.s0b\ts2.s11.s0c\ts2.s1arEnum\ts2.s1arEnum2\ts2.s0a\ts2.s0b\ts2.s0c\ts2.s1a\ts2.s1b\ts2.s1c\ts2.b1\ts2.f1\ts2.str1\ts2.str2\ts2.str3\ts2.s1.s0a\ts2.s1.s0b\ts2.s1.s0c\ts2.s11.s0a\ts2.s11.s0b\ts2.s11.s0c\ts2.s11.s0a\ts2.s11.s0b\ts2.s11.s0c\ts2.s1arEnum\ts2.s1arEnum2";
        static const char* type = "int\tint\tint\tint\tstring\tint[]\tint[10]\tint[10]\tint\tint\tint\tbool\tfloat\tstring\tstring\tstring\tint[]\tint[10]\tint[10]\tint[]\tint[10]\tint[10]\tint[]\tint[10]\tint[10]\tEnum2[]\tEnum2[2]\tint[]\tint[10]\tint[10]\tint\tint\tint\tbool\tfloat\tstring\tstring\tstring\tint[]\tint[10]\tint[10]\tint[]\tint[10]\tint[10]\tint[]\tint[10]\tint[10]\tEnum2[]\tEnum2[2]\tint[]\tint[10]\tint[10]\tint\tint\tint\tbool\tfloat\tstring\tstring\tstring\tint[]\tint[10]\tint[10]\tint[]\tint[10]\tint[10]\tint[]\tint[10]\tint[10]\tEnum2[]\tEnum2[2]";
        static const char* desc = "这个是描述\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";

        stream << title << std::endl << type << std::endl << desc << std::endl;
        Write(stream, def);
    }

    void Write(std::ostream& stream, const Struct2& data)
    {
        sWrite(stream, data);
        stream << std::endl;
    }

    bool Load(const char* data, size_t size, std::vector<Struct2>& out, const char* chunk /*= nullptr*/)
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
        if (!parser.Parse(data, size, chunk))
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
    void sWrite(std::ostream& stream, const Struct0& data, int tab)
    {
        stream << std::string(" ", tab * 4) << "\"s0a\":";
        stream << "[";
        for (size_t i = 0; i < data.s0a.size(); ++i)
        {
            if (i) stream << ",";
            stream << data.s0a[i];
        }
        stream << "]";
        stream << "," << std::endl;
        stream << std::string(" ", tab * 4) << "\"s0b\":";
        stream << "[";
        for (size_t i = 0; i < data.s0b.size(); ++i)
        {
            if (i) stream << ",";
            stream << data.s0b[i];
        }
        stream << "]";
        stream << "," << std::endl;
        stream << std::string(" ", tab * 4) << "\"s0c\":";
        stream << "[";
        for (size_t i = 0; i < data.s0c.size(); ++i)
        {
            if (i) stream << ",";
            stream << data.s0c[i];
        }
        stream << "]";
    }

    void sWrite(std::ostream& stream, const Struct1& data, int tab)
    {
        sWrite(stream, (Struct0&)data, tab);
        stream << "," << std::endl;

        stream << std::string(" ", tab * 4) << "\"s1a\":";
        stream << data.s1a;
        stream << "," << std::endl;
        stream << std::string(" ", tab * 4) << "\"s1b\":";
        stream << data.s1b;
        stream << "," << std::endl;
        stream << std::string(" ", tab * 4) << "\"s1c\":";
        stream << data.s1c;
        stream << "," << std::endl;
        stream << std::string(" ", tab * 4) << "\"b1\":";
        stream << data.b1;
        stream << "," << std::endl;
        stream << std::string(" ", tab * 4) << "\"f1\":";
        stream << data.f1;
        stream << "," << std::endl;
        stream << std::string(" ", tab * 4) << "\"str1\":";
        stream << "\"" << data.str1<< "\"";
        stream << "," << std::endl;
        stream << std::string(" ", tab * 4) << "\"str2\":";
        stream << "\"" << data.str2<< "\"";
        stream << "," << std::endl;
        stream << std::string(" ", tab * 4) << "\"str3\":";
        stream << "\"" << data.str3<< "\"";
        stream << "," << std::endl;
        stream << std::string(" ", tab * 4) << "\"s1\":";
        stream << "{" << std::endl;
        sWrite(stream, data.s1, tab + 1);
        stream << std::endl << std::string(" ", tab * 4) << "}";
        stream << "," << std::endl;
        stream << std::string(" ", tab * 4) << "\"s11\":";
        stream << "[";
        for (size_t i = 0; i < data.s11.size(); ++i)
        {
            if (i) stream << ",";
            stream << "{" << std::endl;
            sWrite(stream, data.s11[i], tab + 1);
            stream << std::endl << std::string(" ", tab * 4) << "}";
        }
        stream << "]";
        stream << "," << std::endl;
        stream << std::string(" ", tab * 4) << "\"s1arEnum\":";
        stream << "[";
        for (size_t i = 0; i < data.s1arEnum.size(); ++i)
        {
            if (i) stream << ",";
            stream << (int)data.s1arEnum[i];
        }
        stream << "]";
        stream << "," << std::endl;
        stream << std::string(" ", tab * 4) << "\"s1arEnum2\":";
        stream << "[";
        for (size_t i = 0; i < data.s1arEnum2.size(); ++i)
        {
            if (i) stream << ",";
            stream << (int)data.s1arEnum2[i];
        }
        stream << "]";
    }

    bool sLoad(const Json::Value& node, Struct0& out)
    {
        if (node.isMember("s0a"))
        {
            const Json::Value& mem = node["s0a"];
            if (mem.type() == Json::ValueType::arrayValue)
            {
                out.s0a.resize(mem.size());
                for (int i = 0; i < (int)mem.size(); ++i)
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

    void Write(std::ostream& stream, const Struct1& data)
    {
        stream << "{" << std::endl;
        sWrite(stream, data, 1);
        stream << std::endl << "}" << std::endl;
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
