#include "CsCrossCall.h"
#include "CsUtility.h"
#include "utility/Utility.h"
#include <fstream>
#include <iostream>

#define _TAB(ex)    std::string((_tab + ex) * 4, ' ')

GCF_NAMESPACE_BEGIN

namespace cs
{
    bool CrossCall::Export(const ICrossCall* module, const std::string& path, const std::string& name)
    {
        CrossCall md;
        md._module = module;
        md._path = path;
        md._name = name;
        return md.Export();
    }

    CrossCall::CrossCall()
    {
    }

    CrossCall::~CrossCall()
    {
        if (_stream != &std::cout)
            delete _stream;
    }

    bool CrossCall::Export()
    {
        CreateFile();
        DeclHeader();
        return true;
    }

    void CrossCall::CreateFile()
    {
        std::ofstream* file = new std::ofstream(utility::ContactPath(_path, _module->Name()) + ".cs");
        if (file->is_open()) _stream = file;
        else _stream = &std::cout;

        *_stream <<
            _TAB(0) << "/*" << std::endl <<
            _TAB(0) << " * this file is auto generated." << std::endl <<
            _TAB(0) << " * please does not edit it manual!" << std::endl <<
            _TAB(0) << "*/" << std::endl <<
            _TAB(0) << "using System;" << std::endl <<
            _TAB(0) << "using System.Collections;" << std::endl <<
            _TAB(0) << "using System.Collections.Generic;" << std::endl << std::endl;
    }

    void CrossCall::DeclHeader()
    {
        *_stream <<
            _TAB(0) << "public class " << _module->Name() << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(1) << "public const uint MODULE_ID = " << _module->ID() << ";" << std::endl <<
            _TAB(1) << "public const uint HASH_CODE = " << utility::HashValue(_module) << ";" << std::endl;
        ++_tab;

        *_stream << std::endl;
        DeclMessage();

        *_stream << std::endl;
        DeclExecutor();

        *_stream << std::endl;
        DeclInvoker();

        *_stream << std::endl;
        DeclProcessor();

        --_tab;
        *_stream <<
            _TAB(0) << "}" << std::endl;
    }

    void CrossCall::DeclMessage()
    {
        *_stream <<
            _TAB(0) << "public enum Message" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(1) << "Invalid = 0," << std::endl;

        ITypeSet* tySet = _module->Scope()->TypeSet();
        for (int i = 0; i < tySet->Size(); ++i)
        {
            IFunction* func = dynamic_cast<IFunction*>(tySet->Get(i));
            if (func == nullptr)
                continue;

            *_stream << _TAB(1) << func->Name() << "," << std::endl;
        }

        *_stream <<
            _TAB(0) << "}" << std::endl;
    }

    void CrossCall::DeclExecutor()
    {
        *_stream <<
            _TAB(0) << "public interface IResponder" << std::endl <<
            _TAB(0) << "{" << std::endl;

        ITypeSet* tySet = _module->Scope()->TypeSet();
        for (int i = 0; i < tySet->Size(); ++i)
        {
            IFunction* func = dynamic_cast<IFunction*>(tySet->Get(i));
            if (func == nullptr)
                continue;

            *_stream << _TAB(1);
            DeclFunc(*_stream, func);
            *_stream << ";" << std::endl;
        }

        *_stream <<
            _TAB(0) << "};" << std::endl;
    }

    void CrossCall::DeclInvoker()
    {
        *_stream <<
            _TAB(0) << "public class Requester" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(1) << "protected CrossCall.IInvoker _invoker = null;" << std::endl << std::endl <<
            _TAB(1) << "public Requester(CrossCall.IInvoker invoker)" << std::endl <<
            _TAB(1) << "{" << std::endl <<
            _TAB(2) << "_invoker = invoker;" << std::endl <<
            _TAB(1) << "}" << std::endl << std::endl;
        ++_tab;

        ITypeSet* tySet = _module->Scope()->TypeSet();
        for (int i = 0; i < tySet->Size(); ++i)
        {
            IFunction* func = dynamic_cast<IFunction*>(tySet->Get(i));
            if (func == nullptr)
                continue;

            ImplInvokerFunc(func);
            if (i + 1 != tySet->Size())
                *_stream << std::endl;
        }

        --_tab;
        *_stream << _TAB(0) << "}" << std::endl;
    }

    void CrossCall::DeclProcessor()
    {
        *_stream <<
            _TAB(0) << "public class Processor : CrossCall.IProcessor" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(1) << "protected IResponder _responder = null;" << std::endl << std::endl <<
            _TAB(1) << "public Processor(IResponder responder)" << std::endl <<
            _TAB(1) << "{" << std::endl <<
            _TAB(2) << "_responder = responder;" << std::endl <<
            _TAB(1) << "}" << std::endl << std::endl;
        ++_tab;

        ImplProcessorDetail();
        *_stream << std::endl;

        ITypeSet* tySet = _module->Scope()->TypeSet();
        for (int i = 0; i < tySet->Size(); ++i)
        {
            IFunction* func = dynamic_cast<IFunction*>(tySet->Get(i));
            if (func == nullptr)
                continue;

            ImplProcessorFunc(func);
            if (i + 1 != tySet->Size())
                *_stream << std::endl;
        }

        --_tab;
        *_stream << _TAB(0) << "}" << std::endl;
    }

    void CrossCall::ImplInvokerFunc(IFunction* func)
    {
        *_stream << _TAB(0) << "public ";
        DeclFunc(*_stream, func);
        *_stream << std::endl <<
            _TAB(0) << "{" << std::endl;

        *_stream <<
            _TAB(1) << "Serialize.IWriter writer = _invoker.Begin(MODULE_ID);" << std::endl <<
            _TAB(1) << "writer.Write(HASH_CODE);" << std::endl <<
            _TAB(1) << "writer.Write((int)Message." << func->Name() << ");" << std::endl << std::endl;

        IVarSet* varSet = func->Scope()->VarSet();
        for (int i = 0; i < varSet->Size(); ++i)
        {
            IVariate* var = varSet->Get(i);
            *_stream <<
                _TAB(1) << "Serialize.Utility.Write(writer, " << var->Name() << ");" << std::endl;
        }

        *_stream << std::endl;
        if (func->RetType())
        {
            std::string defVal = cs_util::DefValue(func->RetType());

            *_stream << _TAB(1) << cs_util::TypeName(func->RetType()) << " __ret__";
            if (!defVal.empty()) *_stream << " = " << defVal;
            *_stream << ";" << std::endl;

            *_stream <<
                _TAB(1) << "Serialize.Utility.Read(_invoker.End(), ref __ret__);" << std::endl <<
                _TAB(1) << "return __ret__;" << std::endl;
        }
        else
        {
            *_stream <<
                _TAB(1) << "_invoker.End();" << std::endl;
        }

        *_stream << _TAB(0) << "}" << std::endl;
    }

    void CrossCall::ImplProcessorDetail()
    {
        *_stream <<
            _TAB(0) << "public void Process(CrossCall.IContext context)" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(1) << "uint code = 0;" << std::endl <<
            _TAB(1) << "int tmp = 0;" << std::endl <<
            _TAB(1) << "context.Param.Read(ref code);" << std::endl <<
            _TAB(1) << "context.Param.Read(ref tmp);" << std::endl <<
            _TAB(1) << "//TODO: check code == HASH_CODE" << std::endl << std::endl <<
            _TAB(1) << "Message msg = (Message)tmp;" << std::endl <<
            _TAB(1) << "switch (msg)" << std::endl <<
            _TAB(1) << "{" << std::endl;

        ITypeSet* tySet = _module->Scope()->TypeSet();
        for (int i = 0; i < tySet->Size(); ++i)
        {
            IFunction* func = dynamic_cast<IFunction*>(tySet->Get(i));
            if (func == nullptr)
                continue;

            *_stream <<
                _TAB(1) << "case Message." << func->Name() << ":" << std::endl <<
                _TAB(2) << "On" << func->Name() << "(context);" << std::endl <<
                _TAB(2) << "break;" << std::endl;
        }

        *_stream <<
            _TAB(1) << "default:" << std::endl <<
            _TAB(2) << "break;" << std::endl <<
            _TAB(1) << "}" << std::endl <<
            _TAB(0) << "}" << std::endl;
    }

    void CrossCall::ImplProcessorFunc(IFunction* func)
    {
        *_stream <<
            _TAB(0) << "void On" << func->Name() << "(CrossCall.IContext context)" << std::endl <<
            _TAB(0) << "{" << std::endl;

        IVarSet* varSet = func->Scope()->VarSet();
        for (int i = 0; i < varSet->Size(); ++i)
        {
            IVariate* var = varSet->Get(i);
            *_stream <<
                _TAB(1) << cs_util::TypeName(var->Type()) << " " << var->Name();

            std::string defVal = cs_util::DefValue(var->Type());
            if (!defVal.empty()) *_stream << " = " << defVal;
            *_stream << ";" << std::endl;
        }
        *_stream << std::endl;

        for (int i = 0; i < varSet->Size(); ++i)
        {
            IVariate* var = varSet->Get(i);
            *_stream << _TAB(1) << "Serialize.Utility.Read(context.Param, ref " << var->Name() << ");" << std::endl;
        }
        *_stream << std::endl;

        // do call
        *_stream << _TAB(1);
        if (func->RetType()) *_stream << "var __ret__ = ";

        *_stream << "_responder." << func->RawName() << "(";
        for (int i = 0; i < varSet->Size(); ++i)
        {
            IVariate* var = varSet->Get(i);
            *_stream << var->Name();
            if (i + 1 != varSet->Size())
                *_stream << ", ";
        }
        *_stream << ");" << std::endl;

        if (func->RetType()) *_stream << _TAB(1) << "Serialize.Utility.Write(context.Ret(), __ret__);" << std::endl;

        *_stream <<
            _TAB(0) << "}" << std::endl;
    }

    void CrossCall::DeclFunc(std::ostream& stream, IFunction* func)
    {
        if (func->RetType()) stream << cs_util::TypeName(func->RetType()) << " ";
        else stream << "void ";

        stream << func->RawName() << "(";

        IVarSet* varSet = func->Scope()->VarSet();
        for (int i = 0; i < varSet->Size(); ++i)
        {
            IVariate* var = varSet->Get(i);
            IType* ty = var->Type();

            stream << cs_util::TypeName(ty) << " " << var->Name();

            if (i + 1 != varSet->Size())
                stream << ", ";
        }

        stream << ")";
    }
}

GCF_NAMESPACE_END
