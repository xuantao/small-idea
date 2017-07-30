#pragma once
#include "Interface.h"
#include <sstream>

CFG_NAMESPACE_BEGIN

class TabCreator : public IVarVistor
{
public:
    TabCreator(const std::string& file);
    virtual ~TabCreator() {}

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
    void CheckFirst();

protected:
    bool _first;
    std::string _file;
    std::stringstream _title;
    std::stringstream _type;
    std::stringstream _desc;
    std::stringstream _default;
};

CFG_NAMESPACE_END
