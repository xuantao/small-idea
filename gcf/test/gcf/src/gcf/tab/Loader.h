#pragma once
#include "TabDef.h"
#include "Parser.h"
#include "../serialize/SerUtility.h"
#include "../serialize/SerBinary.h"
#include "../serialize/SerMemoryStream.h"



namespace tab
{
    template <class Ty>
    class ILoader
    {
    public:
        virtual bool Load(Ty& val) = 0;
    };

    template <class Ty>
    class TextLoader : public ILoader<Ty>
    {
    public:
        TextLoader() : _parser(_idxMap)
        {
        }

    public:
        bool Setup(char* data, int startLine, bool hasDefault)
        {
            char* title = GetLine(data);
            if (title == nullptr)
                return false;

            if (!_idxMap.Setup(detail::Info<Ty>::titles, title))
                return false;

            for (int i = 1; i < startLine; ++i)
            {
                if (GetLine(data) == nullptr)
                    return false;   // not enough data
            }

            if (hasDefault)
            {
                char* text = GetLine(data);
                if (text == nullptr)
                    return false;

                if (!serialize::utility::Read(_parser.Parse(text), _default))
                    return false;
            }

            _data = data;
            _hasDefault = hasDefault;
            return true;
        }

    public:
        virtual bool Load(Ty& val)
        {
            char* line = GetLine(_data);
            if (line == nullptr)
                return false;

            if (_hasDefault)
                val = _default;

            return serialize::utility::Read(_parser.Parse(line), val);
        }

    protected:
        char* GetLine(char*& data)
        {
            if (data == nullptr)
                return nullptr;

            char* text = nullptr;
            while (true)
            {
                text = data;
                // move to line end
                while (*data != '\n' && *data != 0)
                    ++data;

                if (*data == '\n')
                {
                    *data = 0;
                    ++data;

                    if (*text != 0)
                        break; // skip empty line
                }
                else
                {
                    break;
                }
            }

            return *text ? text : nullptr;
        }

    protected:
        char* _data = nullptr;
        bool _hasDefault = false;
        Ty _default;

        IndexMap _idxMap;
        Parser _parser;
    };

    template <class Ty>
    class BinaryLoader : public ILoader<Ty>
    {
    public:
        BinaryLoader() : _reader(&_stream)
        {
        }

    public:
        int HashCode() const { return _code; }

        bool Setup(const void* data, size_t size)
        {
            if (!_stream.Init(data, size))
                return false;

            if (!_reader.Read(_code, nullptr))
                return false;
            if (!_reader.Read(_count, nullptr))
                return false;

            return _count >= 0;
        }

        virtual bool Load(Ty& val)
        {
            if (_count == 0)
                return false;

            --_count;
            return serialize::utility::Read(&_reader, val, nullptr);
        }

    protected:
        int _code = 0;
        int _count = 0;
        serialize::MemoryReadOnly _stream;
        serialize::BinaryReader _reader;
    };
}
