#include "Caller.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <map>
#include <sstream>

class Exe : public Caller::IExecutor
{
public:
    virtual ~Exe() {}

public:
    virtual void Call_A(int a, int b)
    {
        printf("call_a(a:%d, b%d)\n", a, b);
    }

    virtual int Call_B(const std::string& str)
    {
        printf("call_b(str:%s)\n", str.c_str());
        return 101;
    }
};

int main(int argc, char* argv[])
{


    return 1;
}
