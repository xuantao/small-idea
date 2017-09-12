/*
 * 交叉调用接口定义
 */

namespace CrossCall
{
    /*
     * 调用传递的参数和用于返回值传递
    */
    public interface IContext
    {
        Serialize.IReader Param { get; }
        Serialize.IWriter Ret();
    }

    public interface IInvoker
    {
        Serialize.IWriter Begin(uint module);
        Serialize.IReader End();
    }

    public interface IProcessor
    {
        void Process(IContext context);
    }

    public interface ICrossCall
    {
        void DoCall();
    }

    /*
     * 简单的流接口
    */
    public interface IBinaryStream
    {
        bool Read(byte[] data);
        bool Write(byte[] data);
    }
}
