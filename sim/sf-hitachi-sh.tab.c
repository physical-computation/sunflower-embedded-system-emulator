/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.3.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         sf_superh_parse
#define yylex           sf_superh_lex
#define yyerror         sf_superh_error
#define yydebug         sf_superh_debug
#define yynerrs         sf_superh_nerrs

#define yylval          sf_superh_lval
#define yychar          sf_superh_char

/* First part of user prologue.  */
#line 38 "sf-hitachi-sh.y" /* yacc.c:337  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <math.h>
#include "endian-hitachi-sh.h"
#include "sf.h"
#include "mextern.h"

/*
 *	Prevent generated parser from allocating memory on stack, useful
 *	for when running Sunflower on embedded targets.
 */
#define	YYSTACK_USE_ALLOCA	0

#line 96 "sf-hitachi-sh.tab.c" /* yacc.c:337  */
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
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
extern int sf_superh_debug;
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
    T_DUMPDISTRIBUTION = 284,
    T_DUMPPWR = 285,
    T_DUMPREGS = 286,
    T_DUMPSYSREGS = 287,
    T_DUMPTIME = 288,
    T_DUMPTLB = 289,
    T_DYNINSTR = 290,
    T_EBATTINTR = 291,
    T_EFAULTS = 292,
    T_ENABLEPIPELINE = 293,
    T_FILE2NETSEG = 294,
    T_FLTTHRESH = 295,
    T_FORCEAVGPWR = 296,
    T_HELP = 297,
    T_IGNORENODEDEATHS = 298,
    T_INITSEESTATE = 299,
    T_HWSEEREG = 300,
    T_REGISTERRVAR = 301,
    T_INITRANDTABLE = 302,
    T_DEFNDIST = 303,
    T_RANDPRINT = 304,
    T_LOAD = 305,
    T_LOCSTATS = 306,
    T_LISTRVARS = 307,
    T_MALLOCDEBUG = 308,
    T_MMAP = 309,
    T_MODECA = 310,
    T_MODEFF = 311,
    T_NETCORREL = 312,
    T_NETDEBUG = 313,
    T_NETNEWSEG = 314,
    T_NETNODENEWIFC = 315,
    T_NETSEG2FILE = 316,
    T_NETSEGDELETE = 317,
    T_NETSEGFAILDURMAX = 318,
    T_NETSEGFAILPROB = 319,
    T_NETSEGFAILPROBFN = 320,
    T_NETSEGNICATTACH = 321,
    T_NETSEGPROPMODEL = 322,
    T_NEWBATT = 323,
    T_NEWNODE = 324,
    T_NODEFAILDURMAX = 325,
    T_NODEFAILPROB = 326,
    T_NODEFAILPROBFN = 327,
    T_NODETACH = 328,
    T_NUMAREGION = 329,
    T_NUMASTATS = 330,
    T_NUMASTATSALL = 331,
    T_NUMASETMAPID = 332,
    T_ADDVALUETRACE = 333,
    T_DELVALUETRACE = 334,
    T_REGISTERSTABS = 335,
    T_VALUESTATS = 336,
    T_OFF = 337,
    T_ON = 338,
    T_PAUINFO = 339,
    T_NANOPAUSE = 340,
    T_PAUSE = 341,
    T_PFUN = 342,
    T_PIPEFLUSH = 343,
    T_POWERSTATS = 344,
    T_POWERTOTAL = 345,
    T_PWD = 346,
    T_PARSEOBJDUMP = 347,
    T_QUIT = 348,
    T_GETRANDOMSEED = 349,
    T_SETRANDOMSEED = 350,
    T_RATIO = 351,
    T_RENUMBERNODES = 352,
    T_RESETNODECTRS = 353,
    T_RESETALLCTRS = 354,
    T_RESETCPU = 355,
    T_RETRYALG = 356,
    T_RUN = 357,
    T_SAVE = 358,
    T_SENSORSDEBUG = 359,
    T_SETBASENODEID = 360,
    T_SETBATT = 361,
    T_SETBATTFEEDPERIOD = 362,
    T_SETDUMPPWRPERIOD = 363,
    T_SETNETPERIOD = 364,
    T_SETFAULTPERIOD = 365,
    T_SETFREQ = 366,
    T_SETIFCOUI = 367,
    T_SETMEMBASE = 368,
    T_SETNODE = 369,
    T_SETPC = 370,
    T_SETPHYSICSPERIOD = 371,
    T_SETQUANTUM = 372,
    T_SETSCALEALPHA = 373,
    T_SETSCALEK = 374,
    T_SETSCALEVT = 375,
    T_SETSCHEDRANDOM = 376,
    T_SETSCHEDROUNDROBIN = 377,
    T_SETTAG = 378,
    T_SETTIMERDELAY = 379,
    T_SETVDD = 380,
    T_SETMEMRLATENCY = 381,
    T_SETMEMWLATENCY = 382,
    T_SETFLASHRLATENCY = 383,
    T_SETFLASHWLATENCY = 384,
    T_SETNODEMASS = 385,
    T_SETPROPULSIONCOEFFS = 386,
    T_SHAREBUS = 387,
    T_SHOWCLK = 388,
    T_SHOWMEMBASE = 389,
    T_SHOWPIPE = 390,
    T_SHOWTAGS = 391,
    T_SIGNALSRC = 392,
    T_SIGNALSUBSCRIBE = 393,
    T_SIZEMEM = 394,
    T_SIZEPAU = 395,
    T_SPLIT = 396,
    T_SRECL = 397,
    T_LOADMAPFILE = 398,
    T_STEP = 399,
    T_STOP = 400,
    T_SFATAL = 401,
    T_TRACE = 402,
    T_THROTTLE = 403,
    T_THROTTLEWIN = 404,
    T_VERBOSE = 405,
    T_VERSION = 406,
    T_SETLOC = 407,
    T_BPT = 408,
    T_BPTLS = 409,
    T_BPTDEL = 410,
    T_GLOBALTIME = 411,
    T_CYCLES = 412,
    T_INSTRS = 413,
    T_SENSORREADING = 414,
    T_DOTALIGN = 415,
    T_DOTORG = 416,
    T_DOTDATA = 417,
    T_DOTTEXT = 418,
    T_DOTGLOBAL = 419,
    T_DOTFILE = 420,
    T_DOTLONG = 421,
    T_DOTCOMM = 422,
    T_R0 = 423,
    T_R1 = 424,
    T_R2 = 425,
    T_R3 = 426,
    T_R4 = 427,
    T_R5 = 428,
    T_R6 = 429,
    T_R7 = 430,
    T_R8 = 431,
    T_R9 = 432,
    T_R10 = 433,
    T_R11 = 434,
    T_R12 = 435,
    T_R13 = 436,
    T_R14 = 437,
    T_R15 = 438,
    T_GBR = 439,
    T_VBR = 440,
    T_MACH = 441,
    T_MACL = 442,
    T_PC = 443,
    T_PR = 444,
    T_TRA = 445,
    T_SPC = 446,
    T_SR = 447,
    T_SSR = 448,
    T_LABELDEFN = 449,
    T_ADD = 450,
    T_ADDC = 451,
    T_ADDV = 452,
    T_AND = 453,
    T_ANDB = 454,
    T_BF = 455,
    T_BFS = 456,
    T_BRA = 457,
    T_BRAF = 458,
    T_BSR = 459,
    T_BSRF = 460,
    T_BT = 461,
    T_BTS = 462,
    T_CLRMAC = 463,
    T_CLRS = 464,
    T_CLRT = 465,
    T_CMPEQ = 466,
    T_CMPGE = 467,
    T_CMPGT = 468,
    T_CMPHI = 469,
    T_CMPHS = 470,
    T_CMPPL = 471,
    T_CMPPZ = 472,
    T_CMPSTR = 473,
    T_DIV0S = 474,
    T_DIV0U = 475,
    T_DIV1 = 476,
    T_DMULSL = 477,
    T_DMULUL = 478,
    T_DT = 479,
    T_EXTSB = 480,
    T_EXTSW = 481,
    T_EXTUB = 482,
    T_EXTUW = 483,
    T_JMP = 484,
    T_JSR = 485,
    T_LDC = 486,
    T_LDCL = 487,
    T_LDS = 488,
    T_LDSL = 489,
    T_LDTLB = 490,
    T_MACW = 491,
    T_MOV = 492,
    T_MOVB = 493,
    T_MOVL = 494,
    T_MOVW = 495,
    T_MOVA = 496,
    T_MOVT = 497,
    T_MULL = 498,
    T_MULS = 499,
    T_MULSW = 500,
    T_MULU = 501,
    T_MULUW = 502,
    T_NEG = 503,
    T_NEGC = 504,
    T_NOP = 505,
    T_NOT = 506,
    T_OR = 507,
    T_ORB = 508,
    T_PREF = 509,
    T_RFG = 510,
    T_ROTCL = 511,
    T_ROTCR = 512,
    T_ROTL = 513,
    T_ROTR = 514,
    T_RTE = 515,
    T_RTS = 516,
    T_SETS = 517,
    T_SETT = 518,
    T_SHAD = 519,
    T_SHAL = 520,
    T_SHAR = 521,
    T_SHLD = 522,
    T_SHLL = 523,
    T_SHLL2 = 524,
    T_SHLL8 = 525,
    T_SHLL16 = 526,
    T_SHLR = 527,
    T_SHLR2 = 528,
    T_SHLR8 = 529,
    T_SHLR16 = 530,
    T_SLEEP = 531,
    T_STC = 532,
    T_STCL = 533,
    T_STS = 534,
    T_STSL = 535,
    T_SUB = 536,
    T_SUBC = 537,
    T_SUBV = 538,
    T_SWAPB = 539,
    T_SWAPW = 540,
    T_TASB = 541,
    T_TRAPA = 542,
    T_TST = 543,
    T_TSTB = 544,
    T_XOR = 545,
    T_XORB = 546,
    T_XTRCT = 547,
    T_LABEL = 548,
    T_STRING = 549,
    T_MAN = 550
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 72 "sf-hitachi-sh.y" /* yacc.c:352  */

	double		dval;
	ulong		uval;
	long		sval;
	char		*str;
	Rval		*rval;
	DoubleList	*dlist;

#line 441 "sf-hitachi-sh.tab.c" /* yacc.c:352  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE sf_superh_lval;

int sf_superh_parse (void);





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
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
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
#  define YYSIZE_T unsigned
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

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
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
#define YYLAST   2858

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  309
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  188
/* YYNRULES -- Number of rules.  */
#define YYNRULES  544
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1419

#define YYUNDEFTOK  2
#define YYMAXUTOK   551

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     296,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,   300,     2,     2,     2,     2,
     302,   303,     2,   304,   298,   305,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     306,     2,   308,     2,   301,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   297,     2,   299,     2,     2,     2,     2,
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
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   307
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   453,   453,   454,   457,   458,   459,   460,   461,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,   476,
     477,   478,   479,   480,   481,   482,   483,   484,   485,   486,
     487,   488,   489,   490,   491,   492,   493,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     507,   508,   509,   510,   511,   512,   513,   514,   515,   516,
     517,   518,   519,   520,   521,   522,   523,   524,   525,   526,
     527,   528,   529,   530,   531,   532,   533,   534,   535,   536,
     537,   538,   539,   540,   541,   542,   543,   544,   545,   546,
     547,   548,   549,   550,   551,   552,   553,   554,   555,   556,
     557,   558,   559,   560,   561,   562,   563,   564,   565,   566,
     567,   568,   569,   570,   571,   572,   573,   574,   575,   576,
     577,   578,   579,   580,   581,   582,   583,   584,   585,   586,
     587,   588,   589,   590,   591,   592,   593,   594,   595,   596,
     597,   598,   599,   600,   601,   602,   603,   604,   605,   606,
     607,   608,   609,   610,   611,   612,   613,   614,   615,   616,
     617,   618,   619,   620,   621,   622,   623,   624,   625,   626,
     627,   628,   629,   630,   631,   632,   633,   634,   635,   636,
     637,   641,   653,   663,   673,   701,   708,   715,   722,   732,
     742,   752,   762,   776,   794,   808,   826,   840,   858,   872,
     879,   886,   893,   900,   907,   914,   921,   928,   935,   942,
     950,   957,   965,   983,  1001,  1008,  1015,  1022,  1029,  1036,
    1043,  1050,  1057,  1064,  1072,  1085,  1092,  1099,  1111,  1118,
    1125,  1132,  1139,  1147,  1155,  1162,  1169,  1176,  1184,  1191,
    1205,  1213,  1220,  1227,  1241,  1257,  1264,  1271,  1278,  1285,
    1292,  1299,  1306,  1313,  1320,  1327,  1336,  1343,  1350,  1357,
    1364,  1371,  1378,  1385,  1392,  1399,  1406,  1422,  1430,  1439,
    1446,  1453,  1460,  1467,  1481,  1490,  1504,  1511,  1529,  1537,
    1544,  1551,  1558,  1565,  1572,  1579,  1586,  1593,  1600,  1607,
    1614,  1621,  1628,  1638,  1645,  1652,  1659,  1666,  1674,  1681,
    1688,  1695,  1702,  1710,  1718,  1729,  1733,  1737,  1744,  1751,
    1758,  1765,  1772,  1779,  1786,  1793,  1800,  1810,  1817,  1824,
    1832,  1839,  1846,  1853,  1862,  1869,  1876,  1883,  1890,  1897,
    1912,  1919,  1926,  1937,  1948,  1955,  1969,  1996,  2002,  2008,
    2014,  2019,  2024,  2049,  2072,  2113,  2153,  2193,  2233,  2273,
    2311,  2349,  2384,  2419,  2454,  2485,  2515,  2546,  2581,  2616,
    2645,  2674,  2703,  2735,  2767,  2799,  2831,  2863,  2893,  2924,
    2956,  2986,  3018,  3050,  3082,  3114,  3145,  3177,  3209,  3241,
    3273,  3314,  3345,  3376,  3407,  3438,  3469,  3500,  3533,  3564,
    3590,  3621,  3652,  3683,  3716,  3747,  3778,  3809,  3835,  3866,
    3897,  3926,  3958,  3990,  4022,  4054,  4086,  4118,  4150,  4182,
    4214,  4246,  4278,  4310,  4342,  4374,  4406,  4438,  4470,  4502,
    4534,  4567,  4599,  4630,  4668,  4703,  4739,  4774,  4809,  4844,
    4879,  4914,  4949,  4985,  5021,  5058,  5094,  5130,  5167,  5202,
    5233,  5265,  5297,  5329,  5361,  5393,  5425,  5457,  5486,  5518,
    5550,  5580,  5610,  5641,  5670,  5701,  5732,  5763,  5794,  5823,
    5852,  5881,  5910,  5942,  5973,  6004,  6036,  6067,  6098,  6129,
    6160,  6191,  6222,  6253,  6284,  6313,  6344,  6375,  6406,  6437,
    6468,  6501,  6532,  6563,  6594,  6625,  6656,  6689,  6720,  6751,
    6782,  6813,  6844,  6875,  6907,  6939,  6971,  7003,  7035,  7066,
    7096,  7128,  7158,  7188,  7220,  7250,  7280,  7312,  7312,  7329,
    7338,  7933,  7947,  7959,  7971,  7985,  7997,  8009,  8023,  8033,
    8071,  8077,  8098,  8116,  8137,  8150,  8163,  8179,  8188,  8195,
    8196,  8197,  8198,  8199,  8200,  8201,  8202,  8203,  8204,  8205,
    8206,  8207,  8208,  8209,  8210
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
  "T_DISABLEPIPELINE", "T_DUMPALL", "T_DUMPMEM", "T_DUMPPIPE",
  "T_DUMPDISTRIBUTION", "T_DUMPPWR", "T_DUMPREGS", "T_DUMPSYSREGS",
  "T_DUMPTIME", "T_DUMPTLB", "T_DYNINSTR", "T_EBATTINTR", "T_EFAULTS",
  "T_ENABLEPIPELINE", "T_FILE2NETSEG", "T_FLTTHRESH", "T_FORCEAVGPWR",
  "T_HELP", "T_IGNORENODEDEATHS", "T_INITSEESTATE", "T_HWSEEREG",
  "T_REGISTERRVAR", "T_INITRANDTABLE", "T_DEFNDIST", "T_RANDPRINT",
  "T_LOAD", "T_LOCSTATS", "T_LISTRVARS", "T_MALLOCDEBUG", "T_MMAP",
  "T_MODECA", "T_MODEFF", "T_NETCORREL", "T_NETDEBUG", "T_NETNEWSEG",
  "T_NETNODENEWIFC", "T_NETSEG2FILE", "T_NETSEGDELETE",
  "T_NETSEGFAILDURMAX", "T_NETSEGFAILPROB", "T_NETSEGFAILPROBFN",
  "T_NETSEGNICATTACH", "T_NETSEGPROPMODEL", "T_NEWBATT", "T_NEWNODE",
  "T_NODEFAILDURMAX", "T_NODEFAILPROB", "T_NODEFAILPROBFN", "T_NODETACH",
  "T_NUMAREGION", "T_NUMASTATS", "T_NUMASTATSALL", "T_NUMASETMAPID",
  "T_ADDVALUETRACE", "T_DELVALUETRACE", "T_REGISTERSTABS", "T_VALUESTATS",
  "T_OFF", "T_ON", "T_PAUINFO", "T_NANOPAUSE", "T_PAUSE", "T_PFUN",
  "T_PIPEFLUSH", "T_POWERSTATS", "T_POWERTOTAL", "T_PWD", "T_PARSEOBJDUMP",
  "T_QUIT", "T_GETRANDOMSEED", "T_SETRANDOMSEED", "T_RATIO",
  "T_RENUMBERNODES", "T_RESETNODECTRS", "T_RESETALLCTRS", "T_RESETCPU",
  "T_RETRYALG", "T_RUN", "T_SAVE", "T_SENSORSDEBUG", "T_SETBASENODEID",
  "T_SETBATT", "T_SETBATTFEEDPERIOD", "T_SETDUMPPWRPERIOD",
  "T_SETNETPERIOD", "T_SETFAULTPERIOD", "T_SETFREQ", "T_SETIFCOUI",
  "T_SETMEMBASE", "T_SETNODE", "T_SETPC", "T_SETPHYSICSPERIOD",
  "T_SETQUANTUM", "T_SETSCALEALPHA", "T_SETSCALEK", "T_SETSCALEVT",
  "T_SETSCHEDRANDOM", "T_SETSCHEDROUNDROBIN", "T_SETTAG",
  "T_SETTIMERDELAY", "T_SETVDD", "T_SETMEMRLATENCY", "T_SETMEMWLATENCY",
  "T_SETFLASHRLATENCY", "T_SETFLASHWLATENCY", "T_SETNODEMASS",
  "T_SETPROPULSIONCOEFFS", "T_SHAREBUS", "T_SHOWCLK", "T_SHOWMEMBASE",
  "T_SHOWPIPE", "T_SHOWTAGS", "T_SIGNALSRC", "T_SIGNALSUBSCRIBE",
  "T_SIZEMEM", "T_SIZEPAU", "T_SPLIT", "T_SRECL", "T_LOADMAPFILE",
  "T_STEP", "T_STOP", "T_SFATAL", "T_TRACE", "T_THROTTLE", "T_THROTTLEWIN",
  "T_VERBOSE", "T_VERSION", "T_SETLOC", "T_BPT", "T_BPTLS", "T_BPTDEL",
  "T_GLOBALTIME", "T_CYCLES", "T_INSTRS", "T_SENSORREADING", "T_DOTALIGN",
  "T_DOTORG", "T_DOTDATA", "T_DOTTEXT", "T_DOTGLOBAL", "T_DOTFILE",
  "T_DOTLONG", "T_DOTCOMM", "T_R0", "T_R1", "T_R2", "T_R3", "T_R4", "T_R5",
  "T_R6", "T_R7", "T_R8", "T_R9", "T_R10", "T_R11", "T_R12", "T_R13",
  "T_R14", "T_R15", "T_GBR", "T_VBR", "T_MACH", "T_MACL", "T_PC", "T_PR",
  "T_TRA", "T_SPC", "T_SR", "T_SSR", "T_LABELDEFN", "T_ADD", "T_ADDC",
  "T_ADDV", "T_AND", "T_ANDB", "T_BF", "T_BFS", "T_BRA", "T_BRAF", "T_BSR",
  "T_BSRF", "T_BT", "T_BTS", "T_CLRMAC", "T_CLRS", "T_CLRT", "T_CMPEQ",
  "T_CMPGE", "T_CMPGT", "T_CMPHI", "T_CMPHS", "T_CMPPL", "T_CMPPZ",
  "T_CMPSTR", "T_DIV0S", "T_DIV0U", "T_DIV1", "T_DMULSL", "T_DMULUL",
  "T_DT", "T_EXTSB", "T_EXTSW", "T_EXTUB", "T_EXTUW", "T_JMP", "T_JSR",
  "T_LDC", "T_LDCL", "T_LDS", "T_LDSL", "T_LDTLB", "T_MACW", "T_MOV",
  "T_MOVB", "T_MOVL", "T_MOVW", "T_MOVA", "T_MOVT", "T_MULL", "T_MULS",
  "T_MULSW", "T_MULU", "T_MULUW", "T_NEG", "T_NEGC", "T_NOP", "T_NOT",
  "T_OR", "T_ORB", "T_PREF", "T_RFG", "T_ROTCL", "T_ROTCR", "T_ROTL",
  "T_ROTR", "T_RTE", "T_RTS", "T_SETS", "T_SETT", "T_SHAD", "T_SHAL",
  "T_SHAR", "T_SHLD", "T_SHLL", "T_SHLL2", "T_SHLL8", "T_SHLL16", "T_SHLR",
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
  "mulsw_instr", "mulu_instr", "muluw_instr", "neg_instr", "negc_instr",
  "nop_instr", "not_instr", "or_instr", "ori_instr", "orm_instr",
  "pref_instr", "rfg_instr", "rotcl_instr", "rotcr_instr", "rotl_instr",
  "rotr_instr", "rte_instr", "rts_instr", "sets_instr", "sett_instr",
  "shad_instr", "shal_instr", "shar_instr", "shld_instr", "shll_instr",
  "shll2_instr", "shll8_instr", "shll16_instr", "shlr_instr",
  "shlr2_instr", "shlr8_instr", "shlr16_instr", "sleep_instr",
  "stcsr_instr", "stcgbr_instr", "stcvbr_instr", "stcssr_instr",
  "stcspc_instr", "stcr_bank_instr", "stcmsr_instr", "stcmgbr_instr",
  "stcmvbr_instr", "stcmssr_instr", "stcmspc_instr", "stcmr_bank_instr",
  "stsmach_instr", "stsmacl_instr", "stspr_instr", "stsmmach_instr",
  "stsmmacl_instr", "stsmpr_instr", "sub_instr", "subc_instr",
  "subv_instr", "swapb_instr", "swapw_instr", "tas_instr", "trapa_instr",
  "tst_instr", "tsti_instr", "tstb_instr", "xor_instr", "xori_instr",
  "xorm_instr", "xtrct_instr", "rnd", "rnd_const", "rnd_var", "uimm",
  "simm", "dimm", "rdimm", "dimmlist", "dimmlistbody", "disp", "optstring",
  "reg", YY_NULLPTR
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
     545,   546,   547,   548,   549,   550,    10,   123,    44,   125,
      35,    64,    40,    41,    43,    45,    60,   551,    62
};
# endif

#define YYPACT_NINF -780

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-780)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -780,  2562,  -780,  -207,  -207,  -207,  -283,  -283,  -207,  -283,
    -207,  -283,  -283,  -283,  -283,  -283,  -286,  -283,  -281,  -264,
    -265,  -283,  -233,  -283,  -213,  -195,  -283,  -191,  -182,  -181,
    -176,  -175,  -168,  -283,  -283,  -163,  -184,  -283,  -207,  -160,
    -283,  -156,  -155,  -152,  -134,  -133,  -132,  -125,  -124,  -116,
    -113,  -283,  -108,  -106,  -283,  -103,  -283,  -283,  -283,  -283,
    -283,  -283,  -123,  -283,  -283,  -283,  -105,  -283,  -207,  -100,
    -283,   -95,   -96,   -26,  -283,   -93,   -22,   -21,   -20,   -17,
     -16,   -14,  -283,  -283,     5,    20,    37,    38,    41,    40,
      43,  -283,    44,    46,    47,    48,    49,  -283,  -105,  -283,
    -780,  -283,  -283,  -207,  -207,  -207,  -207,  -207,  -283,  -283,
    -283,  -283,  -207,  -283,  -207,  -207,  -207,    50,    51,  -207,
    -207,  -283,  -283,  -283,  -283,  -207,  -207,  -283,    53,  -780,
      54,  -283,  -283,  -283,  -283,  -283,    57,    58,  -234,    59,
      62,  -283,  -283,  -283,    63,  -780,  -207,   -32,    64,  -283,
    -283,  -283,  -780,  -780,  -198,    67,  -198,  -198,   -27,  1767,
     389,  1018,  1018,   412,   -25,  -198,  -198,  -102,  -198,  1018,
    -198,  -198,  -780,  -780,  -780,   428,  1018,  1018,  1018,  1018,
    1018,  1018,  1018,  1018,  1018,  1018,  1018,  1018,  1018,  1018,
    1018,  1018,    61,    65,  1018,    66,  1018,    70,  -780,    71,
     444,   132,  -128,   149,    73,  1018,  1018,  1018,  1018,  1018,
    1018,  1018,  1018,  -780,  1018,   460,   -23,    74,    69,  1018,
    1018,  1018,  1018,  -780,  -780,  -780,  -780,  1018,  1018,  1018,
    1018,  1018,  1018,  1018,  1018,  1018,  1018,  1018,  1018,  -780,
     522,   627,  -153,   -19,  1018,  1018,  1018,  1018,  1018,    75,
      77,   478,    78,   494,    79,  1018,    72,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,    86,    87,    88,    89,    91,  -780,  -207,
      92,    93,    94,    95,    96,  -207,    97,  -207,    98,  -780,
    -283,  -780,  -780,    99,   100,  -780,   101,  -780,   104,  -283,
    -780,  -780,  -780,  -780,  -780,  -780,   103,   105,  -780,  -283,
     107,  -207,  -780,   109,  -207,  -283,  -283,   106,    56,  -207,
     117,  -780,  -780,  -780,  -283,  -780,  -780,  -283,  -780,  -283,
    -207,   120,   119,  -283,  -207,   123,  -283,  -283,  -207,  -780,
    -212,   124,   126,   127,   128,  -283,  -780,  -780,  -283,  -283,
    -283,   129,  -780,  -780,  -780,  -780,   130,   131,   134,  -780,
    -780,  -780,   135,  -780,  -780,   138,  -780,  -780,  -780,  -780,
    -780,   143,   142,  -283,   144,   145,  -780,  -780,  -780,  -780,
     147,   151,  -780,   152,   161,  -780,   165,   167,   168,   170,
    -780,  -780,  -780,   171,   174,   176,   178,   179,   183,  -207,
     184,  -780,  -780,  -105,  -283,   185,   186,  -283,   187,   188,
    -780,   189,  -780,   192,   193,   194,   197,  -780,  -207,  -283,
    -283,  -283,  -283,  -780,   202,  -780,  -780,  -780,  -780,   153,
     205,  -780,  -780,  -780,    39,  1018,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -186,    60,    85,   113,  -186,   141,  -186,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -186,   203,
     204,   211,   212,   213,  -780,  -780,   214,   215,   216,   217,
     222,  -780,   223,   225,   226,   227,  1018,  1018,   233,  1018,
     234,  1018,  1018,  -186,   236,   237,    68,   241,   242,    84,
     243,   244,   245,   115,   249,    80,  -780,   250,   251,   252,
     253,   254,   255,   256,   257,  -186,   275,  -186,  1018,  -186,
    -780,  -780,  -780,  -780,   276,  -780,  -780,   278,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,   279,   280,   346,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   390,   410,
     411,   413,   418,   419,   420,   421,   422,   423,   424,  1018,
    -186,  -186,   425,  -186,  -186,   426,  -186,   427,  -780,  -780,
    -780,  -780,  -780,  -780,   391,  -780,  -780,  -780,  -780,  -780,
     414,  -780,   430,  -780,  -283,  -780,  -780,  -780,   206,   431,
    -780,  -780,   433,  -780,  -780,  -780,  -207,  -283,   436,  -207,
     435,  -207,  -780,   437,  -207,  -283,  -207,   438,  -780,   440,
     441,  -780,   442,  -207,   443,  -780,   434,   439,  -780,  -780,
    -780,  -780,  -288,  -780,  -780,  -780,  -780,  -283,   446,  -283,
    -283,  -780,  -780,  -780,  -780,  -780,  -780,   449,  -780,   453,
    -780,  -780,  -780,   452,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -207,  -780,  -207,  -780,
    -780,  -780,  -283,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -207,   454,   455,   456,  -207,  -780,  -780,  -780,   461,   445,
    -780,   463,   465,   464,  1018,  1018,  1018,   466,  1018,   467,
     468,  1018,  1018,  1018,  1018,  1018,  1018,  1018,  1018,  1018,
    1018,  1018,  1018,  1018,  1018,  -780,  -780,   738,   450,    -8,
     469,   470,   471,  1018,   474,  -164,  -284,   475,   476,  -130,
    -279,  1018,   479,   480,  -109,  -267,   481,  -198,  1018,  1018,
    1018,  1018,  1018,  1018,  1018,  1018,   473,  1018,   485,  -780,
    -780,  1018,  1018,  1018,  1018,  1018,  1018,  1018,  1018,   483,
     484,   486,   487,   488,   489,  1018,  1018,  1018,   490,   491,
     492,  1018,  1018,  1018,  1018,  1018,  -780,  -780,   516,  1018,
     518,   519,  1018,   523,  1018,  -780,  -780,  -780,   472,   526,
    -780,  -780,  -207,  -283,  -207,  -207,  -207,   525,  -207,  -780,
     528,  -283,  -207,  -780,  -780,  -780,  -780,  -780,  -780,   531,
     532,  -288,  -186,  -780,  -283,  -283,  -780,   533,  -780,  -207,
    -207,   534,   536,  -780,  -780,  -780,   537,  -780,   538,  -780,
    -780,  1018,  -780,  -780,  -780,   180,  -780,   529,   195,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,   539,
    -780,  -780,  -780,   540,   541,  1018,  -780,   477,   542,   543,
    1018,   544,   -24,   545,   546,   548,  1018,   550,  -780,    36,
     547,   552,   553,  1018,   554,    52,   555,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,   196,  -780,   556,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,   462,   530,   551,   557,
     558,   560,  -780,  -780,  -780,   561,   562,   563,  -780,  -780,
    -780,  -780,  -780,   604,  -780,   559,   618,  -780,   568,  -780,
    -780,  -780,  -207,   565,  -207,  -207,  -780,  -242,   435,  -207,
    -780,  -283,  -207,  -207,  -207,  -205,  -186,  -283,  -283,  -780,
    -207,  -207,   576,  -780,  -780,   572,  -780,  -780,   573,  -780,
     819,    82,   575,  -780,  -198,  1018,   887,  -780,  1018,   666,
    1018,  -780,  -198,  1018,   845,  -780,  1018,  -107,  1018,  -780,
    -198,  1018,   866,  -780,  1018,   675,  1018,  -780,   177,  -780,
     578,  1018,  1018,  1018,  1018,  1018,  1018,  1018,  1018,  1018,
    -780,   579,  -780,   581,   564,  -780,  -207,  -207,  -207,  -780,
     586,  -207,  -283,  -283,  -207,  -207,  -283,  -780,  -186,  -283,
    -283,  -207,  -207,  -780,  1018,   687,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  1018,   580,   571,   574,   583,
    -780,   589,  -780,   591,   587,   590,   592,   593,  -780,   596,
     599,  -780,   600,   598,   621,   622,   623,  -780,   601,  -780,
     624,   691,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,   734,   764,  -207,  -207,  -207,  -780,  -780,  -207,  -207,
    -207,  -207,  -207,  -283,  -186,  -283,  -283,  -207,  -207,   629,
     636,   631,   931,   638,   639,   640,  1018,   756,   646,   648,
     649,   650,  1018,  1018,   948,   652,   653,   654,   655,  1018,
     656,   657,   658,   659,  -207,   664,  -207,   665,  -207,  -207,
    -207,  -207,   667,  -283,   668,   669,  -207,  -207,  -780,   776,
    -780,   670,   676,  1018,   800,   801,   677,   678,  1018,   803,
    1018,  1018,   679,   681,   682,   702,  1018,   804,  1018,   838,
     704,   840,   825,   846,   847,  -207,  -207,  -207,  -780,  -207,
    -207,   733,   743,  -780,  -283,  -780,  -780,  -207,  -207,   749,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,   750,   769,   770,  -207,  -207,   778,  -283,  -283,  -780,
    -780,  -283,  -207,  -207,  -780,  -780,  -780,  -780,   781,  -207,
    -780,  -207,  -283,   780,  -207,  -207,  -207,  -207,  -207,   782,
    -780,  -207,  -207,  -207,   783,  -207,  -780,  -207,  -207,  -207,
     784,   787,  -207,  -207,  -207,  -780,  -780,  -207,  -207,   790,
    -207,  -207,  -207,  -207,  -207,  -207,  -207,  -207,  -207,  -780,
    -207,  -207,  -207,  -780,  -207,  -207,  -105,  -283,  -207,  -207,
    -207,  -283,  -105,  -283,  -207,  -283,  -283,   789,  -780
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
       0,     0,     0,     0,     0,     0,   527,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   527,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   306,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   236,     0,     0,     0,     0,
       0,     0,   337,   338,     0,     0,     0,     0,     0,     4,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   359,   360,   361,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   447,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   458,   459,   460,   461,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   474,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   335,     3,     7,
       6,     8,     9,    10,    11,    14,    12,    13,    15,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    58,    59,
      60,    61,    62,    57,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     150,   151,   152,   153,   154,   149,   156,   157,   158,   159,
     160,   155,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   515,     0,     0,     0,     0,     0,   511,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   199,
       0,   248,   249,     0,     0,   334,     0,   286,     0,     0,
     252,   253,   250,   251,   265,   255,     0,     0,   287,     0,
       0,     0,   307,     0,     0,     0,     0,     0,     0,     0,
       0,   318,   319,   330,     0,   245,   246,     0,   322,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   528,
       0,     0,     0,     0,     0,     0,   262,   263,     0,     0,
       0,     0,   260,   321,   320,   331,     0,     0,     0,   309,
     317,   240,     0,   181,   267,     0,   266,   222,   223,   224,
     254,     0,     0,     0,     0,     0,   230,   235,   231,   232,
       0,     0,   305,     0,     0,   229,     0,     0,     0,     0,
     218,   219,   228,     0,     0,     0,     0,     0,     0,     0,
       0,   316,   310,   527,     0,     0,     0,     0,     0,     0,
     293,     0,   289,     0,     0,     0,     0,   291,     0,     0,
       0,     0,     0,   204,     0,   336,   341,   523,   524,     0,
       0,   339,   340,   342,     0,     0,     5,   529,   530,   531,
     532,   533,   534,   535,   536,   537,   538,   539,   540,   541,
     542,   543,   544,     0,     0,     0,     0,     0,     0,     0,
     351,   352,   353,   354,   355,   356,   357,   358,     0,     0,
       0,     0,     0,     0,   367,   368,     0,     0,     0,     0,
       0,   375,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   439,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     454,   455,   456,   457,     0,   463,   464,     0,   466,   467,
     468,   469,   470,   471,   472,   473,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   308,   517,
     516,   270,   189,   188,     0,   193,   190,   185,   191,   186,
       0,   195,     0,   197,     0,   227,   288,   314,     0,     0,
     269,   327,     0,   326,   233,   214,     0,     0,     0,     0,
       0,     0,   295,     0,     0,     0,     0,     0,   279,     0,
       0,   281,     0,     0,     0,   182,     0,     0,   519,   507,
     508,   518,     0,   272,   271,   280,   256,     0,     0,     0,
       0,   259,   332,   333,   328,   241,   268,     0,   292,     0,
     221,   198,   302,     0,   243,   296,   220,   215,   216,   217,
     297,   298,   299,   300,   301,   303,     0,   315,     0,   238,
     282,   283,     0,   312,   313,   294,   290,   323,   324,   325,
       0,     0,     0,     0,     0,   205,   525,   526,     0,     0,
     512,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   380,   381,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   452,
     453,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   498,   499,     0,     0,
       0,     0,     0,     0,     0,   192,   194,   196,     0,     0,
     285,   226,     0,     0,     0,     0,     0,     0,     0,   244,
       0,     0,     0,   225,   273,   275,   276,   234,   187,     0,
       0,     0,     0,   264,     0,     0,   207,     0,   329,     0,
       0,     0,     0,   200,   201,   202,     0,   343,     0,   514,
     513,     0,   344,   346,   347,     0,   348,     0,     0,   362,
     363,   364,   365,   366,   369,   371,   372,   373,   374,   376,
     377,   378,   379,   383,   384,   386,   382,   385,   387,     0,
     394,   395,   396,     0,     0,     0,   403,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   425,     0,
       0,     0,     0,     0,     0,     0,     0,   440,   441,   442,
     443,   444,   445,   446,   448,     0,   449,     0,   462,   465,
     476,   477,   479,   475,   478,   480,     0,     0,     0,     0,
       0,     0,   487,   488,   489,     0,     0,     0,   493,   494,
     495,   496,   497,     0,   500,     0,     0,   503,     0,   506,
     247,   242,     0,     0,     0,     0,   521,     0,     0,     0,
     277,     0,     0,     0,     0,     0,     0,     0,     0,   311,
       0,     0,     0,   206,   203,     0,   345,   349,     0,   370,
       0,     0,     0,   422,     0,     0,     0,   407,     0,     0,
       0,   404,     0,     0,     0,   409,     0,     0,     0,   406,
       0,     0,     0,   408,     0,     0,     0,   405,     0,   450,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     501,     0,   504,     0,     0,   212,     0,     0,     0,   520,
       0,     0,     0,     0,     0,     0,     0,   183,     0,     0,
       0,     0,     0,   284,     0,     0,   389,   390,   392,   388,
     391,   393,   397,   398,   399,     0,     0,     0,     0,     0,
     413,     0,   410,     0,     0,     0,     0,     0,   415,     0,
       0,   412,     0,     0,     0,     0,     0,   414,     0,   411,
       0,     0,   482,   483,   485,   481,   484,   486,   490,   491,
     492,     0,     0,     0,     0,     0,   522,   211,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   401,     0,
     402,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   208,     0,
       0,     0,     0,   184,     0,   257,   258,     0,     0,     0,
     429,   432,   419,   426,   435,   416,   431,   421,   428,   424,
     437,   418,   434,   430,   433,   420,   427,   423,   436,   417,
     438,     0,     0,     0,     0,     0,     0,     0,     0,   510,
     509,     0,     0,     0,   350,   451,   502,   505,     0,     0,
     210,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     261,     0,     0,     0,     0,     0,   278,     0,     0,     0,
       0,     0,     0,     0,     0,   209,   274,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   304,
       0,     0,     0,   213,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   239
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -780,  -780,  -780,  -780,   928,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,
    -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,  -780,   395,
    -589,    -3,  -779,     0,  -780,  -144,   -90,  1268
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,   392,   393,   394,   395,
     396,   397,   398,   399,   400,   401,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   422,   423,   424,   425,
     426,   427,   428,   429,   430,   431,   778,   779,   780,   439,
     843,   781,   782,   947,  1087,   591,   500,   614
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
     435,   436,   437,   961,  1018,   441,   432,   443,   532,   776,
     449,   438,   593,   594,  1020,   451,   433,   434,   777,  1026,
    1021,   620,   621,   622,   624,  1027,   626,   627,   847,   453,
     849,  1033,   452,   708,   709,   471,   710,  1034,  1024,   850,
     658,   598,   599,   600,   601,   602,   603,   604,   605,   606,
     607,   608,   609,   610,   611,   612,  1158,  1159,   660,  1031,
     438,  1199,   570,   455,   872,   502,   597,   598,   599,   600,
     601,   602,   603,   604,   605,   606,   607,   608,   609,   610,
     611,   612,   432,   457,   775,   776,   896,   432,   898,  1166,
     900,  1167,   433,   434,   777,   587,   588,   433,   434,   458,
     536,   537,   538,   539,   540,   460,   589,   590,   840,   545,
     469,   547,   548,   549,   461,   462,   552,   553,   841,   842,
     463,   464,   558,   559,   579,   580,   581,   582,   465,   587,
     588,   927,   928,   468,   930,   931,   472,   933,   474,   475,
     589,   590,   476,   578,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
     477,   478,   479,   587,   588,   587,   588,   711,   712,   480,
     713,   495,   481,   659,   589,   590,   589,   590,  1010,  1011,
     482,  1012,  1095,   483,   587,   588,   587,   588,   485,   499,
     486,   587,   588,   488,   503,   589,   590,   589,   590,   505,
     506,   509,   589,   590,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
     597,   598,   599,   600,   601,   602,   603,   604,   605,   606,
     607,   608,   609,   610,   611,   612,   597,   598,   599,   600,
     601,   602,   603,   604,   605,   606,   607,   608,   609,   610,
     611,   612,   597,   598,   599,   600,   601,   602,   603,   604,
     605,   606,   607,   608,   609,   610,   611,   612,  1182,  1183,
     507,  1184,   510,   511,   595,   619,   512,   677,  1119,   513,
     514,  1120,   515,   597,   598,   599,   600,   601,   602,   603,
     604,   605,   606,   607,   608,   609,   610,   611,   612,   518,
     655,   598,   599,   600,   601,   602,   603,   604,   605,   606,
     607,   608,   609,   610,   611,   612,   519,   662,   598,   599,
     600,   601,   602,   603,   604,   605,   606,   607,   608,   609,
     610,   611,   612,   520,   521,   522,   523,   838,  1127,   524,
     526,  1128,   527,   528,   529,   530,   550,   551,  1107,   561,
     562,   568,   569,   760,  1135,   572,   573,  1136,   844,   577,
     583,   592,   646,  1109,  1139,  1210,   647,   649,   728,   679,
     875,   651,   652,  1096,   665,   678,   719,   720,   723,   726,
     729,   730,   887,   845,   731,   732,   879,   733,   735,   736,
     737,   738,   739,   741,   743,   745,   746,   747,   748,   750,
     759,   751,   440,   753,   442,   755,   444,   445,   446,   447,
     448,   846,   450,   762,   767,   768,   454,   884,   456,   771,
     783,   459,   784,   785,   786,   791,   792,   793,   466,   467,
     794,   795,   470,   656,   796,   473,   734,   797,   798,   848,
     800,   801,   740,   802,   742,   803,   484,   836,   804,   487,
     663,   489,   490,   491,   492,   493,   494,   805,   496,   497,
     498,   806,   501,   807,   808,   504,   809,   810,   754,   508,
     811,   756,   812,   818,   813,   814,   761,   516,   517,   815,
     817,   820,   821,   823,   824,   825,   525,   766,   826,   827,
     828,   770,   531,   829,   533,   774,   534,   535,   835,   837,
     939,   851,   852,   541,   542,   543,   544,  1168,   546,   853,
     854,   855,   856,   857,   858,   859,   554,   555,   556,   557,
     860,   861,   560,   862,   863,   864,   563,   564,   565,   566,
     567,   867,   869,   571,   873,   874,   574,   575,   576,   877,
     878,   881,   882,   883,   584,   585,   586,   886,   888,   889,
     890,   891,   892,   893,   894,   895,   816,   597,   598,   599,
     600,   601,   602,   603,   604,   605,   606,   607,   608,   609,
     610,   611,   612,   897,   901,   830,   902,   903,   904,  1234,
     597,   598,   599,   600,   601,   602,   603,   604,   605,   606,
     607,   608,   609,   610,   611,   612,   597,   598,   599,   600,
     601,   602,   603,   604,   605,   606,   607,   608,   609,   610,
     611,   612,   597,   598,   599,   600,   601,   602,   603,   604,
     605,   606,   607,   608,   609,   610,   611,   612,   597,   598,
     599,   600,   601,   602,   603,   604,   605,   606,   607,   608,
     609,   610,   611,   612,   905,  1273,   597,   598,   599,   600,
     601,   602,   603,   604,   605,   606,   607,   608,   609,   610,
     611,   612,   597,   598,   599,   600,   601,   602,   603,   604,
     605,   606,   607,   608,   609,   610,   611,   612,   906,   907,
     908,   909,   910,   911,   912,   913,   914,   935,   915,   613,
     597,   598,   599,   600,   601,   602,   603,   604,   605,   606,
     607,   608,   609,   610,   611,   612,   696,   697,   916,   917,
     936,   918,   617,   698,   699,   700,   919,   920,   921,   922,
     923,   924,   925,   929,   932,   934,   937,   940,   628,   941,
     944,  1019,   946,   949,   953,  1025,   954,   955,   956,   958,
    1032,   959,   963,  1036,   653,   966,   960,   967,   968,   978,
     973,   974,   975,   942,  1009,   977,   945,   979,   948,   980,
     675,   950,   981,   952,   985,   987,   988,  1141,  1080,  1015,
     957,  1045,  1150,  1013,  1014,  1017,  1022,  1023,   721,  1114,
    1029,  1030,  1035,  1047,  1056,  1057,  1152,  1058,  1059,  1060,
    1061,  1065,  1066,  1067,   724,   597,   598,   599,   600,   601,
     602,   603,   604,   605,   606,   607,   608,   609,   610,   611,
     612,   702,   703,   969,  1073,   970,  1075,  1076,   704,   705,
     706,  1078,  1081,  1088,  1090,  1093,  1094,   972,  1102,  1099,
    1108,   976,  1103,  1104,  1191,  1142,  1105,  1110,  1111,  1112,
    1115,  1116,  1118,  1208,  1123,   744,  1124,  1122,  1126,  1130,
    1131,  1132,  1134,  1138,   749,  1240,  1143,  1140,  1223,  1261,
    1151,  1155,  1144,  1145,   752,  1146,  1147,  1148,  1149,  1153,
     757,   758,  1173,  1174,  1243,  1175,  1185,  1244,  1242,   763,
    1211,  1221,   764,  1222,   765,  1227,  1245,  1246,   769,  1247,
    1248,   772,   773,  1249,  1252,  1250,  1251,  1253,  1254,  1259,
     787,  1255,  1262,   788,   789,   790,   597,   598,   599,   600,
     601,   602,   603,   604,   605,   606,   607,   608,   609,   610,
     611,   612,  1003,  1004,  1256,  1257,  1258,  1260,   799,  1005,
    1006,  1007,  1263,  1278,  1279,  1280,  1283,  1284,  1285,  1082,
    1287,  1084,  1085,  1086,  1288,  1089,  1289,  1290,  1291,  1092,
    1296,  1297,  1298,  1299,  1301,  1302,  1303,  1304,  1306,   819,
    1319,  1308,   822,  1313,  1315,  1316,  1100,  1101,  1323,  1324,
    1186,  1328,  1336,  1320,   831,   832,   833,   834,  1193,  1321,
    1325,  1326,  1331,  1200,  1332,  1333,  1202,   597,   598,   599,
     600,   601,   602,   603,   604,   605,   606,   607,   608,   609,
     610,   611,   612,  1176,  1177,  1334,  1338,  1339,  1340,  1341,
    1178,  1179,  1180,   597,   598,   599,   600,   601,   602,   603,
     604,   605,   606,   607,   608,   609,   610,   611,   612,  1195,
    1342,  1343,  1349,  1196,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
    1204,  1350,  1354,  1355,  1205,   597,   598,   599,   600,   601,
     602,   603,   604,   605,   606,   607,   608,   609,   610,   611,
     612,  1188,  1356,  1357,  1360,  1366,  1370,  1380,  1376,  1154,
    1385,  1156,  1157,  1386,  1392,  1418,  1161,   596,  1160,  1163,
    1164,  1165,     0,     0,     0,     0,     0,  1171,  1172,   597,
     598,   599,   600,   601,   602,   603,   604,   605,   606,   607,
     608,   609,   610,   611,   612,  1281,   597,   598,   599,   600,
     601,   602,   603,   604,   605,   606,   607,   608,   609,   610,
     611,   612,  1294,     0,     0,     0,     0,     0,     0,   938,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   943,  1224,  1225,  1226,     0,     0,  1228,     0,
     951,  1231,  1232,     0,     0,     0,     0,     0,  1237,  1238,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   962,     0,   964,   965,   597,   598,   599,   600,
     601,   602,   603,   604,   605,   606,   607,   608,   609,   610,
     611,   612,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   971,     0,     0,
    1264,  1265,  1266,     0,     0,  1267,  1268,  1269,  1270,  1271,
       0,     0,     0,     0,  1276,  1277,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1305,     0,  1307,     0,  1309,  1310,  1311,  1312,     0,
       0,     0,     0,  1317,  1318,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1344,  1345,  1346,     0,  1347,  1348,     0,     0,
       0,     0,     0,     0,  1352,  1353,  1407,     0,     0,     0,
       0,     0,  1413,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1083,     0,
       0,  1358,  1359,     0,     0,     0,  1091,     0,     0,  1364,
    1365,     0,     0,     0,     0,     0,  1367,     0,  1368,  1097,
    1098,  1371,  1372,  1373,  1374,  1375,     0,     0,  1377,  1378,
    1379,     0,  1381,     0,  1382,  1383,  1384,     0,     0,  1387,
    1388,  1389,     0,     0,  1390,  1391,     0,  1393,  1394,  1395,
    1396,  1397,  1398,  1399,  1400,  1401,     0,  1402,  1403,  1404,
       0,  1405,  1406,     0,     0,  1409,  1410,  1411,     0,     0,
       0,  1415,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   615,
     616,   618,     0,     0,     0,   623,     0,   625,     0,     0,
       0,     0,     0,   629,   630,   631,   632,   633,   634,   635,
     636,   637,   638,   639,   640,   641,   642,   643,   644,   645,
       0,     0,   648,     0,   650,     0,     0,     0,   654,   657,
     661,   664,     0,   666,   667,   668,   669,   670,   671,   672,
     673,     0,   674,   676,     0,     0,  1162,   680,   681,   682,
     683,     0,  1169,  1170,     0,   684,   685,   686,   687,   688,
     689,   690,   691,   692,   693,   694,   695,     0,   701,   707,
       0,     0,   714,   715,   716,   717,   718,     0,     0,   722,
       0,   725,     0,   727,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1229,  1230,     0,
       0,  1233,     0,     0,  1235,  1236,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1272,     0,
    1274,  1275,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1314,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1351,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1361,  1362,     0,     0,  1363,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1369,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1408,     0,     0,     0,  1412,     0,  1414,     0,
    1416,  1417,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   839,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   865,   866,     0,   868,     0,   870,
     871,     0,     0,     0,   876,     0,     0,   880,     0,     0,
       0,   885,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   899,     0,     0,     0,
       0,     0,     0,     0,   158,     0,     0,     0,     0,     0,
       0,     0,   160,   161,   162,   163,   164,   165,   166,   167,
       0,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   926,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   982,   983,   984,     0,   986,     0,     0,   989,
     990,   991,   992,   993,   994,   995,   996,   997,   998,   999,
    1000,  1001,  1002,     0,     0,  1008,     0,     0,     0,     0,
       0,  1016,     0,     0,     0,     0,     0,     0,     0,  1028,
       0,     0,     0,     0,     0,     0,  1037,  1038,  1039,  1040,
    1041,  1042,  1043,  1044,     0,  1046,     0,     0,     0,  1048,
    1049,  1050,  1051,  1052,  1053,  1054,  1055,     0,     0,     0,
       0,     0,     0,  1062,  1063,  1064,     0,     0,     0,  1068,
    1069,  1070,  1071,  1072,     0,     0,     0,  1074,     0,     0,
    1077,     0,  1079,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1106,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1113,     0,     0,     0,     0,  1117,     0,
    1121,     0,     0,     0,  1125,     0,     0,  1129,     0,     0,
       0,  1133,     0,  1137,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1181,     0,
       0,     0,     0,  1187,  1189,     0,  1190,     0,  1192,     0,
       0,  1194,  1197,     0,  1198,     0,  1201,     0,     0,  1203,
    1206,     0,  1207,     0,  1209,     0,     0,     0,     0,  1212,
    1213,  1214,  1215,  1216,  1217,  1218,  1219,  1220,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1239,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1241,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1282,     0,     0,     0,  1286,     0,     0,     0,     0,     0,
    1292,  1293,  1295,     0,     0,     0,     0,  1300,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1322,     0,     0,     0,     0,  1327,     0,  1329,  1330,
       0,     0,     2,     0,  1335,     3,  1337,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,     0,    29,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,     0,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,     0,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,     0,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,     0,     0,
       0,     0,   150,   151,   152,   153,   154,   155,   156,   157,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   158,
       0,     0,     0,     0,     0,     0,   159,   160,   161,   162,
     163,   164,   165,   166,   167,     0,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,     0,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,     0,     0,   256,   257
};

static const yytype_int16 yycheck[] =
{
       3,     4,     5,   782,   168,     8,   294,    10,    98,   297,
     296,   294,   156,   157,   298,   296,   304,   305,   306,   298,
     304,   165,   166,   167,   168,   304,   170,   171,   617,   294,
     619,   298,   296,   186,   187,    38,   189,   304,   168,   628,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   298,   299,   202,   168,
     294,   168,   296,   296,   653,    68,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   294,   296,   296,   297,   675,   294,   677,   294,
     679,   296,   304,   305,   306,   293,   294,   304,   305,   294,
     103,   104,   105,   106,   107,   296,   304,   305,   294,   112,
     294,   114,   115,   116,   296,   296,   119,   120,   304,   305,
     296,   296,   125,   126,   156,   157,   158,   159,   296,   293,
     294,   720,   721,   296,   723,   724,   296,   726,   294,   294,
     304,   305,   294,   146,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     294,   294,   294,   293,   294,   293,   294,   186,   187,   294,
     189,   294,   296,   301,   304,   305,   304,   305,   186,   187,
     296,   189,   961,   296,   293,   294,   293,   294,   296,   294,
     296,   293,   294,   296,   294,   304,   305,   304,   305,   294,
     296,   294,   304,   305,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   186,   187,
     296,   189,   294,   294,   301,   300,   296,   300,   302,   296,
     296,   305,   296,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   294,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   296,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   296,   296,   294,   296,   298,   302,   296,
     296,   305,   296,   296,   296,   296,   296,   296,   168,   296,
     296,   294,   294,   297,   302,   296,   294,   305,   298,   296,
     296,   294,   301,   168,   168,   188,   301,   301,   296,   300,
     302,   301,   301,   962,   301,   301,   301,   300,   300,   300,
     294,   294,   302,   298,   296,   296,   302,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   294,   296,
     294,   296,     7,   296,     9,   296,    11,    12,    13,    14,
      15,   298,    17,   296,   294,   296,    21,   302,    23,   296,
     296,    26,   296,   296,   296,   296,   296,   296,    33,    34,
     296,   296,    37,   301,   296,    40,   439,   294,   296,   298,
     296,   296,   445,   296,   447,   294,    51,   294,   296,    54,
     301,    56,    57,    58,    59,    60,    61,   296,    63,    64,
      65,   296,    67,   296,   296,    70,   296,   296,   471,    74,
     296,   474,   296,   563,   296,   296,   479,    82,    83,   296,
     296,   296,   296,   296,   296,   296,    91,   490,   296,   296,
     296,   494,    97,   296,    99,   498,   101,   102,   296,   294,
     294,   298,   298,   108,   109,   110,   111,  1096,   113,   298,
     298,   298,   298,   298,   298,   298,   121,   122,   123,   124,
     298,   298,   127,   298,   298,   298,   131,   132,   133,   134,
     135,   298,   298,   138,   298,   298,   141,   142,   143,   298,
     298,   298,   298,   298,   149,   150,   151,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   559,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   298,   298,   578,   298,   298,   298,  1168,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   298,  1234,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   296,   298,   300,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   298,   298,
     296,   298,   300,   191,   192,   193,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   296,   296,   300,   296,
     294,   875,   297,   296,   296,   879,   296,   296,   296,   296,
     884,   307,   296,   887,   300,   296,   307,   294,   296,   304,
     296,   296,   296,   756,   304,   294,   759,   294,   761,   294,
     300,   764,   298,   766,   298,   298,   298,   305,   296,   298,
     773,   298,   168,   304,   304,   301,   301,   301,   300,   302,
     301,   301,   301,   298,   301,   301,   168,   301,   301,   301,
     301,   301,   301,   301,   300,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   816,   298,   818,   298,   298,   191,   192,
     193,   298,   296,   298,   296,   294,   294,   830,   294,   296,
     301,   834,   296,   296,   168,   305,   298,   298,   298,   298,
     298,   298,   298,   168,   298,   450,   298,   302,   298,   302,
     298,   298,   298,   298,   459,   168,   305,   301,   294,   168,
     301,   296,   305,   305,   469,   305,   305,   305,   305,   301,
     475,   476,   296,   301,   303,   302,   301,   303,   298,   484,
     302,   302,   487,   302,   489,   299,   303,   298,   493,   298,
     303,   496,   497,   303,   298,   303,   303,   298,   298,   298,
     505,   303,   168,   508,   509,   510,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   303,   303,   303,   303,   533,   191,
     192,   193,   168,   304,   298,   304,   298,   298,   298,   942,
     184,   944,   945,   946,   298,   948,   298,   298,   298,   952,
     298,   298,   298,   298,   298,   298,   298,   298,   294,   564,
     184,   296,   567,   296,   296,   296,   969,   970,   168,   168,
    1114,   168,   168,   303,   579,   580,   581,   582,  1122,   303,
     303,   303,   303,  1127,   303,   303,  1130,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   303,   168,   303,   168,   184,
     191,   192,   193,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     184,   184,   299,   188,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   308,   303,   303,   188,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   303,   303,   296,   294,   296,   294,   296,  1082,
     296,  1084,  1085,   296,   294,   296,  1089,   159,  1088,  1092,
    1093,  1094,    -1,    -1,    -1,    -1,    -1,  1100,  1101,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,    -1,    -1,    -1,    -1,    -1,    -1,   744,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   757,  1156,  1157,  1158,    -1,    -1,  1161,    -1,
     765,  1164,  1165,    -1,    -1,    -1,    -1,    -1,  1171,  1172,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   787,    -1,   789,   790,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   822,    -1,    -1,
    1223,  1224,  1225,    -1,    -1,  1228,  1229,  1230,  1231,  1232,
      -1,    -1,    -1,    -1,  1237,  1238,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1264,    -1,  1266,    -1,  1268,  1269,  1270,  1271,    -1,
      -1,    -1,    -1,  1276,  1277,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1305,  1306,  1307,    -1,  1309,  1310,    -1,    -1,
      -1,    -1,    -1,    -1,  1317,  1318,  1406,    -1,    -1,    -1,
      -1,    -1,  1412,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   943,    -1,
      -1,  1344,  1345,    -1,    -1,    -1,   951,    -1,    -1,  1352,
    1353,    -1,    -1,    -1,    -1,    -1,  1359,    -1,  1361,   964,
     965,  1364,  1365,  1366,  1367,  1368,    -1,    -1,  1371,  1372,
    1373,    -1,  1375,    -1,  1377,  1378,  1379,    -1,    -1,  1382,
    1383,  1384,    -1,    -1,  1387,  1388,    -1,  1390,  1391,  1392,
    1393,  1394,  1395,  1396,  1397,  1398,    -1,  1400,  1401,  1402,
      -1,  1404,  1405,    -1,    -1,  1408,  1409,  1410,    -1,    -1,
      -1,  1414,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,    -1,
      -1,    -1,    -1,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,   194,    -1,   196,    -1,    -1,    -1,   200,   201,
     202,   203,    -1,   205,   206,   207,   208,   209,   210,   211,
     212,    -1,   214,   215,    -1,    -1,  1091,   219,   220,   221,
     222,    -1,  1097,  1098,    -1,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,    -1,   240,   241,
      -1,    -1,   244,   245,   246,   247,   248,    -1,    -1,   251,
      -1,   253,    -1,   255,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1162,  1163,    -1,
      -1,  1166,    -1,    -1,  1169,  1170,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1233,    -1,
    1235,  1236,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1273,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1314,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1347,  1348,    -1,    -1,  1351,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1362,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1407,    -1,    -1,    -1,  1411,    -1,  1413,    -1,
    1415,  1416,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   595,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   646,   647,    -1,   649,    -1,   651,
     652,    -1,    -1,    -1,   656,    -1,    -1,   659,    -1,    -1,
      -1,   663,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   678,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   187,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   195,   196,   197,   198,   199,   200,   201,   202,
      -1,   204,   205,   206,   207,   208,   209,   210,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   719,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   292,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   844,   845,   846,    -1,   848,    -1,    -1,   851,
     852,   853,   854,   855,   856,   857,   858,   859,   860,   861,
     862,   863,   864,    -1,    -1,   867,    -1,    -1,    -1,    -1,
      -1,   873,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   881,
      -1,    -1,    -1,    -1,    -1,    -1,   888,   889,   890,   891,
     892,   893,   894,   895,    -1,   897,    -1,    -1,    -1,   901,
     902,   903,   904,   905,   906,   907,   908,    -1,    -1,    -1,
      -1,    -1,    -1,   915,   916,   917,    -1,    -1,    -1,   921,
     922,   923,   924,   925,    -1,    -1,    -1,   929,    -1,    -1,
     932,    -1,   934,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   981,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1015,    -1,    -1,    -1,    -1,  1020,    -1,
    1022,    -1,    -1,    -1,  1026,    -1,    -1,  1029,    -1,    -1,
      -1,  1033,    -1,  1035,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1110,    -1,
      -1,    -1,    -1,  1115,  1116,    -1,  1118,    -1,  1120,    -1,
      -1,  1123,  1124,    -1,  1126,    -1,  1128,    -1,    -1,  1131,
    1132,    -1,  1134,    -1,  1136,    -1,    -1,    -1,    -1,  1141,
    1142,  1143,  1144,  1145,  1146,  1147,  1148,  1149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1242,    -1,    -1,    -1,  1246,    -1,    -1,    -1,    -1,    -1,
    1252,  1253,  1254,    -1,    -1,    -1,    -1,  1259,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1283,    -1,    -1,    -1,    -1,  1288,    -1,  1290,  1291,
      -1,    -1,     0,    -1,  1296,     3,  1298,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    -1,    31,    32,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,    -1,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,    -1,    -1,
      -1,    -1,   160,   161,   162,   163,   164,   165,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   187,
      -1,    -1,    -1,    -1,    -1,    -1,   194,   195,   196,   197,
     198,   199,   200,   201,   202,    -1,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,    -1,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   292,    -1,    -1,   295,   296
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   310,     0,     3,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    31,
      32,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     160,   161,   162,   163,   164,   165,   166,   167,   187,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   295,   296,   311,   312,
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
     473,   474,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,   294,   304,   305,   490,   490,   490,   294,   488,
     488,   490,   488,   490,   488,   488,   488,   488,   488,   296,
     488,   296,   296,   294,   488,   296,   488,   296,   294,   488,
     296,   296,   296,   296,   296,   296,   488,   488,   296,   294,
     488,   490,   296,   488,   294,   294,   294,   294,   294,   294,
     294,   296,   296,   296,   488,   296,   296,   488,   296,   488,
     488,   488,   488,   488,   488,   294,   488,   488,   488,   294,
     495,   488,   490,   294,   488,   294,   296,   296,   488,   294,
     294,   294,   296,   296,   296,   296,   488,   488,   294,   296,
     296,   296,   294,   296,   296,   488,   296,   296,   296,   296,
     296,   488,   495,   488,   488,   488,   490,   490,   490,   490,
     490,   488,   488,   488,   488,   490,   488,   490,   490,   490,
     296,   296,   490,   490,   488,   488,   488,   488,   490,   490,
     488,   296,   296,   488,   488,   488,   488,   488,   294,   294,
     296,   488,   296,   294,   488,   488,   488,   296,   490,   156,
     157,   158,   159,   296,   488,   488,   488,   293,   294,   304,
     305,   494,   294,   494,   494,   301,   313,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   300,   496,   496,   496,   300,   496,   300,
     494,   494,   494,   496,   494,   496,   494,   494,   300,   496,
     496,   496,   496,   496,   496,   496,   496,   496,   496,   496,
     496,   496,   496,   496,   496,   496,   301,   301,   496,   301,
     496,   301,   301,   300,   496,   168,   301,   496,   168,   301,
     494,   496,   168,   301,   496,   301,   496,   496,   496,   496,
     496,   496,   496,   496,   496,   300,   496,   300,   301,   300,
     496,   496,   496,   496,   496,   496,   496,   496,   496,   496,
     496,   496,   496,   496,   496,   496,   184,   185,   191,   192,
     193,   496,   184,   185,   191,   192,   193,   496,   186,   187,
     189,   186,   187,   189,   496,   496,   496,   496,   496,   301,
     300,   300,   496,   300,   300,   496,   300,   496,   296,   294,
     294,   296,   296,   296,   490,   296,   296,   296,   296,   296,
     490,   296,   490,   296,   488,   296,   296,   296,   294,   488,
     296,   296,   488,   296,   490,   296,   490,   488,   488,   294,
     297,   490,   296,   488,   488,   488,   490,   294,   296,   488,
     490,   296,   488,   488,   490,   296,   297,   306,   485,   486,
     487,   490,   491,   296,   296,   296,   296,   488,   488,   488,
     488,   296,   296,   296,   296,   296,   296,   294,   296,   488,
     296,   296,   296,   294,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   296,   296,   490,   296,   495,   488,
     296,   296,   488,   296,   296,   296,   296,   296,   296,   296,
     490,   488,   488,   488,   488,   296,   294,   294,   298,   496,
     294,   304,   305,   489,   298,   298,   298,   489,   298,   489,
     489,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   496,   496,   298,   496,   298,
     496,   496,   489,   298,   298,   302,   496,   298,   298,   302,
     496,   298,   298,   298,   302,   496,   298,   302,   298,   298,
     298,   298,   298,   298,   298,   298,   489,   298,   489,   496,
     489,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   496,   489,   489,   298,
     489,   489,   298,   489,   298,   296,   296,   296,   488,   294,
     296,   296,   490,   488,   294,   490,   297,   492,   490,   296,
     490,   488,   490,   296,   296,   296,   296,   490,   296,   307,
     307,   491,   488,   296,   488,   488,   296,   294,   296,   490,
     490,   488,   490,   296,   296,   296,   490,   294,   304,   294,
     294,   298,   496,   496,   496,   298,   496,   298,   298,   496,
     496,   496,   496,   496,   496,   496,   496,   496,   496,   496,
     496,   496,   496,   184,   185,   191,   192,   193,   496,   304,
     186,   187,   189,   304,   304,   298,   496,   301,   168,   494,
     298,   304,   301,   301,   168,   494,   298,   304,   496,   301,
     301,   168,   494,   298,   304,   301,   494,   496,   496,   496,
     496,   496,   496,   496,   496,   298,   496,   298,   496,   496,
     496,   496,   496,   496,   496,   496,   301,   301,   301,   301,
     301,   301,   496,   496,   496,   301,   301,   301,   496,   496,
     496,   496,   496,   298,   496,   298,   298,   496,   298,   496,
     296,   296,   490,   488,   490,   490,   490,   493,   298,   490,
     296,   488,   490,   294,   294,   491,   489,   488,   488,   296,
     490,   490,   294,   296,   296,   298,   496,   168,   301,   168,
     298,   298,   298,   496,   302,   298,   298,   496,   298,   302,
     305,   496,   302,   298,   298,   496,   298,   302,   305,   496,
     302,   298,   298,   496,   298,   302,   305,   496,   298,   168,
     301,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     168,   301,   168,   301,   490,   296,   490,   490,   298,   299,
     492,   490,   488,   490,   490,   490,   294,   296,   489,   488,
     488,   490,   490,   296,   301,   302,   184,   185,   191,   192,
     193,   496,   186,   187,   189,   301,   494,   496,   184,   496,
     496,   168,   496,   494,   496,   184,   188,   496,   496,   168,
     494,   496,   494,   496,   184,   188,   496,   496,   168,   496,
     188,   302,   496,   496,   496,   496,   496,   496,   496,   496,
     496,   302,   302,   294,   490,   490,   490,   299,   490,   488,
     488,   490,   490,   488,   489,   488,   488,   490,   490,   496,
     168,   496,   298,   303,   303,   303,   298,   298,   303,   303,
     303,   303,   298,   298,   298,   303,   303,   303,   303,   298,
     303,   168,   168,   168,   490,   490,   490,   490,   490,   490,
     490,   490,   488,   489,   488,   488,   490,   490,   304,   298,
     304,   184,   496,   298,   298,   298,   496,   184,   298,   298,
     298,   298,   496,   496,   184,   496,   298,   298,   298,   298,
     496,   298,   298,   298,   298,   490,   294,   490,   296,   490,
     490,   490,   490,   296,   488,   296,   296,   490,   490,   184,
     303,   303,   496,   168,   168,   303,   303,   496,   168,   496,
     496,   303,   303,   303,   303,   496,   168,   496,   168,   303,
     168,   184,   184,   184,   490,   490,   490,   490,   490,   299,
     308,   488,   490,   490,   303,   303,   303,   303,   490,   490,
     296,   488,   488,   488,   490,   490,   294,   490,   490,   488,
     296,   490,   490,   490,   490,   490,   296,   490,   490,   490,
     294,   490,   490,   490,   490,   296,   296,   490,   490,   490,
     490,   490,   294,   490,   490,   490,   490,   490,   490,   490,
     490,   490,   490,   490,   490,   490,   490,   495,   488,   490,
     490,   490,   488,   495,   488,   490,   488,   488,   296
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   309,   310,   310,   311,   311,   311,   311,   311,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   336,   337,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,   392,   393,   394,   395,   396,   397,   398,
     399,   400,   401,   402,   403,   403,   404,   405,   406,   407,
     408,   409,   410,   411,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   422,   423,   424,   425,   426,   427,
     428,   429,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,   485,   485,   486,
     487,   488,   489,   489,   489,   490,   490,   490,   491,   491,
     492,   493,   493,   494,   494,   494,   494,   495,   495,   496,
     496,   496,   496,   496,   496,   496,   496,   496,   496,   496,
     496,   496,   496,   496,   496
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
       1,     2,     3,     6,     9,     3,     3,     4,     3,     3,
       3,     3,     4,     3,     4,     3,     4,     3,     3,     2,
       4,     4,     4,     5,     2,     3,     5,     4,     9,    15,
      11,     7,     6,    21,     3,     3,     3,     3,     2,     2,
       3,     3,     2,     2,     2,     4,     4,     3,     2,     2,
       2,     2,     2,     3,     4,     2,     1,     1,     3,    35,
       2,     3,     5,     3,     4,     2,     2,     5,     2,     2,
       2,     2,     2,     2,     2,     2,     3,     9,     9,     3,
       2,    12,     2,     2,     4,     2,     2,     2,     3,     3,
       3,     3,     3,     4,    15,     4,     4,     5,    13,     3,
       3,     3,     3,     3,     6,     4,     2,     2,     3,     2,
       3,     2,     3,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,    19,     2,     1,     2,     2,     2,
       2,     5,     3,     3,     3,     3,     2,     2,     2,     2,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     4,
       2,     2,     3,     3,     2,     1,     2,     1,     1,     2,
       2,     2,     2,     4,     4,     5,     4,     4,     4,     5,
      10,     2,     2,     2,     2,     2,     2,     2,     2,     1,
       1,     1,     4,     4,     4,     4,     4,     2,     2,     4,
       5,     4,     4,     4,     4,     2,     4,     4,     4,     4,
       3,     3,     4,     4,     4,     4,     4,     4,     6,     6,
       6,     6,     6,     6,     4,     4,     4,     6,     6,     6,
       1,     8,     8,     4,     5,     5,     5,     5,     5,     5,
       6,     6,     6,     6,     6,     6,     9,     9,     9,     9,
       9,     9,     5,     9,     9,     4,     9,     9,     9,     9,
       9,     9,     9,     9,     9,     9,     9,     9,     9,     2,
       4,     4,     4,     4,     4,     4,     4,     1,     4,     4,
       5,    10,     3,     3,     2,     2,     2,     2,     1,     1,
       1,     1,     4,     2,     2,     4,     2,     2,     2,     2,
       2,     2,     2,     2,     1,     4,     4,     4,     4,     4,
       4,     6,     6,     6,     6,     6,     6,     4,     4,     4,
       6,     6,     6,     4,     4,     4,     4,     4,     3,     3,
       4,     5,    10,     4,     5,    10,     4,     1,     1,     8,
       8,     1,     1,     2,     2,     1,     2,     2,     1,     1,
       3,     1,     3,     1,     1,     2,     2,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
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
  unsigned long yylno = yyrline[yyrule];
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
                       &yyvsp[(yyi + 1) - (yynrhs)]
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
            else
              goto append;

          append:
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

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
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
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
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
    default: /* Avoid compiler warnings. */
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
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
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
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
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
# else /* defined YYSTACK_RELOCATE */
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
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
| yyreduce -- do a reduction.  |
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
        case 181:
#line 642 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				mexit(yyengine, "Exiting as per user's request.", 0);
			}
		}
#line 2875 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 182:
#line 654 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			/*
			 *	Only give the architecture type:
			 */
			if (!yyengine->scanning)
			{
				m_newnode(yyengine, (yyvsp[-1].str), 0, 0, 0, NULL, 0, 0);
			}
		}
#line 2889 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 183:
#line 664 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			/*
			 *	Give the architecture type and the (fixed) x/y/z location
			 */
			if (!yyengine->scanning)
			{
				m_newnode(yyengine, (yyvsp[-4].str), (yyvsp[-3].rval)->dval, (yyvsp[-3].rval)->dval, (yyvsp[-3].rval)->dval, NULL, 0, 0);
			}
		}
#line 2903 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 184:
#line 674 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 2935 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 185:
#line 702 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				batt_nodeattach(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 2946 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 186:
#line 709 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				batt_printstats(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 2957 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 187:
#line 716 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				batt_newbatt(yyengine, (yyvsp[-2].uval), (yyvsp[-1].dval));
			}
		}
#line 2968 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 188:
#line 723 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				if (SF_DEBUG) mprint(yyengine, NULL, siminfo, 
						"Setting yyengine->batts[%d].ileak to %f\n",
						yyengine->curbatt, (yyvsp[-1].dval));
				yyengine->batts[yyengine->curbatt].ileak = (yyvsp[-1].dval);
			}
		}
#line 2982 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 189:
#line 733 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				if (SF_DEBUG) mprint(yyengine, NULL, siminfo, 
						"Setting yyengine->batts[%d].Cf to %f\n",
						yyengine->curbatt, (yyvsp[-1].dval));
				yyengine->batts[yyengine->curbatt].Cf = (yyvsp[-1].dval);
			}
		}
#line 2996 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 190:
#line 743 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				if (SF_DEBUG) mprint(yyengine, NULL, siminfo, 
					"Setting yyengine->batts[%d].Inominal to %f\n",
					yyengine->curbatt, (yyvsp[-1].dval));
				yyengine->batts[yyengine->curbatt].Inominal = (yyvsp[-1].dval);
			}
		}
#line 3010 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 191:
#line 753 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				if (SF_DEBUG) mprint(yyengine, NULL, siminfo, 
						"Setting yyengine->batts[%d].Rf to %f\n",
						yyengine->curbatt, (yyvsp[-1].dval));
				yyengine->batts[yyengine->curbatt].Rf = (yyvsp[-1].dval);
			}
		}
#line 3024 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 192:
#line 763 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 3042 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 193:
#line 777 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 3064 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 194:
#line 795 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 3082 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 195:
#line 809 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 3104 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 196:
#line 827 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 3122 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 197:
#line 841 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 3144 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 198:
#line 859 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				if ((yyvsp[-1].uval) >= yyengine->nbatts)
				{
					merror(yyengine, "Battery ID out of range.");
				}
				else
				{
					yyengine->curbatt = (yyvsp[-1].uval);
				}
			}
		}
#line 3162 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 199:
#line 873 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_pcbacktrace(yyengine, yyengine->cp);
			}
		}
#line 3173 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 200:
#line 880 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_setbptglobaltime(yyengine, (yyvsp[-1].uval));
			}
		}
#line 3184 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 201:
#line 887 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_setbptcycles(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 3195 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 202:
#line 894 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_setbptinstrs(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 3206 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 203:
#line 901 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_setbptsensorreading(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-1].dval));
			}
		}
#line 3217 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 204:
#line 908 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_bptls(yyengine);
			}
		}
#line 3228 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 205:
#line 915 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_bptdel(yyengine, (yyvsp[-1].uval));
			}
		}
#line 3239 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 206:
#line 922 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_setloc(yyengine, yyengine->cp, (yyvsp[-3].dval), (yyvsp[-2].dval), (yyvsp[-1].dval));
			}
		}
#line 3250 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 207:
#line 929 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				network_setretryalg(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-1].str));
			}
		}
#line 3261 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 208:
#line 936 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_randprint(yyengine, (yyvsp[-7].str), (yyvsp[-6].dval), (yyvsp[-5].dval), (yyvsp[-4].dval), (yyvsp[-3].dval), (yyvsp[-2].dval), (yyvsp[-1].dval));
			}
		}
#line 3272 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 209:
#line 943 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				//m_registerrvar(yyengine->cp, $2, $3, $4, $5, $6, $7,
				//		$8, $9, $10, $11, $12, $13, $14);
			}
		}
#line 3284 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 210:
#line 951 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_initrandtable(yyengine, (yyvsp[-9].str), (yyvsp[-8].str), (yyvsp[-7].dval), (yyvsp[-6].dval), (yyvsp[-5].dval), (yyvsp[-4].dval), (yyvsp[-3].dval), (yyvsp[-2].dval), (yyvsp[-1].dval));
			}
		}
#line 3295 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 211:
#line 958 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				//m_defndist($2, $4, $6);

			}
		}
#line 3307 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 212:
#line 966 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 3329 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 213:
#line 984 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 3351 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 214:
#line 1002 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->ignoredeaths = (yyvsp[-1].uval);
			}
		}
#line 3362 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 215:
#line 1009 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->voltscale_alpha = (yyvsp[-1].dval);
			}
		}
#line 3373 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 216:
#line 1016 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->voltscale_K = (yyvsp[-1].dval);
			}
		}
#line 3384 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 217:
#line 1023 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->voltscale_Vt = (yyvsp[-1].dval);
			}
		}
#line 3395 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 218:
#line 1030 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->schedtype = SchedRandom;
			}
		}
#line 3406 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 219:
#line 1037 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->schedtype = SchedRoundRobin;
			}
		}
#line 3417 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 220:
#line 1044 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->quantum = (yyvsp[-1].uval);
			}
		}
#line 3428 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 221:
#line 1051 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->baseid = (yyvsp[-1].uval);
			}
		}
#line 3439 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 222:
#line 1058 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_renumbernodes(yyengine);
			}
		}
#line 3450 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 223:
#line 1065 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->trip_ustart = musercputimeusecs();
				yyengine->cp->trip_startclk = yyengine->cp->ICLK;
			}
		}
#line 3462 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 224:
#line 1073 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 3479 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 225:
#line 1086 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				network_netseg2file(yyengine, (yyvsp[-2].uval), (yyvsp[-1].str));
			}
		}
#line 3490 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 226:
#line 1093 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				network_file2netseg(yyengine, (yyvsp[-2].str), (yyvsp[-1].uval));
			}
		}
#line 3501 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 227:
#line 1100 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 3517 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 228:
#line 1112 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->settimerintrdelay(yyengine, yyengine->cp, (yyvsp[0].dval));
			}
		}
#line 3528 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 229:
#line 1119 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->phyperiodpsec = (yyvsp[0].dval);
			}
		}
#line 3539 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 230:
#line 1126 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->battperiodpsec = (yyvsp[0].dval);
			}
		}
#line 3550 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 231:
#line 1133 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->netperiodpsec = (yyvsp[0].dval);
			}
		}
#line 3561 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 232:
#line 1140 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->fperiodpsec = (yyvsp[0].dval);
			}
		}
#line 3572 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 233:
#line 1148 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->force_avgpwr = (yyvsp[-1].dval);
				yyengine->cp->force_sleeppwr = (yyvsp[0].dval);
			}
		}
#line 3584 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 234:
#line 1156 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				network_netsegpropmodel(yyengine, (yyvsp[-2].uval), (yyvsp[-1].uval), (yyvsp[0].dval));
			}
		}
#line 3595 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 235:
#line 1163 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->dumpperiodpsec = (yyvsp[0].dval);
			}
		}
#line 3606 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 236:
#line 1170 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_version(yyengine);
			}
		}
#line 3617 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 237:
#line 1177 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				physics_sensorsdbg(yyengine);
			}
		}
#line 3628 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 238:
#line 1185 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				physics_sigsubscr(yyengine, yyengine->cp, (yyvsp[-1].uval), (yyvsp[0].uval));
			}
		}
#line 3639 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 239:
#line 1196 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				physics_newsigsrc(yyengine, (yyvsp[-33].uval), (yyvsp[-32].str), (yyvsp[-31].dval), (yyvsp[-30].dval), (yyvsp[-29].dval), (yyvsp[-28].dval), (yyvsp[-27].dval), (yyvsp[-26].dval), (yyvsp[-25].dval),
					(yyvsp[-24].dval), (yyvsp[-23].dval), (yyvsp[-22].dval), (yyvsp[-21].dval), (yyvsp[-20].dval), (yyvsp[-19].dval), (yyvsp[-18].dval), (yyvsp[-17].dval), (yyvsp[-16].dval), (yyvsp[-15].dval),
					(yyvsp[-14].dval), (yyvsp[-13].dval), (yyvsp[-12].dval), (yyvsp[-11].str), (yyvsp[-10].uval), (yyvsp[-9].dval), (yyvsp[-8].dval), (yyvsp[-7].dval), (yyvsp[-6].uval), (yyvsp[-5].str),
					(yyvsp[-4].uval), (yyvsp[-3].dval), (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3653 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 240:
#line 1206 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				mprint(yyengine, NULL, siminfo,
					"Current directory: %s\n", mgetpwd());
			}
		}
#line 3665 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 241:
#line 1214 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_parseobjdump(yyengine, yyengine->cp, (yyvsp[-1].str));
			}
		}
#line 3676 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 242:
#line 1221 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_dumpall(yyengine, (yyvsp[-3].str), M_OWRITE, (yyvsp[-2].str), (yyvsp[-1].str));
			}
		}
#line 3687 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 243:
#line 1228 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 3705 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 244:
#line 1242 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				if (((yyvsp[-2].uval) > yyengine->nnodes) || ((yyvsp[-1].uval) > yyengine->nnodes))
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
#line 3725 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 245:
#line 1258 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->step = yyengine->cp->cyclestep;
			}
		}
#line 3736 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 246:
#line 1265 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->step = yyengine->cp->faststep;
			}
		}
#line 3747 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 247:
#line 1272 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->cache_init(yyengine, yyengine->cp, (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3758 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 248:
#line 1279 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->cache_deactivate(yyengine, yyengine->cp);
			}
		}
#line 3769 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 249:
#line 1286 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->cache_printstats(yyengine, yyengine->cp);
			}
		}
#line 3780 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 250:
#line 1293 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->dumpregs(yyengine, yyengine->cp);
			}
		}
#line 3791 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 251:
#line 1300 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->dumpsysregs(yyengine, yyengine->cp);
			}
		}
#line 3802 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 252:
#line 1307 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->dumppipe(yyengine, yyengine->cp);
			}
		}
#line 3813 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 253:
#line 1314 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->dumpdistribution(yyengine, yyengine->cp);
			}
		}
#line 3824 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 254:
#line 1321 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->resetcpu(yyengine, yyengine->cp);
			}
		}
#line 3835 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 255:
#line 1328 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				mprint(yyengine, yyengine->cp, nodeinfo,
					"Dynamic instruction count = [" UVLONGFMT "], fetched instructions = [" UVLONGFMT "]\n",
					yyengine->cp->dyncnt, yyengine->cp->nfetched);	
			}
		}
#line 3848 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 256:
#line 1337 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->nodetach = (yyvsp[-1].uval);
			}
		}
#line 3859 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 257:
#line 1344 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_addvaluetrace(yyengine, yyengine->cp, (yyvsp[-7].str), (yyvsp[-6].uval), (yyvsp[-5].uval), (yyvsp[-4].uval), (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3870 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 258:
#line 1351 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_delvaluetrace(yyengine, yyengine->cp, (yyvsp[-7].str), (yyvsp[-6].uval), (yyvsp[-5].uval), (yyvsp[-4].uval), (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3881 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 259:
#line 1358 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_readstabs(yyengine, yyengine->cp, (yyvsp[-1].str));
			}
		}
#line 3892 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 260:
#line 1365 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_valuestats(yyengine, yyengine->cp);
			}
		}
#line 3903 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 261:
#line 1372 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_numaregion(yyengine, (yyvsp[-10].str), (yyvsp[-9].uval), (yyvsp[-8].uval), (yyvsp[-7].sval), (yyvsp[-6].sval), (yyvsp[-5].sval), (yyvsp[-4].sval), (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3914 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 262:
#line 1379 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_numastats(yyengine, yyengine->cp);
			}
		}
#line 3925 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 263:
#line 1386 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_numastatsall(yyengine);
			}
		}
#line 3936 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 264:
#line 1393 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_numasetmapid(yyengine, (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3947 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 265:
#line 1400 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->dumptlb(yyengine, yyengine->cp);
			}
		}
#line 3958 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 266:
#line 1407 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 3978 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 267:
#line 1423 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				mprint(yyengine, NULL, siminfo,
					"Simulation random seed = %ld\n", yyengine->randseed);
			}
		}
#line 3990 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 268:
#line 1431 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				mprint(yyengine, NULL, siminfo,
					"Resetting random number generation with seed [%ld]\n", (yyvsp[-1].uval));
				yyengine->randseed = mrandominit(yyengine, (yyvsp[-1].uval));
			}
		}
#line 4003 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 269:
#line 1440 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->ENABLE_BATT_LOW_INTR = (yyvsp[-1].uval);
			}
		}
#line 4014 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 270:
#line 1447 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->battery_alert_frac = (yyvsp[-1].dval);
			}
		}
#line 4025 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 271:
#line 1454 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->fail_prob = (yyvsp[-1].dval);
			}
		}
#line 4036 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 272:
#line 1461 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->failure_duration_max = (yyvsp[-1].uval);
			}
		}
#line 4047 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 273:
#line 1468 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 4065 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 274:
#line 1483 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				network_netnewseg(yyengine, (yyvsp[-13].uval), (yyvsp[-12].uval), (yyvsp[-11].uval), (yyvsp[-10].uval), (yyvsp[-9].uval), (yyvsp[-8].uval),
					(yyvsp[-7].dval), (yyvsp[-6].dval), (yyvsp[-5].dval), (yyvsp[-4].uval), (yyvsp[-3].dval), (yyvsp[-2].dval), (yyvsp[-1].dval));
			}
		}
#line 4077 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 275:
#line 1491 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 4095 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 276:
#line 1505 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				network_netsegnicattach(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 4106 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 277:
#line 1512 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 4128 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 278:
#line 1530 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				network_netnodenewifc(yyengine, yyengine->cp, (yyvsp[-11].uval), (yyvsp[-10].dval), (yyvsp[-9].dval), (yyvsp[-8].dval),
					(yyvsp[-7].dval), (yyvsp[-6].uval), (yyvsp[-5].dval), (yyvsp[-4].dval), (yyvsp[-3].dval), (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 4140 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 279:
#line 1538 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				network_netsegdelete(yyengine, (yyvsp[-1].uval));
			}
		}
#line 4151 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 280:
#line 1545 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				merror(yyengine, "Command \"NODEFAILPROBFN\" unimplemented.");
			}
		}
#line 4162 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 281:
#line 1552 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				merror(yyengine, "Command \"NETSEGFAILPROBFN\" unimplemented.");
			}
		}
#line 4173 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 282:
#line 1559 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_sizemem(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 4184 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 283:
#line 1566 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				pau_init(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 4195 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 284:
#line 1573 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->split(yyengine, yyengine->cp, (yyvsp[-4].uval), (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].str));
			}
		}
#line 4206 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 285:
#line 1580 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_dumpmem(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 4217 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 286:
#line 1587 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->pipelined = 0;
			}
		}
#line 4228 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 287:
#line 1594 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->pipelined = 1;
			}
		}
#line 4239 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 288:
#line 1601 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->superH->ENABLE_CLK_INTR = (yyvsp[-1].uval);
			}
		}
#line 4250 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 289:
#line 1608 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->runnable = 0;
			}
		}
#line 4261 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 290:
#line 1615 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				sfatal(yyengine, yyengine->cp, (yyvsp[-1].str));
			}
		}
#line 4272 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 291:
#line 1622 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->verbose ^= 1;
			}
		}
#line 4283 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 292:
#line 1629 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_run(yyengine, yyengine->cp, (yyvsp[-1].str));
			}

			/*	The 'string' is dynamically allocated, in lex.c		*/
			free((yyvsp[-1].str));
		}
#line 4297 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 293:
#line 1639 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->step(yyengine, yyengine->cp, 0);
			}
		}
#line 4308 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 294:
#line 1646 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				go(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 4319 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 295:
#line 1653 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				loadcmds(yyengine, (yyvsp[-1].str));
			}
		}
#line 4330 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 296:
#line 1660 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {	
			if (!yyengine->scanning)
			{
				yyengine->cp->PC = (yyvsp[-1].uval);
			}
		}
#line 4341 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 297:
#line 1667 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				/*	Scale frequency accordingly for provided Vdd	*/
				power_scaledelay(yyengine, yyengine->cp, (yyvsp[-1].dval));
			}
		}
#line 4353 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 298:
#line 1675 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->mem_r_latency = (yyvsp[-1].uval);
			}
		}
#line 4364 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 299:
#line 1682 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->mem_w_latency = (yyvsp[-1].uval);
			}
		}
#line 4375 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 300:
#line 1689 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->flash_r_latency = (yyvsp[-1].uval);
			}
		}
#line 4386 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 301:
#line 1696 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->flash_w_latency = (yyvsp[-1].uval);
			}
		}
#line 4397 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 302:
#line 1703 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				/*	Scale Vdd accordingly for provided frequency	*/
				power_scalevdd(yyengine, yyengine->cp, (yyvsp[-1].dval));
			}
		}
#line 4409 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 303:
#line 1711 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				/*	Set node mass	*/
				massSetNodeMass(yyengine, yyengine->cp, (yyvsp[-1].dval));
			}
		}
#line 4421 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 304:
#line 1719 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				/*	Scale Vdd accordingly for provided frequency	*/
				propulsionSetPropulsionCoeffs(yyengine, yyengine->cp,
											(yyvsp[-17].dval),	(yyvsp[-16].dval),	(yyvsp[-15].dval),	(yyvsp[-14].dval),	(yyvsp[-13].dval),	(yyvsp[-12].dval),
											(yyvsp[-11].dval),	(yyvsp[-10].dval),	(yyvsp[-9].dval),	(yyvsp[-8].dval),	(yyvsp[-7].dval),	(yyvsp[-6].dval),
											(yyvsp[-5].dval),	(yyvsp[-4].dval),	(yyvsp[-3].dval),	(yyvsp[-2].dval),	(yyvsp[-1].dval),	(yyvsp[0].dval));
			}
		}
#line 4436 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 305:
#line 1730 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			yyengine->cp->MEMBASE = (yyvsp[0].uval);
		}
#line 4444 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 306:
#line 1734 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			mprint(yyengine, NULL, siminfo, "Memory base address is %d.",yyengine->cp->MEMBASE);
		}
#line 4452 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 307:
#line 1738 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				help(yyengine);
			}
		}
#line 4463 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 308:
#line 1745 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				man(yyengine, (yyvsp[-1].str));
			}
		}
#line 4474 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 309:
#line 1752 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->flushpipe(yyengine->cp);
			}
		}
#line 4485 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 310:
#line 1759 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->pipeshow = !yyengine->cp->pipeshow;
			}
		}
#line 4496 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 311:
#line 1766 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				savemem(yyengine, yyengine->cp, (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].str));
			}
		}
#line 4507 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 312:
#line 1773 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				load_srec(yyengine, yyengine->cp, (yyvsp[-1].str));
			}
		}
#line 4518 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 313:
#line 1780 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				load_mapfile(yyengine, yyengine->cp, (yyvsp[-1].str));
			}
		}
#line 4529 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 314:
#line 1787 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				cont(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 4540 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 315:
#line 1794 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_sharebus(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 4551 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 316:
#line 1801 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				mprint(yyengine, NULL, siminfo,
					"CLK = " UVLONGFMT ", ICLK = " UVLONGFMT ", TIME = %E, CYCLETIME = %E\n",
					yyengine->cp->CLK, yyengine->cp->ICLK, yyengine->cp->TIME, 
					yyengine->cp->CYCLETIME);
			}
		}
#line 4565 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 317:
#line 1811 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				power_printstats(yyengine, yyengine->cp);
			}
		}
#line 4576 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 318:
#line 1818 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_locstats(yyengine, yyengine->cp);
			}
		}
#line 4587 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 319:
#line 1825 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_listrvars(yyengine);
			}
		}
#line 4598 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 320:
#line 1833 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_on(yyengine, yyengine->cp);
			}
		}
#line 4609 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 321:
#line 1840 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_off(yyengine, yyengine->cp);
			}
		}
#line 4620 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 322:
#line 1847 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				network_netdebug(yyengine, yyengine->cp);
			}
		}
#line 4631 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 323:
#line 1854 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				mprint(yyengine, NULL, siminfo, 
					"WARNING: Trace only works if compiled w/ -MYRMIGKI_DEBUG\n\n");
				yyengine->cp->trace = (yyvsp[-1].uval);
			}
		}
#line 4644 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 324:
#line 1863 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->throttlensec = (yyvsp[-1].uval);
			}
		}
#line 4655 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 325:
#line 1870 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->throttlewin = (yyvsp[-1].uval);
			}
		}
#line 4666 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 326:
#line 1877 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->faultthreshold = (yyvsp[-1].uval);
			}
		}
#line 4677 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 327:
#line 1884 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->ENABLE_TOO_MANY_FAULTS = (yyvsp[-1].uval);
			}
		}
#line 4688 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 328:
#line 1891 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				fault_setnodepfun(yyengine, yyengine->cp, (yyvsp[-1].str));
			}
		}
#line 4699 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 329:
#line 1898 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 4718 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 330:
#line 1913 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				mmblocksdisplay(yyengine);
			}
		}
#line 4729 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 331:
#line 1920 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				pau_printstats(yyengine, yyengine->cp);
			}
		}
#line 4740 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 332:
#line 1927 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				// TODO: we should account for the cost of the m_on and m_off

				m_off(yyengine, yyengine->cp);
				mnsleep((yyvsp[-1].uval));
				m_on(yyengine, yyengine->cp);
			}
		}
#line 4755 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 333:
#line 1938 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				// TODO: we should account for the cost of the m_on and m_off

				m_off(yyengine, yyengine->cp);
				mnsleep((yyvsp[-1].uval) * 1000000000);
				m_on(yyengine, yyengine->cp);
			}
		}
#line 4770 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 334:
#line 1949 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				/*	For now, nothing fun is done with comments	*/
			}
		}
#line 4781 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 335:
#line 1956 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 4796 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 336:
#line 1970 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 4824 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 337:
#line 1997 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
		}
#line 4831 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 338:
#line 2003 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
		}
#line 4838 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 339:
#line 2009 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
		}
#line 4845 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 340:
#line 2015 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
		}
#line 4852 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 341:
#line 2020 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
		}
#line 4859 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 342:
#line 2025 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 4887 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 343:
#line 2050 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 4912 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 344:
#line 2073 "sf-hitachi-sh.y" /* yacc.c:1652  */
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

				if (yyengine->cp->PC - yyengine->cp->MEMBASE < 0 ||
					yyengine->cp->PC - yyengine->cp->MEMBASE > yyengine->cp->MEMSIZE - 1)
				{
					sfatal(yyengine, yyengine->cp, "Invalid PC address. Must be within alocated memory.");
				}
				else
				{
					memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
						&tmp, sizeof(tmp));
				}

				yyengine->cp->PC += 2;
			}
		}
#line 4955 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 345:
#line 2114 "sf-hitachi-sh.y" /* yacc.c:1652  */
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

				if (yyengine->cp->PC - yyengine->cp->MEMBASE < 0 ||
					yyengine->cp->PC - yyengine->cp->MEMBASE > yyengine->cp->MEMSIZE - 1)
				{
					sfatal(yyengine, yyengine->cp, "Invalid PC address. Must be within allocated memory.");
				}
				else
				{
					memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
						&tmp, sizeof(tmp));
				}
				yyengine->cp->PC += 2;
				
			}
		}
#line 4997 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 346:
#line 2154 "sf-hitachi-sh.y" /* yacc.c:1652  */
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

				if (yyengine->cp->PC - yyengine->cp->MEMBASE < 0 ||
					yyengine->cp->PC - yyengine->cp->MEMBASE > yyengine->cp->MEMSIZE - 1)
				{
					sfatal(yyengine, yyengine->cp, "Invalid PC address. Must be within allocated memory.");
				}
				else
				{
					memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
						&tmp, sizeof(tmp));
				}
				yyengine->cp->PC += 2;
			}
		}
#line 5039 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 347:
#line 2194 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
			
				if (yyengine->cp->PC - yyengine->cp->MEMBASE < 0 ||
					yyengine->cp->PC - yyengine->cp->MEMBASE > yyengine->cp->MEMSIZE - 1)
				{
					sfatal(yyengine, yyengine->cp, "Invalid PC address. Must be within allocated memory.");
				}
				else
				{
					memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
						&tmp, sizeof(tmp));
				}
				yyengine->cp->PC += 2;
			}
		}
#line 5081 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 348:
#line 2234 "sf-hitachi-sh.y" /* yacc.c:1652  */
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

				if (yyengine->cp->PC - yyengine->cp->MEMBASE < 0 ||
					yyengine->cp->PC - yyengine->cp->MEMBASE > yyengine->cp->MEMSIZE - 1)
				{
					sfatal(yyengine, yyengine->cp, "Invalid PC address. Must be within allocated memory.");
				}
				else
				{
					memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
						&tmp, sizeof(tmp));
				}
				yyengine->cp->PC += 2;
			}
		}
#line 5123 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 349:
#line 2274 "sf-hitachi-sh.y" /* yacc.c:1652  */
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

				if (yyengine->cp->PC - yyengine->cp->MEMBASE < 0 ||
					yyengine->cp->PC - yyengine->cp->MEMBASE > yyengine->cp->MEMSIZE - 1)
				{
					sfatal(yyengine, yyengine->cp, "Invalid PC address. Must be within allocated memory.");
				}
				else
				{
					memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
						&tmp, sizeof(tmp));
				}
				yyengine->cp->PC += 2;
			}
		}
#line 5163 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 350:
#line 2312 "sf-hitachi-sh.y" /* yacc.c:1652  */
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

				if (yyengine->cp->PC - yyengine->cp->MEMBASE < 0 ||
					yyengine->cp->PC - yyengine->cp->MEMBASE > yyengine->cp->MEMSIZE - 1)
				{
					sfatal(yyengine, yyengine->cp, "Invalid PC address. Must be within allocated memory.");
				}
				else
				{
					memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
						&tmp, sizeof(tmp));
				}
				yyengine->cp->PC += 2;
			}
		}
#line 5203 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 351:
#line 2350 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5240 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 352:
#line 2385 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5277 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 353:
#line 2420 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5314 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 354:
#line 2455 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5347 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 355:
#line 2486 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5379 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 356:
#line 2516 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5412 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 357:
#line 2547 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5449 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 358:
#line 2582 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5486 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 359:
#line 2617 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5517 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 360:
#line 2646 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5548 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 361:
#line 2675 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5579 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 362:
#line 2704 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5613 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 363:
#line 2736 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5647 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 364:
#line 2768 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5681 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 365:
#line 2800 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5715 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 366:
#line 2832 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5749 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 367:
#line 2864 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5781 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 368:
#line 2894 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5814 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 369:
#line 2925 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5848 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 370:
#line 2957 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5880 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 371:
#line 2987 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5914 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 372:
#line 3019 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5948 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 373:
#line 3051 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 5982 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 374:
#line 3083 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6016 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 375:
#line 3115 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6049 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 376:
#line 3146 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6083 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 377:
#line 3178 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6117 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 378:
#line 3210 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6151 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 379:
#line 3242 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6185 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 380:
#line 3274 "sf-hitachi-sh.y" /* yacc.c:1652  */
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


				if (yyengine->cp->PC - yyengine->cp->MEMBASE < 0 ||
					yyengine->cp->PC - yyengine->cp->MEMBASE > yyengine->cp->MEMSIZE - 1)
				{
					sfatal(yyengine, yyengine->cp, "Invalid PC address. Must be within alocated memory.");
				}
				else
				{
					memmove(&yyengine->cp->MEM[yyengine->cp->PC - yyengine->cp->MEMBASE],
						&tmp, sizeof(tmp));
				}

				yyengine->cp->PC += 2;
			}
		}
#line 6228 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 381:
#line 3315 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6261 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 382:
#line 3346 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6294 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 383:
#line 3377 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6327 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 384:
#line 3408 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6360 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 385:
#line 3439 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6393 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 386:
#line 3470 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6426 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 387:
#line 3501 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6461 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 388:
#line 3534 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6494 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 389:
#line 3565 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6522 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 390:
#line 3591 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6555 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 391:
#line 3622 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6588 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 392:
#line 3653 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6621 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 393:
#line 3684 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6656 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 394:
#line 3717 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6689 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 395:
#line 3748 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6722 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 396:
#line 3779 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6755 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 397:
#line 3810 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6783 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 398:
#line 3836 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6816 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 399:
#line 3867 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6849 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 400:
#line 3898 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6880 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 401:
#line 3927 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6914 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 402:
#line 3959 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6948 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 403:
#line 3991 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 6982 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 404:
#line 4023 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7016 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 405:
#line 4055 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7050 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 406:
#line 4087 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7084 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 407:
#line 4119 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7118 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 408:
#line 4151 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7152 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 409:
#line 4183 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7186 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 410:
#line 4215 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7220 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 411:
#line 4247 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7254 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 412:
#line 4279 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7288 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 413:
#line 4311 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7322 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 414:
#line 4343 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7356 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 415:
#line 4375 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7390 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 416:
#line 4407 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7424 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 417:
#line 4439 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7458 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 418:
#line 4471 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7492 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 419:
#line 4503 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7526 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 420:
#line 4535 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7561 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 421:
#line 4568 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7595 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 422:
#line 4600 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7628 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 423:
#line 4631 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7666 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 424:
#line 4669 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7704 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 425:
#line 4704 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7742 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 426:
#line 4740 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7779 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 427:
#line 4775 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7816 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 428:
#line 4810 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7853 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 429:
#line 4845 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7890 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 430:
#line 4880 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7927 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 431:
#line 4915 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 7964 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 432:
#line 4950 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8002 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 433:
#line 4986 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8040 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 434:
#line 5022 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8079 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 435:
#line 5059 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8117 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 436:
#line 5095 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8155 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 437:
#line 5131 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8194 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 438:
#line 5168 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8231 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 439:
#line 5203 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8264 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 440:
#line 5234 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8298 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 441:
#line 5266 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8332 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 442:
#line 5298 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8366 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 443:
#line 5330 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8400 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 444:
#line 5362 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8434 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 445:
#line 5394 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8468 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 446:
#line 5426 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8502 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 447:
#line 5458 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8533 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 448:
#line 5487 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8567 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 449:
#line 5519 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8601 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 450:
#line 5551 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8633 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 451:
#line 5581 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8665 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 452:
#line 5611 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8698 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 453:
#line 5642 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8730 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 454:
#line 5671 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8763 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 455:
#line 5702 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8796 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 456:
#line 5733 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8829 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 457:
#line 5764 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8862 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 458:
#line 5795 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8893 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 459:
#line 5824 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8924 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 460:
#line 5853 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8955 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 461:
#line 5882 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 8986 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 462:
#line 5911 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9020 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 463:
#line 5943 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9053 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 464:
#line 5974 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9086 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 465:
#line 6005 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9120 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 466:
#line 6037 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9153 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 467:
#line 6068 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9186 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 468:
#line 6099 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9219 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 469:
#line 6130 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9252 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 470:
#line 6161 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9285 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 471:
#line 6192 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9318 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 472:
#line 6223 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9351 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 473:
#line 6254 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9384 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 474:
#line 6285 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9415 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 475:
#line 6314 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9448 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 476:
#line 6345 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9481 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 477:
#line 6376 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9514 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 478:
#line 6407 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9547 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 479:
#line 6438 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9580 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 480:
#line 6469 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9615 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 481:
#line 6502 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9648 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 482:
#line 6533 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9681 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 483:
#line 6564 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9714 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 484:
#line 6595 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9747 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 485:
#line 6626 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9780 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 486:
#line 6657 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9815 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 487:
#line 6690 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9848 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 488:
#line 6721 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9881 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 489:
#line 6752 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9914 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 490:
#line 6783 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9947 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 491:
#line 6814 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 9980 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 492:
#line 6845 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 10013 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 493:
#line 6876 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 10047 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 494:
#line 6908 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 10081 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 495:
#line 6940 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 10115 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 496:
#line 6972 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 10149 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 497:
#line 7004 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 10183 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 498:
#line 7036 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 10216 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 499:
#line 7067 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 10248 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 500:
#line 7097 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 10282 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 501:
#line 7129 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 10314 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 502:
#line 7159 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 10346 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 503:
#line 7189 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 10380 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 504:
#line 7221 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 10412 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 505:
#line 7251 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 10444 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 506:
#line 7281 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 10478 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 509:
#line 7330 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				//$$->value = m_randgen(yyengine->cp, $3, $4, $5, $6, $7);
			}
		}
#line 10489 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 510:
#line 7339 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 10506 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 511:
#line 7934 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.uval) = strtoul((yyvsp[0].str), &ep, 0);
			
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid unsigned immediate data %s.", (yyvsp[0].str));
			}
		}
#line 10522 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 512:
#line 7948 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.sval) = strtol((yyvsp[0].str), &ep, 0);
			
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid signed immediate data %s.", (yyvsp[0].str));
			}
		}
#line 10538 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 513:
#line 7960 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.sval) = -(strtol((yyvsp[0].str), &ep, 0));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid signed immediate data %s.", (yyvsp[0].str));
			}
		}
#line 10554 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 514:
#line 7972 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.sval) = +(strtol((yyvsp[0].str), &ep, 0));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid signed immediate data %s.", (yyvsp[0].str));
			}
		}
#line 10570 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 515:
#line 7986 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.dval) = strtod((yyvsp[0].str), &ep);
			
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid double immediate data %s.", (yyvsp[0].str));
			}
		}
#line 10586 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 516:
#line 7998 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.dval) = -(strtod((yyvsp[0].str), &ep));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid double immediate data %s.", (yyvsp[0].str));
			}
		}
#line 10602 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 517:
#line 8010 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.dval) = +(strtod((yyvsp[0].str), &ep));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid double immediate data %s.", (yyvsp[0].str));
			}
		}
#line 10618 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 518:
#line 8024 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			(yyval.rval) = (Rval *) mcalloc(yyengine, 1, sizeof(Rval), "sf.y:rdimm/$$");
			if ((yyval.rval) == NULL)
			{
				sfatal(yyengine, yyengine->cp, "Mcalloc failed");
			}

			(yyval.rval)->dval = (yyvsp[0].dval);
		}
#line 10632 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 520:
#line 8072 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			(yyval.dlist) = (yyvsp[-1].dlist);
		}
#line 10640 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 521:
#line 8078 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 10665 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 522:
#line 8099 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 10684 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 523:
#line 8117 "sf-hitachi-sh.y" /* yacc.c:1652  */
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
#line 10709 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 524:
#line 8138 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			char tmp;
			char *ep = &tmp;


			(yyval.sval) = strtol((yyvsp[0].str), &ep, 0);
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid DISP ([%s]). Possibly due to a .comm.", (yyvsp[0].str));
			}
		}
#line 10726 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 525:
#line 8151 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			char tmp;
			char *ep = &tmp;


			(yyval.sval) = +(strtol((yyvsp[0].str), &ep, 0));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid DISP ([%s]). Possibly due to a .comm.", (yyvsp[0].str));
			}
		}
#line 10743 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 526:
#line 8164 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			char tmp;
			char *ep = &tmp;


			(yyval.sval) = -(strtol((yyvsp[0].str), &ep, 0));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid DISP ([%s]). Possibly due to a .comm.", (yyvsp[0].str));
			}
		}
#line 10760 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 527:
#line 8179 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			/*								*/
			/*	To make handling of empty string in T_RUN rule 		*/
			/*	uniform, we malloc a place holder.			*/
			/*								*/
			char *tmp = mmalloc(yyengine, sizeof(char), "opstring rule in shasm.y");
			tmp[0] = '\0';
			(yyval.str) = tmp;
		}
#line 10774 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 528:
#line 8189 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {
			(yyval.str) = (yyvsp[0].str);
		}
#line 10782 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 529:
#line 8195 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {(yyval.uval) = 0;}
#line 10788 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 530:
#line 8196 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {(yyval.uval) = 1;}
#line 10794 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 531:
#line 8197 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {(yyval.uval) = 2;}
#line 10800 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 532:
#line 8198 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {(yyval.uval) = 3;}
#line 10806 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 533:
#line 8199 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {(yyval.uval) = 4;}
#line 10812 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 534:
#line 8200 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {(yyval.uval) = 5;}
#line 10818 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 535:
#line 8201 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {(yyval.uval) = 6;}
#line 10824 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 536:
#line 8202 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {(yyval.uval) = 7;}
#line 10830 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 537:
#line 8203 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {(yyval.uval) = 8;}
#line 10836 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 538:
#line 8204 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {(yyval.uval) = 9;}
#line 10842 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 539:
#line 8205 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {(yyval.uval) = 10;}
#line 10848 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 540:
#line 8206 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {(yyval.uval) = 11;}
#line 10854 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 541:
#line 8207 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {(yyval.uval) = 12;}
#line 10860 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 542:
#line 8208 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {(yyval.uval) = 13;}
#line 10866 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 543:
#line 8209 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {(yyval.uval) = 14;}
#line 10872 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;

  case 544:
#line 8210 "sf-hitachi-sh.y" /* yacc.c:1652  */
    {(yyval.uval) = 15;}
#line 10878 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
    break;


#line 10882 "sf-hitachi-sh.tab.c" /* yacc.c:1652  */
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
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

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


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
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
#line 8212 "sf-hitachi-sh.y" /* yacc.c:1918  */


#include "lex-hitachi-sh.c"

int
yyerror(char *err)
{
	merror(yyengine, "Invalid command! (for superH)");
	clearistream(yyengine);
	
	return 0;
}
