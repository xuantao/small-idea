/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#include "TestS2C.h"
#include "SerUtility.h"
#include "Cfg_Ser.h"
#include <cassert>

bool TestS2C::Requester::Test(bool b)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_bool);

    serialize::utility::Write(writer, b);

    bool __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

int TestS2C::Requester::Test(int a)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_int);

    serialize::utility::Write(writer, a);

    int __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

float TestS2C::Requester::Test(float f)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_float);

    serialize::utility::Write(writer, f);

    float __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

std::string TestS2C::Requester::Test(const std::string& s)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_string);

    serialize::utility::Write(writer, s);

    std::string __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

Msg TestS2C::Requester::Test(const Msg& msg)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_Msg);

    serialize::utility::Write(writer, msg);

    Msg __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

void TestS2C::Requester::CallBack()
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::CallBack);


    _invoker->End();
}

void TestS2C::Processor::Process(cross_call::IContext* context)
{
    uint32_t code = 0;
    Message msg = Message::Invalid;
    context->Param()->Read(code);
    context->Param()->Read((int&)msg);
    assert(code == HASH_CODE);

    switch (msg)
    {
    case Message::Test_bool:
        OnTest_bool(context);
        break;
    case Message::Test_int:
        OnTest_int(context);
        break;
    case Message::Test_float:
        OnTest_float(context);
        break;
    case Message::Test_string:
        OnTest_string(context);
        break;
    case Message::Test_Msg:
        OnTest_Msg(context);
        break;
    case Message::CallBack:
        OnCallBack(context);
        break;
    default:
        assert(false);
        break;
    }
}

void TestS2C::Processor::OnTest_bool(cross_call::IContext* context)
{
    bool b;

    serialize::utility::Read(context->Param(), b);

    auto __ret__ = _responder->Test(b);
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestS2C::Processor::OnTest_int(cross_call::IContext* context)
{
    int a;

    serialize::utility::Read(context->Param(), a);

    auto __ret__ = _responder->Test(a);
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestS2C::Processor::OnTest_float(cross_call::IContext* context)
{
    float f;

    serialize::utility::Read(context->Param(), f);

    auto __ret__ = _responder->Test(f);
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestS2C::Processor::OnTest_string(cross_call::IContext* context)
{
    std::string s;

    serialize::utility::Read(context->Param(), s);

    auto __ret__ = _responder->Test(s);
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestS2C::Processor::OnTest_Msg(cross_call::IContext* context)
{
    Msg msg;

    serialize::utility::Read(context->Param(), msg);

    auto __ret__ = _responder->Test(msg);
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestS2C::Processor::OnCallBack(cross_call::IContext* context)
{


    _responder->CallBack();
}

