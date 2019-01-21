/*
 * common type predef
 * xuantao, 2017
*/
#pragma once
#include <type_traits>
#include <tuple>
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

    template <typename Fy, typename... Args>
    struct invoke_result
    {
        typedef typename std::result_of<Fy(Args...)>::type type;
    };

    /* check Fy is callable by specified parementers */
    template <typename Fy, typename... Args>
    struct is_callable
    {
        template<typename U> static auto Check(int) -> decltype(std::declval<U>()(std::declval<Args>()...), std::true_type());
        template<typename U> static std::false_type Check(...);

        static constexpr bool value = std::is_same<decltype(Check<Fy>(0)), std::true_type>::value;
    };

    template <typename Fy>
    struct is_callable<Fy, void>
    {
        template<typename U> static auto Check(int) -> decltype(std::declval<U>()(), std::true_type());
        template<typename U> static std::false_type Check(...);

        static constexpr bool value = std::is_same<decltype(Check<Fy>(0)), std::true_type>::value;
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
    SinglyNode(Args&&... args) : value(std::forward<Args>(args)...) { }

    SinglyNode* next = nullptr;
    Ty value;
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

template <typename Ry, typename... Args>
struct ICallable<Ry(Args...)>
{
    virtual ~ICallable() { }

    virtual Ry Call(Args... args) = 0;
    virtual ICallable* Move(void* mem) = 0;
    virtual ICallable* Copy(void* mem) = 0;
};

template <typename Fy, typename... Args>
class CallableObject final : public ICallable<typename std_ext::invoke_result<Fy, Args...>::type (Args...)>
{
public:
    using RetType = typename std_ext::invoke_result<Fy, Args...>::type;
    using BaseType = ICallable<RetType(Args...)>;

    CallableObject(const Fy& fn) : func_(fn) { }
    CallableObject(Fy&& fn) : func_(std::forward<Fy>(fn)) { }
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
    Fy func_;
};

template <typename Fy, typename... Args>
class CallablePackage
{
public:
    static_assert(std_ext::is_callable<Fy, Args...>::value, "not a callable type");
    typedef typename std_ext::invoke_result<Fy, Args...>::type RetType;

    CallablePackage(Fy&& func, Args&&... args)
        : func_(std::forward<Fy>(func))
        , args_(std::forward<Args>(args)...)
    {
    }

    inline RetType operator ()()
    {
        return DoCall(std_ext::make_index_sequence_t<sizeof...(Args)>());
    }

private:
    template <size_t... Idx>
    inline RetType DoCall(std_ext::index_sequence<Idx...>)
    {
        return func_(std::get<Idx>(args_)...);
    }

private:
    Fy func_;
    std::tuple<Args...> args_;
};

template <typename Fy>
class CallablePackage<Fy>
{
public:
    static_assert(std_ext::is_callable<Fy>::value, "not a callable type");
    typedef typename std_ext::invoke_result<Fy>::type RetType;

    CallablePackage(Fy&& func) : func_(std::forward<Fy>(func))
    { }

    inline RetType operator ()() { return func_(); }

private:
    Fy func_;
};

UTILITY_NAMESPACE_END
