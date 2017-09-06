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
        protected CrossCall.ICrossCaller _caller = null;

        public Invoker(CrossCall.ICrossCaller caller)
        {
            _caller = caller;
        }

        void Call_A(int a, int b)
        {
            Serialize.IWriter writer = _caller.BeginCall(MODULE_ID, HASH_CODE);
            writer.Write((int)Message.Msg_Call_A);

            Serialize.Utility.Write(writer, a);
            Serialize.Utility.Write(writer, b);

            _caller.EndCall();
        }

        int Call_B(string str)
        {
            int ret = 0;
            Serialize.IWriter writer = _caller.BeginCall(MODULE_ID, HASH_CODE);
            writer.Write((int)Message.Msg_Call_B);

            Serialize.Utility.Write(writer, str);

            Serialize.IReader reader = _caller.EndCall();
            Serialize.Utility.Read(reader, ref ret);
            return ret;
        }
    }

    public class Processor : CrossCall.IProcessor
    {
        protected IExecutor _executor = null;

        public Processor(IExecutor executor)
        {
            _executor = executor;
        }

        public uint ModuleID() { return MODULE_ID; }
        public uint HashCode() { return HASH_CODE; }

        public void Process(Serialize.IReader reader, Serialize.IWriter writer)
        {
            int tmp = 0;
            reader.Read(ref tmp);
            Message msg = (Message)tmp;

            switch (msg)
            {
            case Message.Msg_Call_A: OnCall_A(reader, writer); break;
            case Message.Msg_Call_B: OnCall_B(reader, writer); break;
            default: break;
            }
        }

        void OnCall_A(Serialize.IReader reader, Serialize.IWriter writer)
        {
            int a = 0;
            int b = 0;

            Serialize.Utility.Read(reader, ref a);
            Serialize.Utility.Read(reader, ref b);

            _executor.Call_A(a, b);
        }

        void OnCall_B(Serialize.IReader reader, Serialize.IWriter writer)
        {
            string str = "";

            Serialize.Utility.Read(reader, ref str);

            Serialize.Utility.Write(writer, _executor.Call_B(str));
        }
    }
}
