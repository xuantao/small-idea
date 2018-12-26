/*
 * Future & Promise
 * like std::future and std::promise but not the same
 * offer "IsReady()" interface, and change the "get()" return value as reference type
 * remove "wait", "wait_for" and "wait_until" interface
*/
#pragma once
#include "common.h"
#include <memory>
#include <cassert>

UTILITY_NAMESPACE_BEGIN

/* internal states */
namespace Future_Internal
{
    /* the final associated state with future ans promise */
    template<class Ty>
    class AssociatedState
    {
    public:
        typedef typename std::conditional<std::is_scalar<Ty>::value, Ty, Ty&>::type RetType;

    public:
        AssociatedState() : m_bReady(false)
        {
        }

        ~AssociatedState()
        {
        }

        AssociatedState(const AssociatedState&) = delete;
        AssociatedState& operator=(const AssociatedState&) = delete;

    public:
        inline bool IsReady() const
        {
            return m_bReady;
        }

        inline RetType GetValue()
        {
            assert(IsReady());
            return m_Result;
        }

        void SetValue(const Ty& val)
        {
            m_Result = val;
            m_bReady = true;
        }

        void SetValue(Ty&& val)
        {
            m_Result = std::forward<Ty>(val);
            m_bReady = true;
        }

    public:
        Ty m_Result;
        bool m_bReady;
    };

    template<class Ty>
    class FutureState
    {
    public:
        typedef std::shared_ptr<AssociatedState<Ty>> AssociatedStatePtr;
        typedef typename AssociatedState<Ty>::RetType RetType;

    public:
        FutureState()
        {
        }

        FutureState(AssociatedStatePtr pState)
            : m_pState(pState)
        {
        }

        ~FutureState()
        {
        }

        FutureState(const FutureState&) = default;
        FutureState(FutureState&&) = default;
        FutureState& operator = (const FutureState&) = default;
        FutureState& operator = (FutureState&&) = default;

    public:
        inline bool IsValid() const
        {
            return (bool)m_pState;
        }

        inline bool IsReady() const
        {
            return (m_pState && m_pState->IsReady());
        }

    protected:
        RetType _GetValue() const
        {
            assert(IsValid() && IsReady());
            return m_pState->GetValue();
        }

    private:
        AssociatedStatePtr m_pState;
    };

    template<class Ty>
    class PromiseState
    {
    public:
        typedef std::shared_ptr<AssociatedState<Ty>> AssociatedStatePtr;

    public:
        PromiseState(AssociatedStatePtr pState)
            : m_pState(pState)
        {
        }

        PromiseState(PromiseState&& other)
            : m_pState(std::move(other.m_pState))
        {
        }

        PromiseState& operator = (PromiseState&& other)
        {
            m_pState = std::move(other.m_pState);
            return *this;
        }

        ~PromiseState()
        {
        }

        PromiseState(const PromiseState&) = delete;
        PromiseState& operator=(const PromiseState&) = delete;

    public:
        void Swap(PromiseState& other)
        {
            m_pState.swap(other.m_pState);
        }

        const AssociatedStatePtr& GetState() const
        {
            return m_pState;
        }

        AssociatedStatePtr& GetState()
        {
            return m_pState;
        }

    private:
        AssociatedStatePtr m_pState;
    };
} // namespace Future_Internal

template<class Ty>
class SharedFuture;

template<class Ty>
class Future
    : public Future_Internal::FutureState<Ty>
{
    typedef Future_Internal::FutureState<Ty> FutureState;
    typedef typename FutureState::RetType RetType;

public:
    Future()
    {
    }

    Future(typename FutureState::AssociatedStatePtr pState)
        : FutureState(pState)
    {
    }

    ~Future()
    {
    }

    Future(Future&&) = default;
    Future& operator = (Future&&) = default;

    Future(const Future&) = delete;
    Future& operator = (const Future&) = delete;

public:
    RetType GetValue()
    {
        this->_GetValue();
    }

    SharedFuture<Ty> Share()
    {
        return (SharedFuture<Ty>(std::move(*this)));
    }
};

template<class Ty>
class Future<Ty&>
    : public Future_Internal::FutureState<Ty*>
{
    typedef Future_Internal::FutureState<Ty*> FutureState;

public:
    Future()
    {
    }

    Future(typename FutureState::AssociatedStatePtr pState)
        : FutureState(pState)
    {
    }

    ~Future()
    {
    }

    Future(Future&&) = default;
    Future& operator=(Future&&) = default;

    Future(const Future&) = delete;
    Future& operator=(const Future&) = delete;

public:
    Ty& GetValue()
    {
        return *this->_GetValue();
    }

    SharedFuture<Ty&> Share()
    {
        return SharedFuture<Ty&>(std::move(*this));
    }
};

template<>
class Future<void>
    : public Future_Internal::FutureState<int>
{
    typedef Future_Internal::FutureState<int> FutureState;

public:
    Future()
    {
    }

    Future(typename FutureState::AssociatedStatePtr pState)
        : FutureState(pState)
    {
    }

    ~Future()
    {
    }

    Future(Future&&) = default;
    Future& operator = (Future&&) = default;

    Future(const Future&) = delete;
    Future& operator = (const Future&) = delete;

public:
    void GetValue()
    {
        this->GetValue();
    }

    SharedFuture<void> Share();
};

template<class Ty>
class SharedFuture : public Future_Internal::FutureState<Ty>
{
    typedef Future_Internal::FutureState<Ty> FutureState;
    typedef typename FutureState::RetType RetType;

public:
    SharedFuture()
    {
    }

    SharedFuture(const SharedFuture& other)
        : FutureState(other)
    {
    }

    SharedFuture& operator = (const SharedFuture& other)
    {
        FutureState::operator = (other);
        return (*this);
    }

    SharedFuture(Future<Ty>&& other)
        : FutureState(std::forward<FutureState>(other))
    {
    }

    SharedFuture(SharedFuture&& other)
        : FutureState(std::move(other))
    {
    }

    SharedFuture& operator = (SharedFuture&& other)
    {
        FutureState::operator = (std::move(other));
        return (*this);
    }

    ~SharedFuture()
    {
    }

public:
    RetType GetValue() const
    {
        return this->_GetValue();
    }
};

template<class Ty>
class SharedFuture<Ty&> : public Future_Internal::FutureState<Ty*>
{
    typedef Future_Internal::FutureState<Ty*> FutureState;

public:
    SharedFuture()
    {
    }

    SharedFuture(const SharedFuture& other)
        : FutureState(other)
    {
    }

    SharedFuture& operator = (const SharedFuture& other)
    {
        FutureState::operator = (other);
        return (*this);
    }

    SharedFuture(Future<Ty&>&& other)
        : FutureState(std::forward<FutureState>(other))
    {
    }

    SharedFuture(SharedFuture&& other)
        : FutureState(std::move(other))
    {
    }

    SharedFuture& operator = (SharedFuture&& other)
    {
        FutureState::operator = (std::move(other));
        return (*this);
    }

    ~SharedFuture()
    {
    }

public:
    Ty& GetValue() const
    {
        return *this->_GetValue();
    }
};

template<>
class SharedFuture<void> : public Future_Internal::FutureState<int>
{
    typedef Future_Internal::FutureState<int> FutureState;

public:
    SharedFuture()
    {
    }

    SharedFuture(const SharedFuture& other)
        : FutureState(other)
    {
    }

    SharedFuture& operator = (const SharedFuture& other)
    {
        FutureState::operator = (other);
        return (*this);
    }

    SharedFuture(SharedFuture&& other)
        : FutureState(std::move(other))
    {
    }

    SharedFuture(Future<void>&& other)
        : FutureState(std::forward<FutureState>(other))
    {
    }

    SharedFuture& operator = (SharedFuture&& other)
    {
        FutureState::operator = (std::move(other));
        return (*this);
    }

    ~SharedFuture()
    {
    }

public:
    void GetValue() const
    {
        this->_GetValue();
    }
};

inline SharedFuture<void> Future<void>::Share()
{
    return SharedFuture<void>(std::move(*this));
}

/* promise */
template<class Ty>
class Promise
{
public:
    Promise()
        : m_Promise(std::make_shared<Future_Internal::AssociatedState<Ty>>())
    {
    }

    Promise(Promise&& other)
        : m_Promise(std::move(other.m_Promise))
    {
    }

    Promise& operator = (Promise&& other)
    {
        Promise(std::move(other)).swap(*this);
        return *this;
    }

    ~Promise()
    {
    }

    Promise(const Promise&) = delete;
    Promise& operator = (const Promise&) = delete;

public:
    void Swap(Promise& other)
    {
        m_Promise.Swap(other.m_Promise);
    }

    Future<Ty> GetFuture()
    {
        return Future<Ty>(m_Promise.GetState());
    }

    void SetValue(const Ty& val)
    {
        m_Promise.GetState()->SetValue(val);
    }

    void SetValue(Ty&& val)
    {
        m_Promise.GetState()->SetValue(std::forward<Ty>(val));
    }

private:
    Future_Internal::PromiseState<Ty> m_Promise;
};

template<class Ty>
class Promise<Ty&>
{
public:
    Promise()
        : m_Promise(std::make_shared<Future_Internal::AssociatedState<Ty*>>())
    {
    }

    Promise(Promise&& other)
        : m_Promise(std::move(other.m_Promise))
    {
    }

    Promise& operator = (Promise&& other)
    {
        Promise(std::move(other)).swap(*this);
        return *this;
    }

    ~Promise()
    {
    }

    Promise(const Promise&) = delete;
    Promise& operator=(const Promise&) = delete;

public:
    void Swap(Promise& other)
    {
        m_Promise.Swap(other.m_Promise);
    }

    Future<Ty&> GetFuture()
    {
        return Future<Ty&>(m_Promise.GetState());
    }

    void SetValue(Ty& val)
    {
        m_Promise._Get_state()->SetValue(&val);
    }

private:
    Future_Internal::PromiseState<Ty*> m_Promise;
};

template<>
class Promise<void>
{
public:
    Promise()
        : m_Promise(std::make_shared<Future_Internal::AssociatedState<int>>())
    {
    }

    Promise(Promise&& other)
        : m_Promise(std::move(other.m_Promise))
    {
    }

    Promise& operator = (Promise&& other)
    {
        Promise(std::move(other)).Swap(*this);
        return (*this);
    }

    ~Promise()
    {
    }

    Promise(const Promise&) = delete;
    Promise& operator=(const Promise&) = delete;

public:
    void Swap(Promise& _Other)
    {
        m_Promise.Swap(_Other.m_Promise);
    }

    Future<void> GetFuture()
    {
        return Future<void>(m_Promise.GetState());
    }

    void SetValue()
    {
        m_Promise.GetState()->SetValue(1);
    }

private:
    Future_Internal::PromiseState<int> m_Promise;
};

UTILITY_NAMESPACE_END
