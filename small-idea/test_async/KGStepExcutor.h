/*
 * 分步执行器
*/
#pragma once

#include <type_traits>
#include <memory>
#include <vector>
#include "KGFuture.h"
#include "KGScopeGuard.h"

/* 步进器守卫 */
class KGStepGuard;

enum class KGSTEP_RET
{
    Continue,
    Idle,
    Failed,
    Completed,
};

/* 分布执行器接口 */
struct IKGStepExcutor
{
    virtual ~IKGStepExcutor() { }
    virtual KGSTEP_RET Step() = 0;
    virtual void Rollback() = 0;
};
typedef std::shared_ptr<IKGStepExcutor> KGStepExcutorPtr;

/* 前置声明 */
namespace StepExcutor_Internal
{
    template <typename Fty>
    KGStepExcutorPtr MakeExcutor(Fty&& fn);

    template <typename Rty, typename Fty>
    KGStepExcutorPtr MakeExcutor(KGFuture<Rty>&& future, Fty&& fn);

    template <typename Rty, typename Fty>
    KGStepExcutorPtr MakeExcutor(const KGSharedFuture<Rty>& future, Fty&& fn);

    template <typename Fty>
    KGStepExcutorPtr MakeExcutor(Fty&& fn, KGSerialAllocator<>* alloc);

    template <typename Rty, typename Fty>
    KGStepExcutorPtr MakeExcutor(KGFuture<Rty>&& future, Fty&& fn, KGSerialAllocator<>* alloc);

    template <typename Rty, typename Fty>
    KGStepExcutorPtr MakeExcutor(const KGSharedFuture<Rty>& future, Fty&& fn, KGSerialAllocator<>* alloc);
}

/* 构建一个分布执行器 */
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

/* 分布执行列表, 顺序执行 */
class KGQueueStepExcutor final : public IKGStepExcutor
{
public:
    KGQueueStepExcutor() : m_Alloc(1024)
    {
    }

    virtual ~KGQueueStepExcutor();

    KGQueueStepExcutor(const KGQueueStepExcutor&) = delete;
    KGQueueStepExcutor& operator = (const KGQueueStepExcutor&) = delete;

public:
    KGSTEP_RET Step() override;
    void Rollback() override { DoRollback(); }

public:
    inline bool Empty() const { return m_Steps.empty(); }
    inline void Add(KGStepExcutorPtr ptr) { m_Steps.push_back(ptr); }

    template <typename Fty>
    inline void Add(Fty&& fn)
    {
        Add(StepExcutor_Internal::MakeExcutor(std::forward<Fty>(fn), &m_Alloc));
    }

    template <typename Rty, typename Fty>
    inline void Add(KGFuture<Rty>&& future, Fty&& fn)
    {
        //Add()
    }

private:
    void DoRollback();

private:
    size_t m_StepIndex = 0;
    KGSTEP_RET m_eRet = KGSTEP_RET::Continue;
    std::vector<KGStepExcutorPtr> m_Steps;
    KGPoolSerialAlloc<512> m_Alloc;
};

/* 并行分布执行 */
class KGParallelStepExcutor : public IKGStepExcutor
{
public:
    KGParallelStepExcutor() = default;
    virtual ~KGParallelStepExcutor() = default;

    KGParallelStepExcutor(const KGParallelStepExcutor&) = delete;
    KGParallelStepExcutor& operator = (const KGParallelStepExcutor&) = delete;

public:
    KGSTEP_RET Step() override;
    void Rollback() override;

public:
    inline bool Empty() const { return m_Steps.empty(); }
    inline void Add(KGStepExcutorPtr ptr) { m_Steps.push_back(ptr); }

    template <typename Fty>
    inline void Add(Fty&& fn)
    {
        Add(MakeStepExcutor(std::forward<Fty>(fn)));
    }

protected:
    std::vector<KGStepExcutorPtr> m_Steps;
};

namespace StepExcutor_Internal
{
    struct GuardImpl;
    /* 适配KGScopeGuard分配器 */
    struct GuardAllocator
    {
        GuardAllocator() : m_pAlloc(nullptr)
        {
            assert(false);
        }

        GuardAllocator(KGSerialAllocator<>* pAlloc) : m_pAlloc(pAlloc)
        {
        }

        void* Alloc(size_t sz)
        {
            return m_pAlloc->Alloc(sz);
        }

        void Dealloc(void*, size_t)
        {
        }

        KGSerialAllocator<>* m_pAlloc;
    };
}

/* 步进器守卫 */
class KGStepGuard
{
private:
    friend StepExcutor_Internal::GuardImpl;
    typedef StepExcutor_Internal::GuardAllocator Allocator;

    KGStepGuard(const Allocator& alloc)// : m_Guarder(std::allocator_arg_t(), alloc)
    {
    }

    ~KGStepGuard()
    {
        //m_Guarder.Dismiss();
    }

    KGStepGuard(const KGStepGuard&) = delete;
    KGStepGuard& operator = (const KGStepGuard&) = delete;

    void Rollback()
    {
        //m_Guarder.Done();
    }

public:
    template <typename Fty>
    void Push(Fty&& func)
    {
        //m_Guarder.Push(std::forward<Fty>(func));
    }

private:
    //KGScopeGuardImpl<Allocator> m_Guarder;
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
        KGSTEP_RET
    >::type;

    template <typename Fty>
    struct ExcutorSignatureCheck
    {
        static constexpr bool value = utility::is_callable<Fty>::value ||
            (!utility::is_callable<Fty, KGStepGuard&&>::value && utility::is_callable<Fty, KGStepGuard&>::value);
    };

    template <bool, typename Fty>
    struct ExcutorCheckImpl
    {
        static constexpr bool value = std::is_void<ExcutorRetType_t<Fty>>::value ||
            std::is_same<bool, ExcutorRetType_t<Fty>>::value ||
            std::is_same<KGSTEP_RET, ExcutorRetType_t<Fty>>::value;
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

    struct GuardWithAlloc : GuardImpl
    {
        GuardWithAlloc() : GuardImpl(GuardAllocator(&m_Alloc))
        {
        }

        KGPoolSerialAlloc<128> m_Alloc;
    };

    struct NoneGuardImpl
    {
        void Rollback()
        {
        }
    };

    /* functor: bool(KGStepGuard&) */
    template <typename Fty>
    inline auto StepForward(GuardImpl& guader, const Fty& fn) -> ExcutorEnableIf_t<Fty, void(KGStepGuard&)>
    {
        fn(guader.GetGuarder());
        return KGSTEP_RET::Completed;
    }

    /* functor: bool(KGStepGuard&) */
    template <typename Fty>
    inline auto StepForward(GuardImpl& guader, const Fty& fn) -> ExcutorEnableIf_t<Fty, bool(KGStepGuard&)>
    {
        return fn(guader.GetGuarder()) ? KGSTEP_RET::Completed : KGSTEP_RET::Failed;
    }

    /* functor: KGSTEP_RET(KGStepGuard&) */
    template <typename Fty>
    inline auto StepForward(GuardImpl& guader, const Fty& fn) -> ExcutorEnableIf_t<Fty, KGSTEP_RET(KGStepGuard&)>
    {
        return fn(guader.GetGuarder());
    }

    /* functor: void() */
    template <typename Fty>
    inline auto StepForward(const NoneGuardImpl&, const Fty& fn) -> ExcutorEnableIf_t<Fty, void()>
    {
        fn();
        return KGSTEP_RET::Completed;
    }

    /* functor: bool() */
    template <typename Fty>
    inline auto StepForward(const NoneGuardImpl&, const Fty& fn) -> ExcutorEnableIf_t<Fty, bool()>
    {
        return fn() ? KGSTEP_RET::Completed : KGSTEP_RET::Failed;
    }

    /* functor: KGSTEP_RET() */
    template <typename Fty>
    inline auto StepForward(const NoneGuardImpl&, const Fty& fn) -> ExcutorEnableIf_t<Fty, KGSTEP_RET()>
    {
        return fn();
    }

    template <typename Fty, typename GuardType>
    struct ExcutorImpl : public IKGStepExcutor
    {
        template <class = typename std::enable_if<!std::is_same<GuardType, GuardImpl>::value, void>::type>
        ExcutorImpl(Fty&& fn)
            : m_Func(std::forward<Fty>(fn))
        {
        }

        template <class = typename std::enable_if<std::is_same<GuardType, GuardImpl>::value, void>::type>
        ExcutorImpl(Fty&& fn, const GuardAllocator& alloc)
            : m_Func(std::forward<Fty>(fn))
            , m_Guarder(alloc)
        {
        }

        virtual ~ExcutorImpl()
        {
        }

        KGSTEP_RET Step() override
        {
            return StepForward(m_Guarder, m_Func);
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
        template <class = typename std::enable_if<!std::is_same<GuardType, GuardImpl>::value, void>::type>
        ExcutorWithFuture(KGFuture<Rty>&& future, Fty&& fn)
            : ExcutorImpl<Fty, GuardType>(std::forward<Fty>(fn))
            , m_Future(std::move(future))
        {
        }

        template <class = typename std::enable_if<std::is_same<GuardType, GuardImpl>::value, void>::type>
        ExcutorWithFuture(KGFuture<Rty>&& future, Fty&& fn, const GuardAllocator& alloc)
            : ExcutorImpl<Fty, GuardType>(std::forward<Fty>(fn), std::forward<GuardType>(alloc))
            , m_Future(std::move(future))
        {
        }

        KGSTEP_RET Step() override
        {
            if (!m_Future.IsReady())
                return KGSTEP_RET::Idle;
            return ExcutorImpl<Fty, GuardType>::Step();
        }

        KGFuture<Rty> m_Future;
    };

    template <typename Rty, typename Fty, typename GuardType>
    struct ExcutorWithSharedFuture : ExcutorImpl<Fty, GuardType>
    {
        template <class = typename std::enable_if<!std::is_same<GuardType, GuardImpl>::value, void>::type>
        ExcutorWithSharedFuture(const KGSharedFuture<Rty>& future, Fty&& fn)
            : ExcutorImpl<Fty, GuardType>(std::forward<Fty>(fn))
            , m_Future(future)
        {
        }

        template <class = typename std::enable_if<std::is_same<GuardType, GuardImpl>::value, void>::type>
        ExcutorWithSharedFuture(const KGSharedFuture<Rty>& future, Fty&& fn, const GuardAllocator& alloc)
            : ExcutorImpl<Fty, GuardType>(std::forward<Fty>(fn),std::forward<GuardType>(alloc))
            , m_Future(future)
        {
        }

        KGSTEP_RET Step() override
        {
            if (!m_Future.IsReady())
                return KGSTEP_RET::Idle;
            return ExcutorImpl<Fty, GuardType>::Step();
        }

        KGSharedFuture<Rty> m_Future;
    };

#define _STEP_EXCUTOR_ERROR_MESSAGE "Functor signature is not allowed! Only support functor with "  \
            "void(), void(KGStepGuard&), bool(), bool(KGStepGuard&), KGSTEP_RET() and KGSTEP_RET(KGStepGuard&)"
#define _STEP_EXCUTOR_CHECK(Fty)    static_assert(StepExcutor_Internal::ExcutorCheck<Fty>::value, _STEP_EXCUTOR_ERROR_MESSAGE)

    template <typename Fty>
    inline KGStepExcutorPtr MakeExcutor(Fty&& fn)
    {
        _STEP_EXCUTOR_CHECK(Fty);

        using guard_type = typename std::conditional<ExcutorHasGuarder<Fty>::value,
            GuardWithAlloc, NoneGuardImpl>::type;
        return std::make_shared<ExcutorImpl<Fty, guard_type>>(std::forward<Fty>(fn));
    }

    template <typename Rty, typename Fty>
    KGStepExcutorPtr MakeExcutor(KGFuture<Rty>&& future, Fty&& fn)
    {
        _STEP_EXCUTOR_CHECK(Fty);

        using guard_type = typename std::conditional<ExcutorHasGuarder<Fty>::value,
            GuardWithAlloc, NoneGuardImpl>::type;
        return std::make_shared<ExcutorWithFuture<Rty, Fty, guard_type>>(std::move(future), std::forward<Fty>(fn));
    }

    template <typename Rty, typename Fty>
    KGStepExcutorPtr MakeExcutor(const KGSharedFuture<Rty>& future, Fty&& fn)
    {
        _STEP_EXCUTOR_CHECK(Fty);

        using guard_type = typename std::conditional<ExcutorHasGuarder<Fty>::value,
            GuardWithAlloc, NoneGuardImpl>::type;
        return std::make_shared<ExcutorWithSharedFuture<Rty, Fty, guard_type>>(future, std::forward<Fty>(fn));
    }

    template <typename Fty>
    KGStepExcutorPtr MakeExcutor(Fty&& fn, KGSerialAllocator<>* alloc)
    {
        using guard_type = typename std::conditional<ExcutorHasGuarder<Fty>::value,
            GuardImpl, NoneGuardImpl>::type;
        return std::allocate_shared<ExcutorImpl>(alloc->GetAdapter<ExcutorImpl>(), std::forward<Fty>(fn));
    }

    template <typename Rty, typename Fty>
    KGStepExcutorPtr MakeExcutor(KGFuture<Rty>&& future, Fty&& fn, KGSerialAllocator<>* alloc)
    {
        using guard_type = typename std::conditional<ExcutorHasGuarder<Fty>::value,
            GuardImpl, NoneGuardImpl>::type;
        return std::allocate_shared<ExcutorWithFuture>(alloc->GetAdapter<ExcutorWithFuture>(), std::forward<Fty>(fn));
    }

    template <typename Rty, typename Fty>
    KGStepExcutorPtr MakeExcutor(const KGSharedFuture<Rty>& future, Fty&& fn, KGSerialAllocator<>* alloc)
    {
        using guard_type = typename std::conditional<ExcutorHasGuarder<Fty>::value,
            GuardImpl, NoneGuardImpl>::type;
        return std::allocate_shared<ExcutorWithSharedFuture>(alloc->GetAdapter<ExcutorWithSharedFuture>(), std::forward<Fty>(fn));
    }
} // namesapce StepExcutor_Internal
