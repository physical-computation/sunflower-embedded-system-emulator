/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 38 "sf.y" /* yacc.c:339  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <math.h>
#include "endian-hitachi-sh.h"
#include "sf.h"
#include "mextern.h"

#line 78 "sf.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_BATTALERTFRAC = 258,
    T_BATTLEAK = 259,
    T_BATTCF = 260,
    T_BATTILEAK = 261,
    T_BATTETALUT = 262,
    T_BATTETALUTNENTRIES = 263,
    T_BATTINOMINAL = 264,
    T_BATTNODEATTACH = 265,
    T_BATTRF = 266,
    T_BATTSTATS = 267,
    T_BATTVBATTLUT = 268,
    T_BATTVBATTLUTNENTRIES = 269,
    T_BATTVLOSTLUT = 270,
    T_BATTVLOSTLUTNENTRIES = 271,
    T_PCBT = 272,
    T_CACHEINIT = 273,
    T_CACHEOFF = 274,
    T_CACHESTATS = 275,
    T_CD = 276,
    T_CLOCKINTR = 277,
    T_COMMENT = 278,
    T_CONT = 279,
    T_DISABLEPIPELINE = 280,
    T_DUMPALL = 281,
    T_DUMPMEM = 282,
    T_DUMPPIPE = 283,
    T_DUMPPWR = 284,
    T_DUMPREGS = 285,
    T_DUMPSYSREGS = 286,
    T_DUMPTIME = 287,
    T_DUMPTLB = 288,
    T_DYNINSTR = 289,
    T_EBATTINTR = 290,
    T_EFAULTS = 291,
    T_ENABLEPIPELINE = 292,
    T_FILE2NETSEG = 293,
    T_FLTTHRESH = 294,
    T_FORCEAVGPWR = 295,
    T_HELP = 296,
    T_IGNORENODEDEATHS = 297,
    T_INITSEESTATE = 298,
    T_HWSEEREG = 299,
    T_REGISTERRVAR = 300,
    T_INITRANDTABLE = 301,
    T_DEFNDIST = 302,
    T_RANDPRINT = 303,
    T_LOAD = 304,
    T_LOCSTATS = 305,
    T_LISTRVARS = 306,
    T_MALLOCDEBUG = 307,
    T_MMAP = 308,
    T_MODECA = 309,
    T_MODEFF = 310,
    T_NETCORREL = 311,
    T_NETDEBUG = 312,
    T_NETNEWSEG = 313,
    T_NETNODENEWIFC = 314,
    T_NETSEG2FILE = 315,
    T_NETSEGDELETE = 316,
    T_NETSEGFAILDURMAX = 317,
    T_NETSEGFAILPROB = 318,
    T_NETSEGFAILPROBFN = 319,
    T_NETSEGNICATTACH = 320,
    T_NETSEGPROPMODEL = 321,
    T_NEWBATT = 322,
    T_NEWNODE = 323,
    T_NODEFAILDURMAX = 324,
    T_NODEFAILPROB = 325,
    T_NODEFAILPROBFN = 326,
    T_NODETACH = 327,
    T_NUMAREGION = 328,
    T_NUMASTATS = 329,
    T_NUMASTATSALL = 330,
    T_NUMASETMAPID = 331,
    T_ADDVALUETRACE = 332,
    T_DELVALUETRACE = 333,
    T_REGISTERSTABS = 334,
    T_VALUESTATS = 335,
    T_OFF = 336,
    T_ON = 337,
    T_PAUINFO = 338,
    T_NANOPAUSE = 339,
    T_PAUSE = 340,
    T_PFUN = 341,
    T_PIPEFLUSH = 342,
    T_POWERSTATS = 343,
    T_POWERTOTAL = 344,
    T_PWD = 345,
    T_PARSEOBJDUMP = 346,
    T_QUIT = 347,
    T_GETRANDOMSEED = 348,
    T_SETRANDOMSEED = 349,
    T_RATIO = 350,
    T_RENUMBERNODES = 351,
    T_RESETNODECTRS = 352,
    T_RESETALLCTRS = 353,
    T_RESETCPU = 354,
    T_RETRYALG = 355,
    T_RUN = 356,
    T_SAVE = 357,
    T_SENSORSDEBUG = 358,
    T_SETBASENODEID = 359,
    T_SETBATT = 360,
    T_SETBATTFEEDPERIOD = 361,
    T_SETDUMPPWRPERIOD = 362,
    T_SETNETPERIOD = 363,
    T_SETFAULTPERIOD = 364,
    T_SETFREQ = 365,
    T_SETIFCOUI = 366,
    T_SETNODE = 367,
    T_SETPC = 368,
    T_SETPHYSICSPERIOD = 369,
    T_SETQUANTUM = 370,
    T_SETSCALEALPHA = 371,
    T_SETSCALEK = 372,
    T_SETSCALEVT = 373,
    T_SETSCHEDRANDOM = 374,
    T_SETSCHEDROUNDROBIN = 375,
    T_SETTAG = 376,
    T_SETTIMERDELAY = 377,
    T_SETVDD = 378,
    T_SETMEMRLATENCY = 379,
    T_SETMEMWLATENCY = 380,
    T_SETFLASHRLATENCY = 381,
    T_SETFLASHWLATENCY = 382,
    T_SHAREBUS = 383,
    T_SHOWCLK = 384,
    T_SHOWPIPE = 385,
    T_SHOWTAGS = 386,
    T_SIGNALSRC = 387,
    T_SIGNALSUBSCRIBE = 388,
    T_SIZEMEM = 389,
    T_SIZEPAU = 390,
    T_SPLIT = 391,
    T_SRECL = 392,
    T_STEP = 393,
    T_STOP = 394,
    T_SFATAL = 395,
    T_TRACE = 396,
    T_THROTTLE = 397,
    T_THROTTLEWIN = 398,
    T_VERBOSE = 399,
    T_VERSION = 400,
    T_SETLOC = 401,
    T_BPT = 402,
    T_BPTLS = 403,
    T_BPTDEL = 404,
    T_GLOBALTIME = 405,
    T_CYCLES = 406,
    T_INSTRS = 407,
    T_SENSORREADING = 408,
    T_DOTALIGN = 409,
    T_DOTORG = 410,
    T_DOTDATA = 411,
    T_DOTTEXT = 412,
    T_DOTGLOBAL = 413,
    T_DOTFILE = 414,
    T_DOTLONG = 415,
    T_DOTCOMM = 416,
    T_R0 = 417,
    T_R1 = 418,
    T_R2 = 419,
    T_R3 = 420,
    T_R4 = 421,
    T_R5 = 422,
    T_R6 = 423,
    T_R7 = 424,
    T_R8 = 425,
    T_R9 = 426,
    T_R10 = 427,
    T_R11 = 428,
    T_R12 = 429,
    T_R13 = 430,
    T_R14 = 431,
    T_R15 = 432,
    T_GBR = 433,
    T_VBR = 434,
    T_MACH = 435,
    T_MACL = 436,
    T_PC = 437,
    T_PR = 438,
    T_TRA = 439,
    T_SPC = 440,
    T_SR = 441,
    T_SSR = 442,
    T_LABELDEFN = 443,
    T_ADD = 444,
    T_ADDC = 445,
    T_ADDV = 446,
    T_AND = 447,
    T_ANDB = 448,
    T_BF = 449,
    T_BFS = 450,
    T_BRA = 451,
    T_BRAF = 452,
    T_BSR = 453,
    T_BSRF = 454,
    T_BT = 455,
    T_BTS = 456,
    T_CLRMAC = 457,
    T_CLRS = 458,
    T_CLRT = 459,
    T_CMPEQ = 460,
    T_CMPGE = 461,
    T_CMPGT = 462,
    T_CMPHI = 463,
    T_CMPHS = 464,
    T_CMPPL = 465,
    T_CMPPZ = 466,
    T_CMPSTR = 467,
    T_DIV0S = 468,
    T_DIV0U = 469,
    T_DIV1 = 470,
    T_DMULSL = 471,
    T_DMULUL = 472,
    T_DT = 473,
    T_EXTSB = 474,
    T_EXTSW = 475,
    T_EXTUB = 476,
    T_EXTUW = 477,
    T_JMP = 478,
    T_JSR = 479,
    T_LDC = 480,
    T_LDCL = 481,
    T_LDS = 482,
    T_LDSL = 483,
    T_LDTLB = 484,
    T_MACW = 485,
    T_MOV = 486,
    T_MOVB = 487,
    T_MOVL = 488,
    T_MOVW = 489,
    T_MOVA = 490,
    T_MOVT = 491,
    T_MULL = 492,
    T_MULS = 493,
    T_MULSW = 494,
    T_MULU = 495,
    T_MULUW = 496,
    T_NEG = 497,
    T_NEGC = 498,
    T_NOP = 499,
    T_NOT = 500,
    T_OR = 501,
    T_ORB = 502,
    T_PREF = 503,
    T_RFG = 504,
    T_ROTCL = 505,
    T_ROTCR = 506,
    T_ROTL = 507,
    T_ROTR = 508,
    T_RTE = 509,
    T_RTS = 510,
    T_SETS = 511,
    T_SETT = 512,
    T_SHAD = 513,
    T_SHAL = 514,
    T_SHAR = 515,
    T_SHLD = 516,
    T_SHLL = 517,
    T_SHLL2 = 518,
    T_SHLL8 = 519,
    T_SHLL16 = 520,
    T_SHLR = 521,
    T_SHLR2 = 522,
    T_SHLR8 = 523,
    T_SHLR16 = 524,
    T_SLEEP = 525,
    T_STC = 526,
    T_STCL = 527,
    T_STS = 528,
    T_STSL = 529,
    T_SUB = 530,
    T_SUBC = 531,
    T_SUBV = 532,
    T_SWAPB = 533,
    T_SWAPW = 534,
    T_TASB = 535,
    T_TRAPA = 536,
    T_TST = 537,
    T_TSTB = 538,
    T_XOR = 539,
    T_XORB = 540,
    T_XTRCT = 541,
    T_LABEL = 542,
    T_STRING = 543,
    T_MAN = 544
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 66 "sf.y" /* yacc.c:355  */

	double		dval;
	ulong		uval;
	long		sval;
	char		*str;
	Rval		*rval;
	DoubleList	*dlist;

#line 414 "sf.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);



/* Copy the second part of user declarations.  */

#line 431 "sf.tab.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2476

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  303
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  187
/* YYNRULES -- Number of rules.  */
#define YYNRULES  537
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1387

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   545

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     290,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,   294,     2,     2,     2,     2,
     296,   297,     2,   298,   292,   299,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     300,     2,   302,     2,   295,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   291,     2,   293,     2,     2,     2,     2,
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
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   301
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   441,   441,   442,   445,   446,   447,   448,   449,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549,   550,   551,   552,   553,   554,
     555,   556,   557,   558,   559,   560,   561,   562,   563,   564,
     565,   566,   567,   568,   569,   570,   571,   572,   573,   574,
     575,   576,   577,   578,   579,   580,   581,   582,   583,   584,
     585,   586,   587,   588,   589,   590,   591,   592,   593,   594,
     595,   596,   597,   598,   599,   600,   601,   602,   603,   604,
     605,   606,   607,   608,   609,   610,   611,   612,   613,   614,
     615,   616,   617,   618,   619,   620,   621,   622,   623,   624,
     628,   640,   650,   660,   688,   695,   702,   709,   719,   729,
     739,   749,   763,   781,   795,   813,   827,   845,   859,   866,
     873,   880,   887,   894,   901,   908,   915,   922,   929,   937,
     944,   952,   970,   988,   995,  1002,  1009,  1016,  1023,  1030,
    1037,  1044,  1051,  1059,  1072,  1079,  1086,  1098,  1105,  1112,
    1119,  1126,  1134,  1142,  1149,  1156,  1163,  1171,  1178,  1192,
    1200,  1207,  1214,  1228,  1245,  1252,  1259,  1266,  1273,  1280,
    1287,  1294,  1301,  1308,  1317,  1324,  1331,  1338,  1345,  1352,
    1359,  1366,  1373,  1380,  1387,  1403,  1411,  1420,  1427,  1434,
    1441,  1448,  1462,  1471,  1485,  1492,  1510,  1518,  1525,  1532,
    1539,  1546,  1553,  1560,  1567,  1574,  1581,  1588,  1595,  1602,
    1609,  1619,  1626,  1633,  1640,  1647,  1655,  1662,  1669,  1676,
    1683,  1691,  1698,  1705,  1712,  1719,  1726,  1733,  1740,  1747,
    1757,  1764,  1771,  1779,  1786,  1793,  1800,  1809,  1816,  1823,
    1830,  1837,  1844,  1859,  1866,  1873,  1884,  1895,  1902,  1916,
    1943,  1949,  1955,  1961,  1966,  1971,  1996,  2020,  2052,  2084,
    2116,  2148,  2180,  2210,  2240,  2275,  2310,  2345,  2376,  2406,
    2437,  2472,  2507,  2536,  2565,  2594,  2626,  2658,  2690,  2722,
    2754,  2784,  2815,  2847,  2877,  2909,  2941,  2973,  3005,  3036,
    3068,  3100,  3132,  3164,  3195,  3226,  3257,  3288,  3319,  3350,
    3381,  3414,  3445,  3471,  3502,  3533,  3564,  3597,  3628,  3659,
    3690,  3716,  3747,  3778,  3807,  3839,  3871,  3903,  3935,  3967,
    3999,  4031,  4063,  4095,  4127,  4159,  4191,  4223,  4255,  4287,
    4319,  4351,  4383,  4415,  4448,  4480,  4511,  4549,  4584,  4620,
    4655,  4690,  4725,  4760,  4795,  4830,  4866,  4902,  4939,  4975,
    5011,  5048,  5083,  5114,  5146,  5178,  5210,  5242,  5274,  5306,
    5338,  5367,  5399,  5431,  5461,  5491,  5522,  5551,  5582,  5613,
    5644,  5675,  5704,  5733,  5762,  5791,  5823,  5854,  5885,  5917,
    5948,  5979,  6010,  6041,  6072,  6103,  6134,  6165,  6194,  6225,
    6256,  6287,  6318,  6349,  6382,  6413,  6444,  6475,  6506,  6537,
    6570,  6601,  6632,  6663,  6694,  6725,  6756,  6788,  6820,  6852,
    6884,  6916,  6947,  6977,  7009,  7039,  7069,  7101,  7131,  7161,
    7193,  7193,  7210,  7219,  7814,  7828,  7840,  7852,  7866,  7878,
    7890,  7904,  7914,  7952,  7958,  7979,  7997,  8018,  8031,  8044,
    8060,  8069,  8076,  8077,  8078,  8079,  8080,  8081,  8082,  8083,
    8084,  8085,  8086,  8087,  8088,  8089,  8090,  8091
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_BATTALERTFRAC", "T_BATTLEAK",
  "T_BATTCF", "T_BATTILEAK", "T_BATTETALUT", "T_BATTETALUTNENTRIES",
  "T_BATTINOMINAL", "T_BATTNODEATTACH", "T_BATTRF", "T_BATTSTATS",
  "T_BATTVBATTLUT", "T_BATTVBATTLUTNENTRIES", "T_BATTVLOSTLUT",
  "T_BATTVLOSTLUTNENTRIES", "T_PCBT", "T_CACHEINIT", "T_CACHEOFF",
  "T_CACHESTATS", "T_CD", "T_CLOCKINTR", "T_COMMENT", "T_CONT",
  "T_DISABLEPIPELINE", "T_DUMPALL", "T_DUMPMEM", "T_DUMPPIPE", "T_DUMPPWR",
  "T_DUMPREGS", "T_DUMPSYSREGS", "T_DUMPTIME", "T_DUMPTLB", "T_DYNINSTR",
  "T_EBATTINTR", "T_EFAULTS", "T_ENABLEPIPELINE", "T_FILE2NETSEG",
  "T_FLTTHRESH", "T_FORCEAVGPWR", "T_HELP", "T_IGNORENODEDEATHS",
  "T_INITSEESTATE", "T_HWSEEREG", "T_REGISTERRVAR", "T_INITRANDTABLE",
  "T_DEFNDIST", "T_RANDPRINT", "T_LOAD", "T_LOCSTATS", "T_LISTRVARS",
  "T_MALLOCDEBUG", "T_MMAP", "T_MODECA", "T_MODEFF", "T_NETCORREL",
  "T_NETDEBUG", "T_NETNEWSEG", "T_NETNODENEWIFC", "T_NETSEG2FILE",
  "T_NETSEGDELETE", "T_NETSEGFAILDURMAX", "T_NETSEGFAILPROB",
  "T_NETSEGFAILPROBFN", "T_NETSEGNICATTACH", "T_NETSEGPROPMODEL",
  "T_NEWBATT", "T_NEWNODE", "T_NODEFAILDURMAX", "T_NODEFAILPROB",
  "T_NODEFAILPROBFN", "T_NODETACH", "T_NUMAREGION", "T_NUMASTATS",
  "T_NUMASTATSALL", "T_NUMASETMAPID", "T_ADDVALUETRACE", "T_DELVALUETRACE",
  "T_REGISTERSTABS", "T_VALUESTATS", "T_OFF", "T_ON", "T_PAUINFO",
  "T_NANOPAUSE", "T_PAUSE", "T_PFUN", "T_PIPEFLUSH", "T_POWERSTATS",
  "T_POWERTOTAL", "T_PWD", "T_PARSEOBJDUMP", "T_QUIT", "T_GETRANDOMSEED",
  "T_SETRANDOMSEED", "T_RATIO", "T_RENUMBERNODES", "T_RESETNODECTRS",
  "T_RESETALLCTRS", "T_RESETCPU", "T_RETRYALG", "T_RUN", "T_SAVE",
  "T_SENSORSDEBUG", "T_SETBASENODEID", "T_SETBATT", "T_SETBATTFEEDPERIOD",
  "T_SETDUMPPWRPERIOD", "T_SETNETPERIOD", "T_SETFAULTPERIOD", "T_SETFREQ",
  "T_SETIFCOUI", "T_SETNODE", "T_SETPC", "T_SETPHYSICSPERIOD",
  "T_SETQUANTUM", "T_SETSCALEALPHA", "T_SETSCALEK", "T_SETSCALEVT",
  "T_SETSCHEDRANDOM", "T_SETSCHEDROUNDROBIN", "T_SETTAG",
  "T_SETTIMERDELAY", "T_SETVDD", "T_SETMEMRLATENCY", "T_SETMEMWLATENCY",
  "T_SETFLASHRLATENCY", "T_SETFLASHWLATENCY", "T_SHAREBUS", "T_SHOWCLK",
  "T_SHOWPIPE", "T_SHOWTAGS", "T_SIGNALSRC", "T_SIGNALSUBSCRIBE",
  "T_SIZEMEM", "T_SIZEPAU", "T_SPLIT", "T_SRECL", "T_STEP", "T_STOP",
  "T_SFATAL", "T_TRACE", "T_THROTTLE", "T_THROTTLEWIN", "T_VERBOSE",
  "T_VERSION", "T_SETLOC", "T_BPT", "T_BPTLS", "T_BPTDEL", "T_GLOBALTIME",
  "T_CYCLES", "T_INSTRS", "T_SENSORREADING", "T_DOTALIGN", "T_DOTORG",
  "T_DOTDATA", "T_DOTTEXT", "T_DOTGLOBAL", "T_DOTFILE", "T_DOTLONG",
  "T_DOTCOMM", "T_R0", "T_R1", "T_R2", "T_R3", "T_R4", "T_R5", "T_R6",
  "T_R7", "T_R8", "T_R9", "T_R10", "T_R11", "T_R12", "T_R13", "T_R14",
  "T_R15", "T_GBR", "T_VBR", "T_MACH", "T_MACL", "T_PC", "T_PR", "T_TRA",
  "T_SPC", "T_SR", "T_SSR", "T_LABELDEFN", "T_ADD", "T_ADDC", "T_ADDV",
  "T_AND", "T_ANDB", "T_BF", "T_BFS", "T_BRA", "T_BRAF", "T_BSR", "T_BSRF",
  "T_BT", "T_BTS", "T_CLRMAC", "T_CLRS", "T_CLRT", "T_CMPEQ", "T_CMPGE",
  "T_CMPGT", "T_CMPHI", "T_CMPHS", "T_CMPPL", "T_CMPPZ", "T_CMPSTR",
  "T_DIV0S", "T_DIV0U", "T_DIV1", "T_DMULSL", "T_DMULUL", "T_DT",
  "T_EXTSB", "T_EXTSW", "T_EXTUB", "T_EXTUW", "T_JMP", "T_JSR", "T_LDC",
  "T_LDCL", "T_LDS", "T_LDSL", "T_LDTLB", "T_MACW", "T_MOV", "T_MOVB",
  "T_MOVL", "T_MOVW", "T_MOVA", "T_MOVT", "T_MULL", "T_MULS", "T_MULSW",
  "T_MULU", "T_MULUW", "T_NEG", "T_NEGC", "T_NOP", "T_NOT", "T_OR",
  "T_ORB", "T_PREF", "T_RFG", "T_ROTCL", "T_ROTCR", "T_ROTL", "T_ROTR",
  "T_RTE", "T_RTS", "T_SETS", "T_SETT", "T_SHAD", "T_SHAL", "T_SHAR",
  "T_SHLD", "T_SHLL", "T_SHLL2", "T_SHLL8", "T_SHLL16", "T_SHLR",
  "T_SHLR2", "T_SHLR8", "T_SHLR16", "T_SLEEP", "T_STC", "T_STCL", "T_STS",
  "T_STSL", "T_SUB", "T_SUBC", "T_SUBV", "T_SWAPB", "T_SWAPW", "T_TASB",
  "T_TRAPA", "T_TST", "T_TSTB", "T_XOR", "T_XORB", "T_XTRCT", "T_LABEL",
  "T_STRING", "T_MAN", "'\\n'", "'{'", "','", "'}'", "'#'", "'@'", "'('",
  "')'", "'+'", "'-'", "'<'", "\"rnd\"", "'>'", "$accept", "input", "line",
  "asm_ctl", "expr", "sf_cmd", "dotalign", "dotdata", "dottext",
  "dotglobal", "dotfile", "dotorg", "dotlong", "dotcomm", "add_instr",
  "addi_instr", "addc_instr", "addv_instr", "and_instr", "andi_instr",
  "andm_instr", "bf_instr", "bfs_instr", "bra_instr", "braf_instr",
  "bsr_instr", "bsrf_instr", "bt_instr", "bts_instr", "clrmac_instr",
  "clrs_instr", "clrt_instr", "cmpeq_instr", "cmpge_instr", "cmpgt_instr",
  "cmphi_instr", "cmphs_instr", "cmppl_instr", "cmppz_instr",
  "cmpstr_instr", "cmpim_instr", "div0s_instr", "div1_instr",
  "dmuls_instr", "dmulu_instr", "dt_instr", "extsb_instr", "extsw_instr",
  "extub_instr", "extuw_instr", "jmp_instr", "jsr_instr", "ldcsr_instr",
  "ldcgbr_instr", "ldcvbr_instr", "ldcssr_instr", "ldcspc_instr",
  "ldcr_bank_instr", "ldcmsr_instr", "ldcmgbr_instr", "ldcmvbr_instr",
  "ldcmssr_instr", "ldcmspc_instr", "ldcmr_bank_instr", "ldsmach_instr",
  "ldsmacl_instr", "ldspr_instr", "ldsmmach_instr", "ldsmmacl_instr",
  "ldsmpr_instr", "ldtlb_instr", "macl_instr", "macw_instr", "mov_instr",
  "movbs_instr", "movws_instr", "movls_instr", "movbl_instr",
  "movwl_instr", "movll_instr", "movbm_instr", "movwm_instr",
  "movlm_instr", "movbp_instr", "movwp_instr", "movlp_instr",
  "movbs0_instr", "movws0_instr", "movls0_instr", "movbl0_instr",
  "movwl0_instr", "movll0_instr", "movi_instr", "movwi_instr",
  "movli_instr", "movblg_instr", "movwlg_instr", "movllg_instr",
  "movbsg_instr", "movwsg_instr", "movlsg_instr", "movbs4_instr",
  "movws4_instr", "movls4_instr", "movbl4_instr", "movwl4_instr",
  "movll4_instr", "mova_instr", "movt_instr", "mull_instr", "muls_instr",
  "mulu_instr", "muluw_instr", "neg_instr", "negc_instr", "nop_instr",
  "not_instr", "or_instr", "ori_instr", "orm_instr", "pref_instr",
  "rfg_instr", "rotcl_instr", "rotcr_instr", "rotl_instr", "rotr_instr",
  "rte_instr", "rts_instr", "sets_instr", "sett_instr", "shad_instr",
  "shal_instr", "shar_instr", "shld_instr", "shll_instr", "shll2_instr",
  "shll8_instr", "shll16_instr", "shlr_instr", "shlr2_instr",
  "shlr8_instr", "shlr16_instr", "sleep_instr", "stcsr_instr",
  "stcgbr_instr", "stcvbr_instr", "stcssr_instr", "stcspc_instr",
  "stcr_bank_instr", "stcmsr_instr", "stcmgbr_instr", "stcmvbr_instr",
  "stcmssr_instr", "stcmspc_instr", "stcmr_bank_instr", "stsmach_instr",
  "stsmacl_instr", "stspr_instr", "stsmmach_instr", "stsmmacl_instr",
  "stsmpr_instr", "sub_instr", "subc_instr", "subv_instr", "swapb_instr",
  "swapw_instr", "tas_instr", "trapa_instr", "tst_instr", "tsti_instr",
  "tstb_instr", "xor_instr", "xori_instr", "xorm_instr", "xtrct_instr",
  "rnd", "rnd_const", "rnd_var", "uimm", "simm", "dimm", "rdimm",
  "dimmlist", "dimmlistbody", "disp", "optstring", "reg", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   477,   478,   479,   480,   481,   482,   483,   484,
     485,   486,   487,   488,   489,   490,   491,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     505,   506,   507,   508,   509,   510,   511,   512,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   537,   538,   539,   540,   541,   542,   543,   544,
      10,   123,    44,   125,    35,    64,    40,    41,    43,    45,
      60,   545,    62
};
# endif

#define YYPACT_NINF -767

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-767)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -767,  2084,  -767,  -229,  -229,  -229,  -235,  -235,  -229,  -235,
    -229,  -235,  -235,  -235,  -235,  -235,  -236,  -235,  -234,  -222,
    -213,  -235,  -194,  -235,  -192,  -191,  -235,  -186,  -182,  -177,
    -174,  -173,  -235,  -235,  -172,  -167,  -235,  -229,  -166,  -235,
    -162,  -155,  -153,  -152,  -134,  -131,  -124,  -115,  -112,  -104,
    -235,  -100,   -98,  -235,   -92,  -235,  -235,  -235,  -235,  -235,
    -235,   -87,  -235,  -235,  -235,   -81,  -235,  -229,   -80,  -235,
     -78,   -79,   -77,  -235,   -76,   -74,   -71,   -72,   -69,   -68,
     -66,  -235,  -235,   -57,   -55,   -48,   -46,   -47,   -45,   -44,
    -235,   -43,   -40,   -37,   -36,   -35,  -235,   -81,  -235,  -767,
    -235,  -235,  -229,  -229,  -229,  -229,  -229,  -235,  -235,  -235,
    -229,  -235,  -229,  -229,  -229,   -34,   -33,  -229,  -229,  -235,
    -235,  -235,  -235,  -235,   -32,   -31,  -235,  -235,  -235,  -235,
    -235,   -27,  -282,   -28,   -25,  -235,  -235,  -235,   -26,  -767,
    -229,   -23,   -21,  -235,  -235,  -235,  -767,  -767,  -226,   -15,
    -226,  -226,   -30,  2190,   402,   920,   920,   418,   -75,  -226,
    -226,   -83,  -226,   920,  -226,  -226,  -767,  -767,  -767,   435,
     920,   920,   920,   920,   920,   920,   920,   920,   920,   920,
     920,   920,   920,   920,   920,   920,   -19,   -13,   920,   -12,
     920,   -11,  -767,   -10,   513,   245,  -127,   379,    -9,   920,
     920,   920,   920,   920,   920,   920,   920,  -767,   920,   597,
     -54,    -8,   -20,   920,   920,   920,   920,  -767,  -767,  -767,
    -767,   920,   920,   920,   920,   920,   920,   920,   920,   920,
     920,   920,   920,  -767,   671,   697,   -49,   -18,   920,   920,
     920,   920,   920,    -1,   -14,   613,    -6,   655,     1,   920,
      11,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,    15,    16,    18,    19,
      20,  -767,  -229,    21,    22,    42,    59,    60,  -229,    61,
    -229,    62,  -767,  -235,  -767,  -767,    63,    64,  -767,    65,
    -767,    68,  -235,  -767,  -767,  -767,  -767,  -767,    67,   133,
    -767,  -235,   134,  -229,  -767,   135,  -229,  -235,  -235,   138,
     136,  -229,   140,  -767,  -767,  -767,  -235,  -767,  -767,  -235,
    -767,  -235,  -229,   143,   142,  -235,  -229,   144,  -235,  -235,
    -229,  -767,  -278,   146,   148,   149,   150,  -235,  -767,  -767,
    -235,  -235,  -235,   151,  -767,  -767,  -767,  -767,   152,   153,
     155,  -767,  -767,  -767,   156,  -767,  -767,   157,  -767,  -767,
    -767,  -767,  -767,   145,   158,  -235,   159,   161,  -767,  -767,
    -767,  -767,   162,   166,   165,   167,  -767,   168,   169,   172,
     174,  -767,  -767,  -767,   175,   179,   181,   182,   183,   184,
    -767,  -767,   -81,  -235,   185,   186,  -235,   187,  -767,   188,
    -767,   190,   191,   192,   194,  -767,  -229,  -235,  -235,  -235,
    -235,  -767,   195,  -767,  -767,  -767,  -767,   173,   198,  -767,
    -767,  -767,   164,   920,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -193,   196,   200,   202,  -193,   203,  -193,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -193,   204,   205,   206,
     207,   208,  -767,  -767,   209,   210,   211,   212,   214,  -767,
     215,   216,   218,   219,   920,   920,   220,   920,   221,   920,
     920,  -193,   222,   223,   197,   224,   225,   213,   226,   227,
     228,   229,   230,   -29,  -767,   231,   232,   234,   235,   236,
     237,   238,   239,  -193,   240,  -193,   920,  -193,  -767,  -767,
    -767,  -767,   241,  -767,  -767,   242,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,   243,   244,   246,   247,   265,   266,
     267,   268,   269,   304,   322,   323,   324,   325,   326,   327,
     328,   329,   331,   332,   333,   334,   336,   920,  -193,  -193,
     337,  -193,  -193,   338,  -193,   340,  -767,  -767,  -767,  -767,
    -767,  -767,   199,  -767,  -767,  -767,  -767,  -767,   343,  -767,
     344,  -767,  -235,  -767,  -767,  -767,   217,   345,  -767,  -767,
     346,  -767,  -767,  -767,  -229,  -235,   233,  -229,   137,  -229,
    -767,   349,  -229,  -235,  -229,   350,  -767,   351,   352,  -767,
     353,  -229,   354,  -767,   189,   347,  -767,  -767,  -767,  -767,
    -233,  -767,  -767,  -767,  -767,  -235,   355,  -235,  -235,  -767,
    -767,  -767,  -767,  -767,  -767,   357,  -767,   249,  -767,  -767,
    -767,   360,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -229,  -767,  -767,  -767,  -235,  -767,
    -767,  -767,  -767,  -767,  -767,  -229,   362,   363,   364,  -229,
    -767,  -767,  -767,   368,   359,  -767,   372,   373,   370,   920,
     920,   920,   371,   920,   374,   376,   920,   920,   920,   920,
     920,   920,   920,   920,   920,   920,   920,   920,   920,   920,
    -767,  -767,   990,   366,    -7,   367,   375,   377,   920,   396,
    -132,  -289,   397,   398,  -129,  -241,   920,   399,   403,   -99,
    -221,   404,  -226,   920,   920,   920,   920,   920,   920,   920,
     920,   405,   920,   408,  -767,  -767,   920,   920,   920,   920,
     920,   920,   920,   920,   406,   407,   409,   410,   411,   412,
     920,   920,   920,   413,   414,   415,   920,   920,   920,   920,
     920,  -767,  -767,   419,   920,   421,   422,   920,   423,   920,
    -767,  -767,  -767,   426,   427,  -767,  -767,  -229,  -235,  -229,
    -229,  -229,   428,  -229,  -767,   429,  -235,  -229,  -767,  -767,
    -767,  -767,  -767,  -767,   430,   433,  -233,  -193,  -767,  -235,
    -235,  -767,   432,  -767,  -229,   437,   440,  -767,  -767,  -767,
     441,  -767,   442,  -767,  -767,   920,  -767,  -767,  -767,   282,
    -767,   447,   541,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,   443,  -767,  -767,  -767,   451,   453,   920,
    -767,   431,   455,   456,   920,   458,   -24,   436,   460,   461,
     920,   462,  -767,   154,   459,   464,   465,   920,   499,   171,
     500,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,   631,
    -767,   501,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
     495,   496,   498,   503,   504,   505,  -767,  -767,  -767,   506,
     507,   509,  -767,  -767,  -767,  -767,  -767,   636,  -767,   514,
     637,  -767,   515,  -767,  -767,  -767,  -229,   510,  -229,  -229,
    -767,  -261,   137,  -229,  -767,  -235,  -229,  -229,  -229,  -271,
    -193,  -235,  -235,  -767,  -229,   521,  -767,  -767,   518,  -767,
    -767,   519,  -767,  1093,    14,   556,  -767,  -226,   920,  1134,
    -767,   920,   652,   920,  -767,  -226,   920,  1016,  -767,   920,
     -50,   920,  -767,  -226,   920,  1055,  -767,   920,   690,   920,
    -767,   672,  -767,   557,   920,   920,   920,   920,   920,   920,
     920,   920,   920,  -767,   559,  -767,   581,   590,  -767,  -229,
    -229,  -229,  -767,   586,  -229,  -235,  -235,  -229,  -229,  -235,
    -767,  -193,  -235,  -235,  -229,  -767,   920,   718,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,   920,   589,   588,
     591,   592,  -767,   594,  -767,   595,   593,   596,   598,   599,
    -767,   600,   602,  -767,   605,   601,   603,   604,   606,  -767,
     607,  -767,   608,   742,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,   744,   746,  -229,  -229,  -229,  -767,  -767,
    -229,  -229,  -229,  -229,  -229,  -235,  -193,  -235,  -235,  -229,
     611,   618,   614,  1232,   619,   621,   622,   920,   738,   625,
     626,   627,   628,   920,   920,  1263,   629,   630,   633,   639,
     920,   640,   641,   643,   644,  -229,   649,  -229,   648,  -229,
    -229,  -229,  -229,   650,  -235,   651,   653,  -229,  -767,   751,
    -767,   642,   645,   920,   782,   784,   656,   657,   920,   788,
     920,   920,   658,   659,   660,   661,   920,   790,   920,   797,
     663,   800,   785,   786,   792,  -229,  -229,  -229,  -767,  -229,
    -229,   675,   670,  -767,  -235,  -767,  -767,  -229,   678,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
     679,   699,   700,  -229,  -229,   709,  -235,  -235,  -767,  -767,
    -235,  -229,  -767,  -767,  -767,  -767,   713,  -229,  -767,  -229,
    -235,   712,  -229,  -229,  -229,  -229,   727,  -767,  -229,  -229,
     715,  -229,  -767,  -229,  -229,   730,   731,  -229,  -229,  -767,
    -767,  -229,   720,  -229,  -229,  -229,  -229,  -229,  -229,  -229,
    -229,  -229,  -767,  -229,  -229,   -81,  -235,  -229,  -229,  -229,
    -235,   -81,  -235,  -229,  -235,   732,  -767
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   520,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   520,     0,   236,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   235,
       0,     0,     0,     0,     0,     0,   330,   331,     0,     0,
       0,     0,     0,     4,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   352,   353,   354,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   393,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   440,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   451,   452,   453,
     454,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   467,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   328,     3,     7,     6,     8,     9,    10,    11,    14,
      12,    13,    15,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    58,    59,    60,    61,    62,    57,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   149,   150,   151,   152,   153,   148,   155,
     156,   157,   158,   159,   154,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   508,     0,     0,     0,     0,
       0,   504,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   198,     0,   247,   248,     0,     0,   327,     0,
     284,     0,     0,   251,   249,   250,   263,   253,     0,     0,
     285,     0,     0,     0,   301,     0,     0,     0,     0,     0,
       0,     0,     0,   311,   312,   323,     0,   244,   245,     0,
     315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   521,     0,     0,     0,     0,     0,     0,   260,   261,
       0,     0,     0,     0,   258,   314,   313,   324,     0,     0,
       0,   303,   310,   239,     0,   180,   265,     0,   264,   221,
     222,   223,   252,     0,     0,     0,     0,     0,   229,   234,
     230,   231,     0,     0,     0,     0,   228,     0,     0,     0,
       0,   217,   218,   227,     0,     0,     0,     0,     0,     0,
     309,   304,   520,     0,     0,     0,     0,     0,   291,     0,
     287,     0,     0,     0,     0,   289,     0,     0,     0,     0,
       0,   203,     0,   329,   334,   516,   517,     0,     0,   332,
     333,   335,     0,     0,     5,   522,   523,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,     0,     0,     0,     0,     0,     0,     0,   344,   345,
     346,   347,   348,   349,   350,   351,     0,     0,     0,     0,
       0,     0,   360,   361,     0,     0,     0,     0,     0,   368,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   447,   448,
     449,   450,     0,   456,   457,     0,   459,   460,   461,   462,
     463,   464,   465,   466,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   302,   510,   509,   268,
     188,   187,     0,   192,   189,   184,   190,   185,     0,   194,
       0,   196,     0,   226,   286,   307,     0,     0,   267,   320,
       0,   319,   232,   213,     0,     0,     0,     0,     0,     0,
     293,     0,     0,     0,     0,     0,   277,     0,     0,   279,
       0,     0,     0,   181,     0,     0,   512,   500,   501,   511,
       0,   270,   269,   278,   254,     0,     0,     0,     0,   257,
     325,   326,   321,   240,   266,     0,   290,     0,   220,   197,
     300,     0,   242,   294,   219,   214,   215,   216,   295,   296,
     297,   298,   299,   308,     0,   237,   280,   281,     0,   306,
     292,   288,   316,   317,   318,     0,     0,     0,     0,     0,
     204,   518,   519,     0,     0,   505,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     373,   374,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   445,   446,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   491,   492,     0,     0,     0,     0,     0,     0,     0,
     191,   193,   195,     0,     0,   283,   225,     0,     0,     0,
       0,     0,     0,     0,   243,     0,     0,     0,   224,   271,
     273,   274,   233,   186,     0,     0,     0,     0,   262,     0,
       0,   206,     0,   322,     0,     0,     0,   199,   200,   201,
       0,   336,     0,   507,   506,     0,   337,   339,   340,     0,
     341,     0,     0,   355,   356,   357,   358,   359,   362,   364,
     365,   366,   367,   369,   370,   371,   372,   376,   377,   379,
     375,   378,   380,     0,   387,   388,   389,     0,     0,     0,
     396,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   418,     0,     0,     0,     0,     0,     0,     0,
       0,   433,   434,   435,   436,   437,   438,   439,   441,     0,
     442,     0,   455,   458,   469,   470,   472,   468,   471,   473,
       0,     0,     0,     0,     0,     0,   480,   481,   482,     0,
       0,     0,   486,   487,   488,   489,   490,     0,   493,     0,
       0,   496,     0,   499,   246,   241,     0,     0,     0,     0,
     514,     0,     0,     0,   275,     0,     0,     0,     0,     0,
       0,     0,     0,   305,     0,     0,   205,   202,     0,   338,
     342,     0,   363,     0,     0,     0,   415,     0,     0,     0,
     400,     0,     0,     0,   397,     0,     0,     0,   402,     0,
       0,     0,   399,     0,     0,     0,   401,     0,     0,     0,
     398,     0,   443,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   494,     0,   497,     0,     0,   211,     0,
       0,     0,   513,     0,     0,     0,     0,     0,     0,     0,
     182,     0,     0,     0,     0,   282,     0,     0,   382,   383,
     385,   381,   384,   386,   390,   391,   392,     0,     0,     0,
       0,     0,   406,     0,   403,     0,     0,     0,     0,     0,
     408,     0,     0,   405,     0,     0,     0,     0,     0,   407,
       0,   404,     0,     0,   475,   476,   478,   474,   477,   479,
     483,   484,   485,     0,     0,     0,     0,     0,   515,   210,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   394,     0,
     395,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   207,     0,
       0,     0,     0,   183,     0,   255,   256,     0,     0,   422,
     425,   412,   419,   428,   409,   424,   414,   421,   417,   430,
     411,   427,   423,   426,   413,   420,   416,   429,   410,   431,
       0,     0,     0,     0,     0,     0,     0,     0,   503,   502,
       0,     0,   343,   444,   495,   498,     0,     0,   209,     0,
       0,     0,     0,     0,     0,     0,     0,   259,     0,     0,
       0,     0,   276,     0,     0,     0,     0,     0,     0,   208,
     272,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   212,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   238
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -767,  -767,  -767,  -767,   874,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,
    -767,  -767,  -767,  -767,  -767,  -767,  -767,  -767,   170,  -589,
      -3,  -766,   -42,  -767,  -136,   -86,   810
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   398,   399,
     400,   401,   402,   403,   404,   405,   406,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   424,   766,   767,   768,   432,   828,
     769,   770,   932,  1071,   579,   492,   602
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
     428,   429,   430,  1004,   946,   434,   431,   436,   558,  1005,
     425,   524,   763,   764,   581,   582,   832,  1149,   834,  1150,
     426,   427,   765,   608,   609,   610,   612,   835,   614,   615,
    1002,  1141,  1142,  1008,   463,   646,   586,   587,   588,   589,
     590,   591,   592,   593,   594,   595,   596,   597,   598,   599,
     600,  1010,   857,   431,   442,   425,   444,  1011,   764,   425,
     648,   575,   576,  1015,   494,   426,   427,   765,   445,   426,
     427,  1017,   577,   578,   881,   446,   883,  1018,   885,   585,
     586,   587,   588,   589,   590,   591,   592,   593,   594,   595,
     596,   597,   598,   599,   600,   825,   448,   451,   450,   528,
     529,   530,   531,   532,   453,   826,   827,   536,   454,   538,
     539,   540,  1181,   455,   543,   544,   456,   457,   460,   912,
     913,   461,   915,   916,   464,   918,   466,   567,   568,   569,
     570,   696,   697,   467,   698,   468,   469,   566,   585,   586,
     587,   588,   589,   590,   591,   592,   593,   594,   595,   596,
     597,   598,   599,   600,   470,   575,   576,   471,   575,   576,
     575,   576,   699,   700,   472,   701,   577,   578,   647,   577,
     578,   577,   578,   994,   995,   473,   996,   433,   474,   435,
    1079,   437,   438,   439,   440,   441,   475,   443,   575,   576,
     477,   447,   478,   449,  1164,  1165,   452,  1166,   480,   577,
     578,   487,   458,   459,   575,   576,   462,   491,   495,   465,
     497,   498,   501,   499,   502,   577,   578,   503,   504,   607,
     476,   505,   506,   479,   507,   481,   482,   483,   484,   485,
     486,   510,   488,   489,   490,   511,   493,   575,   576,   496,
     665,   514,   512,   500,   513,   515,   516,   518,   577,   578,
     519,   508,   509,   520,   521,   522,   541,   542,   550,   551,
     517,   557,   560,   561,   565,   583,   523,   872,   525,   571,
     526,   527,  1102,   580,   667,  1103,   634,   533,   534,   535,
     708,   537,   635,   637,   639,   640,   653,   666,   711,   545,
     546,   547,   548,   549,   707,   714,   552,   553,   554,   555,
     556,   716,   559,   717,   718,   562,   563,   564,   719,   720,
     721,   723,   724,   572,   573,   574,   585,   586,   587,   588,
     589,   590,   591,   592,   593,   594,   595,   596,   597,   598,
     599,   600,   725,   585,   586,   587,   588,   589,   590,   591,
     592,   593,   594,   595,   596,   597,   598,   599,   600,   726,
     727,   729,   731,   733,   734,   735,   736,   738,  1080,   585,
     586,   587,   588,   589,   590,   591,   592,   593,   594,   595,
     596,   597,   598,   599,   600,   585,   586,   587,   588,   589,
     590,   591,   592,   593,   594,   595,   596,   597,   598,   599,
     600,   585,   586,   587,   588,   589,   590,   591,   592,   593,
     594,   595,   596,   597,   598,   599,   600,   643,   586,   587,
     588,   589,   590,   591,   592,   593,   594,   595,   596,   597,
     598,   599,   600,   739,   741,   743,   747,   748,   931,   722,
     750,   755,   756,   785,   759,   728,   771,   730,   772,   773,
     774,   779,   780,   781,  1090,   782,   783,   784,   786,   788,
    1110,   789,   790,  1111,   791,   792,   823,   793,   794,   795,
     742,   821,   796,   744,   797,   798,   804,  1118,   749,   799,
    1119,   800,   801,   802,   803,   806,   807,   809,   810,   754,
     811,   812,   813,   758,   814,   820,   822,   762,   829,   920,
     944,  1151,   830,   860,   831,   833,   836,   837,   838,   839,
     840,   841,   842,   843,   844,   924,   845,   846,   847,   864,
     848,   849,   852,   854,   858,   859,   862,   863,   866,   867,
     868,   929,   871,   873,   874,   869,   875,   876,   877,   878,
     879,   880,   882,   886,   887,   888,   889,   952,   890,   891,
     644,   650,   586,   587,   588,   589,   590,   591,   592,   593,
     594,   595,   596,   597,   598,   599,   600,   892,   893,   894,
     895,   896,  1216,   815,   585,   586,   587,   588,   589,   590,
     591,   592,   593,   594,   595,   596,   597,   598,   599,   600,
     585,   586,   587,   588,   589,   590,   591,   592,   593,   594,
     595,   596,   597,   598,   599,   600,   897,   585,   586,   587,
     588,   589,   590,   591,   592,   593,   594,   595,   596,   597,
     598,   599,   600,   732,   898,   899,   900,   901,   902,   903,
     904,   905,   737,   906,   907,   908,   909,  1254,   910,   914,
     917,   740,   919,   921,   922,   925,   926,   745,   746,   934,
     938,   939,   940,   941,   943,   948,   751,   951,   945,   752,
     953,   753,   957,   958,   959,   757,   961,   962,   760,   761,
     963,   964,   965,   969,   993,   997,   971,   775,   972,   999,
     776,   777,   778,   998,   651,   585,   586,   587,   588,   589,
     590,   591,   592,   593,   594,   595,   596,   597,   598,   599,
     600,  1001,  1006,  1007,  1013,   787,   601,  1029,  1014,  1019,
    1031,  1040,  1041,  1092,  1042,  1043,  1044,  1045,  1049,  1050,
    1051,  1057,   605,  1059,  1060,  1062,  1064,  1065,  1077,  1074,
    1072,  1078,  1083,   805,  1003,  1085,   808,  1097,  1009,   616,
    1086,  1087,  1105,  1016,  1088,  1093,  1020,   816,   817,   818,
     819,   927,  1091,  1094,   930,  1095,   933,  1098,  1099,   935,
    1101,   937,  1106,  1107,  1109,  1113,  1114,  1115,   942,   585,
     586,   587,   588,   589,   590,   591,   592,   593,   594,   595,
     596,   597,   598,   599,   600,   585,   586,   587,   588,   589,
     590,   591,   592,   593,   594,   595,   596,   597,   598,   599,
     600,  1117,  1121,  1122,  1124,  1125,  1123,  1126,  1133,  1135,
    1138,   954,  1127,  1128,  1129,  1130,  1131,   641,  1132,  1134,
    1136,  1155,   956,  1156,  1173,  1157,   960,   585,   586,   587,
     588,   589,   590,   591,   592,   593,   594,   595,   596,   597,
     598,   599,   600,   585,   586,   587,   588,   589,   590,   591,
     592,   593,   594,   595,   596,   597,   598,   599,   600,   684,
     685,  1167,  1190,  1193,  1192,  1203,   686,   687,   688,   585,
     586,   587,   588,   589,   590,   591,   592,   593,   594,   595,
     596,   597,   598,   599,   600,   690,   691,  1204,  1205,  1209,
    1221,  1223,   692,   693,   694,  1224,  1227,  1228,  1225,  1226,
    1229,   663,  1233,  1230,  1234,  1231,  1232,  1235,  1236,  1240,
    1237,  1238,   923,  1239,  1242,  1241,  1243,   709,  1244,  1258,
    1259,  1263,  1260,  1264,  1265,   928,  1267,  1268,  1269,  1270,
    1271,  1276,  1277,   936,  1066,  1278,  1068,  1069,  1070,  1298,
    1073,  1279,  1281,  1282,  1076,  1283,  1284,  1286,  1288,  1299,
    1293,  1295,  1300,  1296,  1302,   947,  1303,   949,   950,   712,
    1307,  1084,  1315,  1304,  1305,  1310,  1311,  1312,  1313,  1317,
    1318,  1168,  1319,  1320,  1321,   603,   604,   606,  1328,  1175,
    1322,   611,  1329,   613,  1182,  1332,  1333,  1184,   955,   617,
     618,   619,   620,   621,   622,   623,   624,   625,   626,   627,
     628,   629,   630,   631,   632,   633,  1334,  1335,   636,  1338,
     638,  1343,  1347,  1355,   642,   645,   649,   652,  1364,   654,
     655,   656,   657,   658,   659,   660,   661,  1352,   662,   664,
    1359,  1360,  1386,   668,   669,   670,   671,   584,     0,     0,
    1143,   672,   673,   674,   675,   676,   677,   678,   679,   680,
     681,   682,   683,     0,   689,   695,     0,     0,   702,   703,
     704,   705,   706,     0,     0,   710,     0,   713,     0,   715,
       0,     0,     0,  1137,     0,  1139,  1140,     0,     0,     0,
    1144,     0,     0,  1146,  1147,  1148,     0,     0,     0,     0,
       0,  1154,   585,   586,   587,   588,   589,   590,   591,   592,
     593,   594,   595,   596,   597,   598,   599,   600,  1067,     0,
       0,     0,     0,     0,     0,     0,  1075,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1081,
    1082,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1206,  1207,  1208,     0,
       0,  1210,     0,     0,  1213,  1214,     0,     0,     0,     0,
       0,  1219,   585,   586,   587,   588,   589,   590,   591,   592,
     593,   594,   595,   596,   597,   598,   599,   600,   987,   988,
       0,     0,     0,     0,     0,   989,   990,   991,   585,   586,
     587,   588,   589,   590,   591,   592,   593,   594,   595,   596,
     597,   598,   599,   600,  1177,     0,     0,     0,  1178,     0,
       0,     0,  1245,  1246,  1247,     0,     0,  1248,  1249,  1250,
    1251,  1252,     0,     0,     0,     0,  1257,   585,   586,   587,
     588,   589,   590,   591,   592,   593,   594,   595,   596,   597,
     598,   599,   600,  1186,     0,     0,     0,  1187,     0,     0,
       0,     0,  1285,     0,  1287,  1145,  1289,  1290,  1291,  1292,
       0,  1152,  1153,     0,  1297,   585,   586,   587,   588,   589,
     590,   591,   592,   593,   594,   595,   596,   597,   598,   599,
     600,  1158,  1159,     0,     0,     0,     0,     0,  1160,  1161,
    1162,     0,  1323,  1324,  1325,     0,  1326,  1327,     0,  1376,
       0,     0,     0,     0,  1331,  1382,   585,   586,   587,   588,
     589,   590,   591,   592,   593,   594,   595,   596,   597,   598,
     599,   600,  1170,     0,     0,  1211,  1212,     0,     0,  1215,
    1336,  1337,  1217,  1218,     0,     0,     0,     0,  1342,     0,
       0,     0,     0,     0,  1344,     0,  1345,     0,     0,  1348,
    1349,  1350,  1351,     0,     0,  1353,  1354,     0,  1356,     0,
    1357,  1358,     0,     0,  1361,  1362,     0,     0,  1363,     0,
    1365,  1366,  1367,  1368,  1369,  1370,  1371,  1372,  1373,     0,
    1374,  1375,     0,     0,  1378,  1379,  1380,     0,     0,     0,
    1384,     0,     0,     0,     0,  1253,     0,  1255,  1256,     0,
       0,     0,     0,   824,   585,   586,   587,   588,   589,   590,
     591,   592,   593,   594,   595,   596,   597,   598,   599,   600,
    1261,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1294,   585,   586,   587,   588,   589,
     590,   591,   592,   593,   594,   595,   596,   597,   598,   599,
     600,  1274,     0,     0,   850,   851,     0,   853,     0,   855,
     856,     0,     0,     0,   861,     0,     0,   865,     0,     0,
       0,   870,     0,     0,  1330,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   884,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1339,  1340,     0,     0,
    1341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1346,     0,     0,     0,     0,     0,     0,   911,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1377,     0,     0,     0,
    1381,     0,  1383,     0,  1385,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   966,
     967,   968,     0,   970,     0,     0,   973,   974,   975,   976,
     977,   978,   979,   980,   981,   982,   983,   984,   985,   986,
       0,     0,   992,     0,     0,     0,     0,     0,  1000,     0,
       0,     0,     0,     0,     0,     0,  1012,     0,     0,     0,
       0,     0,     0,  1021,  1022,  1023,  1024,  1025,  1026,  1027,
    1028,     0,  1030,     0,     0,     0,  1032,  1033,  1034,  1035,
    1036,  1037,  1038,  1039,     0,     0,     0,     0,     0,     0,
    1046,  1047,  1048,     0,     0,     0,  1052,  1053,  1054,  1055,
    1056,     0,     0,     0,  1058,     0,     0,  1061,     0,  1063,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1089,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1096,
       0,     0,     0,     0,  1100,     0,  1104,     0,     0,     0,
    1108,     0,     0,  1112,     0,     0,     0,  1116,     0,  1120,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1163,     0,     0,     0,     0,  1169,  1171,
       0,  1172,     0,  1174,     0,     0,  1176,  1179,     0,  1180,
       0,  1183,     0,     0,  1185,  1188,     0,  1189,     0,  1191,
       0,     0,     0,     0,  1194,  1195,  1196,  1197,  1198,  1199,
    1200,  1201,  1202,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1220,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1222,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1262,     0,     0,     0,  1266,     0,     0,
       0,     0,     0,  1272,  1273,  1275,     0,     0,     0,     0,
    1280,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1301,     0,     0,     0,     0,  1306,     0,
    1308,  1309,     0,     0,     2,     0,  1314,     3,  1316,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,     0,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,     0,   117,   118,   119,   120,
     121,   122,   123,   124,   125,     0,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,     0,     0,     0,     0,   144,   145,
     146,   147,   148,   149,   150,   151,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   152,     0,     0,     0,     0,
       0,     0,   153,   154,   155,   156,   157,   158,   159,   160,
     161,     0,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   152,     0,   250,   251,     0,     0,     0,     0,   154,
     155,   156,   157,   158,   159,   160,   161,     0,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,     0,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249
};

static const yytype_int16 yycheck[] =
{
       3,     4,     5,   292,   770,     8,   288,    10,   290,   298,
     288,    97,   290,   291,   150,   151,   605,   288,   607,   290,
     298,   299,   300,   159,   160,   161,   162,   616,   164,   165,
     162,   292,   293,   162,    37,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   292,   641,   288,   290,   288,   290,   298,   291,   288,
     196,   287,   288,   162,    67,   298,   299,   300,   290,   298,
     299,   292,   298,   299,   663,   288,   665,   298,   667,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   288,   290,   288,   290,   102,
     103,   104,   105,   106,   290,   298,   299,   110,   290,   112,
     113,   114,   162,   290,   117,   118,   290,   290,   290,   708,
     709,   288,   711,   712,   290,   714,   288,   150,   151,   152,
     153,   180,   181,   288,   183,   288,   288,   140,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   288,   287,   288,   288,   287,   288,
     287,   288,   180,   181,   288,   183,   298,   299,   295,   298,
     299,   298,   299,   180,   181,   290,   183,     7,   290,     9,
     946,    11,    12,    13,    14,    15,   290,    17,   287,   288,
     290,    21,   290,    23,   180,   181,    26,   183,   290,   298,
     299,   288,    32,    33,   287,   288,    36,   288,   288,    39,
     288,   290,   288,   290,   288,   298,   299,   288,   290,   294,
      50,   290,   290,    53,   290,    55,    56,    57,    58,    59,
      60,   288,    62,    63,    64,   290,    66,   287,   288,    69,
     294,   288,   290,    73,   290,   290,   290,   290,   298,   299,
     290,    81,    82,   290,   290,   290,   290,   290,   290,   290,
      90,   288,   290,   288,   290,   295,    96,   296,    98,   290,
     100,   101,   296,   288,   294,   299,   295,   107,   108,   109,
     294,   111,   295,   295,   295,   295,   295,   295,   294,   119,
     120,   121,   122,   123,   295,   294,   126,   127,   128,   129,
     130,   290,   132,   288,   288,   135,   136,   137,   290,   290,
     290,   290,   290,   143,   144,   145,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   290,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   290,
     290,   290,   290,   290,   290,   290,   288,   290,   947,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   290,   290,   290,   288,   291,   291,   432,
     290,   288,   290,   288,   290,   438,   290,   440,   290,   290,
     290,   290,   290,   290,   162,   290,   290,   290,   290,   290,
     296,   290,   290,   299,   288,   290,   292,   290,   290,   290,
     463,   288,   290,   466,   290,   290,   552,   296,   471,   290,
     299,   290,   290,   290,   290,   290,   290,   290,   290,   482,
     290,   290,   290,   486,   290,   290,   288,   490,   292,   290,
     301,  1080,   292,   296,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   288,   292,   292,   292,   296,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   288,   292,   292,   292,   296,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   288,   292,   292,
     295,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   292,   292,   292,
     292,   292,  1151,   566,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   292,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   443,   292,   292,   292,   292,   292,   292,
     292,   292,   452,   292,   292,   292,   292,  1216,   292,   292,
     292,   461,   292,   290,   290,   290,   290,   467,   468,   290,
     290,   290,   290,   290,   290,   290,   476,   290,   301,   479,
     290,   481,   290,   290,   290,   485,   288,   298,   488,   489,
     288,   288,   292,   292,   298,   298,   292,   497,   292,   292,
     500,   501,   502,   298,   295,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   295,   295,   295,   295,   525,   294,   292,   295,   295,
     292,   295,   295,   162,   295,   295,   295,   295,   295,   295,
     295,   292,   294,   292,   292,   292,   290,   290,   288,   290,
     292,   288,   290,   553,   860,   288,   556,   296,   864,   294,
     290,   290,   296,   869,   292,   292,   872,   567,   568,   569,
     570,   744,   295,   292,   747,   292,   749,   292,   292,   752,
     292,   754,   292,   292,   292,   296,   292,   292,   761,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   292,   292,   162,   299,   299,   295,   299,   162,   162,
     290,   804,   299,   299,   299,   299,   299,   294,   299,   295,
     295,   290,   815,   295,   162,   296,   819,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   295,   162,   296,   182,   296,   185,   186,   187,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   296,   288,   293,
     162,   292,   185,   186,   187,   297,   292,   292,   297,   297,
     297,   294,   292,   297,   292,   297,   297,   292,   297,   292,
     297,   297,   732,   297,   162,   297,   162,   294,   162,   298,
     292,   292,   298,   292,   292,   745,   178,   292,   292,   292,
     292,   292,   292,   753,   927,   292,   929,   930,   931,   178,
     933,   292,   292,   292,   937,   292,   292,   288,   290,   297,
     290,   290,   297,   290,   162,   775,   162,   777,   778,   294,
     162,   954,   162,   297,   297,   297,   297,   297,   297,   162,
     297,  1097,   162,   178,   178,   155,   156,   157,   293,  1105,
     178,   161,   302,   163,  1110,   297,   297,  1113,   808,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   297,   297,   188,   290,
     190,   288,   290,   288,   194,   195,   196,   197,   288,   199,
     200,   201,   202,   203,   204,   205,   206,   290,   208,   209,
     290,   290,   290,   213,   214,   215,   216,   153,    -1,    -1,
    1072,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,    -1,   234,   235,    -1,    -1,   238,   239,
     240,   241,   242,    -1,    -1,   245,    -1,   247,    -1,   249,
      -1,    -1,    -1,  1066,    -1,  1068,  1069,    -1,    -1,    -1,
    1073,    -1,    -1,  1076,  1077,  1078,    -1,    -1,    -1,    -1,
      -1,  1084,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   928,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   936,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   949,
     950,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1139,  1140,  1141,    -1,
      -1,  1144,    -1,    -1,  1147,  1148,    -1,    -1,    -1,    -1,
      -1,  1154,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
      -1,    -1,    -1,    -1,    -1,   185,   186,   187,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,    -1,    -1,    -1,   182,    -1,
      -1,    -1,  1205,  1206,  1207,    -1,    -1,  1210,  1211,  1212,
    1213,  1214,    -1,    -1,    -1,    -1,  1219,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,    -1,    -1,    -1,   182,    -1,    -1,
      -1,    -1,  1245,    -1,  1247,  1075,  1249,  1250,  1251,  1252,
      -1,  1081,  1082,    -1,  1257,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,    -1,    -1,    -1,    -1,    -1,   185,   186,
     187,    -1,  1285,  1286,  1287,    -1,  1289,  1290,    -1,  1375,
      -1,    -1,    -1,    -1,  1297,  1381,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,    -1,    -1,  1145,  1146,    -1,    -1,  1149,
    1323,  1324,  1152,  1153,    -1,    -1,    -1,    -1,  1331,    -1,
      -1,    -1,    -1,    -1,  1337,    -1,  1339,    -1,    -1,  1342,
    1343,  1344,  1345,    -1,    -1,  1348,  1349,    -1,  1351,    -1,
    1353,  1354,    -1,    -1,  1357,  1358,    -1,    -1,  1361,    -1,
    1363,  1364,  1365,  1366,  1367,  1368,  1369,  1370,  1371,    -1,
    1373,  1374,    -1,    -1,  1377,  1378,  1379,    -1,    -1,    -1,
    1383,    -1,    -1,    -1,    -1,  1215,    -1,  1217,  1218,    -1,
      -1,    -1,    -1,   583,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1254,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,    -1,    -1,   634,   635,    -1,   637,    -1,   639,
     640,    -1,    -1,    -1,   644,    -1,    -1,   647,    -1,    -1,
      -1,   651,    -1,    -1,  1294,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   666,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1326,  1327,    -1,    -1,
    1330,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1340,    -1,    -1,    -1,    -1,    -1,    -1,   707,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1376,    -1,    -1,    -1,
    1380,    -1,  1382,    -1,  1384,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   829,
     830,   831,    -1,   833,    -1,    -1,   836,   837,   838,   839,
     840,   841,   842,   843,   844,   845,   846,   847,   848,   849,
      -1,    -1,   852,    -1,    -1,    -1,    -1,    -1,   858,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   866,    -1,    -1,    -1,
      -1,    -1,    -1,   873,   874,   875,   876,   877,   878,   879,
     880,    -1,   882,    -1,    -1,    -1,   886,   887,   888,   889,
     890,   891,   892,   893,    -1,    -1,    -1,    -1,    -1,    -1,
     900,   901,   902,    -1,    -1,    -1,   906,   907,   908,   909,
     910,    -1,    -1,    -1,   914,    -1,    -1,   917,    -1,   919,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   965,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   999,
      -1,    -1,    -1,    -1,  1004,    -1,  1006,    -1,    -1,    -1,
    1010,    -1,    -1,  1013,    -1,    -1,    -1,  1017,    -1,  1019,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1093,    -1,    -1,    -1,    -1,  1098,  1099,
      -1,  1101,    -1,  1103,    -1,    -1,  1106,  1107,    -1,  1109,
      -1,  1111,    -1,    -1,  1114,  1115,    -1,  1117,    -1,  1119,
      -1,    -1,    -1,    -1,  1124,  1125,  1126,  1127,  1128,  1129,
    1130,  1131,  1132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1156,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1223,    -1,    -1,    -1,  1227,    -1,    -1,
      -1,    -1,    -1,  1233,  1234,  1235,    -1,    -1,    -1,    -1,
    1240,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1263,    -1,    -1,    -1,    -1,  1268,    -1,
    1270,  1271,    -1,    -1,     0,    -1,  1276,     3,  1278,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    -1,    30,    31,    -1,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,    -1,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,    -1,    -1,    -1,    -1,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   181,    -1,    -1,    -1,    -1,
      -1,    -1,   188,   189,   190,   191,   192,   193,   194,   195,
     196,    -1,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,    -1,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   181,    -1,   289,   290,    -1,    -1,    -1,    -1,   189,
     190,   191,   192,   193,   194,   195,   196,    -1,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,    -1,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   304,     0,     3,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    30,    31,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   154,   155,   156,   157,   158,   159,
     160,   161,   181,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     289,   290,   305,   306,   307,   308,   309,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,   392,
     393,   394,   395,   396,   397,   398,   399,   400,   401,   402,
     403,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     423,   424,   425,   426,   427,   428,   429,   430,   431,   432,
     433,   434,   435,   436,   437,   438,   439,   440,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
     473,   474,   475,   476,   477,   288,   298,   299,   483,   483,
     483,   288,   481,   481,   483,   481,   483,   481,   481,   481,
     481,   481,   290,   481,   290,   290,   288,   481,   290,   481,
     290,   288,   481,   290,   290,   290,   290,   290,   481,   481,
     290,   288,   481,   483,   290,   481,   288,   288,   288,   288,
     288,   288,   288,   290,   290,   290,   481,   290,   290,   481,
     290,   481,   481,   481,   481,   481,   481,   288,   481,   481,
     481,   288,   488,   481,   483,   288,   481,   288,   290,   290,
     481,   288,   288,   288,   290,   290,   290,   290,   481,   481,
     288,   290,   290,   290,   288,   290,   290,   481,   290,   290,
     290,   290,   290,   481,   488,   481,   481,   481,   483,   483,
     483,   483,   483,   481,   481,   481,   483,   481,   483,   483,
     483,   290,   290,   483,   483,   481,   481,   481,   481,   481,
     290,   290,   481,   481,   481,   481,   481,   288,   290,   481,
     290,   288,   481,   481,   481,   290,   483,   150,   151,   152,
     153,   290,   481,   481,   481,   287,   288,   298,   299,   487,
     288,   487,   487,   295,   307,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   294,   489,   489,   489,   294,   489,   294,   487,   487,
     487,   489,   487,   489,   487,   487,   294,   489,   489,   489,
     489,   489,   489,   489,   489,   489,   489,   489,   489,   489,
     489,   489,   489,   489,   295,   295,   489,   295,   489,   295,
     295,   294,   489,   162,   295,   489,   162,   295,   487,   489,
     162,   295,   489,   295,   489,   489,   489,   489,   489,   489,
     489,   489,   489,   294,   489,   294,   295,   294,   489,   489,
     489,   489,   489,   489,   489,   489,   489,   489,   489,   489,
     489,   489,   489,   489,   178,   179,   185,   186,   187,   489,
     178,   179,   185,   186,   187,   489,   180,   181,   183,   180,
     181,   183,   489,   489,   489,   489,   489,   295,   294,   294,
     489,   294,   294,   489,   294,   489,   290,   288,   288,   290,
     290,   290,   483,   290,   290,   290,   290,   290,   483,   290,
     483,   290,   481,   290,   290,   290,   288,   481,   290,   290,
     481,   290,   483,   290,   483,   481,   481,   288,   291,   483,
     290,   481,   481,   481,   483,   288,   290,   481,   483,   290,
     481,   481,   483,   290,   291,   300,   478,   479,   480,   483,
     484,   290,   290,   290,   290,   481,   481,   481,   481,   290,
     290,   290,   290,   290,   290,   288,   290,   481,   290,   290,
     290,   288,   290,   290,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   290,   488,   481,   290,   290,   481,   290,
     290,   290,   290,   290,   290,   483,   481,   481,   481,   481,
     290,   288,   288,   292,   489,   288,   298,   299,   482,   292,
     292,   292,   482,   292,   482,   482,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     489,   489,   292,   489,   292,   489,   489,   482,   292,   292,
     296,   489,   292,   292,   296,   489,   292,   292,   292,   296,
     489,   292,   296,   292,   292,   292,   292,   292,   292,   292,
     292,   482,   292,   482,   489,   482,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   489,   482,   482,   292,   482,   482,   292,   482,   292,
     290,   290,   290,   481,   288,   290,   290,   483,   481,   288,
     483,   291,   485,   483,   290,   483,   481,   483,   290,   290,
     290,   290,   483,   290,   301,   301,   484,   481,   290,   481,
     481,   290,   288,   290,   483,   481,   483,   290,   290,   290,
     483,   288,   298,   288,   288,   292,   489,   489,   489,   292,
     489,   292,   292,   489,   489,   489,   489,   489,   489,   489,
     489,   489,   489,   489,   489,   489,   489,   178,   179,   185,
     186,   187,   489,   298,   180,   181,   183,   298,   298,   292,
     489,   295,   162,   487,   292,   298,   295,   295,   162,   487,
     292,   298,   489,   295,   295,   162,   487,   292,   298,   295,
     487,   489,   489,   489,   489,   489,   489,   489,   489,   292,
     489,   292,   489,   489,   489,   489,   489,   489,   489,   489,
     295,   295,   295,   295,   295,   295,   489,   489,   489,   295,
     295,   295,   489,   489,   489,   489,   489,   292,   489,   292,
     292,   489,   292,   489,   290,   290,   483,   481,   483,   483,
     483,   486,   292,   483,   290,   481,   483,   288,   288,   484,
     482,   481,   481,   290,   483,   288,   290,   290,   292,   489,
     162,   295,   162,   292,   292,   292,   489,   296,   292,   292,
     489,   292,   296,   299,   489,   296,   292,   292,   489,   292,
     296,   299,   489,   296,   292,   292,   489,   292,   296,   299,
     489,   292,   162,   295,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   162,   295,   162,   295,   483,   290,   483,
     483,   292,   293,   485,   483,   481,   483,   483,   483,   288,
     290,   482,   481,   481,   483,   290,   295,   296,   178,   179,
     185,   186,   187,   489,   180,   181,   183,   295,   487,   489,
     178,   489,   489,   162,   489,   487,   489,   178,   182,   489,
     489,   162,   487,   489,   487,   489,   178,   182,   489,   489,
     162,   489,   182,   296,   489,   489,   489,   489,   489,   489,
     489,   489,   489,   296,   296,   288,   483,   483,   483,   293,
     483,   481,   481,   483,   483,   481,   482,   481,   481,   483,
     489,   162,   489,   292,   297,   297,   297,   292,   292,   297,
     297,   297,   297,   292,   292,   292,   297,   297,   297,   297,
     292,   297,   162,   162,   162,   483,   483,   483,   483,   483,
     483,   483,   483,   481,   482,   481,   481,   483,   298,   292,
     298,   178,   489,   292,   292,   292,   489,   178,   292,   292,
     292,   292,   489,   489,   178,   489,   292,   292,   292,   292,
     489,   292,   292,   292,   292,   483,   288,   483,   290,   483,
     483,   483,   483,   290,   481,   290,   290,   483,   178,   297,
     297,   489,   162,   162,   297,   297,   489,   162,   489,   489,
     297,   297,   297,   297,   489,   162,   489,   162,   297,   162,
     178,   178,   178,   483,   483,   483,   483,   483,   293,   302,
     481,   483,   297,   297,   297,   297,   483,   483,   290,   481,
     481,   481,   483,   288,   483,   483,   481,   290,   483,   483,
     483,   483,   290,   483,   483,   288,   483,   483,   483,   290,
     290,   483,   483,   483,   288,   483,   483,   483,   483,   483,
     483,   483,   483,   483,   483,   483,   488,   481,   483,   483,
     483,   481,   488,   481,   483,   481,   290
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   303,   304,   304,   305,   305,   305,   305,   305,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   306,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   309,
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,   378,   379,
     380,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   397,   398,
     399,   400,   401,   402,   403,   404,   405,   406,   407,   408,
     409,   410,   411,   412,   413,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   422,   423,   424,   425,   426,   427,
     428,   429,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   478,   479,   480,   481,   482,   482,   482,   483,   483,
     483,   484,   484,   485,   486,   486,   487,   487,   487,   487,
     488,   488,   489,   489,   489,   489,   489,   489,   489,   489,
     489,   489,   489,   489,   489,   489,   489,   489
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     3,     6,     9,     3,     3,     4,     3,     3,     3,
       3,     4,     3,     4,     3,     4,     3,     3,     2,     4,
       4,     4,     5,     2,     3,     5,     4,     9,    15,    11,
       7,     6,    21,     3,     3,     3,     3,     2,     2,     3,
       3,     2,     2,     2,     4,     4,     3,     2,     2,     2,
       2,     2,     3,     4,     2,     1,     1,     3,    34,     2,
       3,     5,     3,     4,     2,     2,     5,     2,     2,     2,
       2,     2,     2,     2,     3,     9,     9,     3,     2,    12,
       2,     2,     4,     2,     2,     2,     3,     3,     3,     3,
       3,     4,    15,     4,     4,     5,    13,     3,     3,     3,
       3,     3,     6,     4,     2,     2,     3,     2,     3,     2,
       3,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     5,     3,     3,     3,     2,
       2,     2,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     4,     2,     2,     3,     3,     2,     1,     2,
       1,     1,     2,     2,     2,     2,     4,     4,     5,     4,
       4,     4,     5,    10,     2,     2,     2,     2,     2,     2,
       2,     2,     1,     1,     1,     4,     4,     4,     4,     4,
       2,     2,     4,     5,     4,     4,     4,     4,     2,     4,
       4,     4,     4,     3,     3,     4,     4,     4,     4,     4,
       4,     6,     6,     6,     6,     6,     6,     4,     4,     4,
       6,     6,     6,     1,     8,     8,     4,     5,     5,     5,
       5,     5,     5,     6,     6,     6,     6,     6,     6,     9,
       9,     9,     9,     9,     9,     5,     9,     9,     4,     9,
       9,     9,     9,     9,     9,     9,     9,     9,     9,     9,
       9,     9,     2,     4,     4,     4,     4,     4,     4,     4,
       1,     4,     4,     5,    10,     3,     3,     2,     2,     2,
       2,     1,     1,     1,     1,     4,     2,     2,     4,     2,
       2,     2,     2,     2,     2,     2,     2,     1,     4,     4,
       4,     4,     4,     4,     6,     6,     6,     6,     6,     6,
       4,     4,     4,     6,     6,     6,     4,     4,     4,     4,
       4,     3,     3,     4,     5,    10,     4,     5,    10,     4,
       1,     1,     8,     8,     1,     1,     2,     2,     1,     2,
       2,     1,     1,     3,     1,     3,     1,     1,     2,     2,
       0,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 180:
#line 629 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				mexit(yyengine, "Exiting as per user's request.", 0);
			}
		}
#line 2758 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 181:
#line 641 "sf.y" /* yacc.c:1646  */
    {
			/*
			 *	Only give the architecture type:
			 */
			if (!yyengine->scanning)
			{
				m_newnode(yyengine, (yyvsp[-1].str), 0, 0, 0, NULL, 0, 0);
			}
		}
#line 2772 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 182:
#line 651 "sf.y" /* yacc.c:1646  */
    {
			/*
			 *	Give the architecture type and the (fixed) x/y/z location
			 */
			if (!yyengine->scanning)
			{
				m_newnode(yyengine, (yyvsp[-4].str), (yyvsp[-3].rval)->dval, (yyvsp[-3].rval)->dval, (yyvsp[-3].rval)->dval, NULL, 0, 0);
			}
		}
#line 2786 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 183:
#line 661 "sf.y" /* yacc.c:1646  */
    {
			/*
			 *	Give the architecture type and the (initial) x/y/z location, and the
			 *	trajectory file, loop flag, and trajectory rate (the number of picoseconds
			 *	per sample in the trajectory file).
			 */
			if (!yyengine->scanning)
			{
				//	rdimm example: 	READ THE FOLLOWING!!!!
				//
				//	this is an example of using rdimm.  For rdimm,
				//	you can't get the value from, e.g., $4, but have to do $4.value
				//	The $X.rv field is the packaged rvar state needed to register
				//	the corresponding parameter using m_register_rvar(), if the
				//	rvar was declared as "{...}", or NULL if it was "<...>"
				//
				m_newnode(yyengine, (yyvsp[-7].str), (yyvsp[-6].rval)->dval, (yyvsp[-5].rval)->dval, (yyvsp[-4].rval)->dval, (yyvsp[-3].str), (yyvsp[-2].uval), (yyvsp[-1].uval));

				//	The above should be changed to:
				//m_newnode($2, $3.value, $4.value, $5.value, $6.value, $7.value,
				//		$3.rv, $4.rv, $5.rv, $6.rv, $7.rv);
				//
				//	and then in m_newnode(), check each of the rv args to
				//	see if it is non-null, and if so, do approp. registration
				//	of state as an rvar
			}
		}
#line 2818 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 184:
#line 689 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				batt_nodeattach(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 2829 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 185:
#line 696 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				batt_printstats(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 2840 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 186:
#line 703 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				batt_newbatt(yyengine, (yyvsp[-2].uval), (yyvsp[-1].dval));
			}
		}
#line 2851 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 187:
#line 710 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				if (SF_DEBUG) mprint(yyengine, NULL, siminfo, 
						"Setting yyengine->batts[%d].ileak to %f\n",
						yyengine->curbatt, (yyvsp[-1].dval));
				yyengine->batts[yyengine->curbatt].ileak = (yyvsp[-1].dval);
			}
		}
#line 2865 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 188:
#line 720 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				if (SF_DEBUG) mprint(yyengine, NULL, siminfo, 
						"Setting yyengine->batts[%d].Cf to %f\n",
						yyengine->curbatt, (yyvsp[-1].dval));
				yyengine->batts[yyengine->curbatt].Cf = (yyvsp[-1].dval);
			}
		}
#line 2879 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 189:
#line 730 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				if (SF_DEBUG) mprint(yyengine, NULL, siminfo, 
					"Setting yyengine->batts[%d].Inominal to %f\n",
					yyengine->curbatt, (yyvsp[-1].dval));
				yyengine->batts[yyengine->curbatt].Inominal = (yyvsp[-1].dval);
			}
		}
#line 2893 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 190:
#line 740 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				if (SF_DEBUG) mprint(yyengine, NULL, siminfo, 
						"Setting yyengine->batts[%d].Rf to %f\n",
						yyengine->curbatt, (yyvsp[-1].dval));
				yyengine->batts[yyengine->curbatt].Rf = (yyvsp[-1].dval);
			}
		}
#line 2907 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 191:
#line 750 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				if ((yyvsp[-2].uval) < yyengine->batts[yyengine->curbatt].etaLUTnentries)
				{
					yyengine->batts[yyengine->curbatt].etaLUT[(yyvsp[-2].uval)] = (yyvsp[-1].dval);
				}
				else
				{
					merror(yyengine, "Invalid etaLUT index.");
				}
			}
		}
#line 2925 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 192:
#line 764 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				double *tmp = (double *)mrealloc(yyengine, yyengine->batts[yyengine->curbatt].etaLUT,
						(yyvsp[-1].uval)*sizeof(double), 
						"shasm.y, (double *) for T_BATTETALUTNENTRIES");
				if (tmp == NULL)
				{
					mexit(yyengine, "Could not resize etaLUT: mcrealloc failed.", -1);
				}
				else
				{
					yyengine->batts[yyengine->curbatt].etaLUT = tmp;
					yyengine->batts[yyengine->curbatt].etaLUTnentries = (yyvsp[-1].uval);
				}
			}
		}
#line 2947 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 193:
#line 782 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				if ((yyvsp[-2].uval) < yyengine->batts[yyengine->curbatt].VbattLUTnentries)
				{
					yyengine->batts[yyengine->curbatt].VbattLUT[(yyvsp[-2].uval)] = (yyvsp[-1].dval);
				}
				else
				{
					merror(yyengine, "Invalid VbattLUT index.");
				}
			}
		}
#line 2965 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 194:
#line 796 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				double *tmp = (double *)mrealloc(yyengine, yyengine->batts[yyengine->curbatt].VbattLUT,
						(yyvsp[-1].uval)*sizeof(double), 
						"shasm.y, (double *) for T_BATTVBATTLUTNENTRIES");
				if (tmp == NULL)
				{
					mexit(yyengine, "Could not resize VbattLUT: mrealloc() failed", -1);
				}
				else
				{
					yyengine->batts[yyengine->curbatt].VbattLUT = tmp;
					yyengine->batts[yyengine->curbatt].VbattLUTnentries = (yyvsp[-1].uval);
				}
			}
		}
#line 2987 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 195:
#line 814 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				if ((yyvsp[-2].uval) < yyengine->batts[yyengine->curbatt].VlostLUTnentries)
				{
					yyengine->batts[yyengine->curbatt].VlostLUT[(yyvsp[-2].uval)] = (yyvsp[-1].dval);
				}
				else
				{
					merror(yyengine, "Invalid VlostLUT index.");
				}
			}
		}
#line 3005 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 196:
#line 828 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				double *tmp = (double *)mrealloc(yyengine, yyengine->batts[yyengine->curbatt].VlostLUT,
						(yyvsp[-1].uval)*sizeof(double), 
						"shasm.y, (double *) for T_BATTVLOSTLUTNENTRIES");
				if (tmp == NULL)
				{
					mexit(yyengine, "Could not resize Vlost: mrealloc failed.", -1);
				}
				else
				{
					yyengine->batts[yyengine->curbatt].VlostLUT = tmp;
					yyengine->batts[yyengine->curbatt].VlostLUTnentries = (yyvsp[-1].uval);
				}
			}
		}
#line 3027 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 197:
#line 846 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				if (((yyvsp[-1].uval) >= yyengine->nbatts) || ((yyvsp[-1].uval) < 0))
				{
					merror(yyengine, "Battery ID out of range.");
				}
				else
				{
					yyengine->curbatt = (yyvsp[-1].uval);
				}
			}
		}
#line 3045 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 198:
#line 860 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_pcbacktrace(yyengine, yyengine->cp);
			}
		}
#line 3056 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 199:
#line 867 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_setbptglobaltime(yyengine, (yyvsp[-1].uval));
			}
		}
#line 3067 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 200:
#line 874 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_setbptcycles(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 3078 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 201:
#line 881 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_setbptinstrs(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 3089 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 202:
#line 888 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_setbptsensorreading(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-1].dval));
			}
		}
#line 3100 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 203:
#line 895 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_bptls(yyengine);
			}
		}
#line 3111 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 204:
#line 902 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_bptdel(yyengine, (yyvsp[-1].uval));
			}
		}
#line 3122 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 205:
#line 909 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_setloc(yyengine, yyengine->cp, (yyvsp[-3].dval), (yyvsp[-2].dval), (yyvsp[-1].dval));
			}
		}
#line 3133 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 206:
#line 916 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				network_setretryalg(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-1].str));
			}
		}
#line 3144 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 207:
#line 923 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_randprint(yyengine, (yyvsp[-7].str), (yyvsp[-6].dval), (yyvsp[-5].dval), (yyvsp[-4].dval), (yyvsp[-3].dval), (yyvsp[-2].dval), (yyvsp[-1].dval));
			}
		}
#line 3155 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 208:
#line 930 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				//m_registerrvar(yyengine->cp, $2, $3, $4, $5, $6, $7,
				//		$8, $9, $10, $11, $12, $13, $14);
			}
		}
#line 3167 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 209:
#line 938 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_initrandtable(yyengine, (yyvsp[-9].str), (yyvsp[-8].str), (yyvsp[-7].dval), (yyvsp[-6].dval), (yyvsp[-5].dval), (yyvsp[-4].dval), (yyvsp[-3].dval), (yyvsp[-2].dval), (yyvsp[-1].dval));
			}
		}
#line 3178 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 210:
#line 945 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				//m_defndist($2, $4, $6);

			}
		}
#line 3190 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 211:
#line 953 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				if (yyengine->cp->machinetype == MACHINE_SUPERH)
				{
					//superHhwSEEreg($2, $3, $4, $5);
				}
				else if (yyengine->cp->machinetype == MACHINE_MSP430)
				{
					//msp430hwSEEreg($2, $3, $4, $5);
				}
				else
				{
					sfatal(yyengine, yyengine->cp, "Unknown machine type");
				}
			}
		}
#line 3212 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 212:
#line 971 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				if (yyengine->cp->machinetype == MACHINE_SUPERH)
				{
					//superHinitSEEstate(yyengine->cp, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21);
				}
				else if (yyengine->cp->machinetype == MACHINE_MSP430)
				{
					//msp430initSEEstate(yyengine->cp, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21);
				}
				else
				{
					sfatal(yyengine, yyengine->cp, "Unknown machine type");
				}
			}
		}
#line 3234 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 213:
#line 989 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->ignoredeaths = (yyvsp[-1].uval);
			}
		}
#line 3245 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 214:
#line 996 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->voltscale_alpha = (yyvsp[-1].dval);
			}
		}
#line 3256 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 215:
#line 1003 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->voltscale_K = (yyvsp[-1].dval);
			}
		}
#line 3267 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 216:
#line 1010 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->voltscale_Vt = (yyvsp[-1].dval);
			}
		}
#line 3278 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 217:
#line 1017 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->schedtype = SchedRandom;
			}
		}
#line 3289 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 218:
#line 1024 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->schedtype = SchedRoundRobin;
			}
		}
#line 3300 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 219:
#line 1031 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->quantum = (yyvsp[-1].uval);
			}
		}
#line 3311 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 220:
#line 1038 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->baseid = (yyvsp[-1].uval);
			}
		}
#line 3322 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 221:
#line 1045 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_renumbernodes(yyengine);
			}
		}
#line 3333 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 222:
#line 1052 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->trip_ustart = musercputimeusecs();
				yyengine->cp->trip_startclk = yyengine->cp->ICLK;
			}
		}
#line 3345 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 223:
#line 1060 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				int i;

				for (i = 0; i < yyengine->nnodes; i++)
				{
					yyengine->sp[i]->trip_ustart = musercputimeusecs();
					yyengine->sp[i]->trip_startclk = yyengine->sp[i]->ICLK;
				}
			}
		}
#line 3362 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 224:
#line 1073 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				network_netseg2file(yyengine, (yyvsp[-2].uval), (yyvsp[-1].str));
			}
		}
#line 3373 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 225:
#line 1080 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				network_file2netseg(yyengine, (yyvsp[-2].str), (yyvsp[-1].uval));
			}
		}
#line 3384 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 226:
#line 1087 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				int	n = mchdir((yyvsp[-1].str));

				if (n < 0)
				{
					merror(yyengine, "Could not change directory to \"%s\".", (yyvsp[-1].str));
				}
			}
		}
#line 3400 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 227:
#line 1099 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->settimerintrdelay(yyengine, yyengine->cp, (yyvsp[0].dval));
			}
		}
#line 3411 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 228:
#line 1106 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->phyperiodpsec = (yyvsp[0].dval);
			}
		}
#line 3422 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 229:
#line 1113 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->battperiodpsec = (yyvsp[0].dval);
			}
		}
#line 3433 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 230:
#line 1120 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->netperiodpsec = (yyvsp[0].dval);
			}
		}
#line 3444 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 231:
#line 1127 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->fperiodpsec = (yyvsp[0].dval);
			}
		}
#line 3455 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 232:
#line 1135 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->force_avgpwr = (yyvsp[-1].dval);
				yyengine->cp->force_sleeppwr = (yyvsp[0].dval);
			}
		}
#line 3467 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 233:
#line 1143 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				network_netsegpropmodel(yyengine, (yyvsp[-2].uval), (yyvsp[-1].uval), (yyvsp[0].dval));
			}
		}
#line 3478 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 234:
#line 1150 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->dumpperiodpsec = (yyvsp[0].dval);
			}
		}
#line 3489 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 235:
#line 1157 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_version(yyengine);
			}
		}
#line 3500 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 236:
#line 1164 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				physics_sensorsdbg(yyengine);
			}
		}
#line 3511 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 237:
#line 1172 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				physics_sigsubscr(yyengine, yyengine->cp, (yyvsp[-1].uval), (yyvsp[0].uval));
			}
		}
#line 3522 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 238:
#line 1183 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				physics_newsigsrc(yyengine, (yyvsp[-32].uval), (yyvsp[-31].str), (yyvsp[-30].dval), (yyvsp[-29].dval), (yyvsp[-28].dval), (yyvsp[-27].dval), (yyvsp[-26].dval), (yyvsp[-25].dval), (yyvsp[-24].dval),
					(yyvsp[-23].dval), (yyvsp[-22].dval), (yyvsp[-21].dval), (yyvsp[-20].dval), (yyvsp[-19].dval), (yyvsp[-18].dval), (yyvsp[-17].dval), (yyvsp[-16].dval), (yyvsp[-15].dval), (yyvsp[-14].dval),
					(yyvsp[-13].dval), (yyvsp[-12].dval), (yyvsp[-11].dval), (yyvsp[-10].str), (yyvsp[-9].uval), (yyvsp[-8].dval), (yyvsp[-7].dval), (yyvsp[-6].dval), (yyvsp[-5].uval), (yyvsp[-4].str),
					(yyvsp[-3].uval), (yyvsp[-2].dval), (yyvsp[-1].uval));
			}
		}
#line 3536 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 239:
#line 1193 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				mprint(yyengine, NULL, siminfo,
					"Current directory: %s\n", mgetpwd());
			}
		}
#line 3548 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 240:
#line 1201 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_parseobjdump(yyengine, yyengine->cp, (yyvsp[-1].str));
			}
		}
#line 3559 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 241:
#line 1208 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_dumpall(yyengine, (yyvsp[-3].str), M_OWRITE, (yyvsp[-2].str), (yyvsp[-1].str));
			}
		}
#line 3570 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 242:
#line 1215 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				if ((yyvsp[-1].uval) >= yyengine->nnodes)
				{
					merror(yyengine, "Node ID out of range.");
				}
				else
				{
					m_setnode(yyengine, (yyvsp[-1].uval));
				}
			}
		}
#line 3588 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 243:
#line 1229 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				if (((yyvsp[-2].uval) > yyengine->nnodes) || ((yyvsp[-2].uval) < 0)\
					|| ((yyvsp[-1].uval) > yyengine->nnodes) || ((yyvsp[-1].uval) < 0))
				{
					merror(yyengine, "Node indeces out of range in call to MMAP!");
				}
				else
				{
					yyengine->sp[(yyvsp[-1].uval)]->MEM  = yyengine->sp[(yyvsp[-2].uval)]->MEM;
					mprint(yyengine, NULL, siminfo,
						"Mapped mem of Node " ULONGFMT " into Node " ULONGFMT "\n", (yyvsp[-2].uval), (yyvsp[-1].uval));
				}
			}
		}
#line 3609 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 244:
#line 1246 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->step = yyengine->cp->cyclestep;
			}
		}
#line 3620 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 245:
#line 1253 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->step = yyengine->cp->faststep;
			}
		}
#line 3631 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 246:
#line 1260 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->cache_init(yyengine, yyengine->cp, (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3642 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 247:
#line 1267 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->cache_deactivate(yyengine, yyengine->cp);
			}
		}
#line 3653 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 248:
#line 1274 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->cache_printstats(yyengine, yyengine->cp);
			}
		}
#line 3664 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 249:
#line 1281 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->dumpregs(yyengine, yyengine->cp);
			}
		}
#line 3675 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 250:
#line 1288 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->dumpsysregs(yyengine, yyengine->cp);
			}
		}
#line 3686 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 251:
#line 1295 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->dumppipe(yyengine, yyengine->cp);
			}
		}
#line 3697 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 252:
#line 1302 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->resetcpu(yyengine, yyengine->cp);
			}
		}
#line 3708 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 253:
#line 1309 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				mprint(yyengine, yyengine->cp, nodeinfo,
					"Dynamic instruction count = [" UVLONGFMT "], fetched instructions = [" UVLONGFMT "]\n",
					yyengine->cp->dyncnt, yyengine->cp->nfetched);	
			}
		}
#line 3721 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 254:
#line 1318 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->nodetach = (yyvsp[-1].uval);
			}
		}
#line 3732 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 255:
#line 1325 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_addvaluetrace(yyengine, yyengine->cp, (yyvsp[-7].str), (yyvsp[-6].uval), (yyvsp[-5].uval), (yyvsp[-4].uval), (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3743 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 256:
#line 1332 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_delvaluetrace(yyengine, yyengine->cp, (yyvsp[-7].str), (yyvsp[-6].uval), (yyvsp[-5].uval), (yyvsp[-4].uval), (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3754 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 257:
#line 1339 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_readstabs(yyengine, yyengine->cp, (yyvsp[-1].str));
			}
		}
#line 3765 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 258:
#line 1346 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_valuestats(yyengine, yyengine->cp);
			}
		}
#line 3776 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 259:
#line 1353 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_numaregion(yyengine, (yyvsp[-10].str), (yyvsp[-9].uval), (yyvsp[-8].uval), (yyvsp[-7].sval), (yyvsp[-6].sval), (yyvsp[-5].sval), (yyvsp[-4].sval), (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3787 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 260:
#line 1360 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_numastats(yyengine, yyengine->cp);
			}
		}
#line 3798 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 261:
#line 1367 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_numastatsall(yyengine);
			}
		}
#line 3809 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 262:
#line 1374 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_numasetmapid(yyengine, (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3820 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 263:
#line 1381 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->dumptlb(yyengine, yyengine->cp);
			}
		}
#line 3831 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 264:
#line 1388 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				if (yyengine->cp->ICLK > 0)
				{
					mprint(yyengine, yyengine->cp, nodeinfo,
						"Ratio of active/sleep cycles = %.6f\n",
						(float)yyengine->cp->CLK/(float)yyengine->cp->ICLK);
				}
				else
				{
					merror(yyengine, "We don't stand a chance.");
				}
			}
		}
#line 3851 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 265:
#line 1404 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				mprint(yyengine, NULL, siminfo,
					"Simulation random seed = %ld\n", yyengine->randseed);
			}
		}
#line 3863 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 266:
#line 1412 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				mprint(yyengine, NULL, siminfo,
					"Resetting random number generation with seed [%ld]\n", (yyvsp[-1].uval));
				yyengine->randseed = mrandominit(yyengine, (yyvsp[-1].uval));
			}
		}
#line 3876 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 267:
#line 1421 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->ENABLE_BATT_LOW_INTR = (yyvsp[-1].uval);
			}
		}
#line 3887 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 268:
#line 1428 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->battery_alert_frac = (yyvsp[-1].dval);
			}
		}
#line 3898 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 269:
#line 1435 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->fail_prob = (yyvsp[-1].dval);
			}
		}
#line 3909 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 270:
#line 1442 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->failure_duration_max = (yyvsp[-1].uval);
			}
		}
#line 3920 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 271:
#line 1449 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				if ((yyvsp[-2].uval) >= MAX_NETSEGMENTS)
				{
					merror(yyengine, "Segment # > max. number of network segments.");
				}
				else
				{
					yyengine->netsegs[(yyvsp[-2].uval)].failure_duration_max = (yyvsp[-1].uval);
				}
			}
		}
#line 3938 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 272:
#line 1464 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				network_netnewseg(yyengine, (yyvsp[-13].uval), (yyvsp[-12].uval), (yyvsp[-11].uval), (yyvsp[-10].uval), (yyvsp[-9].uval), (yyvsp[-8].uval),
					(yyvsp[-7].dval), (yyvsp[-6].dval), (yyvsp[-5].dval), (yyvsp[-4].uval), (yyvsp[-3].dval), (yyvsp[-2].dval), (yyvsp[-1].dval));
			}
		}
#line 3950 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 273:
#line 1472 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				if ((yyvsp[-2].uval) >= yyengine->nnetsegs)
				{
					merror(yyengine, "NETSEGMENT out of range.");
				}
				else
				{
					yyengine->netsegs[(yyvsp[-2].uval)].fail_prob = (yyvsp[-1].dval);
				}
			}
		}
#line 3968 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 274:
#line 1486 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				network_netsegnicattach(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3979 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 275:
#line 1493 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				if ((yyvsp[-3].uval) >= MAX_NETSEGMENTS)
				{
					merror(yyengine, "Segment # > max. number of network segments.");
				}
				else if ((yyvsp[-2].uval) >= yyengine->nnodes)
				{
					merror(yyengine, "Invalid node number.");
				}
				else
				{
					yyengine->netsegs[(yyvsp[-3].uval)].correl_coeffs[(yyvsp[-2].uval)] = (yyvsp[-1].dval);
				}
			}
		}
#line 4001 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 276:
#line 1511 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				network_netnodenewifc(yyengine, yyengine->cp, (yyvsp[-11].uval), (yyvsp[-10].dval), (yyvsp[-9].dval), (yyvsp[-8].dval),
					(yyvsp[-7].dval), (yyvsp[-6].uval), (yyvsp[-5].dval), (yyvsp[-4].dval), (yyvsp[-3].dval), (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 4013 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 277:
#line 1519 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				network_netsegdelete(yyengine, (yyvsp[-1].uval));
			}
		}
#line 4024 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 278:
#line 1526 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				merror(yyengine, "Command \"NODEFAILPROBFN\" unimplemented.");
			}
		}
#line 4035 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 279:
#line 1533 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				merror(yyengine, "Command \"NETSEGFAILPROBFN\" unimplemented.");
			}
		}
#line 4046 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 280:
#line 1540 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_sizemem(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 4057 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 281:
#line 1547 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				pau_init(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 4068 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 282:
#line 1554 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->split(yyengine, yyengine->cp, (yyvsp[-4].uval), (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].str));
			}
		}
#line 4079 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 283:
#line 1561 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_dumpmem(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 4090 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 284:
#line 1568 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->pipelined = 0;
			}
		}
#line 4101 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 285:
#line 1575 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->pipelined = 1;
			}
		}
#line 4112 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 286:
#line 1582 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->superH->ENABLE_CLK_INTR = (yyvsp[-1].uval);
			}
		}
#line 4123 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 287:
#line 1589 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->runnable = 0;
			}
		}
#line 4134 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 288:
#line 1596 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				sfatal(yyengine, yyengine->cp, (yyvsp[-1].str));
			}
		}
#line 4145 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 289:
#line 1603 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->verbose ^= 1;
			}
		}
#line 4156 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 290:
#line 1610 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_run(yyengine, yyengine->cp, (yyvsp[-1].str));
			}

			/*	The 'string' is dynamically allocated, in lex.c		*/
			free((yyvsp[-1].str));
		}
#line 4170 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 291:
#line 1620 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->step(yyengine, yyengine->cp, 0);
			}
		}
#line 4181 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 292:
#line 1627 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				go(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 4192 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 293:
#line 1634 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				loadcmds(yyengine, (yyvsp[-1].str));
			}
		}
#line 4203 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 294:
#line 1641 "sf.y" /* yacc.c:1646  */
    {	
			if (!yyengine->scanning)
			{
				yyengine->cp->PC = (yyvsp[-1].uval);
			}
		}
#line 4214 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 295:
#line 1648 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				/*	Scale frequency accordingly for provided Vdd	*/
				power_scaledelay(yyengine, yyengine->cp, (yyvsp[-1].dval));
			}
		}
#line 4226 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 296:
#line 1656 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->mem_r_latency = (yyvsp[-1].uval);
			}
		}
#line 4237 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 297:
#line 1663 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->mem_w_latency = (yyvsp[-1].uval);
			}
		}
#line 4248 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 298:
#line 1670 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->flash_r_latency = (yyvsp[-1].uval);
			}
		}
#line 4259 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 299:
#line 1677 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->flash_w_latency = (yyvsp[-1].uval);
			}
		}
#line 4270 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 300:
#line 1684 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				/*	Scale Vdd accordingly for provided frequency	*/
				power_scalevdd(yyengine, yyengine->cp, (yyvsp[-1].dval));
			}
		}
#line 4282 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 301:
#line 1692 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				help(yyengine);
			}
		}
#line 4293 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 302:
#line 1699 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				man(yyengine, (yyvsp[-1].str));
			}
		}
#line 4304 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 303:
#line 1706 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->pipeflush(yyengine->cp);
			}
		}
#line 4315 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 304:
#line 1713 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->pipeshow = !yyengine->cp->pipeshow;
			}
		}
#line 4326 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 305:
#line 1720 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				savemem(yyengine, yyengine->cp, (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].str));
			}
		}
#line 4337 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 306:
#line 1727 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				load_srec(yyengine, yyengine->cp, (yyvsp[-1].str));
			}
		}
#line 4348 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 307:
#line 1734 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				cont(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 4359 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 308:
#line 1741 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_sharebus(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 4370 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 309:
#line 1748 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				mprint(yyengine, NULL, siminfo,
					"CLK = " UVLONGFMT ", ICLK = " UVLONGFMT ", TIME = %E, CYCLETIME = %E\n",
					yyengine->cp->CLK, yyengine->cp->ICLK, yyengine->cp->TIME, 
					yyengine->cp->CYCLETIME);
			}
		}
#line 4384 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 310:
#line 1758 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				power_printstats(yyengine, yyengine->cp);
			}
		}
#line 4395 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 311:
#line 1765 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_locstats(yyengine, yyengine->cp);
			}
		}
#line 4406 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 312:
#line 1772 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_listrvars(yyengine);
			}
		}
#line 4417 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 313:
#line 1780 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_on(yyengine, yyengine->cp);
			}
		}
#line 4428 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 314:
#line 1787 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				m_off(yyengine, yyengine->cp);
			}
		}
#line 4439 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 315:
#line 1794 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				network_netdebug(yyengine, yyengine->cp);
			}
		}
#line 4450 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 316:
#line 1801 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				mprint(yyengine, NULL, siminfo, 
					"WARNING: Trace only works if compiled w/ -MYRMIGKI_DEBUG\n\n");
				yyengine->cp->trace = (yyvsp[-1].uval);
			}
		}
#line 4463 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 317:
#line 1810 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->throttlensec = (yyvsp[-1].uval);
			}
		}
#line 4474 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 318:
#line 1817 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->throttlewin = (yyvsp[-1].uval);
			}
		}
#line 4485 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 319:
#line 1824 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->faultthreshold = (yyvsp[-1].uval);
			}
		}
#line 4496 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 320:
#line 1831 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->ENABLE_TOO_MANY_FAULTS = (yyvsp[-1].uval);
			}
		}
#line 4507 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 321:
#line 1838 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				fault_setnodepfun(yyengine, yyengine->cp, (yyvsp[-1].str));
			}
		}
#line 4518 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 322:
#line 1845 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				if ((yyvsp[-2].uval) < yyengine->cp->superH->NIC_NUM_IFCS)
				{
					msnprint((char*)&yyengine->cp->superH->NIC_IFCS[(yyvsp[-2].uval)].IFC_OUI[0],
						NIC_ADDR_LEN, "%s", (yyvsp[-1].str));
				}
				else
				{
					merror(yyengine, "Network interface [%d] out of range.", (yyvsp[-2].uval));
				}
			}
		}
#line 4537 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 323:
#line 1860 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				mmblocksdisplay(yyengine);
			}
		}
#line 4548 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 324:
#line 1867 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				pau_printstats(yyengine, yyengine->cp);
			}
		}
#line 4559 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 325:
#line 1874 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				// TODO: we should account for the cost of the m_on and m_off

				m_off(yyengine, yyengine->cp);
				mnsleep((yyvsp[-1].uval));
				m_on(yyengine, yyengine->cp);
			}
		}
#line 4574 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 326:
#line 1885 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				// TODO: we should account for the cost of the m_on and m_off

				m_off(yyengine, yyengine->cp);
				mnsleep((yyvsp[-1].uval) * 1000000000);
				m_on(yyengine, yyengine->cp);
			}
		}
#line 4589 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 327:
#line 1896 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				/*	For now, nothing fun is done with comments	*/
			}
		}
#line 4600 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 328:
#line 1903 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				/*								*/
				/*	The case where the command is just a solitary newline 	*/
				/*	is actually short circuited and does not even get here 	*/
				/*	We only parse input if # chars read from input is > 1.	*/
				/*								*/
			}
		}
#line 4615 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 329:
#line 1917 "sf.y" /* yacc.c:1646  */
    {
			/*							*/
			/*	Whether yyengine->scanning or not, forcefully align PC 	*/
			/*							*/
			ulong align = (yyvsp[0].uval);

			if (align == 2)
			{
				mprint(yyengine, NULL, siminfo, "adjusting PC from " ULONGFMT " to " ULONGFMT "\n",
					yyengine->cp->PC, (yyengine->cp->PC+16-(yyengine->cp->PC&0xF)));
				yyengine->cp->PC += 16 - (yyengine->cp->PC & 0xF);
			}
			else if (align == 4)
			{
				mprint(yyengine, NULL, siminfo, "adjusting PC from " ULONGFMT " to " ULONGFMT "\n",
					yyengine->cp->PC, (yyengine->cp->PC+16-(yyengine->cp->PC&0xF)));
				yyengine->cp->PC += 256 - (yyengine->cp->PC & 0xFF);
			}
			else
			{
				merror(yyengine, ".align for arbitrary alignment not implemented !!!");
			}
		}
#line 4643 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 330:
#line 1944 "sf.y" /* yacc.c:1646  */
    {
		}
#line 4650 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 331:
#line 1950 "sf.y" /* yacc.c:1646  */
    {
		}
#line 4657 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 332:
#line 1956 "sf.y" /* yacc.c:1646  */
    {
		}
#line 4664 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 333:
#line 1962 "sf.y" /* yacc.c:1646  */
    {
		}
#line 4671 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 334:
#line 1967 "sf.y" /* yacc.c:1646  */
    {
		}
#line 4678 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 335:
#line 1972 "sf.y" /* yacc.c:1646  */
    {
			/*								*/
			/*	disp is _relative_ to the current pc, so convert it	*/
			/*	to absolute address before laying down the value, by	*/
			/*	just doing the opposite of what we did in rule for	*/
			/*	'disp', at the end of this shasm.y			*/
			/*								*/
			long absdisp = ((yyvsp[0].sval) << 1) + yyengine->cp->PC + 4;

			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else
			{
				mprint(yyengine, NULL, siminfo,
					"Laying down raw data, val = [" ULONGFMT "]\n", absdisp);
				memmove(&yyengine->cp->MEM[yyengine->cp->PC], &absdisp, sizeof(absdisp));

				/*	.long : 32 bits == 4 bytes	*/
				yyengine->cp->PC += 4;
			}
		}
#line 4706 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 336:
#line 1997 "sf.y" /* yacc.c:1646  */
    {
			/*						*/
			/*	My understanding is that the .comm is	*/
			/*	a global var definition. The address 	*/
			/*	in memory of the global var is the	*/
			/*	PC of the .comm defn.			*/
			/*						*/
			if (yyengine->scanning)
			{
				char tmp;
				char *ep = &tmp;

				yyengine->cp->PC += strtol((yyvsp[0].str), &ep, 0);
				
				if (*ep != '\0')
				{
					mprint(yyengine, NULL, siminfo, "invalid size for .comm variable");
				}	
			}
		}
#line 4731 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 337:
#line 2021 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)	
				{
					superH_add(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}
		
				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1100;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 4765 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 338:
#line 2053 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_ni tmp;

				if (!yyengine->cp->pipelined)	
				{
					superH_addi(yyengine, yyengine->cp, (yyvsp[-2].sval), (yyvsp[0].uval));
				}
		
				tmp.imm = ((yyvsp[-2].sval)&B11111111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code = B0111;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
				
			}
		}
#line 4799 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 339:
#line 2085 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_addc(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1110;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 4833 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 340:
#line 2117 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_addv(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1111;
				tmp.code_hi = B0011;
			
				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 4867 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 341:
#line 2149 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_and(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}
	
				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1001;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 4901 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 342:
#line 2181 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_andi(yyengine, yyengine->cp, (yyvsp[-2].sval));
				}
	
				tmp.imm = ((yyvsp[-2].sval)&B11111111);
				tmp.code = B11001001;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 4933 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 343:
#line 2211 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_andm(yyengine, yyengine->cp, (yyvsp[-7].sval));
				}
			
				tmp.imm = ((yyvsp[-7].sval)&B11111111);
				tmp.code = B11001101;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 4965 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 344:
#line 2241 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_bf(yyengine, yyengine->cp, (yyvsp[0].sval) - yyengine->cp->PC);
				}

				tmp.disp = ((yyvsp[0].sval)&B11111111);
				tmp.code = B10001011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5002 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 345:
#line 2276 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_bfs(yyengine, yyengine->cp, (yyvsp[0].sval) - yyengine->cp->PC);
				}
	
				tmp.disp = ((yyvsp[0].sval)&B11111111);
				tmp.code = B10001111;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5039 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 346:
#line 2311 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_d12 tmp;

				if (!yyengine->cp->pipelined)
				{
					/*						*/
					/*	The disp is absolute, and we have to	*/
					/*	pass PC relative to bra(). No need to	*/
					/*	PC-2 as in main.c, since no pipelining	*/
					/*						*/
					superH_bra(yyengine, yyengine->cp, (yyvsp[0].sval) - yyengine->cp->PC);
				}

				tmp.disp = (yyvsp[0].sval);
				tmp.code = B1010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5076 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 347:
#line 2346 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;
	
				if (!yyengine->cp->pipelined)
				{
					superH_braf(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00100011;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5109 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 348:
#line 2377 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d12 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_bsr(yyengine, yyengine->cp, (yyvsp[0].sval) - yyengine->cp->PC);
				}

				tmp.disp = ((yyvsp[0].sval)&B111111111111);
				tmp.code = B1011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5141 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 349:
#line 2407 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_bsrf(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00000011;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5174 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 350:
#line 2438 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_bt(yyengine, yyengine->cp, (yyvsp[0].sval) - yyengine->cp->PC);
				}

				tmp.disp = ((yyvsp[0].sval)&B11111111);
				tmp.code = B10001001;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5211 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 351:
#line 2473 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_bts(yyengine, yyengine->cp, (yyvsp[0].sval) - yyengine->cp->PC);
				}

				tmp.disp = ((yyvsp[0].sval)&B11111111);
				tmp.code = B10001101;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5248 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 352:
#line 2508 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_clrmac(yyengine, yyengine->cp);
				}

				tmp.code = B0000000000101000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5279 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 353:
#line 2537 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_clrs(yyengine, yyengine->cp);
				}

				tmp.code = B0000000001001000;
			
				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5310 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 354:
#line 2566 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_clrt(yyengine, yyengine->cp);
				}

				tmp.code = B0000000000001000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5341 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 355:
#line 2595 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_cmpeq(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0000;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5375 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 356:
#line 2627 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_cmpge(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0011;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5409 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 357:
#line 2659 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_cmpgt(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}
	
				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0111;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5443 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 358:
#line 2691 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_cmphi(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0110; 
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5477 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 359:
#line 2723 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_cmphs(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0010;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5511 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 360:
#line 2755 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_cmppl(yyengine, yyengine->cp, (yyvsp[0].uval));
				}
				tmp.dst = ((yyvsp[0].uval)&B111);
				tmp.code_lo = B00010101;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5543 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 361:
#line 2785 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_cmppz(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00010001;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5576 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 362:
#line 2816 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_cmpstr(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}
		
				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1100;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5610 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 363:
#line 2848 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_cmpim(yyengine, yyengine->cp, (yyvsp[-2].sval));
				}

				tmp.imm = ((yyvsp[-2].sval)&B11111111);
				tmp.code = B10001000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5642 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 364:
#line 2878 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_div0s(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0111;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5676 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 365:
#line 2910 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_div1(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0100;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5710 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 366:
#line 2942 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_dmuls(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1101;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5744 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 367:
#line 2974 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_dmulu(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}
			
				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0101;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5778 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 368:
#line 3006 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_dt(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00010000;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5811 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 369:
#line 3037 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_extsb(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1110;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5845 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 370:
#line 3069 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_extsw(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1111;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5879 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 371:
#line 3101 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_extub(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1100;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5913 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 372:
#line 3133 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_extuw(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1101;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5947 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 373:
#line 3165 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_jmp(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00101011;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 5980 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 374:
#line 3196 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_jsr(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00001011;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6013 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 375:
#line 3227 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcsr(yyengine, yyengine->cp, (yyvsp[-2].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.code_lo = B00001110;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6046 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 376:
#line 3258 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcgbr(yyengine, yyengine->cp, (yyvsp[-2].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.code_lo = B00011110;
				tmp.code_hi = B0100;
			
				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6079 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 377:
#line 3289 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcvbr(yyengine, yyengine->cp, (yyvsp[-2].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.code_lo = B00101110;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6112 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 378:
#line 3320 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcssr(yyengine, yyengine->cp, (yyvsp[-2].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.code_lo = B00111110;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6145 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 379:
#line 3351 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcspc(yyengine, yyengine->cp, (yyvsp[-2].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.code_lo = B01001110;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6178 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 380:
#line 3382 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_mbank tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcr_bank(yyengine, yyengine->cp, (yyvsp[0].uval), (yyvsp[-2].uval));
				}
		
				tmp.code_lo = B1110;
				tmp.reg = ((yyvsp[0].uval)&B111);
				tmp.fill = 1;
				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6213 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 381:
#line 3415 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcmsr(yyengine, yyengine->cp, (yyvsp[-3].uval));
				}

				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.code_lo = B00000111;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6246 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 382:
#line 3446 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else
			{
				instr_m tmp;
	
				if (!yyengine->cp->pipelined)
				{
					superH_ldcmgbr(yyengine, yyengine->cp, (yyvsp[-3].uval));
				}

				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.code_lo = B00010111;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6274 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 383:
#line 3472 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcmvbr(yyengine, yyengine->cp, (yyvsp[-3].uval));
				}

				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.code_lo = B00100111;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6307 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 384:
#line 3503 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m  tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcmssr(yyengine, yyengine->cp, (yyvsp[-3].uval));
				}

				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.code_lo = B00110111;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6340 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 385:
#line 3534 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcmspc(yyengine, yyengine->cp, (yyvsp[-3].uval));
				}

				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.code_lo = B01000111;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6373 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 386:
#line 3565 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_mbank tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldcmr_bank(yyengine, yyengine->cp, (yyvsp[0].uval), (yyvsp[-3].uval));
				}

				tmp.code_lo = B0111;
				tmp.reg = ((yyvsp[0].uval)&B111);
				tmp.fill = 1;
				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6408 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 387:
#line 3598 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldsmach(yyengine, yyengine->cp, (yyvsp[-2].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.code_lo = B00001010;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6441 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 388:
#line 3629 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldsmacl(yyengine, yyengine->cp, (yyvsp[-2].uval));
				}
	
				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.code_lo = B00011010;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6474 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 389:
#line 3660 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldspr(yyengine, yyengine->cp, (yyvsp[-2].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.code_lo = B00101010;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6507 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 390:
#line 3691 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldsmmach(yyengine, yyengine->cp, (yyvsp[-3].uval));
				}

				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.code_lo = B00000110;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6535 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 391:
#line 3717 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldsmmacl(yyengine, yyengine->cp, (yyvsp[-3].uval));
				}

				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.code_lo = B00010110;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6568 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 392:
#line 3748 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_m tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldsmpr(yyengine, yyengine->cp, (yyvsp[-3].uval));
				}

				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.code_lo = B00100110;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6601 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 393:
#line 3779 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ldtlb(yyengine, yyengine->cp);
				}

				tmp.code = B0000000000111000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6632 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 394:
#line 3808 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_macl(yyengine, yyengine->cp, (yyvsp[-5].uval), (yyvsp[-1].uval));
				}

				tmp.src = ((yyvsp[-5].uval)&B1111);
				tmp.dst = ((yyvsp[-1].uval)&B1111);
				tmp.code_lo = B1111;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6666 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 395:
#line 3840 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_macw(yyengine, yyengine->cp, (yyvsp[-5].uval), (yyvsp[-1].uval));
				}

				tmp.src = ((yyvsp[-5].uval)&B1111);
				tmp.dst = ((yyvsp[-1].uval)&B1111);
				tmp.code_lo = B1111;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6700 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 396:
#line 3872 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_mov(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0011;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6734 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 397:
#line 3904 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movbs(yyengine, yyengine->cp, (yyvsp[-3].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0000;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6768 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 398:
#line 3936 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movws(yyengine, yyengine->cp, (yyvsp[-3].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0001;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6802 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 399:
#line 3968 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movls(yyengine, yyengine->cp, (yyvsp[-3].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0010;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6836 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 400:
#line 4000 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movbl(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0000;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6870 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 401:
#line 4032 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movwl(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0001;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6904 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 402:
#line 4064 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movll(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
					tmp.code_lo = B0010;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6938 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 403:
#line 4096 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movbm(yyengine, yyengine->cp, (yyvsp[-4].uval), (yyvsp[0].uval));
				}
	
				tmp.src = ((yyvsp[-4].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0100;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 6972 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 404:
#line 4128 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movwm(yyengine, yyengine->cp, (yyvsp[-4].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-4].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0101;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7006 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 405:
#line 4160 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movlm(yyengine, yyengine->cp, (yyvsp[-4].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-4].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0110;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7040 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 406:
#line 4192 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movbp(yyengine, yyengine->cp, (yyvsp[-3].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0100;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7074 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 407:
#line 4224 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movwp(yyengine, yyengine->cp, (yyvsp[-3].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0101;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7108 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 408:
#line 4256 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movlp(yyengine, yyengine->cp, (yyvsp[-3].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0110;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7142 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 409:
#line 4288 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movbs0(yyengine, yyengine->cp, (yyvsp[-7].uval), (yyvsp[-1].uval));
				}

				tmp.src = ((yyvsp[-7].uval)&B1111);
				tmp.dst = ((yyvsp[-1].uval)&B1111);
				tmp.code_lo = B0100;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7176 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 410:
#line 4320 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movws0(yyengine, yyengine->cp, (yyvsp[-7].uval), (yyvsp[-1].uval));
				}

				tmp.src = ((yyvsp[-7].uval)&B1111);
				tmp.dst = ((yyvsp[-1].uval)&B1111);
				tmp.code_lo = B0101;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7210 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 411:
#line 4352 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movls0(yyengine, yyengine->cp, (yyvsp[-7].uval), (yyvsp[-1].uval));
				}

				tmp.src = ((yyvsp[-7].uval)&B1111);
				tmp.dst = ((yyvsp[-1].uval)&B1111);
				tmp.code_lo = B0110;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7244 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 412:
#line 4384 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movbl0(yyengine, yyengine->cp, (yyvsp[-3].uval), (yyvsp[0].uval));
				}
	
				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1100;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7278 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 413:
#line 4416 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movwl0(yyengine, yyengine->cp, (yyvsp[-3].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1101;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}

		}
#line 7313 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 414:
#line 4449 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movll0(yyengine, yyengine->cp, (yyvsp[-3].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1110;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7347 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 415:
#line 4481 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_ni tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movi(yyengine, yyengine->cp, (yyvsp[-2].sval), (yyvsp[0].uval));
				}

				tmp.imm = ((yyvsp[-2].sval)&B11111111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code = B1110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7380 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 416:
#line 4512 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_nd8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movwi(yyengine, yyengine->cp, (yyvsp[-5].sval) - yyengine->cp->PC, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B11111111);
				tmp.disp = ((yyvsp[-5].sval)&B11111111);
				tmp.code = B1001;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7418 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 417:
#line 4550 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_nd8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movli(yyengine, yyengine->cp, (yyvsp[-5].sval) - yyengine->cp->PC, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B11111111);
				tmp.disp = ((yyvsp[-5].sval)&B11111111);
				tmp.code = B1101;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7456 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 418:
#line 4585 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_nd8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movli(yyengine, yyengine->cp, (yyvsp[-2].sval) - yyengine->cp->PC, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B11111111);
				tmp.disp = ((yyvsp[-2].sval)&B11111111);
				tmp.code = B1101;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7494 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 419:
#line 4621 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movblg(yyengine, yyengine->cp, (yyvsp[-5].sval) - yyengine->cp->PC);
				}

				tmp.disp = ((yyvsp[-5].sval)&B11111111);
				tmp.code = B11000100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7531 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 420:
#line 4656 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movwlg(yyengine, yyengine->cp, (yyvsp[-5].sval) - yyengine->cp->PC);
				}

				tmp.disp = ((yyvsp[-5].sval)&B11111111);
				tmp.code = B11000101;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7568 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 421:
#line 4691 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movllg(yyengine, yyengine->cp, (yyvsp[-5].sval) - yyengine->cp->PC);
				}

				tmp.disp = ((yyvsp[-5].sval)&B11111111);
				tmp.code = B11000110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7605 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 422:
#line 4726 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movbsg(yyengine, yyengine->cp, (yyvsp[-3].sval) - yyengine->cp->PC);
				}

				tmp.disp = ((yyvsp[-3].sval)&B11111111);
				tmp.code = B11000000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7642 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 423:
#line 4761 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movwsg(yyengine, yyengine->cp, (yyvsp[-3].sval) - yyengine->cp->PC);
				}

				tmp.disp = ((yyvsp[-3].sval)&B11111111);
				tmp.code = B11000001;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7679 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 424:
#line 4796 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movlsg(yyengine, yyengine->cp, (yyvsp[-3].sval) - yyengine->cp->PC);
				}

				tmp.disp = ((yyvsp[-3].sval)&B11111111);
				tmp.code = B11000010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7716 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 425:
#line 4831 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_nd4 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movbs4(yyengine, yyengine->cp, (yyvsp[-3].sval) - yyengine->cp->PC, (yyvsp[-1].uval));
				}
	
				tmp.dst = ((yyvsp[-1].uval)&B1111);
				tmp.disp = ((yyvsp[-3].sval)&B1111);
				tmp.code = B10000000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7754 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 426:
#line 4867 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_nd4 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movws4(yyengine, yyengine->cp, (yyvsp[-3].sval) - yyengine->cp->PC, (yyvsp[-1].uval));
				}

				tmp.dst = ((yyvsp[-1].uval)&B1111);
				tmp.disp = ((yyvsp[-3].sval)&B1111);
				tmp.code = B10000001;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7792 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 427:
#line 4903 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_nmd tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movls4(yyengine, yyengine->cp, (yyvsp[-7].uval), (yyvsp[-3].sval) - yyengine->cp->PC, (yyvsp[-1].uval));
				}

				tmp.src = ((yyvsp[-7].uval)&B1111);
				tmp.dst = ((yyvsp[-1].uval)&B1111);
				tmp.disp = ((yyvsp[-3].sval)&B1111);
				tmp.code = B0001;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7831 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 428:
#line 4940 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_md tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movbl4(yyengine, yyengine->cp, (yyvsp[-5].sval) - yyengine->cp->PC, (yyvsp[-3].uval));
				}

				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.disp = ((yyvsp[-5].sval)&B1111);
				tmp.code = B10000100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7869 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 429:
#line 4976 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_md tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movwl4(yyengine, yyengine->cp, (yyvsp[-5].sval) - yyengine->cp->PC, (yyvsp[-3].uval));
				}

				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.disp = ((yyvsp[-5].sval)&B1111);
				tmp.code = B10000101;
	
				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7907 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 430:
#line 5012 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_nmd tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movll4(yyengine, yyengine->cp, (yyvsp[-3].uval), (yyvsp[-5].sval) - yyengine->cp->PC, (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-3].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.disp = ((yyvsp[-5].sval)&B1111);
				tmp.code = B0101;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7946 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 431:
#line 5049 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				/*						*/
				/*	The disp is absolute, and we have to	*/
				/*	pass PC relative to bra(). No need to	*/
				/*	PC-2 as in main.c, since no pipelining	*/
				/*						*/
				instr_d8 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_mova(yyengine, yyengine->cp, (yyvsp[-5].sval) - yyengine->cp->PC);
				}

				tmp.disp = ((yyvsp[-5].sval)&B11111111);
				tmp.code = B11000111;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 7983 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 432:
#line 5084 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_movt(yyengine, yyengine->cp, (yyvsp[0].uval));
				}
		
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00101001;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8016 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 433:
#line 5115 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_mull(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}
		
				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0111;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8050 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 434:
#line 5147 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_muls(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1111;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8084 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 435:
#line 5179 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_muls(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1111;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8118 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 436:
#line 5211 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_mulu(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1110;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8152 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 437:
#line 5243 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_mulu(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1110;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8186 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 438:
#line 5275 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_neg(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1011;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8220 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 439:
#line 5307 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_negc(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}
			
				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1010;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8254 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 440:
#line 5339 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_nop(yyengine, yyengine->cp);
				}

				tmp.code = B0000000000001001;
			
				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8285 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 441:
#line 5368 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_not(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B0111;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8319 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 442:
#line 5400 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_or(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1011;
				tmp.code_hi = B0010;
	
				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8353 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 443:
#line 5432 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_ori(yyengine, yyengine->cp, (yyvsp[-2].sval));
				}

				tmp.imm = ((yyvsp[-2].sval)&B11111111);
				tmp.code = B11001011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8385 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 444:
#line 5462 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_orm(yyengine, yyengine->cp, (yyvsp[-7].sval));
				}

				tmp.imm = ((yyvsp[-7].sval)&B11111111);
				tmp.code = B11001111;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8417 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 445:
#line 5492 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_pref(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B10000011;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8450 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 446:
#line 5523 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_rfg(yyengine, yyengine->cp, (yyvsp[0].sval));
				}

				tmp.imm = ((yyvsp[0].sval)&B11111111);
				tmp.code = B11110000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8482 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 447:
#line 5552 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_rotcl(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00100100;
				tmp.code_hi =  B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8515 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 448:
#line 5583 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_rotcr(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00100101;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8548 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 449:
#line 5614 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_rotl(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00000100;
				tmp.code_hi = B0100;
	
				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8581 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 450:
#line 5645 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_rotr(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00000101;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8614 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 451:
#line 5676 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_rte(yyengine, yyengine->cp);
				}

				tmp.code = B0000000000101011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8645 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 452:
#line 5705 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_rts(yyengine, yyengine->cp);
				}

				tmp.code = B0000000000001011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8676 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 453:
#line 5734 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_sets(yyengine, yyengine->cp);
				}

				tmp.code = B0000000001011000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8707 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 454:
#line 5763 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_sett(yyengine, yyengine->cp);
				}

				tmp.code = B0000000000011000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8738 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 455:
#line 5792 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shad(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}
	
				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1100; 
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8772 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 456:
#line 5824 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shal(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00100000;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8805 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 457:
#line 5855 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shar(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00100001;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;	
			}
		}
#line 8838 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 458:
#line 5886 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shld(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1101;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8872 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 459:
#line 5918 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shll(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00000000;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8905 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 460:
#line 5949 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shll2(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00001000;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8938 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 461:
#line 5980 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shll8(yyengine, yyengine->cp, (yyvsp[0].uval));
				}
		
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00011000;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 8971 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 462:
#line 6011 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shll16(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00101000;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9004 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 463:
#line 6042 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shlr(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00000001;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9037 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 464:
#line 6073 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shlr2(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00001001;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9070 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 465:
#line 6104 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shlr8(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00011001;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9103 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 466:
#line 6135 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_shlr16(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00101001;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9136 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 467:
#line 6166 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_0 tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_sleep(yyengine, yyengine->cp);
				}

				tmp.code = B0000000000011011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9167 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 468:
#line 6195 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcsr(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00000010;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9200 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 469:
#line 6226 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcgbr(yyengine, yyengine->cp, (yyvsp[0].uval));
				}
			
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00010010;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9233 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 470:
#line 6257 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcvbr(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00100010;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9266 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 471:
#line 6288 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcssr(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00110010;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9299 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 472:
#line 6319 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcspc(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B01000010;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9332 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 473:
#line 6350 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nbank tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcr_bank(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.code_lo = B0010;
				tmp.reg = ((yyvsp[-2].uval)&B111);
				tmp.fill = 1;
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9367 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 474:
#line 6383 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcmsr(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00000011;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9400 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 475:
#line 6414 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcmgbr(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00010011;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9433 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 476:
#line 6445 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcmvbr(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00100011;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9466 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 477:
#line 6476 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcmssr(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00110011;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9499 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 478:
#line 6507 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcmspc(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B01000011;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9532 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 479:
#line 6538 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nbank tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stcmr_bank(yyengine, yyengine->cp, (yyvsp[-4].uval), (yyvsp[0].uval));
				}

				tmp.code_lo = B0011;
				tmp.reg = ((yyvsp[-4].uval)&B111);
				tmp.fill = 1;
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9567 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 480:
#line 6571 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stsmach(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00001010;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9600 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 481:
#line 6602 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stsmacl(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00011010;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9633 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 482:
#line 6633 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stspr(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00101010;
				tmp.code_hi = B0000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9666 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 483:
#line 6664 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stsmmach(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00000010;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9699 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 484:
#line 6695 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stsmmacl(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00010010;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9732 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 485:
#line 6726 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_stsmpr(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00100010;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9765 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 486:
#line 6757 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_sub(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1000;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9799 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 487:
#line 6789 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_subc(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1010;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9833 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 488:
#line 6821 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_subv(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1011;
				tmp.code_hi = B0011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9867 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 489:
#line 6853 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_swapb(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1000;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9901 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 490:
#line 6885 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_swapw(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1001;
				tmp.code_hi = B0110;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9935 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 491:
#line 6917 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_n tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_tas(yyengine, yyengine->cp, (yyvsp[0].uval));
				}

				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B00011011;
				tmp.code_hi = B0100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 9968 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 492:
#line 6948 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_trapa(yyengine, yyengine->cp, (yyvsp[0].sval));
				}

				tmp.imm = ((yyvsp[0].sval)&B11111111);
				tmp.code = B11000011;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 10000 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 493:
#line 6978 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_tst(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1000;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 10034 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 494:
#line 7010 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_tsti(yyengine, yyengine->cp, (yyvsp[-2].sval));
				}

				tmp.imm = ((yyvsp[-2].sval)&B11111111);
				tmp.code = B11001000;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 10066 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 495:
#line 7040 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_tstm(yyengine, yyengine->cp, (yyvsp[-7].sval));
				}

				tmp.imm = ((yyvsp[-7].sval)&B11111111);
				tmp.code = B11001100;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 10098 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 496:
#line 7070 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_xor(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1010;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 10132 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 497:
#line 7102 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_xori(yyengine, yyengine->cp, (yyvsp[-2].sval));
				}

				tmp.imm = ((yyvsp[-2].sval)&B11111111);
				tmp.code = B11001010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 10164 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 498:
#line 7132 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_i tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_xorm(yyengine, yyengine->cp, (yyvsp[-7].sval));
				}

				tmp.imm = ((yyvsp[-7].sval)&B11111111);
				tmp.code = B11001110;
	
				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 10196 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 499:
#line 7162 "sf.y" /* yacc.c:1646  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 2;
			}
			else if (yyengine->cp->machinetype != MACHINE_SUPERH)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for Hitachi SH nodes only. Check node type.");
			}
			else
			{
				instr_nm tmp;

				if (!yyengine->cp->pipelined)
				{
					superH_xtrct(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval));
				}

				tmp.src = ((yyvsp[-2].uval)&B1111);
				tmp.dst = ((yyvsp[0].uval)&B1111);
				tmp.code_lo = B1101;
				tmp.code_hi = B0010;

				memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
					&tmp, sizeof(tmp));
				yyengine->cp->PC += 2;
			}
		}
#line 10230 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 502:
#line 7211 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				//$$->value = m_randgen(yyengine->cp, $3, $4, $5, $6, $7);
			}
		}
#line 10241 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 503:
#line 7220 "sf.y" /* yacc.c:1646  */
    {
			if (!yyengine->scanning)
			{
				//$$->value = m_randgen(yyengine->cp, $3, $4, $5, $6, $7);
				//$$->rv.pfunid = m_pfunname2id(yyengine->cp, $3);
				//$$->rv.disttabid = m_disttabname2id(yyengine->cp, $3);
				(yyval.rval)->rv.p1 = (yyvsp[-4].dval);
				(yyval.rval)->rv.p2 = (yyvsp[-3].dval);
				(yyval.rval)->rv.p3 = (yyvsp[-2].dval);
				(yyval.rval)->rv.p4 = (yyvsp[-1].dval);
			}
		}
#line 10258 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 504:
#line 7815 "sf.y" /* yacc.c:1646  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.uval) = strtoul((yyvsp[0].str), &ep, 0);
			
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid unsigned immediate data %s.", (yyvsp[0].str));
			}
		}
#line 10274 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 505:
#line 7829 "sf.y" /* yacc.c:1646  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.sval) = strtol((yyvsp[0].str), &ep, 0);
			
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid signed immediate data %s.", (yyvsp[0].str));
			}
		}
#line 10290 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 506:
#line 7841 "sf.y" /* yacc.c:1646  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.sval) = -(strtol((yyvsp[0].str), &ep, 0));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid signed immediate data %s.", (yyvsp[0].str));
			}
		}
#line 10306 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 507:
#line 7853 "sf.y" /* yacc.c:1646  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.sval) = +(strtol((yyvsp[0].str), &ep, 0));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid signed immediate data %s.", (yyvsp[0].str));
			}
		}
#line 10322 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 508:
#line 7867 "sf.y" /* yacc.c:1646  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.dval) = strtod((yyvsp[0].str), &ep);
			
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid double immediate data %s.", (yyvsp[0].str));
			}
		}
#line 10338 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 509:
#line 7879 "sf.y" /* yacc.c:1646  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.dval) = -(strtod((yyvsp[0].str), &ep));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid double immediate data %s.", (yyvsp[0].str));
			}
		}
#line 10354 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 510:
#line 7891 "sf.y" /* yacc.c:1646  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.dval) = +(strtod((yyvsp[0].str), &ep));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid double immediate data %s.", (yyvsp[0].str));
			}
		}
#line 10370 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 511:
#line 7905 "sf.y" /* yacc.c:1646  */
    {
			(yyval.rval) = (Rval *) mcalloc(yyengine, 1, sizeof(Rval), "sf.y:rdimm/$$");
			if ((yyval.rval) == NULL)
			{
				sfatal(yyengine, yyengine->cp, "Mcalloc failed");
			}

			(yyval.rval)->dval = (yyvsp[0].dval);
		}
#line 10384 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 513:
#line 7953 "sf.y" /* yacc.c:1646  */
    {
			(yyval.dlist) = (yyvsp[-1].dlist);
		}
#line 10392 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 514:
#line 7959 "sf.y" /* yacc.c:1646  */
    {
			DoubleListItem	*item;

			(yyval.dlist) = (DoubleList *) mcalloc(yyengine, 1, sizeof(DoubleList), "sf.y:dimmlistbody/$$");
			if ((yyval.dlist) == NULL)
			{
				sfatal(yyengine, yyengine->cp, "Mcalloc failed");
			}

			item = (DoubleListItem *)mcalloc(yyengine, 1, sizeof(DoubleListItem), "sf.y:dimmlistbody/item,1");
			if (item == NULL)
			{
				sfatal(yyengine, yyengine->cp, "Mcalloc failed");
			}
			item->value = (yyvsp[0].dval);
			item->next = NULL;

			(yyval.dlist)->hd = (yyval.dlist)->tl = item;
			(yyval.dlist)->len = 1;
		}
#line 10417 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 515:
#line 7980 "sf.y" /* yacc.c:1646  */
    {
			DoubleListItem	*item;

			item = (DoubleListItem *)mcalloc(yyengine, 1, sizeof(DoubleListItem), "sf.y:dimmlistbody/item,2");
			if (item == NULL)
			{
				sfatal(yyengine, yyengine->cp, "Mcalloc failed");
			}
			item->value = (yyvsp[0].dval);
			item->next = NULL;

			(yyval.dlist)->tl->next = item;
			(yyval.dlist)->tl = item;
		}
#line 10436 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 516:
#line 7998 "sf.y" /* yacc.c:1646  */
    {
			/*								*/
			/*	According to manual, PC of instr two instrs away is	*/
			/*	used to calculate target addr:				*/
			/*			BF					*/
			/*			NOP					*/
			/*			NOP <- this PC is used to calc targ	*/
			/*	We divide by two b'cos the disp is in terms of number	*/
			/*	of instructions, whereas the PC arithmetic is in terms	*/
			/*	of mem loacation, and instructions are placed in mem	*/
			/*	at every other byte.					*/
			/*								*/
			(yyval.sval) = ((yyvsp[0].uval) - (yyengine->cp->PC+4)) >> 1;

			/*	Warn if the PC calculated displacement was not a multiple of 2 */
			if (((yyvsp[0].uval) - (yyengine->cp->PC+4)) & 0x1)
			{
				merror(yyengine, "Disp in terms of mem addr was not on word boundary.");
			}
		}
#line 10461 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 517:
#line 8019 "sf.y" /* yacc.c:1646  */
    {
			char tmp;
			char *ep = &tmp;


			(yyval.sval) = strtol((yyvsp[0].str), &ep, 0);
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid DISP ([%s]). Possibly due to a .comm.", (yyvsp[0].str));
			}
		}
#line 10478 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 518:
#line 8032 "sf.y" /* yacc.c:1646  */
    {
			char tmp;
			char *ep = &tmp;


			(yyval.sval) = +(strtol((yyvsp[0].str), &ep, 0));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid DISP ([%s]). Possibly due to a .comm.", (yyvsp[0].str));
			}
		}
#line 10495 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 519:
#line 8045 "sf.y" /* yacc.c:1646  */
    {
			char tmp;
			char *ep = &tmp;


			(yyval.sval) = -(strtol((yyvsp[0].str), &ep, 0));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid DISP ([%s]). Possibly due to a .comm.", (yyvsp[0].str));
			}
		}
#line 10512 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 520:
#line 8060 "sf.y" /* yacc.c:1646  */
    {
			/*								*/
			/*	To make handling of empty string in T_RUN rule 		*/
			/*	uniform, we malloc a place holder.			*/
			/*								*/
			char *tmp = mmalloc(yyengine, sizeof(char), "opstring rule in shasm.y");
			tmp[0] = '\0';
			(yyval.str) = tmp;
		}
#line 10526 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 521:
#line 8070 "sf.y" /* yacc.c:1646  */
    {
			(yyval.str) = (yyvsp[0].str);
		}
#line 10534 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 522:
#line 8076 "sf.y" /* yacc.c:1646  */
    {(yyval.uval) = 0;}
#line 10540 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 523:
#line 8077 "sf.y" /* yacc.c:1646  */
    {(yyval.uval) = 1;}
#line 10546 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 524:
#line 8078 "sf.y" /* yacc.c:1646  */
    {(yyval.uval) = 2;}
#line 10552 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 525:
#line 8079 "sf.y" /* yacc.c:1646  */
    {(yyval.uval) = 3;}
#line 10558 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 526:
#line 8080 "sf.y" /* yacc.c:1646  */
    {(yyval.uval) = 4;}
#line 10564 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 527:
#line 8081 "sf.y" /* yacc.c:1646  */
    {(yyval.uval) = 5;}
#line 10570 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 528:
#line 8082 "sf.y" /* yacc.c:1646  */
    {(yyval.uval) = 6;}
#line 10576 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 529:
#line 8083 "sf.y" /* yacc.c:1646  */
    {(yyval.uval) = 7;}
#line 10582 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 530:
#line 8084 "sf.y" /* yacc.c:1646  */
    {(yyval.uval) = 8;}
#line 10588 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 531:
#line 8085 "sf.y" /* yacc.c:1646  */
    {(yyval.uval) = 9;}
#line 10594 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 532:
#line 8086 "sf.y" /* yacc.c:1646  */
    {(yyval.uval) = 10;}
#line 10600 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 533:
#line 8087 "sf.y" /* yacc.c:1646  */
    {(yyval.uval) = 11;}
#line 10606 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 534:
#line 8088 "sf.y" /* yacc.c:1646  */
    {(yyval.uval) = 12;}
#line 10612 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 535:
#line 8089 "sf.y" /* yacc.c:1646  */
    {(yyval.uval) = 13;}
#line 10618 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 536:
#line 8090 "sf.y" /* yacc.c:1646  */
    {(yyval.uval) = 14;}
#line 10624 "sf.tab.c" /* yacc.c:1646  */
    break;

  case 537:
#line 8091 "sf.y" /* yacc.c:1646  */
    {(yyval.uval) = 15;}
#line 10630 "sf.tab.c" /* yacc.c:1646  */
    break;


#line 10634 "sf.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
#line 8093 "sf.y" /* yacc.c:1906  */


#include "lex.c"

int
yyerror(char *err)
{
	merror(yyengine, "Invalid command!");
	clearistream(yyengine);
	
	return 0;
}
