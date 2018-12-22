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

namespace StepExcutor_Internal
{
    struct _Guarder;
    class Allocator
    {
    public:
        Allocator() : m_pAlloc(nullptr)
        {
            assert(false);
        }

        Allocator(KGSerialAllocator<>* pAlloc) : m_pAlloc(pAlloc)
        {
        }

        Allocator(const Allocator& other) : m_pAlloc(other.m_pAlloc)
        {
        }

        Allocator& operator = (const Allocator& other)
        {
            m_pAlloc = other.m_pAlloc;
            return *this;
        }

        ~Allocator()
        {
        }

    public:
        void* Alloc(size_t sz)
        {
            return m_pAlloc->Alloc(sz);
        }

        void Dealloc(void*, size_t)
        {
        }

    private:
        KGSerialAllocator<>* m_pAlloc;
    };
}

/* 步进器守卫 */
class KGStepGuard : private KGScopeGuardImpl<StepExcutor_Internal::Allocator>
{
private:
    friend StepExcutor_Internal::_Guarder;

    KGStepGuard(const StepExcutor_Internal::Allocator& alloc)
        : KGScopeGuardImpl<StepExcutor_Internal::Allocator>(std::allocator_arg_t(), alloc)
    {
    }

public:
    ~KGStepGuard() = default;

    KGStepGuard(const KGStepGuard&) = delete;
    KGStepGuard& operator = (const KGStepGuard&) = delete;

public:
    template <typename Fty>
    void Push(Fty&& func)
    {
        KGScopeGuardImpl<StepExcutor_Internal::Allocator>::Push(std::forward<Fty>(func));
    }
};

/* 分布执行接口 */
struct IKGStepExcutor
{
    virtual ~IKGStepExcutor() { }
    virtual KGSTEP_RET Step() = 0;
    virtual void Rollback() = 0;
};
typedef std::shared_ptr<IKGStepExcutor> KGStepExcutorPtr;

namespace StepExcutor_Internal
{
    static constexpr char* ERROR_MESSAGE = "Functor signature is not allowed.\
        Support";

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

    struct _Guarder
    {
        _Guarder(const Allocator& alloc) : m_Guarder(alloc)
        {
        }

        ~_Guarder()
        {
            m_Guarder.Dismiss();
        }

        inline KGStepGuard& GetGuarder()
        {
            return m_Guarder;
        }

        inline void Rollback()
        {
            m_Guarder.Done();
        }

    private:
        KGStepGuard m_Guarder;
    };

    struct GuarderWithAlloc : _Guarder
    {
        GuarderWithAlloc() : _Guarder(Allocator(&m_Alloc))
        {
        }

        KGPoolSerialAlloc<128> m_Alloc;
    };

    struct _NoneGuarder
    {
        void Rollback()
        {
        }
    };

    /* functor: bool(KGStepGuard&) */
    template <typename Fty>
    inline auto StepForward(_Guarder& guader, const Fty& fn) -> excutor_enable_if_t<Fty, void(KGStepGuard&)>
    {
        fn(guader.GetGuarder());
        return KGSTEP_RET::Completed;
    }

    /* functor: bool(KGStepGuard&) */
    template <typename Fty>
    inline auto StepForward(_Guarder& guader, const Fty& fn) -> excutor_enable_if_t<Fty, bool(KGStepGuard&)>
    {
        return fn(guader.GetGuarder()) ? KGSTEP_RET::Completed : KGSTEP_RET::Failed;
    }

    /* functor: KGSTEP_RET(KGStepGuard&) */
    template <typename Fty>
    inline auto StepForward(_Guarder& guader, const Fty& fn) -> excutor_enable_if_t<Fty, KGSTEP_RET(KGStepGuard&)>
    {
        return fn(guader.GetGuarder());
    }

    /* functor: void() */
    template <typename Fty>
    inline auto StepForward(const _NoneGuarder&, const Fty& fn) -> excutor_enable_if_t<Fty, void()>
    {
        fn();
        return KGSTEP_RET::Completed;
    }

    /* functor: bool() */
    template <typename Fty>
    inline auto StepForward(const _NoneGuarder&, const Fty& fn) -> excutor_enable_if_t<Fty, bool()>
    {
        return fn() ? KGSTEP_RET::Completed : KGSTEP_RET::Failed;
    }

    /* functor: KGSTEP_RET() */
    template <typename Fty>
    inline auto StepForward(const _NoneGuarder&, const Fty& fn) -> excutor_enable_if_t<Fty, KGSTEP_RET()>
    {
        return fn();
    }

    template <typename Ty>
    struct Traits
    {
        typedef typename std::conditional<utility::is_callable<Ty, KGStepGuard&>::value,
            GuarderWithAlloc, _NoneGuarder>::type type;
    };

    template <typename Fty, typename GuardType>
    struct ExcutorImpl : public IKGStepExcutor
    {
        template <class = typename std::enable_if<!std::is_same<GuardType, _Guarder>::value, void>::type>
        ExcutorImpl(Fty&& fn)
            : m_Func(std::forward<Fty>(fn))
        {
        }

        template <class = typename std::enable_if<std::is_same<GuardType, _Guarder>::value, void>::type>
        ExcutorImpl(Fty&& fn, const Allocator&)
            : m_Func(std::forward<Fty>(fn))
            , m_Guarder(std::forward<GuardType>(fn))
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
}
//
//template <typename Fty>
//class KGStepExcutor : public IKGStepExcutor
//{
//    typedef typename StepExcutor_Internal::_Traits<Fty>::type Excutor;
//public:
//    KGStepExcutor(Fty&& fn) : m_Func(std::forward<Fty>(fn)) { }
//    virtual ~KGStepExcutor() = default;
//
//    KGStepExcutor(const KGStepExcutor&) = delete;
//    KGStepExcutor& operator = (const KGStepExcutor&) = delete;
//
//public:
//    KGSTEP_RET Step() override
//    {
//        return m_Excutor.Step(m_Func);
//    }
//
//    void Rollback() override
//    {
//        m_Excutor.Rollback();
//    }
//
//private:
//    Fty m_Func;
//    Excutor m_Excutor;
//};

/* 构建一个分布执行器 */
template <typename Fty>
inline KGStepExcutorPtr MakeStepExcutor(Fty&& fn)
{
    static_assert(utility::is_callable<Fty>::value || (!utility::is_callable<Fty, KGStepGuard&&>::value
        && utility::is_callable<Fty, KGStepGuard&>::value), "1111");

    using guard_type = typename std::conditional<StepExcutor_Internal::excutor_has_guarder<Fty>::value,
        StepExcutor_Internal::GuarderWithAlloc, StepExcutor_Internal::_NoneGuarder>::type;
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
