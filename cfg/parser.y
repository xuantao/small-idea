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
            | Program StructDecl    { /* empty */ }
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
                | StructDetect StructBegin StructMember '}' ';'    { printf("}; // struct end\n"); }
                ;

StructDetect    : T_STRUCT Identifier                   { printf("struct %s", $2); }
                | StructDetect StructInherit            { /* empty */ }
                ;

StructBegin     : '{'                                   { printf(" {\n"); }
                ;

StructInherit   : ':' Identifier                        { printf(" : %s", $2); }
                | StructInherit ',' Identifier          { printf(", %s", $3); }
                ;

StructMember    : /* empty */                           { /* empty */ }
                | _StructMember                         { /* empty */ }
                ;

_StructMember   : VariateDecl                           { /* empty */ }
                | _StructMember VariateDecl             { /* empty */ }
                ;

/* const values */
ConstValue      : T_BOOL Identifier '=' BoolValue ';'       { printf("bool %s = %s;\n", $2, $4); }
                | T_INT Identifier '=' IntValue ';'         { printf("int %s = %s;\n", $2, $4); }
                | T_FLOAT Identifier '=' FloatValue ';'     { printf("float %s = %s;\n", $2, $4); }
                | T_STRING Identifier '=' StringValue ';'   { printf("string %s = %s;\n", $2, $4); }
                ;

/* common detect */
VariateDecl     : VariateDef ';'                    { printf(";\n"); }
                | VariateDef '=' VariateValue ';'   { printf(" = %s;\n", $3); }
                | VariateDef VariateArray ';'       { printf(";\n"); }
                ;

VariateDef      : T_BOOL Identifier             { printf("  bool %s", $2); }
                | T_INT Identifier              { printf("  int %s", $2); }
                | T_FLOAT Identifier            { printf("  float %s", $2); }
                | T_STRING Identifier           { printf("  string %s", $2); }
                | Identifier Identifier         { printf("  %s %s", $1, $2); }
                ;

VariateValue    : T_CONSTANT_FALSE              { /* empty */ }
                | T_CONSTANT_TRUE               { /* empty */ }
                | T_CONSTANT_INT                { /* empty */ }
                | T_CONSTANT_FLOAT              { /* empty */ }
                | T_CONSTANT_STRING             { /* empty */ }
                ;

VariateArray    : '[' ']'                               { printf("[]"); }
                | '[' T_CONSTANT_INT ']'                { printf("[%s]", $2); }
                | VariateArray '[' ']'                  { printf("[]"); }
                | VariateArray '[' T_CONSTANT_INT ']'   { printf("[%s]", $3); }
                ;

Identifier      : T_IDENTIFIER          { /* empty */ }
                ;

BoolValue       : T_CONSTANT_FALSE      { /* empty */ }
                | T_CONSTANT_TRUE       { /* empty */ }
                ;

IntValue        : '-' T_CONSTANT_INT    { /* empty */ }
                | T_CONSTANT_INT        { /* empty */ }
                ;

FloatValue      : _FloatValue           { /* empty */ }
                | '-' _FloatValue       { /* empty */ }
                ;

_FloatValue     : T_CONSTANT_INT        { /* empty */ }
                | T_CONSTANT_FLOAT      { /* empty */ }
                ;

StringValue     : T_CONSTANT_STRING     { /* empty */ }
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
