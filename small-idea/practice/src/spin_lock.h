/*
 * 自旋锁
*/
#pragma once
#include "common.h"
#include <atomic>
#include <thread>
#include <cassert>

UTILITY_NAMESPACE_BEGIN

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
        {
        }
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

UTILITY_NAMESPACE_END
