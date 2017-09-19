/*
 * this file is auto generated.
 * please does not edit it manual!
*/
using System;
using System.Collections;
using System.Collections.Generic;

public class Caller
{
    public const int MODULE_ID = 1;
    public const int HASH_CODE = -1871653667;

    public enum Message
    {
        Invalid = 0,
        Call_A_int_int,
        Call_B_string,
    }

    public interface IResponder
    {
        void Call_A(int a, int b);
        int Call_B(string str);
    };

    public class Requester
    {
        protected CrossCall.IInvoker _invoker = null;

        public Requester(CrossCall.IInvoker invoker)
        {
            _invoker = invoker;
        }

        public void Call_A(int a, int b)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Call_A_int_int);

            Serialize.Utility.Write(writer, a);
            Serialize.Utility.Write(writer, b);

            _invoker.End();
        }

        public int Call_B(string str)
        {
            Serialize.IWriter writer = _invoker.Begin(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Call_B_string);

            Serialize.Utility.Write(writer, str);

            int __ret__ = 0;
            Serialize.Utility.Read(_invoker.End(), ref __ret__);
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
            case Message.Call_A_int_int:
                OnCall_A_int_int(context);
                break;
            case Message.Call_B_string:
                OnCall_B_string(context);
                break;
            default:
                break;
            }
        }

        void OnCall_A_int_int(CrossCall.IContext context)
        {
            int a = 0;
            int b = 0;

            Serialize.Utility.Read(context.Param, ref a);
            Serialize.Utility.Read(context.Param, ref b);

            _responder.Call_A(a, b);
        }

        void OnCall_B_string(CrossCall.IContext context)
        {
            string str = "";

            Serialize.Utility.Read(context.Param, ref str);

            var __ret__ = _responder.Call_B(str);
            Serialize.Utility.Write(context.Ret(), __ret__);
        }
    }
}
