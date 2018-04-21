/*
 * logger
 * xuantao, 2018-04-19
*/
#pragma once
#include "common.h"

UTILITY_NAMESPACE_BEGIN

namespace logger
{
    template <typename... Args> int log(char* buff, size_t size, Args&&... args);

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
            template <typename U>static U get_val();
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
            size_t write_size;
            size_t capacity;
        };

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
        void log_mul_impl(char*& buff, int& capacity, int& write_size, bool is_last, Ty&& val)
        {
            if (write_size < 0)
                return;

            int w = log_impl(buff, capacity, std::forward<Ty>(val));
            write_size += w;
            capacity -= w;

            if (w < 0)
                capacity = w;
            else if (w > capacity)
                capacity = 0;
            else
                buff += w;

            if (!is_last)
            {
                if (capacity >= 1)
                {
                    buff[0] = ',';
                    buff += 1;
                    capacity -= 1;
                    write_size += 1;
                }

                if (capacity >= 1)
                {
                    buff[0] = '';
                    buff += 1;
                    write_size += 1;
                    capacity -= 1;
                }
            }
        }

        template <typename...Args, size_t... Idxs>
        int log_mul_impl(char* buff, size_t size, std::tuple<Args..>& vals, index_sequence<idxs...>)
        {
            using its = int[];
            int capacity = (int)size;
            int write_size = 0;
            constexpr N = sizeof...(Args);

            if (capacity >= 1)
            {
                buff[0] = '{';
                buff += 1;
                capacity -= 1;
                write_size += 1;
            }

            its{ 0,
                (log_mul_impl(buff, capacity, write_size, Idxs + 1 == N, std::forward<Args>(std::get<Idxs>(vals))), 0)...
            };

            if (write_size < 0)
                return write_size;

            if (write_size < (int)size)
            {
                buff[0] = '}';
                buff += 1;
                write_size += 1;
            }

            return write_size;
        }
    } // namespace detail

    template <typename... Args>
    inline int log(char* buff, size_t size, Args&&... args)
    {
        return detail::log_mul_impl(
            buff,
            size,
            std::make_tuple(std::forward<Args>(args)...),
            make_index_sequence_t<sizeof...(Args)>()
        );
    }

    template <typename Iter>
    int log(char* buff, size_t size, Iter beg, Iter end)
    {
        int ws = 0;
        int capacity = (int)size;

        if (!capacity)
            return ws;          // full

        *buff = '{';
        buff += 1;
        ws += 1;
        capacity -= 1;

        if (!capacity)
            return ws;          // full

        for (; beg != end; ++beg)
            detail::log_mul_impl(buff, capacity, ws, (beg + 1 == end), *beg);

        if (ws >= 0 && ws < (int)size)
        {
            *buff = '}';
            ws += 1;
        }

        return ws;
    }

    template <typename Ky, typename Vy>
    inline int log(char* buff, size_t size, const std::pair<Ky, Vy>& pair)
    {
        return log(buff, size, pair.first, pair.second);
    }

    template <typename Ty, size_t N>
    inline int log(char* buff, size_t size, const std::array<Ty, N>& val)
    {
        return log(buff, size, val.cbegin(), val.cend());
    }

    template <typename Ty, size_t N>
    inline int log(char* buff, size_t size, Ty[N] val)
    {
        return log(buff, size, &val[0], &val[N]);
    }

    template <typename Ty>
    inline int log(char* buff, size_t size, const std::vector<Ty>& val)
    {
        return log(buff, size, val.cbegin(), val.cend());
    }

    template <typename Ty>
    inline int log(char* buff, size_t size, const std::list<Ty>& val)
    {
        return log(buff, size, val.cbegin(), val.cend());
    }

    template <typename Ky, typename Ty>
    inline int log(char* buff, size_t size, const std::map<Ky, Ty>& val)
    {
        return log(buff, size, val.cbegin(), val.cend());
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
    logger::log(buff, 1024, std::forward<Args>(args)...);
}

inline void log_info_fmt(int level, const char* fmt, ...)
{

}

UTILITY_NAMESPACE_END
