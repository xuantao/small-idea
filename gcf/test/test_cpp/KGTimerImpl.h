#pragma once
#include <list>
#include <map>
#include <functional>
#include "SO3GlobalDef.h"
#include "Engine/KList.h"

struct KGNormalTicker
{
    void Step() { ++nTick; }
    int Now() { return nTick; }

protected:
    int nTick = 0;
};

template <typename Ty>
struct TimerCall;

template <>
struct TimerCall<void>
{
    std::function<void()> proc;
};

namespace timer_impl
{
    template <typename Ky>
    class TimerNode : public KNode
    {
    public:
        typedef Ky key_type;

    public:
        template<typename K>
        TimerNode(K&& key) : _key(std::forward<K>(key)) {}

        virtual ~TimerNode() {}

        const Ky& Key() const { return _key; }

        virtual int Call() = 0;

    protected:
        key_type _key;
    };

    template <typename Ky, typename Fy, typename Ry = std::result_of<Fy>::type>
    class TimerNodeImpl : public TimerNode<Ky>
    {
    public:
        typedef Ky key_type;
        typedef Fy func_type;
        typedef Ry return_type;
        typedef TimerNode<Ky> MyBase;

    public:
        template <typename K, typename F>
        TimerNodeImpl(K&& key, F&& func)
            : MyBase(std::forward<K>(key))
            , _func(std::forward<F>(func))
        {
        }

        virtual ~TimerNodeImpl() {}

        virtual int Call()
        {
            return (int)_func();
        }

    protected:
        func_type _func;
    };

    template <typename Ky, typename Fy>
    class TimerNodeImpl<Ky, Fy, void> : public TimerNode<Ky>
    {
    public:
        typedef Ky key_type;
        typedef Fy func_type;
        typedef void return_type;
        typedef TimerNode<Ky> MyBase;

    public:
        template <typename K, typename F>
        TimerNodeImpl(K&& key, F&& func)
            : MyBase(std::forward<K>(key))
            , _func(std::forward<F>(func))
        {
        }

        virtual ~TimerNodeImpl() {}

        virtual int Call()
        {
            _func();
            return 0;
        }

    protected:
        func_type _func;
    };

    template <typename Ky>
    class TimerHeader
    {
        KNode _root;
    };


    template <typename Ky, typename Vl>
    struct TimerAction
    {
        typedef Ky Key;
        //typedef
    };
}

/*
 * 定时器基础实现
 * 定时器接受的函数形式为 int (Proc)()
 * 其中返回值用来控制定时器的后续行为
 * 当Ret值:
 * = 0: 删除这个定时器
 * < 0: 表示按原先时长再次注册
 * > 0: 按指定时长再次注册
*/
template <typename Ky, typename Ticker>
class KGTimerImpl
{
public:
    typedef Ky KeyType;
    typedef Ticker Ticker;

protected:
    struct Node
    {
        Node* pPrev = nullptr;
        Node* pNext = nullptr;

        int     nLength = 0;
        KeyType Key;
        std::function<int()> fnCall;
    };

    struct Header
    {
        int     nTimeout    = 0;
        Node*   pHead       = nullptr;
        Node*   pTail       = nullptr;

        void Add(Node* pNode)
        {
            pNode->pPrev = pTail;
            if (pTail)
                pTail->pNext = pNode;
            else
                pHead = pTail = pNode;
        }

        void Remove(Node* pNode)
        {
            if (pNode == pTail)
                pTail = pNode->pPrev;
            if (pNode == pHead)
                pHead = pNode->pNext;

            if (pNode->pNext)
                pNode->pNext->pPrev = pNode->pPrev;
            if (pNode->pPrev)
                pNode->pPrev->pNext = pNode->pNext;
        }

        bool operator == (const Header& other) const
        {
            return other.nTimeout == nTimeout;
        }
    };

    typedef std::list<Header> List;
    typedef std::map<KeyType, Node*> Map;

public:
    KGTimerImpl() {}
    KGTimerImpl(const Ticker& ticker) : m_Tiker(ticker) {}
    ~KGTimerImpl() { Clear(); }

public:
    bool IsActive(const KeyType& key) const
    {
        auto itNode = m_mapNodes.find(key);
        return itNode != m_mapNodes.cend();
    }

    void Activate()
    {
        int nNow = m_Tiker.Now();
        while (!m_lstSlots.empty() && m_lstSlots.front().nTimeout <= nNow)
        {
            Node* pNode = m_lstSlots.front().pHead;
            m_lstSlots.pop_front();

            while (pNode)
            {
                Node* pProc = pNode;
                pNode = pNode->pNext;

                pProc->pPrev = nullptr;
                pProc->pNext = nullptr;

                int nRet = 0;
                if (pProc->fnCall)
                    nRet = pProc->fnCall();

                if (nRet == 0)
                {
                    m_mapNodes.erase(pProc->Key);
                    delete pProc;
                }
                else
                {
                    if (nRet > 0) pProc->nLength = nRet;
                    AttachSlot(pProc);
                }
            }
        }
    }

    bool Register(const KeyType& key, int nLength, const std::function<int()>& fnCall)
    {
        bool bResult        = false;
        Node* pNode         = nullptr;
        auto itNode         = m_mapNodes.end();
        auto itSlot         = m_lstSlots.end();

        KGLOG_PROCESS_ERROR(nLength > 0, key, nLength);

        itNode = m_mapNodes.find(key);
        KGLOG_PROCESS_ERROR(itNode == m_mapNodes.end(), key, nLength);

        pNode = new Node();
        pNode->Key      = key;
        pNode->nLength  = nLength;
        pNode->fnCall   = fnCall;

        m_mapNodes.insert(std::make_pair(key, pNode));
        AttachSlot(pNode);

        bResult = true;
    Exit0:
        return bResult;
    }

    bool UnRegister(const KeyType& key)
    {
        bool bResult    = false;
        Node* pNode     = nullptr;
        Header* pHeader = nullptr;
        auto itNode     = m_mapNodes.end();

        itNode = m_mapNodes.find(key);
        KG_PROCESS_ERROR(itNode != m_mapNodes.end());

        pNode = itNode->second;
        m_mapNodes.erase(itNode);

        pHeader = GetSlot(pNode);
        KGLOG_PROCESS_ERROR(pHeader);

        pHeader->Remove(pNode);
        if (pHeader->pTail == nullptr)
            m_lstSlots.remove(*pHeader);

        bResult = true;
    Exit0:
        delete pNode;
        return bResult;
    }

    void Clear()
    {
        for (auto& pair : m_mapNodes)
            delete pair.second;

        m_lstSlots.clear();
        m_mapNodes.clear();
    }

protected:
    void AttachSlot(Node* pNode)
    {
        int nTimeout    = pNode->nLength + m_Tiker.Now();
        Header* pHeader = GetSlot(nTimeout, true);
        KGLOG_PROCESS_ERROR(pHeader);

        pHeader->Add(pNode);

    Exit0:;
    }

    Header* GetSlot(int nTimeout, bool bAutoCreate)
    {
        auto itSlot = std::find_if(m_lstSlots.begin(), m_lstSlots.end(),
            [nTimeout](Header& header) { return header.nTimeout == nTimeout; });

        if (itSlot != m_lstSlots.end())
            return &(*itSlot);

        if (!bAutoCreate)
            return nullptr;

        Header header;
        header.nTimeout = nTimeout;
        header.pHead    = nullptr;
        header.pTail    = nullptr;

        auto itPos = m_lstSlots.begin();
        for (auto it = m_lstSlots.begin(); it != m_lstSlots.end(); ++it)
        {
            if (it->nTimeout > nTimeout)
                break;
            itPos = it;
        }

        itSlot = m_lstSlots.insert(itPos, header);
        assert(itSlot != m_lstSlots.end());
        return &(*itSlot);
    }

    Header* GetSlot(Node* pNode)
    {
        while (pNode->pPrev)
            pNode = pNode->pPrev;

        auto itSlot = std::find_if(m_lstSlots.begin(), m_lstSlots.end(),
            [pNode](Header& header) { return header.pHead == pNode; });

        if (itSlot != m_lstSlots.end())
            return nullptr;
        return &(*itSlot);
    }

protected:
    Ticker  m_Tiker;
    List    m_lstSlots;
    Map     m_mapNodes;
};
