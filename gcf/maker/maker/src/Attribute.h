#pragma once
#include "gcf/gcf.h"

GCF_NAMESPACE_BEGIN

class Attribute final : public IAttribute
{
public:
    Attribute(const std::string& name, std::vector<IValue*>& params)
        : _name(name)
    {
        _params.swap(params);
    }

    virtual ~Attribute() {}

public:
    const std::string Name() const override { return _name; }
    const IValue* GetParam(int index) const override { return _params[index]; }
    int ParamSize() const override { return (int)_params.size(); }

private:
    std::string _name;
    std::vector<IValue*> _params;
};

class AttributeSet final : public IAttributeSet
{
public:
    virtual ~AttributeSet();

public:
    /* IAttributeSet */
    const IAttribute* Get(const std::string& name) const override;
    const IAttribute* Get(int index) const override;
    int Size() const override { return (int)_attributes.size(); }
    bool Traverse(const std::function<bool(const IAttribute*)>& func) const override;

public:
    bool Push(const std::string& name, std::vector<IValue*>& params);

private:
    std::vector<Attribute*> _attributes;
};

GCF_NAMESPACE_END
