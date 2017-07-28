/*
 * tab 文件解析器
*/
#pragma once

#include <array>
#include <memory>

template <size_t N>
class TabLineIter;

template <size_t N>
class TabElementIter
{
public:

};

template <size_t N>
class TabLineIter
{

};

template <size_t N, size_t LB = 3, char SEP = '\t'>
class TabParser
{
public:
    typedef std::array<const char*, N> ElementArray;

    TabParser(const ElementArray& elements)
        : _lineNO(0)
        , _titles(elements)
    {
        _index.fill(N);
    }

public:
    const size_t Size() const { return N; }
    const size_t Lines() const { return _lines.size(); }

    bool Parse(const char* data, size_t size)
    {
        if (data == nullptr || size == 0)
            return false;

        size_t length = 0;
        const char* cur = data;
        const char* end = data + size;
        while (true)
        {
            _lines.push_back(cur);

            cur = std::find_first_of(cur, end, '\n');
            length = std::max(length, cur - _lines.back());

            if (cur == end)
                break;

            ++cur;
        }

        if (Lines() <= LB)
            return false;

        _buffer.reset(new char[length + 1]);

        // parse element index
        Split(0);
        for (size_t i = 0; i < N; ++i)
        {
            for (size_t j = 0; j < N; ++j)
            {
                if (std::strcmp(_titles[i], _datas[j]))
                    _index[i] = j;
            }
        }

        Split(LB);
        return true;
    }

protected:
    void Split(size_t line)
    {
        size_t index = 0;
        size_t size = _lines[line] - _lines[line + 1];
        const char* beg = &_buffer[0];
        const char* end = &_buffer[size - 1];

        std::memcpy(_buffer.get(), _lines[line], size);
        _buffer[size - 1] = 0;

        _datas.fill(end);

        while (true)
        {
            _datas[index++] = beg;
            beg = std::find_first_of(beg, end, SEP);
            if (beg == end)
                break;

            *beg = 0;
            ++beg;
        }

        _lineNO = line;
    }

protected:
    const std::array<const char*, N>& _titles;  // tab titles
    std::array<size_t, N> _index;               // title visit index
    std::vector<const char*> _lines;            // line index

    std::array<const char*, N + 1> _datas;
    std::unique_ptr<char[]> _buffer;
    size_t _lineNO;
};
