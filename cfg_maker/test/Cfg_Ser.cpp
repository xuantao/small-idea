/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#include "Cfg_Ser.h"

namespace serialize
{
    namespace utility
    {
        bool Read(IReader* reader, GlobalType& val)
        {
            return reader->Read((int&)val);
        }

        bool Write(IWriter* writer, GlobalType val)
        {
            return writer->Write((int)val);
        }

        bool Read(IReader* reader, Enum1& val)
        {
            return reader->Read((int&)val);
        }

        bool Write(IWriter* writer, Enum1 val)
        {
            return writer->Write((int)val);
        }

        bool Read(IReader* reader, Detail::Enum2& val)
        {
            return reader->Read((int&)val);
        }

        bool Write(IWriter* writer, Detail::Enum2 val)
        {
            return writer->Write((int)val);
        }

        bool Read(IReader* reader, Struct0& val)
        {
            if (!reader->StructBegin(Struct0::HASH_CODE)) return false;
            if (!Read(reader, val.s0a)) return false;
            if (!Read(reader, val.s0b)) return false;
            if (!Read(reader, val.s0c)) return false;
            if (!reader->StructEnd()) return false;

            return true;
        }

        bool Write(IWriter* writer, const Struct0& val)
        {
            if (!writer->StructBegin(Struct0::HASH_CODE)) return false;
            if (!Write(writer, val.s0a)) return false;
            if (!Write(writer, val.s0b)) return false;
            if (!Write(writer, val.s0c)) return false;
            if (!writer->StructEnd()) return false;

            return true;
        }

        bool Read(IReader* reader, Struct1& val)
        {
            if (!reader->StructBegin(Struct1::HASH_CODE)) return false;
            if (!Read(reader, val.s0a)) return false;
            if (!Read(reader, val.s0b)) return false;
            if (!Read(reader, val.s0c)) return false;
            if (!Read(reader, val.s1a)) return false;
            if (!Read(reader, val.f1)) return false;
            if (!Read(reader, val.str2)) return false;
            if (!Read(reader, val.str3)) return false;
            if (!Read(reader, val.s1)) return false;
            if (!Read(reader, val.s2)) return false;
            if (!Read(reader, val.s3)) return false;
            if (!Read(reader, val.s1arEnum)) return false;
            if (!Read(reader, val.s1arEnum2)) return false;
            if (!reader->StructEnd()) return false;

            return true;
        }

        bool Write(IWriter* writer, const Struct1& val)
        {
            if (!writer->StructBegin(Struct1::HASH_CODE)) return false;
            if (!Write(writer, val.s0a)) return false;
            if (!Write(writer, val.s0b)) return false;
            if (!Write(writer, val.s0c)) return false;
            if (!Write(writer, val.s1a)) return false;
            if (!Write(writer, val.f1)) return false;
            if (!Write(writer, val.str2)) return false;
            if (!Write(writer, val.str3)) return false;
            if (!Write(writer, val.s1)) return false;
            if (!Write(writer, val.s2)) return false;
            if (!Write(writer, val.s3)) return false;
            if (!Write(writer, val.s1arEnum)) return false;
            if (!Write(writer, val.s1arEnum2)) return false;
            if (!writer->StructEnd()) return false;

            return true;
        }

        bool Read(IReader* reader, Struct2& val)
        {
            if (!reader->StructBegin(Struct2::HASH_CODE)) return false;
            if (!Read(reader, val.a)) return false;
            if (!Read(reader, val.b)) return false;
            if (!Read(reader, val.c)) return false;
            if (!Read(reader, val.d)) return false;
            if (!Read(reader, val.s)) return false;
            if (!Read(reader, val.s1)) return false;
            if (!Read(reader, val.s2)) return false;
            if (!Read(reader, val.s3)) return false;
            if (!reader->StructEnd()) return false;

            return true;
        }

        bool Write(IWriter* writer, const Struct2& val)
        {
            if (!writer->StructBegin(Struct2::HASH_CODE)) return false;
            if (!Write(writer, val.a)) return false;
            if (!Write(writer, val.b)) return false;
            if (!Write(writer, val.c)) return false;
            if (!Write(writer, val.d)) return false;
            if (!Write(writer, val.s)) return false;
            if (!Write(writer, val.s1)) return false;
            if (!Write(writer, val.s2)) return false;
            if (!Write(writer, val.s3)) return false;
            if (!writer->StructEnd()) return false;

            return true;
        }

        bool Read(IReader* reader, Wtf& val)
        {
            return reader->Read((int&)val);
        }

        bool Write(IWriter* writer, Wtf val)
        {
            return writer->Write((int)val);
        }

        bool Read(IReader* reader, Sr3& val)
        {
            if (!reader->StructBegin(Sr3::HASH_CODE)) return false;
            if (!Read(reader, val.ID)) return false;
            if (!Read(reader, val.StageID)) return false;
            if (!Read(reader, val.Name)) return false;
            if (!reader->StructEnd()) return false;

            return true;
        }

        bool Write(IWriter* writer, const Sr3& val)
        {
            if (!writer->StructBegin(Sr3::HASH_CODE)) return false;
            if (!Write(writer, val.ID)) return false;
            if (!Write(writer, val.StageID)) return false;
            if (!Write(writer, val.Name)) return false;
            if (!writer->StructEnd()) return false;

            return true;
        }
    }
}
