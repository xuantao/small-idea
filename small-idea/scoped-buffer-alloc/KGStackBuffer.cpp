#include <cassert>
#include "KGStackBuffer.h"
#include "KGStackBufferAllocator.h"

KGStackBuffer::KGStackBuffer(uint32_t nSize)
    : m_pPool(nullptr), m_pBuff(new uint8_t[nSize]), m_nSize(nSize)
{}

KGStackBuffer::~KGStackBuffer()
{
    if (m_pPool)
        m_pPool->Deallocate(m_pBuff, m_nSize);
    else if (m_pBuff)
        delete m_pBuff;
}

void KGStackBuffer::Swap(KGStackBuffer& Other)
{
    m_pPool = Other.m_pPool;
    m_pBuff = Other.m_pBuff;
    m_nSize = Other.m_nSize;

    Other.m_pPool = nullptr;
    Other.m_pBuff = nullptr;
    Other.m_nSize = 0;
}
