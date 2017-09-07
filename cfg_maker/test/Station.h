#pragma once
#include "CrossCallDef.h"
#include "SerializeNormal.h"
#include <map>

namespace cross_call
{
    enum class BufferMode
    {
        Read,
        Write,
    };

    class SwapBuffer : public serialize::IBinaryStream
    {
    public:
        SwapBuffer(char* buffer, int size);

    public:
        BufferMode Mode() const { return _mode; }
        void Startup(BufferMode mode);
        int Endup();

    public:
        virtual bool Read(void* buf, uint32_t size);
        virtual bool Write(const void* buf, uint32_t size);

    protected:
        union Header
        {
            int32_t  ds;
            char*    base;
        };

        BufferMode _mode = BufferMode::Read;
        Header* _header = nullptr;
        int _size = 0;
        int _rp = 0;
        int _wp = 0;
    };

    class Station
    {
    protected:
        class Context : public IContext
        {
        public:
            Context(Station& station) : _staion(station) {}
        public:
            virtual serialize::IReader* Param() { return _staion._reader; }
            virtual serialize::IWriter* Ret() { Ret() _staion.RetParam(); }
        protected:
            Station& _staion;
        };

        class Cross : public ICross
        {
        public:
            Cross(Station& station) : _staion(station) {}
        public:
            virtual serialize::IWriter* BeginCall(uint32_t module) { _staion.BeginCall(module); }
            virtual serialize::IReader* EndCall() { _staion.EndCall(); }
        protected:
            Station& _staion;
        };

    public:
        Station(ICaller* caller, char* buffer, int size);
        ~Station();

    public:
        ICross* GetCross() { return &_cross; }

        bool Register(IProcessor* processor);
        IProcessor* Unregister(uint32_t module);
        IProcessor* GetProcessor(uint32_t module) const;

        void OnCall();

    protected:
        serialize::IWriter* RetParam();
        serialize::IReader* BeginCall(uint32_t module);
        serialize::IWriter* EndCall();

    protected:
        ICaller* _caller = nullptr;
        Context _context;
        Cross _cross;
        std::map<uint32_t, IProcessor*> _procs;

        SwapBuffer _buffer;
        serialize::BinaryReader* _reader = nullptr;
        serialize::BinaryWriter* _writer = nullptr;
    };
}
