#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "utility/async.h"

UTILITY_NAMESPACE_BEGIN

/* 线程池 */
static class KGAsyncTaskPool
{
public:
    KGAsyncTaskPool() = default;
    ~KGAsyncTaskPool() { Destory(); }

    KGAsyncTaskPool(const KGAsyncTaskPool&) = delete;
    KGAsyncTaskPool& operator = (const KGAsyncTaskPool&) = delete;

public:
    bool Create(size_t threadNum)
    {
        assert(threadNum > 0);
        assert(running_ == false);

        if (running_)
            return false;

        running_ = true;
        threads_.resize(threadNum);
        for (auto& t : threads_)
        {
            std::thread tmp(std::bind(&KGAsyncTaskPool::DoThreadWork, this));
            t.swap(tmp);
        }

        return true;
    }

    void Destory()
    {
        if (running_)
        {
            running_ = false;
            condition_.notify_all();

            for (auto& t : threads_)
                t.join();
        }

        while (!tasks_.empty())
            tasks_.pop();
    }

    bool AddTask(AsyncTaskPtr pTask)
    {
        {
            std::lock_guard<std::mutex> cLock(mutex_);
            tasks_.push(pTask);
        }
        condition_.notify_one();
        return true;
    }

    inline bool IsRunning() const { return running_; }
    inline int GetThreadNum() const { return (int)threads_.size(); }

private:
    AsyncTaskPtr PopTask()
    {
        AsyncTaskPtr task;
        while (!tasks_.empty())
        {
            task = tasks_.front();
            tasks_.pop();

            if (task->IsAbandoned())
                task = nullptr;
            else
                break;
        }

        return task;
    }

    void DoThreadWork()
    {
        while (running_)
        {
            AsyncTaskPtr task = nullptr;
            if (tasks_.empty())
            {
                std::unique_lock<std::mutex> lock(mutex_);
                condition_.wait(lock);

                task = PopTask();
            }
            else
            {
                std::lock_guard<std::mutex> lock(mutex_);
                task = PopTask();
            }

            if (task)
                task->Work();
        }
    }

private:
    bool running_ = false;
    std::mutex mutex_;
    std::condition_variable condition_;
    std::vector<std::thread> threads_;
    std::queue<AsyncTaskPtr> tasks_;
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

UTILITY_NAMESPACE_END
