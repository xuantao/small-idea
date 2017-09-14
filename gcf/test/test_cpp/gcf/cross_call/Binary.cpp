#include "Binary.h"
#include <cassert>

namespace cross_call
{
    //////////////////////////////////////////////////////////////////////////
    // NormalReader
    BinaryReader::BinaryReader(IBinaryStream* stream)
        : _stream(stream)
    {
    }

    bool BinaryReader::StructBegin(uint32_t code, const char* name/* = nullptr*/)
    {
        uint32_t check = 0;
        return _stream->Read(&check, 4) && check == code;
    }

    bool BinaryReader::StructEnd()
    {
        return true;
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

    bool BinaryReader::Read(int32_t& val, const char* name/* = nullptr*/)
    {
        return _stream->Read(&val, 4);
    }

    bool BinaryReader::Read(uint32_t& val, const char* name/* = nullptr*/)
    {
        return _stream->Read(&val, 4);
    }

    bool BinaryReader::Read(float& val, const char* name/* = nullptr*/)
    {
        return _stream->Read(&val, 4);
    }

    bool BinaryReader::Read(std::string& val, const char* name/* = nullptr*/)
    {
        uint32_t size = 0;
        char buf[1024] = {0};

        if (!Read(size)) return false;

        assert(size < 1024);
        if (!_stream->Read(buf, size)) return false;

        val = buf;
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    // NormalWriter
    BinaryWriter::BinaryWriter(IBinaryStream* stream)
        : _stream(stream)
    {
    }

    bool BinaryWriter::StructBegin(uint32_t code, const char* name/* = nullptr*/)
    {
        return _stream->Write(&code, 4);
    }

    bool BinaryWriter::StructEnd()
    {
        return true;
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

    bool BinaryWriter::Write(int32_t val, const char* name/* = nullptr*/)
    {
        return _stream->Write(&val, 4);
    }

    bool BinaryWriter::Write(uint32_t val, const char* name/* = nullptr*/)
    {
        return _stream->Write(&val, 4);
    }

    bool BinaryWriter::Write(float val, const char* name/* = nullptr*/)
    {
        return _stream->Write(&val, 4);
    }

    bool BinaryWriter::Write(const std::string& val, const char* name/* = nullptr*/)
    {
        uint32_t size = (uint32_t)val.size();
        assert(size < 1024);

        if (!_stream->Write(&size, 4)) return false;

        return _stream->Write(val.c_str(), size);
    }
}
