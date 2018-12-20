#include "KGSteper.h"
#include <cassert>

//////////////////////////////////////////////////////////////////////////
// KGCustomSteper
KGSTEPER_RET KGCustomSteper::Step()
{
    if (m_Steper)
        return m_Steper();
    return KGSTEPER_RET::Completed;
}

void KGCustomSteper::Stop()
{
    if (m_Stoper)
        m_Stoper();
}

//////////////////////////////////////////////////////////////////////////
// KGSteperQueue

bool KGSteperQueue::Add(KGSteperPtr ptr)
{
    if (!ptr || ptr.get() == this)
        return false;

    m_Stpers.push(ptr);
    return true;
}

KGCustomSteperPtr KGSteperQueue::AddCustom()
{
    KGCustomSteperPtr ptr = std::make_shared<KGCustomSteper>();
    Add(ptr);
    return ptr;
}

KGSTEPER_RET KGSteperQueue::Step()
{
    assert(m_bIsGood);

//    uint64_t ulStart        = RDTSC();
    KGSTEPER_RET nResult    = KGSTEPER_RET::Completed;

    //while (!m_Stpers.empty())
    //{
    //    nResult = m_Stpers.front()->Step();
    //    if (nResult == KGSTEPER_RET::Completed)
    //    {
    //        m_Stopers.push(m_Stpers.front());
    //        m_Stpers.pop();
    //    }
    //    else if (nResult == KGSTEPER_RET::Failed)
    //    {
    //        m_bIsGood = false;
    //        Stop();
    //        break;
    //    }

    //    if (m_ulTimeLimit && (RDTSC() - ulStart) > m_ulTimeLimit)
    //    {
    //        if (!m_Stpers.empty())
    //            nResult = KGSTEPER_RET::Conintue;
    //        break;
    //    }
    //}
    return nResult;
}

void KGSteperQueue::Stop()
{
    while (!m_Stopers.empty())
    {
        m_Stopers.top()->Stop();
        m_Stopers.pop();
    }
}
