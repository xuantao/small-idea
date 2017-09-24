using System;
using System.Runtime.InteropServices;

public static class DllApi
{
    public const string DLL = "test_dll";

    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
    public static extern bool Startup(CallFunc doCall, IntPtr buffer, int size);

    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
    public static extern bool OnCall();

    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
    public static extern void Stutdown();

    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
    public static extern void GetPlayerData(IntPtr ptr);

    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
    public static extern int TestInt(int ptr);

    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
    public static extern float TestFloat(float ptr);

    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
    public static extern long TestInt64(long ptr);

    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
    public static extern double TestDouble(double ptr);
}
