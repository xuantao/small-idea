#pragma once
#include "CfgDef.h"
#include "Interface.h"

CFG_NAMESPACE_BEGIN

class Variate : public IVariate
{
public:
    Variate(IScope* owner);
    ~Variate();

public:
    virtual ElementCategory ElementCat() const { return ElementCategory::Var; }
    virtual bool IsConst() const { return _const; }
    virtual IType* Belong() const { return nullptr; }
    virtual IScope* Owner() const { return _owner; }
    virtual IType* Type() const { return _type; }
    virtual const std::string& Name() const { return _name; }
    virtual const std::string& Desc() const { return _desc; }
    virtual IValue* Value() const { return _value; }
    virtual bool BindValue(IValue* value);

public:
    void SetType(IType* type) { _type = type; }
    void SetName(const std::string& name) { _name = name; }
    void SetConst() { _const = true; }
    void SetDesc(const std::string& desc) { _desc = desc; }
    void UpgradeArray(int length = 0);

protected:
    IScope* _owner;
    IType* _type;
    std::string _name;
    std::string _desc;
    IValue* _value;
    bool _const;
};

CFG_NAMESPACE_END
