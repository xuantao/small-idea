#pragma once

#include "gcf/gcf.h"
#include <ostream>

GCF_NAMESPACE_BEGIN

namespace cpp
{
    class Serialize : public IExporter
    {
    protected:
        Serialize();
        ~Serialize();

    public:
        static Serialize* Create();

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
        std::ostream& DeclRead(std::ostream& stream, const IType* type, bool isDecl);
        std::ostream& DeclWrite(std::ostream& stream, const IType* type, bool isDecl);
        void ImplRead(const IType* type);
        void ImplWrite(const IType* type);
        void Clear();

    protected:
        bool _isFirst = true;
        int _tab = 0;
        std::ostream* _header;
        std::ostream* _cpp;
    };
}

GCF_NAMESPACE_END
