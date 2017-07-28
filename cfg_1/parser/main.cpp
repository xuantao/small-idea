#include <iostream>
#include <fstream>
#include "CfgDef.h"
//#include "Scanner.h"
//#include "Driver.h"
//#include "Context.h"
//#include "CppExporter.h"
#include <array>

#include "TabParser.h"

CFG_NAMESPACE_USING;

void Names()
{
    //static const char* ss[] = {"xuantao", "zouhui", "xiaoyan"};
    //int n = sizeof(ss);
    std::array<const char*, 3> ss2 = { "xuantao", "zouhui", "xiaoyan" };

    TabParser<3> tab(ss2);
    tab.Parse();

    //Count(Temp());
    //Count(Temp2());
    
}

int main(int argc, char** argv)
{
    Names();
    return 1;
    //if (argc < 2)
    //{
    //    std::cerr << "please set parser file" << std::endl;
    //    return 0;
    //}

    //Driver driver;
    //Context context;

    //if (!driver.Parse(argv[1], context))
    //{
    //    std::cerr << "initialize scanner failed" << std::endl;
    //    return 0;
    //}

    //CppExporter cpp;
    //context.Export(&cpp, "../out/cfg", true);
    //context.Export(&cpp, "../out/", false);

    //system("pause");
    return 1;
}
