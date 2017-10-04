#include "Parser.h"
#include <cassert>

namespace tab
{
    Parser::Parser(const IndexMap& idxMap)
        : _idxMap(idxMap)
        , _stream(*this)
        , _reader(&_stream)
    {
    }

    Parser::~Parser()
    {
    }

    serialize::IReader* Parser::Parse(char* line)
    {
        int idx = 0;
        int beg = 0;

        _tokens.assign(_idxMap.TitleSize(), "");
        while (idx < (int)_idxMap.TitleSize())
        {
            int pos = beg;
            while (line[pos] != 0 && line[pos] != '\t')
                ++pos;

            _tokens[idx++] = line + beg;

            if (line[pos] == 0)
                break;

            line[pos] = 0;
            beg = pos + 1;
        }

        _idx = 0;
        return &_reader;
    }

    const char* Parser::Pop()
    {
        if (_idx < 0 || _idx >= (int)_idxMap.Size())
            return nullptr;

        int realIdx = _idxMap.Index(_idx++);
        if (realIdx == -1)
            return "";

        assert(realIdx <= _tokens.size());
        return _tokens[realIdx];
    }

    bool Parser::Push(const char*)
    {
        assert(false);
        return false;
    }
}
