#include "CrossCall.h"
#include <cassert>

namespace cross_call
{
    //////////////////////////////////////////////////////////////////////////
    // SwapMemory
    SwapMemory::SwapMemory(void* buffer, uint32_t size)
        : _header((SwapMemory::Header*)buffer)
        , _size(size - sizeof(SwapMemory::Header))
        , _reader(this)
        , _writer(this)
    {
        assert(size > sizeof(SwapMemory::Header));
        Clear();
    }

    SwapMemory::~SwapMemory()
    { }

    uint32_t SwapMemory::GetDataSize() const
    {
        return _header->ds;
    }

    void SwapMemory::Clear()
    {
        _header->rp = _header->wp = _header->ds = 0;
    }

    bool SwapMemory::Read(void* buf, uint32_t size)
    {
        if (GetDataSize() < size)
            return false;

        char* dst = (char*)buf;
        uint32_t right = _size - _header->rp;
        assert(right <= _size);

        if (right < size)
        {
            memcpy(dst, _header->m + _header->rp, right);
            _header->rp = 0;
            _header->ds -= right;
            size -= right;
            dst += right;
        }

        memcpy(dst, _header->m + _header->rp, size);
        _header->rp += size;
        _header->ds -= size;
        return true;
    }

    bool SwapMemory::Write(const void* buf, uint32_t size)
    {
        if (GetFreeSize() < size)
            return false;

        const char* src = (const char*)buf;
        uint32_t right = _size - _header->wp;
        assert(right <= _size);

        if (right < size)
        {
            memcpy(_header->m + _header->wp, src, right);
            _header->wp = 0;
            _header->ds += right;
            size -= right;
            src += right;
        }

        memcpy(_header->m + _header->wp, src, size);
        _header->wp += size;
        _header->ds += size;
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    // CrossCallManager
    CrossCallManager::CrossCallManager()
    { }

    CrossCallManager::~CrossCallManager()
    {
        delete _caller;
        delete _memory;
        delete _buffer;
    }

    bool CrossCallManager::Init()
    {
        _buffer = new char[1024];
        _memory = new SwapMemory(_buffer, 1024);
        _caller = new CrossCaller(*this);
        return true;
    }

    IProcessor* CrossCallManager::GetProcessor(uint32_t module) const
    {
        auto it = _processor.find(module);
        if (it == _processor.cend())
            return nullptr;

        return it->second;
    }

    bool CrossCallManager::Register(IProcessor* processor)
    {
        if (processor == nullptr)
            return false;
        if (GetProcessor(processor->GetModuleID()) != nullptr)
            return false;

        _processor.insert(std::make_pair(processor->GetModuleID(), processor));
        return true;
    }

    IProcessor* CrossCallManager::Unregister(uint32_t module)
    {
        IProcessor* proc = GetProcessor(module);
        if (proc == nullptr)
            return nullptr;

        _processor.erase(proc->GetModuleID());
        return proc;
    }

    serialize::IWriter* CrossCallManager::BeginCall(uint32_t module, uint32_t code)
    {
        auto writer = _memory->GetWriter();
        writer->Write(module);
        writer->Write(code);
        return writer;
    }

    serialize::IReader* CrossCallManager::EndCall()
    {
        auto reader = _memory->GetReader();
        uint32_t module;
        uint32_t code;
        reader->Read(module);
        reader->Read(code);

        auto proc = GetProcessor(module);
        if (proc == nullptr)
            return nullptr;

        assert(proc->GetHashCode() == code);
        proc->Process(reader, _memory->GetWriter());
        return reader;
    }
}
