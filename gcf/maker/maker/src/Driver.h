/*
 * cfg file parser driver
 * 2017-07-22
*/
#pragma once

#include <string>
#include <iostream>
#include <type_traits>
#include <regex>
#include "Scanner.h"
#include "utility/Utility.h"

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
    bool Parse(Context& context, const std::string& file);
    bool Parse(Context& context, const std::string& path, const std::vector<std::string>& files);

public:
    int ErrorNum() const { return _errorNum; }
    int WarNum() const { return _warNum; }

    Scanner* GetScanner() const { return _scanner; }
    Parser* GetParser() const { return _parser; }
    Context* GetContext() const { return _context; }

public:
    template <class... Types>
    void Warning(const char* format, Types&&... args)
    {
        std::cout << "[WAR]-<" << GetScanner()->Location() << ">:";
        utility::Log(std::cout, format, std::forward<Types>(args)...);
        ++_warNum;
    }

    template <class... Types>
    void Error(const char* format, Types&&... args)
    {
        std::cerr << "[ERR]-<" << GetScanner()->Location() << ">:";
        utility::Log(std::cerr, format, std::forward<Types>(args)...);
        ++_errorNum;
    }

    void Error(const location& loc, const std::string& msg);

protected:
    int _errorNum;
    int _warNum;
    Scanner*    _scanner;
    Parser*     _parser;
    Context*    _context;
};

CFG_NAMESPACE_END
