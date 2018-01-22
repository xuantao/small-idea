﻿/*
 * scoped utilities functions
 * xuantao, 2017
*/
#pragma once

#include "scoped_buffer.h"
#include "scoped_vector.h"
#include "scoped_set.h"
#include "scoped_map.h"
#include <vector>
#include <set>
#include <map>

NAMESPACE_BEGIN

namespace scoped
{
    /*
     * 分配一段域范围内有效缓存
     * 缓存要求分配与释放必须严格保证顺序关系
    */
    scoped_buffer allocate(size_t size);

    template <class Ty>
    scoped_vector<Ty> vector(std::size_t size)
    {
        return scoped_vector<Ty>(allocate(scoped_vector<Ty>::buffer_size(size)));
    }

    template <class Ty>
    scoped_vector<Ty> vector(const std::vector<Ty>& src, std::size_t ext = 0)
    {
        scoped_vector<Ty> dst(allocate(scoped_vector<Ty>::buffer_size(src.size() + ext)));
        for (auto it = src.cbegin(); it != src.cend(); ++it)
            dst.push_back(*it);
        return dst;
    }

    template <class Ty>
    scoped_set<Ty> set(std::size_t size)
    {
        return scoped_set<Ty>(allocate(scoped_set<Ty>::buffer_size(size)));
    }

    template <class Ty>
    scoped_set<Ty> set(const std::set<Ty>& src, std::size_t ext = 0)
    {
        scoped_set<Ty> dst(allocate(scoped_vector<Ty>::buffer_size(src.size() + ext)));
        for (auto it = src.cbegin(); it != src.cend(); ++it)
            dst.insert(*it);
        return dst;
    }

    template <class Kty, class Ty>
    scoped_map<Kty, Ty> map(std::size_t size)
    {
        return scoped_map<Kty, Ty>(allocate(scoped_map<Kty, Ty>::buffer_size(size)));
    }

    template <class Kty, class Ty>
    scoped_map<Kty, Ty> map(const std::map<Kty, Ty>& src, std::size_t ext = 0)
    {
        scoped_map<Kty, Ty> dst(allocate(scoped_map<Kty, Ty>::buffer_size(src.size() + ext)));
        for (auto it = src.cbegin(); it != src.cend(); ++it)
            dst.insert(std::make_pair(it->first, it->second));
        return dst;
    }
}

NAMESPACE_END
