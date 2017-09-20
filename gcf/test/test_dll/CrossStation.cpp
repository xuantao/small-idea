#include "CrossStation.h"

CrossStation* CrossStation::sInstance = nullptr;

CrossStation* CrossStation::GetInstance()
{
    return sInstance;
}

bool CrossStation::Startup(std::function<bool()> caller, char* buffer, int size)
{
    if (sInstance)
        return false;

    CrossStation* station = new CrossStation();
    station->_caller = caller;

    if (!station->Init(buffer, size))
    {
        delete station;
        return false;
    }

    sInstance = station;
    return false;
}

CrossStation::CrossStation()
{
}

CrossStation::~CrossStation()
{
}

void CrossStation::Shutdown()
{
    UnInit();
    delete sInstance;
    sInstance = nullptr;
}

bool CrossStation::DoCall()
{
    if (!_caller)
        return false;

    return _caller();
}
