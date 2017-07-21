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
    {
        class ScanningFile;
    }

    /*
     * token scanner
    */
    class Scanner : protected CfgFlexLexer
    {
    public:
        Scanner();
        virtual ~Scanner();

    public:
        bool Init(const std::string& file);
        void UnInit();

    public:
        const std::string& File() const;
        int LineNO() const;

    public:
        virtual int Lex();
        virtual int yywrap();

    protected:
        void Include(const std::string& file);
        bool EndOfFile();

    protected:
        typedef std::shared_ptr<detail::ScanningFile> FilePtr;
        typedef std::stack<FilePtr> FileStack;

        bool Push(const std::string& file);
        bool Pop();

        FileStack  m_fileStack;
    };
}
