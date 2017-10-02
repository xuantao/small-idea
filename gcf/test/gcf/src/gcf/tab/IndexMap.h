#pragma once
#include <vector>

namespace tab
{
    /*
     * 数据编号映射
     * 解析器的表头到tab表中的文件头的映射关卡
     * TODO: 命名
    */
    class IndexMap
    {
    public:
        bool Setup(const char* const titles[], const char* line);

        // 预期表长度
        int Size() const;

        // 有效字段数
        int Available() const { return _available; }

        int TitleSize() const { return _titleSize; }

        // 索引映射
        int Index(int idx) const;

    protected:
        int _titleSize = 0;
        int _available = 0;
        std::vector<int> _idxMap;
    };
}
