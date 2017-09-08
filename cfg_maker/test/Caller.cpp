/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#include "Caller.h"
#include "SerUtility.h"
#include "Cfg_Ser.h"
#include <cassert>

void Caller::Invoker::Call_A(int a, int b)
{
    serialize::IWriter* writer = _cross->BeginCall(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Msg_Call_A);

    serialize::utility::Write(writer, a);
    serialize::utility::Write(writer, b);

    _caller->EndCall();
}

int Caller::Invoker::Call_B(const std::string& str)
{
    serialize::IWriter* writer = _cross->BeginCall(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Msg_Call_B);

    serialize::utility::Write(writer, str);

    int __ret__;
    serialize::utility::Read(_cross->EndCall(), __ret__);
    return __ret__;
}

void Caller::Processor::Process(cross_call::IContext* context)
{
    uint32_t code = 0
    Message msg = Message::Invalid;
    reader->Read(code);
    reader->Read((int&)msg);
    assert(code == HASH_CODE);

    switch (msg)
    {
    case Message::Msg_Call_A: OnCall_A(context); break;
    case Message::Msg_Call_B: OnCall_B(context); break;
    default: assert(false); break;
    }
}

void Caller::Processor::OnCall_A(cross_call::IContext* context)
{
    int a;
    int b;

    serialize::utility::Read(context->Param(), a);
    serialize::utility::Read(context->Param(), b);

    _executor->Call_A(a, b);
}

void Caller::Processor::OnCall_B(cross_call::IContext* context)
{
    std::string str;

    serialize::utility::Read(context->Param(), str);

    auto __ret__ = _executor->Call_B(str);
    serialize::utility::Read(context->Ret(), __ret__);
}

