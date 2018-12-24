/*
 * 分步执行器
*/
#pragma once

#include <type_traits>
#include <memory>
#include <vector>
#include "KGAsync.h"
#include "KGScopeGuard.h"

enum class KGSTEP_RET
{
    Continue,
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

namespace StepExcutor_Internal
{
    struct guard_allocator
    {
        guard_allocator() : m_pAlloc(nullptr) { assert(false); }
        guard_allocator(KGSerialAllocator<>* pAlloc) : m_pAlloc(pAlloc) { }

        void* Alloc(size_t sz) { return m_pAlloc->Alloc(sz); }
        void Dealloc(void*, size_t) { }

        KGSerialAllocator<>* m_pAlloc;
    };

    struct guarder_impl;

    template <typename Fty>
    KGStepExcutorPtr make_excutor(Fty&& fn);

    template <typename Fty>
    KGStepExcutorPtr make_excutor(Fty&& fn, KGSerialAllocator<>* alloc);
}

/* 步进器守卫 */
class KGStepGuard
{
private:
    friend StepExcutor_Internal::guarder_impl;
    typedef StepExcutor_Internal::guard_allocator Allocator;

    KGStepGuard(const Allocator& alloc) : m_Guarder(std::allocator_arg_t(), alloc)
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
        m_Guarder.Done;
    }

public:
    template <typename Fty>
    void Push(Fty&& func)
    {
        m_Guarder.Push(std::forward<Fty>(func));
    }

private:
    KGScopeGuardImpl<Allocator> m_Guarder;
};

namespace StepExcutor_Internal
{
    template <typename Ty>
    struct functor_traits;

    template <typename R, typename A>
    struct functor_traits<R(A)>
    {
        typedef R ret_type;
        typedef A arg_type;
    };

    template <typename R>
    struct functor_traits<R()>
    {
        typedef R ret_type;
        typedef void arg_type;
    };

    template <>
    struct functor_traits<void()>
    {
        typedef void ret_type;
        typedef void arg_type;
    };

    template <typename Ty>
    using functor_ret_type_t = typename functor_traits<Ty>::ret_type;

    template <typename Ty>
    using functor_arg_type_t = typename functor_traits<Ty>::arg_type;

    template <bool, typename Fty>
    struct ret_type_traits
    {
        typedef typename utility::invoke_result<Fty>::type type;
    };

    template <typename Fty>
    struct ret_type_traits<true, Fty>
    {
        typedef typename utility::invoke_result<Fty, KGStepGuard&>::type type;
    };

    /* check has guarder */
    template <typename Ty>
    struct excutor_has_guarder
    {
        static constexpr bool value = utility::is_callable<Ty, KGStepGuard&>::value;
    };

    template <typename Fty>
    using excutor_ret_type_t = typename ret_type_traits<excutor_has_guarder<Fty>::value, Fty>::type;

    template <typename Fty, typename Call>
    using excutor_enable_if_t = typename std::enable_if<
        utility::is_callable<Fty, functor_arg_type_t<Call>>::value && std::is_same<excutor_ret_type_t<Fty>, functor_ret_type_t<Call>>::value, KGSTEP_RET>::type;

    template <typename Fty>
    struct excutor_signature_checker
    {
        static constexpr bool value = utility::is_callable<Fty>::value ||
            (!utility::is_callable<Fty, KGStepGuard&&>::value && utility::is_callable<Fty, KGStepGuard&>::value);
    };

    template <bool, typename Fty>
    struct excutor_checker_impl
    {
        static constexpr bool value = std::is_void<excutor_ret_type_t<Fty>>::value ||
            std::is_same<bool, excutor_ret_type_t<Fty>>::value ||
            std::is_same<KGSTEP_RET, excutor_ret_type_t<Fty>>::value;
    };

    template <typename Fty>
    struct excutor_checker_impl<false, Fty>
    {
        static constexpr bool value = false;
    };

    template <typename Fty>
    struct excutor_checker
    {
        static constexpr bool value = excutor_checker_impl<excutor_signature_checker<Fty>::value, Fty>::value;
    };

#define _STEP_EXCUTOR_ERROR_MESSAGE "Functor signature is not allowed! Only support functor with "  \
            "void(), void(KGStepGuard&), bool(), bool(KGStepGuard&), KGSTEP_RET() and KGSTEP_RET(KGStepGuard&)"
#define _STEP_EXCUTOR_CHECK(Fty)    static_assert(StepExcutor_Internal::excutor_checker<Fty>::value, _STEP_EXCUTOR_ERROR_MESSAGE)

    struct guarder_impl
    {
        guarder_impl(const guard_allocator& alloc) : m_Guarder(alloc)
        {
        }

        ~guarder_impl()
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

    struct guarder_with_alloc : guarder_impl
    {
        guarder_with_alloc() : guarder_impl(guard_allocator(&m_Alloc))
        {
        }

        KGPoolSerialAlloc<128> m_Alloc;
    };

    struct none_guarder
    {
        void Rollback()
        {
        }
    };

    /* functor: bool(KGStepGuard&) */
    template <typename Fty>
    inline auto StepForward(guarder_impl& guader, const Fty& fn) -> excutor_enable_if_t<Fty, void(KGStepGuard&)>
    {
        fn(guader.GetGuarder());
        return KGSTEP_RET::Completed;
    }

    /* functor: bool(KGStepGuard&) */
    template <typename Fty>
    inline auto StepForward(guarder_impl& guader, const Fty& fn) -> excutor_enable_if_t<Fty, bool(KGStepGuard&)>
    {
        return fn(guader.GetGuarder()) ? KGSTEP_RET::Completed : KGSTEP_RET::Failed;
    }

    /* functor: KGSTEP_RET(KGStepGuard&) */
    template <typename Fty>
    inline auto StepForward(guarder_impl& guader, const Fty& fn) -> excutor_enable_if_t<Fty, KGSTEP_RET(KGStepGuard&)>
    {
        return fn(guader.GetGuarder());
    }

    /* functor: void() */
    template <typename Fty>
    inline auto StepForward(const none_guarder&, const Fty& fn) -> excutor_enable_if_t<Fty, void()>
    {
        fn();
        return KGSTEP_RET::Completed;
    }

    /* functor: bool() */
    template <typename Fty>
    inline auto StepForward(const none_guarder&, const Fty& fn) -> excutor_enable_if_t<Fty, bool()>
    {
        return fn() ? KGSTEP_RET::Completed : KGSTEP_RET::Failed;
    }

    /* functor: KGSTEP_RET() */
    template <typename Fty>
    inline auto StepForward(const none_guarder&, const Fty& fn) -> excutor_enable_if_t<Fty, KGSTEP_RET()>
    {
        return fn();
    }

    template <typename Fty, typename GuardType>
    struct excutor_impl : public IKGStepExcutor
    {
        template <class = typename std::enable_if<!std::is_same<GuardType, guarder_impl>::value, void>::type>
        excutor_impl(Fty&& fn)
            : m_Func(std::forward<Fty>(fn))
        {
        }

        template <class = typename std::enable_if<std::is_same<GuardType, guarder_impl>::value, void>::type>
        excutor_impl(Fty&& fn, const guard_allocator& alloc)
            : m_Func(std::forward<Fty>(fn))
            , m_Guarder(std::forward<GuardType>(alloc))
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

    template <typename Fty>
    inline KGStepExcutorPtr MakeStepExcutor(Fty&& fn)
    {
        _STEP_EXCUTOR_CHECK(Fty);

        using guard_type = typename std::conditional<StepExcutor_Internal::excutor_has_guarder<Fty>::value,
            StepExcutor_Internal::guarder_with_alloc, StepExcutor_Internal::none_guarder>::type;
        return std::make_shared<StepExcutor_Internal::excutor_impl<Fty, guard_type>>(std::forward<Fty>(fn));
    }

    template <typename Fty>
    inline KGStepExcutorPtr MakeStepExcutor(Fty&& fn, KGSerialAllocator<>* alloc)
    {
        _STEP_EXCUTOR_CHECK(Fty);

        using guard_type = typename std::conditional<StepExcutor_Internal::excutor_has_guarder<Fty>::value,
            StepExcutor_Internal::guarder_with_alloc, StepExcutor_Internal::none_guarder>::type;
        return std::make_shared<StepExcutor_Internal::excutor_impl<Fty, guard_type>>(std::forward<Fty>(fn));
    }
}

/* 构建一个分布执行器 */
template <typename Fty>
inline KGStepExcutorPtr MakeStepExcutor(Fty&& fn)
{
    _STEP_EXCUTOR_CHECK(Fty);

    using guard_type = typename std::conditional<StepExcutor_Internal::excutor_has_guarder<Fty>::value,
        StepExcutor_Internal::guarder_with_alloc, StepExcutor_Internal::none_guarder>::type;
    return std::make_shared<StepExcutor_Internal::ExcutorImpl<Fty, guard_type>>(std::forward<Fty>(fn));
}

/* 分布执行列表, 顺序执行 */
class KGQueueStepExcutor final : public IKGStepExcutor
{
public:
    KGQueueStepExcutor() = default;
    virtual ~KGQueueStepExcutor();

    KGQueueStepExcutor(const KGQueueStepExcutor&) = delete;
    KGQueueStepExcutor& operator = (const KGQueueStepExcutor&) = delete;

public:
    inline bool Empty() const { return m_Steps.empty(); }
    inline void Add(KGStepExcutorPtr ptr) { m_Steps.push_back(ptr); }

    template <typename Fty>
    inline void Add(Fty&& fn)
    {
        Add(MakeStepExcutor(std::forward<Fty>(fn)));
    }

    KGSTEP_RET Step() override;
    void Rollback() override { DoRollback(); }

private:
    void DoRollback();

private:
    size_t m_StepIndex = 0;
    KGSTEP_RET m_eRet = KGSTEP_RET::Continue;
    std::vector<KGStepExcutorPtr> m_Steps;
};

///* 并行分布执行 */
//class KGParallelStepExcutor : public IKGStepExcutor
//{
//public:
//    KGParallelStepExcutor() = default;
//    virtual ~KGParallelStepExcutor() = default;
//
//    KGParallelStepExcutor(const KGParallelStepExcutor&) = delete;
//    KGParallelStepExcutor& operator = (const KGParallelStepExcutor&) = delete;
//
//public:
//    inline bool Empty() const { return m_Steps.empty(); }
//    inline void Add(KGStepExcutorPtr ptr) { m_Steps.push_back(ptr); }
//
//    template <typename Fty>
//    inline void Add(Fty&& fn)
//    {
//        Add(MakeStepExcutor(std::forward<Fty>(fn)));
//    }
//
//    void Clear();
//
//    KGSTEP_RET Step() override;
//    void Rollback() override;
//
//protected:
//    std::vector<KGStepExcutorPtr> m_Steps;
//};
//
//template <typename Rty, typename Fty>
//void MakeExcutor(std::future<Rty>&& future,  Fty&& fn)
//{
//
//}
//
//template <typename Rty, typename Fty>
//void MakeExcutor(const std::shared_future<Rty>& future, Fty&& fn)
//{
//
//}
