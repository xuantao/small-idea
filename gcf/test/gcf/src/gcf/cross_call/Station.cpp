#include "Station.h"

namespace cross_call
{
    Station::Station()
        : _context(*this)
        , _cross(*this)
    {
    }

    Station::~Station()
    {
    }

    bool Station::Init(char* buffer, int size)
    {
        _buffer = new SwapBuffer(buffer, size);
        _reader = new Reader(_buffer);
        _writer = new Writer(_buffer);
        return true;
    }

    void Station::UnInit()
    {
        _procs.clear();
        delete _reader;
        delete _writer;
        delete _buffer;
    }

    bool Station::Register(ProcessorPtr processor)
    {
        return _procs.insert(std::make_pair(processor->ID(), processor)).second;
    }

    ProcessorPtr Station::Unregister(int32_t module)
    {
        ProcessorPtr proccessor = GetProcessor(module);
        if (proccessor)
            _procs.erase(module);
        return proccessor;
    }

    ProcessorPtr Station::GetProcessor(int32_t module) const
    {
        const auto it = _procs.find(module);
        if (it == _procs.cend())
            return nullptr;
        return it->second;
    }

    bool Station::OnCall()
    {
        int32_t module = 0;
        ProcessorPtr proc;

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
        return true;
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

        DoCall();

        _buffer->Startup(BufferMode::Read);
        return _reader;
    }
}
