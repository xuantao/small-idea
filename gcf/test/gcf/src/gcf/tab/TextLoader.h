#pragma once
#include "Parser.h"
#include "FileData.h"
#include "../serialize/SerUtility.h"

namespace tab
{
    template <typename Ty>
    struct Info;

    template <class Ty>
    struct TextLoader
    {
    public:
        TextLoader() : _parser(_idxMap)
        {
        }

    public:
        bool Setup(const char* file, int startLine, bool hasDefault)
        {
            auto fd = FileData::shared_from_this();
            if (!fd->Load(file, true))
                return false;

            if (!Setup(fd->Data(), startLine, hasDefault))
                return false;

            _fileData = fd; // hold the file data
            return true;
        }

        bool Setup(char* data, int startLine, bool hasDefault)
        {
            char* title = GetLine(data);
            if (title == nullptr)
                return false;

            if (!_idxMap.Setup(Info<Ty>::titles, title))
                return false;

            for (int i = 1; i < startLine; ++i)
            {
                if (GetLine(data) == nullptr)
                    return false;   // not enough data
            }

            if (_hasDefault)
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

        bool Load(Ty& out)
        {
            char* line = GetLine();
            if (line == nullptr)
                return false;

            if (_hasDefault)
                out = _default;

            return serialize::utility::Read(_parser.Parse(line), out);
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
        std::shared_ptr<FileData> _fileData;
        char* _data = nullptr;
        bool _hasDefault = false;
        Ty _default;

        IndexMap _idxMap;
        Parser _parser;
    };
}
