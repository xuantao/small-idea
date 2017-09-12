/*
 * 作用域对象
*/
#pragma once
#include "gcf/gcf.h"

CFG_NAMESPACE_BEGIN

class NormalScope : public IScope
{
public:
    NormalScope(const std::string& name, IScope* owner)
        : _name(name)
        , _owner(owner)
        , _bindType(nullptr)
        , _bindNs(nullptr)
        , _tySet(nullptr)
        , _varSet(nullptr)
        , _nsSet(nullptr)
    {
    }

    ~NormalScope() {}

public:
    virtual const std::string& Name() const { return _name; }
    virtual IType* BindType() const { return _bindType; }
    virtual INamespace* BindNs() const { return _bindNs; }
    virtual IScope* Owner() const { return _owner; }

    virtual ITypeSet* TypeSet() const { return _tySet; }
    virtual IVarSet* VarSet() const { return _varSet; }
    virtual INsSet* NsSet() const { return _nsSet; }

    virtual IElement* GetElement(const std::string& name) const;
    virtual IScope* GetScope(const std::string& name) const;

    void Bind(IType* type) { _bindType = type; }
    void Bind(INamespace* ns) { _bindNs = ns; }
    void TypeSet(ITypeSet* set) { _tySet = set; }
    void VarSet(IVarSet* set) { _varSet = set; }
    void NsSet(INsSet* set) { _nsSet = set; }

protected:
    std::string _name;
    IScope* _owner;
    IType* _bindType;
    INamespace* _bindNs;
    ITypeSet* _tySet;
    IVarSet* _varSet;
    INsSet* _nsSet;
};

CFG_NAMESPACE_END
