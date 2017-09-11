#include "CppSerialize.h"
#include "CppUtil.h"
#include "Utility.h"
#include <iostream>
#include <fstream>

#define _TAB(ex)    std::string((_tab + ex) * 4, ' ')
CFG_NAMESPACE_BEGIN

namespace cpp
{
    Serialize::Serialize()
    {
    }

    Serialize::~Serialize()
    {
        Clear();
    }

    bool Serialize::Begin(const IScope* global, std::string& path, std::string& name)
    {
        std::string fileName = utility::ContactPath(path, name) + "_Ser";
        std::ofstream* file = new std::ofstream(fileName + ".h");
        if (file->is_open()) _header = file;
        else _header = &std::cout;
        *_header <<
            _TAB(0) << "/*" << std::endl <<
            _TAB(0) << " * this file is auto generated." << std::endl <<
            _TAB(0) << " * please does not edit it manual!" << std::endl <<
            _TAB(0) << "*/" << std::endl <<
            _TAB(0) << "#pragma once" << std::endl << std::endl <<
            _TAB(0) << "#include \"" << name << ".h\"" << std::endl <<
            _TAB(0) << "#include \"gcf/gcf.h\"" << std::endl << std::endl <<
            _TAB(0) << "namespace serialize" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(1) << "namespace utility" << std::endl <<
            _TAB(1) << "{" << std::endl;

        file = new std::ofstream(fileName + ".cpp");
        if (file->is_open()) _cpp = file;
        else _cpp = &std::cout;
        *_cpp <<
            _TAB(0) << "/*" << std::endl <<
            _TAB(0) << " * this file is auto generated." << std::endl <<
            _TAB(0) << " * please does not edit it manual!" << std::endl <<
            _TAB(0) << "*/" << std::endl <<
            _TAB(0) << "#include \"" << name << "_Ser.h\"" << std::endl << std::endl <<
            _TAB(0) << "namespace serialize" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(1) << "namespace utility" << std::endl <<
            _TAB(1) << "{" << std::endl;

        _tab = 2;
        return true;
    }

    bool Serialize::OnType(const IType* type)
    {
        if (type->TypeCat() != TypeCategory::Enum && type->TypeCat() != TypeCategory::Struct)
            return true;

        // process inner types
        do
        {
            if (type->TypeCat() != TypeCategory::Struct)
                break;

            const IStructType* sTy = static_cast<const IStructType*>(type);
            if (sTy->OwnScope() == nullptr)
                break;

            ITypeSet* tySet = sTy->OwnScope()->TypeSet();
            if (tySet == nullptr)
                break;

            for (int i = 0; i < tySet->Size(); ++i)
                OnType(tySet->Get(i));
        } while (false);

        if (!_isFirst) *_header << std::endl;
        DeclRead(*_header, type, true) << std::endl;
        DeclWrite(*_header, type, true) << std::endl;

        if (!_isFirst) *_cpp << std::endl;

        ImplRead(type);
        *_cpp << std::endl;
        ImplWrite(type);

        _isFirst = false;
        return true;
    }

    void Serialize::End()
    {
        _tab = 0;
        *_header <<
            _TAB(1) << "}" << std::endl <<
            _TAB(0) << "}" << std::endl;

        *_cpp <<
            _TAB(1) << "}" << std::endl <<
            _TAB(0) << "}" << std::endl;

        Clear();
    }

    std::ostream& Serialize::DeclRead(std::ostream& stream, const IType* type, bool isDecl)
    {
        stream << _TAB(0) << "bool Read(IReader* reader, " << cpp_util::TypeName(type) << "& val, const char* name";
        if (isDecl)
            stream << " = nullptr);";
        else
            stream << "/* = nullptr*/)";
        return stream;
    }

    std::ostream& Serialize::DeclWrite(std::ostream& stream, const IType* type, bool isDecl)
    {
        stream << _TAB(0) << "bool Write(IWriter* writer, ";
        if (type->TypeCat() == TypeCategory::Enum)
            stream << cpp_util::TypeName(type) << " val, const char* name";
        else if (type->TypeCat() == TypeCategory::Struct)
            stream << "const " << cpp_util::TypeName(type) << "& val, const char* name";
        else
            assert(false);

        if (isDecl)
            stream << " = nullptr);";
        else
            stream << "/* = nullptr*/)";

        return stream;
    }

    void Serialize::ImplRead(const IType* type)
    {
        DeclRead(*_cpp, type, false) << std::endl << _TAB(0) << "{" << std::endl;
        if (type->TypeCat() == TypeCategory::Enum)
        {
            *_cpp <<
                _TAB(1) << "return reader->Read((int&)val, name);" << std::endl;
        }
        else if (type->TypeCat() == TypeCategory::Struct)
        {
            *_cpp <<
                _TAB(1) << "if (!reader->StructBegin(" << cpp_util::TypeName(type) << "::HASH_CODE, name)) return false;" << std::endl << std::endl;

            IVarSet* varSet = ((const IStructType*)type)->Scope()->VarSet();
            for (int i = 0; i < varSet->Size(); ++i)
            {
                IVariate* var = varSet->Get(i);
                if (var->IsConst())
                    continue;

                *_cpp <<
                    _TAB(1) << "if (!Read(reader, val." << var->Name() << ", \"" << var->Name() << "\")) return false;" << std::endl;
            }

            *_cpp << std::endl <<
                _TAB(1) << "return reader->StructEnd();" << std::endl;
        }
        else
        {
            assert(false);
        }

        *_cpp << _TAB(0) << "}" << std::endl;
    }

    void Serialize::ImplWrite(const IType* type)
    {
        DeclWrite(*_cpp, type, false) << std::endl << _TAB(0) << "{" << std::endl;
        if (type->TypeCat() == TypeCategory::Enum)
        {
            *_cpp <<
                _TAB(1) << "return writer->Write((int)val, name);" << std::endl;
        }
        else if (type->TypeCat() == TypeCategory::Struct)
        {
            *_cpp <<
                _TAB(1) << "if (!writer->StructBegin(" << cpp_util::TypeName(type) << "::HASH_CODE, name)) return false;" << std::endl << std::endl;

            IVarSet* varSet = ((const IStructType*)type)->Scope()->VarSet();
            for (int i = 0; i < varSet->Size(); ++i)
            {
                IVariate* var = varSet->Get(i);
                if (var->IsConst())
                    continue;

                *_cpp <<
                    _TAB(1) << "if (!Write(writer, val." << var->Name() << "), \"" << var->Name() << "\") return false;" << std::endl;
            }

            *_cpp << std::endl <<
                _TAB(1) << "return writer->StructEnd();" << std::endl;
        }
        else
        {
            assert(false);
        }

        *_cpp << _TAB(0) << "}" << std::endl;
    }

    void Serialize::Clear()
    {
        if (_header != &std::cout)
            delete _header;
        if (_cpp != &std::cout)
            delete _cpp;

        _header = nullptr;
        _cpp = nullptr;
        _tab = 0;
    }

}

CFG_NAMESPACE_END
