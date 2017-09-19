/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#pragma once

#include "Cfg.h"
#include "gcf/gcf.h"

namespace serialize
{
    namespace utility
    {
        bool Read(IReader* reader, GlobalType& val, const char* name = nullptr);
        bool Write(IWriter* writer, GlobalType val, const char* name = nullptr);

        bool Read(IReader* reader, TabTest_1& val, const char* name = nullptr);
        bool Write(IWriter* writer, const TabTest_1& val, const char* name = nullptr);

        bool Read(IReader* reader, Enum1& val, const char* name = nullptr);
        bool Write(IWriter* writer, Enum1 val, const char* name = nullptr);

        bool Read(IReader* reader, Detail::Enum2& val, const char* name = nullptr);
        bool Write(IWriter* writer, Detail::Enum2 val, const char* name = nullptr);

        bool Read(IReader* reader, Struct0::Innser& val, const char* name = nullptr);
        bool Write(IWriter* writer, const Struct0::Innser& val, const char* name = nullptr);

        bool Read(IReader* reader, Struct0& val, const char* name = nullptr);
        bool Write(IWriter* writer, const Struct0& val, const char* name = nullptr);

        bool Read(IReader* reader, Struct1& val, const char* name = nullptr);
        bool Write(IWriter* writer, const Struct1& val, const char* name = nullptr);

        bool Read(IReader* reader, Struct2& val, const char* name = nullptr);
        bool Write(IWriter* writer, const Struct2& val, const char* name = nullptr);

        bool Read(IReader* reader, Inner& val, const char* name = nullptr);
        bool Write(IWriter* writer, const Inner& val, const char* name = nullptr);

        bool Read(IReader* reader, Msg::Inner& val, const char* name = nullptr);
        bool Write(IWriter* writer, const Msg::Inner& val, const char* name = nullptr);

        bool Read(IReader* reader, Msg& val, const char* name = nullptr);
        bool Write(IWriter* writer, const Msg& val, const char* name = nullptr);

        bool Read(IReader* reader, Wtf& val, const char* name = nullptr);
        bool Write(IWriter* writer, Wtf val, const char* name = nullptr);

        bool Read(IReader* reader, Sr3& val, const char* name = nullptr);
        bool Write(IWriter* writer, const Sr3& val, const char* name = nullptr);
    }
}
