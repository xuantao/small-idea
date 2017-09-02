/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#include "Invoker.h"

void Invoker::Invoker::Call_A(int a, int b)
{
    IWriter* writer = _caller->BeginCall(MODULE_ID, HASH_CODE);
    writer->Write((int)Message::Call_A)

    writer->Write(Call_A);
    writer->Write(Call_A);

    _caller->EndCall();
}

int Invoker::Invoker::Call_B(const std::string& str)
{
    int ret;
    IWriter* writer = _caller->BeginCall(MODULE_ID, HASH_CODE);
    writer->Write((int)Message::Call_B)

    writer->Write(Call_B);

    IReader* reader = _caller->EndCall();
    Read(reader, ret);
    return ret;
}

void Invoker::Processor::Process(IReader* reader, IWriter* writer)
{
    Message msg = Message::Invalid;
    reader->Read((int&)msg)

    switch (msg)
    {
    case Message:Call_A: OnCall_A(reader, writer); break;
    case Message:Call_B: OnCall_B(reader, writer); break;
    default: assert(false); break;
    }
}

void Invoker::Processor::Call_A(int a, int b)
{
    int a;
    int b;

    Read(reader, a);
    Read(reader, b);

    OnCall_A(a, b);
}

int Invoker::Processor::Call_B(const std::string& str)
{
    std::string str;

    Read(reader, str);

    Write(writer, OnCall_B(str));
}

