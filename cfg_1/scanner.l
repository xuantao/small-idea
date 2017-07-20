/* for test c++ scanner */

%{ /* top code define */

#include <iostream>
#include <string>

#define YY_NO_UNISTD_H

int line = 0;
void unrecognized_char(char c)

%}

/* flex options */
%option c++

/* zhFlexLexer */
%option prefix="zh"

/* optimize */
%option batch
%option debug
%option stack

%x INCL

IDENTIFIER   ([_a-zA-Z][_a-zA-Z0-9]*)

%%
include         { BEGIN(INCL); }

<INCL>{
    "<"{IDENTIFIER}">"  { printf("include %s\n", yytext); }
    \"{IDENTIFIER}\"    { printf("include %s\n", yytext); }
    [ \r\t]             { }
    \n                  { ++line; BEGIN(INITIAL);  printf("Start include file\n"); }
    .                   { unrecognized_char(yytext[0]); }
}

\n              { ++ line; }
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
int zhFlexLexer::yywrap()
{
    return 1;
}
