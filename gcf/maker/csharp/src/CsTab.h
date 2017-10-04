#pragma once
#include "gcf/gcf.h"
#include <ostream>
#include <vector>

GCF_NAMESPACE_BEGIN

namespace cs
{
    class Tab : public IExporter
    {
    protected:
        Tab();
        ~Tab();

        Tab(const Tab&) = delete;
        Tab& operator = (const Tab&) = delete;
    public:
        static Tab* Create();

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
        struct NsTabs
        {
            IScope* scope = nullptr;
            std::vector<const IStructType*> tabs;

            NsTabs* owner = nullptr;
            std::vector<NsTabs*> children;
        };

        NsTabs* Record(IScope* owner);
        void DeclPrev(std::ostream& stream);
        void Decl(std::ostream& stream, NsTabs* ns, int tab);
        void Decl(std::ostream& stream, const IStructType* sTy, int tab);
        void DeclTail(std::ostream& stream);

    protected:
        std::string _name;
        std::string _file;

        int _count = 0;
        NsTabs _root;
    };
}

GCF_NAMESPACE_END
