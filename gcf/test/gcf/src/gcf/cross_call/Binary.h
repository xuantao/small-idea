#pragma once

#include "ICrossCall.h"
#include "../serialize/SerBinary.h"

namespace cross_call
{
    class Reader : public serialize::BinaryReader
    {
    public:
        Reader(serialize::IBinaryReadStream* stream);
        virtual ~Reader() {}

    public:
        virtual bool StructBegin(int32_t code, const char* name = nullptr);
        virtual bool StructEnd();
    };

    class Writer : public serialize::BinaryWriter
    {
    public:
        Writer(serialize::IBinaryWriteStream* buffer);
        virtual ~Writer() {}

    public:
        virtual bool StructBegin(int32_t code, const char* name = nullptr);
        virtual bool StructEnd();
    };
}
