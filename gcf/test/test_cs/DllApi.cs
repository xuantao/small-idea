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
}
