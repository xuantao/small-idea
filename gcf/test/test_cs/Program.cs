using System;
using System.Collections.Generic;

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
            CrossStation.Startup(Caller.Instance, 1024);
            var station = CrossStation.Instance;
            station.Register(new TestC2S.Processor(new TestC2SRsp()));

            var req = new TestS2C.Requester(station.Invoker);
            req.Test(false);
            req.Test((byte)1);
            req.Test((int)2);
            req.Test((long)3);
            req.Test(4.0f);
            req.Test(5.0);
            req.Test("xuantao");

            var lb = new List<bool>();
            lb.Add(true);
            lb.Add(false);
            req.Test(lb);

            var fb = new FixedArray<bool, ArrayLength_2>();
            fb[0] = true;
            fb[1] = true;
            req.Test(fb);

            var fby = new FixedArray<byte, ArrayLength_2>();
            fby[0] = 101;
            fby[1] = 102;
            req.Test(fby);

            var li = new List<int> { 103, 104 };
            var lli = new List<List<int>>();
            lli.Add(li);
            lli.Add(li);
            req.Test(lli);

            var fl = new FixedArray<long, ArrayLength_2>();
            fby[0] = 101;
            fby[1] = 102;
            var lfl = new List<FixedArray<long, ArrayLength_2>>();
            lfl.Add(fl);
            lfl.Add(fl);
            req.Test(lfl);

            var flf = new FixedArray<List<float>, ArrayLength_2>();
            flf[0].Add(.01f);
            flf[0].Add(.01f);
            flf[1].Add(.02f);
            flf[1].Add(.02f);
            req.Test(flf);

            var msg = new Msg();
            req.Test(msg);

            var fmsg = new FixedArray<Msg, ArrayLength_2>();
            req.Test(fmsg);

            var rMsg = req.Test(1, 2);
            var rlMsg = req.Test(1, 2, 3);
            var fMsg = req.Test(1, 2, 3, 4);

            req.Test();

            station.Shutdown();
        }

        static void Visit(int val)
        {
            Console.WriteLine("Visit(int val:{0})", val);
        }

        static void Visit(float val)
        {
            Console.WriteLine("Visit(int val:{0})", val);
        }

        //static void Visit<T>(T val)
        //{
        //    Console.WriteLine("Visit(int val:{0})", val);
        //}


        //static void Visit<T>(List<T> lst)
        //{
        //    foreach (var v in lst)
        //        Visit(v);
        //}

        //static void Visit<T, K>(T obj) where T : IList<K>
        //{
        //    Console.WriteLine("Visit<T, K>(T obj:{0})", obj);
        //    foreach (var v in obj)
        //        Visit(v);
        //}

        //static void VisitList<T, K>(object obj)
        //{
        //    // Console.WriteLine("obj:{0}", obj);

        //    if (obj is List<T>)
        //        foreach (var o in (obj as List<T>))
        //        {
        //            VisitList<K, K>(o);
        //        }
        //    else
        //        Visit(obj);
        //}

        static void Visit(object val)
        {
            Console.WriteLine("{0}", val);
        }

        static void Visit<T, L>(FixedArray<T, L> ar)
            where T : new()
            where L : IArrayLength, new()
        {
            for (int i = 0; i < ar.Length; ++i)
                Console.WriteLine("fa:{0}", ar[i]);
        }

        static void Visit<T>(List<T> l)
        {
            foreach (var v in l)
                Visit(v);
        }

        static void Visit<T>(List<List<T>> ll)
        {
            foreach (var l in ll)
                Visit(l);
        }

        static void Visit<T>(List<List<List<T>>> lll)
        {
            foreach (var ll in lll)
                Visit(ll);
        }

        //static void Visit<T>(T obj)
        //{
        //    Console.WriteLine("obj:{0}", obj);
        //}

        //static void DoRead<T>(ref List<T> val, Action<ref T> call)
        //{
        //    int size = 1;

        //    for (int i = 0; i < size; ++i)
        //    {
        //        T t = default(T);
        //        call(ref t);
        //        val.Add(t);
        //    }
        //}

        static void Main(string[] args)
        {
            TestCrossCall();

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

            Console.ReadKey();
        }
    }
}
