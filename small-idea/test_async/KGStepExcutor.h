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

        ~Allocator() {}

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

template <size_t N>
struct StepAllocator
{

};

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
        KGScopeGuard::Push(std::forward<Fty>(func));
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
    template <bool, typename Fty>
    struct ret_type
    {
        typedef typename utility::invoke_result<Fty>::type type;
    };

    template <typename Fty>
    struct ret_type<true, Fty>
    {
        typedef typename utility::invoke_result<Fty, KGStepGuard&>::type type;
    };

    template <bool HasGuard, typename Fty>
    using ret_type_t = typename ret_type<HasGuard, Fty>::type;

    /* check has guarder */
    template <typename Ty>
    struct has_guarder
    {
        static constexpr bool value = utility::is_callable<Ty, KGStepGuard&>::value;
    };

    template <typename Rty, typename Fty>
    struct is_same_ret_type
    {
        static constexpr bool value = std::is_same<Rty, ret_type_t<has_guarder<Fty>::value, Fty>>::value;
    };

    template <bool HasGuard, typename Rty, typename Fty>
    using excutor_enable_if_t = typename std::enable_if<HasGuard == has_guarder<Fty>::value && is_same_ret_type<Rty, Fty>::value, KGSTEP_RET>::type;

    struct _Guarder
    {
        _Guarder(Allocator alloc) : m_Guarder(Allocator(alloc))
        {
        }

        ~_Guarder()
        {
            m_Guarder.Dismiss();
        }

        void Rollback()
        {
            m_Guarder.Done();
        }

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
    inline auto StepForward(_Guarder& guader, const Fty& fn) -> excutor_enable_if_t<true, void, Fty>
    {
        fn(guader.m_Guarder);
        return KGSTEP_RET::Completed;
    }

    /* functor: KGSTEP_RET(KGStepGuard&) */
    template <typename Fty>
    inline auto StepForward(_Guarder& guader, const Fty& fn) -> excutor_enable_if_t<true, KGSTEP_RET, Fty>
    {
        return fn(guader.m_Guarder);
    }

    /* functor: bool(KGStepGuard&) */
    template <typename Fty>
    inline auto StepForward(_Guarder& guader, const Fty& fn) -> excutor_enable_if_t<true, bool, Fty>
    {
        return fn(guader.m_Guarder) ? KGSTEP_RET::Completed : KGSTEP_RET::Failed;
    }

    /* functor: void() */
    template <typename Fty>
    inline auto StepForward(const _NoneGuarder&, const Fty& fn) -> excutor_enable_if_t<false, void, Fty>
    {
        fn();
        return KGSTEP_RET::Completed;
    }

    /* functor: KGSTEP_RET() */
    template <typename Fty>
    inline auto StepForward(const _NoneGuarder&, const Fty& fn) -> excutor_enable_if_t<false, KGSTEP_RET, Fty>
    {
        return fn();
    }

    /* functor: bool() */
    template <typename Fty>
    inline auto StepForward(const _NoneGuarder&, const Fty& fn) -> excutor_enable_if_t<false, bool, Fty>
    {
        return fn() ? KGSTEP_RET::Completed : KGSTEP_RET::Failed;
    }

    template <typename Ty>
    struct Traits
    {
        typedef typename std::conditional<utility::is_callable<Ty, KGStepGuard&>::value,
            GuarderWithAlloc, _NoneGuarder>::type type;
    };

    template <typename Fty, typename GuardType>
    struct StepExcutor : public IKGStepExcutor
    {
        StepExcutor(Fty&& fn) : m_Func(std::forward<Fty>(fn)) { }

        template <class = typename std::enable_if<!std::is_same<GuardType, _Guarder>::value, void>::type>
        StepExcutor(Fty&& fn, const Allocator&)
        {
        }

        virtual ~StepExcutor() = default;

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
    using guard_type = typename std::conditional<StepExcutor_Internal::has_guarder<Fty>::value,
        StepExcutor_Internal::GuarderWithAlloc, StepExcutor_Internal::_NoneGuarder>::type;
    return std::make_shared<StepExcutor_Internal::StepExcutor<Fty, guard_type>>(std::forward<Fty>(fn));
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
