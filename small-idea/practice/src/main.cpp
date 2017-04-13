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

int main(int argc, char* argv[])
{
    std::map<int, int> map;
    map.insert(std::make_pair(1, 1));
    Traverse(map, [ ](int i) {
        printf("1  %d\n", i);
        return true;
    });

    std::set<int> set;
    set.insert(2);
    Traverse(set, [ ](int i) {
        printf("2  %d\n", i);
        return true;
    });

    std::vector<int> vec;
    vec.push_back(3);
    Traverse(vec, [ ](int i) {
        printf("3  %d\n", i);
        return true;
    });
    return 0;
}
