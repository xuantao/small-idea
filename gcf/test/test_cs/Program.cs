using System;
using System.Collections.Generic;

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
        static void Visit<T, K>(List<K> lst)
        {
            foreach (var v in lst)
                Visit<K>(v);
        }

        static void Visit<T>(T obj)
        {
            Console.WriteLine("obj:{0}", obj);
        }

        static void Main(string[] args)
        {
            //TestCrossCall();

            List<int> lst = new List<int> { 1, 2, 3 };
            List<List<int>> ll = new List<List<int>>();
            ll.Add(lst);
            ll.Add(lst);

            Visit<int>(lst);
            Visit<List<int>, int>(ll);

            Console.ReadKey();
        }
    }
}
