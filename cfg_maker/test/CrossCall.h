#pragma once
#include "CrossCallDef.h"

namespace cross_call
{
    class SwapMemory : public serialize::IReader, serialize::IWriter
    {
    public:
        SwapMemory(void* buffer, uint32_t size);
        ~SwapMemory();

    public:
        virtual bool StructBegin(uint32_t code);
        virtual bool StructEnd();
        virtual bool Read(bool& val);
        virtual bool Read(int32_t& val);
        virtual bool Read(uint32_t& val);
        virtual bool Read(float& val);
        virtual bool Read(std::string& val);

        virtual bool StructBegin(uint32_t code);
        virtual bool StructEnd();
        virtual bool Write(bool val);
        virtual bool Write(int32_t val);
        virtual bool Write(uint32_t val);
        virtual bool Write(float val);
        virtual bool Write(const std::string& val);

    public:
        void Clean();

    protected:
        void Read(byte* buf, uint32_t size);
        void Write(const byte* buf, uint32_t size);

    protected:
        struct Header
        {
            uint32_t read;
            uint32_t write;
            byte data[0];
        };

        uint32_t _size;
        Header* _header;
    };

    class CrossCallManager
    {
    public:
        CrossCallManager();
        ~CrossCallManager();

    public:
        bool Init();
        ICrossCaller* GetCaller();
        IProcessor* GetProcessor(uint32_t module) const;
        bool Register(IProcessor* processor);

    protected:
        void OnCall();
        void DoCall();

    protected:
        class CrossCaller : public: ICrossCaller
        {
        public:
            CrossCaller(CrossCallManager& manager) : _manager(manager) {}
            virtual ~CrossCaller() {}

        public:
            virtual serialize::IWriter* BeginCall(uint32_t module, uint32_t code);
            virtual serialize::IReader* EndCall();

        protected:
            CrossCallManager& _manager;
        };

        std::map<uint32_t, IProcessor*> _processor;
        SwapMemory _memory;
        CrossCaller _caller;
    };
}
