#include "IndexMap.h"

namespace tab
{
    bool sEqual(const char* title, const char* src, int length)
    {
        int pos = 0;
        while (title[pos] && pos < length)
        {
            if (title[pos] != src[pos])
                break;
            ++pos;
        }

        return title[pos] == 0 && pos == length;
    }

    bool IndexMap::Setup(const char* const titles[], const char* line)
    {
        if (titles == nullptr || titles[0] == nullptr || line == nullptr)
            return false;

        int size = 0;
        while (titles[size])
            ++size;

        _idxMap.resize(size, -1);
        _titleSize = _available = 0;

        int beg = 0;
        int end = 0;
        do
        {
            end = beg;
            while (line[end] != 0 && line[end] != '\t')
                ++end;

            for (int t = 0; t < size; ++t)
            {
                if (_idxMap[t] != -1)
                    continue;

                if (!sEqual(titles[t], line + beg, end - beg))
                    continue;

                _idxMap[t] = _titleSize;
                ++_available;
            }

            ++_titleSize;
            beg = end + 1;
        } while (line[end] != 0);

        return _available != 0;
    }

    int IndexMap::Size() const
    {
        return (int)_idxMap.size();
    }

    int IndexMap::Index(int idx) const
    {
        if (idx < 0 || idx >= Size())
            return -1;

        return _idxMap[idx];
    }
}
