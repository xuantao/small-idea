#pragma once

#include "CfgDef.h"

CFG_NAMESPACE_BEGIN

struct ExportVarData
{
    std::string type;
    std::string name;
    std::string value;
    const IVariate* var;
};

class IExportObj
{
public:
    virtual std::ostream& operator << (std::ostream& out) = 0;
};

CFG_NAMESPACE_END
