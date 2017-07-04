/*
 * this file is auto generated, please do not edit it manual!
 * 2017/7/4 9:53:33
*/

#pragma once
#include <vector>
#include <string>

class Config
{
public:
    struct Type_1
    {
    };

    struct Type_2
    {
        Type_2()
            : a(0)
            , b(0)
        {}

        int a;
        int b;
    };

    struct Type_3
    {
        Type_3()
            : d1(0.0f)
        {}

        float d1;
        std::string d2;
    };

    struct Type_4
    {
        Type_4()
            : a(0)
            , b(0)
        {}

        int a;
        int b;
        std::vector<std::vector<int> > c;
    };

    struct Type_5
    {
        Type_5()
            : a(0)
            , b(0)
            , c(false)
        {}

        int a;
        int b;
        bool c;
    };

    struct Type_6
    {
        Type_6()
            : a(0)
            , b(0)
            , c(false)
        {}

        int a;
        int b;
        bool c;
        Type_3 d;
    };

    struct Type_7
    {
        Type_7()
        {}

        Type_3 d;
    };

    struct Type_8
    {
        Type_8()
        {}

        Type_2 mem1;
        Type_2 mem2;
    };

public:
    Config();

public:
    bool LoadJson(const char* lpJson, int nSize);

public:
    std::string nomal;
    Type_4 objNrmal;
    Type_8 objComplex;
    std::vector<Type_1> arrayEmpty;
    std::vector<int> arrayNormal;
    std::vector<Type_2> arrayObj;
    std::vector<Type_6> arrayObjComplex;
    std::vector<std::vector<int> > arrayDouble;
};
