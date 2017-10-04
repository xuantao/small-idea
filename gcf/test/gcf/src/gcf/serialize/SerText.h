#pragma once
#include "ISerialize.h"
#include <sstream>
#include <vector>

namespace serialize
{
    class TextReader : public IReader
    {
    public:
        TextReader(ITokenStream* stream);
        ~TextReader();

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
        const char* DoRead();

    protected:
        ITokenStream* _stream = nullptr;
        bool _isArray = false;
        int _idx = 0;
        std::vector<char> _buff;
        std::vector<char*> _array;
    };

    class TextWriter : public IWriter
    {
    public:
        TextWriter(ITokenStream* tokens);
        ~TextWriter();

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
        bool DoWrite(const char* str);

    protected:
        ITokenStream* _stream = nullptr;
        bool _isArray = false;
        std::stringstream _array;
    };
}
