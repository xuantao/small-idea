#include "KGStepExcutor.h"

void KGStepExcutorList::Clear()
{
    m_StepIndex = 0;
    m_Steps.clear();
}

KGSTEP_RET KGStepExcutorList::Step()
{
    if (m_StepIndex >= m_Steps.size())
        return KGSTEP_RET::Completed;

    auto ret = m_Steps[m_StepIndex]->Step();
    if (ret == KGSTEP_RET::Completed)
    {
        ++ m_StepIndex;
        if (m_StepIndex < m_Steps.size())
            ret = KGSTEP_RET::Conintue;
    }

    return ret;
}

void KGStepExcutorList::Rollback()
{
    for (int i = (int)m_StepIndex; i >= 0; --i)
        m_Steps[i]->Rollback();
    m_StepIndex = 0;
}
