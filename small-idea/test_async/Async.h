#pragma once
#include "KGAsyncTask.h"

bool IsAsyncTaskRunning();
bool StartupAsyncTask(int thread);
void ShutdownAsyncTask();
void PushAsyncTask(IKGAsyncTask*);

template <typename Fty>
auto Async(Fty&& call) -> KGFuture<typename std::result_of<Fty()>::type>
{
    assert(IsAsyncTaskRunning());

    auto pTask = new KGAsyncTask<typename std::result_of<Fty()>::type, Fty>(std::forward<Fty>(call));
    auto future = pTask->GetFuture();
    PushAsyncTask(pTask);
    return std::move(future);
}
