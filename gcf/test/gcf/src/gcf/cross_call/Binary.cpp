#include "Binary.h"
#include <cassert>

namespace cross_call
{
    //////////////////////////////////////////////////////////////////////////
    // NormalReader
    Reader::Reader(serialize::IBinaryReadStream* stream)
        : serialize::BinaryReader(stream)
    {
    }

    bool Reader::StructBegin(int32_t code, const char* name/* = nullptr*/)
    {
        int32_t check = 0;
        return _stream->Read(&check, 4) && check == code;
    }

    bool Reader::StructEnd()
    {
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    // NormalWriter
    Writer::Writer(serialize::IBinaryWriteStream* stream)
        : serialize::BinaryWriter(stream)
    {
    }

    bool Writer::StructBegin(int32_t code, const char* name/* = nullptr*/)
    {
        return _stream->Write(&code, 4);
    }

    bool Writer::StructEnd()
    {
        return true;
    }
}
