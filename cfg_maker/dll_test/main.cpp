#include "cross/Station.h"
#include "cross/Caller.h"

#define DLL_EXPORT extern "C" __declspec(dllexport)

class Executor : public Caller::IExecutor
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


class World
{
public:
    bool Init(cross_call::ICaller* caller, void* buffer, int size)
    {
        _executor = new Executor();
        _station = new cross_call::Station(caller, (char*)buffer, size);
        _progress = new Caller::Processor(_executor);

        _station->Register(Caller::MODULE_ID, _progress);
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

private:
    Executor* _executor = nullptr;
    Caller::Processor* _progress = nullptr;
    cross_call::Station* _station = nullptr;
};


typedef void(*FnDoCall)();

struct CrossCaller : cross_call::ICaller
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

DLL_EXPORT void Stutdown()
{
    g_World.Uninit();
    g_Caller.fnCall = nullptr;
}
