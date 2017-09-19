/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#pragma once

#include "Cfg.h"
#include "gcf/gcf.h"

class Caller
{
public:
    static const int32_t MODULE_ID = 1;
    static const int32_t HASH_CODE = -1871653667;

    enum class Message
    {
        Invalid,
        Call_A_int_int,
        Call_B_string,
    };

    class IResponder
    {
    public:
        virtual ~IResponder() {}

    public:
        virtual void Call_A(int a, int b) = 0;
        virtual int Call_B(const std::string& str) = 0;
    };

    class Requester
    {
    public:
        Requester(cross_call::IInvoker* invoker) : _invoker(invoker) { }

    public:
        void Call_A(int a, int b);
        int Call_B(const std::string& str);

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
        void OnCall_A_int_int(cross_call::IContext* context);
        void OnCall_B_string(cross_call::IContext* context);

    protected:
        IResponder* _responder = nullptr;
    };
};
