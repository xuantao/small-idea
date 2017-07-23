/* for test c++ scanner */

%top{
#include <iostream>
#include <string>
#define YY_NO_UNISTD_H

}

/* code define */
%{
#include "Scanner.h"
#include "Parser.h"
#include "Driver.h"

CFG_NAMESPACE_USING;

// declearation lex function
#define YY_DECL Parser::semantic_type Cfg::Scanner::Lex()
#define YY_USER_ACTION Location().columns(yyleng);
#define UPDDATE_LINENO Location().lines(yylength); Location().step()
}

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
REG_IDENTIFIER          ([_a-zA-Z][_a-zA-Z0-9]*)

%%
 /* code to place at the beginning of yylex() */
%{
    Location()->step(); // reset location
%}

 /* copy source text copy to dest file */
^[ \t]*"%{"             { yy_push_state(COPY_SRC); }
<COPY_SRC>{
        ^[ \t]*"%}"     { yy_pop_state(); }
        \n              { UPDATE_LINENO; printf("\n"); }
        .+              { printf("%s", yytext); }
}

 /* include files */
^{REG_INCLUDE}          { yy_push_state(INCL); }
<INCL>{
    {REG_INCLUDE_FILE_1}    { yy_pop_state(); Include(std::string(&yytext[1], yyleng - 2)); }
    {REG_INCLUDE_FILE_2}    { yy_pop_state(); Include(std::string(&yytext[1], yyleng - 2)); }
    [ \t]+                  { }
    \n                      { UPDATE_LINENO; yy_pop_state(); printf("scan include reg failed\n"); }
    .                       { yy_pop_state(); printf("scan include reg failed\n"); }
}

 /* single line comment */
[//+]                   { yy_push_state(COMMENT); }
<COMMENT>{
    \n                  { UPDATE_LINENO; yy_pop_state(); }
    .                   { }
}

 /* lines comments */
"/*"                    { yy_push_state(COMMENT_LINES); }
<COMMENT_LINES>{
    "*/"                { yy_pop_state(); }
    \n                  { UPDATE_LINENO; }
    .                   { }
    <<EOF>>             { printf("comment lines error\n"); yyterminate(); }
}

[ \t\r]+        { Location().step(); }
\n              { UPDATE_LINENO; }
.               { printf("%s", yytext); }
<<EOF>>         { if (!EndOfFile()) yyterminate(); }
%%

int CfgFlexLexer::yylex()
{
    std::cerr << "play call lex to scann data" << std::endl;
    return 0;
}

