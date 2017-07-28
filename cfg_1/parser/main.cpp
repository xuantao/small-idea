#include <iostream>
#include <fstream>
#include "Scanner.h"
#include "Driver.h"
#include "Context.h"
#include "CppExporter.h"
#include <array>

CFG_NAMESPACE_USING;

const char* const * Names()
{
    static const char* ss[] = {"xuantao", "zouhui", "xiaoyan"};
    int n = sizeof(ss);
    //Count(Temp());
    //Count(Temp2());
    return ss;
}

int main(int argc, char** argv)
{
    Names();
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
    context.Export(&cpp, "../out/cfg", true);
    context.Export(&cpp, "../out/", false);

    system("pause");
    return 1;
}
