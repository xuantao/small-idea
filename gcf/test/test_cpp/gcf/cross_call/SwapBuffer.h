#pragma once

#include "ICrossCall.h"

namespace cross_call
{
    enum class BufferMode
    {
        Read,
        Write,
    };

    class SwapBuffer : public IBinaryStream
    {
    public:
        SwapBuffer(char* buffer, int size);

    public:
        BufferMode Mode() const { return _mode; }
        void Startup(BufferMode mode);
        int Endup();

    public:
        virtual bool Read(void* buf, uint32_t size);
        virtual bool Write(const void* buf, uint32_t size);

    protected:
        struct Header
        {
            int32_t  ds;
        };

        BufferMode _mode = BufferMode::Read;
        Header* _header = nullptr;
        char* _base = nullptr;
        int _size = 0;
        int _rp = 0;
        int _wp = 0;
    };
}
