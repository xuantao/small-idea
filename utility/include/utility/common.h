/*
 * common type predef
 * xuantao, 2017
*/
#pragma once

#include <type_traits>
#include <cstdint>
#include <cassert>
#include <memory>

#define UTILITY_NAMESPACE_BEGIN namespace utility {
#define UTILITY_NAMESPACE_END }
#define UTILITY_USING_NAMESPACE using namespace utility;
#define UTILITY_NAMESPCE utility::

#define SCOPED_ALLOCATOR_BLOCK_SIZE 1024 * 1024
#define SCOPED_ALLOCATOR_ALIGN_SIZE sizeof(void*)
#define SCOPED_ALLOCATOR_BOUNDARY_CHECK 1
#define SCOPED_ALLOCATOR_MULTITHREAD 0

#define LOG_DEBUG_INFO 1

#if LOG_DEBUG_INFO
    #include <iostream>
#endif // LOG_DEBUG_INFO

// support c++ 11
namespace std_ext
{
    template<size_t...>
    struct index_sequence {};

    template <size_t N, size_t... Indices>
    struct make_index_sequence : make_index_sequence<N - 1, N - 1, Indices...> {};

    template<size_t... Indices>
    struct make_index_sequence<0, Indices...>
    {
        typedef index_sequence<Indices...> type;
    };

    template <size_t N>
    using make_index_sequence_t = typename make_index_sequence<N>::type;

    template <typename Ty>
    using decay_t = typename std::decay<Ty>::type;

    template <typename Fty, typename... Args>
    struct invoke_result
    {
        typedef typename std::result_of<Fty(Args...)>::type type;
    };

    /* check Fty is callable by specified parementers */
    template <typename Fty, typename... Args>
    struct is_callable
    {
        template<typename U> static auto Check(int) -> decltype(std::declval<U>()(std::declval<Args>()...), std::true_type());
        template<typename U> static std::false_type Check(...);

        static constexpr bool value = std::is_same<decltype(Check<Fty>(0)), std::true_type>::value;
    };

    template <typename Fty>
    struct is_callable<Fty, void>
    {
        template<typename U> static auto Check(int) -> decltype(std::declval<U>()(), std::true_type());
        template<typename U> static std::false_type Check(...);

        static constexpr bool value = std::is_same<decltype(Check<Fty>(0)), std::true_type>::value;
    };

    /* 从VC标准库拷贝出来的
     * 经测试clang 3.4和gcc 4.8 不支持std::align
    */
    inline void* align(size_t _Bound, size_t _Size,
        void*& _Ptr, size_t& _Space)
    {
        // try to carve out _Size bytes on boundary _Bound
        size_t _Off = (size_t)((uintptr_t)_Ptr & (_Bound - 1));
        if (0 < _Off)
            _Off = _Bound - _Off;	// number of bytes to skip
        if (_Space < _Off || _Space - _Off < _Size)
            return (0);
        else
        {   // enough room, update
            _Ptr = (char *)_Ptr + _Off;
            _Space -= _Off;
            return (_Ptr);
        }
    }
}

UTILITY_NAMESPACE_BEGIN

/* 单向链表 */
template <typename Ty>
struct singly_node
{
    typedef singly_node self_type;
    typedef Ty value_type;

    singly_node() { }

    template <typename... Args>
    singly_node(Args&&... args) : value(std::forward<Args>(args)...)
    { }

    self_type* next = nullptr;
    value_type value;
};

/* 单向链表 */
template <typename Ty>
struct SinglyNode
{
    SinglyNode() { }

    template <typename... Args>
    SinglyNode(Args&&... args) : val(std::forward<Args>(args)...)
    { }

    SinglyNode* next_node = nullptr;
    Ty val;
};

inline constexpr size_t AlignPadding(size_t sz, size_t bound = sizeof(void*))
{
    return bound == 0 ? 0 : (bound - sz % bound) % bound;
}

/* 对齐（1，2，4，8...） */
inline constexpr size_t AlignSize(size_t sz, size_t bound = sizeof(void*))
{
    return sz + AlignPadding(sz, bound);
}

template <typename Ty>
struct ICallable;

template <typename Rty, typename... Args>
struct ICallable<Rty(Args...)>
{
    virtual ~ICallable() { }

    virtual Rty Call(Args... args) = 0;
    virtual ICallable* Move(void* mem) = 0;
    virtual ICallable* Copy(void* mem) = 0;
};

template <typename Fty, typename... Args>
class CallableObject final : public ICallable<typename std_ext::invoke_result<Fty, Args...>::type (Args...)>
{
public:
    using RetType = typename std_ext::invoke_result<Fty, Args...>::type;
    using BaseType = ICallable<RetType(Args...)>;

    CallableObject(const Fty& fn) : func_(fn) { }
    CallableObject(Fty&& fn) : func_(std::forward<Fty>(fn)) { }
    ~CallableObject() { }

    RetType Call(Args... args) override
    {
        return func_(std::forward<Args>(args)...);
    }

    BaseType* Move(void* mem) override
    {
        return new (mem) CallableObject(std::move(func_));
    }

    BaseType* Copy(void* mem) override
    {
        return new (mem) CallableObject(func_);
    }

private:
    Fty func_;
};

/* 小对象缓存池大小配置 */
constexpr int SmallObjectNumPtrs = 6 + 16 / sizeof(void*);
constexpr size_t SpaceSize = (SmallObjectNumPtrs - 1) * sizeof(void*);

template <typename Ty>
struct IsLarge : std::bool_constant<SpaceSize < sizeof(Ty)> {};

template <typename Ty>
class SmallObjStorage
{
public:
    SmallObjStorage() { Set(nullptr); }
    ~SmallObjStorage() { Release(); }

    /* not support copy, unknown how to copy */
    SmallObjStorage(const SmallObjStorage&) = delete;
    SmallObjStorage& operator = (const SmallObjStorage&) = delete;

public:
    inline bool IsEmpty() const { return Get() == nullptr; }
    inline bool IsLocal() const { return GetSpace() == Get(); }
    inline Ty* Get() const { return (real_.ptrs_[SmallObjectNumPtrs - 1]); }
    inline void Set(Ty* ptr) { real_.ptrs_[SmallObjectNumPtrs - 1] = ptr; }
    inline void* GetSpace() { return (&real_); }
    inline const void *GetSpace() const { return (&real_); }

    template <typename Impl, typename... Args>
    void Construct(Args&&... args)
    {
        Release();
        DoConstruct<Impl>(IsLarge<Impl>(), std::forward<Args>(args)...);
    }

    void Release()
    {
        if (IsEmpty())
            return;

        if (IsLocal())
            Get()->~Ty();
        else
            delete Get();
        Set(nullptr);
    }

private:
    template <typename Impl, typename... Args>
    inline void DoConstruct(std::true_type, Args&&... args)
    {
        Set(new Impl(std::forward<Args>(args)...));
    }

    template <typename Impl, typename... Args>
    inline void DoConstruct(std::false_type, Args&&... args)
    {
        Set(new (GetSpace()) Impl(std::forward<Args>(args)...));
    }

private:
    union
    {
        std::max_align_t dummy_1_;
        char dummy_2_[SpaceSize];
        Ty* ptrs_[SmallObjectNumPtrs];
    } real_;
};

UTILITY_NAMESPACE_END
