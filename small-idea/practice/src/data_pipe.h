/*
 * 数据管道
 * 用于线程间的数据传递（最优情况是一个发送线程对应一个接收线程）
*/
#pragma once
#include "spin_lock.h"
#include "scoped_buffer.h"
#include "ring_buffer.h"
#include <mutex>

UTILITY_NAMESPACE_BEGIN

template <size_t S, size_t A = sizeof(void*)>
class data_pipe
{
public:
    typedef fixed_ring_buffer<S, A> ring_buffer;
    typedef singly_node<ring_buffer> buffer_node;
    static constexpr size_t block_size = S;

public:
    data_pipe(int max_empty = 10) : _max_empty(max_empty)
    {
        _r_buf = _w_buf = new buffer_node();
    }

    ~data_pipe()
    {
        auto node = _r_buf;
        while (node)
        {
            auto temp = node;
            node = temp->next;

            delete temp;
        }
    }

    data_pipe(const data_pipe&) = delete;
    data_pipe& operator = (const data_pipe&) = delete;

public:
    inline bool empty() const { return _count == 0; }
    inline size_t count() const { return _count; }

    scoped_buffer write(size_t sz)
    {
        if (sz == 0 || sz > block_size)
            return scoped_buffer();

        _w_lock.lock();

        void* data = _w_buf->value.write_begin(sz);
        if (data == nullptr)
        {
            next_node(true);

            data = _w_buf->value.write_begin(sz);
            assert(data);
        }

        return scoped_buffer(&_w_unlocker, data, sz);
    }

    scoped_buffer read()
    {
        if (empty())
            return scoped_buffer();

        _r_lock.lock();

        if (_r_buf->value.empty())
            next_node(false);

        size_t sz = 0;
        void* data = _r_buf->value.read_begin(sz);
        assert(data);

        return scoped_buffer(&_r_unlocker, data, sz);
    }

private:
    void write_end()
    {
        ++_count;
        _w_buf->value.write_end();
        _w_lock.unlock();
    }

    void read_end()
    {
        --_count;
        _r_buf->value.read_end();
        _r_lock.unlock();
    }

    void next_node(bool w_node)
    {
        std::lock_guard<spin_lock> guard(_rw_lock);

        if (w_node)
        {
            if (_w_buf->next = nullptr)
                _w_buf->next = new buffer_node();

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

private:
    struct w_unlocker final : iscoped_deallocator
    {
        w_unlocker(data_pipe* pipe) : _pipe(pipe) {}
        void deallocate(void*, size_t) override { _pipe->write_end(); }

        data_pipe* _pipe;
    };

    struct r_unlocker final : iscoped_deallocator
    {
        r_unlocker(data_pipe* pipe) : _pipe(pipe) {}
        void deallocate(void*, size_t) override { _pipe->read_end(); }

        data_pipe* _pipe;
    };

private:
    int _max_empty;     // max empty node count
    std::atomic<size_t> _count = ATOMIC_VAR_INIT(0);    // data count

    buffer_node* _r_buf = nullptr;
    spin_lock _r_lock;

    buffer_node* _w_buf = nullptr;
    spin_lock _w_lock;

    spin_lock _rw_lock;     // read and write will modify same member data

    w_unlocker _w_unlocker = { this };
    r_unlocker _r_unlocker = { this };
};

UTILITY_NAMESPACE_END
