#pragma once
#include <string>
#include <memory>
#include <vector>
#include <set>

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
    bool Init(const std::vector<std::string>& files);
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

    bool NextFile();
    bool Push(const std::string& file);
    bool Pop();

private:
    std::string _desc;
    Driver& _driver;
    std::vector<FilePtr>  _stack;

    std::vector<std::string> _files;
    std::string _path;
    std::set<std::string> _parsed;
};

CFG_NAMESPACE_END
