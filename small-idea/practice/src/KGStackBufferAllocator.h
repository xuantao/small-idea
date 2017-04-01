/*
 * stack buffer allocator
 * xuantao, Seasun Game 2017
 */
#include <cstdint>
#include "KGStackBuffer.h"
#include "KGStackBufferObj.h"

#pragma once

/*
 * ջBuffer������
 * �ڲ��ڶ��Ϸ���һ���Ļ�����, ����ʱ��ͷ��ʼ���η���, �ͷ��Ǵ�β��ͷ�����ͷ�.
 * ������ʽ��ջһ��, �ȷ�����ͷ�. ����KGStackBuffer��������ά���ͷ�˳��, ��֤
 * ջ����Ч��.
 * 1. ����������������ʱ, ��ֱ�ӴӶ��Ϸ���
 * 2. ��Debugģʽ���м򵥵�β��Խ����
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
