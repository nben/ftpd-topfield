/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     A = 258,
     B = 259,
     C = 260,
     E = 261,
     F = 262,
     I = 263,
     L = 264,
     N = 265,
     P = 266,
     R = 267,
     S = 268,
     T = 269,
     SP = 270,
     CRLF = 271,
     COMMA = 272,
     USER = 273,
     PASS = 274,
     ACCT = 275,
     REIN = 276,
     QUIT = 277,
     PORT = 278,
     PASV = 279,
     EPSV = 280,
     TYPE = 281,
     STRU = 282,
     MODE = 283,
     RETR = 284,
     STOR = 285,
     APPE = 286,
     MLFL = 287,
     MAIL = 288,
     MSND = 289,
     MSOM = 290,
     MSAM = 291,
     MRSQ = 292,
     MRCP = 293,
     ALLO = 294,
     REST = 295,
     RNFR = 296,
     RNTO = 297,
     ABOR = 298,
     DELE = 299,
     CWD = 300,
     LIST = 301,
     NLST = 302,
     SITE = 303,
     STAT = 304,
     HELP = 305,
     NOOP = 306,
     MKD = 307,
     RMD = 308,
     PWD = 309,
     CDUP = 310,
     STOU = 311,
     SMNT = 312,
     SYST = 313,
     SIZE = 314,
     MDTM = 315,
     UMASK = 316,
     IDLE = 317,
     TURBO = 318,
     REBOOT = 319,
     LEXERR = 320,
     STRING = 321,
     NUMBER = 322
   };
#endif
/* Tokens.  */
#define A 258
#define B 259
#define C 260
#define E 261
#define F 262
#define I 263
#define L 264
#define N 265
#define P 266
#define R 267
#define S 268
#define T 269
#define SP 270
#define CRLF 271
#define COMMA 272
#define USER 273
#define PASS 274
#define ACCT 275
#define REIN 276
#define QUIT 277
#define PORT 278
#define PASV 279
#define EPSV 280
#define TYPE 281
#define STRU 282
#define MODE 283
#define RETR 284
#define STOR 285
#define APPE 286
#define MLFL 287
#define MAIL 288
#define MSND 289
#define MSOM 290
#define MSAM 291
#define MRSQ 292
#define MRCP 293
#define ALLO 294
#define REST 295
#define RNFR 296
#define RNTO 297
#define ABOR 298
#define DELE 299
#define CWD 300
#define LIST 301
#define NLST 302
#define SITE 303
#define STAT 304
#define HELP 305
#define NOOP 306
#define MKD 307
#define RMD 308
#define PWD 309
#define CDUP 310
#define STOU 311
#define SMNT 312
#define SYST 313
#define SIZE 314
#define MDTM 315
#define UMASK 316
#define IDLE 317
#define TURBO 318
#define REBOOT 319
#define LEXERR 320
#define STRING 321
#define NUMBER 322




/* Copy the first part of user declarations.  */
#line 37 "ftpcmd.y"


#ifndef lint
/*static char sccsid[] = "@(#)ftpcmd.y	8.3 (Berkeley) 4/6/94";*/
#endif /* not lint */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include <netinet/in.h>
#include <arpa/ftp.h>

#include <ctype.h>
#include <errno.h>
#include <pwd.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#include <unistd.h>
#include <limits.h>
#ifdef HAVE_SYS_UTSNAME_H
#include <sys/utsname.h>
#endif
/* Include glob.h last, because it may define "const" which breaks
   system headers on some platforms. */
#include <glob.h>

#include "extern.h"
#include <tf_io.h>
#include <daemon.h>

#if ! defined (NBBY) && defined (CHAR_BIT)
#define NBBY CHAR_BIT
#endif

off_t restart_point;

static char cbuf[512];           /* Command Buffer.  */
static char *fromname;
static int cmd_type;
static int cmd_form;
static int cmd_bytesz;

struct tab
{
  const char	*name;
  short	token;
  short	state;
  short	implemented;	/* 1 if command is implemented */
  const char	*help;
};

extern struct tab cmdtab[];
extern struct tab sitetab[];
static char *copy         __P ((char *));
static void help          __P ((struct tab *, char *));
static struct tab *lookup __P ((struct tab *, char *));
static int yylex          __P ((void));
static void yyerror       __P ((const char *s));



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

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 118 "ftpcmd.y"
typedef union YYSTYPE {
	long long	i;
	char   *s;
} YYSTYPE;
/* Line 196 of yacc.c.  */
#line 305 "y.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 317 "y.tab.c"

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T) && (defined (__STDC__) || defined (__cplusplus))
# include <stddef.h> /* INFRINGES ON USER NAME SPACE */
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

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

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if defined (__STDC__) || defined (__cplusplus)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     define YYINCLUDED_STDLIB_H
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2005 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM ((YYSIZE_T) -1)
#  endif
#  ifdef __cplusplus
extern "C" {
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if (! defined (malloc) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if (! defined (free) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifdef __cplusplus
}
#  endif
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
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
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   227

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  68
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  17
/* YYNRULES -- Number of rules. */
#define YYNRULES  80
/* YYNRULES -- Number of states. */
#define YYNSTATES  234

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   322

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     4,     7,    10,    15,    20,    26,    30,
      36,    40,    45,    50,    55,    60,    69,    76,    83,    90,
      95,   102,   107,   114,   121,   124,   131,   138,   141,   146,
     153,   156,   161,   164,   171,   178,   182,   187,   192,   199,
     205,   213,   219,   227,   234,   239,   247,   254,   257,   264,
     271,   274,   277,   284,   289,   291,   292,   294,   296,   308,
     310,   312,   314,   316,   320,   322,   326,   328,   330,   334,
     337,   339,   341,   343,   345,   347,   349,   351,   353,   355,
     356
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      69,     0,    -1,    -1,    69,    70,    -1,    69,    71,    -1,
      18,    15,    72,    16,    -1,    19,    15,    73,    16,    -1,
      23,    83,    15,    75,    16,    -1,    24,    83,    16,    -1,
      25,    83,    15,    66,    16,    -1,    25,    83,    16,    -1,
      26,    15,    77,    16,    -1,    27,    15,    78,    16,    -1,
      28,    15,    79,    16,    -1,    39,    15,    67,    16,    -1,
      39,    15,    67,    15,    12,    15,    67,    16,    -1,    29,
      83,    84,    15,    80,    16,    -1,    30,    83,    84,    15,
      80,    16,    -1,    31,    83,    84,    15,    80,    16,    -1,
      47,    83,    84,    16,    -1,    47,    83,    84,    15,    66,
      16,    -1,    46,    83,    84,    16,    -1,    46,    83,    84,
      15,    80,    16,    -1,    49,    83,    84,    15,    80,    16,
      -1,    49,    16,    -1,    44,    83,    84,    15,    80,    16,
      -1,    42,    83,    84,    15,    80,    16,    -1,    43,    16,
      -1,    45,    83,    84,    16,    -1,    45,    83,    84,    15,
      80,    16,    -1,    50,    16,    -1,    50,    15,    66,    16,
      -1,    51,    16,    -1,    52,    83,    84,    15,    80,    16,
      -1,    53,    83,    84,    15,    80,    16,    -1,    54,    83,
      16,    -1,    55,    83,    84,    16,    -1,    48,    15,    50,
      16,    -1,    48,    15,    50,    15,    66,    16,    -1,    48,
      15,    61,    83,    16,    -1,    48,    15,    61,    83,    15,
      82,    16,    -1,    48,    15,    63,    83,    16,    -1,    48,
      15,    63,    83,    15,    67,    16,    -1,    48,    15,    64,
      83,    84,    16,    -1,    48,    15,    62,    16,    -1,    48,
      15,    62,    83,    15,    67,    16,    -1,    56,    83,    84,
      15,    80,    16,    -1,    58,    16,    -1,    59,    83,    84,
      15,    80,    16,    -1,    60,    83,    84,    15,    80,    16,
      -1,    22,    16,    -1,     1,    16,    -1,    41,    83,    84,
      15,    80,    16,    -1,    40,    15,    74,    16,    -1,    66,
      -1,    -1,    66,    -1,    67,    -1,    67,    17,    67,    17,
      67,    17,    67,    17,    67,    17,    67,    -1,    10,    -1,
      14,    -1,     5,    -1,     3,    -1,     3,    15,    76,    -1,
       6,    -1,     6,    15,    76,    -1,     8,    -1,     9,    -1,
       9,    15,    74,    -1,     9,    74,    -1,     7,    -1,    12,
      -1,    11,    -1,    13,    -1,     4,    -1,     5,    -1,    81,
      -1,    66,    -1,    67,    -1,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   152,   152,   154,   161,   165,   170,   176,   198,   203,
     209,   214,   249,   261,   273,   277,   281,   288,   295,   302,
     307,   314,   319,   326,   333,   337,   344,   357,   362,   367,
     374,   378,   395,   399,   406,   413,   418,   423,   427,   433,
     443,   458,   465,   472,   478,   484,   500,   507,   553,   584,
     603,   608,   614,   625,   637,   642,   645,   649,   653,   667,
     671,   675,   682,   687,   692,   697,   702,   706,   711,   717,
     725,   729,   733,   740,   744,   748,   755,   792,   796,   824,
     836
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "A", "B", "C", "E", "F", "I", "L", "N",
  "P", "R", "S", "T", "SP", "CRLF", "COMMA", "USER", "PASS", "ACCT",
  "REIN", "QUIT", "PORT", "PASV", "EPSV", "TYPE", "STRU", "MODE", "RETR",
  "STOR", "APPE", "MLFL", "MAIL", "MSND", "MSOM", "MSAM", "MRSQ", "MRCP",
  "ALLO", "REST", "RNFR", "RNTO", "ABOR", "DELE", "CWD", "LIST", "NLST",
  "SITE", "STAT", "HELP", "NOOP", "MKD", "RMD", "PWD", "CDUP", "STOU",
  "SMNT", "SYST", "SIZE", "MDTM", "UMASK", "IDLE", "TURBO", "REBOOT",
  "LEXERR", "STRING", "NUMBER", "$accept", "cmd_list", "cmd", "rcmd",
  "username", "password", "byte_size", "host_port", "form_code",
  "type_code", "struct_code", "mode_code", "pathname", "pathstring",
  "octal_number", "check_login", "open_topfield", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    68,    69,    69,    69,    70,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    70,    71,    71,    72,    73,    73,    74,    75,    76,
      76,    76,    77,    77,    77,    77,    77,    77,    77,    77,
      78,    78,    78,    79,    79,    79,    80,    81,    82,    83,
      84
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     0,     2,     2,     4,     4,     5,     3,     5,
       3,     4,     4,     4,     4,     8,     6,     6,     6,     4,
       6,     4,     6,     6,     2,     6,     6,     2,     4,     6,
       2,     4,     2,     6,     6,     3,     4,     4,     6,     5,
       7,     5,     7,     6,     4,     7,     6,     2,     6,     6,
       2,     2,     6,     4,     1,     0,     1,     1,    11,     1,
       1,     1,     1,     3,     1,     3,     1,     1,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,    79,    79,    79,
       0,     0,     0,    79,    79,    79,     0,     0,    79,    79,
       0,    79,    79,    79,    79,     0,    79,     0,     0,    79,
      79,    79,    79,    79,     0,    79,    79,     3,     4,    51,
       0,    55,    50,     0,     0,     0,     0,     0,     0,    80,
      80,    80,     0,     0,    80,    80,    27,    80,    80,    80,
      80,     0,    24,    80,     0,    30,    32,    80,    80,     0,
      80,    80,    47,    80,    80,    54,     0,    56,     0,     0,
       8,     0,    10,    62,    64,    66,    67,     0,    70,    72,
      71,     0,    74,    75,    73,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,     0,     0,    79,
      79,    79,    79,     0,     0,     0,     0,    35,     0,     0,
       0,     0,     5,     6,     0,     0,     0,     0,     0,     0,
      69,    11,    12,    13,     0,     0,     0,     0,    14,    53,
       0,     0,     0,     0,    28,     0,    21,     0,    19,     0,
      37,     0,    44,     0,     0,    80,     0,    31,     0,     0,
      36,     0,     0,     0,     0,     7,     9,    61,    59,    60,
      63,    65,    68,    77,     0,    76,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    39,     0,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,    16,
      17,    18,     0,    52,    26,    25,    29,    22,    20,    38,
      78,     0,     0,     0,    43,    23,    33,    34,    46,    48,
      49,     0,     0,    40,    45,    42,     0,    15,     0,     0,
       0,     0,     0,    58
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,     1,    37,    38,    76,    78,   101,   125,   170,    87,
      91,    95,   174,   175,   211,    43,    96
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -75
static const short int yypact[] =
{
     -75,    44,   -75,    -2,   -12,     4,     0,   -75,   -75,   -75,
       7,    12,    27,   -75,   -75,   -75,    36,    67,   -75,   -75,
      43,   -75,   -75,   -75,   -75,    86,    48,    18,    92,   -75,
     -75,   -75,   -75,   -75,    97,   -75,   -75,   -75,   -75,   -75,
      54,    56,   -75,   110,   118,    42,   115,    49,    24,   -75,
     -75,   -75,    64,    79,   -75,   -75,   -75,   -75,   -75,   -75,
     -75,    15,   -75,   -75,    81,   -75,   -75,   -75,   -75,   132,
     -75,   -75,   -75,   -75,   -75,   -75,   133,   -75,   134,    84,
     -75,    87,   -75,   137,   139,   -75,   -13,   140,   -75,   -75,
     -75,   141,   -75,   -75,   -75,   142,   144,   145,   146,    65,
     -75,   147,   149,   150,   151,    91,    96,   123,   125,   -75,
     152,   -75,   -75,   154,   155,   157,   158,   -75,   159,   161,
     162,   163,   -75,   -75,   138,   164,   165,     1,     1,    79,
     -75,   -75,   -75,   -75,   101,   101,   101,   167,   -75,   -75,
     101,   101,   101,   101,   -75,   101,   -75,   104,   -75,   108,
     -75,   127,   -75,   168,   129,   -75,   101,   -75,   101,   101,
     -75,   101,   101,   101,    95,   -75,   -75,   -75,   -75,   -75,
     -75,   -75,   -75,   -75,   166,   -75,   169,   170,   172,   173,
     174,   175,   176,   177,   178,   179,   117,   -75,   121,   130,
     -75,   180,   182,   183,   184,   185,   186,   187,   188,   -75,
     -75,   -75,   143,   -75,   -75,   -75,   -75,   -75,   -75,   -75,
     -75,   190,   191,   192,   -75,   -75,   -75,   -75,   -75,   -75,
     -75,   148,   193,   -75,   -75,   -75,   194,   -75,   153,   195,
     156,   196,   160,   -75
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -75,   -75,   -75,   -75,   -75,   -75,   -74,   -75,    76,   -75,
     -75,   -75,   -26,   -75,   -75,    17,   -50
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned char yytable[] =
{
      97,    98,   129,    40,   102,   103,   167,   104,   105,   106,
     107,   168,   130,   113,    39,   169,    42,   115,   116,    41,
     118,   119,    46,   120,   121,    44,    45,    47,    92,    93,
      49,    50,    51,    64,    65,    54,    55,    94,    57,    58,
      59,    60,    48,    63,     2,     3,    67,    68,    69,    70,
      71,    52,    73,    74,   100,   172,    88,    81,    82,    56,
      89,    90,     4,     5,    62,   108,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,   109,   110,   111,   112,
     137,   138,    53,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    61,    34,    35,    36,   191,   143,   144,    66,   176,
     177,   145,   146,    72,   179,   180,   181,   182,    83,   183,
      75,    84,    77,    85,    86,    79,   151,   153,   154,   155,
     192,    99,   193,   194,    80,   195,   196,   197,   147,   148,
     149,   150,   186,   187,   189,   190,   100,   114,   117,   122,
     123,   124,   127,   126,   128,   164,   131,   132,   133,   134,
     135,   136,   198,   139,   140,   141,   142,   173,   152,   156,
     184,   157,   158,   159,   185,   160,   161,   162,   163,   178,
     165,   166,   199,   188,   210,   200,   201,   202,   212,   203,
     204,   205,   206,   207,   208,   209,   214,   213,   215,   216,
     217,   218,   219,   220,   171,   221,   223,   224,   225,   227,
     222,   228,   230,   232,     0,   226,     0,     0,     0,     0,
     229,     0,     0,   231,     0,     0,     0,   233
};

static const short int yycheck[] =
{
      50,    51,    15,    15,    54,    55,     5,    57,    58,    59,
      60,    10,    86,    63,    16,    14,    16,    67,    68,    15,
      70,    71,    15,    73,    74,     8,     9,    15,     4,     5,
      13,    14,    15,    15,    16,    18,    19,    13,    21,    22,
      23,    24,    15,    26,     0,     1,    29,    30,    31,    32,
      33,    15,    35,    36,    67,   129,     7,    15,    16,    16,
      11,    12,    18,    19,    16,    50,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    61,    62,    63,    64,
      15,    16,    15,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    15,    58,    59,    60,   155,    15,    16,    16,   135,
     136,    15,    16,    16,   140,   141,   142,   143,     3,   145,
      66,     6,    66,     8,     9,    15,   109,   110,   111,   112,
     156,    67,   158,   159,    16,   161,   162,   163,    15,    16,
      15,    16,    15,    16,    15,    16,    67,    66,    16,    16,
      16,    67,    15,    66,    15,    17,    16,    16,    16,    15,
      15,    15,    67,    16,    15,    15,    15,    66,    16,    15,
      66,    16,    15,    15,    66,    16,    15,    15,    15,    12,
      16,    16,    16,    15,    67,    16,    16,    15,    67,    16,
      16,    16,    16,    16,    16,    16,    16,    67,    16,    16,
      16,    16,    16,    16,   128,    17,    16,    16,    16,    16,
      67,    17,    17,    17,    -1,    67,    -1,    -1,    -1,    -1,
      67,    -1,    -1,    67,    -1,    -1,    -1,    67
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    69,     0,     1,    18,    19,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    58,    59,    60,    70,    71,    16,
      15,    15,    16,    83,    83,    83,    15,    15,    15,    83,
      83,    83,    15,    15,    83,    83,    16,    83,    83,    83,
      83,    15,    16,    83,    15,    16,    16,    83,    83,    83,
      83,    83,    16,    83,    83,    66,    72,    66,    73,    15,
      16,    15,    16,     3,     6,     8,     9,    77,     7,    11,
      12,    78,     4,     5,    13,    79,    84,    84,    84,    67,
      67,    74,    84,    84,    84,    84,    84,    84,    50,    61,
      62,    63,    64,    84,    66,    84,    84,    16,    84,    84,
      84,    84,    16,    16,    67,    75,    66,    15,    15,    15,
      74,    16,    16,    16,    15,    15,    15,    15,    16,    16,
      15,    15,    15,    15,    16,    15,    16,    15,    16,    15,
      16,    83,    16,    83,    83,    83,    15,    16,    15,    15,
      16,    15,    15,    15,    17,    16,    16,     5,    10,    14,
      76,    76,    74,    66,    80,    81,    80,    80,    12,    80,
      80,    80,    80,    80,    66,    66,    15,    16,    15,    15,
      16,    84,    80,    80,    80,    80,    80,    80,    67,    16,
      16,    16,    15,    16,    16,    16,    16,    16,    16,    16,
      67,    82,    67,    67,    16,    16,    16,    16,    16,    16,
      16,    17,    67,    16,    16,    16,    67,    16,    17,    67,
      17,    67,    17,    67
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
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
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
    while (0)
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
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr,					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname[yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

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
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
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
      size_t yyn = 0;
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

#endif /* YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
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
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

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
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()
    ;
#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

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
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


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
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
        case 3:
#line 155 "ftpcmd.y"
    {
			if (fromname != NULL)
				free (fromname);
			fromname = (char *) 0;
			restart_point = (off_t) 0;
		}
    break;

  case 5:
#line 166 "ftpcmd.y"
    {
			user((yyvsp[-1].s));
			free((yyvsp[-1].s));
		}
    break;

  case 6:
#line 171 "ftpcmd.y"
    {
			pass((yyvsp[-1].s));
			memset ((yyvsp[-1].s), 0, strlen ((yyvsp[-1].s)));
			free((yyvsp[-1].s));
		}
    break;

  case 7:
#line 177 "ftpcmd.y"
    {
			usedefault = 0;
			if (pdata >= 0) {
				(void) close(pdata);
				pdata = -1;
			}
			if ((yyvsp[-3].i)) {
				if (memcmp (&his_addr.sin_addr,
					&data_dest.sin_addr,
					sizeof (data_dest.sin_addr)) == 0 &&
					ntohs (data_dest.sin_port) >
					IPPORT_RESERVED) {
					reply (200, "PORT command sucessful.");
				}
				else {
					memset (&data_dest, 0,
						sizeof (data_dest));
					reply(500, "Illegal PORT Command");
				}
			}
		}
    break;

  case 8:
#line 199 "ftpcmd.y"
    {
			if ((yyvsp[-1].i))
				passive(0);
		}
    break;

  case 9:
#line 204 "ftpcmd.y"
    {
			if ((yyvsp[-3].i) && (yyvsp[-1].s) && strcmp((yyvsp[-1].s), "ALL") == 0) {
				reply(200, "EPSV ALL command successful");
			}
		}
    break;

  case 10:
#line 210 "ftpcmd.y"
    {
			if ((yyvsp[-1].i))
				passive(1);
		}
    break;

  case 11:
#line 215 "ftpcmd.y"
    {
			switch (cmd_type) {

			case TYPE_A:
				if (cmd_form == FORM_N) {
					reply(200, "Type set to A.");
					type = cmd_type;
					form = cmd_form;
				} else
					reply(504, "Form must be N.");
				break;

			case TYPE_E:
				reply(504, "Type E not implemented.");
				break;

			case TYPE_I:
				reply(200, "Type set to I.");
				type = cmd_type;
				break;

			case TYPE_L:
#if defined (NBBY) && NBBY == 8
				if (cmd_bytesz == 8) {
					reply(200,
					    "Type set to L (byte size 8).");
					type = cmd_type;
				} else
					reply(504, "Byte size must be 8.");
#else /* NBBY == 8 */
				UNIMPLEMENTED for NBBY != 8
#endif /* NBBY == 8 */
			}
		}
    break;

  case 12:
#line 250 "ftpcmd.y"
    {
			switch ((yyvsp[-1].i)) {

			case STRU_F:
				reply(200, "STRU F ok.");
				break;

			default:
				reply(504, "Unimplemented STRU type.");
			}
		}
    break;

  case 13:
#line 262 "ftpcmd.y"
    {
			switch ((yyvsp[-1].i)) {

			case MODE_S:
				reply(200, "MODE S ok.");
				break;

			default:
				reply(502, "Unimplemented MODE type.");
			}
		}
    break;

  case 14:
#line 274 "ftpcmd.y"
    {
			reply(202, "ALLO command ignored.");
		}
    break;

  case 15:
#line 278 "ftpcmd.y"
    {
			reply(202, "ALLO command ignored.");
		}
    break;

  case 16:
#line 282 "ftpcmd.y"
    {
			if ((yyvsp[-4].i) && (yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
				retrieve((char *) 0, (yyvsp[-1].s));
			if ((yyvsp[-1].s) != NULL)
				free((yyvsp[-1].s));
		}
    break;

  case 17:
#line 289 "ftpcmd.y"
    {
			if ((yyvsp[-4].i) && (yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
				store((yyvsp[-1].s), "w", 0);
			if ((yyvsp[-1].s) != NULL)
				free((yyvsp[-1].s));
		}
    break;

  case 18:
#line 296 "ftpcmd.y"
    {
			if ((yyvsp[-4].i) && (yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
				store((yyvsp[-1].s), "a", 0);
			if ((yyvsp[-1].s) != NULL)
				free((yyvsp[-1].s));
		}
    break;

  case 19:
#line 303 "ftpcmd.y"
    {
			if ((yyvsp[-2].i) && (yyvsp[-1].i))
				send_file_list(0, 0);
		}
    break;

  case 20:
#line 308 "ftpcmd.y"
    {
			if ((yyvsp[-4].i) && (yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
				send_file_list((yyvsp[-1].s), 0);
			if ((yyvsp[-1].s) != NULL)
				free((yyvsp[-1].s));
		}
    break;

  case 21:
#line 315 "ftpcmd.y"
    {
			if ((yyvsp[-2].i) && (yyvsp[-1].i))
				send_file_list(0, 1);
		}
    break;

  case 22:
#line 320 "ftpcmd.y"
    {
			if ((yyvsp[-4].i) && (yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
				send_file_list((yyvsp[-1].s), 1);
			if ((yyvsp[-1].s) != NULL)
				free((yyvsp[-1].s));
		}
    break;

  case 23:
#line 327 "ftpcmd.y"
    {
			if ((yyvsp[-4].i) && (yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
				statfilecmd((yyvsp[-1].s));
			if ((yyvsp[-1].s) != NULL)
				free((yyvsp[-1].s));
		}
    break;

  case 24:
#line 334 "ftpcmd.y"
    {
			statcmd();
		}
    break;

  case 25:
#line 338 "ftpcmd.y"
    {
			if ((yyvsp[-4].i) && (yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
				delete((yyvsp[-1].s));
			if ((yyvsp[-1].s) != NULL)
				free((yyvsp[-1].s));
		}
    break;

  case 26:
#line 345 "ftpcmd.y"
    {
		    if ((yyvsp[-4].i) && (yyvsp[-3].i)) {
			if (fromname) {
				renamecmd(fromname, (yyvsp[-1].s));
				free(fromname);
				fromname = (char *) 0;
			} else {
				reply(503, "Bad sequence of commands.");
			}
		    }
		    free ((yyvsp[-1].s));
		}
    break;

  case 27:
#line 358 "ftpcmd.y"
    {
			reply(225, "ABOR command successful.");
			abort_transfer(0);
		}
    break;

  case 28:
#line 363 "ftpcmd.y"
    {
			if ((yyvsp[-2].i) && (yyvsp[-1].i))
				cwd(cred.homedir);
		}
    break;

  case 29:
#line 368 "ftpcmd.y"
    {
			if ((yyvsp[-4].i) && (yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
				cwd((yyvsp[-1].s));
			if ((yyvsp[-1].s) != NULL)
				free((yyvsp[-1].s));
		}
    break;

  case 30:
#line 375 "ftpcmd.y"
    {
			help(cmdtab, (char *) 0);
		}
    break;

  case 31:
#line 379 "ftpcmd.y"
    {
			char *cp = (yyvsp[-1].s);

			if (strncasecmp(cp, "SITE", 4) == 0) {
				cp = (yyvsp[-1].s) + 4;
				if (*cp == ' ')
					cp++;
				if (*cp)
					help(sitetab, cp);
				else
					help(sitetab, (char *) 0);
			} else
				help(cmdtab, (yyvsp[-1].s));
			if ((yyvsp[-1].s) != NULL)
			    free ((yyvsp[-1].s));
		}
    break;

  case 32:
#line 396 "ftpcmd.y"
    {
			reply(200, "NOOP command successful.");
		}
    break;

  case 33:
#line 400 "ftpcmd.y"
    {
			if ((yyvsp[-4].i) && (yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
				makedir((yyvsp[-1].s));
			if ((yyvsp[-1].s) != NULL)
				free((yyvsp[-1].s));
		}
    break;

  case 34:
#line 407 "ftpcmd.y"
    {
			if ((yyvsp[-4].i) && (yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
				removedir((yyvsp[-1].s));
			if ((yyvsp[-1].s) != NULL)
				free((yyvsp[-1].s));
		}
    break;

  case 35:
#line 414 "ftpcmd.y"
    {
			if ((yyvsp[-1].i))
				pwd();
		}
    break;

  case 36:
#line 419 "ftpcmd.y"
    {
			if ((yyvsp[-2].i) && (yyvsp[-1].i))
				cwd("..");
		}
    break;

  case 37:
#line 424 "ftpcmd.y"
    {
			help(sitetab, (char *) 0);
		}
    break;

  case 38:
#line 428 "ftpcmd.y"
    {
			help(sitetab, (yyvsp[-1].s));
			if ((yyvsp[-1].s) != NULL)
			    free ((yyvsp[-1].s));
		}
    break;

  case 39:
#line 434 "ftpcmd.y"
    {
			int oldmask;

			if ((yyvsp[-1].i)) {
				oldmask = umask(0);
				(void) umask(oldmask);
				reply(200, "Current UMASK is %03o", oldmask);
			}
		}
    break;

  case 40:
#line 444 "ftpcmd.y"
    {
			int oldmask;

			if ((yyvsp[-3].i)) {
				if (((yyvsp[-1].i) == -1) || ((yyvsp[-1].i) > 0777)) {
					reply(501, "Bad UMASK value");
				} else {
					oldmask = umask((yyvsp[-1].i));
					reply(200,
					    "UMASK set to %03o (was %03o)",
					    (int)(yyvsp[-1].i), oldmask);
				}
			}
		}
    break;

  case 41:
#line 459 "ftpcmd.y"
    {
			if ((yyvsp[-1].i)) {
				global_turbo_mode = !global_turbo_mode;
				reply(200, "TURBO is now %s", (global_turbo_mode) ? "on" : "off");
			}
		}
    break;

  case 42:
#line 466 "ftpcmd.y"
    {
			if ((yyvsp[-3].i)) {
				global_turbo_mode = !!(yyvsp[-1].i);
				reply(200, "TURBO is now %s", (global_turbo_mode) ? "on" : "off");
			}
		}
    break;

  case 43:
#line 473 "ftpcmd.y"
    {
			if ((yyvsp[-2].i)) {
				reboot_topfield();
			}
		}
    break;

  case 44:
#line 479 "ftpcmd.y"
    {
			reply(200,
			    "Current IDLE time limit is %d seconds; max %d",
				timeout, maxtimeout);
		}
    break;

  case 45:
#line 485 "ftpcmd.y"
    {
		    	if ((yyvsp[-3].i)) {
			    if ((yyvsp[-1].i) < 30 || (yyvsp[-1].i) > maxtimeout) {
				reply (501,
			"Maximum IDLE time must be between 30 and %d seconds",
					maxtimeout);
			    } else {
				timeout = (yyvsp[-1].i);
				(void) alarm((unsigned) timeout);
				reply(200,
					"Maximum IDLE time set to %d seconds",
					timeout);
			    }
			}
		}
    break;

  case 46:
#line 501 "ftpcmd.y"
    {
			if ((yyvsp[-4].i) && (yyvsp[-3].i) && (yyvsp[-1].s) != NULL)
				store((yyvsp[-1].s), "w", 1);
			if ((yyvsp[-1].s) != NULL)
				free((yyvsp[-1].s));
		}
    break;

  case 47:
#line 508 "ftpcmd.y"
    {
		        const char *sys_type; /* Official rfc-defined os type.  */
			char *version = 0; /* A more specific type. */

#ifdef HAVE_UNAME
			struct utsname u;
			if (uname (&u) == 0) {
				version =
				  malloc (strlen (u.sysname)
					  + 1 + strlen (u.release) + 1);
				if (version)
					sprintf (version, "%s %s",
						 u.sysname, u.release);
		        }
#else
#ifdef BSD
			version = "BSD";
#endif
#endif

#ifdef unix
			sys_type = "UNIX";
#else
			sys_type = "UNKNOWN";
#endif

			if (version)
				reply(215, "%s Type: L%d Version: %s",
				      sys_type, NBBY, version);
			else
				reply(215, "%s Type: L%d", sys_type, NBBY);

#ifdef HAVE_UNAME
			if (version)
				free (version);
#endif
		}
    break;

  case 48:
#line 554 "ftpcmd.y"
    {
			if ((yyvsp[-4].i) && (yyvsp[-3].i) && (yyvsp[-1].s) != NULL) {
			  if (type == TYPE_L || type == TYPE_I) {
			    off_t size = filesize((yyvsp[-1].s));

			    if (size < 0) {
			      reply(550, "%s: not a plain file.", (yyvsp[-1].s));
			    }
			    else {
			      reply(213, "%llu", (long long)size);
			    }
			  }
			  else {
			    reply(504, "SIZE not implemented for Type %c.", "?AEIL"[type]);
			  }
			  if ((yyvsp[-1].s) != NULL) {
				free((yyvsp[-1].s));
			  }
			}
		}
    break;

  case 49:
#line 585 "ftpcmd.y"
    {
			if ((yyvsp[-4].i) && (yyvsp[-3].i) && (yyvsp[-1].s) != NULL) {
				time_t t = filetime((yyvsp[-1].s));
				if (t < 0) {
					reply(550, "%s: not a plain file.", (yyvsp[-1].s));
				} else {
					struct tm *tm;
					tm = gmtime(&t);
					reply(213,
					    "%04d%02d%02d%02d%02d%02d",
					    1900 + tm->tm_year, tm->tm_mon+1,
					    tm->tm_mday, tm->tm_hour, tm->tm_min,
					    tm->tm_sec);
				}
			}
			if ((yyvsp[-1].s) != NULL)
				free((yyvsp[-1].s));
		}
    break;

  case 50:
#line 604 "ftpcmd.y"
    {
			reply(221, "Goodbye.");
			dologout(0);
		}
    break;

  case 51:
#line 609 "ftpcmd.y"
    {
			yyerrok;
		}
    break;

  case 52:
#line 615 "ftpcmd.y"
    {
			restart_point = (off_t) 0;
			if ((yyvsp[-4].i) && (yyvsp[-3].i) && (yyvsp[-1].s)) {
			    if (fromname != NULL)
				free (fromname);
			    fromname = renamefrom((yyvsp[-1].s));
			}
			if (fromname == (char *) 0 && (yyvsp[-1].s))
			    free((yyvsp[-1].s));
		}
    break;

  case 53:
#line 626 "ftpcmd.y"
    {
		    	if (fromname != NULL)
				free (fromname);
			fromname = (char *) 0;
			restart_point = (yyvsp[-1].i);
			reply(350, "Restarting at %lld. %s", (long long)restart_point,
			    "Send STORE or RETRIEVE to initiate transfer.");
		}
    break;

  case 55:
#line 642 "ftpcmd.y"
    {
			(yyval.s) = (char *)calloc(1, sizeof(char));
		}
    break;

  case 58:
#line 655 "ftpcmd.y"
    {
			char *a, *p;

			a = (char *)&data_dest.sin_addr;
			a[0] = (yyvsp[-10].i); a[1] = (yyvsp[-8].i); a[2] = (yyvsp[-6].i); a[3] = (yyvsp[-4].i);
			p = (char *)&data_dest.sin_port;
			p[0] = (yyvsp[-2].i); p[1] = (yyvsp[0].i);
			data_dest.sin_family = AF_INET;
		}
    break;

  case 59:
#line 668 "ftpcmd.y"
    {
			(yyval.i) = FORM_N;
		}
    break;

  case 60:
#line 672 "ftpcmd.y"
    {
			(yyval.i) = FORM_T;
		}
    break;

  case 61:
#line 676 "ftpcmd.y"
    {
			(yyval.i) = FORM_C;
		}
    break;

  case 62:
#line 683 "ftpcmd.y"
    {
			cmd_type = TYPE_A;
			cmd_form = FORM_N;
		}
    break;

  case 63:
#line 688 "ftpcmd.y"
    {
			cmd_type = TYPE_A;
			cmd_form = (yyvsp[0].i);
		}
    break;

  case 64:
#line 693 "ftpcmd.y"
    {
			cmd_type = TYPE_E;
			cmd_form = FORM_N;
		}
    break;

  case 65:
#line 698 "ftpcmd.y"
    {
			cmd_type = TYPE_E;
			cmd_form = (yyvsp[0].i);
		}
    break;

  case 66:
#line 703 "ftpcmd.y"
    {
			cmd_type = TYPE_I;
		}
    break;

  case 67:
#line 707 "ftpcmd.y"
    {
			cmd_type = TYPE_L;
			cmd_bytesz = NBBY;
		}
    break;

  case 68:
#line 712 "ftpcmd.y"
    {
			cmd_type = TYPE_L;
			cmd_bytesz = (yyvsp[0].i);
		}
    break;

  case 69:
#line 718 "ftpcmd.y"
    {
			cmd_type = TYPE_L;
			cmd_bytesz = (yyvsp[0].i);
		}
    break;

  case 70:
#line 726 "ftpcmd.y"
    {
			(yyval.i) = STRU_F;
		}
    break;

  case 71:
#line 730 "ftpcmd.y"
    {
			(yyval.i) = STRU_R;
		}
    break;

  case 72:
#line 734 "ftpcmd.y"
    {
			(yyval.i) = STRU_P;
		}
    break;

  case 73:
#line 741 "ftpcmd.y"
    {
			(yyval.i) = MODE_S;
		}
    break;

  case 74:
#line 745 "ftpcmd.y"
    {
			(yyval.i) = MODE_B;
		}
    break;

  case 75:
#line 749 "ftpcmd.y"
    {
			(yyval.i) = MODE_C;
		}
    break;

  case 76:
#line 756 "ftpcmd.y"
    {
			/*
			 * Problem: this production is used for all pathname
			 * processing, but only gives a 550 error reply.
			 * This is a valid reply in some cases but not in others.
			 */
			if (cred.logged_in && (yyvsp[0].s) && *(yyvsp[0].s) == '~') {
				glob_t gl;
				int flags = GLOB_NOCHECK;

#ifdef GLOB_BRACE
				flags |= GLOB_BRACE;
#endif
#ifdef GLOB_QUOTE
				flags |= GLOB_QUOTE;
#endif
#ifdef GLOB_TILDE
				flags |= GLOB_TILDE;
#endif

				memset(&gl, 0, sizeof(gl));
				if (glob((yyvsp[0].s), flags, NULL, &gl) ||
				    gl.gl_pathc == 0) {
					reply(550, "not found");
					(yyval.s) = NULL;
				} else {
					(yyval.s) = strdup(gl.gl_pathv[0]);
				}
				globfree(&gl);
				free((yyvsp[0].s));
			} else
				(yyval.s) = (yyvsp[0].s);
		}
    break;

  case 78:
#line 797 "ftpcmd.y"
    {
			int ret, dec, multby, digit;

			/*
			 * Convert a number that was read as decimal number
			 * to what it would be if it had been read as octal.
			 */
			dec = (yyvsp[0].i);
			multby = 1;
			ret = 0;
			while (dec) {
				digit = dec%10;
				if (digit > 7) {
					ret = -1;
					break;
				}
				ret += digit * multby;
				multby *= 8;
				dec /= 10;
			}
			(yyval.i) = ret;
		}
    break;

  case 79:
#line 824 "ftpcmd.y"
    {
			if (cred.logged_in)
				(yyval.i) = 1;
			else {
				reply(530, "Please login with USER and PASS.");
				(yyval.i) = 0;
			}
		}
    break;

  case 80:
#line 836 "ftpcmd.y"
    {
			(yyval.i) = open_topfield(0);
		}
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 2328 "y.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


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
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  int yytype = YYTRANSLATE (yychar);
	  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
	  YYSIZE_T yysize = yysize0;
	  YYSIZE_T yysize1;
	  int yysize_overflow = 0;
	  char *yymsg = 0;
#	  define YYERROR_VERBOSE_ARGS_MAXIMUM 5
	  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int yyx;

#if 0
	  /* This is so xgettext sees the translatable formats that are
	     constructed on the fly.  */
	  YY_("syntax error, unexpected %s");
	  YY_("syntax error, unexpected %s, expecting %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
#endif
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
	  int yychecklim = YYLAST - yyn;
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
		yysize_overflow |= yysize1 < yysize;
		yysize = yysize1;
		yyfmt = yystpcpy (yyfmt, yyprefix);
		yyprefix = yyor;
	      }

	  yyf = YY_(yyformat);
	  yysize1 = yysize + yystrlen (yyf);
	  yysize_overflow |= yysize1 < yysize;
	  yysize = yysize1;

	  if (!yysize_overflow && yysize <= YYSTACK_ALLOC_MAXIMUM)
	    yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg)
	    {
	      /* Avoid sprintf, as that infringes on the user's name space.
		 Don't have undefined behavior even if the translation
		 produced a string with the wrong number of "%s"s.  */
	      char *yyp = yymsg;
	      int yyi = 0;
	      while ((*yyp = *yyf))
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
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    {
	      yyerror (YY_("syntax error"));
	      goto yyexhaustedlab;
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (YY_("syntax error"));
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (0)
     goto yyerrorlab;

yyvsp -= yylen;
  yyssp -= yylen;
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


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK;
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 841 "ftpcmd.y"


#define	CMD	0	/* beginning of command */
#define	ARGS	1	/* expect miscellaneous arguments */
#define	STR1	2	/* expect SP followed by STRING */
#define	STR2	3	/* expect STRING */
#define	OSTR	4	/* optional SP then STRING */
#define	ZSTR1	5	/* SP then optional STRING */
#define	ZSTR2	6	/* optional STRING after SP */
#define	SITECMD	7	/* SITE command */
#define	NSTR	8	/* Number followed by a string */

struct tab cmdtab[] = {		/* In order defined in RFC 765 */
	{ "USER", USER, STR1, 1,	"<sp> username" },
	{ "PASS", PASS, ZSTR1, 1,	"<sp> password" },
	{ "ACCT", ACCT, STR1, 0,	"(specify account)" },
	{ "SMNT", SMNT, ARGS, 0,	"(structure mount)" },
	{ "REIN", REIN, ARGS, 0,	"(reinitialize server state)" },
	{ "QUIT", QUIT, ARGS, 1,	"(terminate service)", },
	{ "PORT", PORT, ARGS, 1,	"<sp> b0, b1, b2, b3, b4" },
	{ "PASV", PASV, ARGS, 1,	"(set server in passive mode)" },
	{ "EPSV", EPSV, OSTR, 1,	"(set server in extended passive mode)" },
	{ "TYPE", TYPE, ARGS, 1,	"<sp> [ A | E | I | L ]" },
	{ "STRU", STRU, ARGS, 1,	"(specify file structure)" },
	{ "MODE", MODE, ARGS, 1,	"(specify transfer mode)" },
	{ "RETR", RETR, STR1, 1,	"<sp> file-name" },
	{ "STOR", STOR, STR1, 1,	"<sp> file-name" },
	{ "APPE", APPE, STR1, 1,	"<sp> file-name" },
	{ "MLFL", MLFL, OSTR, 0,	"(mail file)" },
	{ "MAIL", MAIL, OSTR, 0,	"(mail to user)" },
	{ "MSND", MSND, OSTR, 0,	"(mail send to terminal)" },
	{ "MSOM", MSOM, OSTR, 0,	"(mail send to terminal or mailbox)" },
	{ "MSAM", MSAM, OSTR, 0,	"(mail send to terminal and mailbox)" },
	{ "MRSQ", MRSQ, OSTR, 0,	"(mail recipient scheme question)" },
	{ "MRCP", MRCP, STR1, 0,	"(mail recipient)" },
	{ "ALLO", ALLO, ARGS, 1,	"allocate storage (vacuously)" },
	{ "REST", REST, ARGS, 1,	"<sp> offset (restart command)" },
	{ "RNFR", RNFR, STR1, 1,	"<sp> file-name" },
	{ "RNTO", RNTO, STR1, 1,	"<sp> file-name" },
	{ "ABOR", ABOR, ARGS, 1,	"(abort operation)" },
	{ "DELE", DELE, STR1, 1,	"<sp> file-name" },
	{ "CWD",  CWD,  OSTR, 1,	"[ <sp> directory-name ]" },
	{ "XCWD", CWD,	OSTR, 1,	"[ <sp> directory-name ]" },
	{ "LIST", LIST, OSTR, 1,	"[ <sp> path-name ]" },
	{ "NLST", NLST, OSTR, 1,	"[ <sp> path-name ]" },
	{ "SITE", SITE, SITECMD, 1,	"site-cmd [ <sp> arguments ]" },
	{ "SYST", SYST, ARGS, 1,	"(get type of operating system)" },
	{ "STAT", STAT, OSTR, 1,	"[ <sp> path-name ]" },
	{ "HELP", HELP, OSTR, 1,	"[ <sp> <string> ]" },
	{ "NOOP", NOOP, ARGS, 1,	"" },
	{ "MKD",  MKD,  STR1, 1,	"<sp> path-name" },
	{ "XMKD", MKD,  STR1, 1,	"<sp> path-name" },
	{ "RMD",  RMD,  STR1, 1,	"<sp> path-name" },
	{ "XRMD", RMD,  STR1, 1,	"<sp> path-name" },
	{ "PWD",  PWD,  ARGS, 1,	"(return current directory)" },
	{ "XPWD", PWD,  ARGS, 1,	"(return current directory)" },
	{ "CDUP", CDUP, ARGS, 1,	"(change to parent directory)" },
	{ "XCUP", CDUP, ARGS, 1,	"(change to parent directory)" },
	{ "STOU", STOU, STR1, 1,	"<sp> file-name" },
	{ "SIZE", SIZE, OSTR, 1,	"<sp> path-name" },
	{ "MDTM", MDTM, OSTR, 1,	"<sp> path-name" },
	{ NULL,   0,    0,    0,	0 }
};

struct tab sitetab[] = {
	{ "UMASK", UMASK, ARGS, 1,	"[ <sp> umask ]" },
	{ "IDLE", IDLE, ARGS, 1,	"[ <sp> maximum-idle-time ]" },
	{ "TURBO", TURBO, ARGS, 1,	"[ <sp> 0|1 ]" },
	{ "REBOOT", REBOOT, ARGS, 1,	"(reboot topfield)" },
	{ "HELP", HELP, OSTR, 1,	"[ <sp> <string> ]" },
	{ NULL,   0,    0,    0,	0 }
};

static struct tab *
lookup(p, cmd)
	struct tab *p;
	char *cmd;
{

	for (; p->name != NULL; p++)
		if (strcmp(cmd, p->name) == 0)
			return (p);
	return (0);
}

#include <arpa/telnet.h>

/*
 * getline - a hacked up version of fgets to ignore TELNET escape codes.
 */
char *
telnet_fgets(char *s, int n, FILE *iop)
{
	int c;
	register char *cs;

	cs = s;
/* tmpline may contain saved command from urgent mode interruption */
	for (c = 0; tmpline[c] != '\0' && --n > 0; ++c) {
		*cs++ = tmpline[c];
		if (tmpline[c] == '\n') {
			*cs++ = '\0';
			if (debug)
				syslog(LOG_DEBUG, "command: %s", s);
			tmpline[0] = '\0';
			return(s);
		}
		if (c == 0)
			tmpline[0] = '\0';
	}
	while ((c = getc(iop)) != EOF) {
		c &= 0377;
		if (c == IAC) {
		    if ((c = getc(iop)) != EOF) {
			c &= 0377;
			switch (c) {
			case WILL:
			case WONT:
				c = getc(iop);
				printf("%c%c%c", IAC, DONT, 0377&c);
				(void) fflush(stdout);
				continue;
			case DO:
			case DONT:
				c = getc(iop);
				printf("%c%c%c", IAC, WONT, 0377&c);
				(void) fflush(stdout);
				continue;
			case IAC:
				break;
			default:
				continue;	/* ignore command */
			}
		    }
		}
		*cs++ = c;
		if (--n <= 0 || c == '\n')
			break;
	}
	if (c == EOF && cs == s)
	    return (NULL);
	*cs++ = '\0';
	if (debug) {
		if (!cred.guest && strncasecmp("pass ", s, 5) == 0) {
			/* Don't syslog passwords */
			syslog(LOG_DEBUG, "command: %.5s ???", s);
		} else {
			register char *cp;
			register int len;

			/* Don't syslog trailing CR-LF */
			len = strlen(s);
			cp = s + len - 1;
			while (cp >= s && (*cp == '\n' || *cp == '\r')) {
				--cp;
				--len;
			}
			syslog(LOG_DEBUG, "command: %.*s", len, s);
		}
	}
	return (s);
}

void
toolong(int signo)
{
  (void)signo;
	reply(421,
	    "Timeout (%d seconds): closing control connection.", timeout);
	if (logging)
		syslog(LOG_INFO, "User %s timed out after %d seconds",
		    (cred.name ? cred.name : "unknown"), timeout);
	dologout(1);
}

static int
yylex()
{
	static int cpos, state;
	char *cp, *cp2;
	struct tab *p;
	int n;
	char c;

	for (;;) {
		switch (state) {

		case CMD:
			(void) signal(SIGALRM, toolong);
			(void) alarm((unsigned) timeout);
			/* Indicate that we are idle and can lose the connection to the topfield */
			ftp_idle();

			if (telnet_fgets(cbuf, sizeof(cbuf)-1, stdin) == NULL) {
				if (feof(stdin)) {
					reply(221, "You could at least say goodbye.");
					dologout(0);
				}
				/* Not EOF. Perhaps we just received a SIGHUP */
				continue;
			}
			(void) alarm(0);
#ifdef HAVE_SETPROCTITLE
			if (strncasecmp(cbuf, "PASS", 4) != NULL)
				setproctitle("%s: %s", proctitle, cbuf);
#endif /* HAVE_SETPROCTITLE */
			if ((cp = strchr(cbuf, '\r'))) {
				*cp++ = '\n';
				*cp = '\0';
			}
			if ((cp = strpbrk(cbuf, " \n")))
				cpos = cp - cbuf;
			if (cpos == 0)
				cpos = 4;
			c = cbuf[cpos];
			cbuf[cpos] = '\0';
			upper(cbuf);
			p = lookup(cmdtab, cbuf);
			cbuf[cpos] = c;
			if (p != 0) {
				if (p->implemented == 0) {
					nack(p->name);
					longjmp(errcatch,0);
					/* NOTREACHED */
				}
				state = p->state;
				yylval.s = (char *)p->name;
				return (p->token);
			}
			break;

		case SITECMD:
			if (cbuf[cpos] == ' ') {
				cpos++;
				return (SP);
			}
			cp = &cbuf[cpos];
			if ((cp2 = strpbrk(cp, " \n")))
				cpos = cp2 - cbuf;
			c = cbuf[cpos];
			cbuf[cpos] = '\0';
			upper(cp);
			p = lookup(sitetab, cp);
			cbuf[cpos] = c;
			if (p != 0) {
				if (p->implemented == 0) {
					state = CMD;
					nack(p->name);
					longjmp(errcatch,0);
					/* NOTREACHED */
				}
				state = p->state;
				yylval.s = (char *)p->name;
				return (p->token);
			}
			state = CMD;
			break;

		case OSTR:
			if (cbuf[cpos] == '\n') {
				state = CMD;
				return (CRLF);
			}
			/* FALLTHROUGH */

		case STR1:
		case ZSTR1:
		dostr1:
			if (cbuf[cpos] == ' ') {
				cpos++;
				state = state == OSTR ? STR2 : state + 1;
				return (SP);
			}
			break;

		case ZSTR2:
			if (cbuf[cpos] == '\n') {
				state = CMD;
				return (CRLF);
			}
			/* FALLTHROUGH */

		case STR2:
			cp = &cbuf[cpos];
			n = strlen(cp);
			cpos += n - 1;
			/*
			 * Make sure the string is nonempty and \n terminated.
			 */
			if (n > 1 && cbuf[cpos] == '\n') {
				cbuf[cpos] = '\0';
				yylval.s = copy(cp);
				cbuf[cpos] = '\n';
				state = ARGS;
				return (STRING);
			}
			break;

		case NSTR:
			if (cbuf[cpos] == ' ') {
				cpos++;
				return (SP);
			}
			if (isdigit(cbuf[cpos])) {
				cp = &cbuf[cpos];
				while (isdigit(cbuf[++cpos]))
					;
				c = cbuf[cpos];
				cbuf[cpos] = '\0';
				yylval.i = strtoll(cp, 0, 10);
				cbuf[cpos] = c;
				state = STR1;
				return (NUMBER);
			}
			state = STR1;
			goto dostr1;

		case ARGS:
			if (isdigit(cbuf[cpos])) {
				cp = &cbuf[cpos];
				while (isdigit(cbuf[++cpos]))
					;
				c = cbuf[cpos];
				cbuf[cpos] = '\0';
				yylval.i = strtoll(cp, 0, 10);
				cbuf[cpos] = c;
				return (NUMBER);
			}
			switch (cbuf[cpos++]) {

			case '\n':
				state = CMD;
				return (CRLF);

			case ' ':
				return (SP);

			case ',':
				return (COMMA);

			case 'A':
			case 'a':
				return (A);

			case 'B':
			case 'b':
				return (B);

			case 'C':
			case 'c':
				return (C);

			case 'E':
			case 'e':
				return (E);

			case 'F':
			case 'f':
				return (F);

			case 'I':
			case 'i':
				return (I);

			case 'L':
			case 'l':
				return (L);

			case 'N':
			case 'n':
				return (N);

			case 'P':
			case 'p':
				return (P);

			case 'R':
			case 'r':
				return (R);

			case 'S':
			case 's':
				return (S);

			case 'T':
			case 't':
				return (T);

			}
			break;

		default:
			fatal("Unknown state in scanner.");
		}
		yyerror((char *) 0);
		state = CMD;
		longjmp(errcatch,0);
	}
}

void
upper(char *s)
{
	while (*s != '\0') {
		if (islower(*s))
			*s = toupper(*s);
		s++;
	}
}

static char *
copy(char *s)
{
	char *p;

	p = malloc((unsigned) strlen(s) + 1);
	if (p == NULL)
		fatal("Ran out of memory.");
	(void) strcpy(p, s);
	return (p);
}

static void
help(struct tab *ctab, char *s)
{
	struct tab *c;
	int width, NCMDS;
	const char *help_type;

	if (ctab == sitetab)
		help_type = "SITE ";
	else
		help_type = "";
	width = 0, NCMDS = 0;
	for (c = ctab; c->name != NULL; c++) {
		int len = strlen(c->name);

		if (len > width)
			width = len;
		NCMDS++;
	}
	width = (width + 8) &~ 7;
	if (s == 0) {
		int i, j, w;
		int columns, lines;

		lreply(214, "The following %scommands are recognized %s.",
		    help_type, "(* =>'s unimplemented)");
		columns = 76 / width;
		if (columns == 0)
			columns = 1;
		lines = (NCMDS + columns - 1) / columns;
		for (i = 0; i < lines; i++) {
			printf("   ");
			for (j = 0; j < columns; j++) {
				c = ctab + j * lines + i;
				printf("%s%c", c->name,
					c->implemented ? ' ' : '*');
				if (c + lines >= &ctab[NCMDS])
					break;
				w = strlen(c->name) + 1;
				while (w < width) {
					putchar(' ');
					w++;
				}
			}
			printf("\r\n");
		}
		(void) fflush(stdout);
		reply(214, "Direct comments to ftp-bugs@%s.", hostname);
		return;
	}
	upper(s);
	c = lookup(ctab, s);
	if (c == (struct tab *)0) {
		reply(502, "Unknown command %s.", s);
		return;
	}
	if (c->implemented)
		reply(214, "Syntax: %s%s %s", help_type, c->name, c->help);
	else
		reply(214, "%s%-*s\t%s; unimplemented.", help_type, width,
		    c->name, c->help);
}

/* ARGSUSED */
static void
yyerror(const char *s)
{
  char *cp;

  (void)s;
  cp = strchr(cbuf,'\n');
  if (cp != NULL)
    *cp = '\0';
  reply(500, "'%s': command not understood.", cbuf);
}


