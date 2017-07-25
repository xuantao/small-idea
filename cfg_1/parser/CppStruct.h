#pragma once

#include "IExportObj.h"

CFG_NAMESPACE_BEGIN

class StructType;
class CppStruct : public IExportObj
{
public:
    CppStruct(const StructType* type);
    ~CppStruct();

public:
    virtual std::ostream& operator << (std::ostream& out) { return out; }

public:
    bool Resolve();

protected:
    const StructType* _struct;
    std::vector<ExportVarData> _varDatas;
};

CFG_NAMESPACE_END
