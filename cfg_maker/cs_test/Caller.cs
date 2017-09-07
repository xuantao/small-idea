/*
 * this file is auto generated.
 * please does not edit it manual!
*/
using System;
using System.Collections;
using System.Collections.Generic;

public class Caller
{
    public const uint MODULE_ID = 1;
    public const uint HASH_CODE = 456984651;

    public enum Message
    {
        Invalid = 0,
        Msg_Call_A,
        Msg_Call_B,
    }

    public interface IExecutor
    {
        void Call_A(int a, int b);
        int Call_B(string str);
    };

    public class Invoker
    {
        protected CrossCall.ICaller _caller = null;

        public Invoker(CrossCall.ICaller caller)
        {
            _caller = caller;
        }

        public void Call_A(int a, int b)
        {
            Serialize.IWriter writer = _caller.BeginCall(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Msg_Call_A);

            Serialize.Utility.Write(writer, a);
            Serialize.Utility.Write(writer, b);

            _caller.EndCall();
        }

        public int Call_B(string str)
        {
            Serialize.IWriter writer = _caller.BeginCall(MODULE_ID);
            writer.Write(HASH_CODE);
            writer.Write((int)Message.Msg_Call_B);

            Serialize.Utility.Write(writer, str);

            int __ret__ = 0;
            Serialize.IReader reader = _caller.EndCall();
            Serialize.Utility.Read(reader, ref __ret__);
            return __ret__;
        }
    }

    public class Processor : CrossCall.IProcessor
    {
        protected IExecutor _executor = null;

        public Processor(IExecutor executor)
        {
            _executor = executor;
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
            case Message.Msg_Call_A: OnCall_A(context); break;
            case Message.Msg_Call_B: OnCall_B(context); break;
            default: break;
            }
        }

        void OnCall_A(CrossCall.IContext context)
        {
            int a = 0;
            int b = 0;

            Serialize.Utility.Read(context.Param, ref a);
            Serialize.Utility.Read(context.Param, ref b);

            _executor.Call_A(a, b);
        }

        void OnCall_B(CrossCall.IContext context)
        {
            string str = "";

            Serialize.Utility.Read(context.Param, ref str);

            var __ret__ = _executor.Call_B(str);
            Serialize.Utility.Write(context.Ret(), __ret__);
        }
    }
}
