#pragma once
#include "gcf/gcf.h"

GCF_NAMESPACE_BEGIN

class ElementLocation : public ILocation
{
public:
    ElementLocation(const char* file, int line)
        : _file(file)
        , _line(line)
    {}

    ~ElementLocation() { }

public:
    const char* File() const override { return _file; }
    int Line() const override { return _line; }

private:
    const char* _file;
    int _line;
};

GCF_NAMESPACE_END
