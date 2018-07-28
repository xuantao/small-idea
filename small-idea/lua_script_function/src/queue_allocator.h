#pragma once
#include <atomic>
#include <thread>
#include <cassert>

class spin_lock
{
public:
    inline void lock()
    {
#ifdef _DEBUG
        auto id = std::this_thread::get_id();
        assert(_thread_id != id);
        _thread_id = id;
#endif // _DEBUG

        while (_locked.test_and_set(std::memory_order_acquire))
        { }
    }

    inline void unlock()
    {
#ifdef _DEBUG
        _thread_id = std::thread::id();
#endif // _DEBUG
        _locked.clear(std::memory_order_release);
    }

private:
    std::atomic_flag _locked = ATOMIC_FLAG_INIT;

#ifdef _DEBUG
    std::thread::id _thread_id;
#endif // _DEBUG
};

class queue_allocator;

class block_buffer
{
    friend class queue_allocator;
    block_buffer(const block_buffer& other) = delete;
    block_buffer& operator = (const block_buffer& other) = delete;

private:
    block_buffer() : _allocator(nullptr) { }

    block_buffer(queue_allocator* allocator, void* buf, size_t sz, bool w)
        : _allocator(allocator)
        , _buff(buf)
        , _size(sz)
        , _write(w)
    { }

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

class queue_buffer
{
public:
    static const size_t align_byte = sizeof(void*);
    static const size_t size_bytes = sizeof(size_t);

public:
    queue_buffer(size_t capacity_size);
    ~queue_buffer();

    queue_buffer(const queue_buffer&) = delete;
    queue_buffer& operator = (const queue_buffer&) = delete;

public:
    inline bool empty() const { return _count == 0; }
    inline size_t count() const { return _count; }
    inline size_t capacity() const { return _capacity; }

    void* read_begin(size_t& sz);
    void read_end();

    void* write_begin(size_t sz);
    void write_end();

private:
    size_t _capacity;
    size_t _w_pos = 0;
    size_t _r_pos = 0;
    size_t _count = 0;
    char* _pool = nullptr;
};

template <typename Ty>
struct singly_node
{
    typedef singly_node self_type;
    typedef Ty value_type;

    singly_node() { }

    template <typename... Args>
    singly_node(Args&&... args) : value(std::forward<Args>(args)...)
    { }

    self_type* next = nullptr;
    value_type value;
};

class queue_allocator
{
    typedef singly_node<queue_buffer> buf_node;
    friend class block_buffer;
    static const int max_empty_node = 4;

public:
    queue_allocator(size_t block);
    ~queue_allocator();

    queue_allocator(const queue_allocator&) = delete;
    queue_allocator& operator = (const queue_allocator&) = delete;

public:
    inline bool empty() const { return _count == 0; }
    inline size_t count() const { return _count; }
    inline size_t block_size() const { return _block; }

    block_buffer write(size_t sz);
    block_buffer read();

private:
    void write_end();
    void read_end();

    void next_node(bool w_node);

private:
    size_t _block;  // each block buffer size
    std::atomic<size_t> _count = ATOMIC_VAR_INIT(0);    // data count

    buf_node* _r_buf = nullptr;
    spin_lock _r_lock;

    buf_node* _w_buf = nullptr;
    spin_lock _w_lock;

    spin_lock _rw_lock;     // read and write will modify same member data
};
