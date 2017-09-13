#pragma once
#include "gcf/gcf.h"
#include <ostream>

GCF_NAMESPACE_BEGIN

namespace cpp
{
    class Declare
    {
    public:
        Declare(const IScope* global, const std::string& path, const std::string& name);
        ~Declare();

    public:
        void DoExport();

        void OnNsBegin(const std::string& name);
        void OnNsEnd();

        void OnInclude(const std::string& file);

        void OnVariate(const IVariate* var);
        void OnType(const IType* type);

    protected:
        bool Decl(const IEnumType* ty);
        bool Decl(const IStructType* ty);
        bool CppVars();

    protected:
        const IScope* _global = nullptr;
        std::string _path;
        std::string _name;
        std::vector<const IVariate*> _vars;

        int _tab = 0;
        bool _lastIsVar = false;
        std::ostream* _header = nullptr;
    };
}

GCF_NAMESPACE_END
