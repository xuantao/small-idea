using System;

public delegate bool CallFunc();


/*
 * TODO: 接口名字有待斟酌
*/
public interface ICaller
{
    bool Startup(CallFunc onCall, IntPtr buffer, int size);
    bool DoCall();
    void Shutdown();
}

public class CrossStation : CrossCall.Station
{
    static CrossStation sInstance = null;

    public static CrossStation Instance { get { return sInstance; } }

    public static bool Startup(ICaller caller, int poolSize)
    {
        if (sInstance != null)
            return false;

        var s = new CrossStation(caller);
        if (!s.DoInit(poolSize))
            return false;

        sInstance = s;
        return true;
    }

    ICaller _caller;

    CrossStation(ICaller caller)
    {
        _caller = caller;
    }

    bool DoInit(int size)
    {
        if (!base.Init(size))
            return false;

        if (!_caller.Startup(_OnCall, _ptr, size))
        {
            UnInit();
            return false;
        }

        return true;
    }

    public void Shutdown()
    {
        _caller.Shutdown();
        _caller = null;

        UnInit();

        if (sInstance == this)
            sInstance = null;
    }

    protected override bool DoCall()
    {
        return _caller.DoCall();
    }

    bool _OnCall()
    {
        return sInstance.OnCall();
    }
}
