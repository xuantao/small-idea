#include <stdio.h>
#include "CfgTabParser.h"
#include "Cfg.h"
#include "CfgUtility.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <map>
#include <sstream>

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

struct Keyfn
{
    int Key(Struct2& test) { return test.a; }
};

template <class Kty>
struct TestKey
{
    typedef Kty key_type;

    virtual key_type GetKey() = 0;
};

struct TestDrive : public TestKey<int>
{
    virtual int GetKey() { return 1; }
};

//int _global_it = 0;
//namespace _global_it
//{
//
//}

int main(int argc, char* argv[])
{
    //typedef std::map<int, TestKey> MapKey;

    TestDrive td;

    int i = 0;

    do 
    {
        if (++i < 10)
        {
            printf("i = %d\n", i);
            continue;
        }

    } while (false);

    //TabDataMap<int, Struct2, Keyfn> data;
    //if (data.Load("../work/out/Struct2.tab"))
    //{
    //    data.Find(1);
    //    printf("xxxx\n");
    //}

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
