/*
 * 模拟C++中的Array,确保语意一致
 */

public interface IArrayLength
{
    int Length { get; }
}

/*
 * 固定长度数组
 */
public class FixedArray<T, L>
    where T : new()
    where L : IArrayLength, new()
{
    protected static L s_len = new L();

    protected T[] _data = new T[s_len.Length];

    public int Length { get { return s_len.Length; } }

    public T this[int index]
    {
        get { return _data[index]; }
        set { _data[index] = value; }
    }

    public FixedArray()
    {
        for (int i = 0; i < Length; ++i)
            _data[i] = new T();
    }

    public static implicit operator T[] (FixedArray<T, L> arr)
    {
        return arr._data;
    }
}
