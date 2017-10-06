/*
 * Cross Call
 */
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace CrossCall
{
    public abstract class Station
    {
        class Invok : IInvoker
        {
            Station _station;

            public Invok(Station station)
            { _station = station; }
            public Serialize.IWriter Begin(int module)
            { return _station.BeginCall(module); }
            public Serialize.IReader End()
            { return _station.EndCall(); }
        }

        class Context : IContext
        {
            Station _station;

            public Context(Station station)
            { _station = station; }
            public Serialize.IReader Param
            { get { return _station._buffer.Reader; } }
            public Serialize.IWriter Ret()
            { return _station.RetParam(); }
        }

        protected IntPtr _ptr = IntPtr.Zero;
        protected int _size;

        Invok _invoker;
        Context _param;
        SwapBuffer _buffer;
        Dictionary<int, IProcessor> _dicProc = new Dictionary<int, IProcessor>();

        public IInvoker Invoker { get { return _invoker; } }

        public int BufferSize { get { return _size; } }

        protected bool Init(int size)
        {
            _size = size;
            _ptr = Marshal.AllocHGlobal(size);
            if (_ptr == IntPtr.Zero)
                return false;

            _invoker = new Invok(this);
            _param = new Context(this);
            _buffer = new SwapBuffer(_ptr, size);

            return true;
        }

        protected void UnInit()
        {
            _dicProc.Clear();
            _buffer = null;
            _param = null;
            _invoker = null;
            Marshal.FreeHGlobal(_ptr);
            _ptr = IntPtr.Zero;
            _size = 0;
        }

        public bool Register(IProcessor processor)
        {
            if (processor == null)
                return false;

            if (_dicProc.ContainsKey(processor.ID))
                return false;

            _dicProc.Add(processor.ID, processor);
            return true;
        }

        public IProcessor GetProcessor(int module)
        {
            IProcessor proc = null;
            _dicProc.TryGetValue(module, out proc);
            return proc;
        }

        public IProcessor Unregister(int module)
        {
            IProcessor proc = null;
            if (_dicProc.TryGetValue(module, out proc))
                _dicProc.Remove(module);
            return proc;
        }

        protected abstract bool DoCall();

        protected bool OnCall()
        {
            DoRecv();

            int module = 0;
            IProcessor proc = null;
            if (_buffer.Reader.Read(ref module, null) && _dicProc.TryGetValue(module, out proc))
            {
                proc.Process(_param);
            }
            else
            {
                // error
            }

            DoSend();
            return true;
        }

        Serialize.IWriter BeginCall(int module)
        {
            _buffer.Startup(BufferMode.Write);
            _buffer.Writer.Write(module);
            return _buffer.Writer;
        }

        Serialize.IReader EndCall()
        {
            DoSend();
            DoCall();
            DoRecv();
            return _buffer.Reader;
        }

        Serialize.IWriter RetParam()
        {
            _buffer.Startup(BufferMode.Write);
            return _buffer.Writer;
        }

        void DoRecv()
        {
            _buffer.Startup(BufferMode.Read);
        }

        void DoSend()
        {
            _buffer.Endup();
        }
    }
}
