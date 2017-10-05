#pragma once
#include "IndexMap.h"
#include "../serialize/SerText.h"

namespace tab
{
    class Parser
    {
    public:
        Parser(const IndexMap& idxMap);
        ~Parser();
        Parser(const Parser&) = delete;
        Parser& operator = (const Parser&) = delete;

    public:
        /*
         * 解析一行数据
         * line数据必须是以0结尾
         * NOTE: 此解析函数会将字符串中的\t替换为0, 转换为独立字符串
        */
        serialize::IReader* Parse(char* line);

    protected:
        const char* Pop();

    protected:
        class TokenStream : public serialize::ITokenReadStream
        {
        public:
            TokenStream(Parser& parser) : _parser(parser) {}
            ~TokenStream() {}
        public:
            virtual const char* Read() { return _parser.Pop(); }
        protected:
            Parser& _parser;
        };

    protected:
        const IndexMap& _idxMap;
        TokenStream _stream;
        serialize::TextReader _reader;
        int _idx = -1;
        std::vector<char*> _tokens;
    };
}
