#include "KGStepInitializer.h"

bool KGStepInitializer::Create(int threadNum)
{
    return m_TaskPool.Create(threadNum);
}

void KGStepInitializer::Destory()
{
    m_TaskPool.Destory();
    m_Steps.Clear();
}

KGSTEP_RET KGStepInitializer::Step()
{
    assert(m_bComplete == false);

    auto ret = m_Steps.Step();
    if (ret != KGSTEP_RET::Conintue)
    {
        m_bComplete = true;

        if (ret == KGSTEP_RET::Failed)
            m_Steps.Rollback();
    }

    return ret;
}
