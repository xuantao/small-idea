#pragma once
#include "Parser.h"
#include "../serialize/SerUtility.h"

namespace tab
{
    namespace detail
    {
        template <class Ty>
        struct Loader
        {
        public:
            Loader(char* text, bool hasDefault)
                : _parser(_idxMap)
                , _data(text)
                , _hasDefault(hasDefault)
            {
            }

        public:
            bool Setup(int startLine)
            {
                char* title = GetLine();
                if (title == nullptr)
                    return false;

                if (!_idxMap.Setup(Info<Ty>::titles, title))
                    return false;

                for (int i = 1; i < startLine; ++i)
                {
                    if (GetLine() == nullptr)
                        return false;   // not enough data
                }

                if (_hasDefault)
                {
                    char* text = GetLine();
                    if (text == nullptr)
                        return false;

                    if (!serialize::utility::Read(_parser.Parse(text), _default))
                        return false;
                }

                return true;
            }

            bool LoadOne(Ty& out)
            {
                char* line = GetLine();
                if (line == nullptr)
                    return false;

                if (_hasDefault)
                    out = _default;

                return serialize::utility::Read(_parser.Parse(line), out);
            }

        protected:
            // this will skip empty line
            char* GetLine()
            {
                char* rd = _data + _rdPos;
                char* text = nullptr;

                while (true)
                {
                    text = rd;

                    // move to line end
                    while (*rd != '\n' && *rd != 0)
                        ++rd;

                    if (*rd == 0)
                        break;

                    *rd = 0;
                    ++rd;

                    if (*text != 0)
                        break;
                }

                _rdPos = (int)(rd - _data);
                return *text ? text : nullptr;
            }

        protected:
            char* _data = nullptr;
            bool _hasDefault = false;
            Ty _default;

            int _rdPos = 0;
            IndexMap _idxMap;
            Parser _parser;
        };
    }

    template <class Ty>
    class DataList
    {
    public:
        // load text
        bool Load(char* data, int startLine = 3, bool hasDefault = true)
        {
            detail::Loader<Ty> loader(data, hasDefault);
            if (!loader.Setup(startLine))
                return false;

            while (true)
            {
                Ty tyData;
                if (!loader.LoadOne(tyData))
                    break;

                _data.push_back(tyData);
            }

            return true;
        }

        // load binary
        bool Load(const void* data, int size)
        {
            return false;
        }
    protected:
        std::vector<Ty> _data;
    };
}
