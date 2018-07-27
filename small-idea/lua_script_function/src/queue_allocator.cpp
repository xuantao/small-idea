#include "queue_allocator.h"

static size_t s_align(size_t sz, size_t bound = sizeof(void*))
{
    if ((sz & (bound-1)))
        return (sz & (~(bound-1))) + bound;
    return sz;
}

block_buffer::block_buffer(queue_allocator* allocator, void* buf, size_t sz, bool write)
    : _allocator(allocator)
    , _buff(buf)
    , _size(sz)
    , _write(write)
{
    _allocator->lock(_write);
}

block_buffer::~block_buffer()
{
    if (_allocator)
    {
        _allocator->unlock(_size, _write);
        _allocator = nullptr;
    }
}

queue_allocator::queue_allocator(size_t cap) : _capacity(cap)
{
    _pool = new char[cap];
}

queue_allocator::~queue_allocator()
{
    delete[] _pool;
    _pool = nullptr;
}

block_buffer queue_allocator::push(size_t sz)
{
    if (_wlock) return block_buffer();

    size_t rp = _rpos;
    size_t wp = _wpos;
    size_t cap = s_align(sz + size_bytes);

    if (wp == rp && !empty())
        return block_buffer();  // full

    if (wp < rp)
    {
        if ((rp - wp) > cap) return block_buffer();
    }
    else if (_capacity - wp < cap)
    {
        if (rp < cap) return block_buffer();

        if (_capacity - wp >= size_bytes)
            *((size_t*)(_pool + wp)) = 0;   // mark
        _wpos = wp = 0;    // jump to front
    }

    char* data = _pool + wp;
    *((size_t*)data) = sz;
    data += size_bytes;

    return block_buffer(this, data, sz, true);
}

block_buffer queue_allocator::pop()
{
    if (empty())
        return block_buffer();

    size_t rp = _rpos;
    size_t sz = *((size_t*)(_pool + rp));

    if (_capacity - rp < size_bytes || sz == 0)
    {
        _rpos = rp = 0;
        sz = *((size_t*)_pool);
    }

    return block_buffer(this, _pool + size_bytes, sz, false);
}

void queue_allocator::lock(bool write)
{
    if (write)
        _wlock = true;
    else
        _rlock = true;
}

void queue_allocator::unlock(size_t sz, bool write)
{
    if (write)
    {
        _wpos += s_align(sz + size_bytes);
        _wlock = false;
        ++_count;
    }
    else
    {
        _rpos += s_align(sz + size_bytes);
        _rlock = false;
        --_count;
    }
}
