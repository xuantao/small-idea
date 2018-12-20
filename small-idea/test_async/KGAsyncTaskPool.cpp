#include "KGAsyncTaskPool.h"
#include <cassert>

KGAsyncTaskPool::~KGAsyncTaskPool()
{
    if (m_bRunning)
        Destory();
}

bool KGAsyncTaskPool::Create(int threadNum)
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

void KGAsyncTaskPool::Destory()
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

bool KGAsyncTaskPool::AddTask(IKGAsyncTask* pTask)
{
    std::lock_guard<std::mutex> cLock(m_Mutex);
    m_Tasks.push(pTask);
    return true;
}

void KGAsyncTaskPool::DoThreadWork()
{
    while (true)
    {
        IKGAsyncTask* pTask = nullptr;
        {
            std::unique_lock<std::mutex> cLock(m_Mutex);
            m_Condition.wait_for(cLock, std::chrono::milliseconds(5));

            if (m_bRunning == false)
            {
                break;
            }
            else if (m_Tasks.empty())
            {
                continue;
            }
            else
            {
                pTask = m_Tasks.front();
                m_Tasks.pop();
            }
        }

        assert(pTask);
        pTask->Work();
        delete pTask;
    }
}
