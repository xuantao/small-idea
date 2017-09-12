﻿#include "Driver.h"
#include "Context.h"
#include "utility/Utility.h"
#include <iostream>
#include <fstream>
#include <array>
CFG_NAMESPACE_USING;

struct Args
{
    std::string srcPath;    // source path
    std::string cfgPath;    // configuration path
    std::string cppFile;    // cpp code file
    std::string csFile;     // Csharp code file
};

static bool ParseArgs(int argc, char** argv, Args& out)
{
    int idx = 1;
    while (idx < argc)
    {
        if (std::strcmp(argv[idx], "-src") == 0)
        {
            if (idx + 1 >= argc)
                break;

            out.srcPath = argv[++idx];
            ++idx;
        }
        else if (std::strcmp(argv[idx], "-cfg") == 0)
        {
            if (idx + 1 >= argc)
                break;

            out.cfgPath = argv[++idx];
            ++idx;
        }
        else if (std::strcmp(argv[idx], "-cpp") == 0)
        {
            if (idx + 1 >= argc)
                break;

            out.cppFile = argv[++idx];
            ++idx;
        }
        else if (std::strcmp(argv[idx], "-cs") == 0)
        {
            if (idx + 1 >= argc)
                break;

            out.csFile = argv[++idx];
            ++idx;
        }
        else
        {
            ++idx;
        }
    }

    return !out.srcPath.empty();
}

static void LogInfo()
{
    std::cout << "configuration file parser" << std::endl <<
        "-src: source file path" << std::endl <<
        "-cfg: target configuration file path" << std::endl <<
        "-cpp: cpp file name(do not need file suffix)" << std::endl <<
        "-cs : csharp file name" << std::endl;
}

static void DoWork(Args& arg)
{
    std::string path = utility::TrimRight(arg.srcPath, "/\\");
    std::vector<std::string> srcs = utility::CollectDir(path, CFG_FILE_SUFFIX);

    if (srcs.empty())
    {
        std::cout << "can not find any file in path: " << arg.srcPath << std::endl;
        return;
    }

    Driver driver;
    Context context(driver);

    bool success = driver.Parse(context, path, srcs);
    utility::Log(std::cout, "parse completed, error:{0} warning:{1}", driver.ErrorNum(), driver.WarNum());

    if (!success)
    {
        std::cout << "parse failed, please fix the errors" << std::endl;
        return;
    }

    //if (!arg.cfgPath.empty())
    //{
    //    TabCreater tab;
    //    context.Export(&tab, arg.cfgPath);

    //    JsonCreater json;
    //    context.Export(&json, arg.cfgPath);
    //}

    //if (!arg.cppFile.empty())
    //{
    //    CppExporter cpp;
    //    context.Export(&cpp, arg.cppFile);
    //}

    //if (!arg.csFile.empty())
    //{
    //    //std::cout << "Csharp exporter has not completed" << std::endl;
    //    CsExporter cs;
    //    context.Export(&cs, arg.csFile);
    //}
}

int main(int argc, char** argv)
{
    Args arg;
    if (ParseArgs(argc, argv, arg))
    {
        DoWork(arg);
    }
    else
    {
        LogInfo();
    }

    system("pause");
    return 1;
}
