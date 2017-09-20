/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#pragma once

#include "Cfg.h"
#include "gcf/gcf.h"
#include <memory>

class TestC2S
{
public:
    static const int32_t MODULE_ID = 2;
    static const int32_t HASH_CODE = -899050904;

    enum class Message
    {
        Invalid,
        Test_bool,
        Test_byte,
        Test_int,
        Test_long,
        Test_float,
        Test_double,
        Test_string,
        Test_bool_A,
        Test_byte_A2,
        Test_int_A_A,
        Test_long_A2_A,
        Test_float_A_A2,
        Test,
        Test_Msg,
        Test_Msg_A2,
        Test_int_int,
        Test_int_int_int,
        Test_int_int_int_int,
    };

    class IResponder
    {
    public:
        virtual ~IResponder() {}

    public:
        virtual bool Test(bool v) = 0;
        virtual int8_t Test(int8_t v) = 0;
        virtual int Test(int v) = 0;
        virtual int64_t Test(int64_t v) = 0;
        virtual float Test(float v) = 0;
        virtual double Test(double v) = 0;
        virtual std::string Test(const std::string& v) = 0;
        virtual void Test(const std::vector<bool>& v) = 0;
        virtual void Test(const std::array<int8_t, 2>& v) = 0;
        virtual void Test(const std::vector<std::vector<int> >& v) = 0;
        virtual void Test(const std::vector<std::array<int64_t, 2> >& v) = 0;
        virtual void Test(const std::array<std::vector<float>, 2>& v) = 0;
        virtual void Test() = 0;
        virtual void Test(const Msg& msg) = 0;
        virtual void Test(const std::array<Msg, 2>& msg) = 0;
        virtual Msg Test(int a, int b) = 0;
        virtual std::vector<Msg> Test(int a, int b, int c) = 0;
        virtual std::array<Msg, 2> Test(int a, int b, int c, int d) = 0;
    };

    class Requester
    {
    public:
        Requester(cross_call::IInvoker* invoker) : _invoker(invoker) { }

    public:
        bool Test(bool v);
        int8_t Test(int8_t v);
        int Test(int v);
        int64_t Test(int64_t v);
        float Test(float v);
        double Test(double v);
        std::string Test(const std::string& v);
        void Test(const std::vector<bool>& v);
        void Test(const std::array<int8_t, 2>& v);
        void Test(const std::vector<std::vector<int> >& v);
        void Test(const std::vector<std::array<int64_t, 2> >& v);
        void Test(const std::array<std::vector<float>, 2>& v);
        void Test();
        void Test(const Msg& msg);
        void Test(const std::array<Msg, 2>& msg);
        Msg Test(int a, int b);
        std::vector<Msg> Test(int a, int b, int c);
        std::array<Msg, 2> Test(int a, int b, int c, int d);

    protected:
        cross_call::IInvoker* _invoker = nullptr;
    };

    class Processor : public cross_call::IProcessor
    {
    public:
        Processor(std::shared_ptr<IResponder> responder) : _responder(responder) { }
        virtual ~Processor() { }

    public:
        virtual int ID() const { return MODULE_ID; }
        virtual void Process(cross_call::IContext* context);

    protected:
        void OnTest_bool(cross_call::IContext* context);
        void OnTest_byte(cross_call::IContext* context);
        void OnTest_int(cross_call::IContext* context);
        void OnTest_long(cross_call::IContext* context);
        void OnTest_float(cross_call::IContext* context);
        void OnTest_double(cross_call::IContext* context);
        void OnTest_string(cross_call::IContext* context);
        void OnTest_bool_A(cross_call::IContext* context);
        void OnTest_byte_A2(cross_call::IContext* context);
        void OnTest_int_A_A(cross_call::IContext* context);
        void OnTest_long_A2_A(cross_call::IContext* context);
        void OnTest_float_A_A2(cross_call::IContext* context);
        void OnTest(cross_call::IContext* context);
        void OnTest_Msg(cross_call::IContext* context);
        void OnTest_Msg_A2(cross_call::IContext* context);
        void OnTest_int_int(cross_call::IContext* context);
        void OnTest_int_int_int(cross_call::IContext* context);
        void OnTest_int_int_int_int(cross_call::IContext* context);

    protected:
        std::shared_ptr<IResponder> _responder;
    };
};
