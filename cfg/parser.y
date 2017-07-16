%{
#include <stdio.h>
#include <stdlib.h>
void yyerror(const char*);
#define YYSTYPE char*
extern FILE *yyin;
%}

/* key words */
%token T_ENUM T_STRUCT

/* basic types*/
%token T_INT T_BOOL T_STRING T_FLOAT

/* constant values */
%token T_CONSTANT_INT T_CONSTANT_STRING T_CONSTANT_TRUE T_CONSTANT_FALSE T_CONSTANT_FLOAT

/* others */
%token T_IDENTIFIER

%%
Program     : /* empty */           { /* empty */ }
            | Program EnumDecl      { /* empty */ }
            | Program ConstValue    { /* empty */ }
            ;

EnumDecl    : EnumIdent '{' EnumMember '}' ';'      { printf("enum end\n"); }
            | T_ENUM T_IDENTIFIER ';'               { printf("enum predef %s\n", $2); }
            ;

EnumIdent   : T_ENUM T_IDENTIFIER                   { printf("enum %s start\n", $2); }
            ;

EnumMember  : /* empty */           { /* empty */ }
            | _EnumMember           { /* empty */ }
            ;

_EnumMember : T_IDENTIFIER                          { printf("%s\n", $1); }
            | _EnumMember ','                       { /* empty */ }
            | _EnumMember ',' T_IDENTIFIER          { printf(", %s\n", $3); }
            | _EnumMember '=' T_CONSTANT_INT        { printf("%s=%s\n", $1, yylval); }

/* const values */
ConstValue  : T_BOOL Identifier '=' BoolValue ';'       { /* empty */ }
            | T_INT Identifier '=' IntValue ';'       { /* empty */ }
            | T_FLOAT Identifier '=' FloatValue ';'       { /* empty */ }
            | T_STRING Identifier '=' StringValue ';'       { /* empty */ }
            ;

/* common detect */
Identifier  : T_IDENTIFIER      { printf("identifier:%s\n", $1); }
            ;

BoolValue   : T_CONSTANT_FALSE  { printf("bool value:false\n"); }
            | T_CONSTANT_TRUE   { printf("bool value:true\n"); }
            ;

IntValue    : T_CONSTANT_INT    { printf("int value:%s\n", $1); }
            ;

FloatValue  : T_CONSTANT_INT  { printf("float value:%s\n", $1); }
            ;

StringValue : T_CONSTANT_STRING { printf("string value:%s\n", $1); }
            ;
%%

int main(int argc, char* argv[])
{
    ++argv, --argc;    /* 跳过argv[0] */
    if (argc > 0) {
        yyin = fopen(argv[0], "r");
    } else {
        yyin = stdin;
    }

    return yyparse();
}
