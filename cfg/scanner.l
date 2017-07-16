%{
#define YYSTYPE char*
#include "y.tab.h"
int line = 1;
void yyerror(const char* msg);
void unrecognized_char(char c);
#define _DUPTEXT { yylval = strdup(yytext); }
%}

/* note \042 is '"' */

OPERATOR     ([-/+*()=,;{}])
INTEGER      ([0-9]+)
STRING       (\042[^\042\n]*\042)
IDENTIFIER   ([_a-zA-Z][_a-zA-Z0-9]*)
WHITESPACE   ([ \t]*)

%%
{OPERATOR}    { return yytext[0]; }
"enum"        { return T_ENUM;    }
"struct"      { return T_STRUCT;  }
"int"         { return T_INT;     }
"string"      { return T_STRING;  }
"bool"        { return T_BOOL;    }
"float"       { return T_FLOAT;   }
"true"        { return T_CONSTANT_TRUE;   }
"false"       { return T_CONSTANT_FALSE;  }

{INTEGER}     { _DUPTEXT; return T_CONSTANT_INT;       }
{STRING}      { _DUPTEXT; return T_CONSTANT_STRING;    }
{IDENTIFIER}  { _DUPTEXT; return T_IDENTIFIER;         }

{WHITESPACE}  { /* ignore every white space */         }
\n            { ++ line; }
.             { unrecognized_char(yytext[0]);          }
%%

int yywrap(void)
{
    return 1;
}

void unrecognized_char(char c)
{
    char buf[32] = "Unrecognized character: }";
    buf[24] = c;
    yyerror(buf);
}

void yyerror(const char* msg)
{
    printf("error at line:%d\n\t %s\n", line, msg);
    exit(-1);
}

