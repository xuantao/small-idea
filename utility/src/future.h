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
        AssociatedState() : ready_(false) { }
        ~AssociatedState() = default;

        AssociatedState(const AssociatedState&) = delete;
        AssociatedState& operator=(const AssociatedState&) = delete;

    public:
        inline bool IsReady() const
        {
            return ready_;
        }

        inline RetType GetValue()
        {
            assert(IsReady());
            return result_;
        }

        inline void SetValue(const Ty& val)
        {
            result_ = val;
            ready_ = true;
        }

        inline void SetValue(Ty&& val)
        {
            result_ = std::forward<Ty>(val);
            ready_ = true;
        }

    public:
        Ty result_;
        bool ready_;
    };

    template<class Ty>
    class FutureState
    {
    public:
        typedef std::shared_ptr<AssociatedState<Ty>> AssociatedStatePtr;
        typedef typename AssociatedState<Ty>::RetType RetType;

    public:
        FutureState() = default;
        FutureState(AssociatedStatePtr state) : state_(state) { }
        FutureState(const FutureState&) = default;
        FutureState(FutureState&&) = default;
        ~FutureState() = default;

        FutureState& operator = (const FutureState&) = default;
        FutureState& operator = (FutureState&&) = default;

    public:
        inline bool IsValid() const
        {
            return (bool)state_;
        }

        inline bool IsReady() const
        {
            assert(IsValid());
            return (IsValid() && state_->IsReady());
        }

    protected:
        inline RetType _GetValue() const
        {
            assert(IsValid() && IsReady());
            return state_->GetValue();
        }

    private:
        AssociatedStatePtr state_;
    };

    template<class Ty>
    class PromiseState
    {
    public:
        typedef std::shared_ptr<AssociatedState<Ty>> AssociatedStatePtr;

    public:
        PromiseState(AssociatedStatePtr state) : state_(state) { }
        PromiseState(PromiseState&& other) : state_(std::move(other.state_)) { }
        ~PromiseState() = default;

        inline PromiseState& operator = (PromiseState&& other)
        {
            state_ = std::move(other.state_);
            return *this;
        }

        PromiseState(const PromiseState&) = delete;
        PromiseState& operator=(const PromiseState&) = delete;

    public:
        inline bool IsExpecting() const { return state_.use_count() > 1; }
        inline void Swap(PromiseState& other) { state_.swap(other.state_); }
        inline const AssociatedStatePtr& GetState() const { return state_; }
        inline AssociatedStatePtr& GetState() { return state_; }

    private:
        AssociatedStatePtr state_;
    };
} // namespace Future_Internal

template<class Ty>
class SharedFuture;

template<class Ty>
class Future : public Future_Internal::FutureState<Ty>
{
    using FutureState = Future_Internal::FutureState<Ty>;
    using RetType = typename FutureState::RetType;
public:
    Future() = default;
    Future(typename FutureState::AssociatedStatePtr state) : FutureState(state) { }
    Future(Future&&) = default;
    ~Future() = default;

    Future& operator = (Future&&) = default;

    Future(const Future&) = delete;
    Future& operator = (const Future&) = delete;

public:
    /* get the async task return value, must be state is READY
     * if the RetType is not a is_scalar type, the VALUE is referenced to avoid copy operate
    */
    inline RetType GetValue() { this->_GetValue(); }

    /* make future shared */
    inline SharedFuture<Ty> Share() { return (SharedFuture<Ty>(std::move(*this))); }
};

template<class Ty>
class Future<Ty&> : public Future_Internal::FutureState<Ty*>
{
    using FutureState = Future_Internal::FutureState<Ty*>;
public:
    Future() = default;
    Future(typename FutureState::AssociatedStatePtr state) : FutureState(state) { }
    Future(Future&&) = default;
    ~Future() = default;

    Future& operator=(Future&&) = default;

    Future(const Future&) = delete;
    Future& operator=(const Future&) = delete;

public:
    /* get the async task return value, must be state is READY */
    inline Ty& GetValue() { return *this->_GetValue(); }

    /* make future shared */
    inline SharedFuture<Ty&> Share() { return SharedFuture<Ty&>(std::move(*this)); }
};

template<>
class Future<void>
    : public Future_Internal::FutureState<int>
{
    using FutureState = Future_Internal::FutureState<int>;
public:
    Future() = default;
    Future(typename FutureState::AssociatedStatePtr state) : FutureState(state) { }
    Future(Future&&) = default;
    ~Future() = default;

    Future& operator = (Future&&) = default;

    Future(const Future&) = delete;
    Future& operator = (const Future&) = delete;

public:
    /* get the async task return value, must be state is READY */
    inline void GetValue() { this->_GetValue(); }

    /* make future shared */
    SharedFuture<void> Share();
};

template<class Ty>
class SharedFuture : public Future_Internal::FutureState<Ty>
{
    using FutureState = Future_Internal::FutureState<Ty>;
    using RetType = typename FutureState::RetType;
public:
    SharedFuture() { }
    SharedFuture(const SharedFuture& other) : FutureState(other) { }
    SharedFuture(Future<Ty>&& other) : FutureState(std::forward<FutureState>(other)) { }
    SharedFuture(SharedFuture&& other) : FutureState(std::move(other)) { }

    ~SharedFuture() { }

    SharedFuture& operator = (const SharedFuture& other)
    {
        FutureState::operator = (other);
        return (*this);
    }

    SharedFuture& operator = (SharedFuture&& other)
    {
        FutureState::operator = (std::move(other));
        return (*this);
    }

public:
    /* get the async task return value, must be state is READY
     * if the RetType is not a is_scalar type, the VALUE is referenced to avoid copy operate
    */
    inline RetType GetValue() const { return this->_GetValue(); }
};

template<class Ty>
class SharedFuture<Ty&> : public Future_Internal::FutureState<Ty*>
{
    using FutureState = Future_Internal::FutureState<Ty*>;
public:
    SharedFuture() { }
    SharedFuture(const SharedFuture& other) : FutureState(other) { }
    SharedFuture(Future<Ty&>&& other) : FutureState(std::forward<FutureState>(other)) { }
    SharedFuture(SharedFuture&& other) : FutureState(std::move(other)) { }
    ~SharedFuture() { }

    SharedFuture& operator = (const SharedFuture& other)
    {
        FutureState::operator = (other);
        return (*this);
    }

    SharedFuture& operator = (SharedFuture&& other)
    {
        FutureState::operator = (std::move(other));
        return (*this);
    }

public:
    /* get the async task return value, must be state is READY */
    inline Ty& GetValue() const { return *this->_GetValue(); }
};

template<>
class SharedFuture<void> : public Future_Internal::FutureState<int>
{
    using FutureState = Future_Internal::FutureState<int>;
public:
    SharedFuture() { }
    SharedFuture(const SharedFuture& other) : FutureState(other) { }
    SharedFuture(SharedFuture&& other) : FutureState(std::move(other)) { }
    SharedFuture(Future<void>&& other) : FutureState(std::forward<FutureState>(other)) { }
    ~SharedFuture() { }

    SharedFuture& operator = (const SharedFuture& other)
    {
        FutureState::operator = (other);
        return (*this);
    }

    SharedFuture& operator = (SharedFuture&& other)
    {
        FutureState::operator = (std::move(other));
        return (*this);
    }

public:
    /* get the async task return value, must be state is READY */
    inline void GetValue() const { this->_GetValue(); }
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
    Promise() : promise_(std::make_shared<Future_Internal::AssociatedState<Ty>>())
    { }

    Promise(Promise&& other) : promise_(std::move(other.promise_))
    { }

    ~Promise() { }

    Promise& operator = (Promise&& other)
    {
        Promise(std::move(other)).swap(*this);
        return *this;
    }

    Promise(const Promise&) = delete;
    Promise& operator = (const Promise&) = delete;

public:
    inline bool IsExpecting() const { return promise_.IsExpecting(); }
    inline void Swap(Promise& other) { promise_.Swap(other.promise_); }
    inline Future<Ty> GetFuture() { return Future<Ty>(promise_.GetState()); }

    inline void SetValue(const Ty& val) { promise_.GetState()->SetValue(val); }
    inline void SetValue(Ty&& val) { promise_.GetState()->SetValue(std::forward<Ty>(val)); }

private:
    Future_Internal::PromiseState<Ty> promise_;
};

template<class Ty>
class Promise<Ty&>
{
public:
    Promise() : promise_(std::make_shared<Future_Internal::AssociatedState<Ty*>>())
    { }

    Promise(Promise&& other) : promise_(std::move(other.promise_))
    { }

    ~Promise() { }

    Promise& operator = (Promise&& other)
    {
        Promise(std::move(other)).swap(*this);
        return *this;
    }

    Promise(const Promise&) = delete;
    Promise& operator=(const Promise&) = delete;

public:
    inline bool IsExpecting() const { return promise_.IsExpecting(); }
    inline void Swap(Promise& other) { promise_.Swap(other.promise_); }
    inline Future<Ty&> GetFuture() { return Future<Ty&>(promise_.GetState()); }
    inline void SetValue(Ty& val) { promise_.GetState()->SetValue(&val); }

private:
    Future_Internal::PromiseState<Ty*> promise_;
};

template<>
class Promise<void>
{
public:
    Promise() : promise_(std::make_shared<Future_Internal::AssociatedState<int>>())
    { }

    Promise(Promise&& other) : promise_(std::move(other.promise_))
    { }

    ~Promise() { }

    Promise& operator = (Promise&& other)
    {
        Promise(std::move(other)).Swap(*this);
        return (*this);
    }

    Promise(const Promise&) = delete;
    Promise& operator=(const Promise&) = delete;

public:
    inline bool IsExpecting() const { return promise_.IsExpecting(); }
    inline void Swap(Promise& _Other) { promise_.Swap(_Other.promise_); }
    inline Future<void> GetFuture() { return Future<void>(promise_.GetState()); }
    inline void SetValue() { promise_.GetState()->SetValue(1); }

private:
    Future_Internal::PromiseState<int> promise_;
};

UTILITY_NAMESPACE_END
