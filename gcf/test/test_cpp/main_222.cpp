#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <tuple>
#include <type_traits>
#include <cstdio>
#include <vector>
#include "header_b.h"
#include "extend.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/*
 * 带格式化输出日志的错误检测
*/
#define KGMLOG_PROCESS_ERROR(Condition, Format, ...) \
    do  \
    {   \
        if (!(Condition))       \
        {                       \
            KGMLog(1, "KGMLOG_PROCESS_ERROR", #Condition, __LINE__, "111", Format, ##__VA_ARGS__);    \
        }                       \
    } while (false)

#define KGMLOG_CHECK_ERROR(Condition, Format, ...) \
    do  \
    {   \
        if (!(Condition))       \
        {                       \
            KGMLog(1, "KGLOG_CHECK_ERROR", #Condition, __LINE__, "111", Format, ##__VA_ARGS__);   \
        }                       \
    } while (false)

//int KGMLog(
//    int Priority,
//    const char* szDescription,
//    const char* szCondition,
//    int nLine,
//    const char* szFunction,
//    const char* szFormat
//)
//{
//    return printf(
//        "%s(%s) at line %d in %s: %s\n",
//        szDescription,
//        szCondition,
//        nLine,
//        szFunction,
//        szFormat);
//}

//int KGMLog(
//    int Priority,
//    const char* szDescription,
//    const char* szCondition,
//    int nLine,
//    const char* szFunction,
//    const char* szFormat,
//    ...
//)
//{
//    char buff[1024] = {0};
//    va_list marker;
//    va_start(marker, szFormat);
//    vsnprintf(buff, 1024 - 1, szFormat, marker);
//    va_end(marker);
//
//    return printf(
//        "%s(%s) at line %d in %s: %s\n",
//        szDescription,
//        szCondition,
//        nLine,
//        szFunction,
//        buff);
//}

template <typename Ty>
struct Detect;

template <>
struct Detect<void>
{
};

struct DetectFunc
{
    static int Dec(const std::function<void()>& fn)
    {
        return 1;
    }

    template <typename Ty>
    static int Dec(const std::function<Ty()>& fn)
    {
        return 2;
    }
};

void test_1()
{
    printf("call test_1\n");
}

int test_2()
{
    printf("call test_2\n");
    return 1;
}



namespace timer_impl
{
    template <typename Ky>
    class TimerNode
    {
    public:
        typedef Ky key_type;

    public:
        template<typename K>
        TimerNode(K&& key) : _key(std::forward<K>(key)) {}

        virtual ~TimerNode() {}

        const Ky& Key() const { return _key; }

        virtual int Call() = 0;

    protected:
        key_type _key;
    };

    template <typename Ky, typename Fy, typename Ry = std::result_of<Fy()>::type>
    class TimerNodeImpl : public TimerNode<Ky>
    {
    public:
        typedef Ky key_type;
        typedef Fy func_type;
        typedef Ry return_type;
        typedef TimerNode<Ky> MyBase;

    public:
        template <typename K, typename F>
        TimerNodeImpl(K&& key, F&& func)
            : MyBase(std::forward<K>(key))
            , _func(std::forward<F>(func))
        {
        }

        virtual ~TimerNodeImpl() {}

        virtual int Call()
        {
            return (int)_func();
        }

    protected:
        func_type _func;
    };

    template <typename Ky, typename Fy>
    class TimerNodeImpl<Ky, Fy, void> : public TimerNode<Ky>
    {
    public:
        typedef Ky key_type;
        typedef Fy func_type;
        typedef void return_type;
        typedef TimerNode<Ky> MyBase;

    public:
        template <typename K, typename F>
        TimerNodeImpl(K&& key, F&& func)
            : MyBase(std::forward<K>(key))
            , _func(std::forward<F>(func))
        {
        }

        virtual ~TimerNodeImpl() {}

        virtual int Call()
        {
            _func();
            return 0;
        }

    protected:
        func_type _func;
    };

    template <typename Ky>
    class TimerHeader
    {
        //KNode _root;
    };


    template <typename Ky, typename Vl>
    struct TimerAction
    {
        typedef Ky Key;
        //typedef
    };

    template <typename Ky, typename Fy>
    TimerNode<Ky>* CreateNode(Ky&& key, Fy&& func)
    {
        auto node = new TimerNodeImpl<Ky, Fy>(std::forward<Ky>(key), std::forward<Fy>(func));
        return node;
    }
}


typedef timer_impl::TimerNode<int> TimerNode;

int main()
{
    //KGMLOG_PROCESS_ERROR(false, "%d", 1);
    ////KGMLOG_PROCESS_ERROR(false, nullptr);
    //KGMLOG_PROCESS_ERROR(false, "");
    //KGMLOG_CHECK_ERROR(false, "%d", 1);
    //bool aApply = false;
    //KGMLOG_PROCESS_ERROR(aApply, "xxTODO: 奇穴的反应用待实现");

    //int a = DetectFunc::Dec(test_1);
    //int b = DetectFunc::Dec(test_2);

    ////std::function<void()>::result_type;
    //std::function<int()>::result_type a1 = 2;

    auto p1 = timer_impl::CreateNode(1, test_1);
    auto p2 = timer_impl::CreateNode(1, test_2);
    p1->Call();
    p2->Call();
    return 1;
}
