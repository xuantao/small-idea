#include <stdio.h>
#include <fstream>
#include <memory>
#include "Config.h"

int main(int argc, char* argv[])
{
    std::ifstream file;
    file.open("const.json");
    if (!file.is_open())
    {
        printf("load file failed\n");
        return 0;
    }

    file.seekg(0, std::ios::beg);
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::auto_ptr<char> buff(new char[size + 1]);
    file.read(buff.get(), size);

    Config config;
    if (buff.get()[0] == 0xef && buff.get()[1] == 0xbb && buff.get()[2] == 0xbf)
    {
        if (config.LoadJson(buff.get() + 3, size - 3))
            printf("parse successed\n");
        else
            printf("parse failed\n");
    }
    else
    {
        if (config.LoadJson(buff.get() + 3, size - 3))
            printf("parse successed\n");
        else
            printf("parse failed\n");
    }

    return 0;
}
