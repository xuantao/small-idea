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
%token TAB JSON ENUM STRUCT CONST BOOL INT FLOAT STRING NAMESPACE VOID MODULE

%token <std::string> IDENTIFIER     "identifier"
%token <std::string> VALUE_TRUE     "true"
%token <std::string> VALUE_FALSE    "false"
%token <std::string> VALUE_INT      "0"
%token <std::string> VALUE_FLOAT    "0.0f"
%token <std::string> VALUE_STRING   "empty"
%token <std::string> VALUE_DESC     "desc"

%type <std::string> BoolValue IntValue FloatValue RefName

//%printer { yyoutput << $$; }

%%
%start Start;
Start   : Program   { }
        ;

Program : /* empty */           { }
        | Program ConstValue    { }
        | Program EnumDecl      { }
        | Program StructDecl    { }
        | Program NsDecl        { }
        ;

/* const value */
ConstValue  : ConstVal_1            { CONTEXT.OnVariateEnd(); }
            ;
ConstVal_1  : ConstVal_2            { }
            | ConstVal_2 VALUE_DESC { CONTEXT.OnVariateDesc($2); }
            ;
ConstVal_2  : ConstVal_3 S_SEMICOLON        { }
            | CONST ConstVal_3 S_SEMICOLON  { CONTEXT.OnVariateConst(); }
            ;
ConstVal_3  : DefBool S_ASSIGN BoolValue        { CONTEXT.OnVariateValue(RawCategory::Bool, $3); }
            | DefInt S_ASSIGN IntValue          { CONTEXT.OnVariateValue(RawCategory::Int, $3); }
            | DefFloat S_ASSIGN FloatValue      { CONTEXT.OnVariateValue(RawCategory::Float, $3); }
            | DefString S_ASSIGN VALUE_STRING   { CONTEXT.OnVariateValue(RawCategory::String, $3); }
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

_EnumVar    : IDENTIFIER                        { CONTEXT.OnEnumMember($1); }
            | IDENTIFIER S_ASSIGN IntValue      { CONTEXT.OnEnumMemberValue($1, $3); }
            | IDENTIFIER S_ASSIGN RefName       { CONTEXT.OnEnumMemberRefer($1, $3); }
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

StyMember   : VarDecl   { }
            ;

StyInner    : StyBegin StyDetail StyEnd { }
            ;

/* structure declear */
ModuleDecl  : ModuleBegin ModuleFunc ModuleEnd  { }
            ;

ModuleBegin : MODULE IDENTIFIER S_LBRACE    { CONTEXT.OnModuleBegin($2) }
            ;

ModuleEnd   : S_RBRACE                  { CONTEXT.OnModuleEnd(); }
            | S_RBRACE S_SEMICOLON      { CONTEXT.OnModuleEnd(); }
            ;

ModuleFunc  :  /* empty */           { }
            | ModuleFunc FuncBegin FuncParam FuncEnd { }
            ;

FuncBegin   : VOID IDENTIFIER S_LPAREN          { CONTEXT.OnFuncBegin($2); }
            | IDENTIFIER IDENTIFIER S_LPAREN    { CONTEXT.OnFuncBegin($1, $2); }
            ;

FuncEnd     : S_RPAREN              { CONTEXT.OnFuncEnd(); }
            | S_RPAREN S_SEMICOLON  { CONTEXT.OnFuncEnd(); }
            ;

FuncParam   : /* empty */           { }
            | FuncParam FuncVar     { }
            ;

FuncVar     : FuncVarDef                    { /* empty */ }
            | FuncVar S_COMMA FuncVarDef    { /* empty */ }
            ;

FuncVarDef  : DefBool                       { }
            | DefBool Array                 { }
            | DefInt                        { }
            | DefInt Array                  { }
            | DefFloat                      { }
            | DefFloat Array                { }
            | DefString                     { }
            | DefString Array               { }
            | DefCustom                     { }
            | DefCustom Array               { }
            ;

/* common detect */
VarDecl     : VarDecl_1                     { CONTEXT.OnVariateEnd(); }
            ;
VarDecl_1   : VarDecl_2                     { }
            | VarDecl_2 VALUE_DESC          { CONTEXT.OnVariateDesc($2); }
            ;
VarDecl_2   : VarDetail S_SEMICOLON         { }
            | CONST VarDetail S_SEMICOLON   { CONTEXT.OnVariateConst(); }
            ;

VarDetail   : DefBool                       { }
            | DefBool AssignBool            { }
            | DefBool Array                 { }
            | DefBool Array AssignBool      { }
            | DefInt                        { }
            | DefInt AssignInt              { }
            | DefInt Array                  { }
            | DefInt Array AssignInt        { }
            | DefFloat                      { }
            | DefFloat AssignFloat          { }
            | DefFloat Array                { }
            | DefFloat Array AssignFloat    { }
            | DefString                     { }
            | DefString AssignStr           { }
            | DefString Array               { }
            | DefString Array AssignStr     { }
            | DefCustom                     { }
            | DefCustom AssignRefer         { }
            | DefCustom Array               { }
            | DefCustom Array AssignRefer   { }
            ;

DefCustom   : RefName IDENTIFIER    { CONTEXT.OnVariateBegin($1, $2); }
            ;
DefBool     : BOOL IDENTIFIER       { CONTEXT.OnVariateBegin(TYPE_BOOL, $2); }
            ;
DefInt      : INT IDENTIFIER        { CONTEXT.OnVariateBegin(TYPE_INT, $2); }
            ;
DefFloat    : FLOAT IDENTIFIER      { CONTEXT.OnVariateBegin(TYPE_FLOAT, $2); }
            ;
DefString   : STRING IDENTIFIER     { CONTEXT.OnVariateBegin(TYPE_STRING, $2); }
            ;

AssignRefer : S_ASSIGN RefName      {  CONTEXT.OnVariateValue($2); }
            ;
AssignBool  : S_ASSIGN BoolValue    { CONTEXT.OnVariateValue(RawCategory::Bool, $2); }
            | AssignRefer           { }
            ;
AssignInt   : S_ASSIGN IntValue     { CONTEXT.OnVariateValue(RawCategory::Int, $2); }
            | AssignRefer           { }
            ;
AssignFloat : S_ASSIGN FloatValue   { CONTEXT.OnVariateValue(RawCategory::Float, $2); }
            | AssignRefer           { }
            ;
AssignStr   : S_ASSIGN VALUE_STRING { CONTEXT.OnVariateValue(RawCategory::String, $2); }
            | AssignRefer           { }
            ;

Array       : ArrayImpl         { }
            | Array ArrayImpl   { }
            ;
ArrayImpl   : S_LBRACK S_RBRACK                 { CONTEXT.OnVariateArray(); }
            | S_LBRACK IntValue S_RBRACK        { CONTEXT.OnVariateArrayLength($2); }
            | S_LBRACK RefName S_RBRACK         { CONTEXT.OnVariateArrayRefer($2); }
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

/* gobal or enum value reference */
RefName     : IDENTIFIER                { $$ = $1; }
            | RefName S_DOT IDENTIFIER  { $$ = $1 + '.' + $3; }
            ;

%%

void cfg::Parser::error(const location_type& l, const std::string& m)
{
    driver.Error(m.c_str());
}
