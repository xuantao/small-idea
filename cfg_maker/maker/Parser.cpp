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
      case 75: // VarConst
        value.move< bool > (that.value);
        break;

      case 28: // "identifier"
      case 29: // "true"
      case 30: // "false"
      case 31: // "0"
      case 32: // "0.0f"
      case 33: // "empty"
      case 34: // "desc"
      case 71: // BoolValue
      case 72: // IntValue
      case 73: // FloatValue
      case 74: // VarDesc
      case 76: // RefName
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
      case 75: // VarConst
        value.copy< bool > (that.value);
        break;

      case 28: // "identifier"
      case 29: // "true"
      case 30: // "false"
      case 31: // "0"
      case 32: // "0.0f"
      case 33: // "empty"
      case 34: // "desc"
      case 71: // BoolValue
      case 72: // IntValue
      case 73: // FloatValue
      case 74: // VarDesc
      case 76: // RefName
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

#line 454 "Parser.cpp" // lalr1.cc:726

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
      case 75: // VarConst
        yylhs.value.build< bool > ();
        break;

      case 28: // "identifier"
      case 29: // "true"
      case 30: // "false"
      case 31: // "0"
      case 32: // "0.0f"
      case 33: // "empty"
      case 34: // "desc"
      case 71: // BoolValue
      case 72: // IntValue
      case 73: // FloatValue
      case 74: // VarDesc
      case 76: // RefName
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
#line 91 "./fb/parser.y" // lalr1.cc:846
    { }
#line 585 "Parser.cpp" // lalr1.cc:846
    break;

  case 3:
#line 94 "./fb/parser.y" // lalr1.cc:846
    { }
#line 591 "Parser.cpp" // lalr1.cc:846
    break;

  case 4:
#line 95 "./fb/parser.y" // lalr1.cc:846
    { }
#line 597 "Parser.cpp" // lalr1.cc:846
    break;

  case 5:
#line 96 "./fb/parser.y" // lalr1.cc:846
    { }
#line 603 "Parser.cpp" // lalr1.cc:846
    break;

  case 6:
#line 97 "./fb/parser.y" // lalr1.cc:846
    { }
#line 609 "Parser.cpp" // lalr1.cc:846
    break;

  case 7:
#line 98 "./fb/parser.y" // lalr1.cc:846
    { }
#line 615 "Parser.cpp" // lalr1.cc:846
    break;

  case 8:
#line 102 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateEnd(); }
#line 621 "Parser.cpp" // lalr1.cc:846
    break;

  case 9:
#line 103 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateConst(); CONTEXT.OnVariateEnd(); }
#line 627 "Parser.cpp" // lalr1.cc:846
    break;

  case 10:
#line 106 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Bool, yystack_[0].value.as< std::string > ()); }
#line 633 "Parser.cpp" // lalr1.cc:846
    break;

  case 11:
#line 107 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Int, yystack_[0].value.as< std::string > ()); }
#line 639 "Parser.cpp" // lalr1.cc:846
    break;

  case 12:
#line 108 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Float, yystack_[0].value.as< std::string > ()); }
#line 645 "Parser.cpp" // lalr1.cc:846
    break;

  case 13:
#line 109 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::String, yystack_[0].value.as< std::string > ()); }
#line 651 "Parser.cpp" // lalr1.cc:846
    break;

  case 14:
#line 113 "./fb/parser.y" // lalr1.cc:846
    { }
#line 657 "Parser.cpp" // lalr1.cc:846
    break;

  case 15:
#line 116 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnNsBegin(yystack_[1].value.as< std::string > ()); }
#line 663 "Parser.cpp" // lalr1.cc:846
    break;

  case 16:
#line 119 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnNsEnd(); }
#line 669 "Parser.cpp" // lalr1.cc:846
    break;

  case 17:
#line 122 "./fb/parser.y" // lalr1.cc:846
    { }
#line 675 "Parser.cpp" // lalr1.cc:846
    break;

  case 18:
#line 123 "./fb/parser.y" // lalr1.cc:846
    { }
#line 681 "Parser.cpp" // lalr1.cc:846
    break;

  case 19:
#line 124 "./fb/parser.y" // lalr1.cc:846
    { }
#line 687 "Parser.cpp" // lalr1.cc:846
    break;

  case 20:
#line 125 "./fb/parser.y" // lalr1.cc:846
    { }
#line 693 "Parser.cpp" // lalr1.cc:846
    break;

  case 21:
#line 126 "./fb/parser.y" // lalr1.cc:846
    { }
#line 699 "Parser.cpp" // lalr1.cc:846
    break;

  case 22:
#line 130 "./fb/parser.y" // lalr1.cc:846
    { }
#line 705 "Parser.cpp" // lalr1.cc:846
    break;

  case 23:
#line 133 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumBegin(yystack_[1].value.as< std::string > ()); }
#line 711 "Parser.cpp" // lalr1.cc:846
    break;

  case 24:
#line 136 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumEnd(); }
#line 717 "Parser.cpp" // lalr1.cc:846
    break;

  case 25:
#line 137 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumEnd(); }
#line 723 "Parser.cpp" // lalr1.cc:846
    break;

  case 26:
#line 140 "./fb/parser.y" // lalr1.cc:846
    { }
#line 729 "Parser.cpp" // lalr1.cc:846
    break;

  case 27:
#line 141 "./fb/parser.y" // lalr1.cc:846
    { }
#line 735 "Parser.cpp" // lalr1.cc:846
    break;

  case 28:
#line 142 "./fb/parser.y" // lalr1.cc:846
    { }
#line 741 "Parser.cpp" // lalr1.cc:846
    break;

  case 29:
#line 145 "./fb/parser.y" // lalr1.cc:846
    { /* empty */ }
#line 747 "Parser.cpp" // lalr1.cc:846
    break;

  case 30:
#line 146 "./fb/parser.y" // lalr1.cc:846
    { /* empty */ }
#line 753 "Parser.cpp" // lalr1.cc:846
    break;

  case 31:
#line 148 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumMember(yystack_[0].value.as< std::string > ()); }
#line 759 "Parser.cpp" // lalr1.cc:846
    break;

  case 32:
#line 149 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumMemberValue(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()); }
#line 765 "Parser.cpp" // lalr1.cc:846
    break;

  case 33:
#line 150 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumMemberRefer(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()); }
#line 771 "Parser.cpp" // lalr1.cc:846
    break;

  case 34:
#line 154 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnPredefine(yystack_[1].value.as< std::string > ()); }
#line 777 "Parser.cpp" // lalr1.cc:846
    break;

  case 35:
#line 155 "./fb/parser.y" // lalr1.cc:846
    { }
#line 783 "Parser.cpp" // lalr1.cc:846
    break;

  case 36:
#line 158 "./fb/parser.y" // lalr1.cc:846
    { }
#line 789 "Parser.cpp" // lalr1.cc:846
    break;

  case 37:
#line 159 "./fb/parser.y" // lalr1.cc:846
    { }
#line 795 "Parser.cpp" // lalr1.cc:846
    break;

  case 38:
#line 162 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructBegin(yystack_[0].value.as< std::string > (), CfgCategory::None); }
#line 801 "Parser.cpp" // lalr1.cc:846
    break;

  case 39:
#line 163 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructBegin(yystack_[0].value.as< std::string > (), CfgCategory::Tab); }
#line 807 "Parser.cpp" // lalr1.cc:846
    break;

  case 40:
#line 164 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructBegin(yystack_[0].value.as< std::string > (), CfgCategory::Json); }
#line 813 "Parser.cpp" // lalr1.cc:846
    break;

  case 41:
#line 167 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructEnd(); }
#line 819 "Parser.cpp" // lalr1.cc:846
    break;

  case 42:
#line 168 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructEnd(); }
#line 825 "Parser.cpp" // lalr1.cc:846
    break;

  case 43:
#line 171 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructInherit(yystack_[0].value.as< std::string > ()); }
#line 831 "Parser.cpp" // lalr1.cc:846
    break;

  case 44:
#line 172 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructInherit(yystack_[0].value.as< std::string > ()); }
#line 837 "Parser.cpp" // lalr1.cc:846
    break;

  case 45:
#line 175 "./fb/parser.y" // lalr1.cc:846
    { }
#line 843 "Parser.cpp" // lalr1.cc:846
    break;

  case 46:
#line 176 "./fb/parser.y" // lalr1.cc:846
    { }
#line 849 "Parser.cpp" // lalr1.cc:846
    break;

  case 47:
#line 177 "./fb/parser.y" // lalr1.cc:846
    { }
#line 855 "Parser.cpp" // lalr1.cc:846
    break;

  case 48:
#line 180 "./fb/parser.y" // lalr1.cc:846
    { }
#line 861 "Parser.cpp" // lalr1.cc:846
    break;

  case 49:
#line 183 "./fb/parser.y" // lalr1.cc:846
    { }
#line 867 "Parser.cpp" // lalr1.cc:846
    break;

  case 50:
#line 187 "./fb/parser.y" // lalr1.cc:846
    {
                    if (!yystack_[3].value.as< std::string > ().empty()) CONTEXT.OnVariateDesc(yystack_[3].value.as< std::string > ());
                    if (yystack_[2].value.as< bool > ()) CONTEXT.OnVariateConst();
                    CONTEXT.OnVariateEnd();
                }
#line 877 "Parser.cpp" // lalr1.cc:846
    break;

  case 51:
#line 194 "./fb/parser.y" // lalr1.cc:846
    { }
#line 883 "Parser.cpp" // lalr1.cc:846
    break;

  case 52:
#line 195 "./fb/parser.y" // lalr1.cc:846
    { }
#line 889 "Parser.cpp" // lalr1.cc:846
    break;

  case 53:
#line 196 "./fb/parser.y" // lalr1.cc:846
    { }
#line 895 "Parser.cpp" // lalr1.cc:846
    break;

  case 54:
#line 197 "./fb/parser.y" // lalr1.cc:846
    { }
#line 901 "Parser.cpp" // lalr1.cc:846
    break;

  case 55:
#line 198 "./fb/parser.y" // lalr1.cc:846
    { }
#line 907 "Parser.cpp" // lalr1.cc:846
    break;

  case 56:
#line 199 "./fb/parser.y" // lalr1.cc:846
    { }
#line 913 "Parser.cpp" // lalr1.cc:846
    break;

  case 57:
#line 200 "./fb/parser.y" // lalr1.cc:846
    { }
#line 919 "Parser.cpp" // lalr1.cc:846
    break;

  case 58:
#line 201 "./fb/parser.y" // lalr1.cc:846
    { }
#line 925 "Parser.cpp" // lalr1.cc:846
    break;

  case 59:
#line 202 "./fb/parser.y" // lalr1.cc:846
    { }
#line 931 "Parser.cpp" // lalr1.cc:846
    break;

  case 60:
#line 203 "./fb/parser.y" // lalr1.cc:846
    { }
#line 937 "Parser.cpp" // lalr1.cc:846
    break;

  case 61:
#line 204 "./fb/parser.y" // lalr1.cc:846
    { }
#line 943 "Parser.cpp" // lalr1.cc:846
    break;

  case 62:
#line 205 "./fb/parser.y" // lalr1.cc:846
    { }
#line 949 "Parser.cpp" // lalr1.cc:846
    break;

  case 63:
#line 206 "./fb/parser.y" // lalr1.cc:846
    { }
#line 955 "Parser.cpp" // lalr1.cc:846
    break;

  case 64:
#line 207 "./fb/parser.y" // lalr1.cc:846
    { }
#line 961 "Parser.cpp" // lalr1.cc:846
    break;

  case 65:
#line 208 "./fb/parser.y" // lalr1.cc:846
    { }
#line 967 "Parser.cpp" // lalr1.cc:846
    break;

  case 66:
#line 209 "./fb/parser.y" // lalr1.cc:846
    { }
#line 973 "Parser.cpp" // lalr1.cc:846
    break;

  case 67:
#line 210 "./fb/parser.y" // lalr1.cc:846
    { }
#line 979 "Parser.cpp" // lalr1.cc:846
    break;

  case 68:
#line 211 "./fb/parser.y" // lalr1.cc:846
    { }
#line 985 "Parser.cpp" // lalr1.cc:846
    break;

  case 69:
#line 212 "./fb/parser.y" // lalr1.cc:846
    { }
#line 991 "Parser.cpp" // lalr1.cc:846
    break;

  case 70:
#line 213 "./fb/parser.y" // lalr1.cc:846
    { }
#line 997 "Parser.cpp" // lalr1.cc:846
    break;

  case 71:
#line 216 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateBegin(yystack_[1].value.as< std::string > (), yystack_[0].value.as< std::string > ()); }
#line 1003 "Parser.cpp" // lalr1.cc:846
    break;

  case 72:
#line 218 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateBegin(TYPE_BOOL, yystack_[0].value.as< std::string > ()); }
#line 1009 "Parser.cpp" // lalr1.cc:846
    break;

  case 73:
#line 220 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateBegin(TYPE_INT, yystack_[0].value.as< std::string > ()); }
#line 1015 "Parser.cpp" // lalr1.cc:846
    break;

  case 74:
#line 222 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateBegin(TYPE_FLOAT, yystack_[0].value.as< std::string > ()); }
#line 1021 "Parser.cpp" // lalr1.cc:846
    break;

  case 75:
#line 224 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateBegin(TYPE_STRING, yystack_[0].value.as< std::string > ()); }
#line 1027 "Parser.cpp" // lalr1.cc:846
    break;

  case 76:
#line 227 "./fb/parser.y" // lalr1.cc:846
    {  CONTEXT.OnVariateValue(yystack_[0].value.as< std::string > ()); }
#line 1033 "Parser.cpp" // lalr1.cc:846
    break;

  case 77:
#line 229 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Bool, yystack_[0].value.as< std::string > ()); }
#line 1039 "Parser.cpp" // lalr1.cc:846
    break;

  case 78:
#line 230 "./fb/parser.y" // lalr1.cc:846
    { }
#line 1045 "Parser.cpp" // lalr1.cc:846
    break;

  case 79:
#line 232 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Int, yystack_[0].value.as< std::string > ()); }
#line 1051 "Parser.cpp" // lalr1.cc:846
    break;

  case 80:
#line 233 "./fb/parser.y" // lalr1.cc:846
    { }
#line 1057 "Parser.cpp" // lalr1.cc:846
    break;

  case 81:
#line 235 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Float, yystack_[0].value.as< std::string > ()); }
#line 1063 "Parser.cpp" // lalr1.cc:846
    break;

  case 82:
#line 236 "./fb/parser.y" // lalr1.cc:846
    { }
#line 1069 "Parser.cpp" // lalr1.cc:846
    break;

  case 83:
#line 238 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::String, yystack_[0].value.as< std::string > ()); }
#line 1075 "Parser.cpp" // lalr1.cc:846
    break;

  case 84:
#line 239 "./fb/parser.y" // lalr1.cc:846
    { }
#line 1081 "Parser.cpp" // lalr1.cc:846
    break;

  case 85:
#line 242 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateArray(); }
#line 1087 "Parser.cpp" // lalr1.cc:846
    break;

  case 86:
#line 243 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateArray(yystack_[1].value.as< std::string > ()); }
#line 1093 "Parser.cpp" // lalr1.cc:846
    break;

  case 87:
#line 244 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateArray(); }
#line 1099 "Parser.cpp" // lalr1.cc:846
    break;

  case 88:
#line 245 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateArray(yystack_[1].value.as< std::string > ()); }
#line 1105 "Parser.cpp" // lalr1.cc:846
    break;

  case 89:
#line 247 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1111 "Parser.cpp" // lalr1.cc:846
    break;

  case 90:
#line 248 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1117 "Parser.cpp" // lalr1.cc:846
    break;

  case 91:
#line 251 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1123 "Parser.cpp" // lalr1.cc:846
    break;

  case 92:
#line 252 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = '-' + yystack_[0].value.as< std::string > (); }
#line 1129 "Parser.cpp" // lalr1.cc:846
    break;

  case 93:
#line 255 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1135 "Parser.cpp" // lalr1.cc:846
    break;

  case 94:
#line 256 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1141 "Parser.cpp" // lalr1.cc:846
    break;

  case 95:
#line 257 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = '-' + yystack_[0].value.as< std::string > (); }
#line 1147 "Parser.cpp" // lalr1.cc:846
    break;

  case 96:
#line 258 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = '-' + yystack_[0].value.as< std::string > (); }
#line 1153 "Parser.cpp" // lalr1.cc:846
    break;

  case 97:
#line 261 "./fb/parser.y" // lalr1.cc:846
    { /* empty */ }
#line 1159 "Parser.cpp" // lalr1.cc:846
    break;

  case 98:
#line 262 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1165 "Parser.cpp" // lalr1.cc:846
    break;

  case 99:
#line 265 "./fb/parser.y" // lalr1.cc:846
    { /* empty */ }
#line 1171 "Parser.cpp" // lalr1.cc:846
    break;

  case 100:
#line 266 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< bool > () = true; }
#line 1177 "Parser.cpp" // lalr1.cc:846
    break;

  case 101:
#line 270 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1183 "Parser.cpp" // lalr1.cc:846
    break;

  case 102:
#line 271 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[2].value.as< std::string > () + '.' + yystack_[0].value.as< std::string > (); }
#line 1189 "Parser.cpp" // lalr1.cc:846
    break;


#line 1193 "Parser.cpp" // lalr1.cc:846
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


  const signed char  Parser ::yypact_ninf_ = -99;

  const signed char  Parser ::yytable_ninf_ = -3;

  const signed char
   Parser ::yypact_[] =
  {
     -99,    16,     5,   -99,    15,    20,   -22,    23,    39,   -99,
     -99,   -99,   -99,   -99,    45,   -99,   -99,     1,    56,    83,
      85,    98,   100,    99,    29,   103,   101,   110,   -99,    43,
      91,   -99,    60,    93,    95,    97,   102,   108,   109,   111,
     113,   114,   -99,   -99,   -99,   -99,   -99,   -99,   -99,   -99,
     -99,   -99,   -99,    14,   116,   -99,    45,   117,   104,   -99,
     -99,   -99,   -99,   -99,   112,   -99,    52,   119,   -99,   -99,
     -99,   -99,   -99,    36,     0,    -4,   105,   106,   -99,   -99,
     -99,   124,   -99,   -99,   -99,   -99,    43,   -99,    46,   107,
     -99,   -99,   -99,   -99,   -99,   -99,    44,   -99,   -99,   -99,
     -99,   -99,   115,   -99,   127,    78,    80,    82,    84,    86,
       6,   -99,   -99,   -99,   -99,   -99,   118,    -1,   -99,    88,
      25,   -99,   -99,    90,    14,   -99,   -99,    92,    12,   -99,
     -99,    94,   -15,   -99,   -99,    96,   -99,   124,   -99,   121,
       4,   -99,   -99,   -99,   -99,   -99,   -99,   -99,   -99,   -99,
     -99,   -99,   125,   -99
  };

  const unsigned char
   Parser ::yydefact_[] =
  {
       3,     0,    97,     1,     0,     0,     0,     0,     0,    98,
       4,     7,    17,     5,    26,     6,    45,     0,     0,     0,
       0,     0,    38,     0,    97,    31,     0,    27,    29,    97,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    39,    40,    23,    34,    15,    16,    18,    21,
      14,    19,    20,     0,    24,    22,    28,    41,     0,    45,
      35,    46,    47,    48,    99,    43,     0,     0,    72,    73,
      74,    75,     8,     0,     0,     0,     0,     0,   101,    91,
      32,    33,    25,    30,    42,    38,    97,   100,     0,     0,
      37,     9,    89,    90,    10,    11,     0,    93,    94,    12,
      13,    92,     0,    49,     0,    67,    51,    55,    59,    63,
       0,    44,    95,    96,   102,    50,     0,     0,    68,    69,
       0,    78,    52,    53,     0,    80,    56,    57,     0,    82,
      60,    61,     0,    84,    64,    65,    71,    76,    85,     0,
       0,    70,    77,    54,    79,    58,    81,    62,    83,    66,
      86,    87,     0,    88
  };

  const short int
   Parser ::yypgoto_[] =
  {
     -99,   -99,   -99,   120,   122,   123,   -99,   -99,   -99,   126,
     -99,   -99,   -99,   -99,    89,   128,   -29,   -99,    55,   -99,
     129,   -99,   -99,   -99,   -99,   -99,    54,    61,    63,    65,
     -98,    32,    21,    26,    24,   -19,    38,   -72,    28,   -28,
     -99,   -50
  };

  const short int
   Parser ::yydefgoto_[] =
  {
      -1,     1,     2,    10,    37,    11,    12,    50,    24,    13,
      14,    55,    26,    27,    28,    15,    16,    17,    60,    66,
      29,    61,    62,    63,   104,   105,    38,    39,    40,    41,
     121,   122,   126,   130,   134,   119,    94,    80,    99,    18,
      88,   137
  };

  const short int
   Parser ::yytable_[] =
  {
      59,    64,    95,    81,    96,    -2,    21,   118,    77,   125,
     129,   133,    30,    78,   138,   102,     3,    31,   148,   151,
      96,   141,    77,     4,     5,     6,     7,    97,    98,   125,
     139,    79,     8,   129,   136,   152,    19,   133,   110,     9,
      78,    20,    78,    97,    98,    79,    47,     4,     5,     6,
       7,    22,   144,    78,    92,    93,     8,    59,    64,    89,
      57,     4,     5,     9,    58,    92,    93,    23,    90,    33,
      34,    35,    36,    25,    78,   112,   113,     9,    32,    33,
      34,    35,    36,    33,    34,    35,    36,   123,   127,   131,
     135,   116,   117,   120,   117,   124,   117,   128,   117,   132,
     117,   116,   140,   120,   140,   124,   140,   128,   140,   132,
     140,    42,    45,    43,    44,    46,    53,    56,    54,    65,
      72,    68,    73,    69,    74,    70,    75,    76,    82,    84,
      71,    91,    85,   102,    87,   111,   150,   101,   100,   115,
     153,   103,   106,   114,    48,    83,    78,    49,   145,   107,
      51,   108,    52,   109,    67,   143,   146,   147,   142,   149,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    86
  };

  const short int
   Parser ::yycheck_[] =
  {
      29,    29,    74,    53,     8,     0,    28,   105,     8,   107,
     108,   109,    11,    28,    15,     9,     0,    16,    33,    15,
       8,   119,     8,    18,    19,    20,    21,    31,    32,   127,
      31,    31,    27,   131,    28,    31,    21,   135,    88,    34,
      28,    21,    28,    31,    32,    31,    17,    18,    19,    20,
      21,    28,   124,    28,    29,    30,    27,    86,    86,     7,
      17,    18,    19,    34,    21,    29,    30,    28,    16,    23,
      24,    25,    26,    28,    28,    31,    32,    34,    22,    23,
      24,    25,    26,    23,    24,    25,    26,   106,   107,   108,
     109,    13,    14,    13,    14,    13,    14,    13,    14,    13,
      14,    13,    14,    13,    14,    13,    14,    13,    14,    13,
      14,    28,    12,    28,    16,    16,    13,     7,    17,    28,
      12,    28,    13,    28,    13,    28,    13,    13,    12,    12,
      28,    12,    28,     9,    22,    28,    15,    31,    33,    12,
      15,    86,    88,    28,    24,    56,    28,    24,   127,    88,
      24,    88,    24,    88,    32,   123,   128,   131,   120,   135,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    59
  };

  const unsigned char
   Parser ::yystos_[] =
  {
       0,    36,    37,     0,    18,    19,    20,    21,    27,    34,
      38,    40,    41,    44,    45,    50,    51,    52,    74,    21,
      21,    28,    28,    28,    43,    28,    47,    48,    49,    55,
      11,    16,    22,    23,    24,    25,    26,    39,    61,    62,
      63,    64,    28,    28,    16,    12,    16,    17,    38,    40,
      42,    44,    50,    13,    17,    46,     7,    17,    21,    51,
      53,    56,    57,    58,    74,    28,    54,    39,    28,    28,
      28,    28,    12,    13,    13,    13,    13,     8,    28,    31,
      72,    76,    12,    49,    12,    28,    55,    22,    75,     7,
      16,    12,    29,    30,    71,    72,     8,    31,    32,    73,
      33,    31,     9,    53,    59,    60,    61,    62,    63,    64,
      76,    28,    31,    32,    28,    12,    13,    14,    65,    70,
      13,    65,    66,    70,    13,    65,    67,    70,    13,    65,
      68,    70,    13,    65,    69,    70,    28,    76,    15,    31,
      14,    65,    71,    66,    72,    67,    73,    68,    33,    69,
      15,    15,    31,    15
  };

  const unsigned char
   Parser ::yyr1_[] =
  {
       0,    35,    36,    37,    37,    37,    37,    37,    38,    38,
      39,    39,    39,    39,    40,    41,    42,    43,    43,    43,
      43,    43,    44,    45,    46,    46,    47,    47,    47,    48,
      48,    49,    49,    49,    50,    50,    51,    51,    52,    52,
      52,    53,    53,    54,    54,    55,    55,    55,    56,    57,
      58,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    60,    61,    62,    63,    64,    65,    66,    66,    67,
      67,    68,    68,    69,    69,    70,    70,    70,    70,    71,
      71,    72,    72,    73,    73,    73,    73,    74,    74,    75,
      75,    76,    76
  };

  const unsigned char
   Parser ::yyr2_[] =
  {
       0,     2,     1,     0,     2,     2,     2,     2,     3,     4,
       3,     3,     3,     3,     3,     3,     1,     0,     2,     2,
       2,     2,     3,     3,     1,     2,     0,     1,     2,     1,
       3,     1,     3,     3,     3,     3,     2,     4,     2,     3,
       3,     1,     2,     1,     3,     0,     2,     2,     1,     3,
       4,     1,     2,     2,     3,     1,     2,     2,     3,     1,
       2,     2,     3,     1,     2,     2,     3,     1,     2,     2,
       3,     2,     2,     2,     2,     2,     2,     2,     1,     2,
       1,     2,     1,     2,     1,     2,     3,     3,     4,     1,
       1,     1,     2,     1,     1,     2,     2,     0,     1,     0,
       1,     1,     3
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
  "$accept", "Start", "Program", "ConstValue", "_ConstValue", "NsDecl",
  "NsBegin", "NsEnd", "NsDetail", "EnumDecl", "EnumBegin", "EnumEnd",
  "EnumMember", "_EnumMem", "_EnumVar", "StructDecl", "StyBegin",
  "_StyBegin", "StyEnd", "StyInherit", "StyDetail", "StyMember",
  "StyInner", "VarDecl", "VarDetail", "DefCustom", "DefBool", "DefInt",
  "DefFloat", "DefString", "AssignRefer", "AssignBool", "AssignInt",
  "AssignFloat", "AssignStr", "Array", "BoolValue", "IntValue",
  "FloatValue", "VarDesc", "VarConst", "RefName", YY_NULL
  };

#if YYDEBUG
  const unsigned short int
   Parser ::yyrline_[] =
  {
       0,    91,    91,    94,    95,    96,    97,    98,   102,   103,
     106,   107,   108,   109,   113,   116,   119,   122,   123,   124,
     125,   126,   130,   133,   136,   137,   140,   141,   142,   145,
     146,   148,   149,   150,   154,   155,   158,   159,   162,   163,
     164,   167,   168,   171,   172,   175,   176,   177,   180,   183,
     187,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   216,   218,   220,   222,   224,   227,   229,   230,   232,
     233,   235,   236,   238,   239,   242,   243,   244,   245,   247,
     248,   251,   252,   255,   256,   257,   258,   261,   262,   265,
     266,   270,   271
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
#line 1688 "Parser.cpp" // lalr1.cc:1156
#line 274 "./fb/parser.y" // lalr1.cc:1157


void cfg::Parser::error(const location_type& l, const std::string& m)
{
    driver.Error(l, m);
}
