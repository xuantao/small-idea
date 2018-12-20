/*
 * 分布初始化器
*/
#pragma once

#include <functional>
#include <vector>
#include "KGStepExcutor.h"
#include "KGAsyncTaskPool.h"

/* 分布初始化器 */
class KGStepInitializer
{
    typedef std::pair<KGStepExcutorPtr, std::function<void()>> Initializer;

public:
    KGStepInitializer() = default;
    virtual ~KGStepInitializer() { }

    KGStepInitializer(const KGStepInitializer&) = delete;
    KGStepInitializer& operator = (const KGStepInitializer&) = delete;

public:
    bool Create(int threadNum);
    void Destory();
    bool IsComplete() const { return m_bComplete; }
    KGSTEP_RET Step();

public:
    void AddStep(KGStepExcutorPtr fn)
    {
        m_Steps.Add(fn);
    }

    template <typename Fty>
    void AddStep(Fty&& fn)
    {
        m_Steps(std::forward<Fty>(fn));
    }

    template <typename Fty>
    auto Async(Fty&& call) -> KGFuture<typename std::result_of<Fty()>::type>
    {
        auto pTask = new KGAsyncTask<typename std::result_of<Fty()>::type, Fty>(std::forward<Fty>(call));
        auto future = pTask->GetFuture();
        m_TaskPool.AddTask(pTask);
        return std::move(future);
    }

private:
    bool m_bComplete = false;
    KGStepExcutorList m_Steps;
    KGAsyncTaskPool m_TaskPool;
};
