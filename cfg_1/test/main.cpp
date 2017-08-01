#include <stdio.h>
#include "TabParser.h"
#include <fstream>
#include <iostream>

int main(int argc, char* argv[])
{
    std::ifstream file;
    file.open("test.tab", std::ios::in);
    if (!file.is_open())
    {
        printf("load failed");
        return 0;
    }

    file.seekg(0, std::ios::beg);
    file.seekg(0, std::ios::end);
    std::size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buffer = new char[size + 1];
    file.read(buffer, size);
    buffer[size] = 0;

    std::array<const char*, 5> title = {"ID","Name", "Type", "Desc", "Check"};
    TabParser<5> parser(title);
    parser.Parse(buffer, size);

    do
    {
        TabIter iter = parser.Curr();
        for (size_t i = 0; i < 5; ++i)
            std::cout << iter.Next() << " ";
        std::cout << std::endl;
    } while (parser.NextLine());

    system("pause");

    return 1;
}
