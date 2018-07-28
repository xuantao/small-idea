#include "data_pipe.h"

UTILITY_NAMESPACE_BEGIN

namespace dp_detail
{
    block_buffer::block_buffer(size_t capacity_size)
        : _capacity(capacity_size)
    {
        _pool = new char[capacity_size];
        assert(_pool);
    }

    block_buffer::~block_buffer()
    {
        delete[] _pool;
    }

    void* block_buffer::read_begin(size_t& sz)
    {
        if (empty())
            return nullptr;

        sz = *((size_t*)(_pool + _r_pos));
        if (_capacity - _r_pos < size_bytes || sz == 0)
        {
            _r_pos = 0; // jump to front
            sz = *((size_t*)_pool);
        }

        return _pool + _r_pos + size_bytes;
    }

    void block_buffer::read_end()
    {
        size_t sz = *((size_t*)(_pool + _r_pos));
        _r_pos += align(sz + size_bytes, align_byte);
        -- _count;
    }

    void* block_buffer::write_begin(size_t sz)
    {
        size_t rp = _r_pos;
        if (_w_pos == rp && !empty())
            return nullptr;  // full

        size_t cap = align(sz + size_bytes);
        if (_w_pos < rp)
        {
            if ((rp - _w_pos) > cap) return nullptr;
        }
        else if (_capacity - _w_pos < cap)
        {
            if (rp < cap) return nullptr;

            if (_capacity - _w_pos >= size_bytes)
                *((size_t*)(_pool + _w_pos)) = 0;   // mark
            _w_pos = 0; // jump to front
        }

        char* data = _pool + _w_pos;
        *((size_t*)data) = sz;
        data += size_bytes;
        return data;
    }

    void block_buffer::write_end()
    {
        size_t sz = *((size_t*)(_pool + _w_pos));
        _w_pos += align(sz + size_bytes, align_byte);
        ++ _count;
    }
} // namespace dp_detail

data_pipe::data_pipe(size_t block) : _block(block)
{
    _r_buf = _w_buf = new buf_node(block);
    assert(_r_buf && _w_buf);
}

data_pipe::~data_pipe()
{
    auto node = _r_buf;
    while (node)
    {
        auto temp = node;
        node = temp->next;

        delete temp;
    }

    _r_buf = nullptr;
    _w_buf = nullptr;
}

locked_buffer data_pipe::write(size_t sz)
{
    if (sz == 0 || sz > _block)
        return locked_buffer();

    _w_lock.lock();

    void* data = _w_buf->value.write_begin(sz);

    if (data == nullptr)
    {
        next_node(true);

        data = _w_buf->value.write_begin(sz);
        assert(data);
    }

    return locked_buffer(&_w_unlocker, data, sz);
}

void data_pipe::write_end()
{
    ++_count;
    _w_buf->value.write_end();
    _w_lock.unlock();
}

locked_buffer data_pipe::read()
{
    if (empty())
        return locked_buffer();

    _r_lock.lock();

    if (_r_buf->value.empty())
        next_node(false);

    size_t sz = 0;
    void* data = _r_buf->value.read_begin(sz);
    assert(data);
    
    return locked_buffer(&_r_unlocker, data, sz);
}

void data_pipe::read_end()
{
    --_count;
    _r_buf->value.read_end();
    _r_lock.unlock();
}

void data_pipe::next_node(bool w_node)
{
    std::lock_guard<spin_lock> guard(_rw_lock);

    if (w_node)
    {
        if (_w_buf->next = nullptr)
            _w_buf->next = new buf_node(_block);

        assert(_w_buf->next);
        _w_buf = _w_buf->next;
    }
    else
    {
        assert(_r_buf != _w_buf);

        int empty_count = 0;
        auto tail = _w_buf;
        auto temp = _r_buf;

        _r_buf = _r_buf->next;
        temp->next = nullptr;

        while (tail->next)
        {
            ++ empty_count;
            tail = tail->next;
        }

        if (empty_count > max_empty_node)
            delete temp;
        else
            tail->next = temp;
    }
}

UTILITY_NAMESPACE_END
