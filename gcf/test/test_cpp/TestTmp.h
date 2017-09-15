#pragma once
#include <cstring>

enum class TestEnum
{
    a
};

struct TmpTy
{
};

template <class Ty>
struct TabInfo
{
    typedef const char* cstr;
    typedef const cstr ccstr;
    //static const ccstr title[] = {nullptr};
};


template <>
struct TabInfo<TmpTy>
{
    static const char* const title[];
    static const char* const types[];
    static const char* const descs[];
};

namespace detail
{
    template <class Ty>
    struct Data {};

    template <>
    struct Data<TestEnum>
    {
        static const int size;
        static const char* const names[];
        static const TestEnum vals[];
    };
}


template <class Info>
struct EnumBase
{
    typedef Info _Info;

    static const char* Name(int v)
    {
        for (int i = 0; i < _Info::size; ++i)
        {
            if (_Info::data[i].value == v)
                return _Info::data[i].name;
        }

        return nullptr;
    }
};

template <class En>
const char* Name(En val)
{
    typedef detail::Data<En> Data;
    for (int i = 0; i < Data::size; ++i)
    {
        if (Data::vals[i] == val)
            return Data::names[i];
    }

    return nullptr;
}

template <class En>
const char* Name(int idx)
{
    typedef detail::Data<En> Data;

    if (idx >= 0 && idx < Data::size)
        return Data::names[idx];

    return nullptr;
}

template <class En>
En Value(const char* name)
{
    typedef detail::Data<En> Data;
    for (int i = 0; i < Data::size; ++i)
    {
        if (std::strcmp(Data::names[i], name) == 0)
            return Data::vals[i];
    }

    return Data::vals[0];
}

template <class En>
En Value(int idx)
{
    typedef detail::Data<En> Data;

    if (idx >= 0 && idx < Data::size)
        return Data::vals[idx];

    return Data::vals[0];
}

template<class En>
int Index(En val)
{
    typedef detail::Data<En> Data;

    for (int i = 0; i < Data::size; ++i)
    {
        if (Data::vals[i] == val)
            return i;
    }

    return -1;
}

template <class En>
int Index(const char* name)
{
    typedef detail::Data<En> Data;

    for (int i = 0; i < Data::size; ++i)
    {
        if (std::strcmp(Data::names[i], name) == 0)
            return i;
    }

    return -1;
}

void TestFunc();
