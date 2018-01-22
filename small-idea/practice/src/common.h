/*
 * common type predef
 * xuantao, 2017
*/
#pragma once

#include <type_traits>
#include <cstdint>
#include <cassert>
#include <memory>

#ifndef NAMESPACE_BEGIN
    #define NAMESPACE_BEGIN namespace zh {
#endif // NAMESPACE_BEGIN

#ifndef NAMESPACE_END
    #define NAMESPACE_END }
#endif // NAMESPACE_END

#ifndef USING_NAMESPACE
    #define USING_NAMESPACE using namespace zh
#endif // USING_NAMESPACE

#define SCOPED_ALLOCATOR_BLOCK_SIZE 1024 * 1024
#define SCOPED_ALLOCATOR_ALIGN_SIZE sizeof(void*)
#define SCOPED_ALLOCATOR_BOUNDARY_CHECK 1

#define SCOPED_ALLOCATOR_MULTITHREAD 0
#define LOG_DEBUG_INFO 1

#if LOG_DEBUG_INFO
    #include <iostream>
#endif // LOG_DEBUG_INFO
