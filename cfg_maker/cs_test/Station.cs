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
        protected class CrossCaller : ICaller
        {
            Station _station;

            public CrossCaller(Station station)
            { _station = station; }
            public Serialize.IWriter BeginCall(uint module)
            { return _station.BeginCall(module); }
            public Serialize.IReader EndCall()
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

        public enum BufferMode
        {
            Read,
            Write,
        }

        protected class SwapBuffer : Serialize.IBinaryStream
        {
            BufferMode _mode;
            int _rp;        // read pos
            int _ds;        // data size
            int _wp;        // write pos

            byte[] _pool;

            public SwapBuffer(byte[] pool)
            { _pool = pool; }

            public BufferMode Mode { get { return _mode; } }

            public void Startup(BufferMode mode, int size)
            {
                _mode = mode;
                _rp = _wp = 4;
                _ds = size;
            }

            public int Endup()
            {
                int size = 0;
                if (_mode == BufferMode.Write)
                {
                    size = _wp;
                    Array.Copy(BitConverter.GetBytes(size), _pool, 4);
                }

                _rp = _wp = 4;
                _ds = 0;
                return size;
            }

            public bool Read(byte[] data)
            {
                if (_mode != BufferMode.Read || _rp + data.Length > _ds)
                    return false;

                Array.Copy(_pool, _rp, data, 0, data.Length);
                _rp += data.Length;
                return true;
            }

            public bool Write(byte[] data)
            {
                if (_mode != BufferMode.Write || _wp + data.Length > _pool.Length)
                    return false;

                Array.Copy(data, 0, _pool, _wp, data.Length);
                _wp += data.Length;
                return true;
            }
        }

        InternalCall _internal;
        IntPtr _ptr;
        int _size;

        byte[] _pool;
        CrossCaller _caller;
        Context _param;

        SwapBuffer _buffer;
        Serialize.BinaryReader _reader;
        Serialize.BinaryWriter _writer;

        Dictionary<uint, IProcessor> _dicProc = new Dictionary<uint, IProcessor>();

        public Station(InternalCall inter, IntPtr ptr, int size)
        {
            _internal = inter;
            _ptr = ptr;
            _size = size;

            _pool = new byte[size];
            _caller = new CrossCaller(this);
            _param = new Context(this);
            _buffer = new SwapBuffer(_pool);
            _reader = new Serialize.BinaryReader(_buffer);
            _writer = new Serialize.BinaryWriter(_buffer);
        }

        public ICaller Caller { get { return _caller; } }

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

        protected Serialize.IWriter BeginCall(uint module)
        {
            _buffer.Startup(BufferMode.Write, 0);
            _writer.Write(module);
            return _writer;
        }

        protected Serialize.IReader EndCall()
        {
            DoSend();
            _internal.DoCall();
            DoRecv();
            return _reader;
        }

        protected Serialize.IReader BeginParam()
        {
            return _reader;
        }

        void EndParam()
        { }

        Serialize.IWriter RetParam()
        {
            _buffer.Startup(BufferMode.Write, 0);
            return _writer;
        }

        protected void DoRecv()
        {
            int size = Marshal.ReadInt32(_ptr, 0);
            Marshal.Copy(_ptr, _pool, 0, size);
            _buffer.Startup(BufferMode.Read, size);
        }

        protected void DoSend()
        {
            int size = _buffer.Endup();
            if (_buffer.Mode == BufferMode.Write)
                Marshal.Copy(_pool, 0, _ptr, size);
            else
                Marshal.WriteInt32(_ptr, 0);    // clear
        }
    }
}
