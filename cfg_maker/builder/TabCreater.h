#pragma once
#include "Interface.h"
#include <sstream>

CFG_NAMESPACE_BEGIN

class TabCreater : public ITabCreater
{
public:
    TabCreater();
    virtual ~TabCreater() {}

public:
    virtual void SetPath(const std::string& path);

public:
    virtual bool OnStart(const IStructType* sType);
    virtual bool OnEnd();
    virtual bool OnVar(const IVariate* var, const IRawType* rType,
        const std::string& title, const std::string& path);
    virtual bool OnVar(const IVariate* var, const IEnumType* eType,
        const std::string& title, const std::string& path);
    virtual bool OnVar(const IVariate* var, const IRawType* rType,
        const std::string& title, const std::string& path, int length);
    virtual bool OnVar(const IVariate* var, const IEnumType* eType,
        const std::string& title, const std::string& path, int length);

protected:
    void Create();
    void Merge();

protected:
    std::string _path;
    std::string _file;
    std::vector<std::string> _title;
    std::vector<std::string> _type;
    std::vector<std::string> _desc;
    std::vector<std::string> _def;
};

CFG_NAMESPACE_END
