﻿#include <iostream>
#include <fstream>
#include "scanner.h"

int main(int argc, char** argv)
{
    zhFlexLexer* pScanner = nullptr;
    std::ostream* out = &std::cout;
    std::istream* in = &std::cin;
    std::ifstream file;

    if (argc > 1)
    {
        file.open(argv[1], std::ios::in);
        if (!file.is_open())
        {
            printf("can not open file %s\n", argv[1]);
            return 0;
        }

        in = &file;
    }

    pScanner = new zhFlexLexer(in, out);
    pScanner->yylex();

    return 1;
}
