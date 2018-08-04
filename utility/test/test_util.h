/*
 * test utilite
 * xuantao, 2017
*/
#pragma once

#include "../common.h"
#include <stdio.h>

UTILITY_NAMESPACE_BEGIN

namespace test
{
    struct Tab
    {
        Tab() : _t(tab) {}
        ~Tab() { tab = _t; }

        static int tab;
    private:
        int _t;
    };

    template <typename ...Ty>
    void log(const char* format, Ty&&... param)
    {
        log(Tab::tab, format, std::forward<Ty>(param)...);
    }

    template <typename ...Ty>
    void log(int tab, const char* format, Ty&&... param)
    {
        while (tab--)
            printf("    ");
        printf(format, std::forward<Ty>(param)...);
        printf("\n");
    }

    struct Obj
    {
        Obj() : _d(0), _idx(++index)
        {
            test::log("i:%d, d:%d TestInt()", _idx, _d);
        }

        Obj(int a) : _d(a), _idx(++index)
        {
            test::log("i:%d, d:%d TestInt(int)", _idx, _d);
        }

        Obj(Obj&& o) : _d(o._d), _idx(++index)
        {
            test::log("i:%d, d:%d TestInt(TestObj&&)", _idx, _d);
        }

        Obj(const Obj& o) : _d(o._d), _idx(++index)
        {
            test::log("i:%d, d:%d TestInt(const TestInt&)", _idx, _d);
        }

        ~Obj()
        {
            test::log("i:%d, d:%d ~TestInt()", _idx, _d);
        }

        Obj& operator = (const Obj& o)
        {
            _d = o._d;
            test::log("i:%d d:%d TestInt& operator = (const TestInt&)", _idx, _d);
            return *this;
        }

        bool operator == (int d) const
        {
            return d == _d;
        }

        bool operator < (const Obj& other) const
        {
            return _d < other._d;
        }
    protected:
        int _d;
        int _idx;

        static int index;
    };
}

UTILITY_NAMESPACE_END
