/*
 * utilities functions
 * xuantao, 2017
*/
#pragma once

#include <vector>
#include <set>
#include <map>
#include "scoped_buffer.h"
#include "scoped_obj_buffer.h"
#include "scoped_vector.h"
#include "scoped_set.h"
#include "scoped_map.h"

NAMESPACE_ZH_BEGIN

namespace util
{
    scoped_buffer allocate(size_t size);

    template<class Ty, typename ...Args>
    scoped_obj_buffer<Ty> allocate(size_t size, Args&& ...args)
    {
        return scoped_obj_buffer<Ty>(allocate(sizeof(Ty) * size), size, std::forward<Args>(args)...);
    }

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

NAMESPACE_ZH_END
