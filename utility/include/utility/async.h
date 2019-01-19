/*
 * 异步任务
 * 需要先初始化线程池
*/
#pragma once
#include <type_traits>
#include <memory>
#include "common.h"
#include "future.h"

UTILITY_NAMESPACE_BEGIN

/* 异步任务接口 */
class IAsyncTask
{
public:
    virtual ~IAsyncTask() {}

    /* check the task is abandoned, whether do real work*/
    virtual bool IsAbandoned() = 0;

    /* do real work in any thread */
    virtual void Work() = 0;
};
typedef std::shared_ptr<IAsyncTask> AsyncTaskPtr;

/* internal implamentation */
namespace Async_Internal
{
    template <bool, typename Fy, typename... Args>
    struct AsyncRetType
    {
        typedef typename std_ext::invoke_result<Fy, Args...>::type type;
    };

    template <typename Fy, typename... Args>
    struct AsyncRetType<false, Fy, Args...>
    {
        typedef void type;
    };

    template <typename Fy, typename... Args>
    using AsyncRetType_t = typename AsyncRetType<std_ext::is_callable<Fy, Args...>::value, Fy, Args...>::type;

    template <typename Fy, typename... Args>
    using EnableIf = typename std::enable_if<std_ext::is_callable<Fy, Args...>::value, Future<AsyncRetType_t<Fy, Args...>>>::type;

    template <typename Ry, typename Fy>
    inline auto DoPromise(Promise<Ry>& promise, Fy& func) -> typename std::enable_if<!std::is_void<Ry>::value>::type
    {
        promise.SetValue(func());
    }

    template <typename Ry, typename Fy>
    inline auto DoPromise(Promise<Ry>& promise, Fy& func) -> typename std::enable_if<std::is_void<Ry>::value>::type
    {
        func();
        promise.SetValue();
    }

    template <typename Fy, typename... Args>
    class Task : public IAsyncTask
    {
    public:
        using Callable = CallablePackage<Fy, Args...>;
        using RetType = typename std_ext::invoke_result<Callable>::type;

        Task(Fy&& fn, Args&&... args) : func_(std::forward<Fy>(fn), std::forward<Args>(args)...)
        { }

        virtual ~Task() { }

    public:
        bool IsAbandoned() override { return false; }
        void Work() override { DoPromise(promise_, func_); }

        inline Future<RetType> GetFuture() { return promise_.GetFuture(); }

    protected:
        Callable func_;
        Promise<RetType> promise_;
    };

    template <typename Fy, typename... Args>
    class ExpectedTask : public Task<Fy, Args...>
    {
    public:
        ExpectedTask(Fy&& fn, Args&&... args) : Task<Fy, Args...>(std::forward<Fy>(fn), std::forward<Args>(args)...)
        { }

        virtual ~ExpectedTask() { }

    public:
        /* when none body is expected this task, task will be enter abandoned state */
        bool IsAbandoned() override { return !this->promise_.IsExpecting(); }
    };
}

namespace Async
{
    /* 启动线程池
     * threadNum指定线程数量
    */
    bool Startup(size_t threadNum);

    /* 结束线程池 */
    void Shutdown();

    /* 是否正在运行 */
    bool IsRunning();

    /* 向线程池压入一个异步任务 */
    void Run(AsyncTaskPtr pTask);

    /* 执行异步任务
     * 创建一个异步任务并当如线程池队列
     * 返回Future<Rty>用来判断任务状态并获取返回值
    */
    template <typename Fy, typename... Args>
    inline auto Run(Fy&& fn, Args&&... args) -> typename Async_Internal::EnableIf<Fy, Args...>
    {
        auto task = std::make_shared<Async_Internal::Task<Fy, Args...>>(std::forward<Fy>(fn), std::forward<Args>(args)...);
        auto future = task->GetFuture();

        Run(task);
        return future;
    }

    /* 执行被期待的异步任务
     * 创建一个异步任务并当如线程池队列
     * 返回Future<Rty>用来判断任务状态并获取返回值
     * 当返回的Future不再被持有时, 任务进入废弃状态可能不会被执行
    */
    template <typename Fy, typename... Args>
    inline auto ExpectRun(Fy&& fn, Args&&... args) -> typename Async_Internal::EnableIf<Fy, Args...>
    {
        auto task = std::make_shared<Async_Internal::ExpectedTask<Fy, Args...>>(std::forward<Fy>(fn), std::forward<Args>(args)...);
        auto future = task->GetFuture();

        Run(task);
        return future;
    }
}

UTILITY_NAMESPACE_END
