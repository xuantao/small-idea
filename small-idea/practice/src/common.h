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
#define UTILITY_USING_NAMESPACE using namespace utilty
#define UTILITY_NAMESPCE utilty::

#define SCOPED_ALLOCATOR_BLOCK_SIZE 1024 * 1024
#define SCOPED_ALLOCATOR_ALIGN_SIZE sizeof(void*)
#define SCOPED_ALLOCATOR_BOUNDARY_CHECK 1
#define SCOPED_ALLOCATOR_MULTITHREAD 0

#define LOG_DEBUG_INFO 1

#if LOG_DEBUG_INFO
    #include <iostream>
#endif // LOG_DEBUG_INFO
