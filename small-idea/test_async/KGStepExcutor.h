/*
 * 分步执行器
*/
#pragma once

#include <type_traits>
#include <memory>
#include <vector>
#include "KGAsync.h"

enum class KGSTEP_RET
{
    Continue,
    Failed,
    Completed,
};

namespace StepExcutor_Internal { struct _Guarder; }

template <size_t N>
struct StepAllocator
{

};


struct KGScopeGuard
{
    void Dismiss() { }

    bool Done() { return true; }

    template <typename Fty>
    void Push(Fty&& func)    { }
};

/* 步进器守卫 */
class KGStepGuard : private KGScopeGuard
{
    friend StepExcutor_Internal::_Guarder;
public:
    KGStepGuard() = default;
    ~KGStepGuard() = default;

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
    template <typename Ty>
    struct _HasGuarder
    {
        template<typename U> static auto Check(int) -> decltype(std::declval<U>()(std::declval<KGStepGuard&>()), std::true_type());
        template<typename U> static std::false_type Check(...);

        static constexpr bool value =
            std::is_same<decltype(Check<Ty>(0)), std::true_type>::value;
    };

    template <typename Ty, bool>
    struct _ExcutorRet
    {
        typedef typename std::result_of<Ty(KGStepGuard&)>::type type;
    };

    template <typename Ty>
    struct _ExcutorRet<Ty, false>
    {
        typedef typename std::result_of<Ty()>::type type;
    };

    struct _Guarder
    {
        ~_Guarder() { guader.Dismiss(); }
        void Rollback() { guader.Done(); }

        KGStepGuard guader;
    };

    struct _NoneGuarder
    {
        void Rollback() { }
    };

    template <typename Ty, bool>
    struct _Excutor;

    template <>
    struct _Excutor<KGSTEP_RET, true> : _Guarder
    {
        template <typename Fty>
        KGSTEP_RET Step(Fty&& fn) { return fn(guader); }
    };

    template <>
    struct _Excutor<KGSTEP_RET, false> : _NoneGuarder
    {
        template <typename Fty>
        KGSTEP_RET Step(Fty&& fn) { return fn(); }
    };

    template <>
    struct _Excutor<bool, true> : _Guarder
    {
        template <typename Fty>
        KGSTEP_RET Step(Fty&& fn) { return fn(guader) ? KGSTEP_RET::Completed : KGSTEP_RET::Failed; }
    };

    template <>
    struct _Excutor<bool, false> : _NoneGuarder
    {
        template <typename Fty>
        KGSTEP_RET Step(Fty&& fn) { return fn() ? KGSTEP_RET::Completed : KGSTEP_RET::Failed; }
    };

    template <>
    struct _Excutor<void, true> : _Guarder
    {
        template <typename Fty>
        KGSTEP_RET Step(Fty&& fn) { fn(guader); return KGSTEP_RET::Completed; }
    };

    template <>
    struct _Excutor<void, false> : _NoneGuarder
    {
        template <typename Fty>
        KGSTEP_RET Step(Fty&& fn) { fn(); return KGSTEP_RET::Completed; }
    };

    template <typename Ty>
    struct _Traits
    {
        typedef _Excutor<typename _ExcutorRet<Ty, _HasGuarder<Ty>::value>::type, _HasGuarder<Ty>::value> type;
    };
}

template <typename Fty>
class KGStepExcutor : public IKGStepExcutor
{
    typedef typename StepExcutor_Internal::_Traits<Fty>::type Excutor;
public:
    KGStepExcutor(Fty&& fn) : m_Func(std::forward<Fty>(fn)) { }
    virtual ~KGStepExcutor() = default;

    KGStepExcutor(const KGStepExcutor&) = delete;
    KGStepExcutor& operator = (const KGStepExcutor&) = delete;

public:
    KGSTEP_RET Step() override
    {
        return m_Excutor.Step(m_Func);
    }

    void Rollback() override
    {
        m_Excutor.Rollback();
    }

private:
    Fty m_Func;
    Excutor m_Excutor;
};

/* 构建一个分布执行器 */
template <typename Fty>
inline KGStepExcutorPtr MakeStepExcutor(Fty&& fn)
{
    return std::make_shared<KGStepExcutor<Fty>>(std::forward<Fty>(fn));
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
