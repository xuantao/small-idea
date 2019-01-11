/*
 * 分步执行器（步进器）
*/
#pragma once

#include <type_traits>
#include <memory>
#include <vector>
#include "future.h"
#include "serial_allocator.h"

UTILITY_NAMESPACE_BEGIN

/* 分布控制器
 * 1. 添加子节点
 * 2. 添加控制守卫
*/
class StepCtrl;

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
#define STEP_EXCUTOR_ERROR_MESSAGE_ "Functor signature is not allowed! Only support functor with "  \
    "void(), void(StepCtrl), bool(), bool(StepCtrl), STEP_STATUS() and STEP_STATUS(StepCtrl)"

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
    template <typename Fy, typename... Args>
    StepExcutorPtr MakeExcutor(Args&&... args);
    template <typename Fy, typename... Args>
    StepExcutorPtr AllocMakeExcutor(SerialAllocator<>* alloc, Args&&... args);

    /* 步进器执行序列 */
    class QueueImpl
    {
        using StepNode = SinglyNode<StepExcutorPtr>;
    public:
        QueueImpl(SerialAllocator<>* alloc) : alloc_(alloc) { }
        ~QueueImpl();

        QueueImpl(const QueueImpl&) = delete;
        QueueImpl& operator = (const QueueImpl&) = delete;

    public:
        inline SerialAllocator<>* GetAlloc() const { return alloc_; }
        inline bool IsEmpty() const { return head_ == nullptr && step_ == nullptr; }
        inline bool IsComplete() const { return head_ == nullptr; }

        STEP_STATUS Step();
        void Rollback();
        void Push(StepExcutorPtr ptr);

    private:
        StepNode* head_ = nullptr;
        StepNode* tail_ = nullptr;
        StepNode* step_ = nullptr;
        SerialAllocator<>* alloc_;
    };

    /* 守护器 */
    class GuardImpl
    {
        using GuardNode = SinglyNode<ICallable<void()>*>;
    public:
        GuardImpl(SerialAllocator<>* alloc) : alloc_(alloc) { }
        ~GuardImpl();

        GuardImpl(const GuardImpl&) = delete;
        GuardImpl& operator = (const GuardImpl&) = delete;

    public:
        inline SerialAllocator<>* GetAlloc() const { return alloc_; }

        template <typename Fy, typename... Args>
        inline void Push(Fy&& func, Args&&... args)
        {
            using Callable = CallableObject<Fy, Args...>;

            auto node = alloc_->Construct<GuardNode>(nullptr);
            node->val = alloc_->Construct<Callable>(std::forward<Fy>(func), std::forward<Args>(args)...);

            node->next_node = head_;
            head_ = node;
        }

        void Rollback();

    private:
        GuardNode* head_ = nullptr;
        SerialAllocator<>* alloc_;
    };

    /* 中间站控制器 */
    class StepCtrlImpl
    {
    public:
        StepCtrlImpl(SerialAllocator<>* alloc) : queue_(alloc), guarder_(alloc)
        { }

        ~StepCtrlImpl()
        {
            if (!queue_.IsComplete())
                Rollback(); // 没有执行完就析构表明出现错误, 需要回滚
        }

    public:
        inline SerialAllocator<>* GetAlloc() { return queue_.GetAlloc(); }
        inline void SubStep(StepExcutorPtr step) { queue_.Push(step); }
        inline STEP_STATUS Step() { return queue_.Step(); }

        inline void Rollback()
        {
            queue_.Rollback();
            guarder_.Rollback();
        }

        template <typename Fy, typename... Args>
        inline void Guard(Fy&& func, Args&&... args)
        {
            guarder_.Push(std::forward<Fy>(func), std::forward<Args>(args)...);
        }

    private:
        QueueImpl queue_;
        GuardImpl guarder_;
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
    QueueStepExcutor() : alloc_(1024)
    {
        queue_ = alloc_.Construct<StepExcutor_Internal::QueueImpl>(&alloc_);
    }

    QueueStepExcutor(size_t cache_block_size) : alloc_(cache_block_size)
    {
        queue_ = alloc_.Construct<StepExcutor_Internal::QueueImpl>(&alloc_);
    }

    virtual ~QueueStepExcutor()
    {
        if (!IsComplete())
            Rollback();

        alloc_.Destruct(queue_);
        queue_ = nullptr;
    }

    QueueStepExcutor(const QueueStepExcutor&) = delete;
    QueueStepExcutor& operator = (const QueueStepExcutor&) = delete;

public:
    STEP_STATUS Step() override { return queue_->Step(); }
    void Rollback() override { queue_->Rollback(); }

public:
    inline bool IsEmpty() const { return queue_->IsEmpty(); }
    inline bool IsComplete() const { return queue_->IsComplete(); }

    inline void Add(StepExcutorPtr ptr)
    {
        queue_->Push(ptr);
    }

    template <typename Fy>
    inline void Add(Fy&& fn)
    {
        queue_->Push(StepExcutor_Internal::AllocMakeExcutor<Fy>(
            &alloc_, std::forward<Fy>(fn)));
    }

    template <typename Ry, typename Fy>
    inline void Add(Future<Ry>&& future, Fy&& fn)
    {
        queue_->Push(StepExcutor_Internal::AllocMakeExcutor<Fy>(
            &alloc_, std::move(future), std::forward<Fy>(fn)));
    }

    template <typename Ry, typename Fy>
    inline void Add(const SharedFuture<Ry>& future, Fy&& fn)
    {
        queue_->Push(StepExcutor_Internal::AllocMakeExcutor<Fy>(
            &alloc_, future, std::forward<Fy>(fn)));
    }

private:
    StepExcutor_Internal::QueueImpl* queue_;
    PooledSerialAlloc<512> alloc_;
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
    void Rollback() override { /* nothing can do */ }

public:
    inline bool IsEmpty() const { return steps_.empty(); }
    inline void AddStep(StepExcutorPtr ptr) { steps_.push_back(ptr); }

protected:
    std::vector<StepExcutorPtr> steps_;
}; // ParallelStepExcutor

/* 步进控制器 */
class StepCtrl
{
public:
    ~StepCtrl() = default;
    StepCtrl(const StepCtrl&) = default;
    StepCtrl& operator = (const StepCtrl&) = default;

public:
    /* 添加子步骤 */
    inline void SubStep(StepExcutorPtr step)
    {
        ctrl_->SubStep(step);
    }

    template <typename Fy>
    inline void SubStep(Fy&& fn)
    {
        ctrl_->SubStep(StepExcutor_Internal::AllocMakeExcutor<Fy>(ctrl_->GetAlloc(), std::forward<Fy>(fn)));
    }

    template <typename Ry, typename Fy>
    inline void SubStep(Future<Ry>&& future, Fy&& fn)
    {
        ctrl_->SubStep(StepExcutor_Internal::AllocMakeExcutor<Fy>(ctrl_->GetAlloc(), std::move(future), std::forward<Fy>(fn)));
    }

    template <typename Ry, typename Fy>
    inline void SubStep(const SharedFuture<Ry>& future, Fy&& fn)
    {
        ctrl_->SubStep(StepExcutor_Internal::AllocMakeExcutor<Fy>(ctrl_->GetAlloc(), future, std::forward<Fy>(fn)));
    }

    /* 守卫函数
     * 将函数+参数打包成一个functor存起来, 当出现错误回滚时执行
    */
    template <typename Fy, typename... Args>
    inline void Guard(Fy&& func, Args&&... args)
    {
        ctrl_->Guard(std::forward<Fy>(func), std::forward<Args>(args)...);
    }

protected:
    StepCtrl() = default;

protected:
    using StepCtrlImpl = StepExcutor_Internal::StepCtrlImpl;

    inline SerialAllocator<>* GetAlloc() { return ctrl_->GetAlloc(); }
    inline void SetCtrlImpl(StepCtrlImpl* ctrl) { ctrl_ = ctrl; }
    inline StepCtrlImpl* GetCtrlImpl() { return ctrl_; }

private:
    StepCtrlImpl* ctrl_;
};

/* 步进器工作站 */
template <size_t Pool = 2048>
class StepExcutorStation : public IStepExcutor, public StepCtrl
{
public:
    StepExcutorStation() : alloc_(Pool > 1024 ? Pool : 1024)
    {
        SetCtrlImpl(alloc_.Construct<StepExcutor_Internal::StepCtrlImpl>(&alloc_));
    }

    virtual ~StepExcutorStation()
    {
        alloc_.Destruct(GetCtrlImpl());
    }

public:
    STEP_STATUS Step() override
    {
        return GetCtrlImpl()->Step();
    }

    void Rollback() override
    {
        GetCtrlImpl()->Rollback();

        alloc_.Destruct(GetCtrlImpl());
        alloc_.Reset();
        SetCtrlImpl(alloc_.Construct<StepExcutor_Internal::StepCtrlImpl>(&alloc_));
    }

private:
    PooledSerialAlloc<Pool> alloc_;
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
        typedef typename std_ext::invoke_result<Fy, StepCtrl>::type type;
    };

    /* check has guarder */
    template <typename Ty>
    struct ExcutorHasGuarder : public std::bool_constant<std_ext::is_callable<Ty, StepCtrl>::value>
    {};

    template <typename Fy>
    using ExcutorRetType_t = typename ExcutorRetTypeTrait<ExcutorHasGuarder<Fy>::value, Fy>::type;

    template <typename Fy>
    struct ExcutorSignatureCheck : public std::bool_constant<
        std_ext::is_callable<Fy>::value || std_ext::is_callable<Fy, StepCtrl>::value
    >{};

    template <bool, typename Fy>
    struct ExcutorCheckImpl : public std::bool_constant<
        std::is_void<ExcutorRetType_t<Fy>>::value ||
        std::is_same<bool, ExcutorRetType_t<Fy>>::value ||
        std::is_same<STEP_STATUS, ExcutorRetType_t<Fy>>::value
    > {};

    template <typename Fy>
    struct ExcutorCheckImpl<false, Fy> : public std::false_type {};

    template <typename Fy>
    struct ExcutorCheck : public std::bool_constant<
        ExcutorCheckImpl<ExcutorSignatureCheck<Fy>::value, Fy>::value
    > {};

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

    class StationExcutorImpl : public IStepExcutor, public StepCtrl
    {
    public:
        StationExcutorImpl(SerialAllocator<>* alloc) : ctrl_(alloc)
        {
            StepCtrl::SetCtrlImpl(&ctrl_);
        }

        virtual ~StationExcutorImpl() = default;

    public:
        STEP_STATUS Step() override { return ctrl_.Step(); }
        void Rollback() override { ctrl_.Rollback(); }

    private:
        StepCtrlImpl ctrl_;
    };

    class StepExcutorStationExt : public StepExcutorStation<128>
    {
    public:
        inline SerialAllocator<>* GetAlloc() { return StepExcutorStation<128>::GetAlloc(); }
    };

    template <typename Fy, typename... Args>
    class ExcutorImplBase : public IStepExcutor
    {
        using Callee = CallablePackage<Fy, Args...>;
    public:
        ExcutorImplBase(Fy&& func, Args&&... args)
            : callee_(std::forward<Fy>(func), std::forward<Args>(args)...)
        {
        }

        virtual ~ExcutorImplBase() { }
        STEP_STATUS Step() override { return StepForward(callee_); }
        void Rollback() override { }

    private:
        Callee callee_;
    };

    template <typename... Args>
    class ExcutorImpl : public ExcutorImplBase<Args...>
    {
    public:
        ExcutorImpl(Args&&... args) : ExcutorImplBase<Args...>(std::forward<Args>(args)...)
        { }

        virtual ~ExcutorImpl() { }
    };

    template <typename Ry, typename... Args>
    class ExcutorImpl<Future<Ry>, Args...> : public ExcutorImplBase<Args...>
    {
    public:
        ExcutorImpl(Future<Ry>&& future, Args&&... args)
            : ExcutorImplBase<Args...>(std::forward<Args>(args)...)
            , future_(std::move(future))
        { }

        virtual ~ExcutorImpl() { }

        STEP_STATUS Step() override
        {
            if (!future_.IsReady())
                return STEP_STATUS::Idle;
            return ExcutorImplBase<Args...>::Step();
        }

    private:
        Future<Ry> future_;
    };

    template <typename Ry, typename... Args>
    class ExcutorImpl<const SharedFuture<Ry>&, Args...> : public ExcutorImplBase<Args...>
    {
    public:
        ExcutorImpl(const SharedFuture<Ry>& future, Args&&... args)
            : ExcutorImplBase<Args...>(std::forward<Args>(args)...)
            , future_(future)
        { }

        virtual ~ExcutorImpl() { }

        STEP_STATUS Step() override
        {
            if (!future_.IsReady())
                return STEP_STATUS::Idle;
            return ExcutorImplBase<Args...>::Step();
        }

    private:
        SharedFuture<Ry> future_;
    };

    inline StepCtrl GetStepCtrl(StepCtrl& station) { return station; }

    template <typename... Args>
    inline StepExcutorPtr PackageExcutor(SerialAllocator<>* alloc, Args&&... args)
    {
        using ExcutorType = ExcutorImpl<Args...>;
        return std::allocate_shared<ExcutorType>(alloc->GetAdapter<ExcutorType>(), std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline StepExcutorPtr MakeExcutorImpl(std::true_type, Args&&... args)
    {
        auto station = std::make_shared<StepExcutorStationExt>();
        auto step = PackageExcutor(station->GetAlloc(), std::forward<Args>(args)..., GetStepCtrl(*station));
        station->SubStep(step);
        return station;
    }

    template <typename... Args>
    inline StepExcutorPtr MakeExcutorImpl(std::false_type, Args&&... args)
    {
        return std::make_shared<ExcutorImpl<Args...>>(std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline StepExcutorPtr AllocMakeExcutorImpl(std::true_type, SerialAllocator<>* alloc, Args&&... args)
    {
        auto station = std::allocate_shared<StationExcutorImpl>(alloc->GetAdapter<StationExcutorImpl>(), alloc);
        auto step = PackageExcutor(alloc, std::forward<Args>(args)..., GetStepCtrl(*station));
        station->SubStep(step);
        return station;
    }

    template <typename... Args>
    inline StepExcutorPtr AllocMakeExcutorImpl(std::false_type, SerialAllocator<>* alloc, Args&&... args)
    {
        return PackageExcutor(alloc, std::forward<Args>(args)...);
    }

    template <typename Fy, typename... Args>
    inline StepExcutorPtr MakeExcutor(Args&&... args)
    {
        static_assert(ExcutorCheck<Fy>::value, STEP_EXCUTOR_ERROR_MESSAGE_);
        return MakeExcutorImpl(ExcutorHasGuarder<Fy>(), std::forward<Args>(args)...);
    }

    template <typename Fy, typename... Args>
    inline StepExcutorPtr AllocMakeExcutor(SerialAllocator<>* alloc, Args&&... args)
    {
        static_assert(ExcutorCheck<Fy>::value, STEP_EXCUTOR_ERROR_MESSAGE_);
        return AllocMakeExcutorImpl(ExcutorHasGuarder<Fy>(), alloc, std::forward<Args>(args)...);
    }
} // namesapce StepExcutor_Internal

UTILITY_NAMESPACE_END
