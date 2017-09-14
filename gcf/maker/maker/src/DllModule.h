#pragma once

class DllModule
{
protected:
    typedef void(*Func)();
public:
    DllModule();
    ~DllModule();
    DllModule(const DllModule&) = delete;
    DllModule& operator = (const DllModule&) = delete;

public:
    bool Load(const char* name);

    template <class Fn>
    Fn GetProc(const char* name)
    {
        return (Fn)GetProcImpl(name);
    }

protected:
    Func GetProcImpl(const char* name);

protected:
    void* _module = nullptr;
};
