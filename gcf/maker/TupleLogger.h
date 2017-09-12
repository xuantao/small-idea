#pragma once
#include "CfgDef.h"
#include <tuple>

CFG_NAMESPACE_BEGIN

#define TUPLE_LOGGER_UST_MACRO 1

namespace detail
{
#define _TUPLE_LOGGER_BEGIN(N)                  \
    template <>                                 \
    struct TupleLogger<N>                       \
    {                                           \
        static const int C = N;                 \
                                                \
        template <class... Args>                \
        static void log(std::ostream& out,      \
            int idx, std::tuple<Args...>& tp)   \
        {                                       \
            switch (idx)                        \
            {                                   \
                case 0: out << std::get<0>(tp); break

#define _TUPLE_LOGGER_CASE(i) case i: out << std::get<i>(tp); break

#define _TUPLE_LOOGER_END                       \
            default:                            \
                break;                          \
            }                                   \
        }                                       \
    }

    template <int N>
    struct TupleLogger
    {
        static const int C = N;

        static void log(std::ostream& out, int idx, ...)
        {
            static_assert(C <= 10, "does not support argument size greater than 10");
        }
    };

    template <>
    struct TupleLogger<0>
    {
        static const int C = 0;

        static void log(std::ostream& out, int idx, ...)
        {
        }
    };

    template <>
    struct TupleLogger<1>
    {
        static const int C = 1;

        template <class... Args>
        static void log(std::ostream& out, int idx, std::tuple<Args...>& tp)
        {
            out << std::get<0>(tp);
        }
    };

#if !TUPLE_LOGGER_UST_MACRO
    template <>
    struct TupleLogger<2>
    {
        static const int C = 2;

        template <class... Args>
        static void log(std::ostream& out, int idx, std::tuple<Args...>& tp)
        {
            switch (idx)
            {
                _TUPLE_LOGGER_CASE(0);
                _TUPLE_LOGGER_CASE(1);
            default:
                break;
            }
        }
    };

    template <>
    struct TupleLogger<3>
    {
        static const int C = 3;

        template <class... Args>
        static void log(std::ostream& out, int idx, std::tuple<Args...>& tp)
        {
            switch (idx)
            {
                _TUPLE_LOGGER_CASE(0);
                _TUPLE_LOGGER_CASE(1);
                _TUPLE_LOGGER_CASE(2);
            }
        }
    };

    template <>
    struct TupleLogger<4>
    {
        static const int C = 4;

        template <class... Args>
        static void log(std::ostream& out, int idx, std::tuple<Args...>& tp)
        {
            switch (idx)
            {
                _TUPLE_LOGGER_CASE(0);
                _TUPLE_LOGGER_CASE(1);
                _TUPLE_LOGGER_CASE(2);
                _TUPLE_LOGGER_CASE(3);
            }
        }
    };

    template <>
    struct TupleLogger<5>
    {
        static const int C = 5;

        template <class... Args>
        static void log(std::ostream& out, int idx, std::tuple<Args...>& tp)
        {
            switch (idx)
            {
                _TUPLE_LOGGER_CASE(0);
                _TUPLE_LOGGER_CASE(1);
                _TUPLE_LOGGER_CASE(2);
                _TUPLE_LOGGER_CASE(3);
                _TUPLE_LOGGER_CASE(4);
            }
        }
    };

    template <>
    struct TupleLogger<6>
    {
        static const int C = 6;

        template <class... Args>
        static void log(std::ostream& out, int idx, std::tuple<Args...>& tp)
        {
            switch (idx)
            {
                _TUPLE_LOGGER_CASE(0);
                _TUPLE_LOGGER_CASE(1);
                _TUPLE_LOGGER_CASE(2);
                _TUPLE_LOGGER_CASE(3);
                _TUPLE_LOGGER_CASE(4);
                _TUPLE_LOGGER_CASE(5);
            }
        }
    };

    template <>
    struct TupleLogger<7>
    {
        static const int C = 7;

        template <class... Args>
        static void log(std::ostream& out, int idx, std::tuple<Args...>& tp)
        {
            switch (idx)
            {
                _TUPLE_LOGGER_CASE(0);
                _TUPLE_LOGGER_CASE(1);
                _TUPLE_LOGGER_CASE(2);
                _TUPLE_LOGGER_CASE(3);
                _TUPLE_LOGGER_CASE(4);
                _TUPLE_LOGGER_CASE(5);
                _TUPLE_LOGGER_CASE(6);
            }
        }
    };

    template <>
    struct TupleLogger<8>
    {
        static const int C = 8;

        template <class... Args>
        static void log(std::ostream& out, int idx, std::tuple<Args...>& tp)
        {
            switch (idx)
            {
                _TUPLE_LOGGER_CASE(0);
                _TUPLE_LOGGER_CASE(1);
                _TUPLE_LOGGER_CASE(2);
                _TUPLE_LOGGER_CASE(3);
                _TUPLE_LOGGER_CASE(4);
                _TUPLE_LOGGER_CASE(5);
                _TUPLE_LOGGER_CASE(6);
                _TUPLE_LOGGER_CASE(7);
            }
        }
    };

    template <>
    struct TupleLogger<9>
    {
        static const int C = 9;

        template <class... Args>
        static void log(std::ostream& out, int idx, std::tuple<Args...>& tp)
        {
            switch (idx)
            {
                _TUPLE_LOGGER_CASE(0);
                _TUPLE_LOGGER_CASE(1);
                _TUPLE_LOGGER_CASE(2);
                _TUPLE_LOGGER_CASE(3);
                _TUPLE_LOGGER_CASE(4);
                _TUPLE_LOGGER_CASE(5);
                _TUPLE_LOGGER_CASE(6);
                _TUPLE_LOGGER_CASE(7);
                _TUPLE_LOGGER_CASE(8);
            }
        }
    };

    template <>
    struct TupleLogger<10>
    {
    public:
        static const int C = 10;

        template <class... Args>
        static void log(std::ostream& out, int idx, std::tuple<Args...>& tp)
        {
            switch (idx)
            {
                _TUPLE_LOGGER_CASE(0);
                _TUPLE_LOGGER_CASE(1);
                _TUPLE_LOGGER_CASE(2);
                _TUPLE_LOGGER_CASE(3);
                _TUPLE_LOGGER_CASE(4);
                _TUPLE_LOGGER_CASE(5);
                _TUPLE_LOGGER_CASE(6);
                _TUPLE_LOGGER_CASE(7);
                _TUPLE_LOGGER_CASE(8);
                _TUPLE_LOGGER_CASE(9);
            }
        }
    };

#else

    // 2 args
    _TUPLE_LOGGER_BEGIN(2);
    _TUPLE_LOGGER_CASE(1);
    _TUPLE_LOOGER_END;

    // 3 args
    _TUPLE_LOGGER_BEGIN(3);
    _TUPLE_LOGGER_CASE(1);
    _TUPLE_LOGGER_CASE(2);
    _TUPLE_LOOGER_END;

    // 4 args
    _TUPLE_LOGGER_BEGIN(4);
    _TUPLE_LOGGER_CASE(1);
    _TUPLE_LOGGER_CASE(2);
    _TUPLE_LOGGER_CASE(3);
    _TUPLE_LOOGER_END;

    // 5 args
    _TUPLE_LOGGER_BEGIN(5);
    _TUPLE_LOGGER_CASE(1);
    _TUPLE_LOGGER_CASE(2);
    _TUPLE_LOGGER_CASE(3);
    _TUPLE_LOGGER_CASE(4);
    _TUPLE_LOOGER_END;

    // 6 args
    _TUPLE_LOGGER_BEGIN(6);
    _TUPLE_LOGGER_CASE(1);
    _TUPLE_LOGGER_CASE(2);
    _TUPLE_LOGGER_CASE(3);
    _TUPLE_LOGGER_CASE(4);
    _TUPLE_LOGGER_CASE(5);
    _TUPLE_LOOGER_END;

    // 7 args
    _TUPLE_LOGGER_BEGIN(7);
    _TUPLE_LOGGER_CASE(1);
    _TUPLE_LOGGER_CASE(2);
    _TUPLE_LOGGER_CASE(3);
    _TUPLE_LOGGER_CASE(4);
    _TUPLE_LOGGER_CASE(5);
    _TUPLE_LOGGER_CASE(6);
    _TUPLE_LOOGER_END;

    // 8 args
    _TUPLE_LOGGER_BEGIN(8);
    _TUPLE_LOGGER_CASE(1);
    _TUPLE_LOGGER_CASE(2);
    _TUPLE_LOGGER_CASE(3);
    _TUPLE_LOGGER_CASE(4);
    _TUPLE_LOGGER_CASE(5);
    _TUPLE_LOGGER_CASE(6);
    _TUPLE_LOGGER_CASE(7);
    _TUPLE_LOOGER_END;

    // 9 args
    _TUPLE_LOGGER_BEGIN(9);
    _TUPLE_LOGGER_CASE(1);
    _TUPLE_LOGGER_CASE(2);
    _TUPLE_LOGGER_CASE(3);
    _TUPLE_LOGGER_CASE(4);
    _TUPLE_LOGGER_CASE(5);
    _TUPLE_LOGGER_CASE(6);
    _TUPLE_LOGGER_CASE(7);
    _TUPLE_LOGGER_CASE(8);
    _TUPLE_LOOGER_END;

    // 10 args
    _TUPLE_LOGGER_BEGIN(10);
    _TUPLE_LOGGER_CASE(1);
    _TUPLE_LOGGER_CASE(2);
    _TUPLE_LOGGER_CASE(3);
    _TUPLE_LOGGER_CASE(4);
    _TUPLE_LOGGER_CASE(5);
    _TUPLE_LOGGER_CASE(6);
    _TUPLE_LOGGER_CASE(7);
    _TUPLE_LOGGER_CASE(8);
    _TUPLE_LOGGER_CASE(9);
    _TUPLE_LOOGER_END;

#endif

#undef _TUPLE_LOGGER_BEGIN
#undef _TUPLE_LOGGER_CASE
#undef _TUPLE_LOGGER_END
}

CFG_NAMESPACE_END
