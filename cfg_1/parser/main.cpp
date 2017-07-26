#include <iostream>
#include <fstream>
#include "Scanner.h"
#include "Driver.h"
#include "Context.h"
#include "CppExporter.h"
#include <array>

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

    CppExporter::Export(std::cout, context.Global(), context.GetFileData(0));

    std::array<int, 2> ar;

    system("pause");
    return 1;
}
