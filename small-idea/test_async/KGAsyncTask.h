/*
 * 异步任务
*/
#pragma once
#include <memory>
#include <cassert>

/* 异步任务接口 */
class IKGAsyncTask
{
public:
    virtual ~IKGAsyncTask() {}
    virtual void Work() = 0;
};

/* 异步状态 */
class KGFutureStateBase
{
public:
    bool IsComplete() const { return m_bComplete; }

protected:
    void MarkComplete()
    {
        assert(m_bComplete == false);
        m_bComplete = true;
    }

private:
    bool m_bComplete = false;
};

template <typename Rty>
class KGFutureState : public KGFutureStateBase
{
public:
    KGFutureState() = default;

public:
    Rty& GetResult() { return m_Ret; }
    const Rty& GetResult() const { return m_Ret; }

    void SetResult(const Rty& ret)
    {
        m_Ret = ret;
        MarkComplete();
    }

    void SetResult(Rty&& ret)
    {
        m_Ret = ret;
        MarkComplete();
    }

private:
    Rty m_Ret;
};

template <typename Rty>
class KGFuture
{
    typedef std::shared_ptr<KGFutureState<Rty>> StatePtr;
public:
    KGFuture(StatePtr ptr) : m_pState(ptr) { }

    KGFuture() = default;
    KGFuture(const KGFuture&) = default;
    KGFuture& operator = (const KGFuture&) = default;

public:
    bool IsValid() const { return (bool)m_pState; }
    bool IsReady() const { return IsValid() ? m_pState->IsComplete() : false; }
    Rty& GetResult() const { return m_pState->GetResult(); }

public:
    StatePtr m_pState;
};

template <typename Rty>
class KGFuture<Rty&>
{
public:
    typedef std::shared_ptr<KGFutureState<Rty*>> StatePtr;

    KGFuture(StatePtr ptr) : m_pState(ptr) { }

    KGFuture() = default;
    KGFuture(const KGFuture&) = default;
    KGFuture& operator = (const KGFuture&) = default;

public:
    bool IsValid() const { return (bool)m_pState; }
    bool IsReady() const { return IsValid() ? m_pState->IsComplete() : false; }
    Rty& GetResult() const { return *m_pState->GetResult(); }

public:
    StatePtr m_pState;
};

template <>
class KGFuture<void>
{
public:
    typedef std::shared_ptr<KGFutureState<int>> StatePtr;

    KGFuture(StatePtr ptr) : m_pState(ptr) { }

    KGFuture() = default;
    KGFuture(const KGFuture&) = default;
    KGFuture& operator = (const KGFuture&) = default;

public:
    bool IsValid() const { return (bool)m_pState; }
    bool IsReady() const { return IsValid() ? m_pState->IsComplete() : false; }
    void GetResult() const { m_pState->GetResult(); }

public:
    StatePtr m_pState;
};

template <typename Rty, typename Fty>
class KGAsyncTask : public IKGAsyncTask
{
public:
    typedef std::shared_ptr<KGFutureState<Rty>> StatePtr;

public:
    KGAsyncTask(Fty&& fn)
        : m_pState(std::make_shared<KGFutureState<Rty>>())
        , m_Func(std::forward<Fty>(fn))
    { }

    virtual ~KGAsyncTask() { }

    KGFuture<Rty> GetFuture() const
    {
        return KGFuture<Rty>(m_pState);
    }

    void Work() override
    {
        m_pState->SetResult(m_Func());
    }

private:
    StatePtr m_pState;
    Fty m_Func;
};

template <typename Rty, typename Fty>
class KGAsyncTask<Rty&, Fty> : public IKGAsyncTask
{
public:
    typedef std::shared_ptr<KGFutureState<Rty*>> StatePtr;

public:
    KGAsyncTask(Fty&& fn)
        : m_pState(std::make_shared<KGFutureState<Rty>>())
        , m_Func(std::forward<Fty>(fn))
    { }

    virtual ~KGAsyncTask() { }

    KGFuture<Rty> GetFuture() const
    {
        return KGFuture<Rty>(m_pState);
    }

    void Work() override
    {
        m_pState->SetResult(&m_Func());
    }

private:
    StatePtr m_pState;
    Fty m_Func;
};

template <typename Fty>
class KGAsyncTask<void, Fty> : public IKGAsyncTask
{
public:
    typedef std::shared_ptr<KGFutureState<int>> StatePtr;

public:
    KGAsyncTask(Fty&& fn)
        : m_pState(std::make_shared<KGFutureState<int>>())
        , m_Func(std::forward<Fty>(fn))
    { }

    virtual ~KGAsyncTask() { }

    KGFuture<void> GetFuture() const
    {
        return KGFuture<void>(m_pState);
    }

    void Work() override
    {
        m_Func();
        m_pState->SetResult(0);
    }

private:
    StatePtr m_pState;
    Fty m_Func;
};
