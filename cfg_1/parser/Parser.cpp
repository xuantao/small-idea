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
#line 38 "../parser.y" // lalr1.cc:407

#include "Driver.h"
#include "Scanner.h"
#include "Context.h"

static Cfg::Parser::symbol_type yylex(Cfg::Driver& driver)
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

#line 11 "../parser.y" // lalr1.cc:473
namespace  Cfg  {
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
      case 25: // "identifier"
      case 26: // "true"
      case 27: // "false"
      case 28: // "0"
      case 29: // "0.0f"
      case 30: // "empty"
      case 55: // BoolValue
      case 56: // IntValue
      case 57: // FloatValue
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
      case 25: // "identifier"
      case 26: // "true"
      case 27: // "false"
      case 28: // "0"
      case 29: // "0.0f"
      case 30: // "empty"
      case 55: // BoolValue
      case 56: // IntValue
      case 57: // FloatValue
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
    #line 30 "../parser.y" // lalr1.cc:726
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
      case 25: // "identifier"
      case 26: // "true"
      case 27: // "false"
      case 28: // "0"
      case 29: // "0.0f"
      case 30: // "empty"
      case 55: // BoolValue
      case 56: // IntValue
      case 57: // FloatValue
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
#line 89 "../parser.y" // lalr1.cc:846
    { }
#line 564 "Parser.cpp" // lalr1.cc:846
    break;

  case 3:
#line 92 "../parser.y" // lalr1.cc:846
    { }
#line 570 "Parser.cpp" // lalr1.cc:846
    break;

  case 4:
#line 93 "../parser.y" // lalr1.cc:846
    { }
#line 576 "Parser.cpp" // lalr1.cc:846
    break;

  case 5:
#line 94 "../parser.y" // lalr1.cc:846
    { }
#line 582 "Parser.cpp" // lalr1.cc:846
    break;

  case 6:
#line 95 "../parser.y" // lalr1.cc:846
    { }
#line 588 "Parser.cpp" // lalr1.cc:846
    break;

  case 7:
#line 96 "../parser.y" // lalr1.cc:846
    { }
#line 594 "Parser.cpp" // lalr1.cc:846
    break;

  case 8:
#line 100 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateEnd(); }
#line 600 "Parser.cpp" // lalr1.cc:846
    break;

  case 9:
#line 103 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(yystack_[0].value.as< std::string > ()); }
#line 606 "Parser.cpp" // lalr1.cc:846
    break;

  case 10:
#line 104 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(yystack_[0].value.as< std::string > ()); }
#line 612 "Parser.cpp" // lalr1.cc:846
    break;

  case 11:
#line 105 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(yystack_[0].value.as< std::string > ()); }
#line 618 "Parser.cpp" // lalr1.cc:846
    break;

  case 12:
#line 106 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(yystack_[0].value.as< std::string > ()); }
#line 624 "Parser.cpp" // lalr1.cc:846
    break;

  case 13:
#line 110 "../parser.y" // lalr1.cc:846
    { }
#line 630 "Parser.cpp" // lalr1.cc:846
    break;

  case 14:
#line 113 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumBegin(yystack_[1].value.as< std::string > ()); }
#line 636 "Parser.cpp" // lalr1.cc:846
    break;

  case 15:
#line 116 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumEnd(); }
#line 642 "Parser.cpp" // lalr1.cc:846
    break;

  case 16:
#line 120 "../parser.y" // lalr1.cc:846
    { }
#line 648 "Parser.cpp" // lalr1.cc:846
    break;

  case 17:
#line 121 "../parser.y" // lalr1.cc:846
    { }
#line 654 "Parser.cpp" // lalr1.cc:846
    break;

  case 18:
#line 122 "../parser.y" // lalr1.cc:846
    { }
#line 660 "Parser.cpp" // lalr1.cc:846
    break;

  case 19:
#line 125 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumMember(yystack_[0].value.as< std::string > ()); }
#line 666 "Parser.cpp" // lalr1.cc:846
    break;

  case 20:
#line 126 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumMember(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()); }
#line 672 "Parser.cpp" // lalr1.cc:846
    break;

  case 21:
#line 127 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumMember(yystack_[0].value.as< std::string > ()); }
#line 678 "Parser.cpp" // lalr1.cc:846
    break;

  case 22:
#line 128 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnEnumMember(yystack_[2].value.as< std::string > (), yystack_[0].value.as< std::string > ()); }
#line 684 "Parser.cpp" // lalr1.cc:846
    break;

  case 23:
#line 132 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnPredefine(yystack_[1].value.as< std::string > ()); }
#line 690 "Parser.cpp" // lalr1.cc:846
    break;

  case 24:
#line 133 "../parser.y" // lalr1.cc:846
    { }
#line 696 "Parser.cpp" // lalr1.cc:846
    break;

  case 25:
#line 136 "../parser.y" // lalr1.cc:846
    { }
#line 702 "Parser.cpp" // lalr1.cc:846
    break;

  case 26:
#line 137 "../parser.y" // lalr1.cc:846
    { }
#line 708 "Parser.cpp" // lalr1.cc:846
    break;

  case 27:
#line 139 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnStructBegin(yystack_[0].value.as< std::string > ()); }
#line 714 "Parser.cpp" // lalr1.cc:846
    break;

  case 28:
#line 142 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnStructEnd(); }
#line 720 "Parser.cpp" // lalr1.cc:846
    break;

  case 29:
#line 146 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnInherit(yystack_[0].value.as< std::string > ()); }
#line 726 "Parser.cpp" // lalr1.cc:846
    break;

  case 30:
#line 147 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnInherit(yystack_[0].value.as< std::string > ()); }
#line 732 "Parser.cpp" // lalr1.cc:846
    break;

  case 31:
#line 150 "../parser.y" // lalr1.cc:846
    { }
#line 738 "Parser.cpp" // lalr1.cc:846
    break;

  case 32:
#line 151 "../parser.y" // lalr1.cc:846
    { }
#line 744 "Parser.cpp" // lalr1.cc:846
    break;

  case 33:
#line 153 "../parser.y" // lalr1.cc:846
    { }
#line 750 "Parser.cpp" // lalr1.cc:846
    break;

  case 34:
#line 154 "../parser.y" // lalr1.cc:846
    { }
#line 756 "Parser.cpp" // lalr1.cc:846
    break;

  case 35:
#line 158 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateEnd(); }
#line 762 "Parser.cpp" // lalr1.cc:846
    break;

  case 36:
#line 161 "../parser.y" // lalr1.cc:846
    { }
#line 768 "Parser.cpp" // lalr1.cc:846
    break;

  case 37:
#line 162 "../parser.y" // lalr1.cc:846
    { }
#line 774 "Parser.cpp" // lalr1.cc:846
    break;

  case 38:
#line 163 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(yystack_[0].value.as< std::string > ()); }
#line 780 "Parser.cpp" // lalr1.cc:846
    break;

  case 39:
#line 164 "../parser.y" // lalr1.cc:846
    { }
#line 786 "Parser.cpp" // lalr1.cc:846
    break;

  case 40:
#line 165 "../parser.y" // lalr1.cc:846
    { }
#line 792 "Parser.cpp" // lalr1.cc:846
    break;

  case 41:
#line 166 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(yystack_[0].value.as< std::string > ()); }
#line 798 "Parser.cpp" // lalr1.cc:846
    break;

  case 42:
#line 167 "../parser.y" // lalr1.cc:846
    { }
#line 804 "Parser.cpp" // lalr1.cc:846
    break;

  case 43:
#line 168 "../parser.y" // lalr1.cc:846
    { }
#line 810 "Parser.cpp" // lalr1.cc:846
    break;

  case 44:
#line 169 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(yystack_[0].value.as< std::string > ()); }
#line 816 "Parser.cpp" // lalr1.cc:846
    break;

  case 45:
#line 170 "../parser.y" // lalr1.cc:846
    { }
#line 822 "Parser.cpp" // lalr1.cc:846
    break;

  case 46:
#line 171 "../parser.y" // lalr1.cc:846
    { }
#line 828 "Parser.cpp" // lalr1.cc:846
    break;

  case 47:
#line 172 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateValue(yystack_[0].value.as< std::string > ()); }
#line 834 "Parser.cpp" // lalr1.cc:846
    break;

  case 48:
#line 173 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateBegin(yystack_[1].value.as< std::string > (), yystack_[0].value.as< std::string > ()); }
#line 840 "Parser.cpp" // lalr1.cc:846
    break;

  case 49:
#line 176 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateBegin("bool", yystack_[0].value.as< std::string > ()); }
#line 846 "Parser.cpp" // lalr1.cc:846
    break;

  case 50:
#line 178 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateBegin("int", yystack_[0].value.as< std::string > ()); }
#line 852 "Parser.cpp" // lalr1.cc:846
    break;

  case 51:
#line 180 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateBegin("float", yystack_[0].value.as< std::string > ()); }
#line 858 "Parser.cpp" // lalr1.cc:846
    break;

  case 52:
#line 182 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateBegin("string", yystack_[0].value.as< std::string > ()); }
#line 864 "Parser.cpp" // lalr1.cc:846
    break;

  case 53:
#line 185 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateArray(); }
#line 870 "Parser.cpp" // lalr1.cc:846
    break;

  case 54:
#line 186 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateArray(yystack_[1].value.as< std::string > ()); }
#line 876 "Parser.cpp" // lalr1.cc:846
    break;

  case 55:
#line 187 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateArray(); }
#line 882 "Parser.cpp" // lalr1.cc:846
    break;

  case 56:
#line 188 "../parser.y" // lalr1.cc:846
    { CONTEXT.OnVariateArray(yystack_[1].value.as< std::string > ()); }
#line 888 "Parser.cpp" // lalr1.cc:846
    break;

  case 57:
#line 191 "../parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 894 "Parser.cpp" // lalr1.cc:846
    break;

  case 58:
#line 192 "../parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 900 "Parser.cpp" // lalr1.cc:846
    break;

  case 59:
#line 195 "../parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 906 "Parser.cpp" // lalr1.cc:846
    break;

  case 60:
#line 196 "../parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 912 "Parser.cpp" // lalr1.cc:846
    break;

  case 61:
#line 199 "../parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 918 "Parser.cpp" // lalr1.cc:846
    break;

  case 62:
#line 200 "../parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 924 "Parser.cpp" // lalr1.cc:846
    break;

  case 63:
#line 201 "../parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 930 "Parser.cpp" // lalr1.cc:846
    break;

  case 64:
#line 202 "../parser.y" // lalr1.cc:846
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 936 "Parser.cpp" // lalr1.cc:846
    break;


#line 940 "Parser.cpp" // lalr1.cc:846
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


  const signed char  Parser ::yypact_ninf_ = -51;

  const signed char  Parser ::yytable_ninf_ = -1;

  const signed char
   Parser ::yypact_[] =
  {
     -51,    16,     6,   -51,   -51,     2,    21,    12,   -51,   -51,
      22,   -51,     7,    -3,    32,    37,    25,    27,    29,    31,
      39,    40,    42,    44,    45,    46,    43,    54,    38,    47,
       7,   -51,    50,    24,    26,    28,    30,    41,   -51,   -51,
     -51,   -51,   -51,   -51,   -51,   -51,   -20,    -5,    -7,    35,
      -5,   -51,   -51,    48,   -51,   -51,   -51,   -51,   -51,   -20,
     -13,    53,    -5,    53,    -7,    53,    49,    53,   -51,    -2,
     -51,   -51,   -51,    52,   -51,   -51,    -9,   -51,   -51,   -51,
     -51,   -51,    55,   -51,   -51,    56,   -11,   -51,   -51,   -51,
      51,   -51,   -51,   -51,   -51,    -5,   -51,   -51,    57,   -51,
     -51,   -51
  };

  const unsigned char
   Parser ::yydefact_[] =
  {
       3,     0,     2,     1,     7,     0,     0,     0,     4,     5,
      16,     6,    31,     0,     0,    27,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    19,     0,    17,     0,     0,
      32,    33,     0,    36,    39,    42,    45,     0,    25,    14,
      23,    49,    50,    51,    52,     8,     0,     0,     0,     0,
       0,    15,    13,    18,    48,    28,    24,    34,    35,     0,
       0,    37,     0,    40,     0,    43,     0,    46,    29,     0,
      57,    58,     9,     0,    59,    10,     0,    61,    62,    11,
      12,    20,    21,    38,    53,     0,     0,    41,    44,    47,
       0,    26,    60,    63,    64,     0,    54,    55,     0,    30,
      22,    56
  };

  const signed char
   Parser ::yypgoto_[] =
  {
     -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,
     -51,   -51,   -51,   -51,   -51,   -51,   -51,    58,   -51,    62,
      63,    67,    68,   -25,    18,   -50,    14
  };

  const signed char
   Parser ::yydefgoto_[] =
  {
      -1,     1,     2,     8,    20,     9,    10,    52,    26,    27,
      11,    12,    13,    56,    69,    29,    30,    31,    32,    33,
      34,    35,    36,    61,    72,    75,    79
  };

  const unsigned char
   Parser ::yytable_[] =
  {
      81,    76,    84,    73,    97,    90,    70,    71,    37,    63,
      65,    67,    87,    38,    91,    85,     3,    98,     4,    93,
      94,    77,    78,    74,     5,     6,     7,    14,    16,    17,
      18,    19,    28,    16,    17,    18,    19,    59,    60,    62,
      60,    64,    60,    66,    60,   100,    15,    25,    39,    40,
      41,    45,    42,    46,    43,    47,    44,    48,    49,    50,
      51,    53,    58,    54,    55,    80,    68,    86,    95,    21,
      22,    96,   101,    82,    23,    24,    99,    83,    88,    89,
      92,     0,     0,     0,     0,     0,     0,     0,    57
  };

  const signed char
   Parser ::yycheck_[] =
  {
      50,     8,    15,     8,    15,     7,    26,    27,    11,    34,
      35,    36,    62,    16,    16,    28,     0,    28,    12,    28,
      29,    28,    29,    28,    18,    19,    20,    25,    21,    22,
      23,    24,    25,    21,    22,    23,    24,    13,    14,    13,
      14,    13,    14,    13,    14,    95,    25,    25,    16,    12,
      25,    12,    25,    13,    25,    13,    25,    13,    13,    13,
      17,     7,    12,    25,    17,    30,    25,    14,    13,     7,
       7,    15,    15,    25,     7,     7,    25,    59,    64,    30,
      28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30
  };

  const unsigned char
   Parser ::yystos_[] =
  {
       0,    32,    33,     0,    12,    18,    19,    20,    34,    36,
      37,    41,    42,    43,    25,    25,    21,    22,    23,    24,
      35,    50,    51,    52,    53,    25,    39,    40,    25,    46,
      47,    48,    49,    50,    51,    52,    53,    11,    16,    16,
      12,    25,    25,    25,    25,    12,    13,    13,    13,    13,
      13,    17,    38,     7,    25,    17,    44,    48,    12,    13,
      14,    54,    13,    54,    13,    54,    13,    54,    25,    45,
      26,    27,    55,     8,    28,    56,     8,    28,    29,    57,
      30,    56,    25,    55,    15,    28,    14,    56,    57,    30,
       7,    16,    28,    28,    29,    13,    15,    15,    28,    25,
      56,    15
  };

  const unsigned char
   Parser ::yyr1_[] =
  {
       0,    31,    32,    33,    33,    33,    33,    33,    34,    35,
      35,    35,    35,    36,    37,    38,    39,    39,    39,    40,
      40,    40,    40,    41,    41,    42,    42,    43,    44,    45,
      45,    46,    46,    47,    47,    48,    49,    49,    49,    49,
      49,    49,    49,    49,    49,    49,    49,    49,    49,    50,
      51,    52,    53,    54,    54,    54,    54,    55,    55,    56,
      56,    57,    57,    57,    57
  };

  const unsigned char
   Parser ::yyr2_[] =
  {
       0,     2,     1,     0,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     1,     0,     1,     2,     1,
       3,     3,     5,     3,     3,     2,     4,     2,     1,     1,
       3,     0,     1,     1,     2,     2,     1,     2,     3,     1,
       2,     3,     1,     2,     3,     1,     2,     3,     2,     2,
       2,     2,     2,     2,     3,     3,     4,     1,     1,     1,
       2,     1,     1,     2,     2
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const  Parser ::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\"(\"", "\")\"", "\"*\"",
  "\"+\"", "\",\"", "\"-\"", "\".\"", "\"/\"", "\":\"", "\";\"", "\"=\"",
  "\"[\"", "\"]\"", "\"{\"", "\"}\"", "ENUM", "STRUCT", "CONST", "BOOL",
  "INT", "FLOAT", "STRING", "\"identifier\"", "\"true\"", "\"false\"",
  "\"0\"", "\"0.0f\"", "\"empty\"", "$accept", "Start", "Program",
  "ConstValue", "_ConstValue", "EnumDecl", "EnumBegin", "EnumEnd",
  "EnumMember", "_EnumMem", "StructDecl", "StructBegin", "_StructBegin",
  "StructEnd", "StructInherit", "StructMember", "_StructMember",
  "VariateDecl", "Variate", "BoolVar", "IntVar", "FloatVar", "StringVar",
  "Array", "BoolValue", "IntValue", "FloatValue", YY_NULL
  };

#if YYDEBUG
  const unsigned char
   Parser ::yyrline_[] =
  {
       0,    89,    89,    92,    93,    94,    95,    96,   100,   103,
     104,   105,   106,   110,   113,   116,   120,   121,   122,   125,
     126,   127,   128,   132,   133,   136,   137,   139,   142,   146,
     147,   150,   151,   153,   154,   158,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   176,
     178,   180,   182,   185,   186,   187,   188,   191,   192,   195,
     196,   199,   200,   201,   202
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


#line 11 "../parser.y" // lalr1.cc:1156
} //  Cfg 
#line 1381 "Parser.cpp" // lalr1.cc:1156
#line 205 "../parser.y" // lalr1.cc:1157


void Cfg::Parser::error(const location_type& l, const std::string& m)
{
    driver.Error(l, m);
}
