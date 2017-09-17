/*
 * 导出数据结构
*/
#pragma once

#include "gcf/gcf.h"
#include <ostream>

GCF_NAMESPACE_BEGIN

namespace cs
{
    class Declare
    {
    public:
        Declare();
        ~Declare();

    public:
        bool Init(const IScope* global, const std::string& path, const std::string& name);
        void DoExport();
        void OnType(const IType* type);

    protected:
        void ExportType(std::ostream& stream, const IEnumType* type);
        void ExportType(std::ostream& stream, const IStructType* type);
        void ExportNs(std::ostream& stream, const INamespace* ns);
        void ExportVars(std::ostream& stream, const IVarSet* vars, const IScope* scope, bool isGlobal);

    protected:
        const IScope* _global = nullptr;
        std::string _path;
        std::string _name;
        int _tab = 0;
        std::ostream* _stream = nullptr;
        std::vector<const IVariate*> _vars;
        std::vector<const IType*> _types;
    };
}

GCF_NAMESPACE_END
