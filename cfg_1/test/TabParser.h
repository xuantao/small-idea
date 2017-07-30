/*
 * tab 文件解析器
*/
#pragma once

#include <array>
#include <memory>
#include <iterator>

template <size_t N>
class TabLineIter;

template <typename Ty>
class TabElementIter : public std::random_access_iterator_tag
{
public:
    typedef Ty* pointer;
    typedef Ty& reference;
    typedef std::ptrdiff_t difference_type;
    typedef TabElementIter<Ty> _Myiter;

public:
    TabElementIter(size_t index, const pointer data, size_t size)
        : _index(index), _data(data), _size(size)
    { }

    TabElementIter(const TabElementIter& other)
        : _index(other._index)
        , _size(other._size)
        , _data(other._data)
    { }

    ~TabElementIter() {}

public:
    TabElementIter& operator = (const TabElementIter& other)
    {
        _index = other._index;
        _size = other._size;
        _data = other._data;
        return *this;
    }

public:
    reference operator*() const
    {
        return _data[_index];
    }

    pointer operator->() const
    {
        return &_data[_index];
    }

    _Myiter& operator++()
    {
        ++_index;
        return *this;
    }

    _Myiter operator++(int)
    {
        _Myiter _Tmp = *this;
        ++*this;
        return (_Tmp);
    }

    _Myiter& operator--()
    {
        --_index;
        return (*this);
    }

    _Myiter operator--(int)
    {
        _Myiter _Tmp = *this;
        --*this;
        return (_Tmp);
    }

    _Myiter& operator+=(difference_type _Off)
    {
        _index += _Off;
        return (*this);
    }

    _Myiter operator+(difference_type _Off) const
    {
        _Myiter _Tmp = *this;
        return (_Tmp += _Off);
    }

    _Myiter& operator-=(difference_type _Off)
    {
        return (*this += -_Off);
    }

    _Myiter operator-(difference_type _Off) const
    {
        _Myiter _Tmp = *this;
        return (_Tmp -= _Off);
    }

    difference_type operator-(const _Myiter& _Right) const
    {
        return _index - _Right._index;
    }

    reference operator[](difference_type _Off) const
    {
        return _data[_Off];
    }

    bool operator == (const _Myiter& _Right) const
    {
        return _data == _Right._data && _index == _Right._index;
    }

    bool operator != (const _Myiter& _Right) const
    {
        return !((*this) == _Right);
    }

protected:
    size_t _index;
    size_t _size;
    const pointer _data;
};

template <size_t N, size_t LB = 3, char SEP = '\t'>
class TabParser
{
public:
    typedef TabElementIter<const char* const> Iterator;

public:
    TabParser(const std::array<const char*, N>& elements)
        : _lineNO(0)
        , _titles(elements)
    {
    }

public:
    const size_t Size() const { return N; }
    const size_t Lines() const { return _lines.size() - LB - 1; }
    const size_t LineNO() const { return _lineNO - LB; }

    Iterator Begin() const { return Iterator(0, &_datas[0], N); }
    Iterator End() const { return Iterator(N, &_datas[0], N); }

    bool NextLine()
    {
        if (LineNO() + 1 >= Lines())
            return false;

        Split(_lineNO + 1);
        return true;
    }

    bool Parse(const char* data, size_t size)
    {
        if (data == nullptr || size == 0)
            return false;

        size_t maxLength = InitLines(data, size);

        if (_lines.size() <= LB + 1)
            return false;

        _buffer.reset(new char[maxLength + 1]);

        InitTitles();

        Split(LB);
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
        size_t size =_lines[line + 1] - _lines[line];
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
    const std::array<const char*, N>& _titles;  // tab titles
    std::vector<const char*> _lines;            // line index
    std::vector<int> _index;                    // title visit index

    std::array<const char*, N + 1> _datas;
    std::unique_ptr<char[]> _buffer;            // line data buffer
    size_t _lineNO;
};
