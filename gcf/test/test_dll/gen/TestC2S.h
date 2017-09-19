/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#pragma once

#include "Cfg.h"
#include "gcf/gcf.h"

class TestC2S
{
public:
    static const int32_t MODULE_ID = 2;
    static const int32_t HASH_CODE = -1185566244;

    enum class Message
    {
        Invalid,
        Test_bool,
        Test_int,
        Test_float,
        Test_string,
        Test_Msg,
        Test_int_A10_A,
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
        virtual void Test(const std::vector<std::array<int, 10> >& ar) = 0;
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
        void Test(const std::vector<std::array<int, 10> >& ar);

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
        void OnTest_int_A10_A(cross_call::IContext* context);

    protected:
        IResponder* _responder = nullptr;
    };
};
