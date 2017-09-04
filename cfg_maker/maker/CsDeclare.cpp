#include "CsDeclare.h"
#include "Utility.h"
#include <fstream>

#define _TAB(ex)    std::string((_tab + ex) * 4, ' ')

CFG_NAMESPACE_BEGIN

namespace cs
{
    CsDeclare::CsDeclare()
    {

    }

    CsDeclare::~CsDeclare()
    {

    }

    bool CsDeclare::Init(const IScope* global, const std::string& path, const std::string& name)
    {
        _global = global;
        _path = path;
        _name = name;
        return true;
    }

    void CsDeclare::DoExport()
    {
        int elements = 0;
        if (_global->VarSet())
            elements += _global->VarSet()->Size();
        if (_global->TypeSet())
            elements += _global->TypeSet()->Size();
        if (_global->NsSet())
            elements += _global->NsSet()->Size();

        if (elements == 0)
            return; // empty

        std::ofstream file(utility::ContactPath(_path, _name) + ".cs");
        if (!file.is_open())
            return;

        file << file << "/*\n * this file is auto generated.\n * please does not edit it manual!\n*/" << std::endl <<
            "using System;" << std::endl <<
            "using System.Collection;" << std::endl <<
            "using System.Collection.General;" << std::endl << std::endl;

        Export(file, _global);
    }

    void CsDeclare::OnType(const IType* type)
    {
        _types.push_back(type);
    }

    void CsDeclare::Export(std::ostream& stream, const IScope* scope)
    {
        IType* bTy = scope->BindNs();
        INamespace* bNs = scope->BindNs();

        if (bTy)
            ExportType(stream, bTy);
        else if (bNs)
            ExportNs(stream, bNs);
        else
            ExportGlobal(stream, scope->VarSet());
    }

    void CsDeclare::ExportType(std::ostream& stream, const IType* type)
    {

    }

    void CsDeclare::ExportNs(std::ostream& stream, const INamespace* ns)
    {

    }

    void CsDeclare::ExportGlobal(std::ostream& stream, const IVarSet* vars)
    {

    }
}

CFG_NAMESPACE_END
