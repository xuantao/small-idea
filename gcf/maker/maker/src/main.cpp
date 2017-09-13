#include "Driver.h"
#include "Context.h"
#include "Args.h"
#include "utility/Utility.h"
#include <iostream>
#include <fstream>
#include <array>
#include <Windows.h>

GCF_NAMESPACE_USING;

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
    //std::string path = utility::TrimRight(arg.srcPath, "/\\");
    //std::vector<std::string> srcs = utility::CollectDir(path, CFG_FILE_SUFFIX);

    //if (srcs.empty())
    //{
    //    std::cout << "can not find any file in path: " << arg.srcPath << std::endl;
    //    return;
    //}

    //Driver driver;
    //Context context(driver);

    //bool success = driver.Parse(context, path, srcs);
    //utility::Log(std::cout, "parse completed, error:{0} warning:{1}", driver.ErrorNum(), driver.WarNum());

    //if (!success)
    //{
    //    std::cout << "parse failed, please fix the errors" << std::endl;
    //    return;
    //}

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
    if (!arg.Load(argc, argv))
    {
        Args::Helper();
        system("pause");
        return 0;
    }
    //if (ParseArgs(argc, argv, arg))
    //{
    //    DoWork(arg);
    //}
    //else
    //{
    //    LogInfo();
    //}
    HMODULE hModule = ::LoadLibraryA("cpp.dll");
    FARPROC proc = ::GetProcAddress(hModule, "GetExporter");
    CreateExporter exp = (CreateExporter)proc;

    IExporter* exporter = exp();

    //exporter->OnBegin(nullptr, "");

    system("pause");
    return 1;
}
