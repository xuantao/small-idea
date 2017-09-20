#include "gcf/gcf.h"
#include "gen/Caller.h"
#include "gen/TestC2S.h"
#include "gen/TestS2C.h"
#include "CrossStation.h"

#define DLL_EXPORT extern "C" __declspec(dllexport)

std::shared_ptr<TestC2S::Requester> g_Invoker;

class ExeS2C : public TestS2C::IResponder
{
public:
    virtual bool Test(bool b)
    {
        printf("cpp Test(bool v:%s)\n", b ? "true" : "false");
        return !b;
    }

    virtual int8_t Test(int8_t v)
    {
        printf("cpp Test(int8_t v:%d)", v);
        return v + 1;
    }

    virtual int Test(int32_t v)
    {
        printf("cpp Test(int32_t v:%d)\n", v);
        return v + 1;
    }

    virtual int64_t Test(int64_t v)
    {
        printf("cpp Test(int64_t v:%lld)\n", v);
        return v + 1;
    }

    virtual float Test(float f)
    {
        printf("cpp Test(float f:%f)\n", f);
        return f + 1.0f;
    }

    virtual double Test(double v)
    {
        printf("cpp Test(double v:%f)\n", v);
        return v + 1.0f;
    }

    virtual std::string Test(const std::string& s)
    {
        printf("cpp Test(const std::string& s:%s)\n", s.c_str());
        return "cpp yaner";
    }

    virtual void Test(const std::vector<bool>& v)
    {
        printf("cpp Test(const std::vector<bool>& v)\n");
    }

    virtual void Test(const std::array<bool, 2>& v)
    {
        printf("cpp Test(const std::array<bool, 2>& v)\n");
    }

    virtual void Test(const std::array<int8_t, 2>& v)
    {
        printf("cpp Test(const std::array<int8_t, 2>& v)\n");
    }

    virtual void Test(const std::vector<std::vector<int> >& v)
    {
        printf("cpp Test(const std::vector<std::vector<int> >& v)\n");
    }

    virtual void Test(const std::vector<std::array<int64_t, 2> >& v)
    {
        printf("cpp Test(const std::vector<std::array<int64_t, 2> >& v)\n");
    }

    virtual void Test(const std::array<std::vector<float>, 2>& v)
    {
        printf("cpp Test(const std::array<std::vector<float>, 2>& v)\n");
    }

    virtual void Test(const Msg& msg)
    {
        printf("cpp Test(const Msg& msg:%s)\n", msg._inner.name.c_str());
    }

    virtual void Test(const std::array<Msg, 2>& msg)
    {
        printf("cpp Test(const std::array<Msg, 2>& msg)\n");
    }

    virtual Msg Test(int a, int b)
    {
        printf("cpp Test(int a:%d, int b:%d)\n", a, b);
        return Msg();
    }

    virtual std::vector<Msg> Test(int a, int b, int c)
    {
        printf("cpp Test(int a:%d, int b:%d, int c:%d)\n", a, b, c);
        return std::vector<Msg>();
    }

    virtual std::array<Msg, 2> Test(int a, int b, int c, int d)
    {
        printf("cpp Test(int a:%d, int b:%d, int c:%d, int d:%d)\n", a, b, c, d);
        return std::array<Msg, 2>();
    }

    virtual void Test()
    {
        g_Invoker->Test(true);
        g_Invoker->Test((int8_t)1);
        g_Invoker->Test((int32_t)2);
        g_Invoker->Test((int64_t)3);
        g_Invoker->Test((float)4.0f);
        g_Invoker->Test((double)5.0);
        g_Invoker->Test("cpp Callback");

        g_Invoker->Test(std::vector<bool>());
        g_Invoker->Test(std::array<int8_t, 2>());
        g_Invoker->Test(std::vector<std::vector<int> >());
        g_Invoker->Test(std::vector<std::array<int64_t, 2> >());
        g_Invoker->Test(std::array<std::vector<float>, 2>());

        g_Invoker->Test();
        g_Invoker->Test(Msg());
        g_Invoker->Test(std::array<Msg, 2>());
        g_Invoker->Test(1, 2);
        g_Invoker->Test(1, 2, 3);
        g_Invoker->Test(1, 2, 3, 4);
    }
};

typedef bool(*FnDoCall)();

DLL_EXPORT bool Startup(FnDoCall call, void* buffer, int size)
{
    if (!CrossStation::Startup(call, (char*)buffer, size))
        return false;

    CrossStation::GetInstance()->Register(
        std::make_shared<TestS2C::Processor>(std::make_shared<ExeS2C>()));

    g_Invoker = std::make_shared<TestC2S::Requester>(
        CrossStation::GetInstance()->Invoker());
    return true;
}

DLL_EXPORT bool OnCall()
{
    return CrossStation::GetInstance()->OnCall();
}

DLL_EXPORT void Stutdown()
{
    CrossStation::GetInstance()->Shutdown();
}
