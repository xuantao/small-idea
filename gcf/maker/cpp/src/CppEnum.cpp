#include "CppEnum.h"
#include "CppUtil.h"
#include "utility/TabVisitor.h"

#define _TAB(ex)    std::string((_tab + ex) * 4, ' ')
GCF_NAMESPACE_BEGIN

namespace cpp
{
    static int sValue(const IValue* val, int def)
    {
        if (val == nullptr)
            return def;
        
        return def;
    }

    Enum::Enum()
    {
    }

    Enum::~Enum()
    {
        Clear();
    }

    Enum* Enum::Create()
    {
        return new Enum();
    }

    void Enum::Release()
    {
        delete this;
    }

    bool Enum::OnBegin(const IScope* global, const char* path, const char* name)
    {
        _file = utility::ContactPath(path, name) + "_Enum";
        _name = name;
        return true;
    }

    void Enum::OnEnd()
    {
        _tab = 1;
        if (_header)
        {
            *_header << 
                _TAB(0) << "}" << std::endl << std::endl <<
                _TAB(0) << "template <class En>" << std::endl <<
                _TAB(0) << "const char* Name(En val)" << std::endl <<
                _TAB(0) << "{" << std::endl <<
                _TAB(1) << "typedef detail::Info<En> Info;" << std::endl << std::endl <<
                _TAB(1) << "for (int i = 0; i < Info::size; ++i)" << std::endl <<
                _TAB(2) << "if (Info::vals[i] == val)" << std::endl <<
                _TAB(3) << "return Info::names[i];" << std::endl << std::endl <<
                _TAB(1) << "return nullptr;" << std::endl <<
                _TAB(0) << "}" << std::endl << std::endl <<

                _TAB(0) << "template <class En>" << std::endl <<
                _TAB(0) << "const char* Name(int idx)" << std::endl <<
                _TAB(0) << "{" << std::endl <<
                _TAB(1) << "typedef detail::Info<En> Info;" << std::endl << std::endl <<
                _TAB(1) << "if (idx >= 0 && idx < Info::size)" << std::endl <<
                _TAB(2) << "return Info::names[idx];" << std::endl <<
                _TAB(1) << "return nullptr;" << std::endl <<
                _TAB(0) << "}" << std::endl << std::endl <<

                _TAB(0) << "template <class En>" << std::endl <<
                _TAB(0) << "En Value(const char* name)" << std::endl <<
                _TAB(0) << "{" << std::endl <<
                _TAB(1) << "typedef detail::Info<En> Info;" << std::endl << std::endl <<
                _TAB(1) << "for (int i = 0; i < Info::size; ++i)" << std::endl <<
                _TAB(2) << "if (std::strcmp(Info::names[i], name) == 0)" << std::endl <<
                _TAB(3) << "return Info::vals[i];" << std::endl << std::endl << std::endl <<
                _TAB(1) << "return Info::vals[0];" << std::endl <<
                _TAB(0) << "}" << std::endl << std::endl <<

                _TAB(0) << "template <class En>" << std::endl <<
                _TAB(0) << "En Value(int idx)" << std::endl <<
                _TAB(0) << "{" << std::endl <<
                _TAB(1) << "typedef detail::Info<En> Info;" << std::endl << std::endl <<
                _TAB(1) << "if (idx >= 0 && idx < Info::size)" << std::endl <<
                _TAB(2) << "return Info::vals[idx];" << std::endl <<
                _TAB(1) << "return Info::vals[0];" << std::endl <<
                _TAB(0) << "}" << std::endl << std::endl <<

                _TAB(0) << "template<class En>" << std::endl <<
                _TAB(0) << "int Index(En val)" << std::endl <<
                _TAB(0) << "{" << std::endl <<
                _TAB(1) << "typedef detail::Info<En> Info;" << std::endl << std::endl <<
                _TAB(1) << "for (int i = 0; i < Info::size; ++i)" << std::endl <<
                _TAB(2) << "if (Info::vals[i] == val)" << std::endl <<
                _TAB(3) << "return i;" << std::endl << std::endl <<
                _TAB(1) << "return -1;" << std::endl <<
                _TAB(0) << "}" << std::endl << std::endl <<

                _TAB(0) << "template <class En>" << std::endl <<
                _TAB(0) << "int Index(const char* name)" << std::endl <<
                _TAB(0) << "{" << std::endl <<
                _TAB(1) << "typedef detail::Info<En> Info;" << std::endl << std::endl <<
                _TAB(1) << "for (int i = 0; i < Info::size; ++i)" << std::endl <<
                _TAB(2) << "if (std::strcmp(Info::names[i], name) == 0)" << std::endl <<
                _TAB(3) << "return i;" << std::endl << std::endl <<
                _TAB(1) << "return -1;" << std::endl <<
                _TAB(0) << "}" << std::endl;

            _tab = 0;
            *_header << "}" << std::endl;
        }

        _tab = 0;
        if (_cpp)
        {
            *_cpp <<
                _TAB(1) << "}" << std::endl <<
                _TAB(0) << "}" << std::endl;
        }

        Clear();
    }

    void Enum::OnType(const IType* type)
    {
        if (type->TypeCat() != TypeCategory::Enum)
            return;

        const IEnumType* sTy = static_cast<const IEnumType*>(type);
        if (!_hasCreated)
        {
            _hasCreated = true;
            CreateFile();
        }

        Decl(sTy);
        Impl(sTy);
    }

    void Enum::Decl(const IEnumType* type)
    {
        if (_header == nullptr)
            return;

        std::string tyName = utility::Contact(utility::Absolute(type), "::");
        *_header << std::endl <<
            _TAB(0) << "template <>" << std::endl <<
            _TAB(0) << "struct Info<" << tyName << ">" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(1) << "static const int size = " << type->Scope()->VarSet()->Size() << ";" << std::endl <<
            _TAB(1) << "static const char* const names[];" << std::endl <<
            _TAB(1) << "static const " << tyName << " vals[];" << std::endl <<
            _TAB(0) << "};" << std::endl;
    }

    void Enum::Impl(const IEnumType* type)
    {
        if (_cpp == nullptr)
            return;

        const IVarSet* varSet = type->Scope()->VarSet();
        std::string tyName = utility::Contact(utility::Absolute(type), "::");

        *_cpp << std::endl <<
            _TAB(0) << "const char* const Info<" << tyName << ">::names[] = {" << std::endl <<
            _TAB(1);
        for (int i = 0; i < varSet->Size(); ++i)
        {
            const IVariate* var = varSet->Get(i);
            if (i)
            {
                *_cpp << ",";
                if ((i % 5) == 0) *_cpp << std::endl << _TAB(1);
                *_cpp << " ";
            }

            *_cpp << "\"" << var->Name() << "\"";
        }
        *_cpp << ", nullptr"<< std::endl <<
            _TAB(0) << "};" << std::endl;

        *_cpp << std::endl <<
            _TAB(0) << "const " << tyName << " Info<" << tyName << ">::vals[] = {" << std::endl <<
            _TAB(1);
        for (int i = 0; i < varSet->Size(); ++i)
        {
            const IVariate* var = varSet->Get(i);
            if (i)
            {
                *_cpp << ",";
                if ((i % 3) == 0) *_cpp << std::endl << _TAB(1);
                *_cpp << " ";
            }

            *_cpp << tyName << "::" << var->Name();
        }

        if (varSet->Size())
            *_cpp << ", " << tyName << "::" << varSet->Get(0)->Name();
        else
            *_cpp << ", (" << tyName << ")0";

        *_cpp << std::endl <<
            _TAB(0) << "};" << std::endl;
    }

    void Enum::CreateFile()
    {
        _header = new std::ofstream(_file + ".h");
        if (!_header->is_open())
        {
            delete _header;
            _header = nullptr;
        }

        _cpp = new std::ofstream(_file + ".cpp");
        if (!_cpp->is_open())
        {
            delete _cpp;
            _cpp = nullptr;
        }

        if (_header)
        {
            *_header <<
                _TAB(0) << "/*" << std::endl <<
                _TAB(0) << " * this file is auto generated." << std::endl <<
                _TAB(0) << " * please does not edit it manual!" << std::endl <<
                _TAB(0) << "*/" << std::endl <<
                _TAB(0) << "#pragma once" << std::endl <<
                _TAB(0) << "#include <cstring>" << std::endl <<
                _TAB(0) << "#include \"" << _name << ".h\"" << std::endl << std::endl <<
                _TAB(0) << "namespace Enum" << std::endl <<
                _TAB(0) << "{" << std::endl <<
                _TAB(1) << "namespace detail" << std::endl <<
                _TAB(1) << "{" << std::endl <<
                _TAB(2) << "// declare enum info template type" << std::endl <<
                _TAB(2) << "template <class Ty> struct Info { };" << std::endl;
        }

        if (_cpp)
        {
            *_cpp <<
                _TAB(0) << "/*" << std::endl <<
                _TAB(0) << " * this file is auto generated." << std::endl <<
                _TAB(0) << " * please does not edit it manual!" << std::endl <<
                _TAB(0) << "*/" << std::endl <<
                _TAB(0) << "#include \"" << _name << "_Enum.h\"" << std::endl << std::endl <<
                _TAB(0) << "namespace Enum" << std::endl <<
                _TAB(0) << "{" << std::endl <<
                _TAB(1) << "namespace detail" << std::endl <<
                _TAB(1) << "{";
        }

        _tab = 2;
    }

    void Enum::Clear()
    {
        delete _header;
        _header = nullptr;

        delete _cpp;
        _cpp = nullptr;

        _file = "";
        _tab = 0;
    }
}

GCF_NAMESPACE_END
