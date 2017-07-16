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
            | Program StructDecl      { /* empty */ }
            ;

/* enum declear */
EnumDecl    : T_ENUM Identifier ';'                     { printf("predef enum %s;\n", $2); }
            | EnumDetect '{' EnumMember '}' ';'         { printf("\n}; // enum end\n"); }
            ;

EnumDetect  : T_ENUM Identifier                         { printf("enum %s {\n", $2); }
            ;

EnumMember  : /* empty */                               { /* empty */ }
            | _EnumMember                               { /* empty */ }
            ;

_EnumMember : Identifier                                { printf("  %s", $1); }
            | Identifier '=' IntValue                   { printf("  %s=%s", $1, $3); }
            | _EnumMember ','                           { /* empty */ }
            | _EnumMember ',' Identifier                { printf(",\n  %s", $3); }
            | _EnumMember ',' Identifier '=' IntValue   { printf(",\n  %s=%s", $3, $5); }
            ;

/* structure declear */
StructDecl      : T_STRUCT Identifier ';'               { printf("predef struct %s;\n", $2); }
                | StructDetect '{' StructMember '}' ';' { printf("\n}; // struct end\n"); }
                ;

StructDetect    : T_STRUCT Identifier                   { printf("struct %s\n", $2); }
                | StructDetect ':' StructInherit        { printf(" : "); }
                ;

/* todo: 继承这里还有问题 */
StructInherit   : Identifier                            { printf("%s", $1); }
                | StructInherit ',' Identifier          { printf(", %s", $3); }
                ;

StructMember    : /* empty */                           { /* empty */ }
                | _StructMember                         { /* empty */ }
                ;

_StructMember   : VariateDecl ';'                       { /* empty */ }
                | _StructMember VariateDecl ';'         { /* empty */ }
                ;      

/* const values */
ConstValue  : T_BOOL Identifier '=' BoolValue ';'       { /* empty */ }
            | T_INT Identifier '=' IntValue ';'         { /* empty */ }
            | T_FLOAT Identifier '=' FloatValue ';'     { /* empty */ }
            | T_STRING Identifier '=' StringValue ';'   { /* empty */ }
            ;

/* common detect */
VariateDecl     : VariateType Identifier                    { printf("define variate 1\n"); }
                | VariateType Identifier '=' VariateValue   { printf("define variate 2\n"); }
                | VariateType Identifier VariateArray       { printf("define variate 3\n"); }
                ;

VariateType     : T_BOOL             { printf("bool\n"); }
                | T_INT              { printf("int\n"); }
                | T_FLOAT            { printf("float\n"); }
                | T_STRING           { printf("string\n"); }
                | Identifier         { printf("usertype\n"); }
                ;

VariateValue    : T_CONSTANT_FALSE              { printf("value %s\n", $1);}
                | T_CONSTANT_TRUE               { printf("value %s\n", $1);}
                | T_CONSTANT_INT                { printf("value %s\n", $1);}
                | T_CONSTANT_FLOAT              { printf("value %s\n", $1);}
                | T_CONSTANT_STRING             { printf("value %s\n", $1);}
                ;

VariateArray    : '[' ']'                               { printf("array no 1 size\n"); }
                | '[' T_CONSTANT_INT ']'                { printf("array with 1 size:%s \n", $2); }
                | VariateArray '[' ']'                  { printf("array no 2 size\n"); }
                | VariateArray '[' T_CONSTANT_INT ']'   { printf("array with 2 size:%s \n", $3); }
                ;

Identifier      : T_IDENTIFIER      { /* empty */ }
                ;

BoolValue       : T_CONSTANT_FALSE  { /* empty */ }
                | T_CONSTANT_TRUE   { /* empty */ }
                ;

IntValue        : T_CONSTANT_INT    { /* empty */ }
                ;

FloatValue      : T_CONSTANT_INT    { /* empty */ }
                ;

StringValue     : T_CONSTANT_STRING { /* empty */ }
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
