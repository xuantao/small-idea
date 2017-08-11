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
      case 76: // BoolValue
      case 77: // IntValue
      case 78: // FloatValue
      case 79: // RefName
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
      case 76: // BoolValue
      case 77: // IntValue
      case 78: // FloatValue
      case 79: // RefName
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

#line 444 "Parser.cpp" // lalr1.cc:726

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
      case 76: // BoolValue
      case 77: // IntValue
      case 78: // FloatValue
      case 79: // RefName
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
#line 570 "Parser.cpp" // lalr1.cc:846
    break;

  case 3:
#line 93 "./fb/parser.y" // lalr1.cc:846
    { }
#line 576 "Parser.cpp" // lalr1.cc:846
    break;

  case 4:
#line 94 "./fb/parser.y" // lalr1.cc:846
    { }
#line 582 "Parser.cpp" // lalr1.cc:846
    break;

  case 5:
#line 95 "./fb/parser.y" // lalr1.cc:846
    { }
#line 588 "Parser.cpp" // lalr1.cc:846
    break;

  case 6:
#line 96 "./fb/parser.y" // lalr1.cc:846
    { }
#line 594 "Parser.cpp" // lalr1.cc:846
    break;

  case 7:
#line 97 "./fb/parser.y" // lalr1.cc:846
    { }
#line 600 "Parser.cpp" // lalr1.cc:846
    break;

  case 8:
#line 101 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateEnd(); }
#line 606 "Parser.cpp" // lalr1.cc:846
    break;

  case 9:
#line 103 "./fb/parser.y" // lalr1.cc:846
    { }
#line 612 "Parser.cpp" // lalr1.cc:846
    break;

  case 10:
#line 104 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateDesc(yystack_[0].value.as< std::string > ()); }
#line 618 "Parser.cpp" // lalr1.cc:846
    break;

  case 11:
#line 106 "./fb/parser.y" // lalr1.cc:846
    { }
#line 624 "Parser.cpp" // lalr1.cc:846
    break;

  case 12:
#line 107 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateConst(); }
#line 630 "Parser.cpp" // lalr1.cc:846
    break;

  case 13:
#line 109 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Bool, yystack_[0].value.as< std::string > ()); }
#line 636 "Parser.cpp" // lalr1.cc:846
    break;

  case 14:
#line 110 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Int, yystack_[0].value.as< std::string > ()); }
#line 642 "Parser.cpp" // lalr1.cc:846
    break;

  case 15:
#line 111 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Float, yystack_[0].value.as< std::string > ()); }
#line 648 "Parser.cpp" // lalr1.cc:846
    break;

  case 16:
#line 112 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::String, yystack_[0].value.as< std::string > ()); }
#line 654 "Parser.cpp" // lalr1.cc:846
    break;

  case 17:
#line 116 "./fb/parser.y" // lalr1.cc:846
    { }
#line 660 "Parser.cpp" // lalr1.cc:846
    break;

  case 18:
#line 119 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnNsBegin(yystack_[1].value.as< std::string > ()); }
#line 666 "Parser.cpp" // lalr1.cc:846
    break;

  case 19:
#line 122 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnNsEnd(); }
#line 672 "Parser.cpp" // lalr1.cc:846
    break;

  case 20:
#line 125 "./fb/parser.y" // lalr1.cc:846
    { }
#line 678 "Parser.cpp" // lalr1.cc:846
    break;

  case 21:
#line 126 "./fb/parser.y" // lalr1.cc:846
    { }
#line 684 "Parser.cpp" // lalr1.cc:846
    break;

  case 22:
#line 127 "./fb/parser.y" // lalr1.cc:846
    { }
#line 690 "Parser.cpp" // lalr1.cc:846
    break;

  case 23:
#line 128 "./fb/parser.y" // lalr1.cc:846
    { }
#line 696 "Parser.cpp" // lalr1.cc:846
    break;

  case 24:
#line 129 "./fb/parser.y" // lalr1.cc:846
    { }
#line 702 "Parser.cpp" // lalr1.cc:846
    break;

  case 25:
#line 133 "./fb/parser.y" // lalr1.cc:846
    { }
#line 708 "Parser.cpp" // lalr1.cc:846
    break;

  case 26:
#line 136 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumBegin(yystack_[1].value.as< std::string > ()); }
#line 714 "Parser.cpp" // lalr1.cc:846
    break;

  case 27:
#line 139 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumEnd(); }
#line 720 "Parser.cpp" // lalr1.cc:846
    break;

  case 28:
#line 140 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumEnd(); }
#line 726 "Parser.cpp" // lalr1.cc:846
    break;

  case 29:
#line 143 "./fb/parser.y" // lalr1.cc:846
    { }
#line 732 "Parser.cpp" // lalr1.cc:846
    break;

  case 30:
#line 144 "./fb/parser.y" // lalr1.cc:846
    { }
#line 738 "Parser.cpp" // lalr1.cc:846
    break;

  case 31:
#line 145 "./fb/parser.y" // lalr1.cc:846
    { }
#line 744 "Parser.cpp" // lalr1.cc:846
    break;

  case 32:
#line 148 "./fb/parser.y" // lalr1.cc:846
    { /* empty */ }
#line 750 "Parser.cpp" // lalr1.cc:846
    break;

  case 33:
#line 149 "./fb/parser.y" // lalr1.cc:846
    { /* empty */ }
#line 756 "Parser.cpp" // lalr1.cc:846
    break;

  case 34:
#line 151 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumMember(yystack_[0].value.as< std::string > ()); }
#line 762 "Parser.cpp" // lalr1.cc:846
    break;

  case 35:
#line 152 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumMemberValue(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()); }
#line 768 "Parser.cpp" // lalr1.cc:846
    break;

  case 36:
#line 153 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumMemberRefer(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()); }
#line 774 "Parser.cpp" // lalr1.cc:846
    break;

  case 37:
#line 157 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnPredefine(yystack_[1].value.as< std::string > ()); }
#line 780 "Parser.cpp" // lalr1.cc:846
    break;

  case 38:
#line 158 "./fb/parser.y" // lalr1.cc:846
    { }
#line 786 "Parser.cpp" // lalr1.cc:846
    break;

  case 39:
#line 161 "./fb/parser.y" // lalr1.cc:846
    { }
#line 792 "Parser.cpp" // lalr1.cc:846
    break;

  case 40:
#line 162 "./fb/parser.y" // lalr1.cc:846
    { }
#line 798 "Parser.cpp" // lalr1.cc:846
    break;

  case 41:
#line 165 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructBegin(yystack_[0].value.as< std::string > (), CfgCategory::None); }
#line 804 "Parser.cpp" // lalr1.cc:846
    break;

  case 42:
#line 166 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructBegin(yystack_[0].value.as< std::string > (), CfgCategory::Tab); }
#line 810 "Parser.cpp" // lalr1.cc:846
    break;

  case 43:
#line 167 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructBegin(yystack_[0].value.as< std::string > (), CfgCategory::Json); }
#line 816 "Parser.cpp" // lalr1.cc:846
    break;

  case 44:
#line 170 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructEnd(); }
#line 822 "Parser.cpp" // lalr1.cc:846
    break;

  case 45:
#line 171 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructEnd(); }
#line 828 "Parser.cpp" // lalr1.cc:846
    break;

  case 46:
#line 174 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructInherit(yystack_[0].value.as< std::string > ()); }
#line 834 "Parser.cpp" // lalr1.cc:846
    break;

  case 47:
#line 175 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructInherit(yystack_[0].value.as< std::string > ()); }
#line 840 "Parser.cpp" // lalr1.cc:846
    break;

  case 48:
#line 178 "./fb/parser.y" // lalr1.cc:846
    { }
#line 846 "Parser.cpp" // lalr1.cc:846
    break;

  case 49:
#line 179 "./fb/parser.y" // lalr1.cc:846
    { }
#line 852 "Parser.cpp" // lalr1.cc:846
    break;

  case 50:
#line 180 "./fb/parser.y" // lalr1.cc:846
    { }
#line 858 "Parser.cpp" // lalr1.cc:846
    break;

  case 51:
#line 183 "./fb/parser.y" // lalr1.cc:846
    { }
#line 864 "Parser.cpp" // lalr1.cc:846
    break;

  case 52:
#line 186 "./fb/parser.y" // lalr1.cc:846
    { }
#line 870 "Parser.cpp" // lalr1.cc:846
    break;

  case 53:
#line 190 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateEnd(); }
#line 876 "Parser.cpp" // lalr1.cc:846
    break;

  case 54:
#line 192 "./fb/parser.y" // lalr1.cc:846
    { }
#line 882 "Parser.cpp" // lalr1.cc:846
    break;

  case 55:
#line 193 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateDesc(yystack_[0].value.as< std::string > ()); }
#line 888 "Parser.cpp" // lalr1.cc:846
    break;

  case 56:
#line 195 "./fb/parser.y" // lalr1.cc:846
    { }
#line 894 "Parser.cpp" // lalr1.cc:846
    break;

  case 57:
#line 196 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateConst(); }
#line 900 "Parser.cpp" // lalr1.cc:846
    break;

  case 58:
#line 199 "./fb/parser.y" // lalr1.cc:846
    { }
#line 906 "Parser.cpp" // lalr1.cc:846
    break;

  case 59:
#line 200 "./fb/parser.y" // lalr1.cc:846
    { }
#line 912 "Parser.cpp" // lalr1.cc:846
    break;

  case 60:
#line 201 "./fb/parser.y" // lalr1.cc:846
    { }
#line 918 "Parser.cpp" // lalr1.cc:846
    break;

  case 61:
#line 202 "./fb/parser.y" // lalr1.cc:846
    { }
#line 924 "Parser.cpp" // lalr1.cc:846
    break;

  case 62:
#line 203 "./fb/parser.y" // lalr1.cc:846
    { }
#line 930 "Parser.cpp" // lalr1.cc:846
    break;

  case 63:
#line 204 "./fb/parser.y" // lalr1.cc:846
    { }
#line 936 "Parser.cpp" // lalr1.cc:846
    break;

  case 64:
#line 205 "./fb/parser.y" // lalr1.cc:846
    { }
#line 942 "Parser.cpp" // lalr1.cc:846
    break;

  case 65:
#line 206 "./fb/parser.y" // lalr1.cc:846
    { }
#line 948 "Parser.cpp" // lalr1.cc:846
    break;

  case 66:
#line 207 "./fb/parser.y" // lalr1.cc:846
    { }
#line 954 "Parser.cpp" // lalr1.cc:846
    break;

  case 67:
#line 208 "./fb/parser.y" // lalr1.cc:846
    { }
#line 960 "Parser.cpp" // lalr1.cc:846
    break;

  case 68:
#line 209 "./fb/parser.y" // lalr1.cc:846
    { }
#line 966 "Parser.cpp" // lalr1.cc:846
    break;

  case 69:
#line 210 "./fb/parser.y" // lalr1.cc:846
    { }
#line 972 "Parser.cpp" // lalr1.cc:846
    break;

  case 70:
#line 211 "./fb/parser.y" // lalr1.cc:846
    { }
#line 978 "Parser.cpp" // lalr1.cc:846
    break;

  case 71:
#line 212 "./fb/parser.y" // lalr1.cc:846
    { }
#line 984 "Parser.cpp" // lalr1.cc:846
    break;

  case 72:
#line 213 "./fb/parser.y" // lalr1.cc:846
    { }
#line 990 "Parser.cpp" // lalr1.cc:846
    break;

  case 73:
#line 214 "./fb/parser.y" // lalr1.cc:846
    { }
#line 996 "Parser.cpp" // lalr1.cc:846
    break;

  case 74:
#line 215 "./fb/parser.y" // lalr1.cc:846
    { }
#line 1002 "Parser.cpp" // lalr1.cc:846
    break;

  case 75:
#line 216 "./fb/parser.y" // lalr1.cc:846
    { }
#line 1008 "Parser.cpp" // lalr1.cc:846
    break;

  case 76:
#line 217 "./fb/parser.y" // lalr1.cc:846
    { }
#line 1014 "Parser.cpp" // lalr1.cc:846
    break;

  case 77:
#line 218 "./fb/parser.y" // lalr1.cc:846
    { }
#line 1020 "Parser.cpp" // lalr1.cc:846
    break;

  case 78:
#line 221 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateBegin(yystack_[1].value.as< std::string > (), yystack_[0].value.as< std::string > ()); }
#line 1026 "Parser.cpp" // lalr1.cc:846
    break;

  case 79:
#line 223 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateBegin(TYPE_BOOL, yystack_[0].value.as< std::string > ()); }
#line 1032 "Parser.cpp" // lalr1.cc:846
    break;

  case 80:
#line 225 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateBegin(TYPE_INT, yystack_[0].value.as< std::string > ()); }
#line 1038 "Parser.cpp" // lalr1.cc:846
    break;

  case 81:
#line 227 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateBegin(TYPE_FLOAT, yystack_[0].value.as< std::string > ()); }
#line 1044 "Parser.cpp" // lalr1.cc:846
    break;

  case 82:
#line 229 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateBegin(TYPE_STRING, yystack_[0].value.as< std::string > ()); }
#line 1050 "Parser.cpp" // lalr1.cc:846
    break;

  case 83:
#line 232 "./fb/parser.y" // lalr1.cc:846
    {  CONTEXT.OnVariateValue(yystack_[0].value.as< std::string > ()); }
#line 1056 "Parser.cpp" // lalr1.cc:846
    break;

  case 84:
#line 234 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Bool, yystack_[0].value.as< std::string > ()); }
#line 1062 "Parser.cpp" // lalr1.cc:846
    break;

  case 85:
#line 235 "./fb/parser.y" // lalr1.cc:846
    { }
#line 1068 "Parser.cpp" // lalr1.cc:846
    break;

  case 86:
#line 237 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Int, yystack_[0].value.as< std::string > ()); }
#line 1074 "Parser.cpp" // lalr1.cc:846
    break;

  case 87:
#line 238 "./fb/parser.y" // lalr1.cc:846
    { }
#line 1080 "Parser.cpp" // lalr1.cc:846
    break;

  case 88:
#line 240 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::Float, yystack_[0].value.as< std::string > ()); }
#line 1086 "Parser.cpp" // lalr1.cc:846
    break;

  case 89:
#line 241 "./fb/parser.y" // lalr1.cc:846
    { }
#line 1092 "Parser.cpp" // lalr1.cc:846
    break;

  case 90:
#line 243 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(RawCategory::String, yystack_[0].value.as< std::string > ()); }
#line 1098 "Parser.cpp" // lalr1.cc:846
    break;

  case 91:
#line 244 "./fb/parser.y" // lalr1.cc:846
    { }
#line 1104 "Parser.cpp" // lalr1.cc:846
    break;

  case 92:
#line 247 "./fb/parser.y" // lalr1.cc:846
    { }
#line 1110 "Parser.cpp" // lalr1.cc:846
    break;

  case 93:
#line 248 "./fb/parser.y" // lalr1.cc:846
    { }
#line 1116 "Parser.cpp" // lalr1.cc:846
    break;

  case 94:
#line 250 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateArray(); }
#line 1122 "Parser.cpp" // lalr1.cc:846
    break;

  case 95:
#line 251 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateArrayLength(yystack_[1].value.as< std::string > ()); }
#line 1128 "Parser.cpp" // lalr1.cc:846
    break;

  case 96:
#line 252 "./fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateArrayRefer(yystack_[1].value.as< std::string > ()); }
#line 1134 "Parser.cpp" // lalr1.cc:846
    break;

  case 97:
#line 255 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1140 "Parser.cpp" // lalr1.cc:846
    break;

  case 98:
#line 256 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1146 "Parser.cpp" // lalr1.cc:846
    break;

  case 99:
#line 259 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1152 "Parser.cpp" // lalr1.cc:846
    break;

  case 100:
#line 260 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = '-' + yystack_[0].value.as< std::string > (); }
#line 1158 "Parser.cpp" // lalr1.cc:846
    break;

  case 101:
#line 263 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1164 "Parser.cpp" // lalr1.cc:846
    break;

  case 102:
#line 264 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1170 "Parser.cpp" // lalr1.cc:846
    break;

  case 103:
#line 265 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = '-' + yystack_[0].value.as< std::string > (); }
#line 1176 "Parser.cpp" // lalr1.cc:846
    break;

  case 104:
#line 266 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = '-' + yystack_[0].value.as< std::string > (); }
#line 1182 "Parser.cpp" // lalr1.cc:846
    break;

  case 105:
#line 270 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1188 "Parser.cpp" // lalr1.cc:846
    break;

  case 106:
#line 271 "./fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[2].value.as< std::string > () + '.' + yystack_[0].value.as< std::string > (); }
#line 1194 "Parser.cpp" // lalr1.cc:846
    break;


#line 1198 "Parser.cpp" // lalr1.cc:846
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


  const signed char  Parser ::yypact_ninf_ = -75;

  const signed char  Parser ::yytable_ninf_ = -1;

  const short int
   Parser ::yypact_[] =
  {
     -75,    17,    90,   -75,    23,    28,    -5,     5,    95,    18,
      29,    42,    67,   104,   -75,   -75,   100,    39,   -75,   -75,
     -75,   105,   -75,   -75,     2,   122,   123,   124,   125,   111,
     112,   126,   129,   131,   -75,   -75,   -75,   -75,   128,   -75,
     -75,    80,   132,   130,   139,   -75,    68,   120,   -75,    30,
       0,     6,   116,   -75,   -75,   -75,   -75,   -75,   -75,   -75,
     -75,   -75,   -75,   -75,   -75,    12,   138,   -75,   105,   140,
     127,    49,   -75,   -75,   -75,   -75,   -75,   -75,   -75,   117,
     141,    41,    55,    65,   113,   115,    25,   -75,     8,   -75,
     -75,   -75,   133,   -75,   -75,    99,   -75,   -75,   -75,   -75,
     -75,   145,   -75,   -75,   -75,   -75,   144,    68,   -75,   -75,
     134,    14,   -75,    41,   -75,    53,   -75,   -75,    55,    12,
     -75,   -75,    65,     4,   -75,   -75,   113,   -12,   -75,   -75,
     115,   135,   -75,   137,   -75,   -75,   -75,   -75,   -75,   -75,
     145,   -75,   142,    32,   -75,   -75,   -75,   -75,   -75,   -75,
     -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75
  };

  const unsigned char
   Parser ::yydefact_[] =
  {
       3,     0,     2,     1,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     8,     9,     0,     7,    20,
       5,    29,     6,    48,     0,     0,     0,     0,     0,     0,
       0,     0,    41,     0,    79,    80,    81,    82,     0,    10,
      11,     0,    34,     0,    30,    32,     0,     0,    39,     0,
       0,     0,     0,    42,    43,    26,    37,    12,    18,    19,
      21,    24,    17,    22,    23,     0,    27,    25,    31,    44,
       0,     0,   105,    48,    38,    49,    50,    51,    53,    54,
       0,    74,    58,    62,    66,    70,     0,    46,     0,    97,
      98,    13,     0,    99,    14,     0,   101,   102,    15,    16,
      35,    36,    28,    33,    45,    41,     0,     0,    55,    56,
       0,     0,    75,    76,    92,     0,    85,    59,    60,     0,
      87,    63,    64,     0,    89,    67,    68,     0,    91,    71,
      72,     0,    78,     0,    40,   100,   103,   104,    57,    52,
      83,    94,     0,     0,    77,    93,    84,    61,    86,    65,
      88,    69,    90,    73,   106,    47,    95,    96
  };

  const short int
   Parser ::yypgoto_[] =
  {
     -75,   -75,   -75,   118,   -75,   -75,   150,   119,   -75,   -75,
     -75,   136,   -75,   -75,   -75,   -75,    93,   143,   -40,   -75,
      59,   -75,    94,   -75,   -75,   -75,   -75,   -75,    97,   -75,
     -45,   -44,   -43,   -41,   -74,    51,    48,    45,    43,    40,
     -42,    57,   -61,    52,   -46
  };

  const short int
   Parser ::yydefgoto_[] =
  {
      -1,     1,     2,    14,    15,    16,    17,    18,    19,    62,
      41,    20,    21,    67,    43,    44,    45,    22,    23,    24,
      74,    88,    46,    75,    76,    77,    78,    79,    80,    81,
      25,    26,    27,    28,   116,   117,   121,   125,   129,   113,
     114,    91,    94,    98,   140
  };

  const unsigned char
   Parser ::yytable_[] =
  {
      86,    82,    83,    84,   100,    85,    73,   112,    92,   120,
     124,   128,    95,    47,    95,   133,    72,     3,    48,   101,
      92,   152,    92,    31,   134,    86,    82,    83,    84,   141,
      85,    93,    72,    32,   131,    96,    97,    96,    97,   144,
      72,   131,    72,    93,    29,    93,    34,   157,   120,    30,
     142,    40,   124,   132,   110,   111,   128,    35,   148,    89,
      90,    86,    82,    83,    84,   143,    85,    73,   115,   111,
      36,   145,     9,    10,    11,    12,   145,    72,   119,   111,
     145,    72,    89,    90,   145,    69,     4,     5,   145,    70,
      71,     9,    10,    11,    12,    37,    72,    59,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,     9,    10,
      11,    12,   118,   122,   126,   130,   123,   111,   127,   111,
     136,   137,    38,    42,    39,    49,    50,    51,    52,    53,
      54,    56,    55,    57,    58,    65,    68,    66,    87,    99,
     102,   108,   104,   109,   131,   105,   138,   156,    33,    60,
      61,   103,    72,   154,   135,   155,   139,   107,   106,   147,
     149,   151,   146,   153,     0,   150,     0,    63,     0,     0,
       0,     0,     0,     0,    64
  };

  const short int
   Parser ::yycheck_[] =
  {
      46,    46,    46,    46,    65,    46,    46,    81,     8,    83,
      84,    85,     8,    11,     8,     7,    28,     0,    16,    65,
       8,    33,     8,    28,    16,    71,    71,    71,    71,    15,
      71,    31,    28,    28,     9,    31,    32,    31,    32,   113,
      28,     9,    28,    31,    21,    31,    28,    15,   122,    21,
     111,    12,   126,    28,    13,    14,   130,    28,   119,    29,
      30,   107,   107,   107,   107,   111,   107,   107,    13,    14,
      28,   113,    23,    24,    25,    26,   118,    28,    13,    14,
     122,    28,    29,    30,   126,    17,    18,    19,   130,    21,
      22,    23,    24,    25,    26,    28,    28,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    23,    24,
      25,    26,    82,    83,    84,    85,    13,    14,    13,    14,
      31,    32,    28,    28,    34,    13,    13,    13,    13,    28,
      28,    12,    16,    12,    16,    13,     7,    17,    28,    33,
      12,    34,    12,    12,     9,    28,    12,    15,     8,    41,
      41,    68,    28,    28,    31,    28,   107,    73,    71,   118,
     122,   126,   115,   130,    -1,   123,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    41
  };

  const unsigned char
   Parser ::yystos_[] =
  {
       0,    36,    37,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    38,    39,    40,    41,    42,    43,
      46,    47,    52,    53,    54,    65,    66,    67,    68,    21,
      21,    28,    28,    41,    28,    28,    28,    28,    28,    34,
      12,    45,    28,    49,    50,    51,    57,    11,    16,    13,
      13,    13,    13,    28,    28,    16,    12,    12,    16,    17,
      38,    42,    44,    46,    52,    13,    17,    48,     7,    17,
      21,    22,    28,    53,    55,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    79,    28,    56,    29,
      30,    76,     8,    31,    77,     8,    31,    32,    78,    33,
      77,    79,    12,    51,    12,    28,    63,    57,    34,    12,
      13,    14,    69,    74,    75,    13,    69,    70,    74,    13,
      69,    71,    74,    13,    69,    72,    74,    13,    69,    73,
      74,     9,    28,     7,    16,    31,    31,    32,    12,    55,
      79,    15,    77,    79,    69,    75,    76,    70,    77,    71,
      78,    72,    33,    73,    28,    28,    15,    15
  };

  const unsigned char
   Parser ::yyr1_[] =
  {
       0,    35,    36,    37,    37,    37,    37,    37,    38,    39,
      39,    40,    40,    41,    41,    41,    41,    42,    43,    44,
      45,    45,    45,    45,    45,    46,    47,    48,    48,    49,
      49,    49,    50,    50,    51,    51,    51,    52,    52,    53,
      53,    54,    54,    54,    55,    55,    56,    56,    57,    57,
      57,    58,    59,    60,    61,    61,    62,    62,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    63,    64,    65,
      66,    67,    68,    69,    70,    70,    71,    71,    72,    72,
      73,    73,    74,    74,    75,    75,    75,    76,    76,    77,
      77,    78,    78,    78,    78,    79,    79
  };

  const unsigned char
   Parser ::yyr2_[] =
  {
       0,     2,     1,     0,     2,     2,     2,     2,     1,     1,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     1,
       0,     2,     2,     2,     2,     3,     3,     1,     2,     0,
       1,     2,     1,     3,     1,     3,     3,     3,     3,     2,
       4,     2,     3,     3,     1,     2,     1,     3,     0,     2,
       2,     1,     3,     1,     1,     2,     2,     3,     1,     2,
       2,     3,     1,     2,     2,     3,     1,     2,     2,     3,
       1,     2,     2,     3,     1,     2,     2,     3,     2,     2,
       2,     2,     2,     2,     2,     1,     2,     1,     2,     1,
       2,     1,     1,     2,     2,     3,     3,     1,     1,     1,
       2,     1,     1,     2,     2,     1,     3
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
  "$accept", "Start", "Program", "ConstValue", "ConstVal_1", "ConstVal_2",
  "ConstVal_3", "NsDecl", "NsBegin", "NsEnd", "NsDetail", "EnumDecl",
  "EnumBegin", "EnumEnd", "EnumMember", "_EnumMem", "_EnumVar",
  "StructDecl", "StyBegin", "_StyBegin", "StyEnd", "StyInherit",
  "StyDetail", "StyMember", "StyInner", "VarDecl", "VarDecl_1",
  "VarDecl_2", "VarDetail", "DefCustom", "DefBool", "DefInt", "DefFloat",
  "DefString", "AssignRefer", "AssignBool", "AssignInt", "AssignFloat",
  "AssignStr", "Array", "ArrayImpl", "BoolValue", "IntValue", "FloatValue",
  "RefName", YY_NULL
  };

#if YYDEBUG
  const unsigned short int
   Parser ::yyrline_[] =
  {
       0,    90,    90,    93,    94,    95,    96,    97,   101,   103,
     104,   106,   107,   109,   110,   111,   112,   116,   119,   122,
     125,   126,   127,   128,   129,   133,   136,   139,   140,   143,
     144,   145,   148,   149,   151,   152,   153,   157,   158,   161,
     162,   165,   166,   167,   170,   171,   174,   175,   178,   179,
     180,   183,   186,   190,   192,   193,   195,   196,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   221,   223,
     225,   227,   229,   232,   234,   235,   237,   238,   240,   241,
     243,   244,   247,   248,   250,   251,   252,   255,   256,   259,
     260,   263,   264,   265,   266,   270,   271
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
#line 1694 "Parser.cpp" // lalr1.cc:1156
#line 274 "./fb/parser.y" // lalr1.cc:1157


void cfg::Parser::error(const location_type& l, const std::string& m)
{
    driver.Error(m.c_str());
}
