﻿#include "SerBinary.h"
#include <cassert>

namespace serialize
{
    //////////////////////////////////////////////////////////////////////////
    // NormalReader
    BinaryReader::BinaryReader(IBinaryReadStream* stream)
        : _stream(stream)
    {
    }

    bool BinaryReader::ArrayBegin(int& length, const char* name /* = nullptr */)
    {
        return _stream->Read(&length, 4);
    }

    bool BinaryReader::ArrayEnd()
    {
        return true;
    }

    bool BinaryReader::Read(bool& val, const char* name/* = nullptr*/)
    {
        return _stream->Read(&val, 1);
    }

    bool BinaryReader::Read(int8_t& val, const char* name/* = nullptr*/)
    {
        return _stream->Read(&val, 1);
    }

    bool BinaryReader::Read(int32_t& val, const char* name/* = nullptr*/)
    {
        return _stream->Read(&val, 4);
    }

    bool BinaryReader::Read(int64_t& val, const char* name/* = nullptr*/)
    {
        return _stream->Read(&val, 8);
    }

    bool BinaryReader::Read(float& val, const char* name/* = nullptr*/)
    {
        return _stream->Read(&val, 4);
    }

    bool BinaryReader::Read(double& val, const char* name/* = nullptr*/)
    {
        return _stream->Read(&val, 8);
    }

    bool BinaryReader::Read(std::string& val, const char* name/* = nullptr*/)
    {
        int32_t size = 0;
        if (!Read(size)) return false;

        val.clear();
        while (size)
        {
            char buf[1024] = {0};
            int read = std::min(1023, size);
            if (!_stream->Read(buf, read))
                return false;

            val += buf;
            size -= read;
        }

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    // NormalWriter
    BinaryWriter::BinaryWriter(IBinaryWriteStream* stream)
        : _stream(stream)
    {
    }

    bool BinaryWriter::ArrayBegin(int length, const char* name /* = nullptr */)
    {
        return _stream->Write(&length, 4);
    }

    bool BinaryWriter::ArrayEnd()
    {
        return true;
    }

    bool BinaryWriter::Write(bool val, const char* name/* = nullptr*/)
    {
        return _stream->Write(&val, 1);
    }

    bool BinaryWriter::Write(int8_t val, const char* name/* = nullptr*/)
    {
        return _stream->Write(&val, 1);
    }

    bool BinaryWriter::Write(int32_t val, const char* name/* = nullptr*/)
    {
        return _stream->Write(&val, 4);
    }

    bool BinaryWriter::Write(int64_t val, const char* name/* = nullptr*/)
    {
        return _stream->Write(&val, 8);
    }

    bool BinaryWriter::Write(float val, const char* name/* = nullptr*/)
    {
        return _stream->Write(&val, 4);
    }

    bool BinaryWriter::Write(double val, const char* name/* = nullptr*/)
    {
        return _stream->Write(&val, 8);
    }

    bool BinaryWriter::Write(const std::string& val, const char* name/* = nullptr*/)
    {
        int32_t size = (int32_t)val.size();
        if (!_stream->Write(&size, 4))
            return false;

        return _stream->Write(val.c_str(), size);
    }
}
