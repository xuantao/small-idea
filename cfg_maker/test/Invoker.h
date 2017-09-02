/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#pragma once

#include "Cfg.h"

class Invoker
{
public:
    static const uint32_t MODULE_ID = 1;
    static const uint32_t HASH_CODE = 0;

    enum class Message
    {
        Invalid,
        Call_A,
        Call_B,
    };

    class IExecutor
    {
    public:
        virtual IExecutor() {}

    public:
        virtual void Call_A(int a, int b) = 0;
        virtual int Call_B(const std::string& str) = 0;
    };

    class Invoker
    {
    public:
        Invoker(ICrossCaller* caller) : _caller(caller)
        { }

    public:
        void Call_A(int a, int b);
        int Call_B(const std::string& str);
    protected:
        ICrossCaller* _caller = nullptr;
    };

    class Processor : public IProcessor
    {
    public:
        Processor(IExecutor* executor) : _executor(executor)
        { }
        virtual ~Processor() { }

    public:
        virtual int GetModuleID() const { return MODULE_ID; }
        virtual int GetHashCode() const { return HASH_CODE; }
        virtual void Process(IReader* reader, IWriter* writer)

    protected:
        void OnCall_A(IReader* reader, IWriter* writer);
        void OnCall_B(IReader* reader, IWriter* writer);
    protected:
        IExecutor* _executor = nullptr;
    };
};