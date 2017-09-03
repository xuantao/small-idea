#include "Caller.h"
#include "CrossCall.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <map>
#include <sstream>

using namespace cross_call;

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

void TestMemory()
{
    char buffer[20] = { 0 };
    SwapMemory mem(buffer, 20);
    serialize::IReader* reader = mem.GetReader();
    serialize::IWriter* writer = mem.GetWriter();

    uint32_t src = 1;
    uint32_t dst;

    std::cout << writer->Write(src) << std::endl;
    std::cout << reader->Read(dst) << std::endl;

    std::cout << writer->Write(src) << std::endl;
    ++src;
    std::cout << writer->Write(src) << std::endl;
    ++src;
    std::cout << writer->Write(src) << std::endl;

    std::cout << reader->Read(dst) << std::endl;
    std::cout << reader->Read(dst) << std::endl;
    std::cout << reader->Read(dst) << std::endl;
}

void TestCrossCall()
{
    Exe excutor;
    CrossCallManager manager;
    manager.Init();

    /*Exe* exe = new Exe(manager.GetCaller());*/
    Caller::Invoker invoker(manager.GetCaller());
    Caller::Processor proc(&excutor);

    manager.Register(&proc);

    invoker.Call_A(1, 2);
    printf("call_b:%d", invoker.Call_B("xuantao"));

}

int main(int argc, char* argv[])
{
    TestMemory();
    TestCrossCall();
    system("pause");
    return 1;
}
