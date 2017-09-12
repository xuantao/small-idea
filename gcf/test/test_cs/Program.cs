using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace cs_test
{

    class TestSend
    {
        public void Send(int a)
        {
            Console.WriteLine("Send");
        }
    }

    class TestArray
    {
        byte[] _data;

        public TestArray(byte[] data)
        {
            _data = data;
        }

        public void Check()
        {
            Console.WriteLine("1:{0}, 2:{1}", _data[0], _data[1]);
        }

        TestSend send = new TestSend();

        public TestSend Get()
        {
            Console.WriteLine("GetSend");
            return send;
        }
    }

    public class Global
    {
        public class InternalCall : CrossCall.ICrossCall
        {
            public void DoCall()
            {
                DllApi.OnCall();
            }
        }

        static Global s_instance = null;

        public static Global Instance { get { return s_instance; } }
        public static void Purge() { s_instance.Purse(); }

        CrossCall.Station _station;
        InternalCall _call;
        IntPtr _pool;

        public Global()
        {
            if (s_instance == null)
            {
                s_instance = this;
                _pool = Marshal.AllocHGlobal(1024);
                _call = new InternalCall();
                _station = new CrossCall.Station(_call, _pool, 1024);
                s2c = new TestS2C.Requester(_station.Invoker);

                _station.Register(TestC2S.MODULE_ID, new TestC2S.Processor(new ExecutorC2S()));
            }
        }

        public CrossCall.Station Station { get { return _station; } }

        public IntPtr Pool { get { return _pool; } }
        public int PoolSize { get { return 1024; } }

        public TestS2C.Requester s2c;

        void Purse()
        {
            s_instance = null;
            Marshal.FreeHGlobal(_pool);
        }

        public static void DoCall()
        {
            s_instance.Station.OnCall();
        }
    }

    public class Excutor : Caller.IResponder
    {
        public void Call_A(int a, int b)
        {
            Console.WriteLine("a:{0} b:{1}", a, b);
        }
        public int Call_B(string str)
        {
            Console.WriteLine("str:{0}", str);
            return 101;
        }
    }

    public class ExecutorC2S : TestC2S.IResponder
    {
        public bool Test(bool b)
        {
            Console.WriteLine("Test1 b:{0}", b);
            return false;
        }

        public int Test(int a)
        {
            Console.WriteLine("Test2 a:{0}", a);
            return 111;
        }

        public float Test(float f)
        {
            Console.WriteLine("Test3 f:{0}", f);
            return 0.99f;
        }

        public string Test(string s)
        {
            Console.WriteLine("Test4 s:{0}", s);
            return "C#.Test4";
        }

        public Msg Test(Msg msg)
        {
            Console.WriteLine("Test5 msg:{0}", msg);
            msg.inner.name = "C#.Test5";
            return msg;
        }

        public void Test(List<FixedArray<int, ArrayLength_10>> ar)
        {
            throw new NotImplementedException();
        }
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

        static int GetInt()
        {
            Console.WriteLine("GetInt");
            return 0;
        }

        static void TestCrossCall()
        {
            var global = new Global();

            if (!DllApi.Startup(Global.DoCall, global.Pool, global.PoolSize))
            {
                Console.WriteLine("starup dll failed");
                return;
            }

            //Caller.Invoker invoker = new Caller.Invoker(global.Station.Caller);
            //invoker.Call_A(1, 2);

            //Console.WriteLine("Call_B.ret:{0}", invoker.Call_B("xuantao"));
            var v1 = global.s2c.Test(true);
            var v2 = global.s2c.Test(222);
            var v3 = global.s2c.Test(0.111f);
            var v4 = global.s2c.Test("xuantao");
            var v5 = global.s2c.Test(new Msg());
            global.s2c.CallBack();

            DllApi.Stutdown();
        }

        static void Main(string[] args)
        {
            Console.ReadKey();
            TestCrossCall();
            Console.ReadKey();
        }
    }
}
