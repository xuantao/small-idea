/*
 * 解析的文件数据
*/
#pragma once
#include "Interface.h"

CFG_NAMESPACE_BEGIN

class FileData : public IFileData
{
public:
    FileData(const std::string& file);
    virtual ~FileData();

public:
    virtual const std::string& File() const { return _file; }
    virtual int Size() const { return (int)_blocks.size(); }
    virtual IBlock* Get(int index) const;
    virtual void Traverse(IFileVisitor* visitor) const;

public:
    void Add(const IType* type);
    void Add(const IVariate* var);

protected:
    std::string _file;
    std::vector<IBlock*> _blocks;
};
CFG_NAMESPACE_END
