#pragma once

#include "Serialize.h"

namespace serialize
{
    class IBuffer
    {
    public:
        virtual ~IBuffer() {}

    public:
        virtual bool Read(void* buf, uint32_t size) = 0;
        virtual bool Write(const void* buf, uint32_t size) = 0;
    };

    class NormalReader : public IReader
    {
    public:
        NormalReader(IBuffer* buffer);
        virtual ~NormalReader() {}

    public:
        virtual bool StructBegin(uint32_t code);
        virtual bool StructEnd();
        virtual bool Read(bool& val);
        virtual bool Read(int32_t& val);
        virtual bool Read(uint32_t& val);
        virtual bool Read(float& val);
        virtual bool Read(std::string& val);

    protected:
        IBuffer* _buffer = nullptr;
    };

    class NormalWriter : public IWriter
    {
    public:
        NormalWriter(IBuffer* buffer);
        virtual ~NormalWriter() {}

    public:
        virtual bool StructBegin(uint32_t code);
        virtual bool StructEnd();
        virtual bool Write(bool val);
        virtual bool Write(int32_t val);
        virtual bool Write(uint32_t val);
        virtual bool Write(float val);
        virtual bool Write(const std::string& val);

    protected:
        IBuffer* _buffer = nullptr;
    };
}

