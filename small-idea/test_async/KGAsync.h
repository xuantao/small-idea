/*
 * 异步任务
*/
#pragma once
#include <type_traits>
#include <memory>
#include <cassert>
#include <tuple>
#include "KGTemplate.h"
#include "KGFuture.h"

/* 异步任务接口 */
class IKGAsyncTask
{
public:
    virtual ~IKGAsyncTask() {}
    virtual void Work() = 0;
};

/* internal implamentation */
namespace Async_Internal
{
    template <typename Rty>
    struct AsyncExcutor
    {
        template <typename Fty, typename Args, size_t... Idxs>
        static void Do(KGPromise<Rty>& promise, Fty& fn, Args& args, KGIndexSequence<Idxs...>&&)
        {
            promise.SetValue(fn(std::get<Idxs>(args)...));
        }
    };

    template <>
    struct AsyncExcutor<void>
    {
        template <typename Fty, typename Args, size_t... Idxs>
        static void Do(KGPromise<void>& promise, Fty& fn, Args& args, KGIndexSequence<Idxs...>&&)
        {
            fn(std::get<Idxs>(args)...);
            promise.SetValue();
        }
    };

    template <typename Rty, typename Fty, typename... Args>
    class Task : public IKGAsyncTask
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
            AsyncExcutor<Rty>::Do(m_Promise, m_Func, m_Args, KGMakeIndexSequence<sizeof...(Args)>());
        }

        KGFuture<Rty> GetFuture()
        {
            return m_Promise.GetFuture();
        }

    private:
        Fty m_Func;
        std::tuple<Args...> m_Args;
        KGPromise<Rty> m_Promise;
    };
}

namespace KGAsync
{
    bool Startup(size_t threadNum);
    void Shutdown();

    void PushTask(IKGAsyncTask* pTask);

    template <typename Fty, typename... Args>
    auto Run(Fty&& fn, Args&&... args) -> KGFuture<typename utility::invoke_result<Fty, Args...>::type>
    {
        using Rty = typename utility::invoke_result<Fty, Args...>::type;
        auto pTask = new Async_Internal::Task<Rty, Fty, Args...>(std::forward<Fty>(fn), std::forward<Args>(args)...);
        auto cFuture = pTask->GetFuture();

        PushTask(pTask);
        return cFuture;
    }
}
