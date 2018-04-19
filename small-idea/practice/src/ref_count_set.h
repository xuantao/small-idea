/*
 * reference count set
 * xuantao, 2017
*/
#pragma once

#include "common.h"
#include <map>

UTILITY_NAMESPACE_BEGIN

/*
 * 使用Map简单包装一个带引用计数的Set
*/
template <class Ty>
class ref_count_set
{
public:
    typedef typename std::remove_reference<Ty>::type _Ty;
    typedef std::ptrdiff_t diff_t;

public:
    diff_t insert(const _Ty& val)
    {
        auto it = _Vals.find(val);
        if (it == _Vals.end())
            it = _Vals.insert(std::make_pair(val, 0)).first;

        ++it->second;
        return it->second;
    }

    /*
     * if does not contain the val, will return -1
     * when ref count dec to 0, will remove the val from set
    */
    diff_t erase(const _Ty& val)
    {
        auto it = _Vals.find(val);
        if (it == _Vals.end())
            return -1;

        --it->second;
        if (it->second > 0)
            return it->second;

        _Vals.erase(it);
        return 0;
    }

    void clear()
    {
        _Vals.clear();
    }

    size_t size() const
    {
        return _Vals.size();
    }

    diff_t ref_count(const _Ty& val) const
    {
        auto it = _Vals.find(val);
        if (it == _Vals.cend())
            return 0;
        return it->second;
    }

    bool contain(const _Ty& val) const
    {
        return ref_count(val) > 0;
    }

    /*
     * visit all elements with function declare like void(const Ty& key, diff_t count)
    */
    template <class Func>
    bool traverse(Func&& visitor) const
    {
        for (const auto& pair : _Vals)
            if (!visitor(pair.first, pair.second))
                return false;

        return true;
    }

private:
    std::map<_Ty, diff_t> _Vals;
};

UTILITY_NAMESPACE_END
