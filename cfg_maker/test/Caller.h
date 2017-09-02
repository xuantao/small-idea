/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#pragma once

#include "Cfg.h"
#include "CrossCallDef.h"

class Caller
{
public:
    static const uint32_t MODULE_ID = 1;
    static const uint32_t HASH_CODE = 0;

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
        Invoker(cross_call::ICrossCaller* caller) : _caller(caller)
        { }

    public:
        void Call_A(int a, int b);
        int Call_B(const std::string& str);
    protected:
        cross_call::ICrossCaller* _caller = nullptr;
    };

    class Processor : public cross_call::IProcessor
    {
    public:
        Processor(IExecutor* executor) : _executor(executor)
        { }
        virtual ~Processor() { }

    public:
        virtual uint32_t GetModuleID() const { return MODULE_ID; }
        virtual uint32_t GetHashCode() const { return HASH_CODE; }
        virtual void Process(serialize::IReader* reader, serialize::IWriter* writer);

    protected:
        void OnCall_A(serialize::IReader* reader, serialize::IWriter* writer);
        void OnCall_B(serialize::IReader* reader, serialize::IWriter* writer);
    protected:
        IExecutor* _executor = nullptr;
    };
};
