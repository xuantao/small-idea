/*
 * 用于序列化的相关接口和辅助函数
*/
#pragma once

#include <stdint.h>
#include <string>
#include <array>
#include <vector>

namespace serialize
{
    class IReader
    {
    public:
        virtual ~IReader() {}

    public:
        virtual bool StructBegin(int32_t code, const char* name = nullptr) = 0;
        virtual bool StructEnd() = 0;
        virtual bool ArrayBegin(int& length, const char* name = nullptr) = 0;
        virtual bool ArrayEnd() = 0;

        virtual bool Read(bool& val, const char* name = nullptr) = 0;
        virtual bool Read(int8_t& val, const char* name = nullptr) = 0;
        virtual bool Read(int32_t& val, const char* name = nullptr) = 0;
        virtual bool Read(int64_t& val, const char* name = nullptr) = 0;
        virtual bool Read(float& val, const char* name = nullptr) = 0;
        virtual bool Read(double& val, const char* name = nullptr) = 0;
        virtual bool Read(std::string& val, const char* name = nullptr) = 0;

        //virtual bool ReadArray(bool& val, const char* name = nullptr) = 0;
        //virtual bool Read(int8_t& val, const char* name = nullptr) = 0;
        virtual bool Read(int32_t* val, int count, const char* name = nullptr) = 0;
        //virtual bool Read(int64_t& val, const char* name = nullptr) = 0;
        //virtual bool Read(float& val, const char* name = nullptr) = 0;
        //virtual bool Read(double& val, const char* name = nullptr) = 0;
        //virtual bool Read(std::string& val, const char* name = nullptr) = 0;
    };

    class IWriter
    {
    public:
        virtual ~IWriter() {}

    public:
        virtual bool StructBegin(int32_t code, const char* name = nullptr) = 0;
        virtual bool StructEnd() = 0;
        virtual bool ArrayBegin(int length, const char* name = nullptr) = 0;
        virtual bool ArrayEnd() = 0;
        virtual bool Write(bool val, const char* name = nullptr) = 0;
        virtual bool Write(int8_t val, const char* name = nullptr) = 0;
        virtual bool Write(int32_t val, const char* name = nullptr) = 0;
        virtual bool Write(int64_t val, const char* name = nullptr) = 0;
        virtual bool Write(float val, const char* name = nullptr) = 0;
        virtual bool Write(double val, const char* name = nullptr) = 0;
        virtual bool Write(const std::string& val, const char* name = nullptr) = 0;
    };
}
