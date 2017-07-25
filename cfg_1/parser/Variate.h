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
    virtual bool IsConst() const { return _const; }
    virtual const IType* Belong() const { return _belong; }
    virtual const IType* Type() const { return _type; }
    virtual const std::string& Name() const { return _name; }
    virtual IValue* Value() const { return _value; }
    virtual void BindValue(IValue* value);

public:
    void SetConst();
    void UpgradeArray(int length = -1);

protected:
    const IType* _belong;
    const IType* _type;
    std::string _name;
    IValue* _value;
    bool _const;
};

CFG_NAMESPACE_END
