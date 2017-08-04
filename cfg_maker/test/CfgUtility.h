/*
 * 基础工具
*/
#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <stdio.h>

#define CFG_INVALID_ENUM    -1000000

namespace cfg
{
    namespace utility
    {
        typedef void(*FnLogger)(const char*);

        bool Convert(const std::string& str, bool& out);
        bool Convert(const std::string& str, int& out);
        bool Convert(const std::string& str, float& out);
        std::vector<std::string> Split(const std::string& str, const std::string& s);
    }

    /*
     * Keyfn example
     struct Keyfn
     {
        key_type key(value_type& value) { return value.key; }
     };
    */

    template <class Ty>
    struct ValueFilter
    {
        typedef Ty value_type;

        bool filter(value_type& value) { return true; }
    };

    /*
     * 配置数据map表
    */
    template <
        class Kty, class Vty, class Keyfn,
        class Filter = ValueFilter<Vty>,
        class Less = std::less<Kty>,
        class Alloc = std::allocator<std::pair<const Kty, Vty> >
    >
    class TabDataMap
    {
    public:
        typedef Kty key_type;
        typedef Vty value_type;
        typedef const value_type* const_pointer;
        typedef Keyfn key_fn;
        typedef Filter value_filter;
        typedef Less less;
        typedef Alloc allocator;
        typedef std::map<key_type, value_type, less, allocator> map_data;

    public:
        bool Load(const char* fileName)
        {
            std::ifstream fs;
            fs.open(fileName, std::ios::in);
            if (!fs.is_open())
                return false;

            size_t size = 0;
            fs.seekg(0, std::ios::end);
            size = fs.tellg();
            fs.seekg(0, std::ios::beg);

            std::unique_ptr<char> buff(new char[size + 1]);
            fs.read(buff.get(), size);
            fs.close();
            buff.get()[size] = 0;

            if (
                size >= 3 &&
                (uint8_t)buff.get()[0] == 0xEF &&
                (uint8_t)buff.get()[1] == 0xBB &&
                (uint8_t)buff.get()[2] == 0xBF
                )
            {
                return Load(buff.get() + 3, size - 3); // skip BOM head
            }
            else
            {
                return Load(buff.get(), size);
            }
        }

        bool Load(const char* data, size_t size)
        {
            std::vector<value_type> vecData;
            if (!Tab::Load(data, size, vecData))
                return false;

            key_fn kfn;
            value_filter vf;
            for (auto it = vecData.begin(); it != vecData.end(); ++it)
            {
                if (!vf.filter(*it))
                    continue;

                if (!_data.insert(std::make_pair(kfn.key(*it), *it)).second)
                    printf("key confict");
            }

            return true;
        }

        const_pointer Find(const key_type& key) const
        {
            auto it = _data.find(key);
            if (it == _data.end())
                return nullptr;
            return &(it->second);
        }

        template <class Func>
        bool Traverse(Func&& func) const
        {
            for (auto it = _data.cbegin(); it != _data.cend();)
            {
                const value_type& val = it->second;
                ++it;

                if (!func(val))
                    return false;
            }
            return true;
        }

    protected:
        map_data _data;
    };
}
