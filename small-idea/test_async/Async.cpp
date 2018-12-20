#include "Async.h"

class AsysTaskPool
{

};

bool IsAsyncTaskRunning()
{
    return true;
}

bool StartupAsyncTask(int thread)
{
    return true;
}

void ShutdownAsyncTask()
{

}

void PushAsyncTask(IKGAsyncTask* pTask)
{
    pTask->Work();
}
