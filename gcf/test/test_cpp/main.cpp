#include "TestTmp.h"
#include "gcf/tab/Parser.h"
#include "gcf/tab/Data.h"
#include "Cfg_Tab.h"
#include "Cfg_Ser.h"
#include <fstream>
#include <stdio.h>

/*
* 读取文件
*/
class TabFileData
{
public:
    TabFileData() : _buff(nullptr), _size(0)
    {
    }

    ~TabFileData()
    {
        delete _buff;
        _buff = nullptr;
    }

public:
    char* Buffer() const { return _buff; }
    size_t Size() const { return _size; }

    bool Load(const char* file)
    {
        std::ifstream fs;
        fs.open(file, std::ios::in);
        if (!fs.is_open())
            return false;

        size_t size = 0;
        fs.seekg(0, std::ios::end);
        size = fs.tellg();
        fs.seekg(0, std::ios::beg);

        // skip bom header
        if (size >= 3)
        {
            char bom[3] = {0};
            fs.read(bom, 3);
            if ((uint8_t)bom[0] == 0xEF && (uint8_t)bom[1] == 0xBB && (uint8_t)bom[2] == 0xBF)
                size -= 3;
            else
                fs.seekg(0, std::ios::beg);
        }

        _buff = new char[size + 1];
        if (_buff == nullptr)
            return false;

        _size = size;
        fs.read(_buff, size);
        fs.close();

        return true;
    }

protected:
    char* _buff;
    size_t _size;
};

void testReader(serialize::IReader* reader, int count)
{
    for (int i = 0; i < count; ++i)
    {
        std::string str;
        reader->Read(str);
        printf("read idx:%d, str:%s\n", i + 1, str.c_str());
    }
}

int main(int argc, char* argv[])
{

    //for (int i = 0; TabInfo<TmpTy>::title[i]; ++i)
    //{
    //    printf("title[%d] = %s\n", i, TabInfo<TmpTy>::title[i]);
    //}

    //const char* n1 = Name(TestEnum::a);
    //const char* n2 = Name<TestEnum>(0);
    //int idx1 = Index<TestEnum>(TestEnum::a);
    //int idx2 = Index<TestEnum>("xuantao");

    //TestEnum val1 = Value<TestEnum>(0);
    //TestEnum val2 = Value<TestEnum>("xuantao");

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

    TabFileData fd;
    if (fd.Load("test.tab"))
    {
        tab::DataList<Sr3> data;
        data.Load(fd.Buffer());
    }

    return 1;
}
