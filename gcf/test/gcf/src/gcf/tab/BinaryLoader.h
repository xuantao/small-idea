#pragma once
#include "TabDef.h"
#include "../serialize/SerUtility.h"
#include "../serialize/SerBinary.h"
#include "../serialize/SerMemoryStream.h"

namespace tab
{
    template <class Ty>
    class BinaryLoader : public ILoader<Ty>
    {
    public:
        bool Setup(const void* data, size_t size)
        {
            if (!_stream.Init(data, size))
                return false;

            serialize::BinaryReader reader(&_stream);
            if (!reader.Read(_count, nullptr))
                return false;

            if (_count == 0)
                return false;

            return true;
        }

    public:
        virtual bool Load(Ty& val)
        {
            if (_index == _count)
                return false;

            serialize::BinaryReader reader(&_stream);
            if (!serialize::utility::Read(reader, val, nullptr))
                return false;

            ++_index;
            return true;
        }

    protected:
        int _index = 0;
        int _count = 0;
        serialize::MemoryReadOnly _stream;
    };
}
