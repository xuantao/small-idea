/*
 * scoped utilities functions
 * xuantao, 2017
*/
#pragma once

#include "scope_buffer.h"
#include "scope_vector.h"
#include "scope_set.h"
#include "scope_map.h"
#include <vector>
#include <set>
#include <map>

UTILITY_NAMESPACE_BEGIN

namespace scoped
{
    /*
     * 分配一段域范围内有效缓存
     * 缓存要求分配与释放必须严格保证顺序关系
    */
    scoped_buffer allocate(size_t size);

    template <class Ty>
    scope_vector<Ty> vector(std::size_t size)
    {
        return scope_vector<Ty>(allocate(scope_vector<Ty>::buffer_size(size)));
    }

    template <class Ty>
    scope_vector<Ty> vector(const std::vector<Ty>& src, std::size_t ext = 0)
    {
        scope_vector<Ty> dst(allocate(scope_vector<Ty>::buffer_size(src.size() + ext)));
        for (auto it = src.cbegin(); it != src.cend(); ++it)
            dst.push_back(*it);
        return dst;
    }

    template <class Ty>
    scope_set<Ty> set(std::size_t size)
    {
        return scope_set<Ty>(allocate(scope_set<Ty>::buffer_size(size)));
    }

    template <class Ty>
    scope_set<Ty> set(const std::set<Ty>& src, std::size_t ext = 0)
    {
        scope_set<Ty> dst(allocate(scope_vector<Ty>::buffer_size(src.size() + ext)));
        for (auto it = src.cbegin(); it != src.cend(); ++it)
            dst.insert(*it);
        return dst;
    }

    template <class Kty, class Ty>
    scope_map<Kty, Ty> map(std::size_t size)
    {
        return scope_map<Kty, Ty>(allocate(scope_map<Kty, Ty>::buffer_size(size)));
    }

    template <class Kty, class Ty>
    scope_map<Kty, Ty> map(const std::map<Kty, Ty>& src, std::size_t ext = 0)
    {
        scope_map<Kty, Ty> dst(allocate(scope_map<Kty, Ty>::buffer_size(src.size() + ext)));
        for (auto it = src.cbegin(); it != src.cend(); ++it)
            dst.insert(std::make_pair(it->first, it->second));
        return dst;
    }
}

UTILITY_NAMESPACE_END
