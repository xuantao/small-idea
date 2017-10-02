namespace Tab
{
    public interface ITokenStream
    {
        string Pop();

        bool Push(string token);
    }
}
