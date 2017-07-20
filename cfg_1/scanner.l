/* for test c++ scanner */

%top{
#include <stdint.h>
#include <iostream>
#include <string>
#define YY_NO_UNISTD_H
}

%{

#include "scanner.h"

int line = 0;
void unrecognized_char(char c);

%}

/* flex options */
%option c++

/* zhFlexLexer */
%option prefix="zh"

/* optimize */
%option batch
%option debug
%option stack
%option yylineno

%x INCL
%x COMMENT
%x COMMENT_LINES

FILE   ([_a-zA-Z/]\.*[_a-zA-Z0-9]*\.?[_a-zA-Z0-9])

%%
^[ \t]*#[ \t]*include         { BEGIN(INCL); }

<INCL>{
    "<"{FILE}">"  { printf("include %s\n", yytext); }
    \"{FILE}\"    { printf("include %s\n", yytext); }
    [ \r\t]             { }
    \n                  { ++line; BEGIN(INITIAL); }
    .                   { unrecognized_char(yytext[0]); }
}

 /* 多行注释 */
"/*"            { BEGIN(COMMENT_LINES); printf("多行注释开始\n"); }
<COMMENT_LINES>{
    "*/"        { BEGIN(INITIAL); printf("多行注释结束\n"); }
    \n          { ++line; }
    .           { }
    <<EOF>>     { /* error */ return 0; }
}

 /* 单行注释 */
[/]{2,}         { BEGIN(COMMENT); printf("单行注释开始\n");  }
<COMMENT>{
    \n          { BEGIN(INITIAL); printf("单行注释结束\n");  }
    .           { }
}

\n              { ++line; }
.               { }
<<EOF>>         { return 0; }
%%

void unrecognized_char(char c)
{
    char buf[32] = "Unrecognized character: }";
    buf[24] = c;
    printf("error %s\n", buf);
}

/*
namespace zh {
    Scanner::Scanner(std::istream* in, std::ostream* out)
        : zhFlexLexer(in, out)
    {
    }

    Scanner::~Scanner()
    {
    }

    void Scanner::set_debug(bool b)
    {
        yy_flex_debug = b;
    }
}
*/
int zhFlexLexer::yylex()
{
    return 0;
}
int zhFlexLexer::yywrap()
{
    return 1;
}
