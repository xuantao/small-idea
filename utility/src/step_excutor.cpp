#include "utility/step_excutor.h"
#include <chrono>

UTILITY_NAMESPACE_BEGIN

STEP_STATUS StepFor(IStepExcutor* pSteper, size_t nDuration)
{
    if (pSteper == nullptr)
        return STEP_STATUS::Completed;

    STEP_STATUS eStatus;
    auto endTime = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(nDuration);
    do
    {
        eStatus = pSteper->Step();
    } while (eStatus == STEP_STATUS::Busy && endTime > std::chrono::high_resolution_clock::now());

    return eStatus;
}

QueueStepExcutor::~QueueStepExcutor()
{
    if (status_ != STEP_STATUS::Completed)
        DoRollback();   // 没有成功结束就回滚
    steps_.clear();
}

STEP_STATUS QueueStepExcutor::Step()
{
    if (step_index_ >= steps_.size())
        return STEP_STATUS::Completed;

    if (IS_STEP_STOPPED(status_))
        return status_;

    status_ = steps_[step_index_]->Step();
    if (status_ == STEP_STATUS::Completed)
    {
        ++ step_index_;
        if (step_index_ < steps_.size())
            status_ = STEP_STATUS::Busy;
    }

    return status_;
}

void QueueStepExcutor::DoRollback()
{
    for (int i = (int)step_index_; i >= 0; --i)
        steps_[i]->Rollback();
    step_index_ = 0;
}

STEP_STATUS ParallelStepExcutor::Step()
{
    if (Empty())
        return STEP_STATUS::Completed;

    STEP_STATUS eRet = STEP_STATUS::Completed;
    size_t nIndex = 0;
    size_t nBusy = 0;
    while (nIndex < steps_.size())
    {
        eRet = steps_[nIndex]->Step();
        switch (eRet)
        {
        case STEP_STATUS::Busy:
            ++nBusy;
        case STEP_STATUS::Idle:
            ++nIndex;
            break;
        case STEP_STATUS::Failed:
            steps_[nIndex]->Rollback();
        case STEP_STATUS::Completed:
            steps_.erase(steps_.begin() + nIndex);
            break;
        }
    }

    return nBusy ? STEP_STATUS::Busy : STEP_STATUS::Idle;
}

UTILITY_NAMESPACE_END
