/*
 * 自旋锁
*/
#pragma once
#include "common.h"
#include <atomic>
#include <thread>
#include <cassert>

UTILITY_NAMESPACE_BEGIN

class SpinLock
{
public:
    inline void lock()
    {
#ifdef _DEBUG
        auto id = std::this_thread::get_id();
        assert(thread_id_ != id);
        thread_id_ = id;
#endif // _DEBUG

        while (locked_.test_and_set(std::memory_order_acquire))
        {
        }
    }

    inline void unlock()
    {
#ifdef _DEBUG
        thread_id_ = std::thread::id();
#endif // _DEBUG

        locked_.clear(std::memory_order_release);
    }

private:
    std::atomic_flag locked_ = ATOMIC_FLAG_INIT;

#ifdef _DEBUG
    std::thread::id thread_id_;
#endif // _DEBUG
};

UTILITY_NAMESPACE_END
