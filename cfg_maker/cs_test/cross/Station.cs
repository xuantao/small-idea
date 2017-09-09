/*
 * Cross Call
 */
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace CrossCall
{
    public class Station
    {
        protected class Invok : IInvoker
        {
            Station _station;

            public Invok(Station station)
            { _station = station; }
            public Serialize.IWriter Begin(uint module)
            { return _station.BeginCall(module); }
            public Serialize.IReader End()
            { return _station.EndCall(); }
        }

        protected class Context : IContext
        {
            Station _station;

            public Context(Station station)
            { _station = station; }
            public Serialize.IReader Param
            { get { return _station._reader; } }
            public Serialize.IWriter Ret()
            { return _station.RetParam(); }
        }

        ICrossCall _caller;
        IntPtr _ptr;
        int _size;

        byte[] _pool;
        Invok _invoker;
        Context _param;

        SwapBuffer _buffer;
        Serialize.BinaryReader _reader;
        Serialize.BinaryWriter _writer;

        Dictionary<uint, IProcessor> _dicProc = new Dictionary<uint, IProcessor>();

        public Station(ICrossCall caller, IntPtr ptr, int size)
        {
            _caller = caller;
            _ptr = ptr;
            _size = size;

            _pool = new byte[size];
            _invoker = new Invok(this);
            _param = new Context(this);
            _buffer = new SwapBuffer(_pool);
            _reader = new Serialize.BinaryReader(_buffer);
            _writer = new Serialize.BinaryWriter(_buffer);
        }

        public IInvoker Invoker { get { return _invoker; } }

        public bool Register(uint module, IProcessor processor)
        {
            if (_dicProc.ContainsKey(module))
                return false;

            _dicProc.Add(module, processor);
            return true;
        }

        public IProcessor GetProcessor(uint module)
        {
            IProcessor proc = null;
            _dicProc.TryGetValue(module, out proc);
            return proc;
        }

        public IProcessor Unregister(uint module)
        {
            IProcessor proc = null;
            if (_dicProc.TryGetValue(module, out proc))
                _dicProc.Remove(module);
            return proc;
        }

        public void OnCall()
        {
            DoRecv();

            uint module = 0;
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
        }

        Serialize.IWriter BeginCall(uint module)
        {
            _buffer.Startup(BufferMode.Write, 0);
            _writer.Write(module);
            return _writer;
        }

        Serialize.IReader EndCall()
        {
            DoSend();
            _caller.DoCall();
            DoRecv();
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
