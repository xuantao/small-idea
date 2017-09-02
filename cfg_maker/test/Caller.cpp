/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#include "Caller.h"
#include "Cfg_Ser.h"
#include <cassert>

void Caller::Invoker::Call_A(int a, int b)
{
    serialize::IWriter* writer = _caller->BeginCall(MODULE_ID, HASH_CODE);
    writer->Write((int)Message::Msg_Call_A);

    serialize::utility::Write(writer, a);
    serialize::utility::Write(writer, b);

    _caller->EndCall();
}

int Caller::Invoker::Call_B(const std::string& str)
{
    int ret;
    serialize::IWriter* writer = _caller->BeginCall(MODULE_ID, HASH_CODE);
    writer->Write((int)Message::Msg_Call_B);

    serialize::utility::Write(writer, str);

    serialize::IReader* reader = _caller->EndCall();
    serialize::utility::Read(reader, ret);
    return ret;
}

void Caller::Processor::Process(serialize::IReader* reader, serialize::IWriter* writer)
{
    Message msg = Message::Invalid;
    reader->Read((int&)msg);

    switch (msg)
    {
    case Message::Msg_Call_A: OnCall_A(reader, writer); break;
    case Message::Msg_Call_B: OnCall_B(reader, writer); break;
    default: assert(false); break;
    }
}

void Caller::Processor::OnCall_A(serialize::IReader* reader, serialize::IWriter* writer)

{
    int a;
    int b;

    serialize::utility::Read(reader, a);
    serialize::utility::Read(reader, b);

    _executor->Call_A(a, b);
}

void Caller::Processor::OnCall_B(serialize::IReader* reader, serialize::IWriter* writer)

{
    std::string str;

    serialize::utility::Read(reader, str);

    serialize::utility::Write(writer, _executor->Call_B(str));
}

