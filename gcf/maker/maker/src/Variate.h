#pragma once
#include "gcf/gcf.h"

GCF_NAMESPACE_BEGIN

class Variate : public IVariate
{
public:
    Variate(
        const std::string file,
        int line,
        const std::string& name,
        IScope* owner
    );

    ~Variate();

public:
    /* IElement */
    ElementCategory ElementCat() const override { return ElementCategory::Var; }
    const std::string& File() const override { return _file; }
    int Line() const override { return _line; }
    const IAttributeSet* Attributes() const override { return _attributes; }

    /* IVariate */
    bool IsConst() const override { return _const; }
    IScope* Owner() const override { return _owner; }
    IType* Type() const override { return _type; }
    const std::string& Name() const override { return _name; }
    const std::string& Desc() const override { return _desc; }
    IValue* Value() const override { return _value; }
    bool BindValue(IValue* value) override;

public:
    void Attributes(IAttributeSet* attributs) { _attributes = attributs; }
    IAttributeSet* Attributes() { return _attributes; }
    IType* Belong() const { return nullptr; }
    void SetType(IType* type) { _type = type; }
    void SetName(const std::string& name) { _name = name; }
    void SetConst() { _const = true; }
    void SetDesc(const std::string& desc) { _desc = desc; }
    void UpgradeArray(int length = 0);

protected:
    std::string _file;
    int _line = 0;
    std::string _name;
    std::string _desc;
    IScope* _owner = nullptr;
    IType* _type = nullptr;
    IValue* _value = nullptr;
    bool _const = false;
    IAttributeSet* _attributes = nullptr;
};

GCF_NAMESPACE_END
