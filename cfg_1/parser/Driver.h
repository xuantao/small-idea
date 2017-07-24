/*
 * cfg file parser driver
 * 2017-07-22
*/
#pragma once

#include <string>
#include "CfgDef.h"

CFG_NAMESPACE_BEGIN

class Context;
class Scanner;
class Parser;
class location;

/*
 * cfg file parser driver
*/
class Driver
{
public:
    Driver();
    ~Driver();

public:
    bool Parse(const std::string& file, Context& context);

public:
    Scanner* GetScanner() const { return m_pScanner; }
    Parser* GetParser() const { return m_pParser; }
    Context* GetContext() const { return m_pContext; }

    void Error(const location& loc, const std::string& msg);

protected:
    Scanner*    m_pScanner;
    Parser*     m_pParser;
    Context*    m_pContext;
};

CFG_NAMESPACE_END
