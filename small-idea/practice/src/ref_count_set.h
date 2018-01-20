/*
 * reference count set
 * xuantao, 2017
*/
#pragma once

#include "common.h"
#include <map>
#include <functional>

NAMESPACE_ZH_BEGIN

/*
 * 使用Map简单包装一个带引用计数的Set
*/
template <class Ty>
class ref_count_set
{
public:
    typedef typename std::remove_reference<Ty>::type _Ty;

public:
    size_t insert(const _Ty& val)
    {
        auto it = _Vals.find(val);
        if (it == _Vals.end())
            it = _Vals.insert(std::make_pair(val, 0)).first;

        ++it->second;
        return it->second;
    }

    size_t erase(const _Ty& val)
    {
        auto it = _Vals.find(val);
        if (it == _Vals.end())
            return 0;

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

    size_t ref_count(const _Ty& val) const
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

    template <class Func>
    bool traverse(Func&& Visitor) const
    {
        bool bResult = true;
        for (auto it = _Vals.cbegin(); it != _Vals.cend();)
        {
            const _Ty& v = it->first;
            size_t c = it->second;
            ++it;

            if (!Visitor(v, c))
            {
                bResult = false;
                break;
            }
        }
        return bResult;
    }

private:
    std::map<_Ty, size_t> _Vals;
};

NAMESPACE_ZH_END
