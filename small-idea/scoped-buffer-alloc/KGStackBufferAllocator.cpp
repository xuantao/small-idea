#include <cassert>
#include "KGStackBufferAllocator.h"

#ifdef _DEBUG
/*
 * 通过在每段Buff的后面写入一些特定字节, 当释放Buff的时候判断是否被修改来检测是否被写越界了.
*/
#define _CHECK_BYTE 0xAA
static void SetCheckBits(uint8_t* pCheck, uint32_t nSize)
{
    for (uint32_t i = 0; i < nSize; ++i)
        pCheck[i] = _CHECK_BYTE;
}

static void CheckBits(uint8_t* pCheck, uint32_t nSize)
{
    uint32_t nOver = 0;
    for (uint32_t i = 0; i < nSize; ++i)
        nOver += (pCheck[i] != _CHECK_BYTE) ? 1 : 0;
    // over write bytes
    assert(nOver == 0);
}

static uint32_t Align(uint32_t nSize, uint32_t nAlign)
{
    uint32_t nRem = nSize % nAlign;
    if (nRem)
        nSize += nAlign - nRem;
    return nSize;
}
#endif

KGStackBufferAllocator::KGStackBufferAllocator(uint32_t nCapacity, uint32_t nAlignment)
    : m_nCpacity(nCapacity)
    , m_nAlignment(nAlignment)
    , m_nAlloc(0)
    , m_pBuffer(0)
{
    m_pBuffer = new uint8_t[m_nCpacity];
    assert(m_pBuffer);
}

KGStackBufferAllocator::~KGStackBufferAllocator()
{
    delete m_pBuffer;
    m_pBuffer = nullptr;
}

KGStackBuffer KGStackBufferAllocator::Allocate(uint32_t nSize)
{
    void* pBuffer   = nullptr;
    uint32_t nAlign = Align(nSize, m_nAlignment);

#ifdef _DEBUG
    if (nSize && nAlign == nSize)
        nAlign += m_nAlignment;
#endif // _DEBUG

    if (nAlign > m_nCpacity - m_nAlloc)
        return KGStackBuffer(nSize);

    pBuffer = &m_pBuffer[m_nAlloc];

#ifdef _DEBUG
    SetCheckBits((uint8_t*)pBuffer + nSize, nAlign - nSize);
#endif // _DEBUG

    m_nAlloc += nAlign;
    return KGStackBuffer(this, pBuffer, nSize);
}

void KGStackBufferAllocator::Deallocate(void* pBuffer, uint32_t nSize)
{
    uint32_t nAlign = Align(nSize, m_nAlignment);
#ifdef _DEBUG
    if (nSize && nAlign == nSize)
        nAlign += m_nAlignment;
#endif // _DEBUG

     assert(m_nAlloc >= nAlign);

#ifdef _DEBUG
    CheckBits((uint8_t*)pBuffer + nSize, nAlign - nSize);
#endif // _DEBUG

    m_nAlloc -= nAlign;
}
