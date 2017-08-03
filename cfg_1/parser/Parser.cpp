// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2013 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.

#line 37 "Parser.cpp" // lalr1.cc:398

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

#include "Parser.hpp"

// User implementation prologue.

#line 51 "Parser.cpp" // lalr1.cc:406
// Unqualified %code blocks.
#line 38 "./fb/parser.y" // lalr1.cc:407

#include "Driver.h"
#include "Scanner.h"
#include "Context.h"

static cfg::Parser::symbol_type yylex(cfg::Driver& driver)
{
    return driver.GetScanner()->Lex();
}

#define CONTEXT (*driver.GetContext())

#line 66 "Parser.cpp" // lalr1.cc:407


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyempty = true)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 11 "./fb/parser.y" // lalr1.cc:473
namespace  cfg  {
#line 152 "Parser.cpp" // lalr1.cc:473

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
   Parser ::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
   Parser :: Parser  (Driver& driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {}

   Parser ::~ Parser  ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
   Parser ::by_state::by_state ()
    : state (empty)
  {}

  inline
   Parser ::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
   Parser ::by_state::move (by_state& that)
  {
    state = that.state;
    that.state = empty;
  }

  inline
   Parser ::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
   Parser ::symbol_number_type
   Parser ::by_state::type_get () const
  {
    return state == empty ? 0 : yystos_[state];
  }

  inline
   Parser ::stack_symbol_type::stack_symbol_type ()
  {}


  inline
   Parser ::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 28: // "identifier"
      case 29: // "true"
      case 30: // "false"
      case 31: // "0"
      case 32: // "0.0f"
      case 33: // "empty"
      case 34: // "desc"
      case 60: // BoolValue
      case 61: // IntValue
      case 62: // FloatValue
      case 63: // ValueDesc
      case 64: // RefValue
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty;
  }

  inline
   Parser ::stack_symbol_type&
   Parser ::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 28: // "identifier"
      case 29: // "true"
      case 30: // "false"
      case 31: // "0"
      case 32: // "0.0f"
      case 33: // "empty"
      case 34: // "desc"
      case 60: // BoolValue
      case 61: // IntValue
      case 62: // FloatValue
      case 63: // ValueDesc
      case 64: // RefValue
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
   Parser ::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
   Parser ::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
   Parser ::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
   Parser ::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
   Parser ::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
   Parser ::debug_stream () const
  {
    return *yycdebug_;
  }

  void
   Parser ::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


   Parser ::debug_level_type
   Parser ::debug_level () const
  {
    return yydebug_;
  }

  void
   Parser ::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline  Parser ::state_type
   Parser ::yy_lr_goto_state_ (state_type yystate, int yylhs)
  {
    int yyr = yypgoto_[yylhs - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yylhs - yyntokens_];
  }

  inline bool
   Parser ::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
   Parser ::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
   Parser ::parse ()
  {
    /// Whether yyla contains a lookahead.
    bool yyempty = true;

    // State.
    int yyn;
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// $$ and @$.
    stack_symbol_type yylhs;

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    // User initialization code.
    #line 30 "./fb/parser.y" // lalr1.cc:726
{
    yyla.location.begin.filename = yyla.location.end.filename = const_cast<std::string*>(&driver.GetScanner()->File());
}

#line 446 "Parser.cpp" // lalr1.cc:726

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULL, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyempty)
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (driver));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
        yyempty = false;
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Discard the token being shifted.
    yyempty = true;

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
    /* Variants are always initialized to an empty instance of the
       correct type. The default $$=$1 action is NOT applied when using
       variants.  */
      switch (yyr1_[yyn])
    {
      case 28: // "identifier"
      case 29: // "true"
      case 30: // "false"
      case 31: // "0"
      case 32: // "0.0f"
      case 33: // "empty"
      case 34: // "desc"
      case 60: // BoolValue
      case 61: // IntValue
      case 62: // FloatValue
      case 63: // ValueDesc
      case 64: // RefValue
        yylhs.value.build< std::string > ();
        break;

      default:
        break;
    }


    // Compute the default @$.
    {
      slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
      YYLLOC_DEFAULT (yylhs.location, slice, yylen);
    }

    // Perform the reduction.
    YY_REDUCE_PRINT (yyn);
    try
      {
        switch (yyn)
          {
  case 2:
#line 90 "./fb/parser.y" // lalr1.cc:846
    { }
#line 573 "Parser.cpp" // lalr1.cc:846
    break;

  case 3:
#line 93 "./fb/parser.y" // lalr1.cc:846
    { }
#line 579 "Parser.cpp" // lalr1.cc:846
    break;

  case 4:
#line 94 "./fb/parser.y" // lalr1.cc:846
    { }
#line 585 "Parser.cpp" // lalr1.cc:846
    break;

  case 5:
#line 95 "./fb/parser.y" // lalr1.cc:846
    { }
#line 591 "Parser.cpp" // lalr1.cc:846
    break;

  case 6:
#line 96 "./fb/parser.y" // lalr1.cc:846
    { }
#line 597 "Parser.cpp" // lalr1.cc:846
    break;

  case 7:
#line 97 "./fb/parser.y" // lalr1.cc:846
    { }
#line 603 "Parser.cpp" // lalr1.cc:846
    break;

  case 8:
#line 101 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateEnd(false, yystack_[2].value.as< std::string > ()); }
#line 609 "Parser.cpp" // lalr1.cc:846
    break;

  case 9:
#line 102 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateEnd(true, yystack_[3].value.as< std::string > ()); }
#line 615 "Parser.cpp" // lalr1.cc:846
    break;

  case 10:
#line 105 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Bool, yystack_[0].value.as< std::string > ()); }
#line 621 "Parser.cpp" // lalr1.cc:846
    break;

  case 11:
#line 106 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Int, yystack_[0].value.as< std::string > ()); }
#line 627 "Parser.cpp" // lalr1.cc:846
    break;

  case 12:
#line 107 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Float, yystack_[0].value.as< std::string > ()); }
#line 633 "Parser.cpp" // lalr1.cc:846
    break;

  case 13:
#line 108 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::String, yystack_[0].value.as< std::string > ()); }
#line 639 "Parser.cpp" // lalr1.cc:846
    break;

  case 14:
#line 112 "./fb/parser.y" // lalr1.cc:846
    { }
#line 645 "Parser.cpp" // lalr1.cc:846
    break;

  case 15:
#line 115 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumBegin(yystack_[1].value.as< std::string > ()); }
#line 651 "Parser.cpp" // lalr1.cc:846
    break;

  case 16:
#line 118 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumEnd(); }
#line 657 "Parser.cpp" // lalr1.cc:846
    break;

  case 17:
#line 122 "./fb/parser.y" // lalr1.cc:846
    { }
#line 663 "Parser.cpp" // lalr1.cc:846
    break;

  case 18:
#line 123 "./fb/parser.y" // lalr1.cc:846
    { }
#line 669 "Parser.cpp" // lalr1.cc:846
    break;

  case 19:
#line 124 "./fb/parser.y" // lalr1.cc:846
    { }
#line 675 "Parser.cpp" // lalr1.cc:846
    break;

  case 20:
#line 127 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumMember(yystack_[0].value.as< std::string > ()); }
#line 681 "Parser.cpp" // lalr1.cc:846
    break;

  case 21:
#line 128 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumMember(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > (), false); }
#line 687 "Parser.cpp" // lalr1.cc:846
    break;

  case 22:
#line 129 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumMember(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > (), true); }
#line 693 "Parser.cpp" // lalr1.cc:846
    break;

  case 23:
#line 130 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumMember(yystack_[0].value.as< std::string > ()); }
#line 699 "Parser.cpp" // lalr1.cc:846
    break;

  case 24:
#line 131 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumMember(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > (), false); }
#line 705 "Parser.cpp" // lalr1.cc:846
    break;

  case 25:
#line 132 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumMember(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > (), true); }
#line 711 "Parser.cpp" // lalr1.cc:846
    break;

  case 26:
#line 136 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnPredefine(yystack_[1].value.as< std::string > ()); }
#line 717 "Parser.cpp" // lalr1.cc:846
    break;

  case 27:
#line 137 "./fb/parser.y" // lalr1.cc:846
    { }
#line 723 "Parser.cpp" // lalr1.cc:846
    break;

  case 28:
#line 140 "./fb/parser.y" // lalr1.cc:846
    { }
#line 729 "Parser.cpp" // lalr1.cc:846
    break;

  case 29:
#line 141 "./fb/parser.y" // lalr1.cc:846
    { }
#line 735 "Parser.cpp" // lalr1.cc:846
    break;

  case 30:
#line 144 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructBegin(yystack_[0].value.as< std::string > (), CfgCategory::None); }
#line 741 "Parser.cpp" // lalr1.cc:846
    break;

  case 31:
#line 145 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructBegin(yystack_[0].value.as< std::string > (), CfgCategory::Tab); }
#line 747 "Parser.cpp" // lalr1.cc:846
    break;

  case 32:
#line 146 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructBegin(yystack_[0].value.as< std::string > (), CfgCategory::Json); }
#line 753 "Parser.cpp" // lalr1.cc:846
    break;

  case 33:
#line 149 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructEnd(); }
#line 759 "Parser.cpp" // lalr1.cc:846
    break;

  case 34:
#line 153 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnInherit(yystack_[0].value.as< std::string > ()); }
#line 765 "Parser.cpp" // lalr1.cc:846
    break;

  case 35:
#line 154 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnInherit(yystack_[0].value.as< std::string > ()); }
#line 771 "Parser.cpp" // lalr1.cc:846
    break;

  case 36:
#line 157 "./fb/parser.y" // lalr1.cc:846
    { }
#line 777 "Parser.cpp" // lalr1.cc:846
    break;

  case 37:
#line 158 "./fb/parser.y" // lalr1.cc:846
    { }
#line 783 "Parser.cpp" // lalr1.cc:846
    break;

  case 38:
#line 160 "./fb/parser.y" // lalr1.cc:846
    { }
#line 789 "Parser.cpp" // lalr1.cc:846
    break;

  case 39:
#line 161 "./fb/parser.y" // lalr1.cc:846
    { }
#line 795 "Parser.cpp" // lalr1.cc:846
    break;

  case 40:
#line 165 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateEnd(false, yystack_[2].value.as< std::string > ()); }
#line 801 "Parser.cpp" // lalr1.cc:846
    break;

  case 41:
#line 166 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateEnd(true, yystack_[3].value.as< std::string > ()); }
#line 807 "Parser.cpp" // lalr1.cc:846
    break;

  case 42:
#line 169 "./fb/parser.y" // lalr1.cc:846
    { }
#line 813 "Parser.cpp" // lalr1.cc:846
    break;

  case 43:
#line 170 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Bool, yystack_[0].value.as< std::string > ()); }
#line 819 "Parser.cpp" // lalr1.cc:846
    break;

  case 44:
#line 171 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(yystack_[0].value.as< std::string > ()); }
#line 825 "Parser.cpp" // lalr1.cc:846
    break;

  case 45:
#line 172 "./fb/parser.y" // lalr1.cc:846
    { }
#line 831 "Parser.cpp" // lalr1.cc:846
    break;

  case 46:
#line 173 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Bool, yystack_[0].value.as< std::string > ()); }
#line 837 "Parser.cpp" // lalr1.cc:846
    break;

  case 47:
#line 174 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(yystack_[0].value.as< std::string > ()); }
#line 843 "Parser.cpp" // lalr1.cc:846
    break;

  case 48:
#line 175 "./fb/parser.y" // lalr1.cc:846
    { }
#line 849 "Parser.cpp" // lalr1.cc:846
    break;

  case 49:
#line 176 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Int, yystack_[0].value.as< std::string > ()); }
#line 855 "Parser.cpp" // lalr1.cc:846
    break;

  case 50:
#line 177 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(yystack_[0].value.as< std::string > ()); }
#line 861 "Parser.cpp" // lalr1.cc:846
    break;

  case 51:
#line 178 "./fb/parser.y" // lalr1.cc:846
    { }
#line 867 "Parser.cpp" // lalr1.cc:846
    break;

  case 52:
#line 179 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Int, yystack_[0].value.as< std::string > ()); }
#line 873 "Parser.cpp" // lalr1.cc:846
    break;

  case 53:
#line 180 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(yystack_[0].value.as< std::string > ()); }
#line 879 "Parser.cpp" // lalr1.cc:846
    break;

  case 54:
#line 181 "./fb/parser.y" // lalr1.cc:846
    { }
#line 885 "Parser.cpp" // lalr1.cc:846
    break;

  case 55:
#line 182 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Float, yystack_[0].value.as< std::string > ()); }
#line 891 "Parser.cpp" // lalr1.cc:846
    break;

  case 56:
#line 183 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(yystack_[0].value.as< std::string > ()); }
#line 897 "Parser.cpp" // lalr1.cc:846
    break;

  case 57:
#line 184 "./fb/parser.y" // lalr1.cc:846
    { }
#line 903 "Parser.cpp" // lalr1.cc:846
    break;

  case 58:
#line 185 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Float, yystack_[0].value.as< std::string > ()); }
#line 909 "Parser.cpp" // lalr1.cc:846
    break;

  case 59:
#line 186 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(yystack_[0].value.as< std::string > ()); }
#line 915 "Parser.cpp" // lalr1.cc:846
    break;

  case 60:
#line 187 "./fb/parser.y" // lalr1.cc:846
    { }
#line 921 "Parser.cpp" // lalr1.cc:846
    break;

  case 61:
#line 188 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::String, yystack_[0].value.as< std::string > ()); }
#line 927 "Parser.cpp" // lalr1.cc:846
    break;

  case 62:
#line 189 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(yystack_[0].value.as< std::string > ()); }
#line 933 "Parser.cpp" // lalr1.cc:846
    break;

  case 63:
#line 190 "./fb/parser.y" // lalr1.cc:846
    { }
#line 939 "Parser.cpp" // lalr1.cc:846
    break;

  case 64:
#line 191 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::String, yystack_[0].value.as< std::string > ()); }
#line 945 "Parser.cpp" // lalr1.cc:846
    break;

  case 65:
#line 192 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(yystack_[0].value.as< std::string > ()); }
#line 951 "Parser.cpp" // lalr1.cc:846
    break;

  case 66:
#line 193 "./fb/parser.y" // lalr1.cc:846
    { }
#line 957 "Parser.cpp" // lalr1.cc:846
    break;

  case 67:
#line 194 "./fb/parser.y" // lalr1.cc:846
    {  CONTEXT.OnVariateValue(yystack_[0].value.as< std::string > ()); }
#line 963 "Parser.cpp" // lalr1.cc:846
    break;

  case 68:
#line 195 "./fb/parser.y" // lalr1.cc:846
    { }
#line 969 "Parser.cpp" // lalr1.cc:846
    break;

  case 69:
#line 196 "./fb/parser.y" // lalr1.cc:846
    {  CONTEXT.OnVariateValue(yystack_[0].value.as< std::string > ()); }
#line 975 "Parser.cpp" // lalr1.cc:846
    break;

  case 70:
#line 199 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateBegin(yystack_[1].value.as< std::string > (), yystack_[0].value.as< std::string > ()); }
#line 981 "Parser.cpp" // lalr1.cc:846
    break;

  case 71:
#line 202 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateBegin(TYPE_BOOL, yystack_[0].value.as< std::string > ()); }
#line 987 "Parser.cpp" // lalr1.cc:846
    break;

  case 72:
#line 204 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateBegin(TYPE_INT, yystack_[0].value.as< std::string > ()); }
#line 993 "Parser.cpp" // lalr1.cc:846
    break;

  case 73:
#line 206 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateBegin(TYPE_FLOAT, yystack_[0].value.as< std::string > ()); }
#line 999 "Parser.cpp" // lalr1.cc:846
    break;

  case 74:
#line 208 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateBegin(TYPE_STRING, yystack_[0].value.as< std::string > ()); }
#line 1005 "Parser.cpp" // lalr1.cc:846
    break;

  case 75:
#line 211 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateArray(); }
#line 1011 "Parser.cpp" // lalr1.cc:846
    break;

  case 76:
#line 212 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateArray(yystack_[1].value.as< std::string > ()); }
#line 1017 "Parser.cpp" // lalr1.cc:846
    break;

  case 77:
#line 213 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateArray(); }
#line 1023 "Parser.cpp" // lalr1.cc:846
    break;

  case 78:
#line 214 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateArray(yystack_[1].value.as< std::string > ()); }
#line 1029 "Parser.cpp" // lalr1.cc:846
    break;

  case 79:
#line 217 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1035 "Parser.cpp" // lalr1.cc:846
    break;

  case 80:
#line 218 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1041 "Parser.cpp" // lalr1.cc:846
    break;

  case 81:
#line 221 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1047 "Parser.cpp" // lalr1.cc:846
    break;

  case 82:
#line 222 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = '-' + yystack_[0].value.as< std::string > (); }
#line 1053 "Parser.cpp" // lalr1.cc:846
    break;

  case 83:
#line 225 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1059 "Parser.cpp" // lalr1.cc:846
    break;

  case 84:
#line 226 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1065 "Parser.cpp" // lalr1.cc:846
    break;

  case 85:
#line 227 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = '-' + yystack_[0].value.as< std::string > (); }
#line 1071 "Parser.cpp" // lalr1.cc:846
    break;

  case 86:
#line 228 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = '-' + yystack_[0].value.as< std::string > (); }
#line 1077 "Parser.cpp" // lalr1.cc:846
    break;

  case 87:
#line 231 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = ""; }
#line 1083 "Parser.cpp" // lalr1.cc:846
    break;

  case 88:
#line 232 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1089 "Parser.cpp" // lalr1.cc:846
    break;

  case 89:
#line 236 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1095 "Parser.cpp" // lalr1.cc:846
    break;

  case 90:
#line 237 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[2].value.as< std::string > () + '.' + yystack_[0].value.as< std::string > (); }
#line 1101 "Parser.cpp" // lalr1.cc:846
    break;


#line 1105 "Parser.cpp" // lalr1.cc:846
          default:
            break;
          }
      }
    catch (const syntax_error& yyexc)
      {
        error (yyexc);
        YYERROR;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yylhs);
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    // Shift the result of the reduction.
    yypush_ (YY_NULL, yylhs);
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state,
                                           yyempty ? yyempty_ : yyla.type_get ()));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyempty)
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyempty = true;
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* $$ was initialized before running the user action.  */
    YY_SYMBOL_PRINT ("Error: discarding", yylhs);
    yylhs.~stack_symbol_type();
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyempty)
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyempty)
          yy_destroy_ (YY_NULL, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULL, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
   Parser ::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
   Parser ::yysyntax_error_ (state_type yystate, symbol_number_type yytoken) const
  {
    std::string yyres;
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yytoken) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (yytoken != yyempty_)
      {
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULL;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char  Parser ::yypact_ninf_ = -81;

  const signed char  Parser ::yytable_ninf_ = -38;

  const short int
   Parser ::yypact_[] =
  {
     -81,    12,     2,   -81,   -81,    -5,    32,    27,    39,   -81,
     -81,   -81,    48,   -81,    17,    62,    59,    49,    51,    74,
     108,   110,   107,   118,   109,    29,   -81,    34,   100,   -81,
      63,   102,   104,   106,   111,   115,   116,   120,   122,   123,
     -81,   -81,   -81,   -81,    37,   -81,   -81,   112,   -81,   -81,
     -81,    46,   113,   119,    85,    87,    89,    91,    93,   -81,
      45,   125,   -81,   -81,   -81,   -81,   -81,    79,    16,     1,
     114,   117,   -81,   -81,   -81,   129,   130,   132,   -81,   -81,
     121,    33,    97,    67,    99,    37,   101,    -3,   103,   -15,
     105,   124,   -81,   -81,   -81,   -81,   -81,   -81,    90,   -81,
     -81,   -81,   -81,   -81,   126,    37,   -81,   129,   -81,   127,
     121,    35,   -81,   129,    67,   -81,   129,    37,   -81,   129,
      -3,   -81,   129,    47,   -81,   -81,   -81,   -81,   -81,   129,
     -81,   129,   -81,   131,   -81,   129,   -81,   129,   -81,   129,
     -81,   129,   -81
  };

  const unsigned char
   Parser ::yydefact_[] =
  {
       3,     0,    87,     1,     7,     0,     0,     0,     0,    88,
       4,     5,    17,     6,    87,     0,     0,     0,     0,     0,
      30,    20,     0,    18,     0,    87,    38,     0,     0,    28,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      31,    32,    15,    26,     0,    16,    14,    19,    33,    27,
      39,     0,     0,     0,    66,    42,    48,    54,    60,    34,
       0,     0,    71,    72,    73,    74,     8,     0,     0,     0,
       0,     0,    89,    81,    21,    22,    23,     0,    70,    40,
       0,     0,    68,     0,    45,     0,    51,     0,    57,     0,
      63,     0,    29,     9,    79,    80,    10,    11,     0,    83,
      84,    12,    13,    82,     0,     0,    41,    67,    75,     0,
       0,     0,    43,    44,     0,    49,    50,     0,    55,    56,
       0,    61,    62,     0,    35,    85,    86,    90,    24,    25,
      76,    69,    77,     0,    46,    47,    52,    53,    58,    59,
      64,    65,    78
  };

  const short int
   Parser ::yypgoto_[] =
  {
     -81,   -81,   -81,   -81,   128,   -81,   -81,   -81,   -81,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   134,    94,   -81,
     -24,   -12,    -8,     3,    36,   -72,   -68,   -80,   148,   -79
  };

  const signed char
   Parser ::yydefgoto_[] =
  {
      -1,     1,     2,    10,    35,    11,    12,    46,    22,    23,
      13,    14,    15,    49,    60,    24,    25,    26,    53,    54,
      36,    37,    38,    39,    82,    96,    74,   101,    27,    75
  };

  const short int
   Parser ::yytable_[] =
  {
      97,   107,    -2,    55,   113,    98,   116,   118,   119,    98,
     122,   112,     3,    72,     4,    56,    17,   115,   121,    57,
       5,     6,     7,     8,    71,    72,   129,    55,    99,   100,
      58,   131,    99,   100,   -36,   135,     9,   128,   137,    56,
     138,   139,   134,    57,   141,    71,   -37,    73,   108,   136,
     132,     9,    91,    18,    58,    19,    51,    31,    32,    33,
      34,    92,    52,     9,   109,    72,   133,    20,    73,    31,
      32,    33,    34,    28,    52,    72,    21,    40,    29,    41,
     140,    30,    31,    32,    33,    34,    31,    32,    33,    34,
      42,    84,    86,    88,    90,    72,    94,    95,    80,    81,
      83,    81,    85,    81,    87,    81,    89,    81,    94,    95,
     110,   111,   114,   111,   117,   111,   120,   111,   123,   111,
      43,   125,   126,    44,    45,    47,    48,    66,    59,    67,
      62,    79,    63,    68,    64,    69,    70,    93,   104,    65,
      76,    78,   130,   105,   106,    77,   142,   102,   103,    72,
      16,     0,   124,     0,   127,     0,     0,     0,    61,    50
  };

  const signed char
   Parser ::yycheck_[] =
  {
      68,    80,     0,    27,    83,     8,    85,    87,    87,     8,
      89,    83,     0,    28,    12,    27,    21,    85,    33,    27,
      18,    19,    20,    21,     8,    28,   105,    51,    31,    32,
      27,   110,    31,    32,    17,   114,    34,   105,   117,    51,
     120,   120,   114,    51,   123,     8,    17,    31,    15,   117,
      15,    34,     7,    21,    51,    28,    22,    23,    24,    25,
      26,    16,    28,    34,    31,    28,    31,    28,    31,    23,
      24,    25,    26,    11,    28,    28,    28,    28,    16,    28,
      33,    22,    23,    24,    25,    26,    23,    24,    25,    26,
      16,    55,    56,    57,    58,    28,    29,    30,    13,    14,
      13,    14,    13,    14,    13,    14,    13,    14,    29,    30,
      13,    14,    13,    14,    13,    14,    13,    14,    13,    14,
      12,    31,    32,    13,    17,     7,    17,    12,    28,    13,
      28,    12,    28,    13,    28,    13,    13,    12,     9,    28,
      28,    28,    15,    13,    12,    51,    15,    33,    31,    28,
       2,    -1,    28,    -1,    28,    -1,    -1,    -1,    30,    25
  };

  const unsigned char
   Parser ::yystos_[] =
  {
       0,    36,    37,     0,    12,    18,    19,    20,    21,    34,
      38,    40,    41,    45,    46,    47,    63,    21,    21,    28,
      28,    28,    43,    44,    50,    51,    52,    63,    11,    16,
      22,    23,    24,    25,    26,    39,    55,    56,    57,    58,
      28,    28,    16,    12,    13,    17,    42,     7,    17,    48,
      52,    22,    28,    53,    54,    55,    56,    57,    58,    28,
      49,    39,    28,    28,    28,    28,    12,    13,    13,    13,
      13,     8,    28,    31,    61,    64,    28,    53,    28,    12,
      13,    14,    59,    13,    59,    13,    59,    13,    59,    13,
      59,     7,    16,    12,    29,    30,    60,    61,     8,    31,
      32,    62,    33,    31,     9,    13,    12,    64,    15,    31,
      13,    14,    60,    64,    13,    61,    64,    13,    62,    64,
      13,    33,    64,    13,    28,    31,    32,    28,    61,    64,
      15,    64,    15,    31,    60,    64,    61,    64,    62,    64,
      33,    64,    15
  };

  const unsigned char
   Parser ::yyr1_[] =
  {
       0,    35,    36,    37,    37,    37,    37,    37,    38,    38,
      39,    39,    39,    39,    40,    41,    42,    43,    43,    43,
      44,    44,    44,    44,    44,    44,    45,    45,    46,    46,
      47,    47,    47,    48,    49,    49,    50,    50,    51,    51,
      52,    52,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      54,    55,    56,    57,    58,    59,    59,    59,    59,    60,
      60,    61,    61,    62,    62,    62,    62,    63,    63,    64,
      64
  };

  const unsigned char
   Parser ::yyr2_[] =
  {
       0,     2,     1,     0,     2,     2,     2,     2,     3,     4,
       3,     3,     3,     3,     3,     3,     1,     0,     1,     2,
       1,     3,     3,     3,     5,     5,     3,     3,     2,     4,
       2,     3,     3,     1,     1,     3,     0,     1,     1,     2,
       3,     4,     1,     3,     3,     2,     4,     4,     1,     3,
       3,     2,     4,     4,     1,     3,     3,     2,     4,     4,
       1,     3,     3,     2,     4,     4,     1,     3,     2,     4,
       2,     2,     2,     2,     2,     2,     3,     3,     4,     1,
       1,     1,     2,     1,     1,     2,     2,     0,     1,     1,
       3
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const  Parser ::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\"(\"", "\")\"", "\"*\"",
  "\"+\"", "\",\"", "\"-\"", "\".\"", "\"/\"", "\":\"", "\";\"", "\"=\"",
  "\"[\"", "\"]\"", "\"{\"", "\"}\"", "TAB", "JSON", "ENUM", "STRUCT",
  "CONST", "BOOL", "INT", "FLOAT", "STRING", "NAMESPACE", "\"identifier\"",
  "\"true\"", "\"false\"", "\"0\"", "\"0.0f\"", "\"empty\"", "\"desc\"",
  "$accept", "Start", "Program", "ConstValue", "_ConstValue", "EnumDecl",
  "EnumBegin", "EnumEnd", "EnumMember", "_EnumMem", "StructDecl",
  "StructBegin", "_StructBegin", "StructEnd", "StructInherit",
  "StructMember", "_StructMember", "VariateDecl", "Variate", "VarCustom",
  "BoolVar", "IntVar", "FloatVar", "StringVar", "Array", "BoolValue",
  "IntValue", "FloatValue", "ValueDesc", "RefValue", YY_NULL
  };

#if YYDEBUG
  const unsigned char
   Parser ::yyrline_[] =
  {
       0,    90,    90,    93,    94,    95,    96,    97,   101,   102,
     105,   106,   107,   108,   112,   115,   118,   122,   123,   124,
     127,   128,   129,   130,   131,   132,   136,   137,   140,   141,
     144,   145,   146,   149,   153,   154,   157,   158,   160,   161,
     165,   166,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     199,   202,   204,   206,   208,   211,   212,   213,   214,   217,
     218,   221,   222,   225,   226,   227,   228,   231,   232,   236,
     237
  };

  // Print the state stack on the debug stream.
  void
   Parser ::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
   Parser ::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


#line 11 "./fb/parser.y" // lalr1.cc:1156
} //  cfg 
#line 1582 "Parser.cpp" // lalr1.cc:1156
#line 240 "./fb/parser.y" // lalr1.cc:1157


void cfg::Parser::error(const location_type& l, const std::string& m)
{
    driver.Error(l, m);
}
