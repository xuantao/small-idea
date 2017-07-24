/* for test c++ scanner */

%top{
#include <iostream>
#include <string>
#define YY_NO_UNISTD_H
}

/* code define */
%{
#include "Scanner.h"
#include "Parser.hpp"
#include "Driver.h"

CFG_NAMESPACE_USING;

// declearation lex function
#define YY_DECL Parser::symbol_type cfg::Scanner::Lex()
#define YY_USER_ACTION { Location().columns(yyleng); }
#define yyterminate() Parser::make_END(Location())
#define _NEW_LINE_ { Location().lines(yyleng); Location().step(); }
%}

/* flex options */
%option c++

/* zhFlexLexer */
%option prefix="Cfg"

/* optimize */
%option batch
%option debug
%option stack
%option yylineno
%option noyywrap
%option outfile="CfgFlexLexer.cpp"

%x INCL
%x COMMENT
%x COMMENT_LINES
%x COPY_SRC

REG_INCLUDE             ([ \t]*#[ \t]*include)
REG_INCLUDE_FILE_1      (\"[ /._//a-zA-Z0-9]+\")
REG_INCLUDE_FILE_2      (<[ /._//a-zA-Z0-9]+>)

REG_INTEGER             ([0-9]+)
REG_FLOAT               ([0-9]*\.?[0-9]+[fF]?)
REG_STRING              (\"[^\"\n]*\")
REG_OPERATOR            ([-/+*()=,:;{}\[\]\.])
REG_IDENTIFIER          ([_a-zA-Z][_a-zA-Z0-9]*)

%%

 /* code to place at the beginning of yylex() */
%{
    // reset location every token lex
    Location().step();
%}

 /* copy source text copy to dest file */
^[ \t]*"%{"     { yy_push_state(COPY_SRC); }
<COPY_SRC>{
        "%}"    { yy_pop_state(); }
        \n      { _NEW_LINE_; }
        .+      { }
        <<EOF>> { m_driver.Error(Location(), "uncomplete in src copy"); return yyterminate(); }
}

 /* include files */
^{REG_INCLUDE}              { yy_push_state(INCL); }
<INCL>{
    {REG_INCLUDE_FILE_1}    { yy_pop_state(); Include(std::string(&yytext[1], yyleng - 2)); }
    {REG_INCLUDE_FILE_2}    { yy_pop_state(); Include(std::string(&yytext[1], yyleng - 2)); }
    [ \t]+                  { }
    \n                      { _NEW_LINE_; yy_pop_state(); m_driver.Error(Location(), "error #"); }
    .                       { yy_pop_state(); Unrecognized(yytext[0]); }
}

 /* single line comment */
[//+]               { yy_push_state(COMMENT); }
<COMMENT>{
    \n              { _NEW_LINE_; yy_pop_state(); }
    .               { }
}

 /* lines comments */
"/*"                { yy_push_state(COMMENT_LINES); }
<COMMENT_LINES>{
    "*/"            { yy_pop_state(); }
    \n              { _NEW_LINE_; }
    .               { }
    <<EOF>>         { m_driver.Error(Location(), "uncomplete in lines comment"); return yyterminate(); }
}

 /* main */
"enum"          { return Parser::make_ENUM(Location()); }
"struct"        { return Parser::make_STRUCT(Location()); }
"const"         { return Parser::make_CONST(Location()); }
"bool"          { return Parser::make_BOOL(Location()); }
"int"           { return Parser::make_INT(Location()); }
"float"         { return Parser::make_FLOAT(Location()); }
"string"        { return Parser::make_STRING(Location()); }
"true"          { return Parser::make_VALUE_TRUE(yytext, Location()); }
"false"         { return Parser::make_VALUE_FALSE(yytext, Location()); }
{REG_OPERATOR}  { return Parser::symbol_type((Parser::token_type)yytext[0], Location()); }
{REG_INTEGER}   { return Parser::make_VALUE_INT(yytext, Location()); }
{REG_FLOAT}     { return Parser::make_VALUE_FLOAT(yytext, Location()); }
{REG_STRING}    { return Parser::make_VALUE_STRING(yytext, Location()); }
{REG_IDENTIFIER}    { return Parser::make_IDENTIFIER(yytext, Location()); }

[ \t\r]+        { Location().step(); }
\n              { _NEW_LINE_; }
.               { Unrecognized(yytext[0]); }
<<EOF>>         { if (!EndOfFile()) return yyterminate(); }

%%

int CfgFlexLexer::yylex()
{
    std::cerr << "play call lex to scann data" << std::endl;
    return 0;
}
