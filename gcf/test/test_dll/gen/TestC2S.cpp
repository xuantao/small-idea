/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#include "TestC2S.h"
#include "Cfg_Ser.h"
#include <cassert>

bool TestC2S::Requester::Test(bool b)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_bool);

    serialize::utility::Write(writer, b);

    bool __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

int TestC2S::Requester::Test(int a)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_int);

    serialize::utility::Write(writer, a);

    int __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

float TestC2S::Requester::Test(float f)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_float);

    serialize::utility::Write(writer, f);

    float __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

std::string TestC2S::Requester::Test(const std::string& s)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_string);

    serialize::utility::Write(writer, s);

    std::string __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

Msg TestC2S::Requester::Test(const Msg& msg)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_Msg);

    serialize::utility::Write(writer, msg);

    Msg __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

void TestC2S::Requester::Test(const std::vector<std::array<int, 10> >& ar)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_int_A10_A);

    serialize::utility::Write(writer, ar);

    _invoker->End();
}

void TestC2S::Processor::Process(cross_call::IContext* context)
{
    int32_t code = 0;
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
    case Message::Test_int_A10_A:
        OnTest_int_A10_A(context);
        break;
    default:
        assert(false);
        break;
    }
}

void TestC2S::Processor::OnTest_bool(cross_call::IContext* context)
{
    bool b;

    serialize::utility::Read(context->Param(), b);

    auto __ret__ = _responder->Test(b);
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestC2S::Processor::OnTest_int(cross_call::IContext* context)
{
    int a;

    serialize::utility::Read(context->Param(), a);

    auto __ret__ = _responder->Test(a);
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestC2S::Processor::OnTest_float(cross_call::IContext* context)
{
    float f;

    serialize::utility::Read(context->Param(), f);

    auto __ret__ = _responder->Test(f);
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestC2S::Processor::OnTest_string(cross_call::IContext* context)
{
    std::string s;

    serialize::utility::Read(context->Param(), s);

    auto __ret__ = _responder->Test(s);
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestC2S::Processor::OnTest_Msg(cross_call::IContext* context)
{
    Msg msg;

    serialize::utility::Read(context->Param(), msg);

    auto __ret__ = _responder->Test(msg);
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestC2S::Processor::OnTest_int_A10_A(cross_call::IContext* context)
{
    std::vector<std::array<int, 10> > ar;

    serialize::utility::Read(context->Param(), ar);

    _responder->Test(ar);
}

