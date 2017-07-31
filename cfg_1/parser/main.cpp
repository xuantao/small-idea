#include <iostream>
#include <fstream>
#include "CfgDef.h"
#include "Scanner.h"
#include "Driver.h"
#include "Context.h"
#include "CppExporter.h"
#include <array>
#include "TabCreator.h"
#include "Utility.h"

CFG_NAMESPACE_USING;

int main(int argc, char** argv)
{

    bool d1 = utility::IsDir(".");
    bool d2 = utility::IsDir("..");
    bool d3 = utility::IsDir("../out");
    bool d4 = utility::IsDir("../out/");
    bool d5 = utility::IsDir("../out/cfg.h");
    bool f1 = utility::IsFile(".");
    bool f2 = utility::IsFile("..");
    bool f3 = utility::IsFile("../out");
    bool f4 = utility::IsFile("../out/");
    bool f5 = utility::IsFile("../out/cfg.h");

    bool c1 = utility::CreateDir("../out");
    bool c2 = utility::CreateDir("../out/test");
    bool c3 = utility::CreateDir("../out/test/");

    utility::TraverseDir("../out", [](const std::string& file, bool dir) {
        std::cout << (dir ? "dir:" : "") << file << std::endl;
        return true;
    });

    auto files = utility::CollectDir("../out/");

    return 1;
    if (argc < 2)
    {
        std::cerr << "please set parser file" << std::endl;
        return 0;
    }

    Driver driver;
    Context context;

    if (!driver.Parse(argv[1], context))
    {
        std::cerr << "initialize scanner failed" << std::endl;
        return 0;
    }

    CppExporter cpp;
    //context.Export(&cpp, "../test/", "Cfg", true);
    //context.Export(&cpp, "../out/", false);

    const auto& tabs = context.TabCfgs();
    const auto& jsons = context.JsonCfgs();

    for (auto it = tabs.cbegin(); it != tabs.cend(); ++it)
    {
        TabCreator tab("../out/" + it->sType->Name() + ".xls");
        utility::Traverse(it->sType, &tab);
    }

    system("pause");
    return 1;
}
