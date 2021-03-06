/* configuration file parser and generator */

%skeleton "lalr1.cc"
%require "3.0"
%defines
%define parser_class_name { Parser }

%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define api.namespace { gcf }

%code requires
{
#include <string>

namespace gcf {
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

static gcf::Parser::symbol_type yylex(gcf::Driver& driver)
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
%token TAB JSON ENUM STRUCT CONST BOOL BYTE INT LONG FLOAT DOUBLE STRING NAMESPACE VOID CROSS

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
        | Program CrossDecl     { }
        | Program NsDecl        { }
        ;

 /* const value */
ConstValue  : VarConst Variate S_SEMICOLON VarDesc              { CONTEXT.OnVariate($2); }
            | Attribute VarConst Variate S_SEMICOLON VarDesc    { CONTEXT.OnVariate($3); }
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
            | NsDetail CrossDecl    { }
            ;

 /* enum declare */
EnumDecl    : EnumBegin EnumMember EnumEnd              { }
            | Attribute EnumBegin EnumMember EnumEnd    { }
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

/* structure declare */
StructDecl  : STRUCT IDENTIFIER S_SEMICOLON         { CONTEXT.OnPredefine($2); }
            | StyBegin StyDetail StyEnd             { }
            | Attribute StyBegin StyDetail StyEnd   { }
            ;

StyBegin    : _StyBegin S_LBRACE                    { }
            | _StyBegin S_COLON StyInherit S_LBRACE { }
            ;

_StyBegin   : STRUCT IDENTIFIER                     { CONTEXT.OnStructBegin($2, CfgCategory::None); }
            | TAB STRUCT IDENTIFIER                 { CONTEXT.OnStructBegin($3, CfgCategory::Tab); }
            | JSON STRUCT IDENTIFIER                { CONTEXT.OnStructBegin($3, CfgCategory::Json); }
            ;

StyEnd      : S_RBRACE                              { CONTEXT.OnStructEnd(); }
            | S_RBRACE S_SEMICOLON                  { CONTEXT.OnStructEnd(); }
            ;

StyInherit  : IDENTIFIER                            { CONTEXT.OnStructInherit($1); }
            | StyInherit S_COMMA IDENTIFIER         { CONTEXT.OnStructInherit($3); }
            ;

StyDetail   : /* empty */                           { }
            | StyDetail StyMember                   { }
            | StyDetail StyInner                    { }
            ;

StyMember   : _StyMember                            { }
            | Attribute _StyMember                  { }
            ;

_StyMember  : VarConst Variate S_SEMICOLON VarDesc  { CONTEXT.OnVariate($2); }
            ;

StyInner    : StyBegin StyDetail StyEnd             { }
            ;

/* cross call declare */
CrossDecl   : CrossBegin CrossFunc CrossEnd             { }
            | Attribute CrossBegin CrossFunc CrossEnd   { }
            ;

CrossBegin  : CROSS IDENTIFIER S_LBRACE                 { CONTEXT.OnCrossBegin($2); }
            ;

CrossEnd    : S_RBRACE                                  { CONTEXT.OnCrossEnd(); }
            | S_RBRACE S_SEMICOLON                      { CONTEXT.OnCrossEnd(); }
            ;

CrossFunc   : /* empty */                               { }
            | CrossFunc Func                            { }
            ;

 /* function */
Func        : FuncBegin FuncParam FuncEnd               { }
            | Attribute FuncBegin FuncParam FuncEnd     { }

FuncBegin   : VOID IDENTIFIER S_LPAREN                  { CONTEXT.OnFuncBegin($2); }
            | Type IDENTIFIER S_LPAREN                  { CONTEXT.OnFuncBegin($2); }
            | Type Array IDENTIFIER S_LPAREN            { CONTEXT.OnFuncBegin($3); }
            ;

FuncEnd     : S_RPAREN                                  { CONTEXT.OnFuncEnd(); }
            | S_RPAREN S_SEMICOLON                      { CONTEXT.OnFuncEnd(); }
            ;

FuncParam   : /* empty */                               { }
            | FuncParam _FuncParam                      { }
            ;

_FuncParam  : Variate                                   { CONTEXT.OnVariate($1); }
            | _FuncParam S_COMMA Variate                { CONTEXT.OnVariate($3); }
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
            | BYTE      { CONTEXT.SetType(RawCategory::Byte); }
            | INT       { CONTEXT.SetType(RawCategory::Int); }
            | LONG      { CONTEXT.SetType(RawCategory::Long); }
            | FLOAT     { CONTEXT.SetType(RawCategory::Float); }
            | DOUBLE    { CONTEXT.SetType(RawCategory::Double); }
            | STRING    { CONTEXT.SetType(RawCategory::String); }
            | RefName   { CONTEXT.SetType($1); }
            ;

Value       : VALUE_TRUE            { CONTEXT.SetValue(RawCategory::Bool, $1); }
            | VALUE_FALSE           { CONTEXT.SetValue(RawCategory::Bool, $1); }
            | VALUE_INT             { CONTEXT.SetValue(RawCategory::Long, $1); }
            | S_MINUS VALUE_INT     { CONTEXT.SetValue(RawCategory::Long, "-" + $2); }
            | VALUE_FLOAT           { CONTEXT.SetValue(RawCategory::Double, $1); }
            | S_MINUS VALUE_FLOAT   { CONTEXT.SetValue(RawCategory::Double, "-" + $2); }
            | VALUE_STRING          { CONTEXT.SetValue(RawCategory::String, $1); }
            | RefName               { CONTEXT.SetValue($1); }
            ;

 /* array, multi/sized/unsized */
Array       : _Array            { }
            | Array _Array      { }
            ;
_Array      : S_LBRACK S_RBRACK             { CONTEXT.SetArray(); }
            | S_LBRACK VALUE_INT S_RBRACK   { CONTEXT.SetArrayLength($2); }
            | S_LBRACK RefName S_RBRACK     { CONTEXT.SetArrayRefer($2); }
            ;

Variate     : Type IDENTIFIER                   { $$= $2; }
            | Type IDENTIFIER S_ASSIGN Value    { $$= $2; }
            | Type IDENTIFIER Array             { $$= $2; }
            ;

 /* struct, member's attributes */
Attribute   : _Attribute                                { }
            | Attribute _Attribute                      { }
            ;
_Attribute  : S_LBRACK AttrDetail S_RBRACK              { }
            ;
AttrDetail  : _AttrDetail                               { }
            | AttrDetail S_COMMA _AttrDetail            { }
            ;
_AttrDetail : IDENTIFIER                                { CONTEXT.OnAttribute($1); }
            | IDENTIFIER S_LPAREN AttrParam S_RPAREN    { CONTEXT.OnAttribute($1); }
            ;
AttrParam   : Value                                     { }
            | AttrParam S_COMMA Value                   { }
            ;

%%

void gcf::Parser::error(const location_type& l, const std::string& m)
{
    driver.Error(m.c_str());
}
