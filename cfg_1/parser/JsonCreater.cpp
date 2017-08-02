﻿#include "JsonCreater.h"
#include "Utility.h"
#include "CppUtil.h"
#include <fstream>
#include <iostream>

CFG_NAMESPACE_BEGIN

#define _DEF_STREAM_    &std::cerr
#define _OUTS_          (*_stream)
#define _TAB_           utility::Tab(*_stream, tab)
#define _TAB_EX_(ex)    std::string((tab + ex) * 4, ' ')

JsonCreater::JsonCreater()
    : _stream(_DEF_STREAM_)
{
}

void JsonCreater::SetPath(const std::string& path)
{
    _path = path;
}

bool JsonCreater::OnStart(const IStructType* sType)
{
    std::ofstream* out = new std::ofstream();
    out->open(_path + "/" + sType->Name() + ".json");
    if (!out->is_open())
    {
        delete out;
        return false;
    }

    _stream = out;

    Create(sType, 0);

    return true;
}

bool JsonCreater::OnEnd()
{
    if (_stream != _DEF_STREAM_)
    {
        static_cast<std::ofstream*>(_stream)->close();
        delete _stream;
        _stream = _DEF_STREAM_;
    }

    return true;
}

void JsonCreater::Create(const IStructType* sType, int tab)
{
    const IVarSet* vars = sType->VarSet();

    _OUTS_ << "{" << std::endl;

    for (int i = 0; i < vars->Size(); ++i)
    {
        const IVariate* var = vars->Get(i);
        const IType* type = var->Type();
        if (i) _OUTS_ << "," << std::endl;

        _OUTS_ << _TAB_EX_(1) << "\"" << var->Name() << "\":";

        if (type->Category() == TypeCategory::Raw)
        {
            const IRawType* rType = static_cast<const IRawType*>(type);
            std::string str = cpp_util::OrignalValue(var->Value());
            if (rType->Raw() == RawCategory::String && str.empty())
                str = "\"\"";
            _OUTS_ << str;
        }
        else if (type->Category() == TypeCategory::Enum)
        {
            _OUTS_ << "0";
        }
        else if (type->Category() == TypeCategory::Struct)
        {
            Create(static_cast<const IStructType*>(type), tab + 1);
        }
        else if (type->Category() == TypeCategory::Array)
        {
            const IArrayType* aTy = static_cast<const IArrayType*>(type);
            const IType* original = aTy->Original();
            _OUTS_ << "[";
            if (original->Category() == TypeCategory::Raw)
            {
                if (aTy->Length() > 0)
                {
                    for (int j = 0; j < aTy->Length(); ++j)
                    {
                        if (j) _OUTS_ << ",";
                        _OUTS_ << cpp_util::DefValue(static_cast<const IRawType*>(original)->Raw());
                    }
                }
            }
            else if (original->Category() == TypeCategory::Enum)
            {
                if (aTy->Length() > 0)
                {
                    for (int j = 0; j < aTy->Length(); ++j)
                    {
                        if (j) _OUTS_ << ",";
                        _OUTS_ << "0";
                    }
                }
            }
            else if (original->Category() == TypeCategory::Struct)
            {
                if (aTy->Length() > 0)
                {
                    for (int j = 0; j < aTy->Length(); ++j)
                    {
                        if (j) _OUTS_ << "," << std::endl << _TAB_EX_(2);
                        Create(static_cast<const IStructType*>(original), tab + 2);
                    }
                }
            }
            _OUTS_ << "]";
        }
    }

    _OUTS_ << std::endl << _TAB_EX_(0) << "}";
}

CFG_NAMESPACE_END
