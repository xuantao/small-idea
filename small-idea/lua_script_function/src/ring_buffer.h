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

class RingBuffer;

class Block
{
    friend class RingBuffer;
    Block(const Block& other) = delete;
    Block& operator = (const Block& other) = delete;

private:
    Block(RingBuffer* buffer, void* data, size_t size, bool push)
        : _buffer(buffer)
        , _data(data)
        , _size(size)
        , _push(push)
    { }

public:
    Block(Block&& other)
        : _buffer(other._buffer)
        , _data(other._data)
        , _size(other._size)
        , _push(other._push)
    {
        other._buffer = nullptr;
    }

    ~Block();

public:
    inline void* Data() const { return _data; }
    inline size_t Size() const { return _size; }
    inline explicit operator bool() const { return _buffer != nullptr; }

private:
    RingBuffer* _buffer;
    void* _data;
    size_t _size;
    bool _push;
};

class RingBuffer
{
public:
    friend class Block;
    RingBuffer(const RingBuffer&) = delete;
    RingBuffer& operator = (const RingBuffer&) = delete;

    static const size_t max_block_size = 0x7fffffff;

public:
    RingBuffer(size_t block) : _blockSize(block) { }
    ~RingBuffer() { UnInit(); }

public:
    bool Init();
    void UnInit();

    inline bool Empty() const { return _pushNum == _popNum; }
    Block Push(size_t size);
    Block Pop();

private:
    struct Address
    {
        int32_t block;
        int32_t pos;
    };

    struct Header
    {
        int32_t size;
    };

private:
    size_t _blockSize;
    Address _writeAddr;
    Address _readAddr;
    std::vector<int8_t*> _blocks;
    std::atomic_long _pushNum = ATOMIC_VAR_INIT(0);
    std::atomic_long _popNum = ATOMIC_VAR_INIT(0);
    SpinLock _popLock;
    SpinLock _pushLock;
};
