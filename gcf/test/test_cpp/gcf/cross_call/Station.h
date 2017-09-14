#pragma once
#include "ICrossCall.h"
#include "Binary.h"
#include "SwapBuffer.h"
#include <map>

namespace cross_call
{
    class Station
    {
    protected:
        class Context : public IContext
        {
        public:
            Context(Station& station) : _staion(station) {}
        public:
            virtual serialize::IReader* Param() { return _staion._reader; }
            virtual serialize::IWriter* Ret() { return _staion.RetParam(); }
        protected:
            Station& _staion;
        };

        class Cross : public IInvoker
        {
        public:
            Cross(Station& station) : _staion(station) {}
        public:
            virtual serialize::IWriter* Begin(uint32_t module) { return _staion.BeginCall(module); }
            virtual serialize::IReader* End() { return _staion.EndCall(); }
        protected:
            Station& _staion;
        };

    public:
        Station(ICrossCall* caller, char* buffer, int size);
        ~Station();

    public:
        IInvoker* Invoker() { return &_cross; }

        bool Register(uint32_t module, IProcessor* processor);
        IProcessor* Unregister(uint32_t module);
        IProcessor* GetProcessor(uint32_t module) const;

        void OnCall();

    protected:
        serialize::IWriter* RetParam();
        serialize::IWriter* BeginCall(uint32_t module);
        serialize::IReader* EndCall();

    protected:
        ICrossCall* _caller = nullptr;
        Context _context;
        Cross _cross;
        std::map<uint32_t, IProcessor*> _procs;

        SwapBuffer* _buffer = nullptr;
        BinaryReader* _reader = nullptr;
        BinaryWriter* _writer = nullptr;
    };
}
