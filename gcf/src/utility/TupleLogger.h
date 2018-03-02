#pragma once
#include "../gcf/gcf.h"
#include <tuple>

GCF_NAMESPACE_BEGIN

namespace detail
{
    /*
     * 输出tuple中的某个元素
    */
    template <size_t N>
    struct TupleLogger
    {
        enum { POS = N - 1 };

        static void log(std::ostream& out, int idx, std::tuple<>& tp)
        {
            // empty param
        }

        template <class... Args>
        static void log(std::ostream& out, int idx, std::tuple<Args...>& tp)
        {
            if (idx == POS)
                out << std::get<POS>(tp);
            else
                TupleLogger<POS>::log(out, idx, tp);
        }
    };

    template <>
    struct TupleLogger<0>
    {
        enum { POS = 0 };

        static void log(std::ostream& out, int idx, std::tuple<>& tp)
        {
            // empty param
        }

        template <class... Args>
        static void log(std::ostream& out, int idx, std::tuple<Args...>& tp)
        {
            assert(idx == POS);
            out << std::get<POS>(tp);
        }
    };
}

GCF_NAMESPACE_END
