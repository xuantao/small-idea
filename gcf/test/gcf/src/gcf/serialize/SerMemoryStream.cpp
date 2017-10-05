#include "SerMemoryStream.h"
#include <cassert>

namespace serialize
{
    MemoryReadOnly::MemoryReadOnly()
    {
    }

    MemoryReadOnly::~MemoryReadOnly()
    {
    }

    bool MemoryReadOnly::Init(const void* buff, size_t size)
    {
        _buff = (const int8_t*)buff;
        _size = size;
        _pos = 0;
        return true;
    }

    void MemoryReadOnly::UnInit()
    {
        _buff = nullptr;
        _size = 0;
        _pos = 0;
    }

    void MemoryReadOnly::Reset()
    {
        _pos = 0;
    }

    bool MemoryReadOnly::Read(void* buf, int32_t size)
    {
        if (_size - _pos < size)
            return false;

        memcpy(buf, _buff + _pos, size);
        _pos += size;
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    // write only
    MemoryWriteOnly::MemoryWriteOnly()
    {
    }

    MemoryWriteOnly::~MemoryWriteOnly()
    {
        if (_buff)
            free(_buff);
    }

    bool MemoryWriteOnly::Write(const void* buf, int32_t size)
    {
        while (_pos + size > _size)
        {
            if (_size == 0)
                _size = 1024;
            else if (_size >= 32 * 1024)
                _size += 32 * 1024;
            else
                _size *= 2;

            if (_buff == nullptr)
                _buff = (int8_t*)malloc(_size);
            else
                _buff = (int8_t*)realloc(_buff, _size);
            assert(_buff && "not enough memory");
        }

        memcpy(_buff + _pos, buf, size);
        _pos += size;

        return true;
    }
}
