/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#include "TestS2C.h"
#include "Cfg_Ser.h"
#include <cassert>

bool TestS2C::Requester::Test(bool v)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_bool);

    serialize::utility::Write(writer, v);

    bool __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

int8_t TestS2C::Requester::Test(int8_t v)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_byte);

    serialize::utility::Write(writer, v);

    int8_t __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

int TestS2C::Requester::Test(int v)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_int);

    serialize::utility::Write(writer, v);

    int __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

int64_t TestS2C::Requester::Test(int64_t v)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_long);

    serialize::utility::Write(writer, v);

    int64_t __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

float TestS2C::Requester::Test(float v)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_float);

    serialize::utility::Write(writer, v);

    float __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

double TestS2C::Requester::Test(double v)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_double);

    serialize::utility::Write(writer, v);

    double __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

std::string TestS2C::Requester::Test(const std::string& v)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_string);

    serialize::utility::Write(writer, v);

    std::string __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

void TestS2C::Requester::Test(const std::vector<bool>& v)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_bool_A);

    serialize::utility::Write(writer, v);

    _invoker->End();
}

void TestS2C::Requester::Test(const std::array<bool, 2>& v)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_bool_A2);

    serialize::utility::Write(writer, v);

    _invoker->End();
}

void TestS2C::Requester::Test(const std::array<int8_t, 2>& v)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_byte_A2);

    serialize::utility::Write(writer, v);

    _invoker->End();
}

void TestS2C::Requester::Test(const std::vector<std::vector<int> >& v)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_int_A_A);

    serialize::utility::Write(writer, v);

    _invoker->End();
}

void TestS2C::Requester::Test(const std::vector<std::array<int64_t, 2> >& v)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_long_A2_A);

    serialize::utility::Write(writer, v);

    _invoker->End();
}

void TestS2C::Requester::Test(const std::array<std::vector<float>, 2>& v)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_float_A_A2);

    serialize::utility::Write(writer, v);

    _invoker->End();
}

void TestS2C::Requester::Test()
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test);


    _invoker->End();
}

void TestS2C::Requester::Test(const Msg& msg)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_Msg);

    serialize::utility::Write(writer, msg);

    _invoker->End();
}

void TestS2C::Requester::Test(const std::array<Msg, 2>& msg)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_Msg_A2);

    serialize::utility::Write(writer, msg);

    _invoker->End();
}

Msg TestS2C::Requester::Test(int a, int b)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_int_int);

    serialize::utility::Write(writer, a);
    serialize::utility::Write(writer, b);

    Msg __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

std::vector<Msg> TestS2C::Requester::Test(int a, int b, int c)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_int_int_int);

    serialize::utility::Write(writer, a);
    serialize::utility::Write(writer, b);
    serialize::utility::Write(writer, c);

    std::vector<Msg> __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

std::array<Msg, 2> TestS2C::Requester::Test(int a, int b, int c, int d)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::Test_int_int_int_int);

    serialize::utility::Write(writer, a);
    serialize::utility::Write(writer, b);
    serialize::utility::Write(writer, c);
    serialize::utility::Write(writer, d);

    std::array<Msg, 2> __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

KGPlayerData TestS2C::Requester::GetPlayerData()
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::GetPlayerData);


    KGPlayerData __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

void TestS2C::Requester::SetPlayerData(const KGPlayerData& data)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::SetPlayerData_KGPlayerData);

    serialize::utility::Write(writer, data);

    _invoker->End();
}

KGPlayerData TestS2C::Requester::TranslatePlayerData(const KGPlayerData& data)
{
    serialize::IWriter* writer = _invoker->Begin(MODULE_ID);
    writer->Write(HASH_CODE);
    writer->Write((int)Message::TranslatePlayerData_KGPlayerData);

    serialize::utility::Write(writer, data);

    KGPlayerData __ret__;
    serialize::utility::Read(_invoker->End(), __ret__);
    return __ret__;
}

void TestS2C::Processor::Process(cross_call::IContext* context)
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
    case Message::Test_byte:
        OnTest_byte(context);
        break;
    case Message::Test_int:
        OnTest_int(context);
        break;
    case Message::Test_long:
        OnTest_long(context);
        break;
    case Message::Test_float:
        OnTest_float(context);
        break;
    case Message::Test_double:
        OnTest_double(context);
        break;
    case Message::Test_string:
        OnTest_string(context);
        break;
    case Message::Test_bool_A:
        OnTest_bool_A(context);
        break;
    case Message::Test_bool_A2:
        OnTest_bool_A2(context);
        break;
    case Message::Test_byte_A2:
        OnTest_byte_A2(context);
        break;
    case Message::Test_int_A_A:
        OnTest_int_A_A(context);
        break;
    case Message::Test_long_A2_A:
        OnTest_long_A2_A(context);
        break;
    case Message::Test_float_A_A2:
        OnTest_float_A_A2(context);
        break;
    case Message::Test:
        OnTest(context);
        break;
    case Message::Test_Msg:
        OnTest_Msg(context);
        break;
    case Message::Test_Msg_A2:
        OnTest_Msg_A2(context);
        break;
    case Message::Test_int_int:
        OnTest_int_int(context);
        break;
    case Message::Test_int_int_int:
        OnTest_int_int_int(context);
        break;
    case Message::Test_int_int_int_int:
        OnTest_int_int_int_int(context);
        break;
    case Message::GetPlayerData:
        OnGetPlayerData(context);
        break;
    case Message::SetPlayerData_KGPlayerData:
        OnSetPlayerData_KGPlayerData(context);
        break;
    case Message::TranslatePlayerData_KGPlayerData:
        OnTranslatePlayerData_KGPlayerData(context);
        break;
    default:
        assert(false);
        break;
    }
}

void TestS2C::Processor::OnTest_bool(cross_call::IContext* context)
{
    bool v;

    serialize::utility::Read(context->Param(), v);

    auto __ret__ = _responder->Test(v);
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestS2C::Processor::OnTest_byte(cross_call::IContext* context)
{
    int8_t v;

    serialize::utility::Read(context->Param(), v);

    auto __ret__ = _responder->Test(v);
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestS2C::Processor::OnTest_int(cross_call::IContext* context)
{
    int v;

    serialize::utility::Read(context->Param(), v);

    auto __ret__ = _responder->Test(v);
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestS2C::Processor::OnTest_long(cross_call::IContext* context)
{
    int64_t v;

    serialize::utility::Read(context->Param(), v);

    auto __ret__ = _responder->Test(v);
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestS2C::Processor::OnTest_float(cross_call::IContext* context)
{
    float v;

    serialize::utility::Read(context->Param(), v);

    auto __ret__ = _responder->Test(v);
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestS2C::Processor::OnTest_double(cross_call::IContext* context)
{
    double v;

    serialize::utility::Read(context->Param(), v);

    auto __ret__ = _responder->Test(v);
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestS2C::Processor::OnTest_string(cross_call::IContext* context)
{
    std::string v;

    serialize::utility::Read(context->Param(), v);

    auto __ret__ = _responder->Test(v);
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestS2C::Processor::OnTest_bool_A(cross_call::IContext* context)
{
    std::vector<bool> v;

    serialize::utility::Read(context->Param(), v);

    _responder->Test(v);
}

void TestS2C::Processor::OnTest_bool_A2(cross_call::IContext* context)
{
    std::array<bool, 2> v;

    serialize::utility::Read(context->Param(), v);

    _responder->Test(v);
}

void TestS2C::Processor::OnTest_byte_A2(cross_call::IContext* context)
{
    std::array<int8_t, 2> v;

    serialize::utility::Read(context->Param(), v);

    _responder->Test(v);
}

void TestS2C::Processor::OnTest_int_A_A(cross_call::IContext* context)
{
    std::vector<std::vector<int> > v;

    serialize::utility::Read(context->Param(), v);

    _responder->Test(v);
}

void TestS2C::Processor::OnTest_long_A2_A(cross_call::IContext* context)
{
    std::vector<std::array<int64_t, 2> > v;

    serialize::utility::Read(context->Param(), v);

    _responder->Test(v);
}

void TestS2C::Processor::OnTest_float_A_A2(cross_call::IContext* context)
{
    std::array<std::vector<float>, 2> v;

    serialize::utility::Read(context->Param(), v);

    _responder->Test(v);
}

void TestS2C::Processor::OnTest(cross_call::IContext* context)
{


    _responder->Test();
}

void TestS2C::Processor::OnTest_Msg(cross_call::IContext* context)
{
    Msg msg;

    serialize::utility::Read(context->Param(), msg);

    _responder->Test(msg);
}

void TestS2C::Processor::OnTest_Msg_A2(cross_call::IContext* context)
{
    std::array<Msg, 2> msg;

    serialize::utility::Read(context->Param(), msg);

    _responder->Test(msg);
}

void TestS2C::Processor::OnTest_int_int(cross_call::IContext* context)
{
    int a;
    int b;

    serialize::utility::Read(context->Param(), a);
    serialize::utility::Read(context->Param(), b);

    auto __ret__ = _responder->Test(a, b);
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestS2C::Processor::OnTest_int_int_int(cross_call::IContext* context)
{
    int a;
    int b;
    int c;

    serialize::utility::Read(context->Param(), a);
    serialize::utility::Read(context->Param(), b);
    serialize::utility::Read(context->Param(), c);

    auto __ret__ = _responder->Test(a, b, c);
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestS2C::Processor::OnTest_int_int_int_int(cross_call::IContext* context)
{
    int a;
    int b;
    int c;
    int d;

    serialize::utility::Read(context->Param(), a);
    serialize::utility::Read(context->Param(), b);
    serialize::utility::Read(context->Param(), c);
    serialize::utility::Read(context->Param(), d);

    auto __ret__ = _responder->Test(a, b, c, d);
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestS2C::Processor::OnGetPlayerData(cross_call::IContext* context)
{


    auto __ret__ = _responder->GetPlayerData();
    serialize::utility::Write(context->Ret(), __ret__);
}

void TestS2C::Processor::OnSetPlayerData_KGPlayerData(cross_call::IContext* context)
{
    KGPlayerData data;

    serialize::utility::Read(context->Param(), data);

    _responder->SetPlayerData(data);
}

void TestS2C::Processor::OnTranslatePlayerData_KGPlayerData(cross_call::IContext* context)
{
    KGPlayerData data;

    serialize::utility::Read(context->Param(), data);

    auto __ret__ = _responder->TranslatePlayerData(data);
    serialize::utility::Write(context->Ret(), __ret__);
}

