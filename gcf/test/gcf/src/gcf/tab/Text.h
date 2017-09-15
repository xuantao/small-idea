#pragma once
#include "TabDef.h"
#include "../serialize/ISerialize.h"
#include <sstream>

namespace tab
{
    class TextReader : public serialize::IReader
    {
    public:
        TextReader(ITokenStream* tokens);
        ~TextReader();

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
        const char* Pop();

    protected:
        ITokenStream* _tokens = nullptr;
        bool _isArray = false;
        int _idx = 0;
        std::vector<std::string> _array;
    };

    class TextWriter : public serialize::IWriter
    {
    public:
        TextWriter(ITokenStream* tokens);
        ~TextWriter();

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
        bool Push(const char* str);

    protected:
        ITokenStream* _tokens = nullptr;
        bool _isArray = false;
        std::stringstream _array;
    };
}
