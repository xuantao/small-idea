#pragma once
#include "CrossCallDef.h"
#include "SerializeNormal.h"
#include <map>

namespace cross_call
{
    class SwapMemory : public serialize::IBuffer
    {
    public:
        SwapMemory(void* buffer, uint32_t size);
        ~SwapMemory();

    public:
        serialize::IReader* GetReader() { return &_reader; }
        serialize::IWriter* GetWriter() { return &_writer; }
        uint32_t GetDataSize() const;
        uint32_t GetFreeSize() const { return _size - GetDataSize(); }
        void Clear();

    public:
        virtual bool Read(void* buf, uint32_t size);
        virtual bool Write(const void* buf, uint32_t size);

    protected:
        struct Header
        {
            uint32_t ds;
            uint32_t rp;
            uint32_t wp;
            char m[0];
        };

        uint32_t _size = 0;
        Header* _header = nullptr;
        serialize::NormalReader _reader;
        serialize::NormalWriter _writer;
    };

    class CrossCallManager
    {
    public:
        CrossCallManager();
        ~CrossCallManager();

    public:
        bool Init();
        ICrossCaller* GetCaller() { return _caller; }
        IProcessor* GetProcessor(uint32_t module) const;
        bool Register(IProcessor* processor);
        IProcessor* Unregister(uint32_t module);

    protected:
        serialize::IWriter* BeginCall(uint32_t module, uint32_t code);
        serialize::IReader* EndCall();

    protected:
        class CrossCaller : public ICrossCaller
        {
        public:
            CrossCaller(CrossCallManager& manager) : _manager(manager) {}
            virtual ~CrossCaller() {}

        public:
            virtual serialize::IWriter* BeginCall(uint32_t module, uint32_t code)
            { return _manager.BeginCall(module, code); }
            virtual serialize::IReader* EndCall()
            { return _manager.EndCall(); }

        protected:
            CrossCallManager& _manager;
        };

        std::map<uint32_t, IProcessor*> _processor;

        char* _buffer = nullptr;
        SwapMemory* _memory = nullptr;
        CrossCaller* _caller = nullptr;
    };
}
