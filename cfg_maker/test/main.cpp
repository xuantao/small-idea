#include <stdio.h>
#include "CfgTabParser.h"
#include "Cfg.h"
#include <fstream>
#include <iostream>
#include <memory>

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

int main(int argc, char* argv[])
{
    TestTab();
    TestJson();

    system("pause");

    return 1;
}
