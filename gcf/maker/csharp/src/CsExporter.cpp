#include "CsExporter.h"
#include "CsDeclare.h"
#include "CsSerialize.h"
#include "CsModule.h"
#include "utility/Utility.h"

CFG_NAMESPACE_BEGIN

CsExporter::CsExporter()
{
}

CsExporter::~CsExporter()
{
}

void CsExporter::OnBegin(const IScope* global, const std::string& file)
{
    utility::SplitPath(file, &_path, &_name);

    cs::CsDeclare decl;
    decl.Init(global, _path, _name);
    decl.DoExport();

    _ser = new cs::Serialize();
    _ser->Begin(global, _path, _name);
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
    cs::Module::Export(module, _path, _name);
}

CFG_NAMESPACE_END
