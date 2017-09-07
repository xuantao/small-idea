#include "Station.h"

namespace cross_call
{
    SwapBuffer::SwapBuffer(char* buffer, int size)
        : _header((SwapBuffer::Header*)buffer)
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

        _rp = _wp = 0;
        return _header->ds;
    }

    bool SwapBuffer::Read(void* buf, uint32_t size)
    {
        return true;
    }

    bool SwapBuffer::Write(const void* buf, uint32_t size)
    {
        return true;
    }
}
