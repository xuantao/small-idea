/*
 * utilities functions
 * xuantao, 2017
*/
#pragma once

#include <vector>
#include <map>
#include <set>
#include "scoped_buffer.h"
#include "scoped_obj_buffer.h"
#include "scoped_vector.h"
#include "scoped_set.h"

NAMESPACE_ZH_BEGIN

namespace util
{
    scoped_buffer allocate(size_t size);

    template<class Ty>
    scoped_obj_buffer<Ty> allocate(size_t count)
    {
        return scoped_obj_buffer<Ty>(allocate(sizeof(Ty) * count), count);
    }

    template<class Ty, typename ...Args>
    scoped_obj_buffer<Ty> allocate(size_t count, Args&& ...args)
    {
        return scoped_obj_buffer<Ty>(allocate(sizeof(Ty) * count), count, args...);
    }

    template <class Ty>
    scoped_vector<Ty> vector(std::size_t count)
    {
        return scoped_vector<Ty>(allocate(scoped_vector<Ty>::buffer_size(count)));
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
    scoped_vector<Ty> vector(const std::set<Ty>& src, std::size_t ext = 0)
    {
        scoped_vector<Ty> dst(allocate(scoped_vector<Ty>::buffer_size(src.size() + ext)));
        for (auto it = src.cbegin(); it != src.cend(); ++it)
            dst.push_back(*it);
        return dst;
    }

    template <class Ky, class Ty>
    scoped_vector<Ty> vector(const std::map<Ky, Ty>& src, std::size_t ext = 0)
    {
        scoped_vector<Ty> dst(allocate(scoped_vector<Ty>::buffer_size(src.size() + ext)));
        for (auto it = src.cbegin(); it != src.cend(); ++it)
            dst.push_back(it->second);
        return dst;
    }

    template <class Ty>
    scoped_set<Ty> set(std::size_t size)
    {
        return scoped_set<Ty>(allocate(scoped_set<Ty>::buffer_size(size)));
    }
}

NAMESPACE_ZH_END
