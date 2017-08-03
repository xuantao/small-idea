#pragma once

#include "IExportObj.h"
#include "CppUtil.h"

CFG_NAMESPACE_BEGIN

class StructType;
class CppStruct : public IExportObj
{
public:
    CppStruct(const StructType* type);
    ~CppStruct();

public:
    virtual bool Declare(std::ostream& stream, int tab = 0);
    virtual bool Json(std::ostream& stream, const std::string& owner, int tab = 0);

public:
    bool Resolve();

protected:
    bool Json(std::ostream& stream, const IVariate* var, const std::string& owner, int tab);

protected:
    const StructType* _struct;
    bool _contructor;
    std::vector<CppVarData> _consts;
    std::vector<CppVarData> _vars;
};

CFG_NAMESPACE_END
