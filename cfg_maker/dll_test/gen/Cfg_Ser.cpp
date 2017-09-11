/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#include "Cfg_Ser.h"

namespace serialize
{
    namespace utility
    {
        bool Read(IReader* reader, GlobalType& val, const char* name/* = nullptr*/)
        {
            return reader->Read((int&)val, name);
        }

        bool Write(IWriter* writer, GlobalType val, const char* name/* = nullptr*/)
        {
            return writer->Write((int)val, name);
        }

        bool Read(IReader* reader, Enum1& val, const char* name/* = nullptr*/)
        {
            return reader->Read((int&)val, name);
        }

        bool Write(IWriter* writer, Enum1 val, const char* name/* = nullptr*/)
        {
            return writer->Write((int)val, name);
        }

        bool Read(IReader* reader, Detail::Enum2& val, const char* name/* = nullptr*/)
        {
            return reader->Read((int&)val, name);
        }

        bool Write(IWriter* writer, Detail::Enum2 val, const char* name/* = nullptr*/)
        {
            return writer->Write((int)val, name);
        }

        bool Read(IReader* reader, Struct0::Innser& val, const char* name/* = nullptr*/)
        {
            if (!reader->StructBegin(Struct0::Innser::HASH_CODE, name)) return false;

            if (!Read(reader, val.a, "a")) return false;
            if (!Read(reader, val.b, "b")) return false;

            return reader->StructEnd();
        }

        bool Write(IWriter* writer, const Struct0::Innser& val, const char* name/* = nullptr*/)
        {
            if (!writer->StructBegin(Struct0::Innser::HASH_CODE, name)) return false;

            if (!Write(writer, val.a), "a") return false;
            if (!Write(writer, val.b), "b") return false;

            return writer->StructEnd();
        }

        bool Read(IReader* reader, Struct0& val, const char* name/* = nullptr*/)
        {
            if (!reader->StructBegin(Struct0::HASH_CODE, name)) return false;

            if (!Read(reader, val.s0a, "s0a")) return false;
            if (!Read(reader, val.s0b, "s0b")) return false;
            if (!Read(reader, val.s0c, "s0c")) return false;

            return reader->StructEnd();
        }

        bool Write(IWriter* writer, const Struct0& val, const char* name/* = nullptr*/)
        {
            if (!writer->StructBegin(Struct0::HASH_CODE, name)) return false;

            if (!Write(writer, val.s0a), "s0a") return false;
            if (!Write(writer, val.s0b), "s0b") return false;
            if (!Write(writer, val.s0c), "s0c") return false;

            return writer->StructEnd();
        }

        bool Read(IReader* reader, Struct1& val, const char* name/* = nullptr*/)
        {
            if (!reader->StructBegin(Struct1::HASH_CODE, name)) return false;

            if (!Read(reader, val.s0a, "s0a")) return false;
            if (!Read(reader, val.s0b, "s0b")) return false;
            if (!Read(reader, val.s0c, "s0c")) return false;
            if (!Read(reader, val.s1a, "s1a")) return false;
            if (!Read(reader, val.f1, "f1")) return false;
            if (!Read(reader, val.str2, "str2")) return false;
            if (!Read(reader, val.str3, "str3")) return false;
            if (!Read(reader, val.s1, "s1")) return false;
            if (!Read(reader, val.s2, "s2")) return false;
            if (!Read(reader, val.s3, "s3")) return false;
            if (!Read(reader, val.s1arEnum, "s1arEnum")) return false;
            if (!Read(reader, val.s1arEnum2, "s1arEnum2")) return false;

            return reader->StructEnd();
        }

        bool Write(IWriter* writer, const Struct1& val, const char* name/* = nullptr*/)
        {
            if (!writer->StructBegin(Struct1::HASH_CODE, name)) return false;

            if (!Write(writer, val.s0a), "s0a") return false;
            if (!Write(writer, val.s0b), "s0b") return false;
            if (!Write(writer, val.s0c), "s0c") return false;
            if (!Write(writer, val.s1a), "s1a") return false;
            if (!Write(writer, val.f1), "f1") return false;
            if (!Write(writer, val.str2), "str2") return false;
            if (!Write(writer, val.str3), "str3") return false;
            if (!Write(writer, val.s1), "s1") return false;
            if (!Write(writer, val.s2), "s2") return false;
            if (!Write(writer, val.s3), "s3") return false;
            if (!Write(writer, val.s1arEnum), "s1arEnum") return false;
            if (!Write(writer, val.s1arEnum2), "s1arEnum2") return false;

            return writer->StructEnd();
        }

        bool Read(IReader* reader, Struct2& val, const char* name/* = nullptr*/)
        {
            if (!reader->StructBegin(Struct2::HASH_CODE, name)) return false;

            if (!Read(reader, val.a, "a")) return false;
            if (!Read(reader, val.b, "b")) return false;
            if (!Read(reader, val.c, "c")) return false;
            if (!Read(reader, val.d, "d")) return false;
            if (!Read(reader, val.s, "s")) return false;
            if (!Read(reader, val.s1, "s1")) return false;
            if (!Read(reader, val.s2, "s2")) return false;
            if (!Read(reader, val.s3, "s3")) return false;

            return reader->StructEnd();
        }

        bool Write(IWriter* writer, const Struct2& val, const char* name/* = nullptr*/)
        {
            if (!writer->StructBegin(Struct2::HASH_CODE, name)) return false;

            if (!Write(writer, val.a), "a") return false;
            if (!Write(writer, val.b), "b") return false;
            if (!Write(writer, val.c), "c") return false;
            if (!Write(writer, val.d), "d") return false;
            if (!Write(writer, val.s), "s") return false;
            if (!Write(writer, val.s1), "s1") return false;
            if (!Write(writer, val.s2), "s2") return false;
            if (!Write(writer, val.s3), "s3") return false;

            return writer->StructEnd();
        }

        bool Read(IReader* reader, Inner& val, const char* name/* = nullptr*/)
        {
            if (!reader->StructBegin(Inner::HASH_CODE, name)) return false;

            if (!Read(reader, val.ii, "ii")) return false;
            if (!Read(reader, val.name, "name")) return false;

            return reader->StructEnd();
        }

        bool Write(IWriter* writer, const Inner& val, const char* name/* = nullptr*/)
        {
            if (!writer->StructBegin(Inner::HASH_CODE, name)) return false;

            if (!Write(writer, val.ii), "ii") return false;
            if (!Write(writer, val.name), "name") return false;

            return writer->StructEnd();
        }

        bool Read(IReader* reader, Msg& val, const char* name/* = nullptr*/)
        {
            if (!reader->StructBegin(Msg::HASH_CODE, name)) return false;

            if (!Read(reader, val.b, "b")) return false;
            if (!Read(reader, val.i, "i")) return false;
            if (!Read(reader, val.f, "f")) return false;
            if (!Read(reader, val.s, "s")) return false;
            if (!Read(reader, val.inner, "inner")) return false;

            return reader->StructEnd();
        }

        bool Write(IWriter* writer, const Msg& val, const char* name/* = nullptr*/)
        {
            if (!writer->StructBegin(Msg::HASH_CODE, name)) return false;

            if (!Write(writer, val.b), "b") return false;
            if (!Write(writer, val.i), "i") return false;
            if (!Write(writer, val.f), "f") return false;
            if (!Write(writer, val.s), "s") return false;
            if (!Write(writer, val.inner), "inner") return false;

            return writer->StructEnd();
        }

        bool Read(IReader* reader, Wtf& val, const char* name/* = nullptr*/)
        {
            return reader->Read((int&)val, name);
        }

        bool Write(IWriter* writer, Wtf val, const char* name/* = nullptr*/)
        {
            return writer->Write((int)val, name);
        }

        bool Read(IReader* reader, Sr3& val, const char* name/* = nullptr*/)
        {
            if (!reader->StructBegin(Sr3::HASH_CODE, name)) return false;

            if (!Read(reader, val.ID, "ID")) return false;
            if (!Read(reader, val.StageID, "StageID")) return false;
            if (!Read(reader, val.Name, "Name")) return false;

            return reader->StructEnd();
        }

        bool Write(IWriter* writer, const Sr3& val, const char* name/* = nullptr*/)
        {
            if (!writer->StructBegin(Sr3::HASH_CODE, name)) return false;

            if (!Write(writer, val.ID), "ID") return false;
            if (!Write(writer, val.StageID), "StageID") return false;
            if (!Write(writer, val.Name), "Name") return false;

            return writer->StructEnd();
        }
    }
}
