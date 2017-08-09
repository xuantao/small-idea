/*
 * 作用域对象
*/
#pragma once
#include "Interface.h"

CFG_NAMESPACE_BEGIN

class NormalScope : public IScope
{
public:
    NormalScope(const std::string& name, IScope* owner)
        : _name(name)
        , _owner(owner)
        , _binding(nullptr)
        , _tySet(nullptr)
        , _varSet(nullptr)
        , _nsSet(nullptr)
    {
    }

    ~NormalScope() {}

public:
    virtual const std::string& Name() const { return _name; }
    virtual IType* Binding() const { return _binding; }
    virtual IScope* Owner() const { return _owner; }

    virtual ITypeSet* TypeSet() const { return _tySet; }
    virtual IVarSet* VarSet() const { return _varSet; }
    virtual INsSet* NsSet() const { return _nsSet; }

    virtual IElement* Get(const std::string& name) const;

    void Bind(IType* type) { _binding = type; }
    void TypeSet(ITypeSet* set) { _tySet = set; }
    void VarSet(IVarSet* set) { _varSet = set; }
    void NsSet(INsSet* set) { _nsSet = set; }

protected:
    std::string _name;
    IScope* _owner;
    IType* _binding;
    ITypeSet* _tySet;
    IVarSet* _varSet;
    INsSet* _nsSet;
};

CFG_NAMESPACE_END
