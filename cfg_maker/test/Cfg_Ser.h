/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#pragma once

#include "Cfg.h"
#include "Serialize.h"

namespace serialize
{
    namespace utility
    {
        bool Read(IReader* reader, GlobalType& val);
        bool Write(IWriter* writer, GlobalType val);

        bool Read(IReader* reader, Enum1& val);
        bool Write(IWriter* writer, Enum1 val);

        bool Read(IReader* reader, Detail::Enum2& val);
        bool Write(IWriter* writer, Detail::Enum2 val);

        bool Read(IReader* reader, Struct0& val);
        bool Write(IWriter* writer, const Struct0& val);

        bool Read(IReader* reader, Struct1& val);
        bool Write(IWriter* writer, const Struct1& val);

        bool Read(IReader* reader, Struct2& val);
        bool Write(IWriter* writer, const Struct2& val);

        bool Read(IReader* reader, Wtf& val);
        bool Write(IWriter* writer, Wtf val);

        bool Read(IReader* reader, Sr3& val);
        bool Write(IWriter* writer, const Sr3& val);
    }
}
