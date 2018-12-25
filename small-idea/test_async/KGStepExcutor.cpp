#include "KGStepExcutor.h"

KGQueueStepExcutor::~KGQueueStepExcutor()
{
    if (m_eRet != KGSTEP_STATUS::Completed)
        DoRollback();
    m_Steps.clear();
}

KGSTEP_STATUS KGQueueStepExcutor::Step()
{
    if (m_StepIndex >= m_Steps.size())
        return KGSTEP_STATUS::Completed;

    if (m_eRet != KGSTEP_STATUS::Busy)
        return m_eRet;

    m_eRet = m_Steps[m_StepIndex]->Step();
    if (m_eRet == KGSTEP_STATUS::Completed)
    {
        ++ m_StepIndex;
        if (m_StepIndex < m_Steps.size())
            m_eRet = KGSTEP_STATUS::Busy;
    }

    return m_eRet;
}

void KGQueueStepExcutor::DoRollback()
{
    for (int i = (int)m_StepIndex; i >= 0; --i)
        m_Steps[i]->Rollback();
    m_StepIndex = 0;
}


KGSTEP_STATUS KGParallelStepExcutor::Step()
{
    if (Empty())
        return KGSTEP_STATUS::Completed;

    KGSTEP_STATUS eRet = KGSTEP_STATUS::Completed;
    size_t nIndex = 0;
    size_t nBusy = 0;
    while (nIndex < m_Steps.size())
    {
        eRet = m_Steps[nIndex]->Step();
        switch (eRet)
        {
        case KGSTEP_STATUS::Busy:
            ++nBusy;
        case KGSTEP_STATUS::Idle:
            ++nIndex;
            break;
        case KGSTEP_STATUS::Failed:
            m_Steps[nIndex]->Rollback();
        case KGSTEP_STATUS::Completed:
            m_Steps.erase(m_Steps.begin() + nIndex);
            break;
        }
    }

    return nBusy ? KGSTEP_STATUS::Busy : KGSTEP_STATUS::Idle;
}
