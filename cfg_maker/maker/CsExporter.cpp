#include "CsExporter.h"
#include "CsDeclare.h"
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
}

void CsExporter::OnEnd()
{
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
}

void CsExporter::OnModule(const IModule* module)
{
}

CFG_NAMESPACE_END
