/*
 * 分步执行器
*/
#pragma once

#include <type_traits>
#include <memory>
#include <vector>
#include "future.h"
#include "serial_allocator.h"

UTILITY_NAMESPACE_BEGIN

/* 站, 管理错误守卫、子步骤 */
class StepStation;

/* 步进器状态 */
enum class STEP_STATUS
{
    Busy,       // 繁忙
    Idle,       // 空闲
    Failed,     // 失败
    Completed,  // 结束
};

/* 判断分布执行器是否结束 */
#define IS_STEP_STOPPED(eStatus)    (eStatus == UTILITY_NAMESPCE STEP_STATUS::Failed || \
    eStatus == UTILITY_NAMESPCE STEP_STATUS::Completed)
/* 静态类型坚持错误提示 */
#define _STEP_EXCUTOR_ERROR_MESSAGE "Functor signature is not allowed! Only support functor with "  \
    "void(), void(StepStation&), bool(), bool(StepStation&), STEP_STATUS() and STEP_STATUS(StepStation&)"

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

/* 执行一段固定时间(单位毫秒)
 * 当步进器遇到Idle, Failed, Completed结束
*/
STEP_STATUS StepFor(IStepExcutor* pSteper, size_t nDuration);

/* 持续执行步进器知道结束
 * 当步进器遇到Failed, Completed结束
*/
STEP_STATUS StepEnd(IStepExcutor* pSteper);

/* 内部实现细节 */
namespace StepExcutor_Internal
{
    class StationImpl;
    class StationWithAllocImpl;

    template <typename Fy, typename... Args>
    StepExcutorPtr MakeExcutor(Args&&... args);
    template <typename Fy, typename... Args>
    StepExcutorPtr AllocMakeExcutor(SerialAllocator<>* alloc, Args&&... args);
    template <typename Fy, typename... Args>
    StepExcutorPtr StationExcutor(StepStation& station, SerialAllocator<>* alloc, Args&&... args);

    class QueuedStepImpl
    {
        using StepNode = SinglyNode<StepExcutorPtr>;
    public:
        QueuedStepImpl(SerialAllocator<>* alloc) : alloc_(alloc)
        {
        }

        ~QueuedStepImpl()
        {
            Clear();
        }

        QueuedStepImpl(const QueuedStepImpl&) = delete;
        QueuedStepImpl& operator = (const QueuedStepImpl&) = delete;

    public:
        inline SerialAllocator<>* GetAlloc() const { return alloc_; }
        inline bool IsEmpty() const { return head_ == nullptr; }

        STEP_STATUS Step();
        void Rollback();
        void Push(StepExcutorPtr ptr);
        void Clear();

    private:
        StepNode* head_ = nullptr;
        StepNode* tail_ = nullptr;
        StepNode* step_ = nullptr;
        SerialAllocator<>* alloc_;
    };

    class StackedGuardImpl
    {
        using GuardNode = SinglyNode<ICallable<void()>*>;
    public:
        StackedGuardImpl(SerialAllocator<>* alloc) : alloc_(alloc)
        {
        }

        ~StackedGuardImpl()
        {
            Clear();
        }

        StackedGuardImpl(const StackedGuardImpl&) = delete;
        StackedGuardImpl& operator = (const StackedGuardImpl&) = delete;

    public:
        inline SerialAllocator<>* GetAlloc() const
        {
            return alloc_;
        }

        template <typename Fy, typename... Args>
        inline void Push(Fy&& func, Args&&... args)
        {
            GuardNode* node = Construct(std::forward<Fy>(func), std::forward<Args>(args)...);
            node->next_node = head_;
            head_ = node;
        }

        void Rollback();
        void Clear();

    private:
        template <typename Fy, typename... Args>
        GuardNode* Construct(Fy&& func, Args&&... args)
        {
            using Callable = CallableObject<Fy, Args...>;
            GuardNode* node = new (alloc_->Alloc(sizeof(GuardNode))) GuardNode(nullptr);
            node->val = new (alloc_->Alloc(sizeof(Callable))) Callable(std::forward<Fy>(func), std::forward<Args>(args)...);
            return node;
        }

    private:
        GuardNode* head_ = nullptr;
        SerialAllocator<>* alloc_;
    };
} // namespace StepExcutor_Internal

/* 构建一个分布执行器
 * Fy支持的函数签名为:
 * 1. void(), void(StepStation&), the status will be STEP_STATUS::Completed
 * 2. bool(), bool(StepStation&), the status will be (ret ? STEP_STATUS::Completed : STEP_STATUS::Failed)
 * 3. STEP_STATUS(), STEP_STATUS(StepStation&)
*/
template <typename Fy>
inline StepExcutorPtr MakeStepExcutor(Fy&& fn)
{
    return StepExcutor_Internal::MakeExcutor<Fy>(std::forward<Fy>(fn));
}

template <typename Ry, typename Fy>
inline StepExcutorPtr MakeStepExcutor(Future<Ry>&& future, Fy&& fn)
{
    return StepExcutor_Internal::MakeExcutor<Fy>(std::move(future), std::forward<Fy>(fn));
}

template <typename Ry, typename Fy>
inline StepExcutorPtr MakeStepExcutor(const SharedFuture<Ry>& future, Fy&& fn)
{
    return StepExcutor_Internal::MakeExcutor<Fy>(future, std::forward<Fy>(fn));
}

/* 队列式步进器 */
class QueueStepExcutor final : public IStepExcutor
{
public:
    QueueStepExcutor()
        : alloc_(1024)
        , queue_(alloc_.GetAlloc())
    {
    }

    QueueStepExcutor(size_t cache_block_size)
        : alloc_(cache_block_size)
        , queue_(alloc_.GetAlloc())
    {
    }

    virtual ~QueueStepExcutor() = default;

    QueueStepExcutor(const QueueStepExcutor&) = delete;
    QueueStepExcutor& operator = (const QueueStepExcutor&) = delete;

public:
    STEP_STATUS Step() override { return queue_.Step(); }
    void Rollback() override { queue_.Clear(); }

public:
    inline bool IsEmpty() const { return queue_.IsEmpty(); }

    inline void Add(StepExcutorPtr ptr)
    {
        queue_.Push(ptr);
    }

    template <typename Fy>
    inline void Add(Fy&& fn)
    {
        queue_.Push(StepExcutor_Internal::AllocMakeExcutor<Fy>(
            alloc_.GetAlloc(), std::forward<Fy>(fn)));
    }

    template <typename Ry, typename Fy>
    inline void Add(Future<Ry>&& future, Fy&& fn)
    {
        queue_.Push(StepExcutor_Internal::AllocMakeExcutor<Fy>(
            alloc_.GetAlloc(), std::move(future), std::forward<Fy>(fn)));
    }

    template <typename Ry, typename Fy>
    inline void Add(const SharedFuture<Ry>& future, Fy&& fn)
    {
        queue_.Push(StepExcutor_Internal::AllocMakeExcutor<Fy>(
            alloc_.GetAlloc(), future, std::forward<Fy>(fn)));
    }

private:
    PoolSerialAlloc<512> alloc_;
    StepExcutor_Internal::QueuedStepImpl queue_;
}; // QueueStepExcutor

/* 并行式步进器 */
class ParallelStepExcutor : public IStepExcutor
{
public:
    ParallelStepExcutor() = default;
    virtual ~ParallelStepExcutor() = default;

    ParallelStepExcutor(const ParallelStepExcutor&) = delete;
    ParallelStepExcutor& operator = (const ParallelStepExcutor&) = delete;

public:
    STEP_STATUS Step() override;
    void Rollback() override { /* nothing can do */}

public:
    inline bool IsEmpty() const { return steps_.empty(); }
    inline void Add(StepExcutorPtr ptr) { steps_.push_back(ptr); }

protected:
    std::vector<StepExcutorPtr> steps_;
}; // ParallelStepExcutor

/* 步进器中转站 */
class StepStation
{
private:
    friend class StepExcutor_Internal::StationImpl;
    friend class StepExcutor_Internal::StationWithAllocImpl;

    StepStation(StepExcutor_Internal::QueuedStepImpl* que, StepExcutor_Internal::StackedGuardImpl* guarder)
        : queue_(que)
        , guarder_(guarder)
    {
    }

    StepStation(const StepStation&) = delete;
    StepStation& operator = (const StepStation&) = delete;

public:
    template <typename Fy>
    inline void SubStep(Fy&& fn)
    {
        queue_->Push(StepExcutor_Internal::StationExcutor<Fy>(*this, queue_->GetAlloc(), std::forward<Fy>(fn)));
    }

    template <typename Ry, typename Fy>
    inline void SubStep(Future<Ry>&& future, Fy&& fn)
    {
        queue_->Push(StepExcutor_Internal::StationExcutor<Fy>(*this, queue_->GetAlloc(), std::move(future), std::forward<Fy>(fn)));
    }

    template <typename Ry, typename Fy>
    inline void SubStep(const SharedFuture<Ry>& future, Fy&& fn)
    {
        queue_->Push(StepExcutor_Internal::StationExcutor<Fy>(*this, queue_->GetAlloc(), future, std::forward<Fy>(fn)));
    }

    template <typename Fy, typename... Args>
    inline void Guard(Fy&& func, Args&&... args)
    {
        guarder_->Push(std::forward<Fy>(func), std::forward<Args>(args)...);
    }

private:
    StepExcutor_Internal::QueuedStepImpl* queue_;
    StepExcutor_Internal::StackedGuardImpl* guarder_;
};

namespace StepExcutor_Internal
{
    /* trait the excutor function return type */
    template <bool, typename Fy>
    struct ExcutorRetTypeTrait
    {
        typedef typename std_ext::invoke_result<Fy>::type type;
    };

    template <typename Fy>
    struct ExcutorRetTypeTrait<true, Fy>
    {
        typedef typename std_ext::invoke_result<Fy, StepStation&>::type type;
    };

    /* check has guarder */
    template <typename Ty>
    struct ExcutorHasGuarder : public std::bool_constant<std_ext::is_callable<Ty, StepStation&>::value>
    {
    };

    template <typename Fy>
    using ExcutorRetType_t = typename ExcutorRetTypeTrait<ExcutorHasGuarder<Fy>::value, Fy>::type;

    template <typename Fy>
    struct ExcutorSignatureCheck
    {
        static constexpr bool value = std_ext::is_callable<Fy>::value ||
            (!std_ext::is_callable<Fy, const StepStation&>::value && std_ext::is_callable<Fy, StepStation&>::value);
    };

    template <bool, typename Fy>
    struct ExcutorCheckImpl
    {
        static constexpr bool value = std::is_void<ExcutorRetType_t<Fy>>::value ||
            std::is_same<bool, ExcutorRetType_t<Fy>>::value ||
            std::is_same<STEP_STATUS, ExcutorRetType_t<Fy>>::value;
    };

    template <typename Fy>
    struct ExcutorCheckImpl<false, Fy>
    {
        static constexpr bool value = false;
    };

    template <typename Fy>
    struct ExcutorCheck
    {
        static constexpr bool value = ExcutorCheckImpl<ExcutorSignatureCheck<Fy>::value, Fy>::value;
    };

    /* functor: void() */
    template <typename Fy>
    inline auto StepForward(Fy& fn) -> typename std::enable_if<std::is_void<ExcutorRetType_t<Fy>>::value, STEP_STATUS>::type
    {
        fn();
        return STEP_STATUS::Completed;
    }

    /* functor: bool() */
    template <typename Fy>
    inline auto StepForward(Fy& fn) -> typename std::enable_if<std::is_same<bool, ExcutorRetType_t<Fy>>::value, STEP_STATUS>::type
    {
        return fn() ? STEP_STATUS::Completed : STEP_STATUS::Failed;
    }

    /* functor: STEP_STATUS() */
    template <typename Fy>
    inline auto StepForward(Fy& fn) -> typename std::enable_if<std::is_same<STEP_STATUS, ExcutorRetType_t<Fy>>::value, STEP_STATUS>::type
    {
        return fn();
    }

    class StationImpl : public IStepExcutor
    {
    public:
        StationImpl(SerialAllocator<>* alloc)
            : queue_(alloc)
            , guarder_(alloc)
            , station_(&queue_, &guarder_)
        {
        }

        ~StationImpl()
        {
            if (!queue_.IsEmpty())
                Rollback();
            guarder_.Clear();
            queue_.Clear();
        }

    public:
        inline StepStation& GetStation() { return station_; }
        STEP_STATUS Step() override { return queue_.Step(); }
        void Rollback() override { guarder_.Rollback(); }

    private:
        QueuedStepImpl queue_;
        StackedGuardImpl guarder_;
        StepStation station_;
    };

    class StationWithAllocImpl : public IStepExcutor
    {
    public:
        StationWithAllocImpl()
            : alloc_(1024)
            , queue_(alloc_.GetAlloc())
            , guarder_(alloc_.GetAlloc())
            , station_(&queue_, &guarder_)
        {
        }

        ~StationWithAllocImpl()
        {
            if (!queue_.IsEmpty())
                Rollback();
            guarder_.Clear();
            queue_.Clear();
        }

    public:
        inline StepStation& GetStation() { return station_; }
        STEP_STATUS Step() override { return queue_.Step(); }
        void Rollback() override { guarder_.Rollback(); }

    private:
        PoolSerialAlloc<128> alloc_;
        QueuedStepImpl queue_;
        StackedGuardImpl guarder_;
        StepStation station_;
    };

    template <typename Fy, typename... Args>
    class ExcutorDetail : public IStepExcutor
    {
        using Callee = CallablePackage<Fy, Args...>;
    public:
        ExcutorDetail(Fy&& func, Args&&... args)
            : callee_(std::forward<Fy>(func), std::forward<Args>(args)...)
        {
        }

        virtual ~ExcutorDetail() { }
        STEP_STATUS Step() override { return StepForward(callee_); }
        void Rollback() override { }

    private:
        Callee callee_;
    };

    template <typename... Args>
    class FutureExcutor : public ExcutorDetail<Args...>
    {
    public:
        FutureExcutor(Args&&... args) : ExcutorDetail<Args...>(std::forward<Args>(args)...)
        {
        }

        virtual ~FutureExcutor() { }
    };

    template <typename Ry, typename... Args>
    class FutureExcutor<Future<Ry>, Args...> : public ExcutorDetail<Args...>
    {
    public:
        FutureExcutor(Future<Ry>&& future, Args&&... args)
            : ExcutorDetail<Args...>(std::forward<Args>(args)...)
            , future_(std::move(future))
        {
        }

        virtual ~FutureExcutor() { }

        STEP_STATUS Step() override
        {
            if (!future_.IsReady())
                return STEP_STATUS::Idle;
            return ExcutorDetail<Args...>::Step();
        }

    private:
        Future<Ry> future_;
    };

    template <typename Ry, typename... Args>
    class FutureExcutor<SharedFuture<Ry>, Args...> : public ExcutorDetail<Args...>
    {
    public:
        FutureExcutor(const SharedFuture<Ry>& future, Args&&... args)
            : ExcutorDetail<Args...>(std::forward<Args>(args)...)
            , future_(future)
        {
        }

        virtual ~FutureExcutor() { }

        STEP_STATUS Step() override
        {
            if (!future_.IsReady())
                return STEP_STATUS::Idle;
            return ExcutorDetail<Args...>::Step();
        }

    private:
        SharedFuture<Ry> future_;
    };

    template <typename... Args>
    inline StepExcutorPtr PackageExcutorImpl(std::true_type, SerialAllocator<>* alloc, Args&&... args)
    {
        using ExcutorType = FutureExcutor<Args...>;
        return std::allocate_shared<ExcutorType>(alloc->GetAdapter<ExcutorType>(), std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline StepExcutorPtr PackageExcutorImpl(std::false_type, Args&&... args)
    {
        return std::make_shared<FutureExcutor<Args...>>(std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline StepExcutorPtr MakeExcutorImpl(std::true_type, Args&&... args)
    {
        std::shared_ptr<StationWithAllocImpl> station = std::make_shared<StationWithAllocImpl>();
        station->GetStation().SubStep(std::forward<Args>(args)...);
        return station;
    }

    template <typename... Args>
    inline StepExcutorPtr MakeExcutorImpl(std::false_type, Args&&... args)
    {
        return PackageExcutorImpl(std::false_type(), std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline StepExcutorPtr AllocMakeExcutorImpl(std::true_type, SerialAllocator<>* alloc, Args&&... args)
    {
        std::shared_ptr<StationImpl> station = std::allocate_shared<StationImpl>(alloc->GetAdapter<StationImpl>(), alloc);
        station->GetStation().SubStep(std::forward<Args>(args)...);
        return station;
    }

    template <typename... Args>
    inline StepExcutorPtr AllocMakeExcutorImpl(std::false_type, SerialAllocator<>* alloc, Args&&... args)
    {
        return PackageExcutorImpl(std::true_type(), alloc, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline StepExcutorPtr StationExcutorImpl(std::true_type, StepStation& station, SerialAllocator<>* alloc, Args&&... args)
    {
        return PackageExcutorImpl<Args..., StepStation&>(std::true_type(), alloc, std::forward<Args>(args)..., station);
    }

    template <typename... Args>
    inline StepExcutorPtr StationExcutorImpl(std::false_type, StepStation& station, SerialAllocator<>* alloc, Args&&... args)
    {
        return PackageExcutorImpl(std::true_type(), alloc, std::forward<Args>(args)...);
    }

    template <typename Fy, typename... Args>
    inline StepExcutorPtr MakeExcutor(Args&&... args)
    {
        static_assert(ExcutorCheck<Fy>::value, _STEP_EXCUTOR_ERROR_MESSAGE);
        return MakeExcutorImpl(ExcutorHasGuarder<Fy>(), std::forward<Args>(args)...);
    }

    template <typename Fy, typename... Args>
    inline StepExcutorPtr AllocMakeExcutor(SerialAllocator<>* alloc, Args&&... args)
    {
        static_assert(ExcutorCheck<Fy>::value, _STEP_EXCUTOR_ERROR_MESSAGE);
        return AllocMakeExcutorImpl(ExcutorHasGuarder<Fy>(), alloc, std::forward<Args>(args)...);
    }

    template <typename Fy, typename... Args>
    inline StepExcutorPtr StationExcutor(StepStation& station, SerialAllocator<>* alloc, Args&&... args)
    {
        static_assert(ExcutorCheck<Fy>::value, _STEP_EXCUTOR_ERROR_MESSAGE);
        return StationExcutorImpl(ExcutorHasGuarder<Fy>(), station, alloc, std::forward<Args>(args)...);
    }
} // namesapce StepExcutor_Internal

UTILITY_NAMESPACE_END
