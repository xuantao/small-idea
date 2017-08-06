#include "TabVisitor.h"
#include "ValueUtil.h"

CFG_NAMESPACE_BEGIN

bool TabVisitor::OnStart(const IStructType* sType)
{
    return true;
}

bool TabVisitor::OnEnd()
{
    return true;
}

bool TabVisitor::OnVar(const IVariate* var, const IRawType* rType,
    const std::string& title, const std::string& path)
{
    _title.push_back(title);
    _type.push_back(rType->Name());
    _desc.push_back(var->Desc());

    if (var->Value())
        _def.push_back(value_util::ToString(var->Value()));
    else
        _def.push_back(value_util::DefValue(rType->Raw()));
    return true;
}

bool TabVisitor::OnVar(const IVariate* var, const IEnumType* eType,
    const std::string& title, const std::string& path)
{
    _title.push_back(title);
    _type.push_back(eType->Name());
    _desc.push_back(var->Desc());

    if (var->Value())
        _def.push_back(value_util::ToString(var->Value()));
    else
        _def.push_back("");
    return true;
}

bool TabVisitor::OnVar(const IVariate* var, const IRawType* rType,
    const std::string& title, const std::string& path, int length)
{
    _title.push_back(title);
    if (length > 0)
        _type.push_back(rType->Name() + "[" + std::to_string(length) + "]");
    else
        _type.push_back(rType->Name() + "[" + "]");

    _desc.push_back(var->Desc());
    _def.push_back(value_util::DefValue(rType->Raw()));
    return true;
}

bool TabVisitor::OnVar(const IVariate* var, const IEnumType* eType,
    const std::string& title, const std::string& path, int length)
{
    _title.push_back(title);
    if (length > 0)
        _type.push_back(eType->Name() + "[" + std::to_string(length) + "]");
    else
        _type.push_back(eType->Name() + "[" + "]");

    _desc.push_back(var->Desc());
    _def.push_back("");
    return true;
}
CFG_NAMESPACE_END
