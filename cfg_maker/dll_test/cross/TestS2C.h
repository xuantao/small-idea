/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#pragma once

#include "Cfg.h"
#include "ICrossCall.h"

class TestS2C
{
public:
    static const uint32_t MODULE_ID = 3;
    static const uint32_t HASH_CODE = 1180210188;

    enum class Message
    {
        Invalid,
        Test_bool,
        Test_int,
        Test_float,
        Test_string,
        Test_Msg,
        CallBack,
    };

    class IResponder
    {
    public:
        virtual ~IResponder() {}

    public:
        virtual bool Test(bool b) = 0;
        virtual int Test(int a) = 0;
        virtual float Test(float f) = 0;
        virtual std::string Test(const std::string& s) = 0;
        virtual Msg Test(const Msg& msg) = 0;
        virtual void CallBack() = 0;
    };

    class Requester
    {
    public:
        Requester(cross_call::IInvoker* invoker) : _invoker(invoker) { }

    public:
        bool Test(bool b);
        int Test(int a);
        float Test(float f);
        std::string Test(const std::string& s);
        Msg Test(const Msg& msg);
        void CallBack();

    protected:
        cross_call::IInvoker* _invoker = nullptr;
    };

    class Processor : public cross_call::IProcessor
    {
    public:
        Processor(IResponder* responder) : _responder(responder) { }
        virtual ~Processor() { }

    public:
        virtual void Process(cross_call::IContext* context);

    protected:
        void OnTest_bool(cross_call::IContext* context);
        void OnTest_int(cross_call::IContext* context);
        void OnTest_float(cross_call::IContext* context);
        void OnTest_string(cross_call::IContext* context);
        void OnTest_Msg(cross_call::IContext* context);
        void OnCallBack(cross_call::IContext* context);

    protected:
        IResponder* _responder = nullptr;
    };
};
