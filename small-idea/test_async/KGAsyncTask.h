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
    inline bool IsComplete() const { return m_bComplete; }

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
    inline Rty& GetResult() { return m_Ret; }
    inline const Rty& GetResult() const { return m_Ret; }

    void SetResult(const Rty& ret)
    {
        m_Ret = ret;
        MarkComplete();
    }

    void SetResult(Rty&& ret)
    {
        m_Ret = std::forward<Rty>(ret);
        MarkComplete();
    }

private:
    Rty m_Ret;
};

template <typename Rty>
class KGFutureBase
{
public:
    typedef std::shared_ptr<KGFutureState<Rty>> StatePtr;

    KGFutureBase(StatePtr ptr) : m_pState(ptr) { }
    KGFutureBase() = default;
    KGFutureBase(const KGFutureBase&) = default;
    KGFutureBase& operator = (const KGFutureBase&) = default;

public:
    inline bool IsValid() const { return (bool)m_pState; }
    inline bool IsReady() const { return IsValid() && m_pState->IsComplete(); }

protected:
    const StatePtr& GetState() const { assert(IsValid()); return m_pState; }

private:
    StatePtr m_pState;
};

template <typename Rty>
class KGFuture : public KGFutureBase<Rty>
{
public:
    typedef KGFutureBase<Rty> BaseType;

    KGFuture(typename BaseType::StatePtr ptr) : BaseType(ptr) { }
    KGFuture() = default;
    KGFuture(const KGFuture&) = default;
    KGFuture& operator = (const KGFuture&) = default;

public:
    inline Rty& GetResult() const { return this->GetState()->GetResult(); }
};

template <typename Rty>
class KGFuture<Rty&> : public KGFutureBase<Rty*>
{
public:
    typedef KGFutureBase<Rty*> BaseType;

    KGFuture(typename BaseType::StatePtr ptr) : BaseType(ptr) { }
    KGFuture() = default;
    KGFuture(const KGFuture&) = default;
    KGFuture& operator = (const KGFuture&) = default;

public:
    inline Rty& GetResult() const { return *this->GetState()->GetResult(); }
};

template <>
class KGFuture<void> : public KGFutureBase<int>
{
public:
    typedef KGFutureBase<int> BaseType;

    KGFuture(typename BaseType::StatePtr ptr) : BaseType(ptr) { }
    KGFuture() = default;
    KGFuture(const KGFuture&) = default;
    KGFuture& operator = (const KGFuture&) = default;

public:
    inline void GetResult() const { this->GetState()->GetResult(); }
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

    KGFuture<Rty> GetFuture() const { return KGFuture<Rty>(m_pState); }

    void Work() override { m_pState->SetResult(m_Func()); }

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

    KGFuture<Rty> GetFuture() const { return KGFuture<Rty>(m_pState); }

    void Work() override { m_pState->SetResult(&m_Func()); }

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

    KGFuture<void> GetFuture() const { return KGFuture<void>(m_pState); }

    void Work() override
    {
        m_Func();
        m_pState->SetResult(0);
    }

private:
    StatePtr m_pState;
    Fty m_Func;
};

namespace KGAsync
{
    bool Startup(size_t threadNum);
    void Shutdown();
    void Run(IKGAsyncTask* pTask);

    template <typename Fty>
    auto Run(Fty&& call) -> KGFuture<typename std::result_of<Fty()>::type>
    {
        auto pTask = new KGAsyncTask<typename std::result_of<Fty()>::type, Fty>(std::forward<Fty>(call));
        auto future = pTask->GetFuture();
        Run(pTask);
        return future;
    }
}
