#include "CppModule.h"
#include "Utility.h"
#include "CppUtil.h"
#include <fstream>
#include <iostream>

#define _TAB(ex)    std::string((_tab + ex) * 4, ' ')

CFG_NAMESPACE_BEGIN

namespace cpp
{
    bool Module::Export(const IModule* module, const std::string& path, const std::string& name)
    {
        Module md(module, path, name);
        return md.Export();
    }

    Module::Module(const IModule* module, const std::string& path, const std::string& name)
        : _module(module)
        , _path(path)
        , _name(name)
    {
    }

    Module::~Module()
    {
        if (_header != &std::cout)
            delete _header;

        if (_cpp != &std::cout)
            delete _cpp;
    }

    bool Module::Export()
    {
        CreateFile();
        DeclHeader();
        ImplCpp();
        return true;
    }

    void Module::CreateFile()
    {
        // header
        std::ofstream* file = new std::ofstream(utility::ContactPath(_path, _module->Name()) + ".h");
        if (file->is_open()) _header = file;
        else _header = &std::cout;

        *_header <<
            _TAB(0) << "/*" << std::endl <<
            _TAB(0) << " * this file is auto generated." << std::endl <<
            _TAB(0) << " * please does not edit it manual!" << std::endl <<
            _TAB(0) << "*/" << std::endl <<
            _TAB(0) << "#pragma once" << std::endl << std::endl <<
            _TAB(0) << "#include \"" << _name << ".h\"" << std::endl <<
            _TAB(0) << "#include \"ICrossCall.h\"" << std::endl << std::endl;

        // cpp
        file = new std::ofstream(utility::ContactPath(_path, _module->Name()) + ".cpp");
        if (file->is_open()) _cpp = file;
        else _cpp = &std::cout;

        *_cpp <<
            _TAB(0) << "/*" << std::endl <<
            _TAB(0) << " * this file is auto generated." << std::endl <<
            _TAB(0) << " * please does not edit it manual!" << std::endl <<
            _TAB(0) << "*/" << std::endl <<
            _TAB(0) << "#include \"" << _module->Name() << ".h\"" << std::endl <<
            _TAB(0) << "#include \"SerUtility.h\"" << std::endl <<
            _TAB(0) << "#include \"" << _name << "_Ser.h\"" << std::endl <<
            _TAB(0) << "#include <cassert>" << std::endl << std::endl;
        //TODO: need include more file
    }

    void Module::DeclHeader()
    {
        *_header <<
            _TAB(0) << "class " << _module->Name() << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(0) << "public:" << std::endl <<
            _TAB(1) << "static const uint32_t MODULE_ID = " << 1 << ";" << std::endl <<
            _TAB(1) << "static const uint32_t HASH_CODE = " << utility::HashValue(_module) << ";" << std::endl;
        ++_tab;

        *_header << std::endl;
        DeclMessage();

        *_header << std::endl;
        DeclExecutor();

        *_header << std::endl;
        DeclInvoker();

        *_header << std::endl;
        DeclProcessor();

        --_tab;
        *_header <<
            _TAB(0) << "};" << std::endl;
    }

    void Module::ImplCpp()
    {
        ImplInvoker();
        ImplProcessor();
    }

    void Module::DeclMessage()
    {
        *_header <<
            _TAB(0) << "enum class Message" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(1) << "Invalid," << std::endl;

        ITypeSet* tySet = _module->Scope()->TypeSet();
        for (int i = 0; i < tySet->Size(); ++i)
        {
            IFunction* func = dynamic_cast<IFunction*>(tySet->Get(i));
            if (func == nullptr)
                continue;

            *_header << _TAB(1) << "Msg_" << func->Name() << "," << std::endl;
        }

        *_header <<
            _TAB(0) << "};" << std::endl;
    }

    void Module::DeclExecutor()
    {
        *_header <<
            _TAB(0) << "class IExecutor" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(0) << "public:" << std::endl <<
            _TAB(1) << "virtual ~IExecutor() {}" << std::endl << std::endl <<
            _TAB(0) << "public:" << std::endl;

        ITypeSet* tySet = _module->Scope()->TypeSet();
        for (int i = 0; i < tySet->Size(); ++i)
        {
            IFunction* func = dynamic_cast<IFunction*>(tySet->Get(i));
            if (func == nullptr)
                continue;

            *_header << _TAB(1) << "virtual ";
            DeclFunc(*_header, func, utility::EMPTY_STR);
            *_header << " = 0;" << std::endl;
        }

        *_header <<
            _TAB(0) << "};" << std::endl;
    }

    void Module::DeclInvoker()
    {
        *_header <<
            _TAB(0) << "class Invoker" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(0) << "public:" << std::endl <<
            _TAB(1) << "Invoker(cross_call::ICross* cross) : _cross(cross)" << " { }" << std::endl << std::endl <<
            _TAB(0) << "public:" << std::endl;

        ITypeSet* tySet = _module->Scope()->TypeSet();
        for (int i = 0; i < tySet->Size(); ++i)
        {
            IFunction* func = dynamic_cast<IFunction*>(tySet->Get(i));
            if (func == nullptr)
                continue;

            *_header << _TAB(1);
            DeclFunc(*_header, func, utility::EMPTY_STR);
            *_header << ";" << std::endl;
        }

        *_header << std::endl <<
            _TAB(0) << "protected:" << std::endl <<
            _TAB(1) << "cross_call::ICross* _cross = nullptr;" << std::endl <<
            _TAB(0) << "};" << std::endl;
    }

    void Module::DeclProcessor()
    {
        *_header <<
            _TAB(0) << "class Processor : public cross_call::IProcessor" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(0) << "public:" << std::endl <<
            _TAB(1) << "Processor(IExecutor* executor) : _executor(executor)" << " { }" << std::endl <<
            _TAB(1) << "virtual ~Processor() { }" << std::endl << std::endl <<
            _TAB(0) << "public:" << std::endl <<
            _TAB(1) << "virtual void Process(cross_call::IContext* context);" << std::endl << std::endl <<
            _TAB(0) << "protected:" << std::endl;

        ITypeSet* tySet = _module->Scope()->TypeSet();
        for (int i = 0; i < tySet->Size(); ++i)
        {
            IFunction* func = dynamic_cast<IFunction*>(tySet->Get(i));
            if (func == nullptr)
                continue;

            *_header << _TAB(1) << "void On" << func->Name() << "(cross_call::IContext* context);" << std::endl;
        }

        *_header << std::endl <<
            _TAB(0) << "protected:" << std::endl <<
            _TAB(1) << "IExecutor* _executor = nullptr;" << std::endl <<
            _TAB(0) << "};" << std::endl;
    }

    void Module::ImplInvoker()
    {
        std::string className = _module->Name() + "::Invoker";

        ITypeSet* tySet = _module->Scope()->TypeSet();
        for (int i = 0; i < tySet->Size(); ++i)
        {
            IFunction* func = dynamic_cast<IFunction*>(tySet->Get(i));
            if (func == nullptr)
                continue;

            DeclFunc(*_cpp, func, className);
            *_cpp << std::endl <<
                _TAB(0) << "{" << std::endl;
            ++_tab;

            ImplInvokerFunc(func);

            --_tab;
            *_cpp <<
                _TAB(0) << "}" << std::endl << std::endl;
        }
    }

    void Module::ImplInvokerFunc(IFunction* func)
    {
        *_cpp <<
            _TAB(0) << "serialize::IWriter* writer = _cross->BeginCall(MODULE_ID);" << std::endl <<
            _TAB(0) << "writer->Write(HASH_CODE);" << std::endl <<
            _TAB(0) << "writer->Write((int)Message::Msg_" << func->Name() << ");" << std::endl << std::endl;

        IVarSet* varSet = func->Scope()->VarSet();
        for (int i = 0; i < varSet->Size(); ++i)
        {
            IVariate* var = varSet->Get(i);
            *_cpp <<
                _TAB(0) << "serialize::utility::Write(writer, " << var->Name() << ");" << std::endl;
        }

        *_cpp << std::endl;
        if (func->RetType())
        {
            *_cpp <<
                _TAB(0) << cpp_util::TypeName(func->RetType()) << " __ret__;" << std::endl <<
                _TAB(0) << "serialize::utility::Read(_cross->EndCall(), __ret__);" << std::endl <<
                _TAB(0) << "return __ret__;" << std::endl;
        }
        else
        {
            *_cpp <<
                _TAB(0) << "_caller->EndCall();" << std::endl;
        }
    }

    void Module::ImplProcessor()
    {
        ImplProcessorDetail();
        *_cpp << std::endl;

        std::string className = _module->Name() + "::Processor";
        ITypeSet* tySet = _module->Scope()->TypeSet();
        for (int i = 0; i < tySet->Size(); ++i)
        {
            IFunction* func = dynamic_cast<IFunction*>(tySet->Get(i));
            if (func == nullptr)
                continue;

            *_cpp <<
                _TAB(0) << "void " << className << "::On" << func->Name() << "(cross_call::IContext* context)" << std::endl <<
                _TAB(0) << "{" << std::endl;
            ++_tab;

            ImplProcessorFunc(func);

            --_tab;
            *_cpp <<
                _TAB(0) << "}" << std::endl << std::endl;
        }
    }

    void Module::ImplProcessorDetail()
    {
        *_cpp <<
            _TAB(0) << "void " << _module->Name() << "::Processor::Process(cross_call::IContext* context)" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(1) << "uint32_t code = 0" << std::endl <<
            _TAB(1) << "Message msg = Message::Invalid;" << std::endl <<
            _TAB(1) << "reader->Read(code);" << std::endl <<
            _TAB(1) << "reader->Read((int&)msg);" << std::endl <<
            _TAB(1) << "assert(code == HASH_CODE);" << std::endl << std::endl <<
            _TAB(1) << "switch (msg)" << std::endl <<
            _TAB(1) << "{" << std::endl;

        ITypeSet* tySet = _module->Scope()->TypeSet();
        for (int i = 0; i < tySet->Size(); ++i)
        {
            IFunction* func = dynamic_cast<IFunction*>(tySet->Get(i));
            if (func == nullptr)
                continue;

            *_cpp <<
                _TAB(1) << "case Message::Msg_" << func->Name() << ": On" << func->Name() << "(context); break;" << std::endl;
        }

        *_cpp <<
            _TAB(1) << "default: assert(false); break;" << std::endl <<
            _TAB(1) << "}" << std::endl <<
            _TAB(0) << "}" << std::endl;
    }

    void Module::ImplProcessorFunc(IFunction* func)
    {
        IVarSet* varSet = func->Scope()->VarSet();
        for (int i = 0; i < varSet->Size(); ++i)
        {
            IVariate* var = varSet->Get(i);
            *_cpp << _TAB(0) << cpp_util::TypeName(var->Type()) << " " << var->Name() << ";" << std::endl;
        }
        *_cpp << std::endl;

        for (int i = 0; i < varSet->Size(); ++i)
        {
            IVariate* var = varSet->Get(i);
            *_cpp << _TAB(0) << "serialize::utility::Read(context->Param(), " << var->Name() << ");" << std::endl;
        }
        *_cpp << std::endl;

        // do call
        *_cpp << _TAB(0);
        if (func->RetType())
            *_cpp << "auto __ret__ = ";

        *_cpp << "_executor->" << func->Name() << "(";
        for (int i = 0; i < varSet->Size(); ++i)
        {
            IVariate* var = varSet->Get(i);
            *_cpp << var->Name();
            if (i + 1 != varSet->Size())
                *_cpp << ", ";
        }
        *_cpp << ");" << std::endl;

        if (func->RetType())
            *_cpp << _TAB(0) << "serialize::utility::Read(context->Ret(), __ret__);" << std::endl;
    }

    void Module::DeclFunc(std::ostream& stream, IFunction* func, const std::string& ClassName)
    {
        if (func->RetType()) stream << cpp_util::TypeName(func->RetType()) << " ";
        else stream << "void ";

        if (!ClassName.empty()) stream << ClassName << "::";

        stream << func->Name() << "(";

        IVarSet* varSet = func->Scope()->VarSet();
        for (int i = 0; i < varSet->Size(); ++i)
        {
            IVariate* var = varSet->Get(i);
            IType* ty = var->Type();
            if (NeedRef(ty))
                stream << "const ";
            stream << cpp_util::TypeName(ty);
            if (NeedRef(ty))
                stream << "&";

            stream << " " << var->Name();
            if (i + 1 != varSet->Size())
                stream << ", ";
        }

        stream << ")";
    }

    bool Module::NeedRef(const IType* ty) const
    {
        if (
            (ty->TypeCat() == TypeCategory::Raw && ((const IRawType*)ty)->RawCat() != RawCategory::String) ||
            ty->TypeCat() == TypeCategory::Enum
            )
        {
            return false;
        }
        return true;
    }
}

CFG_NAMESPACE_END
