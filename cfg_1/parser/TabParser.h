/*
 * tab 文件解析器
*/
#pragma once

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

template <size_t N>
class TabParser
{
public:
    TabParser(const char* data, const char* const elements[])
    {

    }
public:
    const size_t Size() const { return N; }

    bool Parse()
    {
        return true;
    }



protected:
    size_t _index[N];
};
