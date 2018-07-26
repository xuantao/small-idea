#include "ring_buffer.h"

Block::~Block()
{
    if (_buffer)
    {
        if (_push)
            ++_buffer->_pushNum;
        else
            ++_buffer->_popNum;

        _buffer = nullptr;
    }
}
