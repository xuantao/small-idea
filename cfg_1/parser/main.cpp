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

struct Args
{
    std::string srcPath;    // source file
    std::string cfgPath;    // configuration
    std::string cppPath;    // cpp code
    std::string csPath;     // c sharp code
};

int main(int argc, char** argv)
{

    //bool d1 = utility::IsDir(".");
    //bool d2 = utility::IsDir("..");
    //bool d3 = utility::IsDir("../out");
    //bool d4 = utility::IsDir("../out/");
    //bool d5 = utility::IsDir("../out/cfg.h");
    //bool f1 = utility::IsFile(".");
    //bool f2 = utility::IsFile("..");
    //bool f3 = utility::IsFile("../out");
    //bool f4 = utility::IsFile("../out/");
    //bool f5 = utility::IsFile("../out/cfg.h");

    //bool c1 = utility::CreateDir("../out");
    //bool c2 = utility::CreateDir("../out/test");
    //bool c3 = utility::CreateDir("../out/test/");

    //utility::TraverseDir("../out", [ ](const std::string& file, bool dir) {
    //    std::cout << (dir ? "dir:" : "") << file << std::endl;
    //    return true;
    //});

    //auto files = utility::CollectDir("../out/");

    //std::cout << "Left  :l" << utility::TrimLeft(" \tx\t ", " \t") << "r" << std::endl;
    //std::cout << "Left  :l" << utility::TrimLeft(" \t\t ", " \t") << "r" << std::endl;
    //std::cout << "Right :l" << utility::TrimRight(" \t\t ", " \t") << "r" << std::endl;
    //std::cout << "Right :l" << utility::TrimRight(" \tx\t ", " \t") << "r" << std::endl;

    /*std::string p;
    std::string f;
    std::string e;

    utility::SplitPath("", p, f, e);
    std::cout << "p:" << p << " f:" << f << " e:" << e << std::endl;

    utility::SplitPath("test", p, f, e);
    std::cout << "p:" << p << " f:" << f << " e:" << e << std::endl;

    utility::SplitPath("test.txt", p, f, e);
    std::cout << "p:" << p << " f:" << f << " e:" << e << std::endl;

    utility::SplitPath("./out/text", p, f, e);
    std::cout << "p:" << p << " f:" << f << " e:" << e << std::endl;

    utility::SplitPath("./out/text.txt", p, f, e);
    std::cout << "p:" << p << " f:" << f << " e:" << e << std::endl;

    utility::SplitPath(".\\out\\text.txt", p, f, e);
    std::cout << "p:" << p << " f:" << f << " e:" << e << std::endl;


    std::cout << utility::AbsolutePath("../") << std::endl;
    std::cout << utility::AbsolutePath("../../") << std::endl;
    std::cout << utility::AbsolutePath("../xuantao/./") << std::endl;
    std::cout << utility::AbsolutePath("../xuantao/../") << std::endl;
    std::cout << utility::AbsolutePath("../xuantao/../../") << std::endl;

    return 1;*/

    Args arg;
    arg.srcPath = "../work/src";
    arg.cfgPath = "../work/cfg";
    arg.cppPath = "../work/cpp";
    arg.csPath = "../work/cs";

    std::vector<std::string> srcs = utility::CollectDir(arg.srcPath);

    Driver driver;
    Context context(driver);

    if (!driver.Parse(context, srcs))
    {
        std::cerr << "initialize scanner failed" << std::endl;
        return 0;
    }

    CppExporter cpp;
    context.Export(&cpp, "../test/", "Cfg", true);
    //context.Export(&cpp, "../out/", false);

    //const auto& tabs = context.TabCfgs();
    //const auto& jsons = context.JsonCfgs();

    //for (auto it = tabs.cbegin(); it != tabs.cend(); ++it)
    //{
    //    TabCreator tab("../out/" + it->sType->Name() + ".xls");
    //    utility::Traverse(it->sType, &tab);
    //}

    system("pause");
    return 1;
}
