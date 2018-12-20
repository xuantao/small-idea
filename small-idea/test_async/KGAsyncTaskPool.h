/*
 * “Ï≤Ω»ŒŒÒ≥ÿ
*/
#pragma once
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "KGAsyncTask.h"

class KGAsyncTaskPool
{
public:
    KGAsyncTaskPool() = default;
    ~KGAsyncTaskPool();

    KGAsyncTaskPool(const KGAsyncTaskPool&) = delete;
    KGAsyncTaskPool& operator = (const KGAsyncTaskPool&) = delete;

public:
    bool Create(int threadNum);
    void Destory();
    bool AddTask(IKGAsyncTask* pTask);
    inline int GetThreadNum() const { return (int)m_Threads.size(); }

private:
    void DoThreadWork();

private:
    bool m_bRunning = false;
    std::mutex m_Mutex;
    std::condition_variable m_Condition;
    std::vector<std::thread> m_Threads;
    std::queue<IKGAsyncTask*> m_Tasks;
};
