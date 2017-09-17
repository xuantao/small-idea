#include "CppExporter.h"
#include "CppUtil.h"
#include "CppDeclare.h"
#include "CppCrossCall.h"
#include "CppSerialize.h"
#include "CppTab.h"
#include "CppEnum.h"
#include "utility/Utility.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <set>

GCF_NAMESPACE_BEGIN

#define _NEED_TEMP_INT      0x01
#define _NEED_TEMP_BUFFER   0x02
#define _NEED_TEMP_ARRAY    0x04

#define _DEF_STREAM_    &std::cerr
#define _OUTS_          (*_stream)
#define _TAB_           utility::Tab(*_stream, tab)
#define _TAB_EX_(ex)    std::string((tab + ex) * 4, ' ')

CppExporter::CppExporter()
    : _global(nullptr)
{
    _exporters.push_back(cpp::Declare::Create());
    _exporters.push_back(cpp::Serialize::Create());
    _exporters.push_back(cpp::Tab::Create());
    _exporters.push_back(cpp::Enum::Create());
}

CppExporter::~CppExporter()
{
    for (auto it = _exporters.rbegin(); it != _exporters.rend(); ++it)
        (*it)->Release();
    _exporters.clear();
}

void CppExporter::Release()
{
}

bool CppExporter::OnBegin(const IScope* global, const char* path, const char* name)
{
    _global = global;

    if (path) _path = path;
    else _path = "";

    if (name && *name) _name = name;
    else _name = "unnamed";

    for (auto exp : _exporters)
        exp->OnBegin(global, _path.c_str(), _name.c_str());

    return true;
}

void CppExporter::OnEnd()
{
    for (auto exp : _exporters)
        exp->OnEnd();
}

void CppExporter::OnNsBegin(const std::string& name)
{
    for (auto exp : _exporters)
        exp->OnNsBegin(name);
}

void CppExporter::OnNsEnd()
{
    for (auto exp : _exporters)
        exp->OnNsEnd();
}

void CppExporter::OnInclude(const std::string& file)
{
    for (auto exp : _exporters)
        exp->OnInclude(file);
}

void CppExporter::OnVariate(const IVariate* var)
{
    for (auto exp : _exporters)
        exp->OnVariate(var);
}

void CppExporter::OnType(const IType* type)
{
    for (auto exp : _exporters)
        exp->OnType(type);
}

void CppExporter::OnCrossCall(const ICrossCall* module)
{
    cpp::CrossCall::Export(module, _path, _name);

    for (auto exp : _exporters)
        exp->OnCrossCall(module);
}

GCF_NAMESPACE_END
