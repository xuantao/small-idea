using System;

namespace Tab
{
    public class Parser
    {
        class TokenStream : ITokenStream
        {
            Parser _parser = null;
            public TokenStream(Parser parser)
            { _parser = parser; }
            public string Pop()
            { return _parser.Pop(); }
            public bool Push(string token)
            { throw new NotImplementedException(); }
        }

        IndexMap _idxMap;
        TokenStream _stream;
        TextReader _reader;
        int _index = 0;
        string[] _data = null;

        public Parser(IndexMap idxMap)
        {
            _idxMap = idxMap;
            _stream = new TokenStream(this);
            _reader = new TextReader(_stream);
        }

        public Serialize.IReader Parse(string line)
        {
            if (string.IsNullOrEmpty(line))
                return null;

            _index = 0;
            _data = line.Split('\t');
            return _reader;
        }

        string Pop()
        {
            if (_data == null)
                return null;

            int idx = _idxMap.Index(_index);
            ++_index;

            if (idx == -1 || idx >= _data.Length)
                return null;

            return _data[idx];
        }
    }
}
