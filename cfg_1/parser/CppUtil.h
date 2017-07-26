#pragma once

#include "CfgDef.h"
#include <ostream>

CFG_NAMESPACE_BEGIN

struct CppVarData
{
    std::string type;
    std::string name;
    std::string value;
    const IVariate* var;
};

namespace cpp_util
{
    std::string RawName(RawCategory raw);
    std::string DefValue(RawCategory raw);
    std::string TypeName(const IType* scope, const IType* type);
    std::string Value(const IType* scope, const IType* type, const IValue* val);
    std::string OrignalValue(const IValue* val);

    bool Convert(const IVariate* var, CppVarData& out);

    bool Declare(std::ostream& stream, const IVariate* var, int tab);
    bool Declare(std::ostream& stream, const IType* type, int tab);

    bool TabLoader(std::ostream& stream, const IType* type, int tab);
    bool JsonLoader(std::ostream& stream, const IType* type, int tab);
}

CFG_NAMESPACE_END
