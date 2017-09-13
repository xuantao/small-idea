#include "Args.h"
#include <iostream>

bool Args::IsValid() const
{
    return !srcPath.empty() && !expName.empty();
}

bool Args::Load(int argc, char* argv[])
{
    int idx = 1;
    std::string src;
    while (idx < argc)
    {
        if (std::strcmp(argv[idx], "--src") == 0)
        {
            if (idx + 1 >= argc)
                return false;

            src = argv[++idx];
        }
        else if (std::strcmp(argv[idx], "--suffix") == 0)
        {
            if (idx + 1 >= argc)
                return false;

            suffix = argv[++idx];
        }
        else if (std::strcmp(argv[idx], "--name") == 0)
        {
            if (idx + 1 >= argc)
                return false;

            expName = argv[++idx];
        }
        else if (std::strcmp(argv[idx], "--exp") == 0)
        {
            if (idx + 2 >= argc)
                return false;

            expMods.push_back(ExpMode());
            expMods.back().mod = argv[++idx];
            expMods.back().path = argv[++idx];
        }

        ++idx;
    }

    srcPath = src;
    return true;
}

void Args::Helper()
{
    std::cout << "configuration file parser" << std::endl <<
        "--src:    source file path" << std::endl <<
        "--suffix: target configuration file path" << std::endl <<
        "--exp:    cpp file name(do not need file suffix)" << std::endl << std::endl <<
        "example:  maker --src ../src_path/ --suffix .gcf --name cfg --exp cpp ../cpp_path/ --exp csharp ../cssharp_path/" << std::endl;
}
