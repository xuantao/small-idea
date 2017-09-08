#pragma once

#include "ISerialize.h"

namespace serialize
{
    class IBinaryStream
    {
    public:
        virtual ~IBinaryStream() {}

    public:
        virtual bool Read(void* buf, uint32_t size) = 0;
        virtual bool Write(const void* buf, uint32_t size) = 0;
    };

    class BinaryReader : public IReader
    {
    public:
        BinaryReader(IBinaryStream* buffer);
        virtual ~BinaryReader() {}

    public:
        virtual bool StructBegin(uint32_t code, const char* name = nullptr);
        virtual bool StructEnd();
        virtual bool ArrayBegin(int& length, const char* name = nullptr);
        virtual bool ArrayEnd();
        virtual bool Read(bool& val, const char* name = nullptr);
        virtual bool Read(int32_t& val, const char* name = nullptr);
        virtual bool Read(uint32_t& val, const char* name = nullptr);
        virtual bool Read(float& val, const char* name = nullptr);
        virtual bool Read(std::string& val, const char* name = nullptr);

    protected:
        IBinaryStream* _stream = nullptr;
    };

    class BinaryWriter : public IWriter
    {
    public:
        BinaryWriter(IBinaryStream* buffer);
        virtual ~BinaryWriter() {}

    public:
        virtual bool StructBegin(uint32_t code, const char* name = nullptr);
        virtual bool StructEnd();
        virtual bool ArrayBegin(int length, const char* name = nullptr);
        virtual bool ArrayEnd();
        virtual bool Write(bool val, const char* name = nullptr);
        virtual bool Write(int32_t val, const char* name = nullptr);
        virtual bool Write(uint32_t val, const char* name = nullptr);
        virtual bool Write(float val, const char* name = nullptr);
        virtual bool Write(const std::string& val, const char* name = nullptr);

    protected:
        IBinaryStream* _stream = nullptr;
    };
}
