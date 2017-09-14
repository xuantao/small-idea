#include "Driver.h"
#include "Context.h"
#include "Args.h"
#include "DllModule.h"
#include "utility/Utility.h"
#include <iostream>
#include <fstream>
#include <array>

GCF_NAMESPACE_USING;

static bool DoWork(const Args& arg)
{
    std::string path = utility::TrimRight(arg.srcPath, "/\\");
    std::vector<std::string> srcs = utility::CollectDir(path, arg.suffix);

    if (srcs.empty())
    {
        std::cout << "can not find any file in path: " << arg.srcPath << std::endl;
        return false;
    }

    Driver driver;
    Context context(driver);

    bool success = driver.Parse(context, path, srcs);
    utility::Log(std::cout, "parse completed, error:{0} warning:{1}", driver.ErrorNum(), driver.WarNum());

    if (!success)
    {
        std::cout << "parse failed, please fix the errors" << std::endl;
        return false;
    }

    for (const auto& ep : arg.expMods)
    {
        DllModule dll;
        if (!dll.Load(ep.mod.c_str()))
        {
            std::cout << "load exporter module failed:" << ep.mod << std::endl;
            continue;
        }

        auto proc = dll.GetProc<CreateExporter>(EXP_MODULE_API);
        if (proc == nullptr)
        {
            std::cout << "load module:" << ep.mod << " proc:" << EXP_MODULE_API << " failed" << std::endl;
            continue;
        }

        IExporter* exportor = proc();
        if (nullptr == exportor)
        {
            std::cout << "get module:" << ep.mod << " exporter failed" << std::endl;
            continue;
        }

        if (exportor->OnBegin(context.Global(), ep.path.c_str(), arg.expName.c_str()))
        {
            context.Export(exportor);
            exportor->OnEnd();
        }
        exportor->Release();
    }
    return true;
}

int main(int argc, char** argv)
{
    Args arg;
    bool result = false;
    if (arg.Load(argc, argv))
        result = DoWork(arg);
    else
        Args::Helper();

    system("pause");
    return 1;
}
