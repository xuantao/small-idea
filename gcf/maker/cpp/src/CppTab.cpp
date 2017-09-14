#include "CppTab.h"
#include "CppUtil.h"
#include "utility/TabVisitor.h"

#define _TAB(ex)    std::string((_tab + ex) * 4, ' ')
GCF_NAMESPACE_BEGIN

namespace cpp
{
    void sWriteData(std::ostream& stream, const std::vector<std::string>& vec, int tab = 1)
    {
        int _tab = tab;
        stream << _TAB(0);

        for (size_t i = 0; i < vec.size(); ++i)
        {
            if (i) stream << ",";
            if (i && (i % 5) == 0) stream << std::endl << _TAB(0);
            if (i) stream << " ";

            stream << "\"" << vec[i] << "\"";
        }
    }

    Tab::Tab()
    {
    }

    Tab::~Tab()
    {
        Clear();
    }

    Tab* Tab::Create()
    {
        return new Tab();
    }

    void Tab::Release()
    {
        delete this;
    }

    bool Tab::OnBegin(const IScope* global, const char* path, const char* name)
    {
        _file = utility::ContactPath(path, name) + "_Tab";
        _name = name;
        return true;
    }

    void Tab::OnEnd()
    {
        --_tab;
        if (_header)
            *_header << "}" << std::endl;

        if (_cpp)
            *_cpp << "}" << std::endl;

        Clear();
    }

    void Tab::OnType(const IType* type)
    {
        if (type->TypeCat() != TypeCategory::Struct)
            return;

        const IStructType* sTy = static_cast<const IStructType*>(type);
        if (sTy->CfgCat() != CfgCategory::Tab)
            return;

        if (!_hasCreated)
        {
            _hasCreated = true;
            CreateFile();
        }

        Decl(sTy);
        Impl(sTy);
    }

    void Tab::Decl(const IStructType* type)
    {
        if (_header == nullptr)
            return;

        std::string tyName = utility::Contact(utility::Absolute(type), "::");
        *_header << std::endl <<
            _TAB(0) << "template <>" << std::endl <<
            _TAB(0) << "struct Info<" << tyName << ">" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(1) << "static const char* const title[];" << std::endl <<
            _TAB(1) << "static const char* const types[];" << std::endl <<
            _TAB(1) << "static const char* const descs[];" << std::endl <<
            _TAB(0) << "};" << std::endl;
    }

    void Tab::Impl(const IStructType* type)
    {
        if (_cpp == nullptr)
            return;

        utility::TabVisitor visitor;
        std::string tyName = utility::Contact(utility::Absolute(type), "::");
        utility::Traverse(type, &visitor);

        *_cpp << std::endl <<
            _TAB(0) << "const char* const Info<" << tyName << ">::title[] = {" << std::endl;
        sWriteData(*_cpp, visitor.Title(), _tab + 1);
        *_cpp << ", nullptr" << std::endl <<
            _TAB(0) << "};" << std::endl;

        *_cpp << std::endl <<
            _TAB(0) << "const char* const Info<" << tyName << ">::types[] = {" << std::endl;
        sWriteData(*_cpp, visitor.Type(), _tab + 1);
        *_cpp << ", nullptr" << std::endl <<
            _TAB(0) << "};" << std::endl;

        *_cpp << std::endl <<
            _TAB(0) << "const char* const Info<" << tyName << ">::descs[] = {" << std::endl;
        sWriteData(*_cpp, visitor.Describe(), _tab + 1);
        *_cpp << ", nullptr" << std::endl <<
            _TAB(0) << "};" << std::endl;
    }

    void Tab::CreateFile()
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
                _TAB(0) << "#pragma once" << std::endl << std::endl <<
                _TAB(0) << "#include \"" << _name << ".h\"" << std::endl << std::endl <<
                _TAB(0) << "namespace tab" << std::endl <<
                _TAB(0) << "{" << std::endl <<
                _TAB(1) << "// declare Tab Info template type" << std::endl <<
                _TAB(1) << "template <class Ty> struct Info { };" << std::endl;
        }

        if (_cpp)
        {
            *_cpp <<
                _TAB(0) << "/*" << std::endl <<
                _TAB(0) << " * this file is auto generated." << std::endl <<
                _TAB(0) << " * please does not edit it manual!" << std::endl <<
                _TAB(0) << "*/" << std::endl <<
                _TAB(0) << "#include \"" << _name << "_Tab.h\"" << std::endl << std::endl <<
                _TAB(0) << "namespace tab" << std::endl <<
                _TAB(0) << "{";// << std::endl;
        }

        ++_tab;
    }

    void Tab::Clear()
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
