#include <iostream>
#include <fstream>
#include "Scanner.h"
#include "Driver.h"
#include "Context.h"
#include "CppExporter.h"

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

    CppExporter::Export(context.Global(), std::cout);

    system("pause");
    return 1;
}
