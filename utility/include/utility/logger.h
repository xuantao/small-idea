/*
 * logger
 * xuantao, 2018-04-19
*/
#pragma once
#include "common.h"
#include "to_string.h"
#include <algorithm>

UTILITY_NAMESPACE_BEGIN

#define LOG_MAX_STACK_BUFF_SIZE 4096

namespace detail
{
    struct val_name
    {
        const char* name;
        size_t length;
    };

    template <typename Ty>
    inline void to_str_impl(str_buff& lb, std::pair<val_name, Ty>&& val)
    {
        if (lb.failed || lb.capacity <= 0)
            return;

        if (val.first.length)
        {
            to_str_array(lb, val.first.name, val.first.length);
            to_str_impl(lb, ':');
        }
        to_str_impl(lb, val.second);
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
    void parse_names(const char* names, std::array<val_name, N>& ary)
    {
        const char* tmp = names;
        size_t index = 0;

        for (; index < N;)
        {
            auto& els = ary[index];
            ++index;

            while (*tmp == ' ' || *tmp == '\t')
                ++tmp;  // remove empty char

            tmp = skip_str(tmp);

            els.name = tmp;

            const char* next = strchr(tmp, ',');
            if (next == nullptr)
            {
                els.length = strlen(tmp);
                break;
            }

            els.length = next - tmp;
            tmp = next + 1;
        }

        for (; index < N; ++index)
            ary[index].length = 0;
    }

    template <typename... Args, size_t... Idxs>
    inline void log_with_name(char* buf, size_t size, const char* names, std::tuple<Args...>&& vals, index_sequence<Idxs...>&& is)
    {
        constexpr size_t N = sizeof...(Args);
        std::array<val_name, N> name_array;

        parse_names(names, name_array);

        detail::to_str_sep(buf, size, ", ",
            std::make_tuple(std::make_pair(std::get<Idxs>(name_array), std::get<Idxs>(vals))...),
            std::forward<index_sequence<Idxs...>>(is)
        );
    }

    inline const char* empty_str() { return ""; }
    inline const char* empty_str(const char* str) { return str; }
} // namespace detail

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
    char buff[LOG_MAX_STACK_BUFF_SIZE];
    to_str_sep(buff, LOG_MAX_STACK_BUFF_SIZE, ", ", std::forward<Args>(args)...);
    get_log_proxy()->log(level, buff);
}

template <typename... Args>
inline void log_format(int level, const char* fmt, Args&&... args)
{
    char buff[LOG_MAX_STACK_BUFF_SIZE];
    to_str_format(buff, LOG_MAX_STACK_BUFF_SIZE, fmt, std::forward<Args>(args)...);
    get_log_proxy()->log(level, buff);
}

template <typename... Args>
inline void log_with_name(int level, const char* names, Args&&... args)
{
    char buff[LOG_MAX_STACK_BUFF_SIZE];
    detail::log_with_name(buff, LOG_MAX_STACK_BUFF_SIZE, names,
        std::make_tuple(std::forward<Args>(args)...),
        make_index_sequence_t<sizeof...(Args)>()
    );
    get_log_proxy()->log(level, buff);
}

UTILITY_NAMESPACE_END

#define LOG_INFO(Level, ...)        UTILITY_NAMESPCE log_info(Level, ##__VA_ARGS__)
#define LOG_FORMAT(Level, Fmt, ...) UTILITY_NAMESPCE log_format(Level, Fmt, ##__VA_ARGS__)
#define LOG_VALS(Level, ...)        UTILITY_NAMESPCE log_with_name(Level, UTILITY_NAMESPCE detail::empty_str(#__VA_ARGS__), ##__VA_ARGS__)
