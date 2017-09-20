#pragma once
#include "gcf/cross_call/Station.h"
#include <functional>

class CrossStation : public cross_call::Station
{
public:
    static CrossStation* GetInstance();
    static bool Startup(std::function<bool()> caller, char* buffer, int size);

protected:
    CrossStation();
    ~CrossStation();

public:
    void Shutdown();

protected:
    virtual bool DoCall();

protected:
    static CrossStation* sInstance;
protected:
    std::function<bool()> _caller;
};
