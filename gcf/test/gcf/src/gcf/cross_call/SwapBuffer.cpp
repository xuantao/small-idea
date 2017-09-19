#include "SwapBuffer.h"

namespace cross_call
{
    SwapBuffer::SwapBuffer(char* buffer, int size)
        : _header((SwapBuffer::Header*)buffer)
        , _base((char*)buffer)
        , _size(size)
    {
    }

    void SwapBuffer::Startup(BufferMode mode)
    {
        _mode = mode;
        _rp = _wp = 4;
    }

    int SwapBuffer::Endup()
    {
        if (_mode == BufferMode::Write)
            _header->ds = _wp;
        else
            _header->ds = 0;

        _rp = _wp = 4;
        return _header->ds;
    }

    bool SwapBuffer::Read(void* buf, int32_t size)
    {
        if (_mode != BufferMode::Read)
            return false;
        if (_rp + (int)size > _header->ds)
            return false;

        memcpy(buf, _base + _rp, size);
        _rp += (int)size;
        return true;
    }

    bool SwapBuffer::Write(const void* buf, int32_t size)
    {
        if (_mode != BufferMode::Write)
            return false;
        if (_wp + (int)size > _size)
            return false;

        memcpy(_base + _wp, buf, size);
        _wp += (int)size;
        return true;
    }
}
