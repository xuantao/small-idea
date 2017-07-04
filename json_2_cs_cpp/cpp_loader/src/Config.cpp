#include "Config.h"
#include <json/reader.h>

static bool sLoad(const Json::Value& node, Config::Type_1& ret);
static bool sLoad(const Json::Value& node, Config::Type_2& ret);
static bool sLoad(const Json::Value& node, Config::Type_3& ret);
static bool sLoad(const Json::Value& node, Config::Type_4& ret);
static bool sLoad(const Json::Value& node, Config::Type_5& ret);
static bool sLoad(const Json::Value& node, Config::Type_6& ret);
static bool sLoad(const Json::Value& node, Config::Type_7& ret);
static bool sLoad(const Json::Value& node, Config::Type_8& ret);
static bool sLoad(const Json::Value& node, std::vector<int>& ret);
static bool sLoad(const Json::Value& node, std::vector<std::vector<int> >& ret);
static bool sLoad(const Json::Value& node, std::vector<Config::Type_1>& ret);
static bool sLoad(const Json::Value& node, std::vector<Config::Type_2>& ret);
static bool sLoad(const Json::Value& node, std::vector<Config::Type_6>& ret);

static bool sLoad(const Json::Value& node, Config::Type_1& ret)
{
    return true;
}

static bool sLoad(const Json::Value& node, Config::Type_2& ret)
{
    if (node.type() != Json::ValueType::objectValue)
        return false;

    if (node.isMember("a"))
    {
        const Json::Value& mem = node["a"];
        if (mem.type() == Json::ValueType::intValue || mem.type() == Json::ValueType::uintValue)
            ret.a = mem.asInt();
    }

    if (node.isMember("b"))
    {
        const Json::Value& mem = node["b"];
        if (mem.type() == Json::ValueType::intValue || mem.type() == Json::ValueType::uintValue)
            ret.b = mem.asInt();
    }

    return true;
}

static bool sLoad(const Json::Value& node, Config::Type_3& ret)
{
    if (node.type() != Json::ValueType::objectValue)
        return false;

    if (node.isMember("d1"))
    {
        const Json::Value& mem = node["d1"];
        if (mem.type() == Json::ValueType::realValue)
            ret.d1 = (float)mem.asDouble();
    }

    if (node.isMember("d2"))
    {
        const Json::Value& mem = node["d2"];
        if (mem.type() == Json::ValueType::stringValue)
            ret.d2 = mem.asString();
    }

    return true;
}

static bool sLoad(const Json::Value& node, Config::Type_4& ret)
{
    if (node.type() != Json::ValueType::objectValue)
        return false;

    if (node.isMember("a"))
    {
        const Json::Value& mem = node["a"];
        if (mem.type() == Json::ValueType::intValue || mem.type() == Json::ValueType::uintValue)
            ret.a = mem.asInt();
    }

    if (node.isMember("b"))
    {
        const Json::Value& mem = node["b"];
        if (mem.type() == Json::ValueType::intValue || mem.type() == Json::ValueType::uintValue)
            ret.b = mem.asInt();
    }

    if (node.isMember("c"))
    {
        const Json::Value& mem = node["c"];
        sLoad(mem, ret.c);
    }

    return true;
}

static bool sLoad(const Json::Value& node, Config::Type_5& ret)
{
    if (node.type() != Json::ValueType::objectValue)
        return false;

    if (node.isMember("a"))
    {
        const Json::Value& mem = node["a"];
        if (mem.type() == Json::ValueType::intValue || mem.type() == Json::ValueType::uintValue)
            ret.a = mem.asInt();
    }

    if (node.isMember("b"))
    {
        const Json::Value& mem = node["b"];
        if (mem.type() == Json::ValueType::intValue || mem.type() == Json::ValueType::uintValue)
            ret.b = mem.asInt();
    }

    if (node.isMember("c"))
    {
        const Json::Value& mem = node["c"];
        if (mem.type() == Json::ValueType::booleanValue)
            ret.c = mem.asBool();
    }

    return true;
}

static bool sLoad(const Json::Value& node, Config::Type_6& ret)
{
    if (node.type() != Json::ValueType::objectValue)
        return false;

    if (node.isMember("a"))
    {
        const Json::Value& mem = node["a"];
        if (mem.type() == Json::ValueType::intValue || mem.type() == Json::ValueType::uintValue)
            ret.a = mem.asInt();
    }

    if (node.isMember("b"))
    {
        const Json::Value& mem = node["b"];
        if (mem.type() == Json::ValueType::intValue || mem.type() == Json::ValueType::uintValue)
            ret.b = mem.asInt();
    }

    if (node.isMember("c"))
    {
        const Json::Value& mem = node["c"];
        if (mem.type() == Json::ValueType::booleanValue)
            ret.c = mem.asBool();
    }

    if (node.isMember("d"))
    {
        const Json::Value& mem = node["d"];
        sLoad(mem, ret.d);
    }

    return true;
}

static bool sLoad(const Json::Value& node, Config::Type_7& ret)
{
    if (node.type() != Json::ValueType::objectValue)
        return false;

    if (node.isMember("d"))
    {
        const Json::Value& mem = node["d"];
        sLoad(mem, ret.d);
    }

    return true;
}

static bool sLoad(const Json::Value& node, Config::Type_8& ret)
{
    if (node.type() != Json::ValueType::objectValue)
        return false;

    if (node.isMember("mem1"))
    {
        const Json::Value& mem = node["mem1"];
        sLoad(mem, ret.mem1);
    }

    if (node.isMember("mem2"))
    {
        const Json::Value& mem = node["mem2"];
        sLoad(mem, ret.mem2);
    }

    return true;
}

static bool sLoad(const Json::Value& node, std::vector<int>& ret)
{
    if (node.type() != Json::ValueType::arrayValue)
        return false;

    ret.resize(node.size());
    for (int i = 0; i < (int)node.size(); ++i)
    {
        const Json::Value& child = node[i];
        if (child.type() == Json::ValueType::intValue || child.type() == Json::ValueType::uintValue)
            ret[i] = child.asInt();
    }

    return true;
}

static bool sLoad(const Json::Value& node, std::vector<std::vector<int> >& ret)
{
    if (node.type() != Json::ValueType::arrayValue)
        return false;

    ret.resize(node.size());
    for (int i = 0; i < (int)node.size(); ++i)
    {
        sLoad(node[i], ret[i]);
    }

    return true;
}

static bool sLoad(const Json::Value& node, std::vector<Config::Type_1>& ret)
{
    if (node.type() != Json::ValueType::arrayValue)
        return false;

    ret.resize(node.size());
    for (int i = 0; i < (int)node.size(); ++i)
    {
        sLoad(node[i], ret[i]);
    }

    return true;
}

static bool sLoad(const Json::Value& node, std::vector<Config::Type_2>& ret)
{
    if (node.type() != Json::ValueType::arrayValue)
        return false;

    ret.resize(node.size());
    for (int i = 0; i < (int)node.size(); ++i)
    {
        sLoad(node[i], ret[i]);
    }

    return true;
}

static bool sLoad(const Json::Value& node, std::vector<Config::Type_6>& ret)
{
    if (node.type() != Json::ValueType::arrayValue)
        return false;

    ret.resize(node.size());
    for (int i = 0; i < (int)node.size(); ++i)
    {
        sLoad(node[i], ret[i]);
    }

    return true;
}

Config::Config()
{}

bool Config::LoadJson(const char* lpJson, int nSize)
{
    if (lpJson == nullptr)
        return false;

    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(std::string(lpJson, nSize), root, false))
        return false;

    if (root.type() != Json::ValueType::objectValue)
        return false;

    if (root.isMember("nomal"))
    {
        const Json::Value& mem = root["nomal"];
        if (mem.type() == Json::ValueType::stringValue)
            nomal = mem.asString();
    }

    if (root.isMember("objNrmal"))
    {
        const Json::Value& mem = root["objNrmal"];
        sLoad(mem, objNrmal);
    }

    if (root.isMember("objComplex"))
    {
        const Json::Value& mem = root["objComplex"];
        sLoad(mem, objComplex);
    }

    if (root.isMember("arrayEmpty"))
    {
        const Json::Value& mem = root["arrayEmpty"];
        sLoad(mem, arrayEmpty);
    }

    if (root.isMember("arrayNormal"))
    {
        const Json::Value& mem = root["arrayNormal"];
        sLoad(mem, arrayNormal);
    }

    if (root.isMember("arrayObj"))
    {
        const Json::Value& mem = root["arrayObj"];
        sLoad(mem, arrayObj);
    }

    if (root.isMember("arrayObjComplex"))
    {
        const Json::Value& mem = root["arrayObjComplex"];
        sLoad(mem, arrayObjComplex);
    }

    if (root.isMember("arrayDouble"))
    {
        const Json::Value& mem = root["arrayDouble"];
        sLoad(mem, arrayDouble);
    }
    return true;
}
