/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#include "Caller.h"
#include "SerUtility.h"
#include "Cfg_Ser.h"
#include <cassert>

void Caller::Requester::Call_A(int a, int b)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Call_A_int_int);

    serialize::utility::Write(writer, a);
    serialize::utility::Write(writer, b);

    _invoker->End();
}

int Caller::Requester::Call_B(const std::string& str)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Call_B_string);

    serialize::utility::Write(writer, str);

    int __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

void Caller::Processor::Process(cross_call::IContext* context)
{
    uint32_t code = 0;
    Message msg = Message::Invalid;
    context->Param()->Read(code);
    context->Param()->Read((int&)msg);
    assert(code == HASH_CODE);

    switch (msg)
    {
    case Message::Call_A_int_int:
        OnCall_A_int_int(context);
        break;
    case Message::Call_B_string:
        OnCall_B_string(context);
        break;
    default:
        assert(false);
        break;
    }
}

void Caller::Processor::OnCall_A_int_int(cross_call::IContext* context)
{
    int a;
    int b;

    serialize::utility::Read(context->Param(), a);
    serialize::utility::Read(context->Param(), b);

    _responder->Call_A(a, b);
}

void Caller::Processor::OnCall_B_string(cross_call::IContext* context)
{
    std::string str;

    serialize::utility::Read(context->Param(), str);

    auto __ret__ = _responder->Call_B(str);
    serialize::utility::Write(context->Ret(), __ret__);
}

