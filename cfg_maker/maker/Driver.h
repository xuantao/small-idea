/*
 * cfg file parser driver
 * 2017-07-22
*/
#pragma once

#include <string>
#include <iostream>
#include <type_traits>
#include <regex>
#include "TupleLogger.h"
#include "Scanner.h"

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
    Scanner* GetScanner() const { return m_pScanner; }
    Parser* GetParser() const { return m_pParser; }
    Context* GetContext() const { return m_pContext; }

public:
    template <class... Types>
    void Warning(const char* format, Types&&... args)
    {
        std::cout << "[WAR]-<" << GetScanner()->Location() << ">:";
        Log(std::cout, format, std::forward<Types>(args)...);
    }

    template <class... Types>
    void Error(const char* format, Types&&... args)
    {
        std::cerr << "[ERR]-<" << GetScanner()->Location() << ">:";
        Log(std::cerr, format, std::forward<Types>(args)...);
    }

    void Error(const location& loc, const std::string& msg);

protected:
    template <class... Types>
    void Log(std::ostream& out, const char* format, Types&&... args)
    {
        typedef detail::TupleLogger<sizeof...(Types)> Logger;
        std::tuple<Types...> tp(args...);
        std::regex reg("\\{\\d+\\}");
        std::cmatch cm;
        std::ptrdiff_t pos = 0;

        while (std::regex_search(&format[pos], cm, reg))
        {
            int idx = 0;
            std::cout.write(format + pos, cm.position());
            pos += cm.position();
            idx = atoi(format + pos + 1);

            if (idx >= Logger::C)
            {
                out << "(out range at pos:" << pos << ", ";
                out.write(format + pos, cm.length());
                out << ")";
            }
            else
            {
                Logger::log(out, idx, tp);
            }

            pos += cm.length();
        }

        out << format + pos << std::endl;
    }

protected:
    Scanner*    m_pScanner;
    Parser*     m_pParser;
    Context*    m_pContext;
};

CFG_NAMESPACE_END
