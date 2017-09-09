#include "cross/Station.h"
#include "cross/Caller.h"
#include "cross/TestC2S.h"
#include "cross/TestS2C.h"

#define DLL_EXPORT extern "C" __declspec(dllexport)

class Executor : public Caller::IResponder
{
public:
    virtual ~Executor() {}

public:
    virtual void Call_A(int a, int b)
    {
        printf("Call_A(a:%d, b:%d)\n", a, b);
    }

    virtual int Call_B(const std::string& str)
    {
        printf("Call_B(str:%s)\n", str.c_str());
        return 101;
    }
};

TestC2S::Requester* g_Invoker = nullptr;

class ExeS2C : public TestS2C::IResponder
{
public:
    virtual bool Test(bool b)
    {
        printf("cpp Test1.b:%d\n", b);
        return false;
    }

    virtual int Test(int a)
    {
        printf("cpp Test2.a:%d\n", a);
        return 3334444;
    }
    virtual float Test(float f)
    {
        printf("cpp Test3.f:%f\n", f);
        return 0.2222f;
    }

    virtual std::string Test(const std::string& s)
    {
        printf("cpp Test4.s:%s\n", s.c_str());
        return "cpp yaner";
    }

    virtual Msg Test(const Msg& msg)
    {
        printf("cpp Test4.msg:%s\n", msg.inner.name.c_str());
        Msg m2 = msg;

        m2.inner.name = "cpp Test5";
        return m2;
    }

    virtual void CallBack()
    {
        g_Invoker->Test(false);
        g_Invoker->Test(444);
        g_Invoker->Test(0.5555f);
        g_Invoker->Test("cpp Callback");
        g_Invoker->Test(Msg());
    }
};


class World
{
public:
    bool Init(cross_call::ICrossCall* caller, void* buffer, int size)
    {
        _executor = new Executor();
        _station = new cross_call::Station(caller, (char*)buffer, size);
        _progress = new Caller::Processor(_executor);

        _station->Register(Caller::MODULE_ID, _progress);
        g_Invoker = new TestC2S::Requester(_station->Invoker());

        _station->Register(TestS2C::MODULE_ID, new TestS2C::Processor(new ExeS2C()));

        return true;

    }

    void Uninit()
    {
        delete _station;
        _station = nullptr;

        delete _progress;
        _progress = nullptr;

        delete _executor;
        _executor = nullptr;
    }

    cross_call::Station* GetStation() { return _station; }

private:
    Executor* _executor = nullptr;
    Caller::Processor* _progress = nullptr;
    cross_call::Station* _station = nullptr;
};


typedef void(*FnDoCall)();

struct CrossCaller : cross_call::ICrossCall
{
    FnDoCall fnCall = nullptr;

    void DoCall()
    {
        if (fnCall != nullptr)
            fnCall();
        else
            printf("not init\n");
    }
};

CrossCaller g_Caller;
World g_World;

DLL_EXPORT bool Startup(FnDoCall call, void* buffer, int size)
{
    g_Caller.fnCall = call;
    return g_World.Init(&g_Caller, buffer, size);
}

DLL_EXPORT void OnCall()
{
    g_World.GetStation()->OnCall();
}

DLL_EXPORT void Stutdown()
{
    g_World.Uninit();
    g_Caller.fnCall = nullptr;
}
