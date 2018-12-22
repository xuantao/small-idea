/*
 * Future & Promise
 * like std::future and std::promise but not the same
 * offer "IsReady()" interface, and change the "get()" return value as reference type
 * remove "wait", "wait_for" and "wait_until" interface
*/
#pragma once
#include <memory>
#include <cassert>

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

        FutureState(AssociatedStatePtr _New_state)
            : m_pState(_New_state)
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

        PromiseState& operator = (PromiseState&& v)
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
class KGSharedFuture;

template<class Ty>
class KGFuture
    : public Future_Internal::FutureState<Ty>
{
    typedef Future_Internal::FutureState<Ty> FutureState;
    typedef typename FutureState::RetType RetType;

public:
    KGFuture()
    {
    }

    KGFuture(typename FutureState::AssociatedStatePtr _State)
        : FutureState(_State)
    {
    }

    ~KGFuture()
    {
    }

    KGFuture(KGFuture&&) = default;
    KGFuture& operator = (KGFuture&&) = default;

    KGFuture(const KGFuture&) = delete;
    KGFuture& operator = (const KGFuture&) = delete;

public:
    RetType GetValue()
    {
        this->_GetValue();
    }

    KGSharedFuture<Ty> Share()
    {
        return (KGSharedFuture<Ty>(std::move(*this)));
    }
};

template<class Ty>
class KGFuture<Ty&>
    : public Future_Internal::FutureState<Ty*>
{
    typedef Future_Internal::FutureState<Ty*> FutureState;

public:
    KGFuture()
    {
    }

    KGFuture(typename FutureState::AssociatedStatePtr _State)
        : FutureState(_State)
    {
    }

    ~KGFuture()
    {
    }

    KGFuture(KGFuture&&) = default;
    KGFuture& operator=(KGFuture&&) = default;

    KGFuture(const KGFuture&) = delete;
    KGFuture& operator=(const KGFuture&) = delete;

public:
    Ty& GetValue()
    {
        return *this->_GetValue();
    }

    KGSharedFuture<Ty&> Share()
    {
        return KGSharedFuture<Ty&>(std::move(*this));
    }
};

template<>
class KGFuture<void>
    : public Future_Internal::FutureState<int>
{
    typedef Future_Internal::FutureState<int> FutureState;

public:
    KGFuture()
    {
    }

    KGFuture(typename FutureState::AssociatedStatePtr _State)
        : FutureState(_State)
    {
    }

    ~KGFuture()
    {
    }

    KGFuture(KGFuture&&) = default;
    KGFuture& operator = (KGFuture&&) = default;

    KGFuture(const KGFuture&) = delete;
    KGFuture& operator = (const KGFuture&) = delete;

public:
    void GetValue()
    {
        this->GetValue();
    }

    KGSharedFuture<void> Share();
};

template<class Ty>
class KGSharedFuture : public Future_Internal::FutureState<Ty>
{
    typedef Future_Internal::FutureState<Ty> FutureState;
    typedef typename FutureState::RetType RetType;

public:
    KGSharedFuture()
    {
    }

    KGSharedFuture(const KGSharedFuture& _Other)
        : FutureState(_Other)
    {
    }

    KGSharedFuture& operator = (const KGSharedFuture& _Right)
    {
        FutureState::operator = (_Right);
        return (*this);
    }

    KGSharedFuture(KGFuture<Ty>&& _Other)
        : FutureState(std::forward<FutureState>(_Other))
    {
    }

    KGSharedFuture(KGSharedFuture&& _Other)
        : FutureState(std::move(_Other))
    {
    }

    KGSharedFuture& operator = (KGSharedFuture&& _Right)
    {
        FutureState::operator = (std::move(_Right));
        return (*this);
    }

    ~KGSharedFuture()
    {
    }

public:
    RetType GetValue() const
    {
        return this->_GetValue();
    }
};

template<class Ty>
class KGSharedFuture<Ty&> : public Future_Internal::FutureState<Ty*>
{
    typedef Future_Internal::FutureState<Ty*> FutureState;

public:
    KGSharedFuture()
    {
    }

    KGSharedFuture(const KGSharedFuture& _Other)
        : FutureState(_Other)
    {
    }

    KGSharedFuture& operator = (const KGSharedFuture& _Right)
    {
        FutureState::operator = (_Right);
        return (*this);
    }

    KGSharedFuture(KGFuture<Ty&>&& _Other)
        : FutureState(std::forward<FutureState>(_Other))
    {
    }

    KGSharedFuture(KGSharedFuture&& _Other)
        : FutureState(std::move(_Other))
    {
    }

    KGSharedFuture& operator = (KGSharedFuture&& _Right)
    {
        FutureState::operator = (std::move(_Right));
        return (*this);
    }

    ~KGSharedFuture()
    {
    }

public:
    Ty& GetValue() const
    {
        return *this->_GetValue();
    }
};

template<>
class KGSharedFuture<void> : public Future_Internal::FutureState<int>
{
    typedef Future_Internal::FutureState<int> FutureState;

public:
    KGSharedFuture()
    {
    }

    KGSharedFuture(const KGSharedFuture& _Other)
        : FutureState(_Other)
    {
    }

    KGSharedFuture& operator = (const KGSharedFuture& _Right)
    {
        FutureState::operator = (_Right);
        return (*this);
    }

    KGSharedFuture(KGSharedFuture&& _Other)
        : FutureState(std::move(_Other))
    {
    }

    KGSharedFuture(KGFuture<void>&& _Other)
        : FutureState(std::forward<FutureState>(_Other))
    {
    }

    KGSharedFuture& operator = (KGSharedFuture&& _Right)
    {
        FutureState::operator = (std::move(_Right));
        return (*this);
    }

    ~KGSharedFuture()
    {
    }

public:
    void GetValue() const
    {
        this->_GetValue();
    }
};

inline KGSharedFuture<void> KGFuture<void>::Share()
{
    return KGSharedFuture<void>(std::move(*this));
}

/* promise */
template<class Ty>
class KGPromise
{
public:
    KGPromise()
        : m_Promise(std::make_shared<Future_Internal::AssociatedState<Ty>>())
    {
    }

    KGPromise(KGPromise&& other)
        : m_Promise(std::move(other.m_Promise))
    {
    }

    KGPromise& operator = (KGPromise&& other)
    {
        KGPromise(std::move(other)).swap(*this);
        return *this;
    }

    ~KGPromise()
    {
    }

    KGPromise(const KGPromise&) = delete;
    KGPromise& operator = (const KGPromise&) = delete;

public:
    void Swap(KGPromise& other)
    {
        m_Promise.Swap(other.m_Promise);
    }

    KGFuture<Ty> GetFuture()
    {
        return KGFuture<Ty>(m_Promise.GetState());
    }

    void SetValue(const Ty& _Val)
    {
        m_Promise.GetState()->SetValue(_Val);
    }

    void SetValue(Ty&& _Val)
    {
        m_Promise.GetState()->SetValue(std::forward<Ty>(_Val));
    }

private:
    Future_Internal::PromiseState<Ty> m_Promise;
};

template<class Ty>
class KGPromise<Ty&>
{
public:
    KGPromise()
        : m_Promise(std::make_shared<Future_Internal::AssociatedState<Ty*>>())
    {
    }

    KGPromise(KGPromise&& other)
        : m_Promise(std::move(other.m_Promise))
    {
    }

    KGPromise& operator = (KGPromise&& other)
    {
        KGPromise(std::move(other)).swap(*this);
        return *this;
    }

    ~KGPromise()
    {
    }

    KGPromise(const KGPromise&) = delete;
    KGPromise& operator=(const KGPromise&) = delete;

public:
    void Swap(KGPromise& other)
    {
        m_Promise.Swap(other.m_Promise);
    }

    KGFuture<Ty&> GetFuture()
    {
        return KGFuture<Ty&>(m_Promise.GetState());
    }

    void SetValue(Ty& _Val)
    {
        m_Promise._Get_state()->SetValue(&_Val);
    }

private:
    Future_Internal::PromiseState<Ty*> m_Promise;
};

template<>
class KGPromise<void>
{
public:
    KGPromise()
        : m_Promise(std::make_shared<Future_Internal::AssociatedState<int>>())
    {
    }

    KGPromise(KGPromise&& other)
        : m_Promise(std::move(other.m_Promise))
    {
    }

    KGPromise& operator = (KGPromise&& other)
    {
        KGPromise(std::move(other)).Swap(*this);
        return (*this);
    }

    ~KGPromise()
    {
    }

    KGPromise(const KGPromise&) = delete;
    KGPromise& operator=(const KGPromise&) = delete;

public:
    void Swap(KGPromise& _Other)
    {
        m_Promise.Swap(_Other.m_Promise);
    }

    KGFuture<void> GetFuture()
    {
        return KGFuture<void>(m_Promise.GetState());
    }

    void SetValue()
    {
        m_Promise.GetState()->SetValue(1);
    }

private:
    Future_Internal::PromiseState<int> m_Promise;
};
