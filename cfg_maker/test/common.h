#pragma once
#include <string>

class Serializer
{
public:
    virtual ~Serializer() {}

public:
    virtual bool ReadStructBegin(int hashID) = 0;
    virtual bool ReadStructEnd() = 0;
    virtual bool Read(int& n) = 0;
    virtual bool Read(float& f) = 0;
    virtual bool Read(bool& b) = 0;
    virtual bool Read(std::string& s) = 0;

    virtual bool WriteStructBegin(int hashID) = 0;
    virtual bool WriteStructEnd() = 0;
    virtual bool Write(int n) = 0;
    virtual bool Write(float f) = 0;
    virtual bool Write(bool b) = 0;
    virtual bool Write(const std::string& s) = 0;
};

class Exchanger
{
public:
    virtual ~Exchanger() {}

public:
    virtual void Call(int module, int protocol) = 0;
};
