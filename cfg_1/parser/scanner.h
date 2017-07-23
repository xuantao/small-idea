#pragma once
#include <string>
#include <memory>
#include <stack>

#define YY_BUF_SIZE 16384
#ifndef yyFlexLexer
#define yyFlexLexer CfgFlexLexer
#include "FlexLexer.h"
#endif

namespace Cfg
{
    namespace detail
    { class ScanningFile; }
    class location;
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
        virtual int Lex();

    protected:
        bool Include(const std::string& file);
        bool EndOfFile();

    protected:
        typedef std::shared_ptr<detail::ScanningFile> FilePtr;
        typedef std::stack<FilePtr> FileStack;

        bool Push(const std::string& file);
        bool Pop();

    private:
        Driver& m_driver;
        FileStack  m_fileStack;
    };
}

