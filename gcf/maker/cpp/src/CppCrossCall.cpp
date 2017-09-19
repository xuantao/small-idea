#include "CppCrossCall.h"
#include "CppUtil.h"
#include "utility/Utility.h"
#include <fstream>
#include <iostream>

#define _TAB(ex)    std::string((_tab + ex) * 4, ' ')

GCF_NAMESPACE_BEGIN

namespace cpp
{
    bool CrossCall::Export(const ICrossCall* cross, const std::string& path, const std::string& name)
    {
        CrossCall md;
        md._cross = cross;
        md._path = path;
        md._name = name;
        return md.Export();
    }

    CrossCall::CrossCall()
    {
    }

    CrossCall::~CrossCall()
    {
        if (_header != &std::cout)
            delete _header;

        if (_cpp != &std::cout)
            delete _cpp;
    }

    bool CrossCall::Export()
    {
        CreateFile();
        DeclHeader();
        ImplCpp();
        return true;
    }

    void CrossCall::CreateFile()
    {
        // header
        std::ofstream* file = new std::ofstream(utility::ContactPath(_path, _cross->Name()) + ".h");
        if (file->is_open()) _header = file;
        else _header = &std::cout;

        *_header <<
            _TAB(0) << "/*" << std::endl <<
            _TAB(0) << " * this file is auto generated." << std::endl <<
            _TAB(0) << " * please does not edit it manual!" << std::endl <<
            _TAB(0) << "*/" << std::endl <<
            _TAB(0) << "#pragma once" << std::endl << std::endl <<
            _TAB(0) << "#include \"" << _name << ".h\"" << std::endl <<
            _TAB(0) << "#include \"gcf/gcf.h\"" << std::endl << std::endl;

        // cpp
        file = new std::ofstream(utility::ContactPath(_path, _cross->Name()) + ".cpp");
        if (file->is_open()) _cpp = file;
        else _cpp = &std::cout;

        *_cpp <<
            _TAB(0) << "/*" << std::endl <<
            _TAB(0) << " * this file is auto generated." << std::endl <<
            _TAB(0) << " * please does not edit it manual!" << std::endl <<
            _TAB(0) << "*/" << std::endl <<
            _TAB(0) << "#include \"" << _cross->Name() << ".h\"" << std::endl <<
            _TAB(0) << "#include \"" << _name << "_Ser.h\"" << std::endl <<
            _TAB(0) << "#include <cassert>" << std::endl << std::endl;
        //TODO: need include more file
    }

    void CrossCall::DeclHeader()
    {
        *_header <<
            _TAB(0) << "class " << _cross->Name() << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(0) << "public:" << std::endl <<
            _TAB(1) << "static const int32_t MODULE_ID = " << _cross->ID() << ";" << std::endl <<
            _TAB(1) << "static const int32_t HASH_CODE = " << utility::HashValue(_cross) << ";" << std::endl;
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

    void CrossCall::ImplCpp()
    {
        ImplInvoker();
        ImplProcessor();
    }

    void CrossCall::DeclMessage()
    {
        *_header <<
            _TAB(0) << "enum class Message" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(1) << "Invalid," << std::endl;

        ITypeSet* tySet = _cross->Scope()->TypeSet();
        for (int i = 0; i < tySet->Size(); ++i)
        {
            IFunction* func = dynamic_cast<IFunction*>(tySet->Get(i));
            if (func == nullptr)
                continue;

            *_header << _TAB(1) << func->Name() << "," << std::endl;
        }

        *_header <<
            _TAB(0) << "};" << std::endl;
    }

    void CrossCall::DeclExecutor()
    {
        *_header <<
            _TAB(0) << "class IResponder" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(0) << "public:" << std::endl <<
            _TAB(1) << "virtual ~IResponder() {}" << std::endl << std::endl <<
            _TAB(0) << "public:" << std::endl;

        ITypeSet* tySet = _cross->Scope()->TypeSet();
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

    void CrossCall::DeclInvoker()
    {
        *_header <<
            _TAB(0) << "class Requester" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(0) << "public:" << std::endl <<
            _TAB(1) << "Requester(cross_call::IInvoker* invoker) : _invoker(invoker)" << " { }" << std::endl << std::endl <<
            _TAB(0) << "public:" << std::endl;

        ITypeSet* tySet = _cross->Scope()->TypeSet();
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
            _TAB(1) << "cross_call::IInvoker* _invoker = nullptr;" << std::endl <<
            _TAB(0) << "};" << std::endl;
    }

    void CrossCall::DeclProcessor()
    {
        *_header <<
            _TAB(0) << "class Processor : public cross_call::IProcessor" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(0) << "public:" << std::endl <<
            _TAB(1) << "Processor(IResponder* responder) : _responder(responder)" << " { }" << std::endl <<
            _TAB(1) << "virtual ~Processor() { }" << std::endl << std::endl <<
            _TAB(0) << "public:" << std::endl <<
            _TAB(1) << "virtual void Process(cross_call::IContext* context);" << std::endl << std::endl <<
            _TAB(0) << "protected:" << std::endl;

        ITypeSet* tySet = _cross->Scope()->TypeSet();
        for (int i = 0; i < tySet->Size(); ++i)
        {
            IFunction* func = dynamic_cast<IFunction*>(tySet->Get(i));
            if (func == nullptr)
                continue;

            *_header << _TAB(1) << "void On" << func->Name() << "(cross_call::IContext* context);" << std::endl;
        }

        *_header << std::endl <<
            _TAB(0) << "protected:" << std::endl <<
            _TAB(1) << "IResponder* _responder = nullptr;" << std::endl <<
            _TAB(0) << "};" << std::endl;
    }

    void CrossCall::ImplInvoker()
    {
        std::string className = _cross->Name() + "::Requester";

        ITypeSet* tySet = _cross->Scope()->TypeSet();
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

    void CrossCall::ImplInvokerFunc(IFunction* func)
    {
        *_cpp <<
            _TAB(0) << "serialize::IWriter* writer = _invoker->Begin(MODULE_ID);" << std::endl <<
            _TAB(0) << "writer->Write(HASH_CODE);" << std::endl <<
            _TAB(0) << "writer->Write((int)Message::" << func->Name() << ");" << std::endl << std::endl;

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
                _TAB(0) << "serialize::utility::Read(_invoker->End(), __ret__);" << std::endl <<
                _TAB(0) << "return __ret__;" << std::endl;
        }
        else
        {
            *_cpp <<
                _TAB(0) << "_invoker->End();" << std::endl;
        }
    }

    void CrossCall::ImplProcessor()
    {
        ImplProcessorDetail();
        *_cpp << std::endl;

        std::string className = _cross->Name() + "::Processor";
        ITypeSet* tySet = _cross->Scope()->TypeSet();
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

    void CrossCall::ImplProcessorDetail()
    {
        *_cpp <<
            _TAB(0) << "void " << _cross->Name() << "::Processor::Process(cross_call::IContext* context)" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(1) << "int32_t code = 0;" << std::endl <<
            _TAB(1) << "Message msg = Message::Invalid;" << std::endl <<
            _TAB(1) << "context->Param()->Read(code);" << std::endl <<
            _TAB(1) << "context->Param()->Read((int&)msg);" << std::endl <<
            _TAB(1) << "assert(code == HASH_CODE);" << std::endl << std::endl <<
            _TAB(1) << "switch (msg)" << std::endl <<
            _TAB(1) << "{" << std::endl;

        ITypeSet* tySet = _cross->Scope()->TypeSet();
        for (int i = 0; i < tySet->Size(); ++i)
        {
            IFunction* func = dynamic_cast<IFunction*>(tySet->Get(i));
            if (func == nullptr)
                continue;

            *_cpp <<
                _TAB(1) << "case Message::" << func->Name() << ":" << std::endl <<
                _TAB(2) << "On" << func->Name() << "(context);" << std::endl <<
                _TAB(2) << "break;" << std::endl;
        }

        *_cpp <<
            _TAB(1) << "default:" << std::endl <<
            _TAB(2) << "assert(false);" << std::endl <<
            _TAB(2) << "break;" << std::endl <<
            _TAB(1) << "}" << std::endl <<
            _TAB(0) << "}" << std::endl;
    }

    void CrossCall::ImplProcessorFunc(IFunction* func)
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

        *_cpp << "_responder->" << func->RawName() << "(";
        for (int i = 0; i < varSet->Size(); ++i)
        {
            IVariate* var = varSet->Get(i);
            *_cpp << var->Name();
            if (i + 1 != varSet->Size())
                *_cpp << ", ";
        }
        *_cpp << ");" << std::endl;

        if (func->RetType())
            *_cpp << _TAB(0) << "serialize::utility::Write(context->Ret(), __ret__);" << std::endl;
    }

    void CrossCall::DeclFunc(std::ostream& stream, IFunction* func, const std::string& ClassName)
    {
        if (func->RetType()) stream << cpp_util::TypeName(func->RetType()) << " ";
        else stream << "void ";

        if (!ClassName.empty()) stream << ClassName << "::";

        stream << func->RawName() << "(";

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

    bool CrossCall::NeedRef(const IType* ty) const
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

GCF_NAMESPACE_END
