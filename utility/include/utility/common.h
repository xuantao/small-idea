/*
 * common type predef
 * xuantao, 2017
*/
#pragma once

#include <type_traits>
#include <cstdint>
#include <cassert>
#include <memory>

#define UTILITY_NAMESPACE_BEGIN namespace utility {
#define UTILITY_NAMESPACE_END }
#define UTILITY_USING_NAMESPACE using namespace utility;
#define UTILITY_NAMESPCE utility::

#define SCOPED_ALLOCATOR_BLOCK_SIZE 1024 * 1024
#define SCOPED_ALLOCATOR_ALIGN_SIZE sizeof(void*)
#define SCOPED_ALLOCATOR_BOUNDARY_CHECK 1
#define SCOPED_ALLOCATOR_MULTITHREAD 0

#define LOG_DEBUG_INFO 1

#if LOG_DEBUG_INFO
    #include <iostream>
#endif // LOG_DEBUG_INFO

// support c++ 11
namespace std_ext
{
    template<size_t...>
    struct index_sequence {};

    template <size_t N, size_t... Indices>
    struct make_index_sequence : make_index_sequence<N - 1, N - 1, Indices...> {};

    template<size_t... Indices>
    struct make_index_sequence<0, Indices...>
    {
        typedef index_sequence<Indices...> type;
    };

    template <size_t N>
    using make_index_sequence_t = typename make_index_sequence<N>::type;

    template <typename Ty>
    using decay_t = typename std::decay<Ty>::type;

    template <typename Fty, typename... Args>
    struct invoke_result
    {
        typedef typename std::result_of<Fty(Args...)>::type type;
    };

    /* check Fty is callable by specified parementers */
    template <typename Fty, typename... Args>
    struct is_callable
    {
        template<typename U> static auto Check(int) -> decltype(std::declval<U>()(std::declval<Args>()...), std::true_type());
        template<typename U> static std::false_type Check(...);

        static constexpr bool value = std::is_same<decltype(Check<Fty>(0)), std::true_type>::value;
    };

    template <typename Fty>
    struct is_callable<Fty, void>
    {
        template<typename U> static auto Check(int) -> decltype(std::declval<U>()(), std::true_type());
        template<typename U> static std::false_type Check(...);

        static constexpr bool value = std::is_same<decltype(Check<Fty>(0)), std::true_type>::value;
    };
}

UTILITY_NAMESPACE_BEGIN

/* 单向链表 */
template <typename Ty>
struct singly_node
{
    typedef singly_node self_type;
    typedef Ty value_type;

    singly_node() { }

    template <typename... Args>
    singly_node(Args&&... args) : value(std::forward<Args>(args)...)
    { }

    self_type* next = nullptr;
    value_type value;
};

/* 单向链表 */
template <typename Ty>
struct SinglyNode
{
    SinglyNode() { }

    template <typename... Args>
    SinglyNode(Args&&... args) : Value(std::forward<Args>(args)...)
    { }

    SinglyNode* pNext = nullptr;
    Ty Value;
};

inline constexpr size_t align_padding(size_t sz, size_t bound = sizeof(void*))
{
    return bound == 0 ? 0 : (bound - sz % bound) % bound;
}

/* 对齐（1，2，4，8...） */
inline constexpr size_t align_size(size_t sz, size_t bound = sizeof(void*))
{
    return sz + align_padding(sz, bound);
}

inline constexpr size_t KGAlignPadding(size_t sz, size_t bound = sizeof(void*))
{
    return bound == 0 ? 0 : (bound - sz % bound) % bound;
}

/* 对齐（1，2，4，8...） */
inline constexpr size_t KGAlignSize(size_t sz, size_t bound = sizeof(void*))
{
    return sz + KGAlignPadding(sz, bound);
}

UTILITY_NAMESPACE_END
