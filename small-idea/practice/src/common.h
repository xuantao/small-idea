/*
 * common type predef
 * xuantao, 2017
*/
#pragma once

#include <type_traits>
#include <cstdint>
#include <cassert>
#include <memory>

#define UTILITY_NAMESPACE_BEGIN namespace utilty {
#define UTILITY_NAMESPACE_END }
#define UTILITY_USING_NAMESPACE using namespace utilty;
#define UTILITY_NAMESPCE utilty::

#define SCOPED_ALLOCATOR_BLOCK_SIZE 1024 * 1024
#define SCOPED_ALLOCATOR_ALIGN_SIZE sizeof(void*)
#define SCOPED_ALLOCATOR_BOUNDARY_CHECK 1
#define SCOPED_ALLOCATOR_MULTITHREAD 0

#define LOG_DEBUG_INFO 1

#if LOG_DEBUG_INFO
    #include <iostream>
#endif // LOG_DEBUG_INFO

UTILITY_NAMESPACE_BEGIN

// support c++ 11

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

UTILITY_NAMESPACE_END
