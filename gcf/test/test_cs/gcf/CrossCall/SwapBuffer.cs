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
    public unsafe class SwapBuffer
    {
        BufferMode _mode;

        System.IO.UnmanagedMemoryStream _readStream;
        System.IO.UnmanagedMemoryStream _writeStream;

        BinaryReader _reader;
        BinaryWriter _writer;

        public SwapBuffer(IntPtr ptr, int size)
        {
            _readStream = new System.IO.UnmanagedMemoryStream((byte*)ptr.ToPointer(), size, size, System.IO.FileAccess.Read);
            _writeStream = new System.IO.UnmanagedMemoryStream((byte*)ptr.ToPointer(), size, size, System.IO.FileAccess.Write);

            _reader = new BinaryReader(new System.IO.BinaryReader(_readStream));
            _writer = new BinaryWriter(new System.IO.BinaryWriter(_writeStream));
        }

        public BufferMode Mode { get { return _mode; } }
        public Serialize.IReader Reader { get { return _reader; } }
        public Serialize.IWriter Writer { get { return _writer; } }

        public void Startup(BufferMode mode)
        {
            _mode = mode;
            if (mode == BufferMode.Read)
            {
                int length = 0;
                _readStream.Seek(0, System.IO.SeekOrigin.Begin);
                _reader.Read(ref length);

                /*
                 * skip the first length data
                 * do has some way to set reader data length limit?
                */
                _writeStream.Seek(0, System.IO.SeekOrigin.End);
            }
            else
            {
                _writeStream.Seek(0, System.IO.SeekOrigin.Begin);
                _readStream.Seek(0, System.IO.SeekOrigin.End);
                // prepare read size
                _writer.Write((int)4);
            }
        }

        public int Endup()
        {
            long size = 0;
            if (_mode == BufferMode.Read)
            {
                size = _readStream.Seek(0, System.IO.SeekOrigin.Current);
            }
            else
            {
                size = _writeStream.Seek(0, System.IO.SeekOrigin.Current);
                _writeStream.Seek(0, System.IO.SeekOrigin.Begin);
                _writer.Write((int)size);
                _writeStream.Seek(0, System.IO.SeekOrigin.End);
            }
            return (int)size;
        }
    }
}
