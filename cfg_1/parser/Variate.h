#pragma once

#include "CfgDef.h"
#include <string>
#include <vector>

CFG_NAMESPACE_BEGIN

class Variate : public IVariate
{
public:
    Variate(const IType* belong, const IType* type, const std::string& name);
    ~Variate();

public:
    virtual bool IsConst() { return false; }
    virtual const IType* Belong() const { return _belong; }
    virtual const IType* Type() const { return _type; }
    virtual const std::string& Name() const { return _name; }
    virtual IValue* Value() const { return _value; }

public:
    void UpgradeArray(int length = -1);
    void BindValue(IValue* value);

protected:
    const IType* _belong;
    const IType* _type;
    std::string _name;
    IValue* _value;
};

CFG_NAMESPACE_END
