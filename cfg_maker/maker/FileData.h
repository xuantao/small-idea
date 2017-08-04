/*
 * 解析的文件数据
*/
#pragma once
#include "Interface.h"

CFG_NAMESPACE_BEGIN

namespace detail
{
    class FileBlock;
}

class FileData
{
public:
    FileData(const std::string& file);
    virtual ~FileData();

public:
    const std::string& File() const { return _file; }
    void Export(IExporter* visitor, bool merge) const;

public:
    void Add(const IType* type);
    void Add(const IVariate* var);

    void Inlcude(const std::string& file);

    void NamesapceBegin(const std::string& name);
    void NamesapceEnd();

protected:
    std::string _file;
    std::vector<detail::FileBlock*> _blocks;
};
CFG_NAMESPACE_END
