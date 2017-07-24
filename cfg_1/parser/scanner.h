#pragma once
#include <string>
#include <memory>
#include <vector>

#define YY_BUF_SIZE 16384
#ifndef yyFlexLexer
#define yyFlexLexer CfgFlexLexer
#include "FlexLexer.h"
#endif

#include "Parser.hpp"

CFG_NAMESPACE_BEGIN

namespace detail
{
    class ScanningFile;
}
class Driver;

/*
 * token scanner
*/
class Scanner : protected CfgFlexLexer
{
public:
    Scanner(Driver& driver);
    virtual ~Scanner();

public:
    bool Init(const std::string& file);
    void UnInit();

public:
    const std::string& File() const;
    location& Location();

public:
    virtual Parser::symbol_type Lex();

protected:
    bool Include(const std::string& file);
    bool EndOfFile();
    void Unrecognized(char c);

protected:
    typedef std::shared_ptr<detail::ScanningFile> FilePtr;
    typedef std::vector<FilePtr> FileStack;

    bool Push(const std::string& file);
    bool Pop();

private:
    Driver& m_driver;
    FileStack  m_fileStack;
};

CFG_NAMESPACE_END
