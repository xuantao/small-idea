#pragma once
#include "gcf/gcf.h"
#include <ostream>

GCF_NAMESPACE_BEGIN

namespace cpp
{
    class CrossCall
    {
    protected:
        CrossCall();
        ~CrossCall();
    public:
        static bool Export(const ICrossCall* module, const std::string& path, const std::string& name);

    protected:
        bool Export();

        void CreateFile();
        void DeclHeader();
        void ImplCpp();

        void DeclMessage();
        void DeclExecutor();
        void DeclInvoker();
        void DeclProcessor();

        void ImplInvoker();
        void ImplInvokerFunc(IFunction* func);
        void ImplProcessor();
        void ImplProcessorDetail();
        void ImplProcessorFunc(IFunction* func);

        void DeclFunc(std::ostream& stream, IFunction* func, const std::string& ClassName);
        bool NeedRef(const IType* ty) const;

    protected:
        std::string _path;
        std::string _name;

        int _tab = 0;
        const ICrossCall* _cross = nullptr;
        std::ostream* _header = nullptr;
        std::ostream* _cpp = nullptr;
    };
}

GCF_NAMESPACE_END
