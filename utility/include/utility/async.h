﻿/*
 * 异步任务
*/
#pragma once
#include <type_traits>
#include <memory>
#include <cassert>
#include <tuple>
#include "common.h"
#include "future.h"

UTILITY_NAMESPACE_BEGIN

/* 异步任务接口 */
class IAsyncTask
{
public:
    virtual ~IAsyncTask() {}
    virtual void Work() = 0;
};
typedef std::shared_ptr<IAsyncTask> AsyncTaskPtr;

/* internal implamentation */
namespace Async_Internal
{
    template <bool, typename Fty, typename... Args>
    struct AsyncRetType
    {
        typedef typename std_ext::invoke_result<Fty, Args...>::type type;
    };

    template <typename Fty, typename... Args>
    struct AsyncRetType<false, Fty, Args...>
    {
        typedef void type;
    };

    template <typename Fty, typename... Args>
    using AsyncRetType_t = typename AsyncRetType<std_ext::is_callable<Fty, Args...>::value, Fty, Args...>::type;

    template <typename Fty, typename... Args>
    using EnableIf = typename std::enable_if<
        std_ext::is_callable<Fty, Args...>::value,
        Future<AsyncRetType_t<Fty, Args...>>
    >::type;

    template <typename Rty, typename Fty, typename Args, size_t... Idxs>
    static auto DoWork(Promise<Rty>& promise, Fty& fn, Args& args, std_ext::index_sequence<Idxs...>&&) -> typename std::enable_if<!std::is_void<Rty>::value>::type
    {
        promise.SetValue(fn(std::get<Idxs>(args)...));
    }

    template <typename Rty, typename Fty, typename Args, size_t... Idxs>
    static auto DoWork(Promise<Rty>& promise, Fty& fn, Args& args, std_ext::index_sequence<Idxs...>&&) -> typename std::enable_if<std::is_void<Rty>::value>::type
    {
        fn(std::get<Idxs>(args)...);
        promise.SetValue();
    }

    template <typename Rty, typename Fty, typename... Args>
    class Task : public IAsyncTask
    {
    public:
        Task(Fty&& fn, Args&&... args)
            : m_Func(std::forward<Fty>(fn))
            , m_Args(std::forward<Args>(args)...)
            , m_Promise()
        {
        }

        virtual ~Task() = default;

        void Work() override
        {
            DoWork(m_Promise, m_Func, m_Args, std_ext::make_index_sequence_t<sizeof...(Args)>());
        }

        Future<Rty> GetFuture()
        {
            return m_Promise.GetFuture();
        }

    private:
        Fty m_Func;
        std::tuple<Args...> m_Args;
        Promise<Rty> m_Promise;
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
    template <typename Fty, typename... Args>
    auto Run(Fty&& fn, Args&&... args) -> typename Async_Internal::EnableIf<Fty, Args...>
    {
        using Rty = Async_Internal::AsyncRetType_t<Fty, Args...>;
        auto pTask = std::make_shared<Async_Internal::Task<Rty, Fty, Args...>>(std::forward<Fty>(fn), std::forward<Args>(args)...);
        auto cFuture = pTask->GetFuture();

        Run(pTask);
        return cFuture;
    }
}

UTILITY_NAMESPACE_END