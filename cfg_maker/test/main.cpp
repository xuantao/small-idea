#include <stdio.h>
#include "CfgTabParser.h"
#include "Cfg.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <map>

using namespace cfg;

struct Buffer
{
    Buffer() : buff(nullptr), size(0)
    {
    }

    char* buff;
    size_t size;
};

bool LoadFile(const std::string& file, Buffer& buff)
{
    std::ifstream inF;
    inF.open(file, std::ios::in);
    if (!inF.is_open())
        return false;

    inF.seekg(0, std::ios::beg);
    inF.seekg(0, std::ios::end);
    buff.size = inF.tellg();
    inF.seekg(0, std::ios::beg);

    buff.buff = new char[buff.size + 1];
    inF.read(buff.buff, buff.size);
    inF.close();

    buff.buff[buff.size] = 0;
    return true;
}

void TestJson()
{
    Buffer buf;
    if (!LoadFile("../work/out/Struct1.json", buf))
    {
        printf("load json file failed\n");
        return;
    }

    Struct1 s1;
    Json::Load(buf.buff, buf.size, s1);
}

void TestTab()
{
    Buffer buf;
    if (!LoadFile("../work/out/Struct2.tab", buf))
    {
        printf("load tab file failed\n");
        return;
    }

    std::vector<Struct2> data;
    Tab::Load(buf.buff, buf.size, data);
}

/*
 * pair maker example
struct PmakerExample
{
    std::pair<KeyType, ValueType> operator() (ValueType& value)
    {
        return std::make_pair(value.key, value);
    }
};
*/

struct PmakerExample
{
    std::pair<int, Struct2> operator() (Struct2& key)
    {
        return std::make_pair(key.a, key);
    }
};

template <class Kty, class Vty, class Pmaker, class Less = std::less<Kty>, class Alloc = std::allocator<std::pair<const Kty, Vty> > >
class TabDataMap
{
public:
    typedef Kty key_type;
    typedef Vty value_type;
    typedef const value_type* const_pointer;
    typedef Pmaker pair_maker;
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

        pair_maker maker;
        for (auto it = vecData.begin(); it != vecData.end(); ++it)
        {
            if (!_data.insert(maker(*it)).second)
                printf("key conficted");
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


struct TestKey
{
    int id;
    int data;
};


int main(int argc, char* argv[])
{
    typedef std::map<int, TestKey> MapKey;

    TabDataMap<int, Struct2, PmakerExample> dataMap;
    if (dataMap.Load("../work/out/Struct2.tab"))
    {
        dataMap.Find(1);
        printf("xxxx\n");
    }

    //ks.find(key);

    //Struct1 s1;
    //Json::Write(std::cout, s1);

    //Struct2 s2;
    //Tab::Write(std::cout, s2);

    //TestTab();
    //TestJson();

    system("pause");

    return 1;
}
