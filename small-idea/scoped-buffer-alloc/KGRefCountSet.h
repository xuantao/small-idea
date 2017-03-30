/*
 * reference count set
 * xuantao, Seasun Game 2017
 */
#pragma once

#include <map>
#include <functional>

/*
 * 使用Map简单包装一个带引用计数的Set
 */
template <class Ty>
class KGRefCountSet
{
public:
    typedef Ty value_type;
    typedef typename std::remove_reference<Ty>::type _Ty;
public:
    int Insert(const _Ty& val)
    {
        auto it = m_Vals.find(val);
        if (it == m_Vals.end())
            it = m_Vals.insert(std::make_pair(val, 0)).first;

        ++ it->second;
        return it->second;
    }

    inline int Insert(const _Ty&& value)
    {
        return Insert(value);
    }

    int Remove(const value_type& value)
    {
        auto it = m_Vals.find(value);
        if (it == m_Vals.end())
            return 0;

        -- it->second;
        if (it->second > 0)
            return it->second;

        m_Vals.erase(it);
        return 0;
    }

    int GetSize() const
    {
        return (int)m_Vals.size();
    }

    void Clear()
    {
        m_Vals.clear();
    }

    int GetRefCount(const value_type& value) const
    {
        auto it = m_Vals.find(value);
        if (it == m_Vals.cend())
            return 0;
        return it->second;
    }

    bool Contain(const value_type& value) const
    {
        return GetRefCount(value) > 0;
    }

    template <class Func>
    bool Traverse(Func&& Visitor) const
    {
        bool bResult = true;
        for (auto it = m_Vals.cbegin(); it != m_Vals.cend();)
        {
            const value_type& v = it->first;
            ++it;

            if (!Visitor(v))
            {
                bResult = false;
                break;
            }
        }
        return bResult;
    }

private:
    typedef std::map<value_type, int> KGVALUE_MAP;

    KGVALUE_MAP m_Vals;
};

