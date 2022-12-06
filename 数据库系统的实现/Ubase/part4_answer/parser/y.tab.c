
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "parse.y"

//
// parser.y: yacc specification for simple SQL-like query language
//

#include <stdlib.h>
#include "heapfile.h"
#include "parse.h"

extern "C" int isatty(int);
extern int yylex();
extern int yywrap();
extern void reset_scanner();
extern void quit();

void yyerror(char *);

extern char *yytext;                    // tokens in string format
static NODE *parse_tree;                // root of parse tree
extern int yylineno;


/* Line 189 of yacc.c  */
#line 96 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     RW_CREATE = 258,
     RW_BUILD = 259,
     RW_REBUILD = 260,
     RW_DROP = 261,
     RW_DESTROY = 262,
     RW_PRINT = 263,
     RW_LOAD = 264,
     RW_HELP = 265,
     RW_QUIT = 266,
     RW_SELECT = 267,
     RW_INTO = 268,
     RW_WHERE = 269,
     RW_INSERT = 270,
     RW_DELETE = 271,
     RW_PRIMARY = 272,
     RW_NUMBUCKETS = 273,
     RW_ALL = 274,
     RW_FROM = 275,
     RW_SET = 276,
     RW_AS = 277,
     RW_TABLE = 278,
     RW_AND = 279,
     RW_OR = 280,
     RW_NOT = 281,
     RW_UPDATE = 282,
     RW_VALUES = 283,
     INT_TYPE = 284,
     REAL_TYPE = 285,
     CHAR_TYPE = 286,
     T_EQ = 287,
     T_LT = 288,
     T_LE = 289,
     T_GT = 290,
     T_GE = 291,
     T_NE = 292,
     T_EOF = 293,
     NOTOKEN = 294,
     T_INT = 295,
     T_REAL = 296,
     T_STRING = 297,
     T_QSTRING = 298,
     T_SHELL_CMD = 299
   };
#endif
/* Tokens.  */
#define RW_CREATE 258
#define RW_BUILD 259
#define RW_REBUILD 260
#define RW_DROP 261
#define RW_DESTROY 262
#define RW_PRINT 263
#define RW_LOAD 264
#define RW_HELP 265
#define RW_QUIT 266
#define RW_SELECT 267
#define RW_INTO 268
#define RW_WHERE 269
#define RW_INSERT 270
#define RW_DELETE 271
#define RW_PRIMARY 272
#define RW_NUMBUCKETS 273
#define RW_ALL 274
#define RW_FROM 275
#define RW_SET 276
#define RW_AS 277
#define RW_TABLE 278
#define RW_AND 279
#define RW_OR 280
#define RW_NOT 281
#define RW_UPDATE 282
#define RW_VALUES 283
#define INT_TYPE 284
#define REAL_TYPE 285
#define CHAR_TYPE 286
#define T_EQ 287
#define T_LT 288
#define T_LE 289
#define T_GT 290
#define T_GE 291
#define T_NE 292
#define T_EOF 293
#define NOTOKEN 294
#define T_INT 295
#define T_REAL 296
#define T_STRING 297
#define T_QSTRING 298
#define T_SHELL_CMD 299




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 23 "parse.y"

  int ival;
  float rval;
  char *sval;
  NODE *n;



/* Line 214 of yacc.c  */
#line 229 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 241 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  50
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   131

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  38
/* YYNRULES -- Number of rules.  */
#define YYNRULES  81
/* YYNRULES -- Number of states.  */
#define YYNSTATES  154

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   299

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      46,    47,    50,     2,    48,     2,    49,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    45,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     6,     8,    10,    12,    14,    16,    18,
      20,    22,    24,    26,    28,    30,    32,    34,    36,    38,
      45,    49,    53,    55,    58,    60,    71,    75,    77,    79,
      83,    85,    87,    92,    98,   106,   110,   116,   122,   125,
     133,   137,   140,   143,   149,   151,   154,   156,   159,   161,
     164,   166,   168,   170,   174,   178,   182,   186,   188,   192,
     196,   198,   200,   204,   206,   210,   214,   216,   219,   222,
     228,   231,   233,   235,   237,   239,   241,   243,   245,   247,
     249,   251
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      52,     0,    -1,    53,    45,    -1,    44,    -1,     1,    -1,
      38,    -1,    54,    -1,    57,    -1,    62,    -1,    63,    -1,
      64,    -1,    65,    -1,    66,    -1,    67,    -1,    68,    -1,
      69,    -1,    70,    -1,    71,    -1,    88,    -1,    12,    79,
      73,    20,    55,    75,    -1,    46,    55,    47,    -1,    56,
      48,    55,    -1,    56,    -1,    87,    87,    -1,    87,    -1,
      15,    13,    87,    46,    58,    47,    28,    46,    60,    47,
      -1,    59,    48,    58,    -1,    59,    -1,    87,    -1,    61,
      48,    60,    -1,    61,    -1,    86,    -1,    16,    20,    87,
      75,    -1,    27,    87,    21,    81,    75,    -1,     3,    23,
      87,    46,    83,    47,    72,    -1,     7,    23,    87,    -1,
       4,    87,    46,    87,    47,    -1,     6,    87,    46,    87,
      47,    -1,     6,    87,    -1,     9,    23,    87,    20,    46,
      43,    47,    -1,     8,    23,    87,    -1,    10,    74,    -1,
      11,    45,    -1,    17,    87,    18,    32,    40,    -1,    88,
      -1,    13,    87,    -1,    88,    -1,    23,    87,    -1,    88,
      -1,    14,    76,    -1,    88,    -1,    77,    -1,    78,    -1,
      80,    85,    86,    -1,    80,    85,    80,    -1,    46,    79,
      47,    -1,    80,    48,    79,    -1,    80,    -1,    87,    49,
      87,    -1,    87,    49,    50,    -1,    87,    -1,    50,    -1,
      82,    48,    81,    -1,    82,    -1,    87,    32,    86,    -1,
      84,    48,    83,    -1,    84,    -1,    87,    29,    -1,    87,
      30,    -1,    87,    31,    46,    86,    47,    -1,    87,    31,
      -1,    33,    -1,    34,    -1,    35,    -1,    36,    -1,    32,
      -1,    37,    -1,    40,    -1,    41,    -1,    43,    -1,    42,
      -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   122,   122,   127,   135,   141,   148,   149,   150,   151,
     152,   153,   154,   158,   159,   160,   161,   162,   163,   170,
     191,   195,   199,   206,   210,   217,   226,   230,   237,   244,
     248,   255,   262,   269,   277,   284,   291,   307,   311,   318,
     324,   331,   338,   345,   349,   356,   360,   367,   371,   378,
     382,   389,   390,   394,   401,   408,   412,   416,   423,   427,
     432,   436,   444,   448,   455,   462,   466,   473,   477,   481,
     485,   492,   496,   500,   504,   508,   512,   519,   523,   527,
     534,   540
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "RW_CREATE", "RW_BUILD", "RW_REBUILD",
  "RW_DROP", "RW_DESTROY", "RW_PRINT", "RW_LOAD", "RW_HELP", "RW_QUIT",
  "RW_SELECT", "RW_INTO", "RW_WHERE", "RW_INSERT", "RW_DELETE",
  "RW_PRIMARY", "RW_NUMBUCKETS", "RW_ALL", "RW_FROM", "RW_SET", "RW_AS",
  "RW_TABLE", "RW_AND", "RW_OR", "RW_NOT", "RW_UPDATE", "RW_VALUES",
  "INT_TYPE", "REAL_TYPE", "CHAR_TYPE", "T_EQ", "T_LT", "T_LE", "T_GT",
  "T_GE", "T_NE", "T_EOF", "NOTOKEN", "T_INT", "T_REAL", "T_STRING",
  "T_QSTRING", "T_SHELL_CMD", "';'", "'('", "')'", "','", "'.'", "'*'",
  "$accept", "start", "command", "query", "table_list", "table", "insert",
  "attrib_list", "attrib", "value_list", "val", "delete", "update",
  "create", "destroy", "build", "drop", "load", "print", "help", "quit",
  "opt_primary_attr", "opt_into_relname", "opt_relname", "opt_where",
  "qual", "selection", "join", "non_mt_qualattr_list", "qualattr",
  "non_mt_attrval_list", "attrval", "non_mt_attrtype_list", "attrtype",
  "op", "value", "string", "nothing", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,    59,    40,    41,    44,    46,
      42
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    51,    52,    52,    52,    52,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    54,
      55,    55,    55,    56,    56,    57,    58,    58,    59,    60,
      60,    61,    62,    63,    64,    65,    66,    67,    67,    68,
      69,    70,    71,    72,    72,    73,    73,    74,    74,    75,
      75,    76,    76,    77,    78,    79,    79,    79,    80,    80,
      80,    80,    81,    81,    82,    83,    83,    84,    84,    84,
      84,    85,    85,    85,    85,    85,    85,    86,    86,    86,
      87,    88
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     6,
       3,     3,     1,     2,     1,    10,     3,     1,     1,     3,
       1,     1,     4,     5,     7,     3,     5,     5,     2,     7,
       3,     2,     2,     5,     1,     2,     1,     2,     1,     2,
       1,     1,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     1,     3,     1,     3,     3,     1,     2,     2,     5,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     4,     0,     0,     0,     0,     0,     0,    81,     0,
       0,     0,     0,     0,     5,     3,     0,     0,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,     0,    80,     0,    38,     0,     0,     0,     0,    41,
      48,    42,     0,    61,    81,    57,    60,     0,     0,     0,
       1,     2,     0,     0,     0,    35,    40,     0,    47,     0,
       0,     0,    46,     0,     0,     0,    81,     0,     0,     0,
       0,     0,    55,    45,     0,    56,    59,    58,     0,     0,
      32,    50,    81,    63,     0,     0,    66,     0,    36,    37,
       0,     0,    81,    22,    24,     0,    27,    28,    49,    51,
      52,     0,    33,     0,     0,    81,     0,    67,    68,    70,
       0,     0,    19,     0,    23,     0,     0,    75,    71,    72,
      73,    74,    76,     0,    62,    77,    78,    79,    64,     0,
      34,    44,    65,     0,    39,    20,    21,     0,    26,    54,
      53,     0,     0,     0,     0,    69,     0,    30,    31,     0,
      25,     0,    43,    29
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    16,    17,    18,    92,    93,    19,    95,    96,   146,
     147,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,   130,    61,    39,    80,    98,    99,   100,    44,    45,
      82,    83,    85,    86,   123,   148,    46,    81
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -85
static const yytype_int8 yypact[] =
{
      11,   -85,   -15,   -12,   -12,    13,    29,    31,    35,    18,
     -37,    57,    22,   -12,   -85,   -85,    83,    39,   -85,   -85,
     -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,
     -85,   -12,   -85,    40,    41,   -12,   -12,   -12,   -12,   -85,
     -85,   -85,   -37,   -85,    72,    42,    44,   -12,   -12,    68,
     -85,   -85,    46,   -12,   -12,   -85,   -85,    74,   -85,    48,
     -12,    76,   -85,   -37,   -11,    51,    84,   -12,   -12,    52,
      54,    56,   -85,   -85,    -5,   -85,   -85,   -85,   -12,    -2,
     -85,   -85,    84,    58,    73,    60,    61,    43,   -85,   -85,
      69,    -5,    84,    63,   -12,    67,    70,   -85,   -85,   -85,
     -85,    45,   -85,   -12,    19,    87,   -12,   -85,   -85,    71,
      75,    77,   -85,    -5,   -85,    88,   -12,   -85,   -85,   -85,
     -85,   -85,   -85,    26,   -85,   -85,   -85,   -85,   -85,   -12,
     -85,   -85,   -85,    19,   -85,   -85,   -85,    79,   -85,   -85,
     -85,    97,    80,    19,    89,   -85,    81,    82,   -85,    91,
     -85,    19,   -85,   -85
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -85,   -85,   -85,   -85,   -84,   -85,   -85,     4,   -85,   -32,
     -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,   -85,
     -85,   -85,   -85,   -85,   -76,   -85,   -85,   -85,   -38,   -77,
      20,   -85,    23,   -85,   -85,   -80,    -3,     3
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -82
static const yytype_int16 yytable[] =
{
      33,    34,   101,    30,    59,    32,   102,   111,    31,    42,
      49,    40,     1,    43,     2,     3,   112,     4,     5,     6,
       7,     8,     9,    10,   128,    75,    11,    12,    52,   136,
      32,    32,    55,    56,    57,    58,    35,    32,    13,    76,
      32,    91,    48,   140,    65,    66,   139,    62,    43,    14,
      69,    70,    36,   142,    37,    15,   -81,    73,    38,   125,
     126,    77,   127,    41,    84,    87,   125,   126,    32,   127,
      47,    94,   107,   108,   109,    97,    43,   117,   118,   119,
     120,   121,   122,    50,    51,    60,    53,    54,    94,    67,
      63,   114,    68,    64,    71,    72,    74,    78,    79,    88,
      84,    89,    90,    87,   129,   104,   103,   105,   131,   106,
      94,   113,   110,    97,   115,   144,   137,   133,   116,   153,
     138,   149,   134,   124,   135,   143,   141,   145,   150,   132,
     151,   152
};

static const yytype_uint8 yycheck[] =
{
       3,     4,    79,     0,    42,    42,    82,    91,    23,    46,
      13,     8,     1,    50,     3,     4,    92,     6,     7,     8,
       9,    10,    11,    12,   104,    63,    15,    16,    31,   113,
      42,    42,    35,    36,    37,    38,    23,    42,    27,    50,
      42,    46,    20,   123,    47,    48,   123,    44,    50,    38,
      53,    54,    23,   133,    23,    44,    45,    60,    23,    40,
      41,    64,    43,    45,    67,    68,    40,    41,    42,    43,
      13,    74,    29,    30,    31,    78,    50,    32,    33,    34,
      35,    36,    37,     0,    45,    13,    46,    46,    91,    21,
      48,    94,    46,    49,    20,    47,    20,    46,    14,    47,
     103,    47,    46,   106,    17,    32,    48,    47,   105,    48,
     113,    48,    43,   116,    47,    18,    28,    46,    48,   151,
     116,    32,    47,   103,    47,    46,   129,    47,    47,   106,
      48,    40
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,     6,     7,     8,     9,    10,    11,
      12,    15,    16,    27,    38,    44,    52,    53,    54,    57,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      88,    23,    42,    87,    87,    23,    23,    23,    23,    74,
      88,    45,    46,    50,    79,    80,    87,    13,    20,    87,
       0,    45,    87,    46,    46,    87,    87,    87,    87,    79,
      13,    73,    88,    48,    49,    87,    87,    21,    46,    87,
      87,    20,    47,    87,    20,    79,    50,    87,    46,    14,
      75,    88,    81,    82,    87,    83,    84,    87,    47,    47,
      46,    46,    55,    56,    87,    58,    59,    87,    76,    77,
      78,    80,    75,    48,    32,    47,    48,    29,    30,    31,
      43,    55,    75,    48,    87,    47,    48,    32,    33,    34,
      35,    36,    37,    85,    81,    40,    41,    43,    86,    17,
      72,    88,    83,    46,    47,    47,    55,    28,    58,    80,
      86,    87,    86,    46,    18,    47,    60,    61,    86,    32,
      47,    48,    40,    60
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 123 "parse.y"
    {
		parse_tree = (yyvsp[(1) - (2)].n);
		YYACCEPT;
	}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 128 "parse.y"
    {
	        if(!isatty(0))
		    puts((yyvsp[(1) - (1)].sval));
		(void)system((yyvsp[(1) - (1)].sval));
		parse_tree = NULL;
		YYACCEPT;
	}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 136 "parse.y"
    {
		reset_scanner();
		parse_tree = NULL;
		YYACCEPT;
	}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 142 "parse.y"
    {
		quit();
	}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 164 "parse.y"
    {
		(yyval.n) = NULL;
	}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 172 "parse.y"
    {
		NODE *where;
		NODE *qualattr_list = replace_alias_in_qualattr_list((yyvsp[(5) - (6)].n), (yyvsp[(2) - (6)].n));
		if (qualattr_list == NULL) { // something wrong in qualattr_list
		  (yyval.n) = NULL;
		}
		else {
		  where = replace_alias_in_condition((yyvsp[(5) - (6)].n), (yyvsp[(6) - (6)].n));
		  if ((where == NULL) && ((yyvsp[(6) - (6)].n) != NULL)) {
		     (yyval.n) = NULL; //something wrong in where condition
		  }
		  else {
		    (yyval.n) = query_node((yyvsp[(3) - (6)].sval), qualattr_list, where);
		  }
		}
	}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 192 "parse.y"
    {
		(yyval.n) = (yyvsp[(2) - (3)].n);
	}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 196 "parse.y"
    {
		(yyval.n) = prepend((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));
	}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 200 "parse.y"
    {
		(yyval.n) = list_node((yyvsp[(1) - (1)].n));
	}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 207 "parse.y"
    {
		(yyval.n) = alias_node((yyvsp[(1) - (2)].sval), (yyvsp[(2) - (2)].sval));
	}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 211 "parse.y"
    {
		(yyval.n) = alias_node((yyvsp[(1) - (1)].sval), NULL);
	}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 218 "parse.y"
    {
		NODE* tmp = merge_attr_value_list((yyvsp[(5) - (10)].n), (yyvsp[(9) - (10)].n));
		if (tmp == NULL) (yyval.n)=NULL;
		else (yyval.n) = insert_node((yyvsp[(3) - (10)].sval), tmp);
	}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 227 "parse.y"
    {
		(yyval.n) = prepend((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));
	}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 231 "parse.y"
    {
		(yyval.n) = list_node((yyvsp[(1) - (1)].n));
	}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 238 "parse.y"
    {
		(yyval.n) = attrval_node((yyvsp[(1) - (1)].sval), NULL);
	}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 245 "parse.y"
    {
		(yyval.n) = prepend((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));
	}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 249 "parse.y"
    {
		(yyval.n) = list_node((yyvsp[(1) - (1)].n));
	}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 256 "parse.y"
    {
		(yyval.n) = (yyvsp[(1) - (1)].n);
	}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 263 "parse.y"
    { 
		(yyval.n) = delete_node((yyvsp[(3) - (4)].sval), (yyvsp[(4) - (4)].n));
	}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 270 "parse.y"
    {
		(yyval.n) = update_node((yyvsp[(2) - (5)].sval),(yyvsp[(4) - (5)].n),(yyvsp[(5) - (5)].n));
	}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 278 "parse.y"
    {
		(yyval.n) = create_node((yyvsp[(3) - (7)].sval), (yyvsp[(5) - (7)].n), (yyvsp[(7) - (7)].n));
	}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 285 "parse.y"
    {
		(yyval.n) = destroy_node((yyvsp[(3) - (3)].sval));
	}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 292 "parse.y"
    {
		(yyval.n) = build_node((yyvsp[(2) - (5)].sval), (yyvsp[(4) - (5)].sval), 0);
	}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 308 "parse.y"
    {
		(yyval.n) = drop_node((yyvsp[(2) - (5)].sval), (yyvsp[(4) - (5)].sval));
	}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 312 "parse.y"
    {
		(yyval.n) = drop_node((yyvsp[(2) - (2)].sval), NULL);
	}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 319 "parse.y"
    {
		(yyval.n) = load_node((yyvsp[(3) - (7)].sval), (yyvsp[(6) - (7)].sval));
	}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 325 "parse.y"
    {
		(yyval.n) = print_node((yyvsp[(3) - (3)].sval));
	}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 332 "parse.y"
    {
		(yyval.n) = help_node((yyvsp[(2) - (2)].sval));
	}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 339 "parse.y"
    {
		quit();
	}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 346 "parse.y"
    {
		(yyval.n) = primattr_node((yyvsp[(2) - (5)].sval), (yyvsp[(5) - (5)].ival));
	}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 350 "parse.y"
    {
		(yyval.n) = NULL;
	}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 357 "parse.y"
    {
		(yyval.sval) = (yyvsp[(2) - (2)].sval);
	}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 361 "parse.y"
    {
		(yyval.sval) = NULL;
	}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 368 "parse.y"
    {
		(yyval.sval) = (yyvsp[(2) - (2)].sval);
	}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 372 "parse.y"
    {
		(yyval.sval) = NULL;
	}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 379 "parse.y"
    {
		(yyval.n) = (yyvsp[(2) - (2)].n);
	}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 383 "parse.y"
    {
		(yyval.n) = NULL;
	}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 395 "parse.y"
    {
		(yyval.n) = select_node((yyvsp[(1) - (3)].n), (yyvsp[(2) - (3)].ival), (yyvsp[(3) - (3)].n));
	}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 402 "parse.y"
    {
		(yyval.n) = join_node((yyvsp[(1) - (3)].n), (yyvsp[(2) - (3)].ival), (yyvsp[(3) - (3)].n));
	}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 409 "parse.y"
    {
		(yyval.n) = (yyvsp[(2) - (3)].n);
	}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 413 "parse.y"
    {
		(yyval.n) = prepend((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));
	}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 417 "parse.y"
    {
		(yyval.n) = list_node((yyvsp[(1) - (1)].n));
	}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 424 "parse.y"
    {
		(yyval.n) = qualattr_node((yyvsp[(1) - (3)].sval), (yyvsp[(3) - (3)].sval));
	}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 428 "parse.y"
    {
                char attr[] = "*";
		(yyval.n) = qualattr_node((yyvsp[(1) - (3)].sval), attr);
	}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 433 "parse.y"
    {
		(yyval.n) = qualattr_node(NULL, (yyvsp[(1) - (1)].sval));
	}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 437 "parse.y"
    {
                char attr[] = "*";
                (yyval.n) = qualattr_node(NULL, attr);
        }
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 445 "parse.y"
    {
		(yyval.n) = prepend((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));
	}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 449 "parse.y"
    {
		(yyval.n) = list_node((yyvsp[(1) - (1)].n));
	}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 456 "parse.y"
    {
		(yyval.n) = attrval_node((yyvsp[(1) - (3)].sval), (yyvsp[(3) - (3)].n));
	}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 463 "parse.y"
    {
		(yyval.n) = prepend((yyvsp[(1) - (3)].n), (yyvsp[(3) - (3)].n));
	}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 467 "parse.y"
    {
		(yyval.n) = list_node((yyvsp[(1) - (1)].n));
	}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 474 "parse.y"
    {
		(yyval.n) = attrtype_node((yyvsp[(1) - (2)].sval), 'i'-128);
	}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 478 "parse.y"
    {
		(yyval.n) = attrtype_node((yyvsp[(1) - (2)].sval), 'f'-128);
	}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 482 "parse.y"
    {
	 	(yyval.n) = attrtype_node((yyvsp[(1) - (5)].sval), (yyvsp[(4) - (5)].n)->u.VALUE.u.ival);
	}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 486 "parse.y"
    {
		(yyval.n) = attrtype_node((yyvsp[(1) - (2)].sval), 2);
	}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 493 "parse.y"
    {
		(yyval.ival) = LT;
	}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 497 "parse.y"
    {
		(yyval.ival) = LTE;
	}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 501 "parse.y"
    {
		(yyval.ival) = GT;
	}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 505 "parse.y"
    {
		(yyval.ival) = GTE;
	}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 509 "parse.y"
    {
		(yyval.ival) = EQ;
	}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 513 "parse.y"
    {
		(yyval.ival) = NE;
	}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 520 "parse.y"
    {
		(yyval.n) = int_node((yyvsp[(1) - (1)].ival));
	}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 524 "parse.y"
    {
		(yyval.n) = float_node((yyvsp[(1) - (1)].rval));
	}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 528 "parse.y"
    {
		(yyval.n) = string_node((yyvsp[(1) - (1)].sval));
	}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 535 "parse.y"
    {
		(yyval.sval) = (yyvsp[(1) - (1)].sval);
	}
    break;



/* Line 1455 of yacc.c  */
#line 2183 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_((char*)"语法错误"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_((char*)"语法错误"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_((char*)"内存耗尽"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 544 "parse.y"


void parse(void)
{
  extern void new_query();
  extern void interp(NODE *);

  for(;;){
    yylineno=1;
    // reset parser and scanner for a new query
    new_query();

    // print a prompt
    printf("%s", PROMPT);
    fflush(stdout);

    // if a query was successfully read, interpret it
    if(yyparse() == 0 && parse_tree != NULL)
      interp(parse_tree);
  }
}

void yyerror(char *s)
{
    printf("SQL错误: %s，出错位置：%d\n", s, yylineno);
    //puts(s);
}

