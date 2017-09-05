#include "CsExporter.h"
#include "CsDeclare.h"
#include "CsSerialize.h"
#include "Utility.h"

CFG_NAMESPACE_BEGIN

CsExporter::CsExporter()
{
}

CsExporter::~CsExporter()
{
}

void CsExporter::OnBegin(const IScope* global, const std::string& file)
{
    std::string path;
    std::string name;
    utility::SplitPath(file, &path, &name);

    cs::CsDeclare decl;
    decl.Init(global, path, name);
    decl.DoExport();

    _ser = new cs::Serialize();
    _ser->Begin(global, path, name);
}

void CsExporter::OnEnd()
{
    _ser->End();
    delete _ser;
    _ser = nullptr;
}

void CsExporter::OnNsBegin(const std::string& name)
{
}

void CsExporter::OnNsEnd()
{
}

void CsExporter::OnInclude(const std::string& file)
{
}

void CsExporter::OnVariate(const IVariate* var)
{
}

void CsExporter::OnType(const IType* type)
{
    _ser->OnType(type);
}

void CsExporter::OnModule(const IModule* module)
{
}

CFG_NAMESPACE_END
