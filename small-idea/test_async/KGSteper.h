/*
 * 步进器
*/
#pragma once

#include <stack>
#include <queue>
#include <functional>
#include <memory>
//#include "SO3GlobalDef.h"

enum class KGSTEPER_RET
{
    Failed,
    Completed,
    Conintue,
};

struct IKGSteper
{
    virtual KGSTEPER_RET Step() = 0;
    virtual void Stop() = 0;
    virtual int Count() const = 0;
};

typedef std::shared_ptr<IKGSteper> KGSteperPtr;

class KGCustomSteper;
typedef std::shared_ptr<KGCustomSteper> KGCustomSteperPtr;

class KGSteperQueue;
typedef std::shared_ptr<KGSteperQueue> KGSteperQueuePtr;

/*
 * 通用实现
*/
class KGCustomSteper : public IKGSteper
{
public:
    KGCustomSteper()
    {
    }

    KGCustomSteper(const std::function<KGSTEPER_RET()>& steper) : m_Steper(steper)
    {
    }

    KGCustomSteper(const std::function<KGSTEPER_RET()>& steper, std::function<void()> stoper)
        : m_Steper(steper)
        , m_Stoper(stoper)
    {
    }

    virtual ~KGCustomSteper()
    {
    }

    KGCustomSteper(const KGCustomSteper&) = delete;
    KGCustomSteper& operator = (const KGCustomSteper&) = delete;

public:
    virtual KGSTEPER_RET Step();
    virtual void Stop();
    virtual int Count() const { return 1; }

public:
    std::function<KGSTEPER_RET()>   m_Steper;
    std::function<void()>           m_Stoper;
};

/*
 * 队列
*/
class KGSteperQueue : public IKGSteper
{
public:
    KGSteperQueue(uint64_t ulLimit = INT_MAX)
        : m_ulTimeLimit(ulLimit)
    {
    }

    virtual ~KGSteperQueue() {}

    KGSteperQueue(const KGSteperQueue&) = delete;
    KGSteperQueue& operator = (const KGSteperQueue&) = delete;

public:
    bool Add(KGSteperPtr ptr);
    KGCustomSteperPtr AddCustom();

public:
    virtual KGSTEPER_RET Step();
    virtual void Stop();
    virtual int Count() const { return 1; }

protected:
    bool        m_bIsGood = true;
    uint64_t    m_ulTimeLimit = 0;
    std::queue<KGSteperPtr> m_Stpers;
    std::stack<KGSteperPtr> m_Stopers;
};

