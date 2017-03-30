/*
* stack buffer object array
* xuantao, Seasun Game 2017
*/

#include <type_traits>
#include "KGStackBuffer.h"

#pragma once

/*
 * 构造目标对象, 区分POD类型, 如果是存在构造函数的, 使用 new (placement) Ty(...) 构造
*/
template <class Ty, bool isPod>
struct KGConstructor
{};

template <class Ty>
struct KGConstructor<Ty, true>
{
    static void Construct(Ty* pObj, uint32_t nCount)
    {}

    static void Destruct(Ty* pObj, uint32_t nCount)
    {}

    template <typename ...Args>
    static void Construct(Ty* pObj, uint32_t nCount, Args&& ...args)
    {
        for (uint32_t i = 0; i < nCount; ++i)
            new (&pObj[i]) Ty(args...);
    }
};

template <class Ty>
struct KGConstructor<Ty, false>
{
    static void Construct(Ty* pObj, uint32_t nCount)
    {
        for (uint32_t i = 0; i < nCount; ++i)
            new (&pObj[i]) Ty();
    }

    template <typename ...Args>
    static void Construct(Ty* pObj, uint32_t nCount, Args&& ...args)
    {
        for (uint32_t i = 0; i < nCount; ++i)
            new (&pObj[i]) Ty(args...);
    }

    static void Destruct(Ty* pObj, uint32_t nCount)
    {
        uint32_t nIdx = nCount;
        while (nIdx)
            (&pObj[--nIdx])->~Ty();
    }
};

/*
 * 用来维护StackBuffer构造的Obj数组
*/
template <class Ty>
class KGStackBufferObj : public KGStackBuffer, public KGConstructor<Ty, std::is_pod<Ty>::value>
{
public:
    KGStackBufferObj(KGStackBufferObj&& Other)
        : KGStackBuffer(Other)
        , m_nCount(Other.m_nCount)
    {
        Other.m_nCount = 0;
    }

    KGStackBufferObj(KGStackBuffer&& Buffer, uint32_t nCount)
        : KGStackBuffer(Buffer)
        , m_nCount(nCount)
    {
        Construct(GetBuffer(), m_nCount);
    }

    template <typename ...Args>
    KGStackBufferObj(KGStackBuffer&& Buffer, uint32_t nCount, Args&& ...args)
        : KGStackBuffer(Buffer)
        , m_nCount(nCount)
    {
        Construct(GetBuffer(), m_nCount, args...);
    }

    ~KGStackBufferObj()
    {
        Destruct(GetBuffer(), m_nCount);
    }

protected:
    KGStackBufferObj(const KGStackBufferObj&);
    KGStackBufferObj& operator = (const KGStackBufferObj&);

public:
    Ty* GetBuffer() const { return (Ty*)KGStackBuffer::GetBuffer(); }
    uint32_t GetCount() const { return m_nCount; }

protected:
    uint32_t m_nCount;
};
