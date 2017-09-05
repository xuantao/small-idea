#include "CsSerialize.h"
#include "CsUtility.h"
#include "Utility.h"
#include <iostream>
#include <fstream>

#define _TAB(ex)    std::string((_tab + ex) * 4, ' ')
CFG_NAMESPACE_BEGIN

namespace cs
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
        std::ofstream* file = new std::ofstream(fileName + ".cs");
        if (file->is_open()) _stream = file;
        else _stream = &std::cout;

        *_stream <<
            _TAB(0) << "/*" << std::endl <<
            _TAB(0) << " * this file is auto generated." << std::endl <<
            _TAB(0) << " * please does not edit it manual!" << std::endl <<
            _TAB(0) << "*/" << std::endl <<
            _TAB(0) << "using System;" << std::endl <<
            _TAB(0) << "using System.Collections;" << std::endl <<
            _TAB(0) << "using System.Collections.Generic;" << std::endl << std::endl <<
            _TAB(0) << "namespace Serialize" << std::endl <<
            _TAB(0) << "{" << std::endl <<
            _TAB(1) << "public static partial class Utility" << std::endl <<
            _TAB(1) << "{" << std::endl;

        _tab = 2;
        return true;
    }

    bool Serialize::OnType(const IType* type)
    {
        if (type->TypeCat() != TypeCategory::Enum && type->TypeCat() != TypeCategory::Struct)
            return true;

        if (!_isFirst) *_stream << std::endl;

        DeclRead(type);
        *_stream << std::endl;
        DeclWrite(type);

        _isFirst = false;
        return true;
    }

    void Serialize::End()
    {
        _tab = 0;

        *_stream <<
            _TAB(1) << "}" << std::endl <<
            _TAB(0) << "}" << std::endl << std::endl;

        Clear();
    }

    void Serialize::DeclRead(const IType* type)
    {
        std::string tyName = cs_util::TypeName(type);

        *_stream <<
            _TAB(0) << "public static bool Read(IReader reader, ref " << tyName << " val)" << std::endl <<
            _TAB(0) << "{" << std::endl;

        if (type->TypeCat() == TypeCategory::Enum)
        {
            *_stream <<
                _TAB(1) << "int tmp = 0;" << std::endl <<
                _TAB(1) << "if (!reader.Read(ref tmp))" << std::endl <<
                _TAB(2) << "return false;" << std::endl << std::endl <<
                _TAB(1) << "val = (" << tyName << ")tmp;" << std::endl <<
                _TAB(1) << "return true;" << std::endl;
        }
        else if (type->TypeCat() == TypeCategory::Struct)
        {
            *_stream <<
                _TAB(1) << "if (!reader.StructBegin(" << tyName << ".HASH_CODE))" << std::endl <<
                _TAB(2) << "return false;" << std::endl << std::endl <<
                _TAB(1) << "if (val == null)" << std::endl <<
                _TAB(2) << "val = new " << tyName << "();" << std::endl << std::endl;

            IVarSet* varSet = ((const IStructType*)type)->Scope()->VarSet();
            for (int i = 0; i < varSet->Size(); ++i)
            {
                if (varSet->Get(i)->IsConst())
                    continue;

                *_stream <<
                    _TAB(1) << "if (!Read(reader, ref val." << varSet->Get(i)->Name() << ")) return false;" << std::endl;
            }

            *_stream << std::endl <<
                _TAB(1) << "if (!reader.StructEnd())" << std::endl <<
                _TAB(2) << "return false;" << std::endl << std::endl <<
                _TAB(1) << "return true;" << std::endl;
        }
        else
        {
            assert(false);
        }

        *_stream << _TAB(0) << "}" << std::endl;
    }

    void Serialize::DeclWrite(const IType* type)
    {
        std::string tyName = cs_util::TypeName(type);

        *_stream <<
            _TAB(0) << "public static bool Write(IWriter writer, " << tyName << " val)" << std::endl <<
            _TAB(0) << "{" << std::endl;

        if (type->TypeCat() == TypeCategory::Enum)
        {
            *_stream <<
                _TAB(1) << "return writer.Write((int)val);" << std::endl;
        }
        else if (type->TypeCat() == TypeCategory::Struct)
        {
            *_stream <<
                _TAB(1) << "if (!writer.StructBegin(" << tyName << ".HASH_CODE))" << std::endl <<
                _TAB(2) << "return false;" << std::endl << std::endl;

            IVarSet* varSet = ((const IStructType*)type)->Scope()->VarSet();
            for (int i = 0; i < varSet->Size(); ++i)
            {
                if (varSet->Get(i)->IsConst())
                    continue;

                *_stream <<
                    _TAB(1) << "if (!Write(writer, val." << varSet->Get(i)->Name() << ")) return false;" << std::endl;
            }

            *_stream << std::endl <<
                _TAB(1) << "if (!writer.StructEnd())" << std::endl <<
                _TAB(2) << "return false;" << std::endl << std::endl <<
                _TAB(1) << "return true;" << std::endl;
        }
        else
        {
            assert(false);
        }

        *_stream << _TAB(0) << "}" << std::endl;
    }

    void Serialize::Clear()
    {
        if (_stream != &std::cout)
            delete _stream;

        _stream = nullptr;
        _tab = 0;
    }

}

CFG_NAMESPACE_END
