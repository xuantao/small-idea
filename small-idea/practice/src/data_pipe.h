/*
 * 数据管道
 * 用于线程间的数据传递
*/
#pragma once
#include "spin_lock.h"
#include "locked_buffer.h"

UTILITY_NAMESPACE_BEGIN

namespace dp_detail
{
    class block_buffer
    {
    public:
        static const size_t align_byte = sizeof(void*);
        static const size_t size_bytes = sizeof(size_t);

    public:
        block_buffer(size_t capacity_size);
        ~block_buffer();

        block_buffer(const block_buffer&) = delete;
        block_buffer& operator = (const block_buffer&) = delete;

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
} // namespace dp_detail

class data_pipe
{
    typedef singly_node<dp_detail::block_buffer> buf_node;
    friend class locked_buffer;
    static const int max_empty_node = 4;

public:
    data_pipe(size_t block);
    ~data_pipe();

    data_pipe(const data_pipe&) = delete;
    data_pipe& operator = (const data_pipe&) = delete;

public:
    inline bool empty() const { return _count == 0; }
    inline size_t count() const { return _count; }
    inline size_t block_size() const { return _block; }

    locked_buffer write(size_t sz);
    locked_buffer read();

private:
    void write_end();
    void read_end();

    void next_node(bool w_node);

private:
    struct w_unlocker final : ibuffer_unlocker
    {
        void unlock(void*, size_t) override { _pipe->write_end(); }
        data_pipe* _pipe;
    }

    struct r_unlocker final : ibuffer_unlocker
    {
        void unlock(void*, size_t) override { _pipe->read_end(); }
        data_pipe* _pipe;
    }

private:
    size_t _block;  // each block buffer size
    std::atomic<size_t> _count = ATOMIC_VAR_INIT(0);    // data count

    buf_node* _r_buf = nullptr;
    spin_lock _r_lock;

    buf_node* _w_buf = nullptr;
    spin_lock _w_lock;

    spin_lock _rw_lock;     // read and write will modify same member data

    w_unlocker _w_unlocker = { this };
    r_unlocker _r_unlocker = { this };
};

UTILITY_NAMESPACE_END
