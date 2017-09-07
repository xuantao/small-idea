/*
 * 交叉调用接口定义
 */

namespace CrossCall
{
    public interface IContext
    {
        Serialize.IReader Param { get; }
        Serialize.IWriter Ret();
    }

    public interface ICaller
    {
        Serialize.IWriter BeginCall(uint module);
        Serialize.IReader EndCall();
    }

    public interface IProcessor
    {
        void Process(IContext context);
    }

    public interface InternalCall
    {
        void DoCall();
    }
}
