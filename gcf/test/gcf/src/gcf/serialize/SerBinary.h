﻿#pragma once
#include "ISerialize.h"

namespace serialize
{
    class BinaryReader : public serialize::IReader
    {
    public:
        BinaryReader(IBinaryReadStream* stream);
        virtual ~BinaryReader() {}

    public:
        virtual bool StructBegin(int32_t code, const char* name = nullptr) { return true; }
        virtual bool StructEnd() { return true; }

        virtual bool ArrayBegin(int& length, const char* name = nullptr);
        virtual bool ArrayEnd();
        virtual bool Read(bool& val, const char* name = nullptr);
        virtual bool Read(int8_t& val, const char* name = nullptr);
        virtual bool Read(int32_t& val, const char* name = nullptr);
        virtual bool Read(int64_t& val, const char* name = nullptr);
        virtual bool Read(float& val, const char* name = nullptr);
        virtual bool Read(double& val, const char* name = nullptr);
        virtual bool Read(std::string& val, const char* name = nullptr);

    protected:
        IBinaryReadStream* _stream = nullptr;
    };

    class BinaryWriter : public serialize::IWriter
    {
    public:
        BinaryWriter(IBinaryWriteStream* stream);
        virtual ~BinaryWriter() {}

    public:
        virtual bool StructBegin(int32_t code, const char* name = nullptr) { return true; }
        virtual bool StructEnd() { return true; }

        virtual bool ArrayBegin(int length, const char* name = nullptr);
        virtual bool ArrayEnd();
        virtual bool Write(bool val, const char* name = nullptr);
        virtual bool Write(int8_t val, const char* name = nullptr);
        virtual bool Write(int32_t val, const char* name = nullptr);
        virtual bool Write(int64_t val, const char* name = nullptr);
        virtual bool Write(float val, const char* name = nullptr);
        virtual bool Write(double val, const char* name = nullptr);
        virtual bool Write(const std::string& val, const char* name = nullptr);

    protected:
        IBinaryWriteStream* _stream = nullptr;
    };
}
