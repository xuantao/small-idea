#pragma once
#include "TabDef.h"
#include "Parser.h"
#include "FileData.h"
#include "../serialize/SerUtility.h"

namespace tab
{
    template <class Ty>
    class TextLoader : public Loader<Ty>
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
}
