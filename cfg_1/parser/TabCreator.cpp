#include "TabCreator.h"
#include "CppUtil.h"
#include <fstream>
#include <iostream>

CFG_NAMESPACE_BEGIN

TabCreator::TabCreator(const std::string& file)
    : _first(true)
    , _file(file)
{ }

bool TabCreator::OnStart(const IStructType* sType)
{
    return true;
}

bool TabCreator::OnEnd()
{
    std::ofstream fs;
    fs.open(_file);
    if (!fs.is_open())
    {
        std::cerr << "open file failed " << _file << std::endl;
        return true;
    }

    fs << _title.str() << std::endl <<
        _type.str() << std::endl <<
        _desc.str() << std::endl <<
        _default.str() << std::endl;
    return true;
}

bool TabCreator::OnVar(const IVariate* var, const IRawType* rType,
    const std::string& title, const std::string& path)
{
    CheckFirst();
    _title << title;
    _type << rType->Name();
    _desc << var->Desc();
    if (var->Value())
        _default << cpp_util::OrignalValue(var->Value());
    else
        _default << cpp_util::DefValue(rType->Raw());
    return true;
}

bool TabCreator::OnVar(const IVariate* var, const IEnumType* eType,
    const std::string& title, const std::string& path)
{
    CheckFirst();
    _title << title;
    _type << eType->Name();
    _desc << var->Desc();
    _default << "";
    return true;
}

bool TabCreator::OnVar(const IVariate* var, const IRawType* rType,
    const std::string& title, const std::string& path, int length)
{
    CheckFirst();
    _title << title;

    _type << rType->Name();
    if (length > 0)
        _type << "[" << length << "]";
    else
        _type << "[]";

    _desc << var->Desc();
    _default << "";
    return true;
}

bool TabCreator::OnVar(const IVariate* var, const IEnumType* eType,
    const std::string& title, const std::string& path, int length)
{
    CheckFirst();
    _title << title;

    _type << eType->Name();
    if (length > 0)
        _type << "[" << length << "]";
    else
        _type << "[]";

    _desc << var->Desc();
    _default << "";
    return true;
}

void TabCreator::CheckFirst()
{
    if (!_first)
    {
        _title << '\t';
        _type << '\t';
        _desc << '\t';
        _default << '\t';
    }
    _first = false;
}

CFG_NAMESPACE_END
