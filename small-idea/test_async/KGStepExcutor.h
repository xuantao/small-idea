/*
 * 分步执行器
*/
#pragma once

#include <type_traits>
#include <memory>
#include <vector>
#include "KGFuture.h"
#include "KGScopeGuard.h"

enum class KGSTEP_STATUS
{
    Busy,       // 繁忙
    Idle,       // 空闲
    Failed,     // 失败
    Completed,  // 结束
};

/* 判断分布执行器是否结束 */
#define IS_STEP_STOPPED(eStatus)    (eStatus == KGSTEP_STATUS::Failed || eStatus == KGSTEP_STATUS::Completed)

/* 步进器守卫 */
class KGStepGuard;

/* 分布执行器接口 */
struct IKGStepExcutor
{
    virtual ~IKGStepExcutor() { }

    /* 向前走一步 */
    virtual KGSTEP_STATUS Step() = 0;

    /* 错误回滚 */
    virtual void Rollback() = 0;
};
typedef std::shared_ptr<IKGStepExcutor> KGStepExcutorPtr;


/* 构建一个分布执行器
 * Fty支持的函数签名为:
 * 1. void(), void(KStepGuard&)
 * 2. bool(), bool(KGStepGuard&),
 * 3. KGSTEP_STATUS(), KGSTEP_STATUS(KGStepGuard&)
*/
template <typename Fty>
KGStepExcutorPtr MakeStepExcutor(Fty&& fn);

template <typename Rty, typename Fty>
KGStepExcutorPtr MakeStepExcutor(KGFuture<Rty>&& future, Fty&& fn);

template <typename Rty, typename Fty>
KGStepExcutorPtr MakeStepExcutor(const KGSharedFuture<Rty>& future, Fty&& fn);

/* 执行一段固定时间(单位毫秒)
 * 当步进器遇到Idle, Failed, Completed结束
*/
KGSTEP_STATUS StepFor(IKGStepExcutor* pSteper, size_t nDuration);
inline KGSTEP_STATUS StepFor(KGStepExcutorPtr pSteper, size_t nDuration) { return StepFor(pSteper.get(), nDuration); }

namespace StepExcutor_Internal
{
    template <typename Fty> KGStepExcutorPtr MakeExcutor(Fty&& fn, KGSerialAllocator<>* alloc);
    template <typename Rty, typename Fty> KGStepExcutorPtr MakeExcutor(KGFuture<Rty>&& future, Fty&& fn, KGSerialAllocator<>* alloc);
    template <typename Rty, typename Fty> KGStepExcutorPtr MakeExcutor(const KGSharedFuture<Rty>& future, Fty&& fn, KGSerialAllocator<>* alloc);
}
/* 分布执行列表, 顺序执行 */
class KGQueueStepExcutor final : public IKGStepExcutor
{
public:
    KGQueueStepExcutor() : m_Alloc(1024) { }
    KGQueueStepExcutor(size_t cache_block_size) : m_Alloc(cache_block_size) {}

    virtual ~KGQueueStepExcutor();

    KGQueueStepExcutor(const KGQueueStepExcutor&) = delete;
    KGQueueStepExcutor& operator = (const KGQueueStepExcutor&) = delete;

public:
    KGSTEP_STATUS Step() override;
    void Rollback() override { DoRollback(); }

public:
    inline bool Empty() const { return m_Steps.empty(); }
    inline void Add(KGStepExcutorPtr ptr) { m_Steps.push_back(ptr); }

    template <typename Fty>
    inline void Add(Fty&& fn)
    {
        Add(StepExcutor_Internal::MakeExcutor(
            std::forward<Fty>(fn), reinterpret_cast<KGSerialAllocator<>*>(&m_Alloc)));
    }

    template <typename Rty, typename Fty>
    inline void Add(KGFuture<Rty>&& future, Fty&& fn)
    {
        Add(StepExcutor_Internal::MakeExcutor(
            std::move(future), std::forward<Fty>(fn), reinterpret_cast<KGSerialAllocator<>*>(&m_Alloc)));
    }

    template <typename Rty, typename Fty>
    inline void Add(const KGSharedFuture<Rty>& future, Fty&& fn)
    {
        Add(StepExcutor_Internal::MakeExcutor(
            future, std::forward<Fty>(fn), reinterpret_cast<KGSerialAllocator<>*>(&m_Alloc)));
    }

private:
    void DoRollback();

private:
    size_t m_StepIndex = 0;
    KGSTEP_STATUS m_eRet = KGSTEP_STATUS::Idle;
    std::vector<KGStepExcutorPtr> m_Steps;
    KGPoolSerialAlloc<512> m_Alloc;
}; // KGQueueStepExcutor


/* 并行分布执行 */
class KGParallelStepExcutor : public IKGStepExcutor
{
public:
    KGParallelStepExcutor() = default;
    virtual ~KGParallelStepExcutor() = default;

    KGParallelStepExcutor(const KGParallelStepExcutor&) = delete;
    KGParallelStepExcutor& operator = (const KGParallelStepExcutor&) = delete;

public:
    KGSTEP_STATUS Step() override;
    void Rollback() override { /* can do nothing */}

public:
    inline bool Empty() const { return m_Steps.empty(); }
    inline void Add(KGStepExcutorPtr ptr) { m_Steps.push_back(ptr); }

protected:
    std::vector<KGStepExcutorPtr> m_Steps;
}; // KGParallelStepExcutor

namespace StepExcutor_Internal
{
    struct GuardImpl;
    struct GuardWithCache;
    typedef KGAllocatorAdapter<int8_t, KGSerialAllocator<>> GuardAllocator;
}

/* 步进器守卫 */
class KGStepGuard
{
private:
    friend StepExcutor_Internal::GuardImpl;
    friend StepExcutor_Internal::GuardWithCache;
    typedef StepExcutor_Internal::GuardAllocator Allocator;

    KGStepGuard(const Allocator& alloc) : m_Guarder(alloc)
    {
    }

    ~KGStepGuard()
    {
        m_Guarder.Dismiss();
    }

    KGStepGuard(const KGStepGuard&) = delete;
    KGStepGuard& operator = (const KGStepGuard&) = delete;

    void Rollback()
    {
        m_Guarder.Done();
    }

public:
    template <typename Fty>
    void Push(Fty&& func)
    {
        m_Guarder.Push(std::forward<Fty>(func));
    }

private:
    KGScopeGuardImpl<Allocator> m_Guarder;
}; // class KGStepGuard

namespace StepExcutor_Internal
{
    template <typename Ty>
    struct FunctorTrait;

    template <typename R, typename A>
    struct FunctorTrait<R(A)>
    {
        typedef R ret_type;
        typedef A arg_type;
    };

    template <typename R>
    struct FunctorTrait<R()>
    {
        typedef R ret_type;
        typedef void arg_type;
    };

    template <>
    struct FunctorTrait<void()>
    {
        typedef void ret_type;
        typedef void arg_type;
    };

    template <typename Ty>
    using FunctorRetType_t = typename FunctorTrait<Ty>::ret_type;
    template <typename Ty>
    using FunctorArgType_t = typename FunctorTrait<Ty>::arg_type;

    /* trait the excutor function return type */
    template <bool, typename Fty>
    struct ExcutorRetTypeTrait
    {
        typedef typename utility::invoke_result<Fty>::type type;
    };

    template <typename Fty>
    struct ExcutorRetTypeTrait<true, Fty>
    {
        typedef typename utility::invoke_result<Fty, KGStepGuard&>::type type;
    };

    /* check has guarder */
    template <typename Ty>
    struct ExcutorHasGuarder
    {
        static constexpr bool value = utility::is_callable<Ty, KGStepGuard&>::value;
    };

    template <typename Fty>
    using ExcutorRetType_t = typename ExcutorRetTypeTrait<ExcutorHasGuarder<Fty>::value, Fty>::type;

    template <typename Fty, typename Call>
    using ExcutorEnableIf_t = typename std::enable_if<
        utility::is_callable<Fty, FunctorArgType_t<Call>>::value && std::is_same<ExcutorRetType_t<Fty>, FunctorRetType_t<Call>>::value,
        KGSTEP_STATUS
    >::type;

    template <typename Fty>
    struct ExcutorSignatureCheck
    {
        static constexpr bool value = utility::is_callable<Fty>::value ||
            (!utility::is_callable<Fty, const KGStepGuard&>::value && utility::is_callable<Fty, KGStepGuard&>::value);
    };

    template <bool, typename Fty>
    struct ExcutorCheckImpl
    {
        static constexpr bool value = std::is_void<ExcutorRetType_t<Fty>>::value ||
            std::is_same<bool, ExcutorRetType_t<Fty>>::value ||
            std::is_same<KGSTEP_STATUS, ExcutorRetType_t<Fty>>::value;
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
        GuardImpl(KGSerialAllocator<>* pAlloc) : m_Guarder(pAlloc->GetAdapter<int8_t>())
        {
        }

        GuardImpl(const GuardAllocator& alloc) : m_Guarder(alloc)
        {
        }

        ~GuardImpl()
        {
        }

        inline KGStepGuard& GetGuarder()
        {
            return m_Guarder;
        }

        inline void Rollback()
        {
            m_Guarder.Rollback();
        }

    private:
        KGStepGuard m_Guarder;
    };

    struct GuardWithCache
    {
        GuardWithCache(KGSerialAllocator<>*) : m_Guarder(m_Alloc.GetAdapter<int8_t>())
        {
        }

        inline KGStepGuard& GetGuarder()
        {
            return m_Guarder;
        }

        inline void Rollback()
        {
            m_Guarder.Rollback();
        }

    private:
        KGPoolSerialAlloc<128> m_Alloc;
        KGStepGuard m_Guarder;
    };

    struct NoneGuardImpl
    {
        NoneGuardImpl(KGSerialAllocator<>*) { }

        inline int GetGuarder() { return 0; }
        void Rollback() { }
    };

    /* functor: bool(KGStepGuard&) */
    template <typename Fty>
    inline auto StepForward(KGStepGuard& guader, Fty& fn) -> ExcutorEnableIf_t<Fty, void(KGStepGuard&)>
    {
        fn(guader);
        return KGSTEP_STATUS::Completed;
    }

    /* functor: bool(KGStepGuard&) */
    template <typename Fty>
    inline auto StepForward(KGStepGuard& guader, Fty& fn) -> ExcutorEnableIf_t<Fty, bool(KGStepGuard&)>
    {
        return fn(guader) ? KGSTEP_STATUS::Completed : KGSTEP_STATUS::Failed;
    }

    /* functor: KGSTEP_STATUS(KGStepGuard&) */
    template <typename Fty>
    inline auto StepForward(KGStepGuard& guader, Fty& fn) -> ExcutorEnableIf_t<Fty, KGSTEP_STATUS(KGStepGuard&)>
    {
        return fn(guader);
    }

    /* functor: void() */
    template <typename Fty>
    inline auto StepForward(int, Fty& fn) -> ExcutorEnableIf_t<Fty, void()>
    {
        fn();
        return KGSTEP_STATUS::Completed;
    }

    /* functor: bool() */
    template <typename Fty>
    inline auto StepForward(int, Fty& fn) -> ExcutorEnableIf_t<Fty, bool()>
    {
        return fn() ? KGSTEP_STATUS::Completed : KGSTEP_STATUS::Failed;
    }

    /* functor: KGSTEP_STATUS() */
    template <typename Fty>
    inline auto StepForward(int, Fty& fn) -> ExcutorEnableIf_t<Fty, KGSTEP_STATUS()>
    {
        return fn();
    }

    template <typename Fty, typename GuardType>
    struct ExcutorImpl : public IKGStepExcutor
    {
        ExcutorImpl(Fty&& fn, KGSerialAllocator<>* alloc)
            : m_Func(std::forward<Fty>(fn))
            , m_Guarder(alloc)
        {
        }

        virtual ~ExcutorImpl()
        {
        }

        KGSTEP_STATUS Step() override
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
        ExcutorWithFuture(KGFuture<Rty>&& future, Fty&& fn, KGSerialAllocator<>* alloc)
            : ExcutorImpl<Fty, GuardType>(std::forward<Fty>(fn), alloc)
            , m_Future(std::move(future))
        {
            assert(m_Future.IsValid());
        }

        KGSTEP_STATUS Step() override
        {
            if (!m_Future.IsReady())
                return KGSTEP_STATUS::Idle;
            return ExcutorImpl<Fty, GuardType>::Step();
        }

        KGFuture<Rty> m_Future;
    };

    template <typename Rty, typename Fty, typename GuardType>
    struct ExcutorWithSharedFuture : ExcutorImpl<Fty, GuardType>
    {
        ExcutorWithSharedFuture(const KGSharedFuture<Rty>& future, Fty&& fn, KGSerialAllocator<>* alloc)
            : ExcutorImpl<Fty, GuardType>(std::forward<Fty>(fn), alloc)
            , m_Future(future)
        {
            assert(m_Future.IsValid());
        }

        KGSTEP_STATUS Step() override
        {
            if (!m_Future.IsReady())
                return KGSTEP_STATUS::Idle;
            return ExcutorImpl<Fty, GuardType>::Step();
        }

        KGSharedFuture<Rty> m_Future;
    };

#define _STEP_EXCUTOR_ERROR_MESSAGE "Functor signature is not allowed! Only support functor with "  \
            "void(), void(KGStepGuard&), bool(), bool(KGStepGuard&), KGSTEP_STATUS() and KGSTEP_STATUS(KGStepGuard&)"
#define _STEP_EXCUTOR_CHECK(Fty)    static_assert(StepExcutor_Internal::ExcutorCheck<Fty>::value, _STEP_EXCUTOR_ERROR_MESSAGE)

    template <typename Fty>
    inline KGStepExcutorPtr MakeExcutor(Fty&& fn)
    {
        _STEP_EXCUTOR_CHECK(Fty);

        using guard_type = typename std::conditional<ExcutorHasGuarder<Fty>::value,
            GuardWithCache, NoneGuardImpl>::type;
        return std::make_shared<ExcutorImpl<Fty, guard_type>>(std::forward<Fty>(fn), nullptr);
    }

    template <typename Rty, typename Fty>
    inline KGStepExcutorPtr MakeExcutor(KGFuture<Rty>&& future, Fty&& fn)
    {
        _STEP_EXCUTOR_CHECK(Fty);

        using guard_type = typename std::conditional<ExcutorHasGuarder<Fty>::value,
            GuardWithCache, NoneGuardImpl>::type;
        return std::make_shared<ExcutorWithFuture<Rty, Fty, guard_type>>(std::move(future), std::forward<Fty>(fn), nullptr);
    }

    template <typename Rty, typename Fty>
    inline KGStepExcutorPtr MakeExcutor(const KGSharedFuture<Rty>& future, Fty&& fn)
    {
        _STEP_EXCUTOR_CHECK(Fty);

        using guard_type = typename std::conditional<ExcutorHasGuarder<Fty>::value,
            GuardWithCache, NoneGuardImpl>::type;
        return std::make_shared<ExcutorWithSharedFuture<Rty, Fty, guard_type>>(future, std::forward<Fty>(fn), nullptr);
    }

    template <typename Fty>
    inline KGStepExcutorPtr MakeExcutor(Fty&& fn, KGSerialAllocator<>* alloc)
    {
        _STEP_EXCUTOR_CHECK(Fty);

        using guard_type = typename std::conditional<ExcutorHasGuarder<Fty>::value, GuardImpl, NoneGuardImpl>::type;
        using exutor_type = ExcutorImpl<Fty, guard_type>;
        return std::allocate_shared<exutor_type>(alloc->GetAdapter<exutor_type>(), std::forward<Fty>(fn), alloc);
    }

    template <typename Rty, typename Fty>
    inline KGStepExcutorPtr MakeExcutor(KGFuture<Rty>&& future, Fty&& fn, KGSerialAllocator<>* alloc)
    {
        _STEP_EXCUTOR_CHECK(Fty);

        using guard_type = typename std::conditional<ExcutorHasGuarder<Fty>::value, GuardImpl, NoneGuardImpl>::type;
        using exutor_type = ExcutorWithFuture<Rty, Fty, guard_type>;
        return std::allocate_shared<exutor_type>(alloc->GetAdapter<exutor_type>(),
            std::move(future), std::forward<Fty>(fn), alloc);
    }

    template <typename Rty, typename Fty>
    inline KGStepExcutorPtr MakeExcutor(const KGSharedFuture<Rty>& future, Fty&& fn, KGSerialAllocator<>* alloc)
    {
        _STEP_EXCUTOR_CHECK(Fty);

        using guard_type = typename std::conditional<ExcutorHasGuarder<Fty>::value, GuardImpl, NoneGuardImpl>::type;
        using exutor_type = ExcutorWithSharedFuture<Rty, Fty, guard_type>;
        return std::allocate_shared<exutor_type>(alloc->GetAdapter<exutor_type>(),
            future, std::forward<Fty>(fn), alloc);
    }
} // namesapce StepExcutor_Internal

/* 构建一个分布执行器
 * Fty支持的函数签名为:
 * 1. void(), void(KStepGuard&)
 * 2. bool(), bool(KGStepGuard&),
 * 3. KGSTEP_STATUS(), KGSTEP_STATUS(KGStepGuard&)
*/
template <typename Fty>
inline KGStepExcutorPtr MakeStepExcutor(Fty&& fn)
{
    return StepExcutor_Internal::MakeExcutor(std::forward<Fty>(fn));
}

template <typename Rty, typename Fty>
inline KGStepExcutorPtr MakeStepExcutor(KGFuture<Rty>&& future, Fty&& fn)
{
    return StepExcutor_Internal::MakeExcutor(std::move(future), std::forward<Fty>(fn));
}

template <typename Rty, typename Fty>
inline KGStepExcutorPtr MakeStepExcutor(const KGSharedFuture<Rty>& future, Fty&& fn)
{
    return StepExcutor_Internal::MakeExcutor(future, std::forward<Fty>(fn));
}
