#include "TestTmp.h"
#include "gcf/tab/Parser.h"
#include "gcf/tab/Data.h"
#include "Cfg_Tab.h"
#include "Cfg_Ser.h"
#include "Cfg_Enum.h"
#include <fstream>
#include <stdio.h>

namespace test_name_space
{
    struct test_name_space
    {
        int a;
    };
}

namespace test_name_space
{
    struct test_b
    {
        int a;
    };
}

void testReader(serialize::IReader* reader, int count)
{
    for (int i = 0; i < count; ++i)
    {
        std::string str;
        reader->Read(str);
        printf("read idx:%d, str:%s\n", i + 1, str.c_str());
    }
}

void TestBool(bool& b)
{
    printf("void TestBool(bool b)");
}

template <class Ty>
void TestBool(std::vector<Ty>& v)
{
    //for (size_t i = 0; i < v.size(); i++)
    for (auto it = v.begin(); it != v.end(); ++it)
    {
        bool b;
        TestBool(b);
        *it = b;
    }
    printf("xxxxxxxxxxxx");
}

int main(int argc, char* argv[])
{
    const char* const arr[] = {"xuantao", "zouhui", "xiaoyan", nullptr};
    const char* lineTitle = "papa\txuantao\txiaoyan\tzouhui\tmama";
    char lineData1[1024] = "d1\td2\td3";
    char lineData2[1024] = "d2\td3\td4\td5";
    tab::IndexMap im;
    im.Setup(arr, lineTitle);

    tab::Parser parser(im);
    serialize::IReader* reader = parser.Parse(lineData1);
    testReader(reader, 5);

    printf("11111111111\n");

    reader = parser.Parse(lineData2);
    testReader(reader, 5);

    tab::DataList<Sr3> data;
    data.LoadText("test.tab");

    const char* name1 = Enum::Name(Detail::Enum2::d);
    const char* name2 = Enum::Name<Detail::Enum2>(2);
    Wtf wtf1 = Enum::Value<Wtf>("Float");
    Wtf wtf2 = Enum::Value<Wtf>(4);
    int idx1 = Enum::Index(GlobalType::Int);
    int idx2 = Enum::Index<GlobalType>("Int");

    test_name_space::test_name_space tns;

    system("pause");
    return 1;
}
