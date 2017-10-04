#include "CsExporter.h"
#include "CsDeclare.h"
#include "CsSerialize.h"
#include "CsTab.h"
#include "CsCrossCall.h"
#include "utility/Utility.h"

GCF_NAMESPACE_BEGIN

CsExporter::CsExporter()
{
    _exporters.push_back(cs::Serialize::Create());
    _exporters.push_back(cs::Tab::Create());
}

CsExporter::~CsExporter()
{
    for (auto it = _exporters.rbegin(); it != _exporters.rend(); ++it)
        (*it)->Release();
    _exporters.clear();
}

void CsExporter::Release()
{

}

bool CsExporter::OnBegin(const IScope* global, const char* path, const char* name)
{
    if (path) _path = path;
    else _path = "";

    if (name && *name) _name = name;
    else _name = "unnamed";

    cs::Declare decl;
    decl.Init(global, _path, _name);
    decl.DoExport();

    for (auto exp : _exporters)
        exp->OnBegin(global, _path.c_str(), _name.c_str());
    return true;
}

void CsExporter::OnEnd()
{
    for (auto exp : _exporters)
        exp->OnEnd();
}

void CsExporter::OnNsBegin(const std::string& name)
{
    for (auto exp : _exporters)
        exp->OnNsBegin(name);
}

void CsExporter::OnNsEnd()
{
    for (auto exp : _exporters)
        exp->OnNsEnd();
}

void CsExporter::OnInclude(const std::string& file)
{
    for (auto exp : _exporters)
        exp->OnInclude(file);
}

void CsExporter::OnVariate(const IVariate* var)
{
    for (auto exp : _exporters)
        exp->OnVariate(var);
}

void CsExporter::OnType(const IType* type)
{
    for (auto exp : _exporters)
        exp->OnType(type);
}

void CsExporter::OnCrossCall(const ICrossCall* module)
{
    cs::CrossCall::Export(module, _path, _name);

    for (auto exp : _exporters)
        exp->OnCrossCall(module);
}

GCF_NAMESPACE_END
