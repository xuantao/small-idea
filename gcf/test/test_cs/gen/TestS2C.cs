/*
 * this file is auto generated.
 * please does not edit it manual!
*/
using System;
using System.Collections;
using System.Collections.Generic;

public class TestS2C
{
    public const uint MODULE_ID = 3;
    public const uint HASH_CODE = 1180210188;

    public enum Message
    {
        Invalid = 0,
        Test_bool,
        Test_int,
        Test_float,
        Test_string,
        Test_Msg,
        CallBack,
    }

    public interface IResponder
    {
        bool Test(bool b);
        int Test(int a);
        float Test(float f);
        string Test(string s);
        Msg Test(Msg msg);
        void CallBack();
    };

    public class Requester
    {
        protected CrossCall.IInvoker _invoker = null;

        public Requester(CrossCall.IInvoker invoker)
        {
            _invoker = invoker;
        }

        public bool Test(bool b)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_bool);

            Serialize.Utility.Write(writer, b);

            bool __ret__ = false;
            Serialize.Utility.Read(_invoker.End(), ref __ret__);
            return __ret__;
        }

        public int Test(int a)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_int);

            Serialize.Utility.Write(writer, a);

            int __ret__ = 0;
            Serialize.Utility.Read(_invoker.End(), ref __ret__);
            return __ret__;
        }

        public float Test(float f)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_float);

            Serialize.Utility.Write(writer, f);

            float __ret__ = 0.0f;
            Serialize.Utility.Read(_invoker.End(), ref __ret__);
            return __ret__;
        }

        public string Test(string s)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_string);

            Serialize.Utility.Write(writer, s);

            string __ret__ = "";
            Serialize.Utility.Read(_invoker.End(), ref __ret__);
            return __ret__;
        }

        public Msg Test(Msg msg)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Test_Msg);

            Serialize.Utility.Write(writer, msg);

            Msg __ret__ = new Msg();
            Serialize.Utility.Read(_invoker.End(), ref __ret__);
            return __ret__;
        }

        public void CallBack()
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.CallBack);


            _invoker.End();
        }
    }

    public class Processor : CrossCall.IProcessor
    {
        protected IResponder _responder = null;

        public Processor(IResponder responder)
        {
            _responder = responder;
        }

        public void Process(CrossCall.IContext context)
        {
            uint code = 0;
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
            case Message.Test_int:
                OnTest_int(context);
                break;
            case Message.Test_float:
                OnTest_float(context);
                break;
            case Message.Test_string:
                OnTest_string(context);
                break;
            case Message.Test_Msg:
                OnTest_Msg(context);
                break;
            case Message.CallBack:
                OnCallBack(context);
                break;
            default:
                break;
            }
        }

        void OnTest_bool(CrossCall.IContext context)
        {
            bool b = false;

            Serialize.Utility.Read(context.Param, ref b);

            var __ret__ = _responder.Test(b);
            Serialize.Utility.Write(context.Ret(), __ret__);
        }

        void OnTest_int(CrossCall.IContext context)
        {
            int a = 0;

            Serialize.Utility.Read(context.Param, ref a);

            var __ret__ = _responder.Test(a);
            Serialize.Utility.Write(context.Ret(), __ret__);
        }

        void OnTest_float(CrossCall.IContext context)
        {
            float f = 0.0f;

            Serialize.Utility.Read(context.Param, ref f);

            var __ret__ = _responder.Test(f);
            Serialize.Utility.Write(context.Ret(), __ret__);
        }

        void OnTest_string(CrossCall.IContext context)
        {
            string s = "";

            Serialize.Utility.Read(context.Param, ref s);

            var __ret__ = _responder.Test(s);
            Serialize.Utility.Write(context.Ret(), __ret__);
        }

        void OnTest_Msg(CrossCall.IContext context)
        {
            Msg msg = new Msg();

            Serialize.Utility.Read(context.Param, ref msg);

            var __ret__ = _responder.Test(msg);
            Serialize.Utility.Write(context.Ret(), __ret__);
        }

        void OnCallBack(CrossCall.IContext context)
        {


            _responder.CallBack();
        }
    }
}
