/*
 * 分步执行器
*/
#pragma once

#include <type_traits>
#include <memory>
#include <vector>
#include "future.h"
#include "scope_guard.h"

UTILITY_NAMESPACE_BEGIN

enum class STEP_STATUS
{
    Busy,       // 繁忙
    Idle,       // 空闲
    Failed,     // 失败
    Completed,  // 结束
};

/* 判断分布执行器是否结束 */
#define IS_STEP_STOPPED(eStatus) (eStatus == UTILITY_NAMESPCE STEP_STATUS::Failed || \
    eStatus == UTILITY_NAMESPCE STEP_STATUS::Completed)

/* 步进器守卫 */
class StepGuard;

/* 分布执行器接口 */
struct IStepExcutor
{
    virtual ~IStepExcutor() { }

    /* 向前走一步 */
    virtual STEP_STATUS Step() = 0;

    /* 错误回滚 */
    virtual void Rollback() = 0;
};
typedef std::shared_ptr<IStepExcutor> StepExcutorPtr;


/* 构建一个分布执行器
 * Fty支持的函数签名为:
 * 1. void(), void(tepGuard&), the status will be STEP_STATUS::Completed
 * 2. bool(), bool(StepGuard&), the status will be (ret ? STEP_STATUS::Completed : STEP_STATUS::Failed)
 * 3. STEP_STATUS(), STEP_STATUS(StepGuard&)
*/
template <typename Fty>
StepExcutorPtr MakeStepExcutor(Fty&& fn);

template <typename Rty, typename Fty>
StepExcutorPtr MakeStepExcutor(Future<Rty>&& future, Fty&& fn);

template <typename Rty, typename Fty>
StepExcutorPtr MakeStepExcutor(const SharedFuture<Rty>& future, Fty&& fn);

/* 执行一段固定时间(单位毫秒)
 * 当步进器遇到Idle, Failed, Completed结束
*/
STEP_STATUS StepFor(IStepExcutor* pSteper, size_t nDuration);
inline STEP_STATUS StepFor(StepExcutorPtr pSteper, size_t nDuration) { return StepFor(pSteper.get(), nDuration); }

/* 持续执行步进器知道结束
 * 当步进器遇到Failed, Completed结束
*/
STEP_STATUS StepEnd(IStepExcutor* pSteper);
inline STEP_STATUS StepEnd(StepExcutorPtr pSteper) { return StepEnd(pSteper.get()); }

namespace StepExcutor_Internal
{
    template <typename Fty> StepExcutorPtr MakeExcutor(Fty&& fn, SerialAllocator<>* alloc);
    template <typename Rty, typename Fty> StepExcutorPtr MakeExcutor(Future<Rty>&& future, Fty&& fn, SerialAllocator<>* alloc);
    template <typename Rty, typename Fty> StepExcutorPtr MakeExcutor(const SharedFuture<Rty>& future, Fty&& fn, SerialAllocator<>* alloc);
}
/* 分布执行列表, 顺序执行 */
class QueueStepExcutor final : public IStepExcutor
{
public:
    QueueStepExcutor() : alloc_(1024) { }
    QueueStepExcutor(size_t cache_block_size) : alloc_(cache_block_size) {}

    virtual ~QueueStepExcutor();

    QueueStepExcutor(const QueueStepExcutor&) = delete;
    QueueStepExcutor& operator = (const QueueStepExcutor&) = delete;

public:
    STEP_STATUS Step() override;
    void Rollback() override { DoRollback(); }

public:
    inline bool Empty() const { return steps_.empty(); }
    inline void Add(StepExcutorPtr ptr) { steps_.push_back(ptr); }

    template <typename Fty>
    inline void Add(Fty&& fn)
    {
        Add(StepExcutor_Internal::MakeExcutor(
            std::forward<Fty>(fn), alloc_.GetAlloc()));
    }

    template <typename Rty, typename Fty>
    inline void Add(Future<Rty>&& future, Fty&& fn)
    {
        Add(StepExcutor_Internal::MakeExcutor(
            std::move(future), std::forward<Fty>(fn), alloc_.GetAlloc()));
    }

    template <typename Rty, typename Fty>
    inline void Add(const SharedFuture<Rty>& future, Fty&& fn)
    {
        Add(StepExcutor_Internal::MakeExcutor(
            future, std::forward<Fty>(fn), alloc_.GetAlloc()));
    }

private:
    void DoRollback();

private:
    size_t step_index_ = 0;
    STEP_STATUS status_ = STEP_STATUS::Idle;
    std::vector<StepExcutorPtr> steps_;
    PoolSerialAlloc<512> alloc_;
}; // QueueStepExcutor


/* 并行分布执行 */
class ParallelStepExcutor : public IStepExcutor
{
public:
    ParallelStepExcutor() = default;
    virtual ~ParallelStepExcutor() = default;

    ParallelStepExcutor(const ParallelStepExcutor&) = delete;
    ParallelStepExcutor& operator = (const ParallelStepExcutor&) = delete;

public:
    STEP_STATUS Step() override;
    void Rollback() override { /* can do nothing */}

public:
    inline bool Empty() const { return steps_.empty(); }
    inline void Add(StepExcutorPtr ptr) { steps_.push_back(ptr); }

protected:
    std::vector<StepExcutorPtr> steps_;
}; // ParallelStepExcutor

namespace StepExcutor_Internal
{
    struct GuardImpl;
    struct GuardWithCache;
    typedef AllocatorAdapter<int8_t, SerialAllocator<>> GuardAllocator;
}

/* 步进器守卫 */
class StepGuard
{
private:
    friend StepExcutor_Internal::GuardImpl;
    friend StepExcutor_Internal::GuardWithCache;
    typedef StepExcutor_Internal::GuardAllocator Allocator;

    StepGuard(const Allocator& alloc) : guarder_(alloc)
    {
    }

    ~StepGuard()
    {
        guarder_.Dismiss();
    }

    StepGuard(const StepGuard&) = delete;
    StepGuard& operator = (const StepGuard&) = delete;

    void Rollback()
    {
        guarder_.Done();
    }

public:
    template <typename Fty>
    void Push(Fty&& func)
    {
        guarder_.Push(std::forward<Fty>(func));
    }

private:
    ScopeGuard<Allocator> guarder_;
}; // class KGStepGuard

namespace StepExcutor_Internal
{
    /* trait the excutor function return type */
    template <bool, typename Fty>
    struct ExcutorRetTypeTrait
    {
        typedef typename std_ext::invoke_result<Fty>::type type;
    };

    template <typename Fty>
    struct ExcutorRetTypeTrait<true, Fty>
    {
        typedef typename std_ext::invoke_result<Fty, StepGuard&>::type type;
    };

    /* check has guarder */
    template <typename Ty>
    struct ExcutorHasGuarder
    {
        static constexpr bool value = std_ext::is_callable<Ty, StepGuard&>::value;
    };

    template <typename Fty>
    using ExcutorRetType_t = typename ExcutorRetTypeTrait<ExcutorHasGuarder<Fty>::value, Fty>::type;

    template <typename Fty>
    struct ExcutorSignatureCheck
    {
        static constexpr bool value = std_ext::is_callable<Fty>::value ||
            (!std_ext::is_callable<Fty, const StepGuard&>::value && std_ext::is_callable<Fty, StepGuard&>::value);
    };

    template <bool, typename Fty>
    struct ExcutorCheckImpl
    {
        static constexpr bool value = std::is_void<ExcutorRetType_t<Fty>>::value ||
            std::is_same<bool, ExcutorRetType_t<Fty>>::value ||
            std::is_same<STEP_STATUS, ExcutorRetType_t<Fty>>::value;
    };

    template <typename Fty>
    struct ExcutorCheckImpl<false, Fty>
    {
        static constexpr bool value = false;
    };

    template <typename Fty>
    struct ExcutorCheck
    {
        static constexpr bool value = ExcutorCheckImpl<ExcutorSignatureCheck<Fty>::value, Fty>::value;
    };

    struct GuardImpl
    {
        GuardImpl(SerialAllocator<>* pAlloc) : m_Guarder(pAlloc->GetAdapter<int8_t>())
        {
        }

        inline StepGuard& GetGuarder() { return m_Guarder; }
        inline void Rollback() { m_Guarder.Rollback(); }

    private:
        StepGuard m_Guarder;
    };

    struct GuardWithCache
    {
        GuardWithCache(SerialAllocator<>*) : m_Guarder(m_Alloc.GetAdapter<int8_t>())
        {
        }

        inline StepGuard& GetGuarder() { return m_Guarder; }
        inline void Rollback() { m_Guarder.Rollback(); }

    private:
        PoolSerialAlloc<128> m_Alloc;
        StepGuard m_Guarder;
    };

    struct NoneGuardImpl
    {
        NoneGuardImpl(SerialAllocator<>*) { }

        inline int GetGuarder() { return 0; }
        inline void Rollback() { }
    };

    /* functor: bool(StepGuard&) */
    template <typename Fty>
    inline auto StepForward(StepGuard& guader, Fty& fn) -> typename std::enable_if<std::is_void<ExcutorRetType_t<Fty>>::value, void>::type
    {
        fn(guader);
        return STEP_STATUS::Completed;
    }

    /* functor: bool(StepGuard&) */
    template <typename Fty>
    inline auto StepForward(StepGuard& guader, Fty& fn) -> typename std::enable_if<std::is_same<bool, ExcutorRetType_t<Fty>>::value, bool>::type
    {
        return fn(guader) ? STEP_STATUS::Completed : STEP_STATUS::Failed;
    }

    /* functor: STEP_STATUS(StepGuard&) */
    template <typename Fty>
    inline auto StepForward(StepGuard& guader, Fty& fn) -> typename std::enable_if<std::is_same<STEP_STATUS, ExcutorRetType_t<Fty>>::value, STEP_STATUS>::type
    {
        return fn(guader);
    }

    /* functor: void() */
    template <typename Fty>
    inline auto StepForward(int, Fty& fn) -> typename std::enable_if<std::is_void<ExcutorRetType_t<Fty>>::value, void>::type
    {
        fn();
        return STEP_STATUS::Completed;
    }

    /* functor: bool() */
    template <typename Fty>
    inline auto StepForward(int, Fty& fn) -> typename std::enable_if<std::is_same<bool, ExcutorRetType_t<Fty>>::value, bool>::type
    {
        return fn() ? STEP_STATUS::Completed : STEP_STATUS::Failed;
    }

    /* functor: STEP_STATUS() */
    template <typename Fty>
    inline auto StepForward(int, Fty& fn) -> typename std::enable_if<std::is_same<STEP_STATUS, ExcutorRetType_t<Fty>>::value, STEP_STATUS>::type
    {
        return fn();
    }

    template <typename Fty, typename GuardType>
    struct ExcutorImpl : public IStepExcutor
    {
        ExcutorImpl(Fty&& fn, SerialAllocator<>* alloc)
            : m_Func(std::forward<Fty>(fn))
            , m_Guarder(alloc)
        {
        }

        virtual ~ExcutorImpl()
        {
        }

        STEP_STATUS Step() override
        {
            return StepForward(m_Guarder.GetGuarder(), m_Func);
        }

        void Rollback() override
        {
            m_Guarder.Rollback();
        }

        Fty m_Func;
        GuardType m_Guarder;
    };

    template <typename Rty, typename Fty, typename GuardType>
    struct ExcutorWithFuture : ExcutorImpl<Fty, GuardType>
    {
        ExcutorWithFuture(Future<Rty>&& future, Fty&& fn, SerialAllocator<>* alloc)
            : ExcutorImpl<Fty, GuardType>(std::forward<Fty>(fn), alloc)
            , m_Future(std::move(future))
        {
            assert(m_Future.IsValid());
        }

        virtual ~ExcutorWithFuture()
        {
        }

        STEP_STATUS Step() override
        {
            if (!m_Future.IsReady())
                return STEP_STATUS::Idle;
            return ExcutorImpl<Fty, GuardType>::Step();
        }

        Future<Rty> m_Future;
    };

    template <typename Rty, typename Fty, typename GuardType>
    struct ExcutorWithSharedFuture : ExcutorImpl<Fty, GuardType>
    {
        ExcutorWithSharedFuture(const SharedFuture<Rty>& future, Fty&& fn, SerialAllocator<>* alloc)
            : ExcutorImpl<Fty, GuardType>(std::forward<Fty>(fn), alloc)
            , m_Future(future)
        {
            assert(m_Future.IsValid());
        }

        virtual ~ExcutorWithSharedFuture()
        {
        }

        STEP_STATUS Step() override
        {
            if (!m_Future.IsReady())
                return STEP_STATUS::Idle;
            return ExcutorImpl<Fty, GuardType>::Step();
        }

        SharedFuture<Rty> m_Future;
    };

#define _STEP_EXCUTOR_ERROR_MESSAGE "Functor signature is not allowed! Only support functor with "  \
            "void(), void(StepGuard&), bool(), bool(StepGuard&), STEP_STATUS() and STEP_STATUS(StepGuard&)"
#define _STEP_EXCUTOR_CHECK(Fty)    static_assert(StepExcutor_Internal::ExcutorCheck<Fty>::value, _STEP_EXCUTOR_ERROR_MESSAGE)

    template <typename Fty>
    inline StepExcutorPtr MakeExcutor(Fty&& fn)
    {
        _STEP_EXCUTOR_CHECK(Fty);

        using guard_type = typename std::conditional<ExcutorHasGuarder<Fty>::value,
            GuardWithCache, NoneGuardImpl>::type;
        return std::make_shared<ExcutorImpl<Fty, guard_type>>(std::forward<Fty>(fn), nullptr);
    }

    template <typename Rty, typename Fty>
    inline StepExcutorPtr MakeExcutor(Future<Rty>&& future, Fty&& fn)
    {
        _STEP_EXCUTOR_CHECK(Fty);

        using guard_type = typename std::conditional<ExcutorHasGuarder<Fty>::value,
            GuardWithCache, NoneGuardImpl>::type;
        return std::make_shared<ExcutorWithFuture<Rty, Fty, guard_type>>(std::move(future), std::forward<Fty>(fn), nullptr);
    }

    template <typename Rty, typename Fty>
    inline StepExcutorPtr MakeExcutor(const SharedFuture<Rty>& future, Fty&& fn)
    {
        _STEP_EXCUTOR_CHECK(Fty);

        using guard_type = typename std::conditional<ExcutorHasGuarder<Fty>::value,
            GuardWithCache, NoneGuardImpl>::type;
        return std::make_shared<ExcutorWithSharedFuture<Rty, Fty, guard_type>>(future, std::forward<Fty>(fn), nullptr);
    }

    template <typename Fty>
    inline StepExcutorPtr MakeExcutor(Fty&& fn, SerialAllocator<>* alloc)
    {
        _STEP_EXCUTOR_CHECK(Fty);

        using guard_type = typename std::conditional<ExcutorHasGuarder<Fty>::value, GuardImpl, NoneGuardImpl>::type;
        using exutor_type = ExcutorImpl<Fty, guard_type>;
        return std::allocate_shared<exutor_type>(alloc->GetAdapter<exutor_type>(), std::forward<Fty>(fn), alloc);
    }

    template <typename Rty, typename Fty>
    inline StepExcutorPtr MakeExcutor(Future<Rty>&& future, Fty&& fn, SerialAllocator<>* alloc)
    {
        _STEP_EXCUTOR_CHECK(Fty);

        using guard_type = typename std::conditional<ExcutorHasGuarder<Fty>::value, GuardImpl, NoneGuardImpl>::type;
        using exutor_type = ExcutorWithFuture<Rty, Fty, guard_type>;
        return std::allocate_shared<exutor_type>(alloc->GetAdapter<exutor_type>(),
            std::move(future), std::forward<Fty>(fn), alloc);
    }

    template <typename Rty, typename Fty>
    inline StepExcutorPtr MakeExcutor(const SharedFuture<Rty>& future, Fty&& fn, SerialAllocator<>* alloc)
    {
        _STEP_EXCUTOR_CHECK(Fty);

        using guard_type = typename std::conditional<ExcutorHasGuarder<Fty>::value, GuardImpl, NoneGuardImpl>::type;
        using exutor_type = ExcutorWithSharedFuture<Rty, Fty, guard_type>;
        return std::allocate_shared<exutor_type>(alloc->GetAdapter<exutor_type>(),
            future, std::forward<Fty>(fn), alloc);
    }
} // namesapce StepExcutor_Internal

template <typename Fty>
inline StepExcutorPtr MakeStepExcutor(Fty&& fn)
{
    return StepExcutor_Internal::MakeExcutor(std::forward<Fty>(fn));
}

template <typename Rty, typename Fty>
inline StepExcutorPtr MakeStepExcutor(Future<Rty>&& future, Fty&& fn)
{
    return StepExcutor_Internal::MakeExcutor(std::move(future), std::forward<Fty>(fn));
}

template <typename Rty, typename Fty>
inline StepExcutorPtr MakeStepExcutor(const SharedFuture<Rty>& future, Fty&& fn)
{
    return StepExcutor_Internal::MakeExcutor(future, std::forward<Fty>(fn));
}

UTILITY_NAMESPACE_END
