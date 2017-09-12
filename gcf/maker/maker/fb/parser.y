/* configuration file parser and generator */

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

namespace cfg {
    class Driver;
}

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
%token TAB JSON ENUM STRUCT CONST BOOL INT FLOAT STRING NAMESPACE VOID MODULE

%token <std::string> IDENTIFIER     "identifier"
%token <std::string> VALUE_TRUE     "true"
%token <std::string> VALUE_FALSE    "false"
%token <std::string> VALUE_INT      "0"
%token <std::string> VALUE_FLOAT    "0.0f"
%token <std::string> VALUE_STRING   "empty"
%token <std::string> VALUE_DESC     "desc"

%type <std::string> RefName Variate

//%printer { yyoutput << $$; }

%%
%start Start;
Start   : Program   { }
        ;

Program : /* empty */           { }
        | Program ConstValue    { }
        | Program EnumDecl      { }
        | Program StructDecl    { }
        | Program ModuleDecl    { }
        | Program NsDecl        { }
        ;

/* const value */
ConstValue  : VarConst Variate S_SEMICOLON VarDesc      { CONTEXT.OnVariate($2); }
            ;

 /* name space */
NsDecl      : NsBegin NsDetail NsEnd        { }
            ;

NsBegin     : NAMESPACE IDENTIFIER S_LBRACE { CONTEXT.OnNsBegin($2); }
            ;

NsEnd       : S_RBRACE                      { CONTEXT.OnNsEnd(); }
            ;

NsDetail    : /* empty */           { }
            | NsDetail ConstValue   { }
            | NsDetail EnumDecl     { }
            | NsDetail StructDecl   { }
            | NsDetail NsDecl       { }
            | NsDetail ModuleDecl   { }
            ;

/* enum declear */
EnumDecl    : EnumBegin EnumMember EnumEnd      { }
            ;

EnumBegin   : ENUM IDENTIFIER S_LBRACE          { CONTEXT.OnEnumBegin($2); }
            ;

EnumEnd     : S_RBRACE                          { CONTEXT.OnEnumEnd(); }
            | S_RBRACE S_SEMICOLON              { CONTEXT.OnEnumEnd(); }
            ;

EnumMember  : /* empty */                       { }
            | _EnumMem                          { }
            | _EnumMem S_COMMA                  { }
            ;

_EnumMem    : _EnumVar                          { /* empty */ }
            | _EnumMem S_COMMA _EnumVar         { /* empty */ }

_EnumVar    : IDENTIFIER                            { CONTEXT.OnEnumMember($1); }
            | IDENTIFIER S_ASSIGN VALUE_INT         { CONTEXT.OnEnumMemberValue($1, $3); }
            | IDENTIFIER S_ASSIGN S_MINUS VALUE_INT { CONTEXT.OnEnumMemberValue($1, "-" + $4); }
            | IDENTIFIER S_ASSIGN RefName           { CONTEXT.OnEnumMemberRefer($1, $3); }
            ;

/* structure declear */
StructDecl  : STRUCT IDENTIFIER S_SEMICOLON { CONTEXT.OnPredefine($2); }
            | StyBegin StyDetail StyEnd     { }
            ;

StyBegin    : _StyBegin S_LBRACE                    { }
            | _StyBegin S_COLON StyInherit S_LBRACE { }
            ;

_StyBegin   : STRUCT IDENTIFIER         { CONTEXT.OnStructBegin($2, CfgCategory::None); }
            | TAB STRUCT IDENTIFIER     { CONTEXT.OnStructBegin($3, CfgCategory::Tab); }
            | JSON STRUCT IDENTIFIER    { CONTEXT.OnStructBegin($3, CfgCategory::Json); }
            ;

StyEnd      : S_RBRACE                  { CONTEXT.OnStructEnd(); }
            | S_RBRACE S_SEMICOLON      { CONTEXT.OnStructEnd(); }
            ;

StyInherit  : IDENTIFIER                    { CONTEXT.OnStructInherit($1); }
            | StyInherit S_COMMA IDENTIFIER { CONTEXT.OnStructInherit($3); }
            ;

StyDetail   : /* empty */           { }
            | StyDetail StyMember   { }
            | StyDetail StyInner    { }
            ;

StyMember   : VarConst Variate S_SEMICOLON VarDesc  { CONTEXT.OnVariate($2); }
            ;

StyInner    : StyBegin StyDetail StyEnd { }
            ;

/* module declear */
ModuleDecl  : ModuleBegin ModuleFunc ModuleEnd  { }
            ;

ModuleBegin : MODULE IDENTIFIER S_LBRACE    { CONTEXT.OnModuleBegin($2); }
            ;

ModuleEnd   : S_RBRACE                  { CONTEXT.OnModuleEnd(); }
            | S_RBRACE S_SEMICOLON      { CONTEXT.OnModuleEnd(); }
            ;

ModuleFunc  :  /* empty */           { }
            | ModuleFunc FuncBegin FuncParam FuncEnd { }
            ;

FuncBegin   : VOID IDENTIFIER S_LPAREN    { CONTEXT.OnFuncBegin($2); }
            | Type IDENTIFIER S_LPAREN    { CONTEXT.OnFuncBegin($2); }
            ;

FuncEnd     : S_RPAREN              { CONTEXT.OnFuncEnd(); }
            | S_RPAREN S_SEMICOLON  { CONTEXT.OnFuncEnd(); }
            ;

FuncParam   : /* empty */           { }
            | FuncParam FuncParam1  { }
            ;

FuncParam1  : Variate                       { CONTEXT.OnVariate($1); }
            | FuncParam1 S_COMMA Variate    { CONTEXT.OnVariate($3); }
            ;

/* common detect */
VarConst    : /* empty */   { }
            | CONST         { CONTEXT.SetConst(); }
            ;
VarDesc     : /* empty */   { }
            | VALUE_DESC    { CONTEXT.SetDesc($1); }
            ;

/* gobal or enum value reference */
RefName     : IDENTIFIER                { $$ = $1; }
            | RefName S_DOT IDENTIFIER  { $$ = $1 + '.' + $3; }
            ;

Type        : BOOL      { CONTEXT.SetType(RawCategory::Bool); }
            | INT       { CONTEXT.SetType(RawCategory::Int); }
            | FLOAT     { CONTEXT.SetType(RawCategory::Float); }
            | STRING    { CONTEXT.SetType(RawCategory::String); }
            | RefName   { CONTEXT.SetType($1); }
            ;

Value       : VALUE_TRUE            { CONTEXT.SetValue(RawCategory::Bool, $1); }
            | VALUE_FALSE           { CONTEXT.SetValue(RawCategory::Bool, $1); }
            | VALUE_INT             { CONTEXT.SetValue(RawCategory::Int, $1); }
            | S_MINUS VALUE_INT     { CONTEXT.SetValue(RawCategory::Int, "-" + $2); }
            | VALUE_FLOAT           { CONTEXT.SetValue(RawCategory::Float, $1); }
            | S_MINUS VALUE_FLOAT   { CONTEXT.SetValue(RawCategory::Float, "-" + $2); }
            | VALUE_STRING          { CONTEXT.SetValue(RawCategory::String, $1); }
            | RefName               { CONTEXT.SetValue($1); }
            ;

Array22     : Array222_             { }
            | Array22 Array222_     { }
            ;
Array222_   : S_LBRACK S_RBRACK             { CONTEXT.SetArray(); }
            | S_LBRACK VALUE_INT S_RBRACK   { CONTEXT.SetArrayLength($2); }
            | S_LBRACK RefName S_RBRACK     { CONTEXT.SetArrayRefer($2); }
            ;

Variate     : Type IDENTIFIER                   { $$= $2; }
            | Type IDENTIFIER S_ASSIGN Value    { $$= $2; }
            | Type IDENTIFIER Array22           { $$= $2; }
            ;

%%

void cfg::Parser::error(const location_type& l, const std::string& m)
{
    driver.Error(m.c_str());
}
