﻿// $Id: scanner.h 17 2007-08-19 18:51:39Z tb $

#ifndef EXAMPLE_SCANNER_H
#define EXAMPLE_SCANNER_H

// Flex expects the signature of yylex to be defined in the macro YY_DECL, and
// the C++ parser expects it to be declared. We can factor both as follows.
/*
#ifndef YY_DECL

#define YY_DECL                     \
    example::Parser::token_type             \
    example::Scanner::lex(              \
    example::Parser::semantic_type* yylval,     \
    example::Parser::location_type* yylloc      \
    )
#endif
*/

#ifndef YY_DECL
#define YY_DECL int zh::Scanner::lex()
#endif

#ifndef __FLEX_LEXER_H
#define yyFlexLexer zhFlexLexer
#include "FlexLexer.h"
#undef yyFlexLexer
#endif

namespace zh
{
    /** Scanner is a derived class to add some extra function to the scanner
     * class. Flex itself creates a class named yyFlexLexer, which is renamed using
     * macros to ExampleFlexLexer. However we change the context of the generated
     * yylex() function to be contained within the Scanner class. This is required
     * because the yylex() defined in ExampleFlexLexer has no parameters. */
    class Scanner : public zhFlexLexer
    {
    public:
        /** Create a new scanner object. The streams arg_yyin and arg_yyout default
         * to cin and cout, but that assignment is only made when initializing in
         * yylex(). */
        Scanner(std::istream* arg_yyin = 0,
            std::ostream* arg_yyout = 0);

        /** Required for virtual functions */
        virtual ~Scanner();

        virtual int lex();

        /** Enable debug output (via arg_yyout) if compiled into the scanner. */
        void set_debug(bool b);
    protected:
        void Include(const char* pFile, bool bRelative);
    };

} // namespace example

#endif // EXAMPLE_SCANNER_H
