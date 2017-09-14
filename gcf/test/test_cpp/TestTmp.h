#pragma once

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

void TestFunc();
