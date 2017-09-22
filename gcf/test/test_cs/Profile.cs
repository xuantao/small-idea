using System;
using System.Collections.Generic;
using System.Diagnostics;


public class Profile
{
    Stopwatch _watch = new Stopwatch();

    string _name;
    long _times = 0;
    long _cost = 0;

    public string Name
    { get { return _name; } }

    public long Times
    { get { return _times; } }

    public long Cost
    { get { return _watch.ElapsedMilliseconds; } }

    public Profile(string name)
    {
        _name = name;
    }

    public void Start()
    {
        _watch.Start();
    }

    public void Stop()
    {
        ++_times;
        _watch.Stop();
    }

    public void Clear()
    {
        _times = 0;
        _watch.Stop();
        _watch.Reset();
    }
}

public class ProfileManager
{
    static ProfileManager sInstance = new ProfileManager();
    public static ProfileManager Instance
    { get { return sInstance; } }

    Dictionary<string, Profile> _dicProfile = new Dictionary<string, Profile>();

    public void Start(string name)
    {
        Makesure(name).Start();
    }

    public void Stop(string name)
    {
        Makesure(name).Stop();
    }

    public void Clear()
    {
        _dicProfile.Clear();
    }

    public void Log()
    {
        foreach (var pair in _dicProfile)
        {
            Console.WriteLine("name:{0}\tconst:{1}\ttimes:{2}", pair.Key, pair.Value.Cost, pair.Value.Times);
        }
    }

    Profile Makesure(string name)
    {
        Profile profile = null;
        if (!_dicProfile.TryGetValue(name, out profile))
        {
            profile = new Profile(name);
            _dicProfile.Add(name, profile);
        }

        return profile;
    }
}
