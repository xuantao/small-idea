/*
 * stack buffer holder
 * xuantao, Seasun Game 2017
 */
#include <cstdint>
#include <type_traits>

#pragma once

class KGStackBufferAllocator;

/*
 * 栈缓存Buffer, 辅助保证内存的分配释放时序(先入后出)
*/
class KGStackBuffer
{
    friend class KGStackBufferAllocator;
protected:
    KGStackBuffer(KGStackBuffer& Other)
    { Swap(Other); }

    KGStackBuffer(uint32_t nSize);

    KGStackBuffer(KGStackBufferAllocator* pPool, void* pData, uint32_t nSize)
        : m_pPool(pPool), m_pBuff(pData), m_nSize(nSize)
    {}

public:
    KGStackBuffer(KGStackBuffer&& Other)
    { Swap(Other); }

    ~KGStackBuffer();

public:
    inline void* GetBuffer() const { return m_pBuff; }
    inline uint32_t GetSize() const { return m_nSize; }

protected:
    void Swap(KGStackBuffer& Other);

protected:
    KGStackBufferAllocator* m_pPool;
    void*                   m_pBuff;
    uint32_t                m_nSize;
};
