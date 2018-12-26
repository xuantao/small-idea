﻿#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "Async.h"

/* 线程池 */
static class KGAsyncTaskPool
{
public:
    KGAsyncTaskPool() = default;
    ~KGAsyncTaskPool()
    {
        Destory();
    }

    KGAsyncTaskPool(const KGAsyncTaskPool&) = delete;
    KGAsyncTaskPool& operator = (const KGAsyncTaskPool&) = delete;

public:
    bool Create(size_t threadNum)
    {
        assert(threadNum > 0);
        assert(m_bRunning == false);

        if (m_bRunning)
            return false;

        m_bRunning = true;
        m_Threads.resize(threadNum);
        for (auto& t : m_Threads)
        {
            std::thread tmp(std::bind(&KGAsyncTaskPool::DoThreadWork, this));
            t.swap(tmp);
        }

        return true;
    }

    void Destory()
    {
        if (m_bRunning)
        {
            m_bRunning = false;
            m_Condition.notify_all();

            for (auto& t : m_Threads)
                t.join();
        }

        while (!m_Tasks.empty())
            m_Tasks.pop();
    }

    bool AddTask(AsyncTaskPtr pTask)
    {
        {
            std::lock_guard<std::mutex> cLock(m_Mutex);
            m_Tasks.push(pTask);
        }
        m_Condition.notify_one();
        return true;
    }

    inline bool IsRunning() const { return m_bRunning; }
    inline int GetThreadNum() const { return (int)m_Threads.size(); }

private:
    void DoThreadWork()
    {
        while (m_bRunning)
        {
            AsyncTaskPtr pTask = nullptr;
            if (m_Tasks.empty())
            {
                std::unique_lock<std::mutex> cLock(m_Mutex);
                m_Condition.wait(cLock);

                if (m_bRunning == false)
                    break;
                if (m_Tasks.empty())
                    continue;

                pTask = m_Tasks.front();
                m_Tasks.pop();
            }
            else
            {
                std::lock_guard<std::mutex> cLock(m_Mutex);
                if (m_Tasks.empty())
                    continue;

                pTask = m_Tasks.front();
                m_Tasks.pop();
            }

            pTask->Work();
        }
    }

private:
    bool m_bRunning = false;
    std::mutex m_Mutex;
    std::condition_variable m_Condition;
    std::vector<std::thread> m_Threads;
    std::queue<AsyncTaskPtr> m_Tasks;
} s_TaskPool;

namespace Async
{
    bool Startup(size_t threadNum)
    {
        assert(!s_TaskPool.IsRunning());
        return s_TaskPool.Create(threadNum);
    }

    void Shutdown()
    {
        s_TaskPool.Destory();
    }

    bool IsRunning()
    {
        return s_TaskPool.IsRunning();
    }

    void Run(AsyncTaskPtr pTask)
    {
        assert(pTask);
        assert(s_TaskPool.IsRunning());
        s_TaskPool.AddTask(pTask);
    }
}
