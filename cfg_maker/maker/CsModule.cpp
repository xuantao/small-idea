#include "CsModule.h"
#include "Utility.h"
#include "CsUtility.h"
#include <fstream>
#include <iostream>

#define _TAB(ex)    std::string((_tab + ex) * 4, ' ')

CFG_NAMESPACE_BEGIN

namespace cs
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
        if (_stream != &std::cout)
            delete _stream;
    }

    bool Module::Export()
    {
        CreateFile();
        DeclHeader();
        return true;
    }

    void Module::CreateFile()
    {
        std::ofstream* file = new std::ofstream(utility::ContactPath(_path, _module->Name()) + ".cs");
        if (file->is_open()) _stream = file;
        else _stream = &std::cout;

        *_stream <<
            _TAB(0) << "/*" << std::endl << 
            _TAB(0) << " * this file is auto generated." << std::endl <<
            _TAB(0) << " * please does not edit it manual!" << std::endl<<
            _TAB(0) << "*/" << std::endl <<
            _TAB(0) << "using System;" << std::endl <<
            _TAB(0) << "using System.Collections;" << std::endl <<
            _TAB(0) << "using System.Collections.Generic;" << std::endl << std::endl;
    }

    void Module::DeclHeader()
    {
        *_stream <<
            _TAB(0) << "public class " << _module->Name() << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(1) << "public const uint MODULE_ID = " << 1 << ";" << std::endl <<
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

    void Module::DeclMessage()
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

            *_stream << _TAB(1) << "Msg_" << func->Name() << "," << std::endl;
        }

        *_stream <<
            _TAB(0) << "}" << std::endl;
    }

    void Module::DeclExecutor()
    {
        *_stream <<
            _TAB(0) << "public interface IExecutor" << std::endl <<
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

    void Module::DeclInvoker()
    {
        *_stream <<
            _TAB(0) << "public class Invoker" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(1) << "protected CrossCall.ICrossCaller _caller = null;" << std::endl << std::endl <<
            _TAB(1) << "public Invoker(CrossCall.ICrossCaller caller)" << std::endl <<
            _TAB(1) << "{" << std::endl <<
            _TAB(2) << "_caller = caller;" << std::endl <<
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

    void Module::DeclProcessor()
    {
        *_stream <<
            _TAB(0) << "public class Processor : CrossCall.IProcessor" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(1) << "protected IExecutor _executor = null;" << std::endl << std::endl <<
            _TAB(1) << "public Processor(IExecutor executor)" << std::endl <<
            _TAB(1) << "{" << std::endl <<
            _TAB(2) << "_executor = executor;" << std::endl <<
            _TAB(1) << "}" << std::endl << std::endl <<
            _TAB(1) << "public uint ModuleID() { return MODULE_ID; }" << std::endl <<
            _TAB(1) << "public uint HashCode() { return HASH_CODE; }" << std::endl << std::endl;
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

    void Module::ImplInvokerFunc(IFunction* func)
    {
        *_stream << _TAB(0);
        DeclFunc(*_stream, func);
        *_stream << std::endl <<
            _TAB(0) << "{" << std::endl;

        if (func->RetType())
        {
            std::string defVal = cs_util::DefValue(func->RetType());
            *_stream << _TAB(1) << cs_util::TypeName(func->RetType()) << " ret";
            if (!defVal.empty()) *_stream << " = " << defVal;
            *_stream << ";" << std::endl;
        }

        *_stream <<
            _TAB(1) << "Serialize.IWriter writer = _caller.BeginCall(MODULE_ID, HASH_CODE);" << std::endl <<
            _TAB(1) << "writer.Write((int)Message.Msg_" << func->Name() << ");" << std::endl << std::endl;

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
            *_stream <<
                _TAB(1) << "Serialize.IReader reader = _caller.EndCall();" << std::endl <<
                _TAB(1) << "Serialize.Utility.Read(reader, ref ret);" << std::endl <<
                _TAB(1) << "return ret;" << std::endl;
        }
        else
        {
            *_stream <<
                _TAB(1) << "_caller.EndCall();" << std::endl;
        }

        *_stream << _TAB(0) << "}" << std::endl;
    }

    void Module::ImplProcessorDetail()
    {
        *_stream <<
            _TAB(0) << "public void Process(Serialize.IReader reader, Serialize.IWriter writer)" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(1) << "int tmp = 0;" << std::endl <<
            _TAB(1) << "reader.Read(ref tmp);" << std::endl <<
            _TAB(1) << "Message msg = (Message)tmp;" << std::endl << std::endl <<
            _TAB(1) << "switch (msg)" << std::endl <<
            _TAB(1) << "{" << std::endl;

        ITypeSet* tySet = _module->Scope()->TypeSet();
        for (int i = 0; i < tySet->Size(); ++i)
        {
            IFunction* func = dynamic_cast<IFunction*>(tySet->Get(i));
            if (func == nullptr)
                continue;

            *_stream <<
                _TAB(1) << "case Message.Msg_" << func->Name() << ": On" << func->Name() << "(reader, writer); break;" << std::endl;
        }

        *_stream <<
            _TAB(1) << "default: break;" << std::endl <<
            _TAB(1) << "}" << std::endl <<
            _TAB(0) << "}" << std::endl;
    }

    void Module::ImplProcessorFunc(IFunction* func)
    {
        *_stream << 
            _TAB(0) << "void On" << func->Name() << "(Serialize.IReader reader, Serialize.IWriter writer)" << std::endl <<
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
            *_stream << _TAB(1) << "Serialize.Utility.Read(reader, ref " << var->Name() << ");" << std::endl;
        }
        *_stream << std::endl;

        // do call
        *_stream << _TAB(1);
        if (func->RetType()) *_stream << "Serialize.Utility.Write(writer, ";
        *_stream << "_executor." << func->Name() << "(";
        for (int i = 0; i < varSet->Size(); ++i)
        {
            IVariate* var = varSet->Get(i);
            *_stream << var->Name();
            if (i + 1 != varSet->Size())
                *_stream << ", ";
        }
        if (func->RetType()) *_stream << ")";
        *_stream << ");" << std::endl;

        *_stream <<
            _TAB(0) << "}" << std::endl;
    }

    void Module::DeclFunc(std::ostream& stream, IFunction* func)
    {
        if (func->RetType()) stream << cs_util::TypeName(func->RetType()) << " ";
        else stream << "void ";

        stream << func->Name() << "(";

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

CFG_NAMESPACE_END
