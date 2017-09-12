#pragma once
#include "Interface.h"
#include <sstream>

CFG_NAMESPACE_BEGIN

class JsonCreater : public IJsonCreater
{
public:
    JsonCreater();
    virtual ~JsonCreater() {}

public:
    virtual void SetPath(const std::string& path);

public:
    virtual bool OnStart(const IStructType* sType);
    virtual bool OnEnd();

protected:
    void Create(const IStructType* sType, int tab);

protected:
    std::ostream* _stream;
    std::string _path;
    std::string _file;
};

CFG_NAMESPACE_END
