/*
 * to_strings
 * xuantao, 2018-04-19
 * 不能将to_string及其相关重载放置在命名空间中
 * 模板函数引用时，友元声明名字查找可能有问题
*/
#pragma once

#include "common.h"
#include <string.h>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <regex>
#include <algorithm>

inline int to_string(char* buff, size_t size, const char* val)
{
    return snprintf(buff, size, (const char*)(val ? val : "nullptr"));
}

inline int to_string(char* buff, size_t size, const std::string& val)
{
    return snprintf(buff, size, val.c_str());
}

inline int to_string(char* buff, size_t size, bool val)
{
    return snprintf(buff, size, (const char*)(val ? "true" : "false"));
}

inline int to_string(char* buff, size_t size, char val)
{
    return snprintf(buff, size, "%c", val);
}

inline int to_string(char* buff, size_t size, unsigned char val)
{
    return snprintf(buff, size, "%d", val);
}

inline int to_string(char* buff, size_t size, short val)
{
    return snprintf(buff, size, "%d", val);
}

inline int to_string(char* buff, size_t size, unsigned short val)
{
    return snprintf(buff, size, "%d", val);
}

inline int to_string(char* buff, size_t size, int val)
{
    return snprintf(buff, size, "%d", val);
}

inline int to_string(char* buff, size_t size, unsigned int val)
{
    return snprintf(buff, size, "%d", val);
}

inline int to_string(char* buff, size_t size, long val)
{
    return snprintf(buff, size, "%ld", val);
}

inline int to_string(char* buff, size_t size, unsigned long val)
{
    return snprintf(buff, size, "%ld", val);
}

inline int to_string(char* buff, size_t size, long long val)
{
    return snprintf(buff, size, "%lld", val);
}

inline int to_string(char* buff, size_t size, unsigned long long val)
{
    return snprintf(buff, size, "%lld", val);
}

inline int to_string(char* buff, size_t size, float val)
{
    return snprintf(buff, size, "%f", val);
}

inline int to_string(char* buff, size_t size, double val)
{
    return snprintf(buff, size, "%f", val);
}

inline int to_string(char* buff, size_t size, const void* val)
{
    return snprintf(buff, size, "0x%p", val);
}

inline int to_string(char* buff, size_t size, std::nullptr_t)
{
    return snprintf(buff, size, "nullptr");
}

inline int _to_string_array(char* buff, size_t size, const char* src, size_t len)
{
    if (size == 0) return 0;

    size_t index = 0;
    size_t capacity = std::min(len, size - 1);
    while (src[index] && index < capacity)
    {
        buff[index] = src[index];
        ++index;
    }

    buff[index] = 0;
    return (int)index;
}

template <size_t N>
inline int to_string(char* buff, size_t size, const char(&val)[N])
{
    static_assert(N > 0, "const char array size must be greater than 0");
    return _to_string_array(buff, size, val, N);
}

template <size_t N>
inline int to_string(char* buff, size_t size, char(&val)[N])
{
    static_assert(N > 0, "char array size must be greater than 0");
    return _to_string_array(buff, size, val, N);
}

UTILITY_NAMESPACE_BEGIN

template <typename Ty> int to_str_t(char* buff, size_t size, Ty&& val);

namespace detail
{
    struct tostring_type {};
    struct enum_type {};
    struct pointer_type {};
    struct normal_type {};

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
            tostring_type,  // 重载了to_string
            typename std::conditional<
                std::is_enum<src_type>::value,
                enum_type,  // 枚举
                typename std::conditional<
                    std::is_pointer<src_type>::value,
                    pointer_type,   // 指针类型
                    normal_type     // 普通类型
                >::type
            >::type
        >::type type;
    };

    struct str_buff
    {
        bool failed;
        char* buff;
        int capacity;
    };

    inline void to_str_array(str_buff& sb, const char* src, size_t n)
    {
        int w = _to_string_array(sb.buff, sb.capacity, src, n);
        if (w < 0)
        {
            sb.failed = true;
            return;
        }

        w = std::min(sb.capacity, w);
        sb.buff += w;
        sb.capacity -= w;
    }

    template <typename Ty>
    inline int to_str_pointer(char* buff, size_t size, Ty* val, std::true_type)
    {
        return to_string(buff, size, *val);
    }

    template <typename Ty>
    inline int to_str_pointer(char* buff, size_t size, Ty* val, std::false_type)
    {
        return to_str_t(buff, size, *val);  // 这里要慎重, 避免陷入死循环
    }

    template <typename Ty>
    inline int to_str_dispatch(char* buff, size_t size, Ty&& val, tostring_type)
    {
        return to_string(buff, size, std::forward<Ty>(val));
    }

    template <typename Ty>
    inline int to_str_dispatch(char* buff, size_t size, Ty&& val, enum_type)
    {
        return to_string(buff, size, static_cast<int>(val));
    }

    template <typename Ty>
    inline int to_str_dispatch(char* buff, size_t size, Ty* val, pointer_type)
    {
        if (val == nullptr)
            return to_string(buff, size, nullptr);

        return to_str_pointer(buff, size, val,
            typename std::conditional<has_to_string<Ty>::value, std::true_type, std::false_type>::type()
        );
    }

    template <typename Ty>
    inline int to_str_dispatch(char* buff, size_t size, Ty&& val, normal_type)
    {
        return to_string(buff, size, static_cast<const void*>(&val));
    }

    template <typename Ty>
    inline int to_str_dispatch(char* buff, size_t size, Ty&& val)
    {
        return to_str_dispatch(buff, size, std::forward<Ty>(val), typename type_detect<Ty>::type());
    }

    template <typename Ty>
    inline void to_str_impl(str_buff& sb, Ty&& val)
    {
        if (sb.failed || sb.capacity <= 0)
            return;

        int w = to_str_t(sb.buff, sb.capacity, std::forward<Ty>(val));
        sb.failed = w < 0;
        if (sb.failed)
            return;

        sb.buff += w;
        sb.capacity -= w;
    }

    template <typename Ty>
    inline void to_str_sep(str_buff& sb, Ty&& val, const char* sep, bool is_last)
    {
        to_str_impl(sb, std::forward<Ty>(val));
        if (!is_last)
            to_str_impl(sb, sep);
    }

    template <typename... Args, size_t... Idxs>
    inline int to_str_sep(char* buff, size_t size, const char* sep, std::tuple<Args...>&& vals, index_sequence<Idxs...>&&)
    {
        using its = int[];
        constexpr size_t N = sizeof...(Args) - 1;

        if (size)
            buff[0] = 0;

        str_buff sb{false, buff, (int)size};
        its{0,
            (to_str_sep(sb, std::forward<Args>(std::get<Idxs>(vals)), sep, Idxs == N), 0)...
        };

        if (sb.failed) return -1;
        else if (sb.capacity <= 0) return (int)size;
        else return (int)size - sb.capacity;
    }

    template <typename... Args, size_t... Idxs>
    inline int to_str_mul(char* buff, size_t size, std::tuple<Args...>&& vals, index_sequence<Idxs...>&&)
    {
        using its = int[];
        if (size)
            buff[0] = 0;

        str_buff sb{false, buff , (int)size};
        its{0,
            (to_str_impl(sb, std::forward<Args>(std::get<Idxs>(vals))), 0)...
        };

        if (sb.failed) return -1;
        else if (sb.capacity <= 0) return (int)size;
        else return (int)size - sb.capacity;
    }

    template <typename Ty>
    char* to_str_item(str_buff& sb, Ty&& val)
    {
        char* str = sb.buff;
        if (sb.capacity == 0)
            return str;

        to_str_impl(sb, std::forward<Ty>(val));

        if (sb.capacity)
        {
            sb.capacity -= 1;
            sb.buff += 1;
        }
        return str;
    }
} // namespace detail

template <typename... Args>
inline int to_str_sep(char* buff, size_t size, const char* sep, Args&&... args)
{
    return detail::to_str_sep(buff, size, sep,
        std::make_tuple(std::forward<Args>(args)...),
        make_index_sequence_t<sizeof...(Args)>()
    );
}

template <typename... Args>
inline int to_str_mul(char* buff, size_t size, Args&&... args)
{
    return detail::to_str_mul(buff, size,
        std::make_tuple(std::forward<Args>(args)...),
        make_index_sequence_t<sizeof...(Args)>()
    );
}

template <typename Iter>
int to_str_iter(char* buff, size_t size, Iter beg, Iter end)
{
    detail::str_buff sb{false, buff, (int)size};

    detail::to_str_impl(sb, '{');
    for (; beg != end; ++beg)
    {
        detail::to_str_impl(sb, *beg);
        if (beg + 1 != end)
            detail::to_str_impl(sb, ", ");
    }
    detail::to_str_impl(sb, '}');

    if (sb.failed) return -1;
    else if (sb.capacity <= 0) return (int)size;
    else return (int)size - sb.capacity;
}

template <typename Ty>
inline int to_str_t(char* buff, size_t size, Ty&& val)
{
    return detail::to_str_dispatch(buff, size, std::forward<Ty>(val));
}

template <typename Ky, typename Vy>
inline int to_str_t(char* buff, size_t size, const std::pair<Ky, Vy>& pair)
{
    return to_str_sep(buff, size, ":", '{', pair.first, pair.second, '}');
}

template <typename Ty, size_t N>
inline int to_str_t(char* buff, size_t size, const std::array<Ty, N>& val)
{
    return to_str_iter(buff, size, val.cbegin(), val.cend());
}

template <size_t N>
inline int to_str_t(char* buff, size_t size, const char(&val)[N])
{
    static_assert(N > 0, "const char array size must be greater than 0");
    return _to_string_array(buff, size, val, N);
}

template <size_t N>
inline int to_str_t(char* buff, size_t size, char(&val)[N])
{
    static_assert(N > 0, "char array size must be greater than 0");
    return _to_string_array(buff, size, val, N);
}

template <typename Ty, size_t N>
inline int to_str_t(char* buff, size_t size, Ty(&val)[N])
{
    return to_str_iter(buff, size, &val[0], &val[N]);
}

template <typename Ty>
inline int to_str_t(char* buff, size_t size, const std::vector<Ty>& val)
{
    return to_str_iter(buff, size, val.cbegin(), val.cend());
}

template <typename Ty>
inline int to_str_t(char* buff, size_t size, const std::list<Ty>& val)
{
    return to_str_iter(buff, size, val.cbegin(), val.cend());
}

template <typename Ky, typename Ty>
inline int to_str_t(char* buff, size_t size, const std::map<Ky, Ty>& val)
{
    return to_str_iter(buff, size, val.cbegin(), val.cend());
}

template <typename R, typename... Args>
inline int to_str_t(char* buff, size_t size, R(*func)(Args...))
{
    return snprintf(buff, size, "function_ptr:0x%p", func);
}

template <typename R, typename C, typename... Args>
inline int to_str_t(char* buff, size_t size, R(C::*func)(Args...))
{
    return snprintf(buff, size, "member_function_ptr:0x%p", func);
}

template <typename R, typename C, typename... Args>
inline int to_str_t(char* buff, size_t size, R(C::*func)(Args...) const)
{
    return snprintf(buff, size, "const member_function_ptr:0x%p", func);
}

template <typename R, typename C>
inline int to_str_t(char* buff, size_t size, R C::*mem)
{
    return snprintf(buff, size, "member_data_offset:0x%p", mem);
}

template <typename... Args>
inline int to_str_format(char* buff, size_t size, const char* fmt, Args&&... args)
{
    using its = int[];
    constexpr size_t N = (sizeof...(Args));

    // convert args to string
    size_t index = 0;
    char param_buff[4096];
    detail::str_buff sb_param{false, param_buff, 4096};
    std::array<const char*, N> params;

    its{0,
        (params[index++] = detail::to_str_item(sb_param, std::forward<Args>(args)), 0)...
    };

    std::regex reg("\\{\\d+\\}");
    std::cmatch cm;
    std::ptrdiff_t pos = 0;
    detail::str_buff sb{false, buff, (int)size};

    // scan param index
    while (std::regex_search(&fmt[pos], cm, reg))
    {
        detail::to_str_array(sb, fmt + pos, cm.position());
        pos += cm.position();

        size_t idx = atoi(fmt + pos + 1);

        if (idx >= N)
        {
            detail::to_str_impl(sb, "(out range at pos:");
            detail::to_str_impl(sb, pos);
            detail::to_str_impl(sb, ", ");
            detail::to_str_array(sb, fmt + pos, cm.length());
            detail::to_str_impl(sb, ')');
        }
        else
        {
            detail::to_str_impl(sb, params[idx]);
        }

        pos += cm.length();
    }

    if (sb.failed)
        return -1;
    else if (sb.capacity < 0)
        return (int)size;
    else
        return (int)size - sb.capacity;
}

UTILITY_NAMESPACE_END
