#pragma once
#include "CfgDef.h"
#include <ostream>

CFG_NAMESPACE_BEGIN

class StructType;

struct UniqueVarName
{
    UniqueVarName()
    {

    }

    UniqueVarName(const IVariate* _var, const std::string& _name)
        : var(_var), name(_name)
    {
    }

    const IVariate* var;
    std::string name;
};

namespace util
{
    static const std::string EMPTY_STR;
    static const std::vector<std::string> EMPTY_VEC_STR;

    bool Convert(const std::string& str, bool& out);
    bool Convert(const std::string& str, bool& out, bool def);

    bool Convert(const std::string& str, int& out);
    bool Convert(const std::string& str, int& out, int def);

    bool Convert(const std::string& str, float& out);
    bool Convert(const std::string& str, float& out, float def);

    std::string TrimFileSuffix(const std::string& file, char c = '.');

    std::string Contact(const std::vector<std::string>& path, const std::string& c);
    std::vector<std::string> Split(const std::string& str, char s);

    std::vector<std::string> Absolute(const IType* type);
    std::vector<std::string> Relative(const IType* self, const IType* other);

    IType* FindType(IType* scope, const std::string& path);
    IVariate* FindVar(IType* scope, const std::string& path);

    std::ostream& Tab(std::ostream& stream, int tab);

    std::vector<UniqueVarName> UniqueName(const StructType* sType, const std::string& owner = EMPTY_STR);
}

CFG_NAMESPACE_END
