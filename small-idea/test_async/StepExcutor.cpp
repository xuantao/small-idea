#include "StepExcutor.h"
#include <chrono>

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
    if (m_eRet != STEP_STATUS::Completed)
        DoRollback();   // 没有成功结束就回滚
    m_Steps.clear();
}

STEP_STATUS QueueStepExcutor::Step()
{
    if (m_StepIndex >= m_Steps.size())
        return STEP_STATUS::Completed;

    if (IS_STEP_STOPPED(m_eRet))
        return m_eRet;

    m_eRet = m_Steps[m_StepIndex]->Step();
    if (m_eRet == STEP_STATUS::Completed)
    {
        ++ m_StepIndex;
        if (m_StepIndex < m_Steps.size())
            m_eRet = STEP_STATUS::Busy;
    }

    return m_eRet;
}

void QueueStepExcutor::DoRollback()
{
    for (int i = (int)m_StepIndex; i >= 0; --i)
        m_Steps[i]->Rollback();
    m_StepIndex = 0;
}


STEP_STATUS ParallelStepExcutor::Step()
{
    if (Empty())
        return STEP_STATUS::Completed;

    STEP_STATUS eRet = STEP_STATUS::Completed;
    size_t nIndex = 0;
    size_t nBusy = 0;
    while (nIndex < m_Steps.size())
    {
        eRet = m_Steps[nIndex]->Step();
        switch (eRet)
        {
        case STEP_STATUS::Busy:
            ++nBusy;
        case STEP_STATUS::Idle:
            ++nIndex;
            break;
        case STEP_STATUS::Failed:
            m_Steps[nIndex]->Rollback();
        case STEP_STATUS::Completed:
            m_Steps.erase(m_Steps.begin() + nIndex);
            break;
        }
    }

    return nBusy ? STEP_STATUS::Busy : STEP_STATUS::Idle;
}
