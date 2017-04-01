/*
 * stack buffer allocator
 * xuantao, Seasun Game 2017
 */
#include <cstdint>
#include "KGStackBuffer.h"
#include "KGStackBufferObj.h"

#pragma once

/*
 * 栈Buffer分配器
 * 内部在堆上分配一块大的缓冲区, 分配时从头开始依次分配, 释放是从尾到头依次释放.
 * 操作形式和栈一致, 先分配后释放. 再由KGStackBuffer辅助类来维护释放顺序, 保证
 * 栈的有效性.
 * 1. 当缓存区不够分配时, 则直接从堆上分配
 * 2. 在Debug模式下有简单的尾部越界检测
*/
class KGStackBufferAllocator
{
    friend class KGStackBuffer;
public:
    KGStackBufferAllocator(uint32_t nCapacity, uint32_t nAlignment);
    ~KGStackBufferAllocator();

private:
    KGStackBufferAllocator(const KGStackBufferAllocator&);
    KGStackBufferAllocator& operator = (const KGStackBufferAllocator&) { return *this; }

public:
    KGStackBuffer Allocate(uint32_t nSize);

    template<class Ty>
    KGStackBufferObj<Ty> Allocate(uint32_t nCount)
    {
        return KGStackBufferObj<Ty>(Allocate(sizeof(Ty) * nCount), nCount);
    }

    template<class Ty, typename ...Args>
    KGStackBufferObj<Ty> Allocate(uint32_t nCount, Args&& ...args)
    {
        return KGStackBufferObj<Ty>(Allocate(sizeof(Ty) * nCount), nCount, args...);
    }

    uint32_t GetCapacity() const { return m_nCpacity; }

    uint32_t GetAlignment() const { return m_nAlignment; }

    uint32_t GetSize() const { return m_nCpacity - m_nAlloc; }

protected:
    void Deallocate(void* pBuffer, uint32_t nSize);

private:
    uint32_t    m_nCpacity;
    uint32_t    m_nAlignment;

    uint32_t    m_nAlloc;
    uint8_t*    m_pBuffer;
};
