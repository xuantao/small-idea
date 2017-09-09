using System;
using System.Runtime.InteropServices;

public static class DllApi
{
    public const string DLL = "dll_test";

    public delegate void DoCall();

    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
    public static extern bool Startup(DoCall doCall, IntPtr buffer, int size);

    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
    public static extern void OnCall();

    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)]
    public static extern void Stutdown();
}
