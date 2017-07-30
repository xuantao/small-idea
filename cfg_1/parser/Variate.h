#pragma once
#include "CfgDef.h"
#include "Interface.h"

CFG_NAMESPACE_BEGIN

class Variate : public IVariate
{
public:
    Variate(IType* belong, IType* type, const std::string& name);
    ~Variate();

public:
    virtual bool IsConst() const { return _const; }
    virtual IType* Belong() const { return _belong; }
    virtual IType* Type() const { return _type; }
    virtual const std::string& Name() const { return _name; }
    virtual const std::string& Desc() const { return _desc; }
    virtual IValue* Value() const { return _value; }
    virtual void BindValue(IValue* value);

public:
    void SetConst();
    void SetDesc(const std::string& desc) { _desc = desc; }
    void UpgradeArray(int length = -1);

protected:
    IType* _belong;
    IType* _type;
    std::string _name;
    std::string _desc;
    IValue* _value;
    bool _const;
};

CFG_NAMESPACE_END
