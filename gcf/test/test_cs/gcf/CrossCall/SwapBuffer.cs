using System;

namespace CrossCall
{
    public enum BufferMode
    {
        Read,
        Write,
    }

    /*
     * 数据交换缓存
     * 缓冲区的首4字节为数据块长度,数据的读写都是从第5字节开始
    */
    public class SwapBuffer : IBinaryStream
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

            //ProfileManager.Instance.Start("write bytes");

            Array.Copy(data, 0, _pool, _wp, data.Length);
            _wp += data.Length;

            //ProfileManager.Instance.Stop("write bytes");
            return true;
        }
    }
}
