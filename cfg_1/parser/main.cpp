#include <iostream>
#include <fstream>
#include "Scanner.h"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "please set parser file" << std::endl;
        return 0;
    }

    Cfg::Scanner* pScanner = new Cfg::Scanner();

    if (!pScanner->Init(argv[1]))
    {
        std::cerr << "initialize scanner failed" << std::endl;
        return 0;
    }

    while (0 != pScanner->Lex())
        ;

    system("pause");
    return 1;
}
