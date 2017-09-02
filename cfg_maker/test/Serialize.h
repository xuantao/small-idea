#pragma once

#include <stdint.h>
#include <string>

namespace serialize
{
    class IReader
    {
    public:
        virtual ~IReader() {}
    public:
        virtual bool StructBegin(int hashID) = 0;
        virtual bool StructEnd() = 0;
        virtual bool Read(int& n) = 0;
        virtual bool Read(float& f) = 0;
        virtual bool Read(bool& b) = 0;
        virtual bool Read(std::string& s) = 0;
    };

    class IWriter
    {
    public:
        virtual ~IWriter() {}
    public:
        virtual bool StructBegin(int hashID) = 0;
        virtual bool StructEnd() = 0;
        virtual bool Write(int n) = 0;
        virtual bool Write(float f) = 0;
        virtual bool Write(bool b) = 0;
        virtual bool Write(const std::string& s) = 0;
    };
}
