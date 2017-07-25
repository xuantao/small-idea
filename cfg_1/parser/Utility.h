#pragma once
#include "CfgDef.h"
#include <string>
#include <vector>

CFG_NAMESPACE_BEGIN

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

    std::string Contact(const std::vector<std::string>& path, const std::string& c);
    std::vector<std::string> Split(const std::string& str, char s);

    std::vector<std::string> Absolute(const IType* type);
    std::vector<std::string> Relative(const IType* self, const IType* other);

    const IType* FindType(const IType* scope, const std::string& path);
    const IVariate* FindVar(const IType* scope, const std::string& path);
}

CFG_NAMESPACE_END
