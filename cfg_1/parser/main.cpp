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
