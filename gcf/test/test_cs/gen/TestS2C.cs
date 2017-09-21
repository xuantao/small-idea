/*
 * this file is auto generated.
 * please does not edit it manual!
*/
using System;
using System.Collections;
using System.Collections.Generic;

public class TestS2C
{
    public const int MODULE_ID = 3;
    public const int HASH_CODE = 850647883;

    public enum Message
    {
        Invalid = 0,
        Test_bool,
        Test_byte,
        Test_int,
        Test_long,
        Test_float,
        Test_double,
        Test_string,
        Test_bool_A,
        Test_bool_A2,
        Test_byte_A2,
        Test_int_A_A,
        Test_long_A2_A,
        Test_float_A_A2,
        Test,
        Test_Msg,
        Test_Msg_A2,
        Test_int_int,
        Test_int_int_int,
        Test_int_int_int_int,
    }

    public interface IResponder
    {
        bool Test(bool v);
        byte Test(byte v);
        int Test(int v);
        long Test(long v);
        float Test(float v);
        double Test(double v);
        string Test(string v);
        void Test(List<bool> v);
        void Test(FixedArray<bool, ArrayLength_2> v);
        void Test(FixedArray<byte, ArrayLength_2> v);
        void Test(List<List<int>> v);
        void Test(List<FixedArray<long, ArrayLength_2>> v);
        void Test(FixedArray<List<float>, ArrayLength_2> v);
        void Test();
        void Test(Msg msg);
        void Test(FixedArray<Msg, ArrayLength_2> msg);
        Msg Test(int a, int b);
        List<Msg> Test(int a, int b, int c);
        FixedArray<Msg, ArrayLength_2> Test(int a, int b, int c, int d);
    };

    public class Requester
    {
        protected CrossCall.IInvoker _invoker = null;

        public Requester(CrossCall.IInvoker invoker)
        {
            _invoker = invoker;
        }

        public bool Test(bool v)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_bool);

            Serialize.Utility.Write(writer, v, "v");

            bool __ret__ = false;
            Serialize.Utility.Read(_invoker.End(), ref __ret__);
            return __ret__;
        }

        public byte Test(byte v)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_byte);

            Serialize.Utility.Write(writer, v, "v");

            byte __ret__ = 0;
            Serialize.Utility.Read(_invoker.End(), ref __ret__);
            return __ret__;
        }

        public int Test(int v)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_int);

            Serialize.Utility.Write(writer, v, "v");

            int __ret__ = 0;
            Serialize.Utility.Read(_invoker.End(), ref __ret__);
            return __ret__;
        }

        public long Test(long v)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_long);

            Serialize.Utility.Write(writer, v, "v");

            long __ret__ = 0;
            Serialize.Utility.Read(_invoker.End(), ref __ret__);
            return __ret__;
        }

        public float Test(float v)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_float);

            Serialize.Utility.Write(writer, v, "v");

            float __ret__ = 0.0f;
            Serialize.Utility.Read(_invoker.End(), ref __ret__);
            return __ret__;
        }

        public double Test(double v)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_double);

            Serialize.Utility.Write(writer, v, "v");

            double __ret__ = 0.0;
            Serialize.Utility.Read(_invoker.End(), ref __ret__);
            return __ret__;
        }

        public string Test(string v)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_string);

            Serialize.Utility.Write(writer, v, "v");

            string __ret__ = "";
            Serialize.Utility.Read(_invoker.End(), ref __ret__);
            return __ret__;
        }

        public void Test(List<bool> v)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_bool_A);

            Serialize.Utility.DoWrite(writer, v, "v", Serialize.Utility.Write);

            _invoker.End();
        }

        public void Test(FixedArray<bool, ArrayLength_2> v)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_bool_A2);

            Serialize.Utility.DoWrite(writer, v, "v", Serialize.Utility.Write);

            _invoker.End();
        }

        public void Test(FixedArray<byte, ArrayLength_2> v)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_byte_A2);

            Serialize.Utility.DoWrite(writer, v, "v", Serialize.Utility.Write);

            _invoker.End();
        }

        public void Test(List<List<int>> v)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_int_A_A);

            Serialize.Utility.DoWrite(writer, v, "v", (w1, v1, n1) => {
                return Serialize.Utility.DoWrite(w1, v1, n1, Serialize.Utility.Write);
            });

            _invoker.End();
        }

        public void Test(List<FixedArray<long, ArrayLength_2>> v)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_long_A2_A);

            Serialize.Utility.DoWrite(writer, v, "v", (w1, v1, n1) => {
                return Serialize.Utility.DoWrite(w1, v1, n1, Serialize.Utility.Write);
            });

            _invoker.End();
        }

        public void Test(FixedArray<List<float>, ArrayLength_2> v)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_float_A_A2);

            Serialize.Utility.DoWrite(writer, v, "v", (w1, v1, n1) => {
                return Serialize.Utility.DoWrite(w1, v1, n1, Serialize.Utility.Write);
            });

            _invoker.End();
        }

        public void Test()
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test);


            _invoker.End();
        }

        public void Test(Msg msg)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_Msg);

            Serialize.Utility.Write(writer, msg, "msg");

            _invoker.End();
        }

        public void Test(FixedArray<Msg, ArrayLength_2> msg)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_Msg_A2);

            Serialize.Utility.DoWrite(writer, msg, "msg", Serialize.Utility.Write);

            _invoker.End();
        }

        public Msg Test(int a, int b)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_int_int);

            Serialize.Utility.Write(writer, a, "a");
            Serialize.Utility.Write(writer, b, "b");

            Msg __ret__ = new Msg();
            Serialize.Utility.Read(_invoker.End(), ref __ret__);
            return __ret__;
        }

        public List<Msg> Test(int a, int b, int c)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_int_int_int);

            Serialize.Utility.Write(writer, a, "a");
            Serialize.Utility.Write(writer, b, "b");
            Serialize.Utility.Write(writer, c, "c");

            List<Msg> __ret__ = new List<Msg>();
            Serialize.Utility.DoRead(_invoker.End(), ref __ret__, "", Serialize.Utility.Read);
            return __ret__;
        }

        public FixedArray<Msg, ArrayLength_2> Test(int a, int b, int c, int d)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_int_int_int_int);

            Serialize.Utility.Write(writer, a, "a");
            Serialize.Utility.Write(writer, b, "b");
            Serialize.Utility.Write(writer, c, "c");
            Serialize.Utility.Write(writer, d, "d");

            FixedArray<Msg, ArrayLength_2> __ret__ = new FixedArray<Msg, ArrayLength_2>();
            Serialize.Utility.DoRead(_invoker.End(), ref __ret__, "", Serialize.Utility.Read);
            return __ret__;
        }
    }

    public class Processor : CrossCall.IProcessor
    {
        protected IResponder _responder = null;

        public Processor(IResponder responder)
        {
            _responder = responder;
        }

        public int ID { get { return MODULE_ID; } }

        public void Process(CrossCall.IContext context)
        {
            int code = 0;
            int tmp = 0;
            context.Param.Read(ref code);
            context.Param.Read(ref tmp);
            //TODO: check code == HASH_CODE

            Message msg = (Message)tmp;
            switch (msg)
            {
            case Message.Test_bool:
                OnTest_bool(context);
                break;
            case Message.Test_byte:
                OnTest_byte(context);
                break;
            case Message.Test_int:
                OnTest_int(context);
                break;
            case Message.Test_long:
                OnTest_long(context);
                break;
            case Message.Test_float:
                OnTest_float(context);
                break;
            case Message.Test_double:
                OnTest_double(context);
                break;
            case Message.Test_string:
                OnTest_string(context);
                break;
            case Message.Test_bool_A:
                OnTest_bool_A(context);
                break;
            case Message.Test_bool_A2:
                OnTest_bool_A2(context);
                break;
            case Message.Test_byte_A2:
                OnTest_byte_A2(context);
                break;
            case Message.Test_int_A_A:
                OnTest_int_A_A(context);
                break;
            case Message.Test_long_A2_A:
                OnTest_long_A2_A(context);
                break;
            case Message.Test_float_A_A2:
                OnTest_float_A_A2(context);
                break;
            case Message.Test:
                OnTest(context);
                break;
            case Message.Test_Msg:
                OnTest_Msg(context);
                break;
            case Message.Test_Msg_A2:
                OnTest_Msg_A2(context);
                break;
            case Message.Test_int_int:
                OnTest_int_int(context);
                break;
            case Message.Test_int_int_int:
                OnTest_int_int_int(context);
                break;
            case Message.Test_int_int_int_int:
                OnTest_int_int_int_int(context);
                break;
            default:
                break;
            }
        }

        void OnTest_bool(CrossCall.IContext context)
        {
            bool v = false;

            Serialize.Utility.Read(context.Param, ref v, "v");

            var __ret__ = _responder.Test(v);
            Serialize.Utility.Write(context.Ret(), __ret__);
        }

        void OnTest_byte(CrossCall.IContext context)
        {
            byte v = 0;

            Serialize.Utility.Read(context.Param, ref v, "v");

            var __ret__ = _responder.Test(v);
            Serialize.Utility.Write(context.Ret(), __ret__);
        }

        void OnTest_int(CrossCall.IContext context)
        {
            int v = 0;

            Serialize.Utility.Read(context.Param, ref v, "v");

            var __ret__ = _responder.Test(v);
            Serialize.Utility.Write(context.Ret(), __ret__);
        }

        void OnTest_long(CrossCall.IContext context)
        {
            long v = 0;

            Serialize.Utility.Read(context.Param, ref v, "v");

            var __ret__ = _responder.Test(v);
            Serialize.Utility.Write(context.Ret(), __ret__);
        }

        void OnTest_float(CrossCall.IContext context)
        {
            float v = 0.0f;

            Serialize.Utility.Read(context.Param, ref v, "v");

            var __ret__ = _responder.Test(v);
            Serialize.Utility.Write(context.Ret(), __ret__);
        }

        void OnTest_double(CrossCall.IContext context)
        {
            double v = 0.0;

            Serialize.Utility.Read(context.Param, ref v, "v");

            var __ret__ = _responder.Test(v);
            Serialize.Utility.Write(context.Ret(), __ret__);
        }

        void OnTest_string(CrossCall.IContext context)
        {
            string v = "";

            Serialize.Utility.Read(context.Param, ref v, "v");

            var __ret__ = _responder.Test(v);
            Serialize.Utility.Write(context.Ret(), __ret__);
        }

        void OnTest_bool_A(CrossCall.IContext context)
        {
            List<bool> v = new List<bool>();

            Serialize.Utility.DoRead(context.Param, ref v, "v", Serialize.Utility.Read);

            _responder.Test(v);
        }

        void OnTest_bool_A2(CrossCall.IContext context)
        {
            FixedArray<bool, ArrayLength_2> v = new FixedArray<bool, ArrayLength_2>();

            Serialize.Utility.DoRead(context.Param, ref v, "v", Serialize.Utility.Read);

            _responder.Test(v);
        }

        void OnTest_byte_A2(CrossCall.IContext context)
        {
            FixedArray<byte, ArrayLength_2> v = new FixedArray<byte, ArrayLength_2>();

            Serialize.Utility.DoRead(context.Param, ref v, "v", Serialize.Utility.Read);

            _responder.Test(v);
        }

        void OnTest_int_A_A(CrossCall.IContext context)
        {
            List<List<int>> v = new List<List<int>>();

            Serialize.Utility.DoRead(context.Param, ref v, "v", delegate (Serialize.IReader r1, ref List<int> v1, string n1) {
                return Serialize.Utility.DoRead(r1, ref v1, n1, Serialize.Utility.Read);
            });

            _responder.Test(v);
        }

        void OnTest_long_A2_A(CrossCall.IContext context)
        {
            List<FixedArray<long, ArrayLength_2>> v = new List<FixedArray<long, ArrayLength_2>>();

            Serialize.Utility.DoRead(context.Param, ref v, "v", delegate (Serialize.IReader r1, ref FixedArray<long, ArrayLength_2> v1, string n1) {
                return Serialize.Utility.DoRead(r1, ref v1, n1, Serialize.Utility.Read);
            });

            _responder.Test(v);
        }

        void OnTest_float_A_A2(CrossCall.IContext context)
        {
            FixedArray<List<float>, ArrayLength_2> v = new FixedArray<List<float>, ArrayLength_2>();

            Serialize.Utility.DoRead(context.Param, ref v, "v", delegate (Serialize.IReader r1, ref List<float> v1, string n1) {
                return Serialize.Utility.DoRead(r1, ref v1, n1, Serialize.Utility.Read);
            });

            _responder.Test(v);
        }

        void OnTest(CrossCall.IContext context)
        {


            _responder.Test();
        }

        void OnTest_Msg(CrossCall.IContext context)
        {
            Msg msg = new Msg();

            Serialize.Utility.Read(context.Param, ref msg, "msg");

            _responder.Test(msg);
        }

        void OnTest_Msg_A2(CrossCall.IContext context)
        {
            FixedArray<Msg, ArrayLength_2> msg = new FixedArray<Msg, ArrayLength_2>();

            Serialize.Utility.DoRead(context.Param, ref msg, "msg", Serialize.Utility.Read);

            _responder.Test(msg);
        }

        void OnTest_int_int(CrossCall.IContext context)
        {
            int a = 0;
            int b = 0;

            Serialize.Utility.Read(context.Param, ref a, "a");
            Serialize.Utility.Read(context.Param, ref b, "b");

            var __ret__ = _responder.Test(a, b);
            Serialize.Utility.Write(context.Ret(), __ret__);
        }

        void OnTest_int_int_int(CrossCall.IContext context)
        {
            int a = 0;
            int b = 0;
            int c = 0;

            Serialize.Utility.Read(context.Param, ref a, "a");
            Serialize.Utility.Read(context.Param, ref b, "b");
            Serialize.Utility.Read(context.Param, ref c, "c");

            var __ret__ = _responder.Test(a, b, c);
            Serialize.Utility.DoWrite(context.Ret(), __ret__, "", Serialize.Utility.Write);
        }

        void OnTest_int_int_int_int(CrossCall.IContext context)
        {
            int a = 0;
            int b = 0;
            int c = 0;
            int d = 0;

            Serialize.Utility.Read(context.Param, ref a, "a");
            Serialize.Utility.Read(context.Param, ref b, "b");
            Serialize.Utility.Read(context.Param, ref c, "c");
            Serialize.Utility.Read(context.Param, ref d, "d");

            var __ret__ = _responder.Test(a, b, c, d);
            Serialize.Utility.DoWrite(context.Ret(), __ret__, "", Serialize.Utility.Write);
        }
    }
}
