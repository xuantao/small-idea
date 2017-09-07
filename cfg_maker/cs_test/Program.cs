using System;
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
        public class InternalCall : CrossCall.InternalCall
        {
            public void DoCall()
            {
                s_instance.Station.OnCall();
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
            }
        }

        public CrossCall.Station Station { get { return _station; } }

        void Purse()
        {
            s_instance = null;
            Marshal.FreeHGlobal(_pool);
        }
    }

    public class Excutor : Caller.IExecutor
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

        static void Main(string[] args)
        {
            var global = new Global();
            Caller.Invoker invoker = new Caller.Invoker(global.Station.Caller);
            Excutor exe = new Excutor();

            global.Station.Register(Caller.MODULE_ID, new Caller.Processor(exe));

            invoker.Call_A(1, 2);
            int ret = invoker.Call_B("xuantao");
            Console.WriteLine("ret:{0}", ret);

            Console.ReadKey();
        }
    }
}
