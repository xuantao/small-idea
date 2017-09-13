#pragma once

#include "gcf/gcf.h"
#include <string>
#include <vector>

struct ExpMode
{
    std::string mod;
    std::string path;
};

struct Args
{
    std::string srcPath;
    std::string suffix = FILE_SUFFIX;
    std::string expName;
    std::vector<ExpMode> expMods;

    bool IsValid() const;
    bool Load(int argc, char* argv[]);

    static void Helper();
};
