﻿#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "KGAsyncTask.h"

/* 线程池 */
static class KGAsyncTaskPool
{
public:
    KGAsyncTaskPool() = default;
    ~KGAsyncTaskPool()
    {
        if (m_bRunning)
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
        if (m_bRunning == false)
            return;

        m_bRunning = false;
        m_Condition.notify_all();

        for (auto& t : m_Threads)
            t.join();

        while (!m_Tasks.empty())
        {
            delete m_Tasks.front();
            m_Tasks.pop();
        }
    }

    bool AddTask(IKGAsyncTask* pTask)
    {
        std::lock_guard<std::mutex> cLock(m_Mutex);
        m_Tasks.push(pTask);
        return true;
    }

    inline int GetThreadNum() const { return (int)m_Threads.size(); }

private:
    void DoThreadWork()
    {
        while (true)
        {
            IKGAsyncTask* pTask = nullptr;
            {
                std::unique_lock<std::mutex> cLock(m_Mutex);
                std::cv_status status = m_Condition.wait_for(cLock, std::chrono::milliseconds(5));
                if (m_bRunning == false)
                    break;
                if (status != std::cv_status::no_timeout)
                    continue;
                if (m_Tasks.empty())
                    continue;

                pTask = m_Tasks.front();
                m_Tasks.pop();
            }

            assert(pTask);
            pTask->Work();
            delete pTask;
        }
    }

private:
    bool m_bRunning = false;
    std::mutex m_Mutex;
    std::condition_variable m_Condition;
    std::vector<std::thread> m_Threads;
    std::queue<IKGAsyncTask*> m_Tasks;
} s_TaskPool;

namespace KGAsync
{
    bool Startup(size_t threadNum)
    {
        assert(s_TaskPool.GetThreadNum() == 0);
        return s_TaskPool.Create(threadNum);
    }

    void Shutdown()
    {
        s_TaskPool.Destory();
    }

    void Run(IKGAsyncTask* pTask)
    {
        assert(pTask);
        assert(s_TaskPool.GetThreadNum());
        s_TaskPool.AddTask(pTask);
    }
}
