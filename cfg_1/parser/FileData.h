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

class FileData : public IFileData
{
public:
    FileData(const std::string& file);
    virtual ~FileData();

public:
    virtual const std::string& File() const { return _file; }
    virtual void Traverse(IExporter* visitor) const;

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
