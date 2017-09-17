#pragma once
#include "gcf/gcf.h"
#include <ostream>

GCF_NAMESPACE_BEGIN

namespace cpp
{
    class Declare : public IExporter
    {
    protected:
        Declare();
        ~Declare();

        Declare(const Declare&) = delete;
        Declare& operator = (const Declare&) = delete;
    public:
        static Declare* Create();

    public:
        virtual void Release();

    public:
        virtual bool OnBegin(const IScope* global, const char* path, const char* name);
        virtual void OnEnd();
        virtual void OnNsBegin(const std::string& name);
        virtual void OnNsEnd();

        virtual void OnInclude(const std::string& file);

        virtual void OnVariate(const IVariate* var);
        virtual void OnType(const IType* type);

    public:
        virtual void OnCrossCall(const ICrossCall* module) {}

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
