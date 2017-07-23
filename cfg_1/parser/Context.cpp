#include "Context.h"
#include <stdio.h>

CFG_NAMESPACE_BEGIN

void Context::OnParse(const std::string& file)
{
    printf("Start Parse File %s\n", file.c_str());
}

void Context::OnIncludeBegin(const std::string& file)
{

}

void Context::OnIncludeEnd()
{

}

void Context::OnPredefine(const std::string& name)
{
    printf("struct %s;\n", name.c_str());
}

void Context::OnStructBegin(const std::string& name)
{
    printf("struct %s{\n", name.c_str());
}

void Context::OnInherit(const std::string& name)
{
    printf("  inherit:%s\n", name.c_str());
}

void Context::OnStructEnd()
{
    printf("}\n");
}

void Context::OnEnumBegin(const std::string& name)
{
    printf("enum %s {\n", name.c_str());
}

void Context::OnEnumMember(const std::string& name, const std::string& value/* = ""*/)
{
    if (value.empty())
        printf("%s\n", name.c_str());
    else
        printf("%s=%s\n", name.c_str(), value.c_str());
}

void Context::OnEnumEnd()
{
    printf("}\n");
}

void Context::OnVariateBegin(const std::string& type, const std::string& name)
{
    printf("%s %s", type.c_str(), name.c_str());
}

void Context::OnVariateValue(const std::string& value)
{
    printf(" = %s", value.c_str());
}

void Context::OnVariateArray(const std::string& length/* = ""*/)
{
    printf("[%s]", length.c_str());
}

void Context::OnVariateEnd()
{
    printf(";\n");
}

CFG_NAMESPACE_END
