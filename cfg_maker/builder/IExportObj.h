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
    virtual bool Declare(std::ostream& stream, int tab = 0) = 0;
    //virtual bool Json(std::ostream& stream, int tab = 0) = 0;
};

CFG_NAMESPACE_END
