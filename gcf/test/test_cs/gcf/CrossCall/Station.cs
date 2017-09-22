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
            { get { return _station._reader; } }
            public Serialize.IWriter Ret()
            { return _station.RetParam(); }
        }

        protected IntPtr _ptr = IntPtr.Zero;
        protected int _size;

        byte[] _pool;
        Invok _invoker;
        Context _param;

        SwapBuffer _buffer;
        BinaryReader _reader;
        BinaryWriter _writer;

        Dictionary<int, IProcessor> _dicProc = new Dictionary<int, IProcessor>();

        public IInvoker Invoker { get { return _invoker; } }

        public int BufferSize { get { return _size; } }

        protected bool Init(int size)
        {
            _size = size;
            _ptr = Marshal.AllocHGlobal(size);
            if (_ptr == IntPtr.Zero)
                return false;

            _pool = new byte[size];
            _invoker = new Invok(this);
            _param = new Context(this);
            _buffer = new SwapBuffer(_pool);
            _reader = new BinaryReader(_buffer);
            _writer = new BinaryWriter(_buffer);

            return true;
        }

        protected void UnInit()
        {
            _dicProc.Clear();
            _writer = null;
            _reader = null;
            _buffer = null;
            _param = null;
            _invoker = null;
            _pool = null;
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
            if (_reader.Read(ref module, null) && _dicProc.TryGetValue(module, out proc))
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
            _buffer.Startup(BufferMode.Write, 0);
            _writer.Write(module);
            return _writer;
        }

        Serialize.IReader EndCall()
        {
            ProfileManager.Instance.Start("do send");
            DoSend();
            ProfileManager.Instance.Stop("do send");

            ProfileManager.Instance.Start("do call");
            DoCall();
            ProfileManager.Instance.Stop("do call");

            ProfileManager.Instance.Start("do recv");
            DoRecv();
            ProfileManager.Instance.Stop("do recv");
            return _reader;
        }

        Serialize.IWriter RetParam()
        {
            _buffer.Startup(BufferMode.Write, 0);
            return _writer;
        }

        void DoRecv()
        {
            int size = Marshal.ReadInt32(_ptr, 0);
            Marshal.Copy(_ptr, _pool, 0, size);
            _buffer.Startup(BufferMode.Read, size);
        }

        void DoSend()
        {
            int size = _buffer.Endup();
            if (_buffer.Mode == BufferMode.Write)
                Marshal.Copy(_pool, 0, _ptr, size);
            else
                Marshal.WriteInt32(_ptr, 0);    // clear
        }
    }
}
