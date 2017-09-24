using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Runtime.InteropServices;

class Writer : Serialize.IWriter
{
    public bool ArrayBegin(int length, string name = null)
    {
        Console.WriteLine("ArrayBegin(int length:{0}, string name:{1})", length, name);
        return true;
    }

    public bool ArrayEnd()
    {
        Console.WriteLine("ArrayEnd()");
        return true;
    }

    public bool StructBegin(int code, string name = null)
    {
        Console.WriteLine("StructBegin(int code:{0}, string name:{1})", code, name);
        return true;
    }

    public bool StructEnd()
    {
        Console.WriteLine("StructEnd()");
        return true;
    }

    public bool Write(bool val, string name = null)
    {
        Console.Write("ArrayBegin(bool val:{0}, string name:{1})", val, name);
        return true;
    }

    public bool Write(byte val, string name = null)
    {
        Console.WriteLine("Write(byte val:{0}, string name:{1})", val, name);
        return true;
    }

    public bool Write(int val, string name = null)
    {
        Console.WriteLine("Write(int val:{0}, string name:{1})", val, name);
        return true;
    }

    public bool Write(long val, string name = null)
    {
        Console.WriteLine("Write(long val:{0}, string name:{1})", val, name);
        return true;
    }

    public bool Write(float val, string name = null)
    {
        Console.WriteLine("Write(float val:{0}, string name:{1})", val, name);
        return true;
    }

    public bool Write(double val, string name = null)
    {
        Console.WriteLine("Write(double val:{0}, string name:{1})", val, name);
        return true;
    }

    public bool Write(string val, string name = null)
    {
        Console.WriteLine("Write(string val:{0}, string name:{1})", val, name);
        return true;
    }
}

namespace Serialize
{
    public static partial class Utility
    {

        public static void TestWrite()
        {
            Writer writer = new Writer();
            List<int> lst = new List<int> { 1, 2, 3 };

            List<List<int>> ll = new List<List<int>>();
            ll.Add(lst);
            ll.Add(lst);

            List<List<List<int>>> lll = new List<List<List<int>>>();
            lll.Add(ll);

            DoWrite(writer, lst, null, Write);
            //DoWrite(writer, ll, null, delegate (Serialize.IWriter w, List<int> l, string n)
            //{
            //    return DoWrite(w, l, n, Write);
            //});

            DoWrite(writer, ll, null, (w, l, n) =>
            {
                return DoWrite(w, l, n, Write);
            });

            DoWrite(writer, lll, null, (w1, l1, n1) =>
            {
                return DoWrite(w1, l1, n1, (w2, l2, n2) =>
                {
                    return DoWrite(w2, l2, n2, Write);
                });
            });
        }
    }
}

namespace cs_test
{
    class Caller : ICaller
    {
        protected Caller() { }

        public bool Startup(CallFunc onCall, IntPtr buffer, int size)
        {
            DllApi.Startup(onCall, buffer, size);
            return true;
        }

        public bool DoCall()
        {
            DllApi.OnCall();
            return true;
        }

        public void Shutdown()
        {
            DllApi.Stutdown();
        }

        public static Caller Instance = new Caller();
    }

    class Program
    {
        const int test_count = 10000;
        static void WriteArrayLength(int count)
        {
            for (int i = 1; i < count; ++i)
            {
                Console.WriteLine("public class ArrayLength_{0} : IArrayLength", i);
                Console.WriteLine("{");
                Console.WriteLine("    public int Length {{ get {{ return {0}; }} }}", i);
                Console.WriteLine("}");
                Console.WriteLine();
            }
        }

        static void TestCrossCall()
        {
            CrossStation.Startup(Caller.Instance, 1024 * 24);
            var station = CrossStation.Instance;
            station.Register(new TestC2S.Processor(new TestC2SRsp()));

            var req = new TestS2C.Requester(station.Invoker);
            //req.Test(false);
            //req.Test((byte)1);
            //req.Test((int)2);
            //req.Test((long)3);
            //req.Test(4.0f);
            //req.Test(5.0);
            //req.Test("xuantao");

            //var lb = new List<bool>();
            //lb.Add(true);
            //lb.Add(false);
            //req.Test(lb);

            //var fb = new FixedArray<bool, ArrayLength_2>();
            //fb[0] = true;
            //fb[1] = true;
            //req.Test(fb);

            //var fby = new FixedArray<byte, ArrayLength_2>();
            //fby[0] = 101;
            //fby[1] = 102;
            //req.Test(fby);

            //var li = new List<int> { 103, 104 };
            //var lli = new List<List<int>>();
            //lli.Add(li);
            //lli.Add(li);
            //req.Test(lli);

            //var fl = new FixedArray<long, ArrayLength_2>();
            //fby[0] = 101;
            //fby[1] = 102;
            //var lfl = new List<FixedArray<long, ArrayLength_2>>();
            //lfl.Add(fl);
            //lfl.Add(fl);
            //req.Test(lfl);

            //var flf = new FixedArray<List<float>, ArrayLength_2>();
            //flf[0].Add(.01f);
            //flf[0].Add(.01f);
            //flf[1].Add(.02f);
            //flf[1].Add(.02f);
            //req.Test(flf);

            //var msg = new Msg();
            //req.Test(msg);

            //var fmsg = new FixedArray<Msg, ArrayLength_2>();
            //req.Test(fmsg);

            //var rMsg = req.Test(1, 2);
            //var rlMsg = req.Test(1, 2, 3);
            //var fMsg = req.Test(1, 2, 3, 4);

            //req.Test();

            long cost = 0;
            Stopwatch watch = Stopwatch.StartNew();
            //watch.Start();
            //for (int i = 0; i < test_count; ++i)
            //{
            //    KGPlayerData data = req.GetPlayerData();
            //}
            //Console.WriteLine("test cross call, GetPlayerData cost:{0}", watch.ElapsedMilliseconds);

            //watch.Reset();
            watch.Start();
            for (int i = 0; i < test_count; ++i)
            {
                KGPlayerData data = new KGPlayerData();
                req.SetPlayerData(data);
            }
            cost = watch.ElapsedMilliseconds;
            Console.WriteLine("test cross call, SetPlayerData cost:{0}", cost);

            //watch.Reset();
            //watch.Start();
            //for (int i = 0; i < test_count; ++i)
            //{
            //    KGPlayerData data = new KGPlayerData();
            //    data = req.TranslatePlayerData(data);
            //}
            //cost = watch.ElapsedMilliseconds;
            //Console.WriteLine("test cross call, TranslatePlayerData cost:{0}", cost);

            watch.Stop();
            //ProfileManager.Instance.Log();


            ProfileManager.Instance.Start("test org int");
            for (int i = 0; i < test_count; ++i)
            {
                var val = DllApi.TestInt(i);
            }
            ProfileManager.Instance.Stop("test org int");

            ProfileManager.Instance.Start("test org float");
            for (int i = 0; i < test_count; ++i)
            {
                var val = DllApi.TestFloat((float)i);
            }
            ProfileManager.Instance.Stop("test org float");

            ProfileManager.Instance.Start("test org long");
            for (int i = 0; i < test_count; ++i)
            {
                var val = DllApi.TestInt64((long)i);
            }
            ProfileManager.Instance.Stop("test org long");

            ProfileManager.Instance.Start("test org double");
            for (int i = 0; i < test_count; ++i)
            {
                var val = DllApi.TestDouble((double)i);
            }
            ProfileManager.Instance.Stop("test org double");

            ProfileManager.Instance.Start("test cross int");
            for (int i = 0; i < test_count; ++i)
            {
                var val = req.Test(i);
            }
            ProfileManager.Instance.Stop("test cross int");

            ProfileManager.Instance.Start("test cross float");
            for (int i = 0; i < test_count; ++i)
            {
                var val = req.Test((float)i);
            }
            ProfileManager.Instance.Stop("test cross float");

            ProfileManager.Instance.Start("test cross long");
            for (int i = 0; i < test_count; ++i)
            {
                var val = req.Test((long)i);
            }
            ProfileManager.Instance.Stop("test cross long");

            ProfileManager.Instance.Start("test cross double");
            for (int i = 0; i < test_count; ++i)
            {
                var val = req.Test((double)i);
            }
            ProfileManager.Instance.Stop("test cross double");


            ProfileManager.Instance.Log();
        }

        static void TestOrgCall()
        {
            Stopwatch watch = Stopwatch.StartNew();
            watch.Start();

            for (int i = 0; i < test_count; ++i)
            {
                DummyJX3MClient.PlayerData playerData = new DummyJX3MClient.PlayerData(1);
                var size = Marshal.SizeOf(playerData);
                var ptr = Marshal.AllocHGlobal(size);
                Marshal.StructureToPtr(playerData, ptr, false);
                DllApi.GetPlayerData(ptr);
                //var newData = Marshal.PtrToStructure<DummyJX3MClient.PlayerData>(ptr);
                Marshal.FreeHGlobal(ptr);
            }

            long cost = watch.ElapsedMilliseconds;
            watch.Stop();

            Console.WriteLine("test org call, cost:{0}", cost);
        }

        static void Main(string[] args)
        {
            TestCrossCall();
            TestOrgCall();
            //List<int> l = new List<int> { 1, 2, 3 };

            //List<List<int>> ll = new List<List<int>>();
            //ll.Add(l);
            //ll.Add(l);

            //List<List<List<int>>> lll = new List<List<List<int>>>();
            //lll.Add(ll);

            //Console.Write("list 1");
            //Visit(l);

            //Console.Write("list 2");
            //Visit(ll);

            //Console.Write("list 3");
            //Visit(lll);

            //FixedArray<int, ArrayLength_2> f2 = new FixedArray<int, ArrayLength_2>();
            //List<FixedArray<int, ArrayLength_2>> lf = new List<FixedArray<int, ArrayLength_2>>();
            //lf.Add(f2);
            //Console.Write("list lf");
            //Visit(lf);
            //Serialize.Utility.TestWrite();
            //VisitList<List<List<int>>, List<int>>(lll);

            //VisitList<List<int>, int>(ll);

            //VisitList<int, int>(lst);

            //CrossStation.Instance.Shutdown();
            Console.ReadKey();
        }
    }
}
