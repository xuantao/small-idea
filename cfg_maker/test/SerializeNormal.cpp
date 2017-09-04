#include "SerializeNormal.h"
#include <cassert>

namespace serialize
{
    //////////////////////////////////////////////////////////////////////////
    // NormalReader
    NormalReader::NormalReader(IBuffer* buffer)
        : _buffer(buffer)
    { }

    bool NormalReader::StructBegin(uint32_t code)
    {
        uint32_t check = 0;
        return _buffer->Read(&check, 4) && check == code;
    }

    bool NormalReader::StructEnd()
    {
        return true;
    }

    bool NormalReader::Read(bool& val)
    {
        return _buffer->Read(&val, 1);
    }

    bool NormalReader::Read(int32_t& val)
    {
        return _buffer->Read(&val, 4);
    }

    bool NormalReader::Read(uint32_t& val)
    {
        return _buffer->Read(&val, 4);
    }

    bool NormalReader::Read(float& val)
    {
        return _buffer->Read(&val, 4);
    }

    bool NormalReader::Read(std::string& val)
    {
        uint32_t size = 0;
        char buf[1024] = { 0 };

        if (!Read(size)) return false;

        assert(size < 1024);
        if (!_buffer->Read(buf, size)) return false;

        val = buf;
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    // NormalWriter
    NormalWriter::NormalWriter(IBuffer* buffer)
        : _buffer(buffer)
    { }

    bool NormalWriter::StructBegin(uint32_t code)
    {
        return _buffer->Write(&code, 4);
    }

    bool NormalWriter::StructEnd()
    {
        return true;
    }

    bool NormalWriter::Write(bool val)
    {
        return _buffer->Write(&val, 1);
    }

    bool NormalWriter::Write(int32_t val)
    {
        return _buffer->Write(&val, 4);
    }

    bool NormalWriter::Write(uint32_t val)
    {
        return _buffer->Write(&val, 4);
    }

    bool NormalWriter::Write(float val)
    {
        return _buffer->Write(&val, 4);
    }

    bool NormalWriter::Write(const std::string& val)
    {
        uint32_t size = (uint32_t)val.size();
        assert(size < 1024);

        if (!_buffer->Write(&size, 4)) return false;

        return _buffer->Write(val.c_str(), size);
    }
}
