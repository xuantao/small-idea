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

/* 分布执行控制器
 * 1. 添加子节点
 * 2. 添加控制守卫
*/
class StepCtrl;

/* 步进器状态 */
enum class STEP_STATUS
{
    Busy,       // 繁忙
    Idle,       // 空闲(当等待异步任务结束时)
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

    /* 向前走一步并返回当前状态 */
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
    /* 守护函数 */
    using GaurdFuncPtr = ICallOnly<>*;

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
        using GuardNode = SinglyNode<GaurdFuncPtr>;
    public:
        GuardImpl(SerialAllocator<>* alloc) : alloc_(alloc) { }
        ~GuardImpl();

    public:
        inline SerialAllocator<>* GetAlloc() const { return alloc_; }
        inline void Push(GaurdFuncPtr func) { head_ = alloc_->Construct<GuardNode>(head_, func); }

        void Rollback();

    private:
        GuardNode* head_ = nullptr;
        SerialAllocator<>* alloc_;
    };

    /* 中间站控制器 */
    class StepCtrlImpl
    {
    public:
        StepCtrlImpl(SerialAllocator<>* alloc) : queue_(alloc), guarder_(alloc) { }

        ~StepCtrlImpl()
        {
            if (!queue_.IsComplete())
                Rollback(); // 没有执行完就析构表明出现错误, 需要回滚
        }

    public:
        inline SerialAllocator<>* GetStepAlloc() { return queue_.GetAlloc(); }
        inline SerialAllocator<>* GetGuardAlloc() { return guarder_.GetAlloc(); }

        inline void SubStep(StepExcutorPtr step) { queue_.Push(step); }
        inline void Guard(GaurdFuncPtr func) { guarder_.Push(func); }
        inline STEP_STATUS Step() { return queue_.Step(); }

        inline void Rollback()
        {
            queue_.Rollback();
            guarder_.Rollback();
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

    QueueStepExcutor(size_t block_size) : alloc_(block_size)
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

    void Rollback() override
    {
        queue_->Rollback();
        /* 清空&重建 */
        alloc_.Destruct(queue_);
        alloc_.Reset();
        queue_ = alloc_.Construct<StepExcutor_Internal::QueueImpl>(&alloc_);
    }

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
    using StepCtrlImpl = StepExcutor_Internal::StepCtrlImpl;
public:
    StepCtrl(StepCtrlImpl* ctrl) : ctrl_(ctrl) { }
    StepCtrl(StepCtrl&& other) : ctrl_(other.ctrl_) { other.ctrl_ = nullptr; }
    ~StepCtrl() { }

    StepCtrl(const StepCtrl&) = delete;
    StepCtrl& operator = (const StepCtrl&) = delete;

public:
    /* 添加子步骤 */
    inline void SubStep(StepExcutorPtr step)
    {
        ctrl_->SubStep(step);
    }

    template <typename Fy>
    inline void SubStep(Fy&& fn)
    {
        ctrl_->SubStep(StepExcutor_Internal::AllocMakeExcutor<Fy>(ctrl_->GetStepAlloc(), std::forward<Fy>(fn)));
    }

    template <typename Ry, typename Fy>
    inline void SubStep(Future<Ry>&& future, Fy&& fn)
    {
        ctrl_->SubStep(StepExcutor_Internal::AllocMakeExcutor<Fy>(ctrl_->GetStepAlloc(), std::move(future), std::forward<Fy>(fn)));
    }

    template <typename Ry, typename Fy>
    inline void SubStep(const SharedFuture<Ry>& future, Fy&& fn)
    {
        ctrl_->SubStep(StepExcutor_Internal::AllocMakeExcutor<Fy>(ctrl_->GetStepAlloc(), future, std::forward<Fy>(fn)));
    }

    /* 守卫函数
     * 将函数+参数打包成一个functor存起来, 当出现错误回滚时执行
    */
    template <typename Fy, typename... Args>
    inline void Guard(Fy&& func, Args&&... args)
    {
        using Package = PackageCall<Fy, Args...>;
        using Callable = CallableObject<Package>;
        auto call = ctrl_->GetGuardAlloc()->Construct<Callable>(std::forward<Fy>(func), std::forward<Args>(args)...);
        assert(call);
        ctrl_->Guard(call);
    }

private:
    StepCtrlImpl* ctrl_;
};

/* 步进器工作站基类
 * 这里提供受保护的构造、初始化操作是为了可以手动控制创建、销毁流程
 * 当子类要持有内存分配器时，不能直接使用构造函数构造对象。
 * 为了要保证成员变量的初始化顺序
*/
class StepExcutorStationBase : public IStepExcutor
{
public:
    StepExcutorStationBase(SerialAllocator<>* alloc) { Init(alloc); }
    virtual ~StepExcutorStationBase() { Uninit(); }

    StepExcutorStationBase(const StepExcutorStationBase&) = delete;
    StepExcutorStationBase& operator = (const StepExcutorStationBase&) = delete;

public:
    STEP_STATUS Step() override { return ctrl_->Step(); }
    void Rollback() override { ctrl_->Rollback(); }

    inline StepCtrl GetCtrl() const { return StepCtrl(ctrl_); }

protected:
    StepExcutorStationBase() { }

    inline void Init(SerialAllocator<>* alloc)
    {
        assert(ctrl_ == nullptr);
        ctrl_ = alloc->Construct<StepExcutor_Internal::StepCtrlImpl>(alloc);
    }

    void Uninit()
    {
        if (ctrl_)
        {
            ctrl_->GetStepAlloc()->Destruct(ctrl_);
            ctrl_ = nullptr;
        }
    }

protected:
    StepExcutor_Internal::StepCtrlImpl* ctrl_ = nullptr;
};

/* 步进器工作站
 * 工作站是一次性的, 执行完毕、出错回滚以后变不再可用
 * Pool: 指定持有缓存大小, 避免过多细碎内存分配
*/
template <size_t Pool = 2048>
class StepExcutorStation : public StepExcutorStationBase
{
public:
    StepExcutorStation() : alloc_((Pool > 1024 ? Pool : 1024))
    {
        Init(&alloc_);
    }

    StepExcutorStation(size_t block_size) : alloc_(block_size)
    {
        Init(&alloc_);
    }

    virtual ~StepExcutorStation()
    {
        Uninit();
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
    struct ExcutorHasGuarder : public std::bool_constant<
        std_ext::is_callable<Ty, StepCtrl>::value> {};

    template <typename Fy>
    using ExcutorRetType_t = typename ExcutorRetTypeTrait<ExcutorHasGuarder<Fy>::value, Fy>::type;

    template <typename Fy>
    struct ExcutorSignatureCheck : public std::bool_constant<
        std_ext::is_callable<Fy>::value || std_ext::is_callable<Fy, StepCtrl>::value> {};

    template <bool, typename Fy>
    struct ExcutorCheckImpl : public std::bool_constant<
        std::is_void<ExcutorRetType_t<Fy>>::value ||
        std::is_same<bool, ExcutorRetType_t<Fy>>::value ||
        std::is_same<STEP_STATUS, ExcutorRetType_t<Fy>>::value> {};

    template <typename Fy>
    struct ExcutorCheckImpl<false, Fy> : public std::false_type {};

    template <typename Fy>
    struct ExcutorCheck : public std::bool_constant<
        ExcutorCheckImpl<ExcutorSignatureCheck<Fy>::value, Fy>::value> {};

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

    /* 内部使用的控制器, 共享内存分配器 */
    struct InnerStation1 : public StepExcutorStationBase
    {
        InnerStation1(SerialAllocator<>* alloc) : StepExcutorStationBase(alloc) { }
        inline StepCtrlImpl* GetCtrlImpl() const { return ctrl_; }
    };

    /* 内部使用的控制器, 独立使用内存分配器 */
    class InnerStation2 : public StepExcutorStation<128>
    {
    public:
        InnerStation2() : StepExcutorStation(1024) { }
        inline StepCtrlImpl* GetCtrlImpl() const { return ctrl_; }
    };

    /* 打包执行函数 */
    template <typename Fy, typename... Args>
    struct StepCallable;

    template <typename Fy>
    struct StepCallable<Fy>
    {
        static_assert(std_ext::is_callable<Fy>::value, "not a callable type");
        using RetType = typename std_ext::invoke_result<Fy>::type;

        StepCallable(Fy&& func) : func_(std::forward<Fy>(func)) { }

        inline RetType operator() () { return func_(); }
    private:
        Fy func_;
    };

    template <typename Fy>
    struct StepCallable<Fy, StepCtrlImpl*>
    {
        static_assert(std_ext::is_callable<Fy, StepCtrl>::value, "not a callable type");
        using RetType = typename std_ext::invoke_result<Fy, StepCtrl>::type;

        StepCallable(Fy&& func, StepCtrlImpl* ctrl) : func_(std::forward<Fy>(func)), ctrl_(ctrl) { }

        inline RetType operator() () { return func_(StepCtrl(ctrl_)); }
    private:
        Fy func_;
        StepCtrlImpl* ctrl_;
    };

    template <typename Fy, typename... Args>
    class ExcutorImplBase : public IStepExcutor
    {
        using Callable = StepCallable<Fy, Args...>;
    public:
        ExcutorImplBase(Fy&& func, Args&&... args) : func_(std::forward<Fy>(func), std::forward<Args>(args)...)
        { }
        virtual ~ExcutorImplBase() { }

        STEP_STATUS Step() override { return StepForward(func_); }
        void Rollback() override { }

    private:
        Callable func_;
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

    template <typename... Args>
    inline StepExcutorPtr PackageExcutor(SerialAllocator<>* alloc, Args&&... args)
    {
        using ExcutorType = ExcutorImpl<Args...>;
        return std::allocate_shared<ExcutorType>(alloc->GetAdapter<ExcutorType>(), std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline StepExcutorPtr MakeExcutorImpl(std::true_type, Args&&... args)
    {
        auto station = std::make_shared<InnerStation2>();
        auto step = PackageExcutor(station->GetCtrlImpl()->GetStepAlloc(), std::forward<Args>(args)..., station->GetCtrlImpl());
        station->GetCtrlImpl()->SubStep(step);
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
        auto station = std::allocate_shared<InnerStation1>(alloc->GetAdapter<InnerStation1>(), alloc);
        auto step = PackageExcutor(alloc, std::forward<Args>(args)..., station->GetCtrlImpl());
        station->GetCtrlImpl()->SubStep(step);
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
