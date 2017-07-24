#pragma once
#include "CfgDef.h"
#include <string>
#include <vector>

CFG_NAMESPACE_BEGIN

namespace util
{
    const static std::string STR_EMPTY;

    bool Convert(const std::string& str, bool& out, bool def);
    bool Convert(const std::string& str, bool& out)
    {
        return Convert(str, out, out);
    }

    bool Convert(const std::string& str, int& out, int def);
    bool Convert(const std::string& str, int& out)
    {
        return Convert(str, out, out);
    }

    bool Convert(const std::string& str, float& out, float def);
    bool Convert(const std::string& str, float& out)
    {
        return Convert(str, out, out);
    }

    std::vector<std::string> Split(const std::string& str, char s);
}

CFG_NAMESPACE_END
