#pragma once
#include <atomic>
#include <vector>

class SpinLock {
public:
    void lock()
    {
        while (_locked.test_and_set(std::memory_order_acquire))
        {
        }
    }

    void unlock()
    {
        _locked.clear(std::memory_order_release);
    }

private:
    std::atomic_flag _locked = ATOMIC_FLAG_INIT;
};

class queue_allocator;

class block_buffer
{
    friend class queue_allocator;
    block_buffer(const block_buffer& other) = delete;
    block_buffer& operator = (const block_buffer& other) = delete;

private:
    block_buffer() : _allocator(nullptr) { }
    block_buffer(queue_allocator* allocator, void* buf, size_t sz, bool write);

public:
    block_buffer(block_buffer&& other)
        : _allocator(other._allocator)
        , _buff(other._buff)
        , _size(other._size)
        , _write(other._write)
    {
        other._allocator = nullptr;
    }

    ~block_buffer();

public:
    inline void* get() const { return _buff; }
    inline size_t size() const { return _size; }
    inline explicit operator bool() const { return _allocator != nullptr; }

private:
    queue_allocator* _allocator;
    void* _buff;
    size_t _size;
    bool _write;
};

class queue_allocator
{
public:
    friend class block_buffer;
    queue_allocator(const queue_allocator&) = delete;
    queue_allocator& operator = (const queue_allocator&) = delete;

    static const size_t size_bytes = sizeof(size_t);

public:
    queue_allocator(size_t cap);
    ~queue_allocator();

public:
    inline bool empty() const { return _count == 0; }
    inline size_t count() const { return _count; }
    inline size_t capacity() const { return _capacity; }

    block_buffer push(size_t sz);
    block_buffer pop();

private:
    void lock(bool write);
    void unlock(size_t sz, bool write);

private:
    size_t _capacity;

    bool _wlock = false;
    size_t _wpos = 0;

    bool _rlock = false;
    size_t _rpos = 0;

    int _count = 0;
    char* _pool = nullptr;

    //std::atomic<size_t> _wpos = ATOMIC_VAR_INIT(0);
    //std::atomic<size_t> _rpos = ATOMIC_VAR_INIT(0);
    //std::atomic_long _count = ATOMIC_VAR_INIT(0);
};
