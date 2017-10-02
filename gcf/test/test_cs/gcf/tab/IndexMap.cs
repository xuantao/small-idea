namespace Tab
{
    /*
     * tab表title索引的映射关系
    */
    public class IndexMap
    {
        int[] _idxMap = null;

        public bool Setup(string[] titles, string line)
        {
            if (titles.Length == 0 || string.IsNullOrEmpty(line))
                return false;

            string[] elements = line.Split('\t');
            if (elements.Length == 0)
                return false;

            _idxMap = new int[titles.Length];
            _idxMap.SetValue(-1, 0, _idxMap.Length - 1);
            for (int titleIndex = 0; titleIndex < titles.Length; ++titleIndex)
            {
                for (int elementIndex = 0; elementIndex < elements.Length; ++elementIndex)
                {
                    if (_idxMap[elementIndex] != -1)
                        continue;

                    if (titles[titleIndex] == elements[elementIndex])
                        _idxMap[titleIndex] = elementIndex;
                }
            }
            return false;
        }

        public int Index(int idx)
        {
            if (idx < 0 || idx >= _idxMap.Length)
                return -1;
            return _idxMap[idx];
        }
    }
}
