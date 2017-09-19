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
#line 37 "../fb/parser.y" // lalr1.cc:407

#include "Driver.h"
#include "Scanner.h"
#include "Context.h"

static gcf::Parser::symbol_type yylex(gcf::Driver& driver)
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

#line 11 "../fb/parser.y" // lalr1.cc:473
namespace  gcf  {
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
      case 33: // "identifier"
      case 34: // "true"
      case 35: // "false"
      case 36: // "0"
      case 37: // "0.0f"
      case 38: // "empty"
      case 39: // "desc"
      case 72: // RefName
      case 77: // Variate
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
      case 33: // "identifier"
      case 34: // "true"
      case 35: // "false"
      case 36: // "0"
      case 37: // "0.0f"
      case 38: // "empty"
      case 39: // "desc"
      case 72: // RefName
      case 77: // Variate
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
    #line 29 "../fb/parser.y" // lalr1.cc:726
{
    yyla.location.begin.filename = yyla.location.end.filename = const_cast<std::string*>(&driver.GetScanner()->File());
}

#line 440 "Parser.cpp" // lalr1.cc:726

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
      case 33: // "identifier"
      case 34: // "true"
      case 35: // "false"
      case 36: // "0"
      case 37: // "0.0f"
      case 38: // "empty"
      case 39: // "desc"
      case 72: // RefName
      case 77: // Variate
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
#line 89 "../fb/parser.y" // lalr1.cc:846
    { }
#line 564 "Parser.cpp" // lalr1.cc:846
    break;

  case 3:
#line 92 "../fb/parser.y" // lalr1.cc:846
    { }
#line 570 "Parser.cpp" // lalr1.cc:846
    break;

  case 4:
#line 93 "../fb/parser.y" // lalr1.cc:846
    { }
#line 576 "Parser.cpp" // lalr1.cc:846
    break;

  case 5:
#line 94 "../fb/parser.y" // lalr1.cc:846
    { }
#line 582 "Parser.cpp" // lalr1.cc:846
    break;

  case 6:
#line 95 "../fb/parser.y" // lalr1.cc:846
    { }
#line 588 "Parser.cpp" // lalr1.cc:846
    break;

  case 7:
#line 96 "../fb/parser.y" // lalr1.cc:846
    { }
#line 594 "Parser.cpp" // lalr1.cc:846
    break;

  case 8:
#line 97 "../fb/parser.y" // lalr1.cc:846
    { }
#line 600 "Parser.cpp" // lalr1.cc:846
    break;

  case 9:
#line 101 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariate(yystack_[2].value.as< std::string > ()); }
#line 606 "Parser.cpp" // lalr1.cc:846
    break;

  case 10:
#line 105 "../fb/parser.y" // lalr1.cc:846
    { }
#line 612 "Parser.cpp" // lalr1.cc:846
    break;

  case 11:
#line 108 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnNsBegin(yystack_[1].value.as< std::string > ()); }
#line 618 "Parser.cpp" // lalr1.cc:846
    break;

  case 12:
#line 111 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnNsEnd(); }
#line 624 "Parser.cpp" // lalr1.cc:846
    break;

  case 13:
#line 114 "../fb/parser.y" // lalr1.cc:846
    { }
#line 630 "Parser.cpp" // lalr1.cc:846
    break;

  case 14:
#line 115 "../fb/parser.y" // lalr1.cc:846
    { }
#line 636 "Parser.cpp" // lalr1.cc:846
    break;

  case 15:
#line 116 "../fb/parser.y" // lalr1.cc:846
    { }
#line 642 "Parser.cpp" // lalr1.cc:846
    break;

  case 16:
#line 117 "../fb/parser.y" // lalr1.cc:846
    { }
#line 648 "Parser.cpp" // lalr1.cc:846
    break;

  case 17:
#line 118 "../fb/parser.y" // lalr1.cc:846
    { }
#line 654 "Parser.cpp" // lalr1.cc:846
    break;

  case 18:
#line 119 "../fb/parser.y" // lalr1.cc:846
    { }
#line 660 "Parser.cpp" // lalr1.cc:846
    break;

  case 19:
#line 123 "../fb/parser.y" // lalr1.cc:846
    { }
#line 666 "Parser.cpp" // lalr1.cc:846
    break;

  case 20:
#line 126 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumBegin(yystack_[1].value.as< std::string > ()); }
#line 672 "Parser.cpp" // lalr1.cc:846
    break;

  case 21:
#line 129 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumEnd(); }
#line 678 "Parser.cpp" // lalr1.cc:846
    break;

  case 22:
#line 130 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumEnd(); }
#line 684 "Parser.cpp" // lalr1.cc:846
    break;

  case 23:
#line 133 "../fb/parser.y" // lalr1.cc:846
    { }
#line 690 "Parser.cpp" // lalr1.cc:846
    break;

  case 24:
#line 134 "../fb/parser.y" // lalr1.cc:846
    { }
#line 696 "Parser.cpp" // lalr1.cc:846
    break;

  case 25:
#line 135 "../fb/parser.y" // lalr1.cc:846
    { }
#line 702 "Parser.cpp" // lalr1.cc:846
    break;

  case 26:
#line 138 "../fb/parser.y" // lalr1.cc:846
    { /* empty */ }
#line 708 "Parser.cpp" // lalr1.cc:846
    break;

  case 27:
#line 139 "../fb/parser.y" // lalr1.cc:846
    { /* empty */ }
#line 714 "Parser.cpp" // lalr1.cc:846
    break;

  case 28:
#line 141 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumMember(yystack_[0].value.as< std::string > ()); }
#line 720 "Parser.cpp" // lalr1.cc:846
    break;

  case 29:
#line 142 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumMemberValue(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()); }
#line 726 "Parser.cpp" // lalr1.cc:846
    break;

  case 30:
#line 143 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumMemberValue(yystack_[3].value.as< std::string > (), "-" + yystack_[0].value.as< std::string > ()); }
#line 732 "Parser.cpp" // lalr1.cc:846
    break;

  case 31:
#line 144 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumMemberRefer(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()); }
#line 738 "Parser.cpp" // lalr1.cc:846
    break;

  case 32:
#line 148 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnPredefine(yystack_[1].value.as< std::string > ()); }
#line 744 "Parser.cpp" // lalr1.cc:846
    break;

  case 33:
#line 149 "../fb/parser.y" // lalr1.cc:846
    { }
#line 750 "Parser.cpp" // lalr1.cc:846
    break;

  case 34:
#line 152 "../fb/parser.y" // lalr1.cc:846
    { }
#line 756 "Parser.cpp" // lalr1.cc:846
    break;

  case 35:
#line 153 "../fb/parser.y" // lalr1.cc:846
    { }
#line 762 "Parser.cpp" // lalr1.cc:846
    break;

  case 36:
#line 156 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructBegin(yystack_[0].value.as< std::string > (), CfgCategory::None); }
#line 768 "Parser.cpp" // lalr1.cc:846
    break;

  case 37:
#line 157 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructBegin(yystack_[0].value.as< std::string > (), CfgCategory::Tab); }
#line 774 "Parser.cpp" // lalr1.cc:846
    break;

  case 38:
#line 158 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructBegin(yystack_[0].value.as< std::string > (), CfgCategory::Json); }
#line 780 "Parser.cpp" // lalr1.cc:846
    break;

  case 39:
#line 161 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructEnd(); }
#line 786 "Parser.cpp" // lalr1.cc:846
    break;

  case 40:
#line 162 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructEnd(); }
#line 792 "Parser.cpp" // lalr1.cc:846
    break;

  case 41:
#line 165 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructInherit(yystack_[0].value.as< std::string > ()); }
#line 798 "Parser.cpp" // lalr1.cc:846
    break;

  case 42:
#line 166 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnStructInherit(yystack_[0].value.as< std::string > ()); }
#line 804 "Parser.cpp" // lalr1.cc:846
    break;

  case 43:
#line 169 "../fb/parser.y" // lalr1.cc:846
    { }
#line 810 "Parser.cpp" // lalr1.cc:846
    break;

  case 44:
#line 170 "../fb/parser.y" // lalr1.cc:846
    { }
#line 816 "Parser.cpp" // lalr1.cc:846
    break;

  case 45:
#line 171 "../fb/parser.y" // lalr1.cc:846
    { }
#line 822 "Parser.cpp" // lalr1.cc:846
    break;

  case 46:
#line 174 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariate(yystack_[2].value.as< std::string > ()); }
#line 828 "Parser.cpp" // lalr1.cc:846
    break;

  case 47:
#line 177 "../fb/parser.y" // lalr1.cc:846
    { }
#line 834 "Parser.cpp" // lalr1.cc:846
    break;

  case 48:
#line 181 "../fb/parser.y" // lalr1.cc:846
    { }
#line 840 "Parser.cpp" // lalr1.cc:846
    break;

  case 49:
#line 184 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnCrossBegin(yystack_[1].value.as< std::string > ()); }
#line 846 "Parser.cpp" // lalr1.cc:846
    break;

  case 50:
#line 187 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnCrossEnd(); }
#line 852 "Parser.cpp" // lalr1.cc:846
    break;

  case 51:
#line 188 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnCrossEnd(); }
#line 858 "Parser.cpp" // lalr1.cc:846
    break;

  case 52:
#line 191 "../fb/parser.y" // lalr1.cc:846
    { }
#line 864 "Parser.cpp" // lalr1.cc:846
    break;

  case 53:
#line 192 "../fb/parser.y" // lalr1.cc:846
    { }
#line 870 "Parser.cpp" // lalr1.cc:846
    break;

  case 54:
#line 195 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnFuncBegin(yystack_[1].value.as< std::string > ()); }
#line 876 "Parser.cpp" // lalr1.cc:846
    break;

  case 55:
#line 196 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnFuncBegin(yystack_[1].value.as< std::string > ()); }
#line 882 "Parser.cpp" // lalr1.cc:846
    break;

  case 56:
#line 197 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnFuncBegin(yystack_[1].value.as< std::string > ()); }
#line 888 "Parser.cpp" // lalr1.cc:846
    break;

  case 57:
#line 200 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnFuncEnd(); }
#line 894 "Parser.cpp" // lalr1.cc:846
    break;

  case 58:
#line 201 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnFuncEnd(); }
#line 900 "Parser.cpp" // lalr1.cc:846
    break;

  case 59:
#line 204 "../fb/parser.y" // lalr1.cc:846
    { }
#line 906 "Parser.cpp" // lalr1.cc:846
    break;

  case 60:
#line 205 "../fb/parser.y" // lalr1.cc:846
    { }
#line 912 "Parser.cpp" // lalr1.cc:846
    break;

  case 61:
#line 208 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariate(yystack_[0].value.as< std::string > ()); }
#line 918 "Parser.cpp" // lalr1.cc:846
    break;

  case 62:
#line 209 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.OnVariate(yystack_[0].value.as< std::string > ()); }
#line 924 "Parser.cpp" // lalr1.cc:846
    break;

  case 63:
#line 213 "../fb/parser.y" // lalr1.cc:846
    { }
#line 930 "Parser.cpp" // lalr1.cc:846
    break;

  case 64:
#line 214 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetConst(); }
#line 936 "Parser.cpp" // lalr1.cc:846
    break;

  case 65:
#line 216 "../fb/parser.y" // lalr1.cc:846
    { }
#line 942 "Parser.cpp" // lalr1.cc:846
    break;

  case 66:
#line 217 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetDesc(yystack_[0].value.as< std::string > ()); }
#line 948 "Parser.cpp" // lalr1.cc:846
    break;

  case 67:
#line 221 "../fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 954 "Parser.cpp" // lalr1.cc:846
    break;

  case 68:
#line 222 "../fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[2].value.as< std::string > () + '.' + yystack_[0].value.as< std::string > (); }
#line 960 "Parser.cpp" // lalr1.cc:846
    break;

  case 69:
#line 225 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetType(RawCategory::Bool); }
#line 966 "Parser.cpp" // lalr1.cc:846
    break;

  case 70:
#line 226 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetType(RawCategory::Byte); }
#line 972 "Parser.cpp" // lalr1.cc:846
    break;

  case 71:
#line 227 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetType(RawCategory::Int); }
#line 978 "Parser.cpp" // lalr1.cc:846
    break;

  case 72:
#line 228 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetType(RawCategory::Long); }
#line 984 "Parser.cpp" // lalr1.cc:846
    break;

  case 73:
#line 229 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetType(RawCategory::Float); }
#line 990 "Parser.cpp" // lalr1.cc:846
    break;

  case 74:
#line 230 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetType(RawCategory::Double); }
#line 996 "Parser.cpp" // lalr1.cc:846
    break;

  case 75:
#line 231 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetType(RawCategory::String); }
#line 1002 "Parser.cpp" // lalr1.cc:846
    break;

  case 76:
#line 232 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetType(yystack_[0].value.as< std::string > ()); }
#line 1008 "Parser.cpp" // lalr1.cc:846
    break;

  case 77:
#line 235 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetValue(RawCategory::Bool, yystack_[0].value.as< std::string > ()); }
#line 1014 "Parser.cpp" // lalr1.cc:846
    break;

  case 78:
#line 236 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetValue(RawCategory::Bool, yystack_[0].value.as< std::string > ()); }
#line 1020 "Parser.cpp" // lalr1.cc:846
    break;

  case 79:
#line 237 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetValue(RawCategory::Int, yystack_[0].value.as< std::string > ()); }
#line 1026 "Parser.cpp" // lalr1.cc:846
    break;

  case 80:
#line 238 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetValue(RawCategory::Int, "-" + yystack_[0].value.as< std::string > ()); }
#line 1032 "Parser.cpp" // lalr1.cc:846
    break;

  case 81:
#line 239 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetValue(RawCategory::Float, yystack_[0].value.as< std::string > ()); }
#line 1038 "Parser.cpp" // lalr1.cc:846
    break;

  case 82:
#line 240 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetValue(RawCategory::Float, "-" + yystack_[0].value.as< std::string > ()); }
#line 1044 "Parser.cpp" // lalr1.cc:846
    break;

  case 83:
#line 241 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetValue(RawCategory::String, yystack_[0].value.as< std::string > ()); }
#line 1050 "Parser.cpp" // lalr1.cc:846
    break;

  case 84:
#line 242 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetValue(yystack_[0].value.as< std::string > ()); }
#line 1056 "Parser.cpp" // lalr1.cc:846
    break;

  case 85:
#line 245 "../fb/parser.y" // lalr1.cc:846
    { }
#line 1062 "Parser.cpp" // lalr1.cc:846
    break;

  case 86:
#line 246 "../fb/parser.y" // lalr1.cc:846
    { }
#line 1068 "Parser.cpp" // lalr1.cc:846
    break;

  case 87:
#line 248 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetArray(); }
#line 1074 "Parser.cpp" // lalr1.cc:846
    break;

  case 88:
#line 249 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetArrayLength(yystack_[1].value.as< std::string > ()); }
#line 1080 "Parser.cpp" // lalr1.cc:846
    break;

  case 89:
#line 250 "../fb/parser.y" // lalr1.cc:846
    { CONTEXT.SetArrayRefer(yystack_[1].value.as< std::string > ()); }
#line 1086 "Parser.cpp" // lalr1.cc:846
    break;

  case 90:
#line 253 "../fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > ()= yystack_[0].value.as< std::string > (); }
#line 1092 "Parser.cpp" // lalr1.cc:846
    break;

  case 91:
#line 254 "../fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > ()= yystack_[2].value.as< std::string > (); }
#line 1098 "Parser.cpp" // lalr1.cc:846
    break;

  case 92:
#line 255 "../fb/parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > ()= yystack_[1].value.as< std::string > (); }
#line 1104 "Parser.cpp" // lalr1.cc:846
    break;


#line 1108 "Parser.cpp" // lalr1.cc:846
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


  const signed char  Parser ::yypact_ninf_ = -72;

  const signed char  Parser ::yytable_ninf_ = -3;

  const signed char
   Parser ::yypact_[] =
  {
     -72,    44,     7,   -72,     1,    48,    23,    39,   -72,    47,
      49,   -72,   -72,   -72,   -72,    58,   -72,   -72,     5,   -72,
     -72,   -15,    59,    60,    79,    84,    82,    85,    67,    87,
      86,    95,   -72,    30,    71,   -72,    50,   -72,   -72,   -72,
     -72,   -72,   -72,   -72,   -72,    96,    73,    97,   -72,   -72,
     -72,   -72,   -72,   -72,   -72,   -72,   -72,   -72,   -72,   -72,
     -72,     9,    98,   -72,    58,    99,    74,   -72,   -72,   -72,
     -72,   -15,   -72,    -1,   100,    75,   -72,   -72,   -10,    80,
       6,    76,    78,   -72,    96,   -72,   -72,   -72,   -72,    30,
     104,    88,   -72,   -72,   114,    37,    35,   115,    22,   -72,
     -72,    -3,   105,   -72,   -72,   -72,   -72,    76,   -72,   -72,
     108,   -72,   116,   -72,   -72,   107,    31,   -72,   121,   -72,
      17,   -72,   -72,   -72,   -72,   -72,    96,   -72,   -72,   -72,
     -15,   -72,   -72,   -72,   -72,   -72,   -72
  };

  const unsigned char
   Parser ::yydefact_[] =
  {
       3,     0,    63,     1,     0,     0,     0,     0,    64,     0,
       0,     4,     8,    13,     5,    23,     6,    43,     0,     7,
      52,     0,     0,     0,     0,    36,     0,     0,    63,    28,
       0,    24,    26,    63,     0,    34,     0,    69,    70,    71,
      72,    73,    74,    75,    67,    76,     0,     0,    37,    38,
      20,    32,    11,    49,    12,    14,    17,    10,    15,    16,
      18,     0,    21,    19,    25,    39,     0,    43,    33,    44,
      45,     0,    41,     0,    50,     0,    48,    59,     0,     0,
      90,    65,     0,    29,    31,    22,    27,    40,    36,    63,
       0,     0,    35,    51,     0,     0,     0,     0,     0,    85,
      68,     0,    92,    66,     9,    30,    47,    65,    42,    54,
      57,    53,    60,    61,    87,     0,     0,    55,     0,    86,
       0,    77,    78,    79,    81,    83,    84,    91,    46,    58,
       0,    88,    89,    56,    80,    82,    62
  };

  const signed char
   Parser ::yypgoto_[] =
  {
     -72,   -72,   -72,   101,   102,   -72,   -72,   -72,   103,   -72,
     -72,   -72,   -72,    61,   106,   -32,   -72,    38,   -72,    65,
     -72,   -72,   109,   -72,   -72,   -72,   -72,   -72,   -72,   -72,
     -31,    19,   -58,    92,   -72,    53,    -8,   -71
  };

  const signed char
   Parser ::yydefgoto_[] =
  {
      -1,     1,     2,    11,    12,    13,    57,    28,    14,    15,
      63,    30,    31,    32,    16,    17,    18,    68,    73,    33,
      69,    70,    19,    20,    76,    36,    77,   111,    95,   112,
      21,   104,    45,    46,   127,    98,    99,    47
  };

  const short int
   Parser ::yytable_[] =
  {
      90,    67,    71,    84,    96,   120,    91,    -2,    37,    38,
      39,    40,    41,    42,    43,    92,    34,    82,    44,   101,
      96,    35,    22,    97,   113,     4,     5,     6,     7,     8,
      44,   121,   122,   123,   124,   125,    96,     9,   116,    10,
      79,   110,    44,   126,     3,    83,   132,    65,     4,     5,
     114,    66,     8,   134,   135,   118,    24,    67,    71,   136,
      37,    38,    39,    40,    41,    42,    43,    74,    44,    23,
      44,   115,    25,    37,    38,    39,    40,    41,    42,    43,
      26,    75,    27,    44,    54,     4,     5,     6,     7,     8,
     119,    29,    48,    49,   119,    50,    51,     9,    52,    10,
      61,    53,    64,    62,    72,    79,    80,    88,    94,    81,
      85,    87,    93,   100,   105,   103,   107,   109,   117,    96,
     129,   108,   131,   130,   133,    86,   128,   106,    78,    55,
      56,    58,    89,   102,    59,     0,     0,    60
  };

  const short int
   Parser ::yycheck_[] =
  {
      71,    33,    33,    61,    14,     8,     7,     0,    23,    24,
      25,    26,    27,    28,    29,    16,    11,     8,    33,    13,
      14,    16,    21,    33,    95,    18,    19,    20,    21,    22,
      33,    34,    35,    36,    37,    38,    14,    30,    96,    32,
       9,     4,    33,   101,     0,    36,    15,    17,    18,    19,
      15,    21,    22,    36,    37,    33,    33,    89,    89,   130,
      23,    24,    25,    26,    27,    28,    29,    17,    33,    21,
      33,    36,    33,    23,    24,    25,    26,    27,    28,    29,
      33,    31,    33,    33,    17,    18,    19,    20,    21,    22,
      98,    33,    33,    33,   102,    16,    12,    30,    16,    32,
      13,    16,     7,    17,    33,     9,    33,    33,    33,    12,
      12,    12,    12,    33,    36,    39,    12,     3,     3,    14,
      12,    33,    15,     7,     3,    64,   107,    89,    36,    28,
      28,    28,    67,    80,    28,    -1,    -1,    28
  };

  const unsigned char
   Parser ::yystos_[] =
  {
       0,    41,    42,     0,    18,    19,    20,    21,    22,    30,
      32,    43,    44,    45,    48,    49,    54,    55,    56,    62,
      63,    70,    21,    21,    33,    33,    33,    33,    47,    33,
      51,    52,    53,    59,    11,    16,    65,    23,    24,    25,
      26,    27,    28,    29,    33,    72,    73,    77,    33,    33,
      16,    12,    16,    16,    17,    43,    44,    46,    48,    54,
      62,    13,    17,    50,     7,    17,    21,    55,    57,    60,
      61,    70,    33,    58,    17,    31,    64,    66,    73,     9,
      33,    12,     8,    36,    72,    12,    53,    12,    33,    59,
      77,     7,    16,    12,    33,    68,    14,    33,    75,    76,
      33,    13,    75,    39,    71,    36,    57,    12,    33,     3,
       4,    67,    69,    77,    15,    36,    72,     3,    33,    76,
       8,    34,    35,    36,    37,    38,    72,    74,    71,    12,
       7,    15,    15,     3,    36,    37,    77
  };

  const unsigned char
   Parser ::yyr1_[] =
  {
       0,    40,    41,    42,    42,    42,    42,    42,    42,    43,
      44,    45,    46,    47,    47,    47,    47,    47,    47,    48,
      49,    50,    50,    51,    51,    51,    52,    52,    53,    53,
      53,    53,    54,    54,    55,    55,    56,    56,    56,    57,
      57,    58,    58,    59,    59,    59,    60,    61,    62,    63,
      64,    64,    65,    65,    66,    66,    66,    67,    67,    68,
      68,    69,    69,    70,    70,    71,    71,    72,    72,    73,
      73,    73,    73,    73,    73,    73,    73,    74,    74,    74,
      74,    74,    74,    74,    74,    75,    75,    76,    76,    76,
      77,    77,    77
  };

  const unsigned char
   Parser ::yyr2_[] =
  {
       0,     2,     1,     0,     2,     2,     2,     2,     2,     4,
       3,     3,     1,     0,     2,     2,     2,     2,     2,     3,
       3,     1,     2,     0,     1,     2,     1,     3,     1,     3,
       4,     3,     3,     3,     2,     4,     2,     3,     3,     1,
       2,     1,     3,     0,     2,     2,     4,     3,     3,     3,
       1,     2,     0,     4,     3,     3,     4,     1,     2,     0,
       2,     1,     3,     0,     1,     0,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     2,     1,     1,     1,     2,     2,     3,     3,
       2,     4,     3
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const  Parser ::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\"(\"", "\")\"", "\"*\"",
  "\"+\"", "\",\"", "\"-\"", "\".\"", "\"/\"", "\":\"", "\";\"", "\"=\"",
  "\"[\"", "\"]\"", "\"{\"", "\"}\"", "TAB", "JSON", "ENUM", "STRUCT",
  "CONST", "BOOL", "BYTE", "INT", "LONG", "FLOAT", "DOUBLE", "STRING",
  "NAMESPACE", "VOID", "CROSS", "\"identifier\"", "\"true\"", "\"false\"",
  "\"0\"", "\"0.0f\"", "\"empty\"", "\"desc\"", "$accept", "Start",
  "Program", "ConstValue", "NsDecl", "NsBegin", "NsEnd", "NsDetail",
  "EnumDecl", "EnumBegin", "EnumEnd", "EnumMember", "_EnumMem", "_EnumVar",
  "StructDecl", "StyBegin", "_StyBegin", "StyEnd", "StyInherit",
  "StyDetail", "StyMember", "StyInner", "CrossDecl", "CrossBegin",
  "CrossEnd", "CrossFunc", "FuncBegin", "FuncEnd", "FuncParam",
  "_FuncParam", "VarConst", "VarDesc", "RefName", "Type", "Value", "Array",
  "_Array", "Variate", YY_NULL
  };

#if YYDEBUG
  const unsigned char
   Parser ::yyrline_[] =
  {
       0,    89,    89,    92,    93,    94,    95,    96,    97,   101,
     105,   108,   111,   114,   115,   116,   117,   118,   119,   123,
     126,   129,   130,   133,   134,   135,   138,   139,   141,   142,
     143,   144,   148,   149,   152,   153,   156,   157,   158,   161,
     162,   165,   166,   169,   170,   171,   174,   177,   181,   184,
     187,   188,   191,   192,   195,   196,   197,   200,   201,   204,
     205,   208,   209,   213,   214,   216,   217,   221,   222,   225,
     226,   227,   228,   229,   230,   231,   232,   235,   236,   237,
     238,   239,   240,   241,   242,   245,   246,   248,   249,   250,
     253,   254,   255
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


#line 11 "../fb/parser.y" // lalr1.cc:1156
} //  gcf 
#line 1582 "Parser.cpp" // lalr1.cc:1156
#line 258 "../fb/parser.y" // lalr1.cc:1157


void gcf::Parser::error(const location_type& l, const std::string& m)
{
    driver.Error(m.c_str());
}
