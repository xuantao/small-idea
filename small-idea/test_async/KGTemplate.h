// Copyright 2018 Seasun Games, All Rights Reserved.
// Created by Carber on 2018/2/22.

#pragma once

#include <type_traits>
#include <tuple>

/*-----------------------------------------------------------------------------
Standard templates.
-----------------------------------------------------------------------------*/

template<typename ReferencedType>
inline ReferencedType* IfAThenAElseB(ReferencedType* A, ReferencedType* B)
{
    const size_t uIntA = reinterpret_cast<size_t>(A);
    const size_t uIntB = reinterpret_cast<size_t>(B);

    // Compute a mask which has all bits set if IntA is zero, and no bits set if it's non-zero.
    const size_t uMaskB = -(!uIntA);

    return reinterpret_cast<ReferencedType*>(uIntA | (uMaskB & uIntB));
}

// --------------std::make_index_sequence && std::index_sequence (c++14 support)-----------
template<size_t...>
struct KGIndexSequence {}; // std::index_sequence

template <size_t N, size_t... Indices>
struct _KGMakeIndexSequence : _KGMakeIndexSequence<N - 1, N - 1, Indices...> {};

template<size_t... Indices>
struct _KGMakeIndexSequence<0, Indices...>
{
    typedef KGIndexSequence<Indices...> Type;
};

template <size_t N>
using KGMakeIndexSequence = typename _KGMakeIndexSequence<N>::Type; // std::make_index_sequence

/**
* Returns the same type passed to it.  This is useful in a few cases, but mainly for inhibiting template argument deduction in function arguments, e.g.:
*
* template <typename T>
* void Func1(T Val); // Can be called like Func(123) or Func<int>(123);
*
* template <typename T>
* void Func2(typename KIdentity<T>::Type Val); // Must be called like Func<int>(123)
*/
template <typename T>
struct KGIdentity
{
    typedef T Type;
};

/* 单向链表 */
template <typename Ty>
struct KGSinglyNode
{
    typedef KGSinglyNode SinglyNode;

    KGSinglyNode() { }

    template <typename... Args>
    KGSinglyNode(Args&&... args) : Value(std::forward<Args>(args)...)
    { }

    SinglyNode* pNext = nullptr;
    Ty Value;
};

inline constexpr size_t KGAlignPadding(size_t sz, size_t bound = sizeof(void*))
{
    return bound == 0 ? 0 : (bound - sz % bound) % bound;
}

/* 对齐（1，2，4，8...） */
inline constexpr size_t KGAlignSize(size_t sz, size_t bound = sizeof(void*))
{
    return sz + KGAlignPadding(sz, bound);
}

namespace utility
{
    template <typename... Rtys, size_t... Idxs>
    inline std::tuple<Rtys&...> Tie(std::tuple<Rtys...>& tp, KGIndexSequence<Idxs...>)
    {
        return std::tie(std::get<Idxs>(tp)...);
    }

    template <typename... Rtys>
    inline std::tuple<Rtys&...> Tie(std::tuple<Rtys...>& tp)
    {
        return Tie(tp, KGMakeIndexSequence<sizeof...(Rtys)>());
    }

    // c++ 14 standard
    template<class Ty>
    struct is_null_pointer : std::is_same<std::nullptr_t, typename std::remove_cv<Ty>::type>
    {
    };

    template<class Ty>
    struct remove_cvref
    {
        typedef typename std::remove_cv<typename std::remove_reference<Ty>::type>::type type;
    };

    /* 去除所有修饰符, (const char* const -> char) */
    template <typename Ty>
    struct nude_type
    {
        typedef typename std::remove_pointer<
            typename remove_cvref<
                typename std::remove_pointer<Ty>::type
            >::type
        >::type type;
    };

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
}

