/*
 * 交叉调用接口定义
 */

namespace CrossCall
{
    public interface IProcessor
    {
        uint ModuleID();
        uint HashCode();
        void Process(Serialize.IReader reader, Serialize.IWriter writer);
    }

    public interface ICrossCaller
    {
        Serialize.IWriter BeginCall(uint module, uint code);
        Serialize.IReader EndCall();
    }
}
