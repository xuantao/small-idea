#pragma once

#include <string>
#include "CfgDef.h"

CFG_NAMESPACE_BEGIN

class Context
{
public:

public:
    void OnParse(const std::string& file);

    void OnIncludeBegin(const std::string& file);
    void OnIncludeEnd();

    void OnPredefine(const std::string& name);
    void OnStructBegin(const std::string& name);
    void OnInherit(const std::string& name);
    void OnStructEnd();

    void OnEnumBegin(const std::string& name);
    void OnEnumMember(const std::string& name, const std::string& value = "");
    void OnEnumEnd();

    void OnVariateBegin(const std::string& type, const std::string& name);
    void OnVariateValue(const std::string& value);
    void OnVariateArray(const std::string& length = "");
    void OnVariateEnd();
};

CFG_NAMESPACE_END
