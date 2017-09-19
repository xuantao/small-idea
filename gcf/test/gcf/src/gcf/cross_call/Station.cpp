#include "Station.h"

namespace cross_call
{
    Station::Station(ICrossCall* caller, char* buffer, int size)
        : _caller(caller)
        , _context(*this)
        , _cross(*this)
    {
        _buffer = new SwapBuffer(buffer, size);
        _reader = new BinaryReader(_buffer);
        _writer = new BinaryWriter(_buffer);
    }

    Station::~Station()
    {
        delete _reader;
        delete _writer;
        delete _buffer;
    }

    bool Station::Register(int32_t module, IProcessor* processor)
    {
        return _procs.insert(std::make_pair(module, processor)).second;
    }

    IProcessor* Station::Unregister(int32_t module)
    {
        IProcessor* proccessor = GetProcessor(module);
        if (proccessor)
            _procs.erase(module);
        return proccessor;
    }

    IProcessor* Station::GetProcessor(int32_t module) const
    {
        const auto it = _procs.find(module);
        if (it == _procs.cend())
            return nullptr;
        return it->second;
    }

    void Station::OnCall()
    {
        int32_t module = 0;
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

    serialize::IWriter* Station::BeginCall(int32_t module)
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
