#include "KGStepExcutor.h"

KGQueueStepExcutor::~KGQueueStepExcutor()
{
    if (m_eRet != KGSTEP_RET::Completed)
        DoRollback();
}

KGSTEP_RET KGQueueStepExcutor::Step()
{
    if (m_StepIndex >= m_Steps.size())
        return KGSTEP_RET::Completed;

    if (m_eRet != KGSTEP_RET::Continue)
        return m_eRet;

    m_eRet = m_Steps[m_StepIndex]->Step();
    if (m_eRet == KGSTEP_RET::Completed)
    {
        ++ m_StepIndex;
        if (m_StepIndex < m_Steps.size())
            m_eRet = KGSTEP_RET::Continue;
    }

    return m_eRet;
}

void KGQueueStepExcutor::DoRollback()
{
    for (int i = (int)m_StepIndex; i >= 0; --i)
        m_Steps[i]->Rollback();
    m_StepIndex = 0;
}
