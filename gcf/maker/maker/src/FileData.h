/*
 * 解析的文件数据
*/
#pragma once
#include "gcf/gcf.h"

GCF_NAMESPACE_BEGIN

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
    const std::string& Path() const { return _path; }
    void Export(IExporter* visitor, bool merge) const;

public:
    void Add(const IType* type);
    void Add(const IVariate* var);
    void Add(const IModule* var);

    void Inlcude(const std::string& file);

    void NsBegin(const std::string& name);
    void NsEnd();

protected:
    std::string _file;
    std::string _path;
    std::vector<detail::FileBlock*> _blocks;
};
GCF_NAMESPACE_END
