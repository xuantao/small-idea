/*
 * logger
 * xuantao, 2018-04-19
*/
#pragma once
#include "common.h"
#include "to_string.h"
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>

UTILITY_NAMESPACE_BEGIN
/*
namespace logger
{
    template <typename Ty> int log(char* buff, size_t size, Ty&& val);

    namespace detail
    {
        struct tostring_type {};
        struct enum_type {};
        struct pointer_type {};
        struct normal_type {};

        template <typename Ty>
        inline int log_impl(char* buff, size_t size, Ty&& val);

        template <typename Ty>
        struct has_to_string
        {
            template <typename U> static U get_val();
            template <typename U> static auto check(int) -> decltype(to_string((char*)0, 0, get_val<U>()), std::true_type());
            template <typename U> static std::false_type check(...);

            static constexpr bool value = decltype(check<Ty>(0))::value;
        };

        template <typename Ty>
        struct type_detect {
            typedef typename std::remove_reference<Ty>::type src_type;

            typedef typename std::conditional<
                has_to_string<src_type>::value,
                tostring_type,
                typename std::conditional<
                    std::is_enum<src_type>::value,
                    enum_type,
                    typename std::conditional<
                        std::is_pointer<src_type>::value,
                        pointer_type,
                        normal_type
                    >::type
                >::type
            >::type type;
        };

        struct log_buff
        {
            char* buff;
            bool failed;
            int capacity;
        };

        struct val_name
        {
            const char* name;
            size_t length;
        };

        inline void log_str_array(log_buff lb, const char* src, size_t n)
        {
            int s = (int)(n - (src[n - 1] ? 0 : 1));    // 数组有可能最后一个元素不是结束符
            s = std::min(s, lb.capacity);
            //strncpy(lb.buff, src, s);
            lb.buff += s;
            lb.capacity -= s;
        }

        inline int log_str_array(char* buff, size_t size, const char* src, size_t n)
        {
            size_t s = n - (src[n - 1] ? 0 : 1);    // 数组有可能最后一个元素不是结束符
            s = std::min(s, size);
            //strncpy(buff, src, s);
            return (int)s;
        }

        template <typename Ty>
        inline int log_dispatch_pointer(char* buff, size_t size, Ty* val, std::true_type)
        {
            return to_string(buff, size, *val);
        }

        template <typename Ty>
        inline int log_dispatch_pointer(char* buff, size_t size, Ty* val, std::false_type)
        {
            return log(buff, size, *val);   // 这里要慎重, 避免陷入死循环
        }

        template <typename Ty>
        inline int log_dispatch(char* buff, size_t size, Ty&& val, tostring_type)
        {
            return to_string(buff, size, std::forward<Ty>(val));
        }

        template <typename Ty>
        inline int log_dispatch(char* buff, size_t size, Ty&& val, enum_type)
        {
            return to_string(buff, size, static_cast<int>(val));
        }

        template <typename Ty>
        int log_dispatch(char* buff, size_t size, Ty* val, pointer_type)
        {
            if (val == nullptr)
                return to_string(buff, size, nullptr);
            else
                return log_dispatch_pointer(buff, size, val,
                    typename std::conditional<has_to_string<Ty>::value, std::true_type, std::false_type>::type()
                );
        }

        template <typename Ty>
        int log_dispatch(char* buff, size_t size, Ty&& val, normal_type)
        {
            return to_string(buff, size, static_cast<const void*>(&val));
        }

        template <typename Ty>
        inline int log_impl(char* buff, size_t size, Ty&& val)
        {
            return log_dispatch(buff, size, std::forward<Ty>(val), typename type_detect<Ty>::type());
        }

        template <typename Ty>
        inline void log_impl(log_buff& lb, Ty&& val)
        {
            if (lb.failed || lb.capacity <= 0)
                return;

            int w = log(lb, lb.capacity, std::forward<Ty>(val));
            lb.failed = w < 0;
            if (lb.failed)
                return;

            lb.buff += w;
            lb.capacity -= w;
        }

        template <typename Ty>
        inline void log_impl(log_buff& lb, Ty&& val, std::pair<const val_name&, Ty>&& val)
        {
            if (lb.failed || lb.capacity <= 0)
                return;

            if (length)
            {
                log_str_array(lb, val.first.name, val.first.length);
                log_impl(lb, ':');
            }
            log_impl(lb, val.second);
        }

        template <typename Ty>
        inline void log_sep(log_buff& lb, Ty&& val, const char* sep, bool is_last)
        {
            log_impl(lb, std::forward<Ty>(val));
            if (!is_last && sep && *sep)
                log_impl(lb, sep);
        }

        template <typename... Args, size_t... Idxs>
        inline int log_sep(char* buff, size_t size, const char* sep, std::tuple<Args...>& vals, index_sequence<Idxs...>)
        {
            using its = int[];
            log_buff lb{buff, false, (int)size};

            its{0,
                (log_sep(lb, std::forward<Args>(std::get<Idxs>(vals)), sep, Idxs + 1 == N), 0)...
            };

            if (lb.failed) return -1;
            else if (lb.capacity <= 0) return size;
            else return size - lb.capacity;
        }

        template <typename...Args, size_t... Idxs>
        inline int log_mul(char* buff, size_t size, std::tuple<Args...>& vals, index_sequence<Idxs...>)
        {
            using its = int[];
            log_buff lb{buff, false, (int)size};

            its{0,
                (log_impl(lb, std::forward<Args>(std::get<Idxs>(vals))), 0)...
            };

            if (lb.failed) return -1;
            else if (lb.capacity <= 0) return size;
            else return size - lb.capacity;
        }

        inline const char* skip_str(const char* str)
        {
            if (*str != '"')
                return str;

            do {
                ++str;
                if (*str == '"' && str[-1] != '\\')
                {
                    ++str;
                    break;
                }
            } while (*str);

            return str;
        }

        template <size_t N>
        void _parse_names(const char* names, std::array<val_name, N>& ary)
        {
            const char* tmp = names;
            size_t index = 0;

            for (; index < N; ++index)
            {
                auto& els = ary[index];

                tmp = skip_str(tmp);
                while (*tmp == ' ' || *tmp == '\t')
                    ++tmp;  // remove empty char

                els.name = tmp;

                const char* next = strchr(tmp, ',');
                if (next)
                {
                    els.length = strlen(tmp);
                    break;
                }

                els.length = next - tmp;
                tmp = next + 1;
            }

            for (; index < N; ++i)
                ary[index].length = 0;
        }

        template <typename... Args, size_t... Idxs>
        inline void log_with_name(char* buf, size_t size, const char* names, std::tuple<Args...>& vals, index_sequence<Idxs...> is)
        {
            constexpr size_t N = sizeof...(Args);
            std::array<val_name, N> name_array;

            _parse_names(names, name_array);

            return detail::log_sep(buff, size, ", ",
                std::make_tuple(std::make_pair(std::get<Idxs>(name_array), std::get<Idxs>(vals))...),
                is
            );
        }
    } // namespace detail

    template <typename... Args>
    inline int log_sep(char* buff, size_t size, const char* sep, Args&&... args)
    {
        return detail::log_sep(buff, size, sep,
            std::make_tuple(std::forward<Args>(args)...),
            make_index_sequence_t<sizeof...(Args)>()
        );
    }

    template <typename... Args>
    inline int log_mul(char* buff, size_t size, Args&&... args)
    {
        return detail::log_mul(buff, size,
            std::make_tuple(std::forward<Args>(args)...),
            make_index_sequence_t<sizeof...(Args)>()
        );
    }

    template <typename Iter>
    int log_iter(char* buff, size_t size, Iter beg, Iter end)
    {
        detail::log_buff lb{buff, false, (int)size};

        detail::log_impl(lb, '{');
        for (; beg != end; ++beg)
        {
            detail::log_impl(lb, *beg);
            if (beg + 1 != end)
                detail::log_impl(lb, ", ");
        }
        detail::log_impl(lb, '}');

        if (lb.failed) return -1;
        else if (lb.capacity <= 0) return size;
        else return size - lb.capacity;
    }

    template <typename Ty>
    inline int log(char* buff, size_t size, Ty&& val)
    {
        return detail::log_impl(buff, size, std::forward<Ty>(val));
    }

    template <typename Ky, typename Vy>
    inline int log(char* buff, size_t size, const std::pair<Ky, Vy>& pair)
    {
        return log_sep(buff, size, ":", '{', pair.first, pair.second, '}');
    }

    template <typename Ty, size_t N>
    inline int log(char* buff, size_t size, const std::array<Ty, N>& val)
    {
        return log_iter(buff, size, val.cbegin(), val.cend());
    }

    template <size_t N>
    inline int log(char* buff, size_t size, const char (&val)[N])
    {
        static_assert(N > 0, "char array size must be greater than 0");
        return detail::log_str_array(buff, size, val, N);
    }

    template <size_t N>
    inline int log(char* buff, size_t size, char (&val)[N])
    {
        static_assert(N > 0, "char array size must be greater than 0");
        return detail::log_str_array(buff, size, val, N);
    }

    template <typename Ty, size_t N>
    inline int log(char* buff, size_t size, Ty (&val)[N])
    {
        return log_iter(buff, size, &val[0], &val[N]);
    }

    template <typename Ty>
    inline int log(char* buff, size_t size, const std::vector<Ty>& val)
    {
        return log_iter(buff, size, val.cbegin(), val.cend());
    }

    template <typename Ty>
    inline int log(char* buff, size_t size, const std::list<Ty>& val)
    {
        return log_iter(buff, size, val.cbegin(), val.cend());
    }

    template <typename Ky, typename Ty>
    inline int log(char* buff, size_t size, const std::map<Ky, Ty>& val)
    {
        return log_iter(buff, size, val.cbegin(), val.cend());
    }

    template <typename R, typename... Args>
    inline int log(char* buff, size_t size, R(*func)(Args...))
    {
        return to_string(buff, size, static_cast<void*>(func));
    }

    template <typename R, typename C, typename... Args>
    inline int log(char* buff, size_t size, R(C::*func)(Args...))
    {
        return to_string(buff, size, static_cast<void*>(func));
    }

    template <typename R, typename C, typename... Args>
    inline int log(char* buff, size_t size, R(C::*func)(Args...) const)
    {
        return to_string(buff, size, static_cast<void*>(func));
    }

    template <typename R, typename C>
    inline int log(char* buff, size_t size, R C::*mem)
    {
        return to_string(buff, size, static_cast<void*>(mem));
    }
} // namespace logger

struct ilog_proxy
{
    virtual ~ilog_proxy() {}
    virtual void log(int level, const char* msg) = 0;
};

void set_log_proxy(ilog_proxy* proxy);
ilog_proxy* get_log_proxy();

template <typename... Args>
inline void log_info(int level, Args&&... args)
{
    char buff[1024];
    logger::log_sep(buff, 1024, ", ", std::forward<Args>(args)...);
    get_log_proxy()->log(level, buff);
}

template <typename... Args>
inline void log_fmt(int level, const char* fmt, Args&&... args)
{

}

template <typename... Args>
inline void log_with_name(int level, const char* names, Args&&... args)
{
    char buff[1024];
    logger::detail::log_with_name(buff, 1024, names,
        std::make_tuple(std::forward<Args>(args)...),
        make_index_sequence_t<sizeof...(Args)>()
    );
    get_log_proxy()->log(level, buff);
}
*/
UTILITY_NAMESPACE_END
