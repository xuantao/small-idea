#pragma once
#include "ISerialize.h"

namespace serialize
{
    class MemoryReadOnly : public IBinaryReadStream
    {
    public:
        MemoryReadOnly();
        ~MemoryReadOnly();
        MemoryReadOnly(const MemoryReadOnly&) = delete;
        MemoryReadOnly& operator = (const MemoryReadOnly&) = delete;

    public:
        bool Init(const void* buff, size_t size);
        void UnInit();
        void Reset();

    public:
        virtual bool Read(void* buf, int32_t size);

    protected:
        size_t _pos = 0;
        size_t _size = 0;
        const int8_t* _buff = nullptr;
    };

    class MemoryWriteOnly : public IBinaryWriteStream
    {
    public:
        MemoryWriteOnly();
        ~MemoryWriteOnly();
        MemoryWriteOnly(const MemoryWriteOnly&) = delete;
        MemoryWriteOnly& operator = (const MemoryWriteOnly&) = delete;

    public:
        void* Data() { return _buff; }
        size_t Size() { return _pos; }

    public:
        virtual bool Read(void* buf, int32_t size);
        virtual bool Write(const void* buf, int32_t size);

    protected:
        size_t _pos = 0;
        size_t _size = 0;
        int8_t* _buff = nullptr;
    };
}
