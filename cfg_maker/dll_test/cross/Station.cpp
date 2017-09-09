#include "Station.h"

namespace cross_call
{
    SwapBuffer::SwapBuffer(char* buffer, int size)
        : _header((SwapBuffer::Header*)buffer)
        , _base((char*)buffer)
        , _size(size)
    {
    }

    void SwapBuffer::Startup(BufferMode mode)
    {
        _mode = mode;
        _rp = _wp = 4;
    }

    int SwapBuffer::Endup()
    {
        if (_mode == BufferMode::Write)
            _header->ds = _wp;
        else
            _header->ds = 0;

        _rp = _wp = 4;
        return _header->ds;
    }

    bool SwapBuffer::Read(void* buf, uint32_t size)
    {
        if (_mode != BufferMode::Read)
            return false;
        if (_rp + (int)size > _header->ds)
            return false;

        memcpy(buf, _base + _rp, size);
        _rp += (int)size;
        return true;
    }

    bool SwapBuffer::Write(const void* buf, uint32_t size)
    {
        if (_mode != BufferMode::Write)
            return false;
        if (_wp + (int)size > _size)
            return false;

        memcpy(_base + _wp, buf, size);
        _wp += (int)size;
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    // Station
    Station::Station(ICrossCall* caller, char* buffer, int size)
        : _caller(caller)
        , _context(*this)
        , _cross(*this)
    {
        _buffer = new SwapBuffer(buffer, size);
        _reader = new serialize::BinaryReader(_buffer);
        _writer = new serialize::BinaryWriter(_buffer);
    }

    Station::~Station()
    {
        delete _reader;
        delete _writer;
        delete _buffer;
    }

    bool Station::Register(uint32_t module, IProcessor* processor)
    {
        return _procs.insert(std::make_pair(module, processor)).second;
    }

    IProcessor* Station::Unregister(uint32_t module)
    {
        IProcessor* proccessor = GetProcessor(module);
        if (proccessor)
            _procs.erase(module);
        return proccessor;
    }

    IProcessor* Station::GetProcessor(uint32_t module) const
    {
        const auto it = _procs.find(module);
        if (it == _procs.cend())
            return nullptr;
        return it->second;
    }

    void Station::OnCall()
    {
        uint32_t module = 0;
        IProcessor* proc = nullptr;

        _buffer->Startup(BufferMode::Read);
        _reader->Read(module);
        proc = GetProcessor(module);

        if (proc)
        {
            proc->Process(&_context);
        }
        else
        {
            //TODO: error
        }

        _buffer->Endup();
    }

    serialize::IWriter* Station::RetParam()
    {
        _buffer->Startup(BufferMode::Write);
        return _writer;
    }

    serialize::IWriter* Station::BeginCall(uint32_t module)
    {
        _buffer->Startup(BufferMode::Write);
        _writer->Write(module);
        return _writer;
    }

    serialize::IReader* Station::EndCall()
    {
        _buffer->Endup();

        _caller->DoCall();
        _buffer->Startup(BufferMode::Read);
        return _reader;
    }
}
