/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#pragma once

#include "Cfg.h"
#include "ICrossCall.h"

class Caller
{
public:
    static const uint32_t MODULE_ID = 0;
    static const uint32_t HASH_CODE = 456984651;

    enum class Message
    {
        Invalid,
        Msg_Call_A,
        Msg_Call_B,
    };

    class IExecutor
    {
    public:
        virtual ~IExecutor() {}

    public:
        virtual void Call_A(int a, int b) = 0;
        virtual int Call_B(const std::string& str) = 0;
    };

    class Invoker
    {
    public:
        Invoker(cross_call::ICross* cross) : _cross(cross) { }

    public:
        void Call_A(int a, int b);
        int Call_B(const std::string& str);

    protected:
        cross_call::ICross* _cross = nullptr;
    };

    class Processor : public cross_call::IProcessor
    {
    public:
        Processor(IExecutor* executor) : _executor(executor) { }
        virtual ~Processor() { }

    public:
        virtual void Process(cross_call::IContext* context);

    protected:
        void OnCall_A(cross_call::IContext* context);
        void OnCall_B(cross_call::IContext* context);

    protected:
        IExecutor* _executor = nullptr;
    };
};
