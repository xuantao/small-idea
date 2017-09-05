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
    { }

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
        *_header << "/*\n * this file is auto generated.\n * please does not edit it manual!\n*/" << std::endl <<
            "#pragma once" << std::endl << std::endl <<
            "#include \"" << name << ".h\"" << std::endl <<
            "#include \"Serialize.h\"" << std::endl << std::endl <<
            _TAB(0) << "namespace serialize" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(1) << "namespace utility" << std::endl <<
            _TAB(1) << "{" << std::endl;

        file = new std::ofstream(fileName + ".cpp");
        if (file->is_open()) _cpp = file;
        else _cpp = &std::cout;
        *_cpp << "/*\n * this file is auto generated.\n * please does not edit it manual!\n*/" << std::endl <<
            "#include \"" << name << "_Ser.h\"" << std::endl << std::endl <<
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

        if (!_isFirst) *_header << std::endl;
        DeclRead(*_header, type) << ";" << std::endl;
        DeclWrite(*_header, type) << ";" << std::endl;

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

    std::ostream& Serialize::DeclRead(std::ostream& stream, const IType* type)
    {
        stream << _TAB(0) << "bool Read(IReader* reader, " << cpp_util::TypeName(type) << "& val)";
        return stream;
    }

    std::ostream& Serialize::DeclWrite(std::ostream& stream, const IType* type)
    {
        stream << _TAB(0) << "bool Write(IWriter* writer, ";
        if (type->TypeCat() == TypeCategory::Enum)
            stream << cpp_util::TypeName(type) << " val)";
        else if (type->TypeCat() == TypeCategory::Struct)
            stream << "const " << cpp_util::TypeName(type) << "& val)";
        else
            assert(false);

        return stream;
    }

    void Serialize::ImplRead(const IType* type)
    {
        DeclRead(*_cpp, type) << std::endl << _TAB(0) << "{" << std::endl;
        if (type->TypeCat() == TypeCategory::Enum)
        {
            *_cpp <<
                _TAB(1) << "return reader->Read((int&)val);" << std::endl;
        }
        else if (type->TypeCat() == TypeCategory::Struct)
        {
            *_cpp <<
                _TAB(1) << "if (!reader->StructBegin(" << cpp_util::TypeName(type) << "::HASH_CODE)) return false;" << std::endl;

            IVarSet* varSet = ((const IStructType*)type)->Scope()->VarSet();
            for (int i = 0; i < varSet->Size(); ++i)
            {
                if (varSet->Get(i)->IsConst())
                    continue;

                *_cpp <<
                    _TAB(1) << "if (!Read(reader, val." << varSet->Get(i)->Name() << ")) return false;" << std::endl;
            }

            *_cpp <<
                _TAB(1) << "if (!reader->StructEnd()) return false;" << std::endl << std::endl <<
                _TAB(1) << "return true;" << std::endl;
        }
        else
        {
            assert(false);
        }

        *_cpp << _TAB(0) << "}" << std::endl;
    }

    void Serialize::ImplWrite(const IType* type)
    {
        DeclWrite(*_cpp, type) << std::endl << _TAB(0) << "{" << std::endl;
        if (type->TypeCat() == TypeCategory::Enum)
        {
            *_cpp <<
                _TAB(1) << "return writer->Write((int)val);" << std::endl;
        }
        else if (type->TypeCat() == TypeCategory::Struct)
        {
            *_cpp <<
                _TAB(1) << "if (!writer->StructBegin(" << cpp_util::TypeName(type) << "::HASH_CODE)) return false;" << std::endl;

            IVarSet* varSet = ((const IStructType*)type)->Scope()->VarSet();
            for (int i = 0; i < varSet->Size(); ++i)
            {
                if (varSet->Get(i)->IsConst())
                    continue;

                *_cpp <<
                    _TAB(1) << "if (!Write(writer, val." << varSet->Get(i)->Name() << ")) return false;" << std::endl;
            }

            *_cpp <<
                _TAB(1) << "if (!writer->StructEnd()) return false;" << std::endl << std::endl <<
                _TAB(1) << "return true;" << std::endl;
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
