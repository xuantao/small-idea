#include "ring_buffer.h"

Block::~Block()
{
    if (_buffer)
    {
        if (_push)
            ++_buffer->_num;
        else
            ++_buffer->_num;

        _buffer = nullptr;
    }
}

Block RingBuffer::Push(size_t size)
{
    std::lock_guard guard(_pushLock);
    size_t rp = _read;
    
    if (_write < rp)
    {
        if ((rp - _write) <= size)
            return Block();
    }
    else 
    {
        if (_blockSize - _write < size)
            return Block();
    }
    
    void* data = _pool + _write;
    guard += size;
    return Block(this, data, size, true);
}

Block RingBuffer::Pop()
{
}