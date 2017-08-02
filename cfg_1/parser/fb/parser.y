/* configuration file parser and generator*/

%skeleton "lalr1.cc"
%require "3.0"
%defines
%define parser_class_name { Parser }

%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define api.namespace { cfg }

%code requires
{
#include <string>
#include "CfgDef.h"

CFG_NAMESPACE_BEGIN
class Driver;
CFG_NAMESPACE_END

//#define YYSTYPE std::string
}

%lex-param {Driver& driver}
%parse-param {Driver& driver}

%locations
%initial-action
{
    @$.begin.filename = @$.end.filename = const_cast<std::string*>(&driver.GetScanner()->File());
}

%define parse.trace
%define parse.error verbose

%code
{
#include "Driver.h"
#include "Scanner.h"
#include "Context.h"

static cfg::Parser::symbol_type yylex(cfg::Driver& driver)
{
    return driver.GetScanner()->Lex();
}

#define CONTEXT (*driver.GetContext())
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
%token TAB JSON ENUM STRUCT CONST BOOL INT FLOAT STRING NAMESPACE

%token <std::string> IDENTIFIER     "identifier"
%token <std::string> VALUE_TRUE     "true"
%token <std::string> VALUE_FALSE    "false"
%token <std::string> VALUE_INT      "0"
%token <std::string> VALUE_FLOAT    "0.0f"
%token <std::string> VALUE_STRING   "empty"
%token <std::string> VALUE_DESC     "desc"

%type <std::string> BoolValue IntValue FloatValue RefValue ValueDesc

//%printer { yyoutput << $$; }

%%
%start Start;
Start   : Program   { }
        ;

Program : /* empty */           { }
        | Program ConstValue    { }
        | Program EnumDecl      { }
        | Program StructDecl    { }
        | Program S_SEMICOLON   { }
        ;

/* const value */
ConstValue  : ValueDesc _ConstValue S_SEMICOLON         { CONTEXT.OnVariateEnd(false, $1); }
            | ValueDesc CONST _ConstValue S_SEMICOLON   { CONTEXT.OnVariateEnd(true, $1); }
            ;

_ConstValue : BoolVar S_ASSIGN BoolValue        { CONTEXT.OnVariateValue(RawCategory::Bool, $3); }
            | IntVar S_ASSIGN IntValue          { CONTEXT.OnVariateValue(RawCategory::Int, $3); }
            | FloatVar S_ASSIGN FloatValue      { CONTEXT.OnVariateValue(RawCategory::Float, $3); }
            | StringVar S_ASSIGN VALUE_STRING   { CONTEXT.OnVariateValue(RawCategory::String, $3); }
            ;

/* enum declear */
EnumDecl    : EnumBegin EnumMember EnumEnd      { }
            ;

EnumBegin   : ENUM IDENTIFIER S_LBRACE          { CONTEXT.OnEnumBegin($2); }
            ;

EnumEnd     : S_RBRACE                          { CONTEXT.OnEnumEnd(); }
            //| EnumEnd S_SEMICOLON               { }
            ;

EnumMember  : /* empty */                       { }
            | _EnumMem                          { }
            | _EnumMem S_COMMA                  { }
            ;

_EnumMem    : IDENTIFIER                        { CONTEXT.OnEnumMember($1); }
            | IDENTIFIER S_ASSIGN IntValue      { CONTEXT.OnEnumMember($1, $3, false); }
            | IDENTIFIER S_ASSIGN RefValue      { CONTEXT.OnEnumMember($1, $3, true); }
            | _EnumMem S_COMMA IDENTIFIER       { CONTEXT.OnEnumMember($3); }
            | _EnumMem S_COMMA IDENTIFIER S_ASSIGN IntValue { CONTEXT.OnEnumMember($3, $5, false); }
            | _EnumMem S_COMMA IDENTIFIER S_ASSIGN RefValue { CONTEXT.OnEnumMember($3, $5, true); }
            ;

/* structure declear */
StructDecl      : STRUCT IDENTIFIER S_SEMICOLON         { CONTEXT.OnPredefine($2); }
                | StructBegin StructMember StructEnd    { }
                ;

StructBegin     : _StructBegin S_LBRACE                         { }
                | _StructBegin S_COLON StructInherit S_LBRACE   { }
                ;

_StructBegin    : STRUCT IDENTIFIER         { CONTEXT.OnStructBegin($2, CfgCategory::None); }
                | TAB STRUCT IDENTIFIER     { CONTEXT.OnStructBegin($3, CfgCategory::Tab); }
                | JSON STRUCT IDENTIFIER    { CONTEXT.OnStructBegin($3, CfgCategory::Json); }
                ;

StructEnd       : S_RBRACE              { CONTEXT.OnStructEnd(); }
                //| StructEnd S_SEMICOLON { }
                ;

StructInherit   : IDENTIFIER                        { CONTEXT.OnInherit($1); }
                | StructInherit S_COMMA IDENTIFIER  { CONTEXT.OnInherit($3); }
                ;

StructMember    : /* empty */                   { }
                | _StructMember                 { }
                ;
_StructMember   : VariateDecl                   { }
                | _StructMember VariateDecl     { }
                ;

/* common detect */
VariateDecl : ValueDesc Variate S_SEMICOLON         { CONTEXT.OnVariateEnd(false, $1); }
            | ValueDesc CONST Variate S_SEMICOLON   { CONTEXT.OnVariateEnd(true, $1); }
            ;

Variate     : BoolVar                           { }
            | BoolVar S_ASSIGN BoolValue        { CONTEXT.OnVariateValue(RawCategory::Bool, $3); }
            | BoolVar S_ASSIGN RefValue         { CONTEXT.OnVariateValue($3); }
            | BoolVar Array                     { }
            | BoolVar Array S_ASSIGN BoolValue  { CONTEXT.OnVariateValue(RawCategory::Bool, $4); }
            | BoolVar Array S_ASSIGN RefValue   { CONTEXT.OnVariateValue($4); }
            | IntVar                            { }
            | IntVar S_ASSIGN IntValue          { CONTEXT.OnVariateValue(RawCategory::Int, $3); }
            | IntVar S_ASSIGN RefValue          { CONTEXT.OnVariateValue($3); }
            | IntVar Array                      { }
            | IntVar Array S_ASSIGN IntValue    { CONTEXT.OnVariateValue(RawCategory::Int, $4); }
            | IntVar Array S_ASSIGN RefValue    { CONTEXT.OnVariateValue($4); }
            | FloatVar                          { }
            | FloatVar S_ASSIGN FloatValue      { CONTEXT.OnVariateValue(RawCategory::Float, $3); }
            | FloatVar S_ASSIGN RefValue        { CONTEXT.OnVariateValue($3); }
            | FloatVar Array                    { }
            | FloatVar Array S_ASSIGN FloatValue    { CONTEXT.OnVariateValue(RawCategory::Float, $4); }
            | FloatVar Array S_ASSIGN RefValue      { CONTEXT.OnVariateValue($4); }
            | StringVar                         { }
            | StringVar S_ASSIGN VALUE_STRING   { CONTEXT.OnVariateValue(RawCategory::String, $3); }
            | StringVar S_ASSIGN RefValue       { CONTEXT.OnVariateValue($3); }
            | StringVar Array                   { }
            | StringVar Array S_ASSIGN VALUE_STRING { CONTEXT.OnVariateValue(RawCategory::String, $4); }
            | StringVar Array S_ASSIGN RefValue     { CONTEXT.OnVariateValue($4); }
            | VarCustom                         { }
            | VarCustom S_ASSIGN RefValue       {  CONTEXT.OnVariateValue($3); }
            | VarCustom Array                   { }
            | VarCustom Array S_ASSIGN RefValue {  CONTEXT.OnVariateValue($4); }
            ;

VarCustom   : IDENTIFIER IDENTIFIER             { CONTEXT.OnVariateBegin($1, $2); }
            ;

BoolVar     : BOOL IDENTIFIER                   { CONTEXT.OnVariateBegin(TYPE_BOOL, $2); }
            ;
IntVar      : INT IDENTIFIER                    { CONTEXT.OnVariateBegin(TYPE_INT, $2); }
            ;
FloatVar    : FLOAT IDENTIFIER                  { CONTEXT.OnVariateBegin(TYPE_FLOAT, $2); }
            ;
StringVar   : STRING IDENTIFIER                 { CONTEXT.OnVariateBegin(TYPE_STRING, $2); }
            ;

Array       : S_LBRACK S_RBRACK                 { CONTEXT.OnVariateArray(); }
            | S_LBRACK VALUE_INT S_RBRACK       { CONTEXT.OnVariateArray($2); }
            | Array S_LBRACK S_RBRACK           { CONTEXT.OnVariateArray(); }
            | Array S_LBRACK VALUE_INT S_RBRACK { CONTEXT.OnVariateArray($3); }
            ;

BoolValue   : VALUE_TRUE            { $$ = $1; }
            | VALUE_FALSE           { $$ = $1; }
            ;

IntValue    : VALUE_INT             { $$ = $1; }
            | S_MINUS VALUE_INT     { $$ = '-' + $2; }
            ;

FloatValue  : VALUE_INT             { $$ = $1; }
            | VALUE_FLOAT           { $$ = $1; }
            | S_MINUS VALUE_INT     { $$ = '-' + $2; }
            | S_MINUS VALUE_FLOAT   { $$ = '-' + $2; }
            ;

ValueDesc   : /* empty */           { $$ = ""; }
            | VALUE_DESC            { $$ = $1; }
            ;

/* gobal or enum value reference */
RefValue    : IDENTIFIER                { $$ = $1; }
            | RefValue S_DOT IDENTIFIER { $$ = $1 + '.' + $3; }
            ;

%%

void cfg::Parser::error(const location_type& l, const std::string& m)
{
    driver.Error(l, m);
}
