#pragma once
#include "TabDef.h"
#include "../serialize/ISerialize.h"
#include <sstream>
#include <vector>

namespace tab
{
    class TextReader : public serialize::IReader
    {
    public:
        TextReader(ITokenStream* tokens);
        ~TextReader();

    public:
        virtual bool StructBegin(int32_t code, const char* name = nullptr);
        virtual bool StructEnd();
        virtual bool ArrayBegin(int& length, const char* name = nullptr);
        virtual bool ArrayEnd();
        virtual bool Read(bool& val, const char* name = nullptr);
        virtual bool Read(int8_t& val, const char* name = nullptr);
        virtual bool Read(int32_t& val, const char* name = nullptr);
        virtual bool Read(int64_t& val, const char* name = nullptr);
        virtual bool Read(float& val, const char* name = nullptr);
        virtual bool Read(double& val, const char* name = nullptr);
        virtual bool Read(std::string& val, const char* name = nullptr);
        virtual bool Read(int32_t* val, int32_t count, const char* name = nullptr);
    protected:
        const char* Pop();

    protected:
        ITokenStream* _tokens = nullptr;
        bool _isArray = false;
        int _idx = 0;
        std::vector<char> _buff;
        std::vector<char*> _array;
    };

    class TextWriter : public serialize::IWriter
    {
    public:
        TextWriter(ITokenStream* tokens);
        ~TextWriter();

    public:
        virtual bool StructBegin(int32_t code, const char* name = nullptr);
        virtual bool StructEnd();
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
        bool Push(const char* str);

    protected:
        ITokenStream* _tokens = nullptr;
        bool _isArray = false;
        std::stringstream _array;
    };
}
