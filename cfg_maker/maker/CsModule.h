#pragma once
#include "Interface.h"
#include <ostream>

CFG_NAMESPACE_BEGIN

namespace cs
{
    class Module
    {
    protected:
        Module(const IModule* module, const std::string& path, const std::string& name);
        ~Module();
    public:
        static bool Export(const IModule* module, const std::string& path, const std::string& name);

    protected:
        bool Export();

        void CreateFile();
        void DeclHeader();

        void DeclMessage();
        void DeclExecutor();
        void DeclInvoker();
        void DeclProcessor();

        void ImplInvoker();
        void ImplInvokerFunc(IFunction* func);
        void ImplProcessorDetail();
        void ImplProcessorFunc(IFunction* func);

        void DeclFunc(std::ostream& stream, IFunction* func);

    protected:
        std::string _path;
        std::string _name;

        int _tab = 0;
        const IModule* _module = nullptr;
        std::ostream* _stream = nullptr;
    };
}

CFG_NAMESPACE_END
