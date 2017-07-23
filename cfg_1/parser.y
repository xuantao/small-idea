/* configuration file parser and generator*/

%skeleton "lalr1.cc"
%require "3.0"
%defines
%define parser_class_name { Parser }

%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define api.namespace { Cfg }

%code requires
{
#include <string>
#include "CfgDef.h"

CFG_NAMESPACE_BEGIN
class Driver;
CFG_NAMESPACE_END

//#define YYSTYPE std::string
}

//%lex-param {Driver& driver}
%parse-param {Driver& driver}

%locations
%initial-action
{
    @$.begin.filename = @$.end.filename = &driver.file;
}

%define parse.trace
%define parse.error verbose

%code
{
#include "Driver.h"
#include "Scanner.h"

static Parser::symbol_type yylex(Driver& drive)
{
    return driver.GetScanner()->Lex();
}

}

%define api.token.prefix {TOK_}

/* assii symbols */
%token
    END             0       "end of file"
    S_LPAREN        40      "("
    S_RPAREN        41      ")"
    S_STAR          42      "*"
    S_PLUS          43      "+"
    S_COMMA         44      ","
    S_MINUS         45      "-"
    S_DOT           46      "."
    S_SLASH         47      "/"
    S_COLON         58      ":"
    S_SEMICOLON     59      ";"
    S_ASSIGN        61      "="
    S_LBRACK        91      "["
    S_RBRACK        93      "]"
    S_LBRACE        123     "{"
    S_RBRACE        125     "}"
    ;

/* key words */
%token ENUM STRUCT BOOL INT FLOAT STRING

/* value */
%token IDENTIFIER VALUE_TRUE VALUE_FALSE VALUE_INT VALUE_FLOAT VALUE_STRING

/*
%token <std::string> VALUE_TRUE     "true"
%token <std::string> VALUE_FALSE    "false"
%token <std::string> VALUE_INT      "0"
%token <std::string> VALUE_FLOAT    "0.0f"
%token <std::string> VALUE_STRING   "empty"
%token <std::string> IDENTIFIER     "identifier"
*/

/* %type <int> exp */

//%printer { yyoutput << $$; }

%%
%start Start;
Start   : /* empty*/        { }
        ;
%%

void Cfg::Parser::error(const location_type& l, const std::string& m)
{
    driver.Error(l, m);
}
