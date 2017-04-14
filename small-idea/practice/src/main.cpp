#include <stdio.h>
#include <map>
#include <set>
#include <vector>

template <class Key, class Value, class Func>
bool Traverse(const std::map<Key, Value>& map, Func&& func)
{
    auto it = map.cbegin();
    while (it != map.cend())
    {
        const Value& v = it->second;
        ++it;

        if (!func(v))
            return false;
    }

    return true;
}

template <class Ty, class Func>
bool Traverse(const std::set<Ty>& set, Func&& func)
{
    auto it = set.cbegin();
    while (it != set.cend())
    {
        const Ty& v = *it;
        ++it;

        if (!func(v))
            return false;
    }

    return true;
}

template <class Ty, class Func>
bool Traverse(const std::vector<Ty>& vec, Func&& func)
{
    auto it = vec.cbegin();
    while (it != vec.cend())
    {
        const Ty& v = *it;
        ++it;

        if (!func(v))
            return false;
    }

    return true;
}

struct test_const
{
    int get()
    {
        return 1;
    }

    int get() const
    {
        return const_cast<test_const*>(this)->get();
    }
};

int main(int argc, char* argv[])
{
    test_const ts;
    const test_const cts;
    ts.get();
    cts.get();

    return 0;
}
