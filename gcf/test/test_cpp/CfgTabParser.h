/*
 * tab 文件解析器
*/
#pragma once

#include <vector>
#include <array>
#include <memory>
#include <cassert>

namespace cfg
{
    class TabLineIter
    {
    public:
        TabLineIter(const char* const * data, const char* const * title, size_t size, const char* chunk, size_t lineNO)
            : _data(data)
            , _title(title)
            , _size(size)
            , _chunk(chunk)
            , _lineNO(lineNO)
            , _index(-1)
        {
        }

    public:
        const char* Chunk() const { return _chunk; }
        size_t LineNO() const { return _lineNO; }

        const char* Value() const
        {
            assert(_index >= 0 && _index < _size);
            return _data[_index];
        }

        const char* Title() const
        {
            assert(_index >= 0 && _index < _size);
            return _title[_index];
        }

        bool MoveNext()
        {
            if (_index + 1 >= _size)
            {
                assert(false);
                return false;
            }

            ++_index;
            return true;
        }

    protected:
        const char* const * _data;
        const char* const * _title;
        const char* _chunk;
        size_t _lineNO;
        size_t _size;
        int _index;
    };

    /*
     * tab 文件数据解析器
     * N: 每行拥有的数据数量
     * LB: 起始行号, 起始行一般为(title, type, desc)
     * SEP: 分隔符, 一行元素的分割符
    */
    template <size_t N, size_t LB = 3, char SEP = '\t'>
    class TabParser
    {
    public:
        static_assert(LB > 0, "first line is title, can not be set 0");
    public:
        TabParser(const std::array<const char*, N>& elements)
            : _lineNO(LB - 1)
            , _titles(elements)
            , _chunk("")
        {
        }

    public:
        const size_t Size() const { return N; }
        const size_t Lines() const { return _lines.size() - LB - 1; }
        const size_t LineNO() const { return _lineNO - LB; }

        TabLineIter LineIter()
        {
            return TabLineIter(&_datas[0], &_titles[0], N, "", LineNO());
        }

        bool NextLine()
        {
            if (LineNO() + 1 >= Lines())
                return false;

            Split(_lineNO + 1);
            return true;
        }

        bool Parse(const char* data, size_t size, const char* chunk = nullptr)
        {
            if (data == nullptr || size == 0)
                return false;

            _chunk = chunk ? chunk : "";

            size_t maxLength = InitLines(data, size);

            if (_lines.size() <= LB + 1)
                return false;

            _buffer.reset(new char[maxLength + 1]);

            InitTitles();

            return true;
        }

    protected:
        size_t InitLines(const char* data, size_t size)
        {
            size_t length = 0;
            const char* cur = data;
            const char* end = data + size;
            while (true)
            {
                _lines.push_back(cur);

                cur = std::find(cur, end, '\n');
                length = std::max(length, (size_t)(cur - _lines.back()));

                if (cur == end || cur + 1 == end)   // skip last empty line
                {
                    _lines.push_back(cur + 1);
                    break;
                }

                ++cur;
            }
            return length;
        }

        void InitTitles()
        {
            const char* cur = _lines[0];
            const char* end = _lines[1] - 1;

            while (true)
            {
                const char* beg = cur;
                cur = std::find(beg, end, SEP);
                _index.push_back(N);
                for (size_t i = 0; i < N; ++i)
                {
                    if (std::strncmp(_titles[i], beg, cur - beg) == 0)
                    {
                        _index.back() = i;
                        break;
                    }
                }

                if (cur == end)
                    break;
                else
                    ++cur;
            }
        }

        void Split(size_t line)
        {
            size_t pos = 0;
            size_t size = _lines[line + 1] - _lines[line];
            char* beg = &_buffer[0];
            char* end = &_buffer[size - 1];

            std::memcpy(_buffer.get(), _lines[line], size);
            _buffer[size - 1] = 0;

            _datas.fill(end);

            while (pos < _index.size())
            {
                _datas[_index[pos]] = beg;

                beg = std::find(beg, end, SEP);
                *beg = 0;

                if (beg == end)
                    break;

                ++beg;
                ++pos;
            }

            _lineNO = line;
        }

    protected:
        const std::array<const char*, N>& _titles;  // titles
        std::vector<const char*> _lines;            // line data
        std::vector<size_t> _index;                 // title -> index

        const char* _chunk;
        std::array<const char*, N + 1> _datas;      // line element data
        std::unique_ptr<char[]> _buffer;            // line data buffer
        size_t _lineNO;                             // current line NO.
    };
} // end of namespace cfg
