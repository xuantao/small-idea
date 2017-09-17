#pragma once
#include "gcf/gcf.h"
#include <fstream>

GCF_NAMESPACE_BEGIN

namespace cpp
{
    class Enum : public IExporter
    {
    protected:
        Enum();
        ~Enum();

        Enum(const Enum&) = delete;
        Enum& operator = (const Enum&) = delete;
    public:
        static Enum* Create();

    public:
        virtual void Release();
    public:
        virtual bool OnBegin(const IScope* global, const char* path, const char* name);
        virtual void OnEnd();
        virtual void OnType(const IType* type);

    public:
        virtual void OnNsBegin(const std::string& name) {}
        virtual void OnNsEnd() {}
        virtual void OnInclude(const std::string& file) {}
        virtual void OnVariate(const IVariate* var) {}
        virtual void OnCrossCall(const ICrossCall* module) {}

    protected:
        void Decl(const IEnumType* type);
        void Impl(const IEnumType* type);

        void CreateFile();
        void Clear();

    protected:
        int _tab = 0;
        bool _hasCreated = false;
        std::string _name;
        std::string _file;
        std::ofstream* _header = nullptr;
        std::ofstream* _cpp = nullptr;
    };
}

GCF_NAMESPACE_END
