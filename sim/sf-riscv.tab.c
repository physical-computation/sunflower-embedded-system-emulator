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
#define yyparse         sf_riscv_parse
#define yylex           sf_riscv_lex
#define yyerror         sf_riscv_error
#define yydebug         sf_riscv_debug
#define yynerrs         sf_riscv_nerrs

#define yylval          sf_riscv_lval
#define yychar          sf_riscv_char

/* First part of user prologue.  */
#line 39 "sf-riscv.y" /* yacc.c:337  */

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

#line 96 "sf-riscv.tab.c" /* yacc.c:337  */
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
extern int sf_riscv_debug;
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
    T_DUMPHIST = 287,
    T_DUMPHISTPRETTY = 288,
    T_LDHISTRND = 289,
    T_ADDHIST = 290,
    T_DUMPSYSREGS = 291,
    T_DUMPTIME = 292,
    T_DUMPTLB = 293,
    T_DYNINSTR = 294,
    T_EBATTINTR = 295,
    T_EFAULTS = 296,
    T_ENABLEPIPELINE = 297,
    T_FILE2NETSEG = 298,
    T_FLTTHRESH = 299,
    T_FORCEAVGPWR = 300,
    T_HELP = 301,
    T_IGNORENODEDEATHS = 302,
    T_INITSEESTATE = 303,
    T_HWSEEREG = 304,
    T_REGISTERRVAR = 305,
    T_INITRANDTABLE = 306,
    T_DEFNDIST = 307,
    T_RANDPRINT = 308,
    T_LOAD = 309,
    T_LOCSTATS = 310,
    T_LISTRVARS = 311,
    T_MALLOCDEBUG = 312,
    T_MMAP = 313,
    T_MODECA = 314,
    T_MODEFF = 315,
    T_NETCORREL = 316,
    T_NETDEBUG = 317,
    T_NETNEWSEG = 318,
    T_NETNODENEWIFC = 319,
    T_NETSEG2FILE = 320,
    T_NETSEGDELETE = 321,
    T_NETSEGFAILDURMAX = 322,
    T_NETSEGFAILPROB = 323,
    T_NETSEGFAILPROBFN = 324,
    T_NETSEGNICATTACH = 325,
    T_NETSEGPROPMODEL = 326,
    T_NEWBATT = 327,
    T_NEWNODE = 328,
    T_NODEFAILDURMAX = 329,
    T_NODEFAILPROB = 330,
    T_NODEFAILPROBFN = 331,
    T_NODETACH = 332,
    T_NUMAREGION = 333,
    T_NUMASTATS = 334,
    T_NUMASTATSALL = 335,
    T_NUMASETMAPID = 336,
    T_ADDVALUETRACE = 337,
    T_DELVALUETRACE = 338,
    T_REGISTERSTABS = 339,
    T_VALUESTATS = 340,
    T_OFF = 341,
    T_ON = 342,
    T_PAUINFO = 343,
    T_NANOPAUSE = 344,
    T_PAUSE = 345,
    T_PFUN = 346,
    T_PIPEFLUSH = 347,
    T_POWERSTATS = 348,
    T_POWERTOTAL = 349,
    T_PWD = 350,
    T_PARSEOBJDUMP = 351,
    T_QUIT = 352,
    T_GETRANDOMSEED = 353,
    T_SETRANDOMSEED = 354,
    T_RATIO = 355,
    T_RENUMBERNODES = 356,
    T_RESETNODECTRS = 357,
    T_RESETALLCTRS = 358,
    T_RESETCPU = 359,
    T_RETRYALG = 360,
    T_RUN = 361,
    T_SAVE = 362,
    T_SENSORSDEBUG = 363,
    T_SETBASENODEID = 364,
    T_SETBATT = 365,
    T_SETBATTFEEDPERIOD = 366,
    T_SETDUMPPWRPERIOD = 367,
    T_SETNETPERIOD = 368,
    T_SETFAULTPERIOD = 369,
    T_SETFREQ = 370,
    T_SETIFCOUI = 371,
    T_SETMEMBASE = 372,
    T_SETNODE = 373,
    T_SETPC = 374,
    T_SETPHYSICSPERIOD = 375,
    T_SETQUANTUM = 376,
    T_SETSCALEALPHA = 377,
    T_SETSCALEK = 378,
    T_SETSCALEVT = 379,
    T_SETSCHEDRANDOM = 380,
    T_SETSCHEDROUNDROBIN = 381,
    T_SETTAG = 382,
    T_SETTIMERDELAY = 383,
    T_SETVDD = 384,
    T_SETMEMRLATENCY = 385,
    T_SETMEMWLATENCY = 386,
    T_SETFLASHRLATENCY = 387,
    T_SETFLASHWLATENCY = 388,
    T_SETNODEMASS = 389,
    T_SETPROPULSIONCOEFFS = 390,
    T_SHAREBUS = 391,
    T_SHOWCLK = 392,
    T_SHOWMEMBASE = 393,
    T_SHOWPIPE = 394,
    T_SHOWTAGS = 395,
    T_SIGNALSRC = 396,
    T_SIGNALSUBSCRIBE = 397,
    T_SIZEMEM = 398,
    T_SIZEPAU = 399,
    T_SPLIT = 400,
    T_SRECL = 401,
    T_LOADMAPFILE = 402,
    T_STEP = 403,
    T_STOP = 404,
    T_SFATAL = 405,
    T_TRACE = 406,
    T_THROTTLE = 407,
    T_THROTTLEWIN = 408,
    T_VERBOSE = 409,
    T_VERSION = 410,
    T_SETLOC = 411,
    T_BPT = 412,
    T_BPTLS = 413,
    T_BPTDEL = 414,
    T_TAINTMEM = 415,
    T_TAINTREG = 416,
    T_FTAINTREG = 417,
    T_DUMPTAINTDISTR = 418,
    T_GLOBALTIME = 419,
    T_CYCLES = 420,
    T_INSTRS = 421,
    T_SENSORREADING = 422,
    T_DOTALIGN = 423,
    T_DOTORG = 424,
    T_DOTDATA = 425,
    T_DOTTEXT = 426,
    T_DOTGLOBAL = 427,
    T_DOTFILE = 428,
    T_DOTLONG = 429,
    T_DOTCOMM = 430,
    T_X0 = 431,
    T_X1 = 432,
    T_X2 = 433,
    T_X3 = 434,
    T_X4 = 435,
    T_X5 = 436,
    T_X6 = 437,
    T_X7 = 438,
    T_X8 = 439,
    T_X9 = 440,
    T_X10 = 441,
    T_X11 = 442,
    T_X12 = 443,
    T_X13 = 444,
    T_X14 = 445,
    T_X15 = 446,
    T_X16 = 447,
    T_X17 = 448,
    T_X18 = 449,
    T_X19 = 450,
    T_X20 = 451,
    T_X21 = 452,
    T_X22 = 453,
    T_X23 = 454,
    T_X24 = 455,
    T_X25 = 456,
    T_X26 = 457,
    T_X27 = 458,
    T_X28 = 459,
    T_X29 = 460,
    T_X30 = 461,
    T_X31 = 462,
    T_PC = 463,
    T_F0 = 464,
    T_F1 = 465,
    T_F2 = 466,
    T_F3 = 467,
    T_F4 = 468,
    T_F5 = 469,
    T_F6 = 470,
    T_F7 = 471,
    T_F8 = 472,
    T_F9 = 473,
    T_F10 = 474,
    T_F11 = 475,
    T_F12 = 476,
    T_F13 = 477,
    T_F14 = 478,
    T_F15 = 479,
    T_F16 = 480,
    T_F17 = 481,
    T_F18 = 482,
    T_F19 = 483,
    T_F20 = 484,
    T_F21 = 485,
    T_F22 = 486,
    T_F23 = 487,
    T_F24 = 488,
    T_F25 = 489,
    T_F26 = 490,
    T_F27 = 491,
    T_F28 = 492,
    T_F29 = 493,
    T_F30 = 494,
    T_F31 = 495,
    T_LABELDEFN = 496,
    T_ADD = 497,
    T_ADDI = 498,
    T_AND = 499,
    T_ANDI = 500,
    T_AUIPC = 501,
    T_BEQ = 502,
    T_BGE = 503,
    T_BGEU = 504,
    T_BLT = 505,
    T_BLTU = 506,
    T_BNE = 507,
    T_FENCE = 508,
    T_FENCE_I = 509,
    T_JAL = 510,
    T_JALR = 511,
    T_LB = 512,
    T_LBU = 513,
    T_LH = 514,
    T_LHU = 515,
    T_LUI = 516,
    T_LW = 517,
    T_OR = 518,
    T_ORI = 519,
    T_SB = 520,
    T_SH = 521,
    T_SLL = 522,
    T_SLLI = 523,
    T_SLT = 524,
    T_SLTI = 525,
    T_SLTIU = 526,
    T_SLTU = 527,
    T_SRA = 528,
    T_SRAI = 529,
    T_SRL = 530,
    T_SRLI = 531,
    T_SUB = 532,
    T_SW = 533,
    T_XOR = 534,
    T_XORI = 535,
    T_LABEL = 536,
    T_STRING = 537,
    T_MAN = 538
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 73 "sf-riscv.y" /* yacc.c:352  */

	double		dval;
	ulong		uval;
	long		sval;
	char		*str;
	Rval		*rval;
	DoubleList	*dlist;

#line 429 "sf-riscv.tab.c" /* yacc.c:352  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE sf_riscv_lval;

int sf_riscv_parse (void);





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
#define YYLAST   2221

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  293
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  65
/* YYNRULES -- Number of rules.  */
#define YYNRULES  321
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  980

#define YYUNDEFTOK  2
#define YYMAXUTOK   539

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     284,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   292,   286,   291,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     288,     2,   290,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   285,     2,   287,     2,     2,     2,     2,
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
     275,   276,   277,   278,   279,   280,   281,   282,   283,   289
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   444,   444,   445,   448,   449,   450,   451,   452,   455,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,   482,   483,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   494,   495,   496,   497,
     498,   499,   500,   501,   502,   503,   504,   505,   509,   522,
     535,   548,   561,   574,   584,   594,   622,   629,   636,   643,
     653,   663,   673,   683,   697,   715,   729,   747,   761,   779,
     793,   800,   807,   814,   821,   828,   835,   842,   849,   856,
     863,   871,   878,   886,   904,   922,   929,   936,   943,   950,
     957,   964,   971,   978,   985,   993,  1006,  1013,  1020,  1032,
    1039,  1046,  1053,  1060,  1068,  1076,  1083,  1090,  1097,  1105,
    1112,  1126,  1134,  1141,  1148,  1162,  1178,  1185,  1192,  1199,
    1206,  1213,  1220,  1227,  1234,  1241,  1248,  1255,  1262,  1269,
    1276,  1285,  1292,  1299,  1306,  1313,  1320,  1327,  1334,  1341,
    1348,  1355,  1371,  1379,  1388,  1395,  1402,  1409,  1416,  1430,
    1439,  1453,  1460,  1478,  1486,  1493,  1500,  1507,  1514,  1521,
    1528,  1535,  1542,  1549,  1556,  1563,  1570,  1577,  1587,  1594,
    1601,  1608,  1615,  1623,  1630,  1637,  1644,  1651,  1659,  1667,
    1678,  1683,  1687,  1694,  1701,  1708,  1715,  1722,  1729,  1736,
    1743,  1750,  1760,  1767,  1774,  1782,  1789,  1796,  1803,  1812,
    1819,  1826,  1833,  1840,  1847,  1862,  1869,  1876,  1887,  1898,
    1905,  1919,  1946,  1952,  1958,  1964,  1969,  1974,  1999,  2022,
    2063,  2103,  2144,  2184,  2220,  2268,  2313,  2358,  2403,  2448,
    2493,  2500,  2507,  2550,  2589,  2629,  2669,  2709,  2749,  2785,
    2825,  2866,  2906,  2951,  2996,  3037,  3078,  3119,  3159,  3199,
    3240,  3281,  3322,  3363,  3404,  3445,  3490,  3531,  3571,  3571,
    3588,  3597,  4192,  4206,  4218,  4230,  4244,  4256,  4268,  4282,
    4292,  4330,  4336,  4357,  4375,  4396,  4409,  4422,  4438,  4447,
    4454,  4455,  4456,  4457,  4458,  4459,  4460,  4461,  4462,  4463,
    4464,  4465,  4466,  4467,  4468,  4469,  4470,  4471,  4472,  4473,
    4474,  4475,  4476,  4477,  4478,  4479,  4480,  4481,  4482,  4483,
    4484,  4485
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
  "T_DUMPDISTRIBUTION", "T_DUMPPWR", "T_DUMPREGS", "T_DUMPHIST",
  "T_DUMPHISTPRETTY", "T_LDHISTRND", "T_ADDHIST", "T_DUMPSYSREGS",
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
  "T_TAINTMEM", "T_TAINTREG", "T_FTAINTREG", "T_DUMPTAINTDISTR",
  "T_GLOBALTIME", "T_CYCLES", "T_INSTRS", "T_SENSORREADING", "T_DOTALIGN",
  "T_DOTORG", "T_DOTDATA", "T_DOTTEXT", "T_DOTGLOBAL", "T_DOTFILE",
  "T_DOTLONG", "T_DOTCOMM", "T_X0", "T_X1", "T_X2", "T_X3", "T_X4", "T_X5",
  "T_X6", "T_X7", "T_X8", "T_X9", "T_X10", "T_X11", "T_X12", "T_X13",
  "T_X14", "T_X15", "T_X16", "T_X17", "T_X18", "T_X19", "T_X20", "T_X21",
  "T_X22", "T_X23", "T_X24", "T_X25", "T_X26", "T_X27", "T_X28", "T_X29",
  "T_X30", "T_X31", "T_PC", "T_F0", "T_F1", "T_F2", "T_F3", "T_F4", "T_F5",
  "T_F6", "T_F7", "T_F8", "T_F9", "T_F10", "T_F11", "T_F12", "T_F13",
  "T_F14", "T_F15", "T_F16", "T_F17", "T_F18", "T_F19", "T_F20", "T_F21",
  "T_F22", "T_F23", "T_F24", "T_F25", "T_F26", "T_F27", "T_F28", "T_F29",
  "T_F30", "T_F31", "T_LABELDEFN", "T_ADD", "T_ADDI", "T_AND", "T_ANDI",
  "T_AUIPC", "T_BEQ", "T_BGE", "T_BGEU", "T_BLT", "T_BLTU", "T_BNE",
  "T_FENCE", "T_FENCE_I", "T_JAL", "T_JALR", "T_LB", "T_LBU", "T_LH",
  "T_LHU", "T_LUI", "T_LW", "T_OR", "T_ORI", "T_SB", "T_SH", "T_SLL",
  "T_SLLI", "T_SLT", "T_SLTI", "T_SLTIU", "T_SLTU", "T_SRA", "T_SRAI",
  "T_SRL", "T_SRLI", "T_SUB", "T_SW", "T_XOR", "T_XORI", "T_LABEL",
  "T_STRING", "T_MAN", "'\\n'", "'{'", "','", "'}'", "'<'", "\"rnd\"",
  "'>'", "'-'", "'+'", "$accept", "input", "line", "asm_ctl", "expr",
  "sf_cmd", "dotalign", "dotdata", "dottext", "dotglobal", "dotfile",
  "dotorg", "dotlong", "dotcomm", "add_riscv_instr", "addi_riscv_instr",
  "and_riscv_instr", "andi_riscv_instr", "auipc_riscv_instr",
  "beq_riscv_instr", "bne_riscv_instr", "blt_riscv_instr",
  "bge_riscv_instr", "bltu_riscv_instr", "bgeu_riscv_instr",
  "fence_riscv_instr", "fence_i_riscv_instr", "jal_riscv_instr",
  "jalr_riscv_instr", "lb_riscv_instr", "lbu_riscv_instr",
  "lh_riscv_instr", "lhu_riscv_instr", "lui_riscv_instr", "lw_riscv_instr",
  "or_riscv_instr", "ori_riscv_instr", "sb_riscv_instr", "sh_riscv_instr",
  "sll_riscv_instr", "slli_riscv_instr", "slt_riscv_instr",
  "slti_riscv_instr", "sltiu_riscv_instr", "sltu_riscv_instr",
  "sra_riscv_instr", "srai_riscv_instr", "srl_riscv_instr",
  "srli_riscv_instr", "sub_riscv_instr", "sw_riscv_instr",
  "xor_riscv_instr", "xori_riscv_instr", "rnd", "rnd_const", "rnd_var",
  "uimm", "simm", "dimm", "rdimm", "dimmlist", "dimmlistbody", "disp",
  "optstring", "reg", YY_NULLPTR
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
     535,   536,   537,   538,    10,   123,    44,   125,    60,   539,
      62,    45,    43
};
# endif

#define YYPACT_NINF -665

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-665)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -665,  1937,  -665,  -258,  -258,  -258,  -279,  -279,  -258,  -279,
    -258,  -279,  -279,  -279,  -279,  -279,  -261,  -279,  -254,  -240,
    -237,  -279,  -238,  -279,  -236,  -235,  -279,  -234,  -233,  -232,
    -279,  -279,  -279,  -279,  -231,  -230,  -229,  -279,  -279,  -228,
    -225,  -279,  -258,  -226,  -279,  -223,  -222,  -221,  -220,  -219,
    -218,  -217,  -214,  -213,  -212,  -279,  -211,  -209,  -279,  -208,
    -279,  -279,  -279,  -279,  -279,  -279,  -216,  -279,  -279,  -279,
    -215,  -279,  -258,  -205,  -279,  -204,  -203,  -202,  -279,  -199,
    -198,  -197,  -196,  -195,  -194,  -193,  -279,  -279,  -190,  -191,
    -189,  -188,  -185,  -186,  -184,  -279,  -183,  -182,  -181,  -175,
    -174,  -279,  -215,  -279,  -665,  -279,  -279,  -258,  -258,  -258,
    -258,  -258,  -279,  -279,  -279,  -279,  -258,  -279,  -258,  -258,
    -258,  -173,  -172,  -258,  -258,  -279,  -279,  -279,  -279,  -258,
    -258,  -279,  -170,  -665,  -166,  -279,  -279,  -279,  -279,  -279,
    -163,  -159,  -272,  -160,  -157,  -279,  -279,  -279,  -155,  -665,
    -258,  -124,  -154,  -279,  -279,   231,   231,  -153,  -279,  -279,
    -665,  -665,  -273,  -150,  -273,  -273,    85,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,  -665,  -665,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,  -151,  -665,  -665,  -665,
    -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,
    -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,
    -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,
    -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,
    -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,
    -148,  -147,  -146,  -145,  -144,  -665,  -258,  -143,  -142,  -141,
    -140,  -139,  -258,  -138,  -258,  -135,  -665,  -279,  -665,  -665,
    -134,  -133,  -665,  -132,  -665,  -129,  -279,  -665,  -665,  -665,
    -130,  -128,  -127,  -279,  -665,  -665,  -665,  -126,  -114,  -665,
    -279,  -113,  -258,  -665,   -85,  -258,  -279,  -279,   -82,  -206,
    -258,   -83,  -665,  -665,  -665,  -279,  -665,  -665,  -279,  -665,
    -279,  -258,   -80,   -81,  -279,  -258,   -79,  -279,  -279,  -258,
    -665,  -271,   -78,   -77,   -76,   -75,  -279,  -665,  -665,  -279,
    -279,  -279,   -74,  -665,  -665,  -665,  -665,   -72,   -70,   -69,
    -665,  -665,  -665,   -66,  -665,  -665,   -64,  -665,  -665,  -665,
    -665,  -665,   -58,   -56,  -279,   -52,   -50,  -665,  -665,  -665,
    -665,   -48,   -45,  -665,   -46,   -44,  -665,   -42,   -40,   -38,
     -37,  -665,  -665,  -665,   -36,   -35,   -34,   -33,   -32,   -31,
    -258,   -30,  -665,  -665,  -215,  -279,   -29,   -28,  -279,   -27,
     -26,  -665,   -25,  -665,   -23,   -22,   -20,   -19,  -665,  -258,
    -279,  -279,  -279,  -279,  -665,   -18,  -279,  -665,  -665,  -665,
    -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,
    -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,
    -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -279,
    -279,  -665,  -665,  -665,  -665,  -665,   -15,   -14,  -665,  -665,
    -665,  -200,  -665,  -192,  -187,  -131,   -60,   -16,   -13,   -12,
     -11,   -10,    -9,    -7,    -6,    -5,    -4,    -2,    -1,     0,
       1,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    14,    15,    17,    18,    19,    20,    22,    23,    24,
    -665,  -665,  -665,  -665,  -665,  -665,    27,  -665,  -665,  -665,
    -665,  -665,    28,  -665,    29,  -665,  -279,  -665,  -665,  -665,
      33,    32,  -665,  -665,  -665,  -279,  -665,  -665,    35,  -665,
    -665,  -665,  -258,  -279,    38,  -258,    36,  -258,  -665,    39,
    -258,  -279,  -258,    40,  -665,    41,    82,  -665,    83,  -258,
      84,  -665,  -152,   -17,  -665,  -665,  -665,  -665,  -260,  -665,
    -665,  -665,  -665,  -279,    86,  -279,  -279,  -665,  -665,  -665,
    -665,  -665,  -665,    87,  -665,    90,  -665,  -665,  -665,    89,
    -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,
    -665,  -665,  -258,  -665,  -258,  -665,  -665,  -665,  -279,  -665,
    -665,  -665,  -665,  -665,  -665,  -665,  -258,    91,    92,    93,
    -258,  -665,  -279,  -279,  -279,  -665,  -665,    96,   231,   231,
     231,   231,  -256,   231,   231,   231,   231,   231,   231,  -256,
     231,   231,   231,   231,   231,  -256,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,  -665,  -665,  -665,    95,    97,
    -665,    98,  -665,  -258,  -279,  -258,  -258,  -258,    31,  -258,
    -665,    99,  -279,  -258,  -665,  -665,  -665,  -665,  -665,  -665,
     102,   103,  -260,  -256,  -665,  -279,  -279,  -665,   104,  -665,
    -258,  -258,   107,   106,  -665,  -665,  -665,   108,  -279,  -279,
    -279,  -665,    88,    94,   100,   105,  -665,   111,   112,  -665,
     109,   110,   113,   114,   115,   116,  -665,   117,   118,   119,
     153,   154,  -665,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,  -665,  -665,  -665,  -258,   176,  -258,  -258,  -665,
    -249,    36,  -258,  -665,  -279,  -258,  -258,  -258,  -255,  -256,
    -279,  -279,  -665,  -258,  -258,   177,  -665,  -665,  -279,   178,
     179,   231,  -256,   231,  -256,  -665,  -665,  -256,  -256,  -279,
    -256,  -279,  -256,  -256,  -256,  -279,  -256,  -279,  -256,   231,
    -256,  -256,  -256,   231,  -256,   231,  -256,  -279,   231,   231,
    -256,   231,  -256,   231,  -256,   231,  -256,   182,  -665,  -258,
    -258,  -258,  -665,  -207,  -258,  -279,  -279,  -258,  -258,  -279,
    -665,  -256,  -279,  -279,  -258,  -258,  -665,   181,  -665,  -665,
    -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,
    -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,
    -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,
    -665,  -665,  -665,  -665,  -258,  -258,  -258,  -665,  -665,  -258,
    -258,  -258,  -258,  -258,  -279,  -256,  -279,  -279,  -258,  -258,
    -665,  -258,   184,  -258,   183,  -258,  -258,  -258,  -258,   185,
    -279,   186,   187,  -258,  -258,  -258,  -258,  -258,  -665,  -258,
    -258,   188,  -241,  -665,  -279,  -665,  -665,  -258,  -258,  -258,
    -258,   189,  -279,  -279,  -665,  -665,  -279,  -258,  -258,   190,
    -258,  -665,  -258,  -279,   192,  -258,  -258,  -258,  -258,  -258,
     193,  -665,  -258,  -258,  -258,   196,  -258,  -665,  -258,  -258,
    -258,   195,   197,  -258,  -258,  -258,  -665,  -665,  -258,  -258,
     198,  -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,  -258,
    -665,  -258,  -258,  -258,  -665,  -258,  -258,  -215,  -279,  -258,
    -258,  -258,  -279,  -215,  -279,  -258,  -279,  -279,   199,  -665
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     288,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   288,     0,   118,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   191,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   117,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     222,   223,     0,     0,     0,     0,     4,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   240,   241,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   220,     3,     7,
       6,     8,     9,    10,    11,    14,    12,    13,    15,    18,
      19,    20,    21,    22,    23,    24,    29,    27,    25,    28,
      26,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,   276,
       0,     0,     0,     0,     0,   272,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    80,     0,   129,   130,
       0,     0,   219,     0,   171,     0,     0,   137,   138,   131,
       0,     0,     0,     0,   136,   150,   140,     0,     0,   172,
       0,     0,     0,   192,     0,     0,     0,     0,     0,     0,
       0,     0,   203,   204,   215,     0,   126,   127,     0,   207,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     289,     0,     0,     0,     0,     0,     0,   147,   148,     0,
       0,     0,     0,   145,   206,   205,   216,     0,     0,     0,
     194,   202,   121,     0,    58,   152,     0,   151,   103,   104,
     105,   139,     0,     0,     0,     0,     0,   111,   116,   112,
     113,     0,     0,   190,     0,     0,   110,     0,     0,     0,
       0,    99,   100,   109,     0,     0,     0,     0,     0,     0,
       0,     0,   201,   195,   288,     0,     0,     0,     0,     0,
       0,   178,     0,   174,     0,     0,     0,     0,   176,     0,
       0,     0,     0,     0,    85,     0,     0,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   307,   308,   309,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,     0,
       0,    62,   221,   226,   284,   285,     0,     0,   224,   225,
     227,     0,     5,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     193,   277,   278,   155,    70,    69,     0,    74,    71,    66,
      72,    67,     0,    76,     0,    78,     0,   108,   173,   199,
       0,     0,   132,   133,   134,     0,   154,   212,     0,   211,
     114,    95,     0,     0,     0,     0,     0,     0,   180,     0,
       0,     0,     0,     0,   164,     0,     0,   166,     0,     0,
       0,    63,     0,     0,   280,   268,   269,   279,     0,   157,
     156,   165,   141,     0,     0,     0,     0,   144,   217,   218,
     213,   122,   153,     0,   177,     0,   102,    79,   187,     0,
     124,   181,   101,    96,    97,    98,   182,   183,   184,   185,
     186,   188,     0,   200,     0,   119,   167,   168,     0,   197,
     198,   179,   175,   208,   209,   210,     0,     0,     0,     0,
       0,    86,     0,     0,     0,   287,   286,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    73,    75,    77,     0,     0,
     170,     0,   107,     0,     0,     0,     0,     0,     0,     0,
     125,     0,     0,     0,   106,   158,   160,   161,   115,    68,
       0,     0,     0,     0,   149,     0,     0,    88,     0,   214,
       0,     0,     0,     0,    81,    82,    83,     0,     0,     0,
       0,   228,     0,     0,     0,     0,   273,     0,     0,   233,
       0,     0,     0,     0,     0,     0,   242,     0,     0,     0,
       0,     0,   248,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   128,   123,   135,     0,     0,     0,     0,   282,
       0,     0,     0,   162,     0,     0,     0,     0,     0,     0,
       0,     0,   196,     0,     0,     0,    87,    84,     0,     0,
       0,     0,     0,     0,     0,   274,   275,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,   281,     0,     0,     0,     0,     0,     0,     0,
      64,     0,     0,     0,     0,     0,   169,     0,    60,    61,
     229,   230,   231,   232,   234,   237,   239,   236,   238,   235,
     243,   244,   245,   246,   247,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,     0,     0,     0,   283,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    89,     0,
       0,     0,     0,    65,     0,   142,   143,     0,     0,     0,
       0,     0,     0,     0,   271,   270,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   146,     0,     0,     0,     0,     0,   163,     0,     0,
       0,     0,     0,     0,     0,     0,    90,   159,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     189,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   120
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -665,  -665,  -665,  -665,   203,  -665,  -665,  -665,  -665,  -665,
    -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,
    -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,
    -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,
    -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,  -665,
    -665,  -665,  -665,  -665,  -665,  -665,   962,  -561,    -3,  -554,
    -664,  -665,  -149,   -96,    -8
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   554,   555,   556,   266,   709,   557,   558,
     668,   750,   458,   331,   449
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
     262,   263,   264,   265,   682,   268,   363,   270,   454,   455,
     265,   259,   401,   551,   552,   460,   461,   553,   456,   457,
     260,   261,   259,   276,   259,   552,   706,   819,   553,   820,
     278,   260,   261,   260,   261,   707,   708,   811,   812,   302,
     410,   411,   412,   413,   279,   280,   282,   285,   284,   915,
     287,   288,   289,   294,   295,   296,   299,   300,   303,   305,
     306,   307,   308,   309,   310,   311,   326,   330,   716,   333,
     312,   313,   314,   316,   722,   317,   319,   334,   336,   536,
     868,   337,   338,   340,   341,   342,   617,   813,   343,   344,
     345,   346,   349,   350,   618,   351,   352,   353,   354,   619,
     355,   357,   358,   359,   367,   368,   369,   370,   371,   360,
     361,   381,   382,   376,   392,   378,   379,   380,   393,   399,
     383,   384,   759,   400,   403,   404,   389,   390,   758,   408,
     414,   451,   459,   500,   501,   502,     0,   680,   503,   504,
     505,   507,   508,   509,   510,   511,   513,   409,   450,   515,
     517,   518,   519,   520,   522,   620,   523,   524,   526,   463,
     464,   465,   466,   467,   468,   469,   470,   471,   472,   473,
     527,   529,   474,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,   489,   490,   491,
     492,   493,   494,   495,   496,   497,   498,   499,   821,   531,
     535,   538,   543,   544,     0,   547,   559,   560,   561,   562,
     567,   831,   568,   833,   569,   570,   834,   835,   571,   837,
     572,   839,   840,   841,   573,   843,   621,   845,   574,   847,
     848,   849,   576,   851,   577,   853,   578,   579,   580,   857,
     581,   859,   582,   861,   583,   863,   584,   585,   586,   587,
     588,   589,   590,   591,   593,   596,   597,   599,   600,   601,
     875,   602,   603,   506,   604,   605,   611,   615,   616,   512,
     622,   514,   681,   623,   624,   625,   626,   627,     0,   628,
     629,   630,   631,     0,   632,   633,   634,   635,   636,   637,
     638,   639,   640,   641,   642,   643,   644,   645,   594,   530,
     646,   647,   532,   648,   649,   650,   651,   537,   652,   653,
     654,   655,   656,   657,   890,   659,   660,   751,   542,   662,
     665,   667,   546,   670,   674,   675,   550,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   676,   677,   679,   462,
     684,   687,   688,   689,   771,   694,   695,   696,   701,   742,
     772,   743,   744,   753,   756,   757,   773,   592,   762,   765,
     766,   774,   767,   775,   776,   777,   778,     0,     0,   779,
     780,   781,   782,   783,   784,   785,   606,   417,   418,   419,
     420,   421,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   786,
     787,   788,   789,   790,   791,   792,   793,   794,   795,   796,
     797,   798,   799,   800,   801,   802,   803,   804,   805,   806,
     808,   826,   828,   829,   864,   880,   896,   898,     0,   903,
     905,   906,   927,   921,     0,   914,   931,   937,   941,   946,
     953,   947,     0,   979,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   663,
       0,     0,   666,     0,   669,     0,     0,   671,     0,   673,
       0,     0,     0,     0,     0,     0,   678,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   690,
       0,   691,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   693,     0,     0,     0,   697,     0,     0,
     702,   703,   704,   705,     0,   710,   711,   712,   713,   714,
     715,     0,   717,   718,   719,   720,   721,     0,   723,   724,
     725,   726,   727,   728,   729,   730,   731,   732,   733,   734,
     735,   736,   737,   738,   739,   740,   741,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     745,     0,   747,   748,   749,     0,   752,     0,     0,     0,
     755,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   763,   764,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   807,     0,   809,   810,     0,     0,     0,   814,
       0,     0,   816,   817,   818,     0,     0,     0,     0,     0,
     824,   825,     0,   830,     0,   832,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   846,     0,     0,     0,   850,     0,   852,     0,     0,
     855,   856,     0,   858,     0,   860,     0,   862,     0,     0,
       0,     0,     0,     0,     0,     0,   865,   866,   867,     0,
       0,   869,     0,     0,   872,   873,     0,     0,     0,     0,
       0,   878,   879,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   881,   882,   883,     0,     0,   884,   885,   886,   887,
     888,   968,     0,     0,     0,   893,   894,   974,   895,     0,
     897,     0,   899,   900,   901,   902,     0,     0,     0,     0,
     907,   908,   909,   910,   911,     0,   912,   913,     0,     0,
       0,     0,     0,     0,   917,   918,   919,   920,     0,     0,
       0,     0,     0,     0,   925,   926,     0,   928,     0,   929,
       0,     0,   932,   933,   934,   935,   936,     0,     0,   938,
     939,   940,     0,   942,     0,   943,   944,   945,     0,     0,
     948,   949,   950,     0,     0,   951,   952,     0,   954,   955,
     956,   957,   958,   959,   960,   961,   962,     0,   963,   964,
     965,     0,   966,   967,     0,     0,   970,   971,   972,   267,
       0,   269,   976,   271,   272,   273,   274,   275,     0,   277,
       0,     0,     0,   281,     0,   283,     0,     0,   286,     0,
       0,     0,   290,   291,   292,   293,     0,     0,     0,   297,
     298,     0,     0,   301,     0,     0,   304,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   315,     0,     0,
     318,     0,   320,   321,   322,   323,   324,   325,     0,   327,
     328,   329,     0,   332,     0,     0,   335,     0,     0,     0,
     339,     0,     0,     0,     0,     0,     0,     0,   347,   348,
       0,     0,     0,     0,     0,     0,     0,   356,     0,     0,
       0,     0,     0,   362,     0,   364,     0,   365,   366,     0,
       0,     0,     0,     0,   372,   373,   374,   375,     0,   377,
       0,     0,     0,     0,     0,     0,     0,   385,   386,   387,
     388,     0,     0,   391,     0,     0,     0,   394,   395,   396,
     397,   398,     0,     0,   402,     0,     0,   405,   406,   407,
       0,     0,     0,     0,     0,   415,   416,     0,     0,     0,
     452,   453,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   516,
       0,     0,     0,     0,     0,     0,     0,     0,   521,     0,
       0,     0,     0,     0,     0,   525,     0,     0,     0,     0,
       0,     0,   528,     0,     0,     0,     0,     0,   533,   534,
       0,     0,     0,     0,     0,     0,     0,   539,     0,     0,
     540,     0,   541,     0,     0,     0,   545,     0,     0,   548,
     549,     0,     0,     0,     0,     0,     0,     0,   563,     0,
       0,   564,   565,   566,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   575,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   595,     0,     0,
     598,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   607,   608,   609,   610,     0,     0,   612,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   613,   614,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   658,     0,
       0,     0,     0,     0,     0,     0,     0,   661,     0,     0,
       0,     0,     0,     0,     0,   664,     0,     0,     0,     0,
       0,     0,     0,   672,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   683,     0,   685,   686,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     692,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   698,   699,   700,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   746,     0,     0,     0,
       0,     0,     0,     0,   754,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   760,   761,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     768,   769,   770,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   815,     0,     0,     0,
       0,     0,   822,   823,     0,     0,     0,     0,     0,     0,
     827,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   836,     0,   838,     0,     0,     0,   842,     0,   844,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   854,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   870,   871,     0,
       0,   874,     0,     0,   876,   877,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   889,     0,   891,   892,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   904,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   916,     0,     0,     0,
       0,     0,     0,     0,   922,   923,     0,     0,   924,     0,
       0,     0,     0,     0,     0,   930,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     969,     0,     0,     0,   973,     0,   975,     2,   977,   978,
       3,     0,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,     0,    29,    30,
      31,    32,    33,    34,     0,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,     0,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,     0,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,     0,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,     0,     0,     0,     0,   158,   159,   160,   161,   162,
     163,   164,   165,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,     0,     0,
     206,   207
};

static const yytype_int16 yycheck[] =
{
       3,     4,     5,   282,   558,     8,   102,    10,   281,   282,
     282,   282,   284,   284,   285,   164,   165,   288,   291,   292,
     291,   292,   282,   284,   282,   285,   282,   282,   288,   284,
     284,   291,   292,   291,   292,   291,   292,   286,   287,    42,
     164,   165,   166,   167,   284,   282,   284,   282,   284,   290,
     284,   284,   284,   284,   284,   284,   284,   282,   284,   282,
     282,   282,   282,   282,   282,   282,   282,   282,   629,    72,
     284,   284,   284,   284,   635,   284,   284,   282,   282,   285,
     287,   284,   284,   282,   282,   282,   286,   751,   284,   284,
     284,   284,   282,   284,   286,   284,   284,   282,   284,   286,
     284,   284,   284,   284,   107,   108,   109,   110,   111,   284,
     284,   284,   284,   116,   284,   118,   119,   120,   284,   282,
     123,   124,   683,   282,   284,   282,   129,   130,   682,   284,
     284,   284,   282,   284,   282,   282,    -1,   289,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   150,   156,   284,
     284,   284,   284,   282,   284,   286,   284,   284,   284,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     284,   284,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   759,   284,
     282,   284,   282,   284,    -1,   284,   284,   284,   284,   284,
     284,   772,   284,   774,   284,   284,   777,   778,   284,   780,
     284,   782,   783,   784,   282,   786,   286,   788,   284,   790,
     791,   792,   284,   794,   284,   796,   284,   282,   284,   800,
     284,   802,   284,   804,   284,   806,   284,   284,   284,   284,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     821,   284,   284,   266,   284,   284,   284,   282,   282,   272,
     286,   274,   289,   286,   286,   286,   286,   286,    -1,   286,
     286,   286,   286,    -1,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   394,   302,
     286,   286,   305,   286,   286,   286,   286,   310,   286,   286,
     286,   284,   284,   284,   875,   282,   284,   286,   321,   284,
     282,   285,   325,   284,   284,   284,   329,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   284,   284,   284,   166,
     284,   284,   282,   284,   286,   284,   284,   284,   282,   284,
     286,   284,   284,   284,   282,   282,   286,   390,   284,   282,
     284,   286,   284,   282,   282,   286,   286,    -1,    -1,   286,
     286,   286,   286,   286,   286,   286,   409,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     284,   284,   284,   284,   282,   284,   282,   284,    -1,   284,
     284,   284,   282,   284,    -1,   287,   284,   284,   282,   284,
     282,   284,    -1,   284,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   532,
      -1,    -1,   535,    -1,   537,    -1,    -1,   540,    -1,   542,
      -1,    -1,    -1,    -1,    -1,    -1,   549,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   592,
      -1,   594,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   606,    -1,    -1,    -1,   610,    -1,    -1,
     618,   619,   620,   621,    -1,   623,   624,   625,   626,   627,
     628,    -1,   630,   631,   632,   633,   634,    -1,   636,   637,
     638,   639,   640,   641,   642,   643,   644,   645,   646,   647,
     648,   649,   650,   651,   652,   653,   654,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     663,    -1,   665,   666,   667,    -1,   669,    -1,    -1,    -1,
     673,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   690,   691,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   745,    -1,   747,   748,    -1,    -1,    -1,   752,
      -1,    -1,   755,   756,   757,    -1,    -1,    -1,    -1,    -1,
     763,   764,    -1,   771,    -1,   773,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   789,    -1,    -1,    -1,   793,    -1,   795,    -1,    -1,
     798,   799,    -1,   801,    -1,   803,    -1,   805,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   809,   810,   811,    -1,
      -1,   814,    -1,    -1,   817,   818,    -1,    -1,    -1,    -1,
      -1,   824,   825,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   864,   865,   866,    -1,    -1,   869,   870,   871,   872,
     873,   967,    -1,    -1,    -1,   878,   879,   973,   881,    -1,
     883,    -1,   885,   886,   887,   888,    -1,    -1,    -1,    -1,
     893,   894,   895,   896,   897,    -1,   899,   900,    -1,    -1,
      -1,    -1,    -1,    -1,   907,   908,   909,   910,    -1,    -1,
      -1,    -1,    -1,    -1,   917,   918,    -1,   920,    -1,   922,
      -1,    -1,   925,   926,   927,   928,   929,    -1,    -1,   932,
     933,   934,    -1,   936,    -1,   938,   939,   940,    -1,    -1,
     943,   944,   945,    -1,    -1,   948,   949,    -1,   951,   952,
     953,   954,   955,   956,   957,   958,   959,    -1,   961,   962,
     963,    -1,   965,   966,    -1,    -1,   969,   970,   971,     7,
      -1,     9,   975,    11,    12,    13,    14,    15,    -1,    17,
      -1,    -1,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,
      -1,    -1,    30,    31,    32,    33,    -1,    -1,    -1,    37,
      38,    -1,    -1,    41,    -1,    -1,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,
      58,    -1,    60,    61,    62,    63,    64,    65,    -1,    67,
      68,    69,    -1,    71,    -1,    -1,    74,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    -1,
      -1,    -1,    -1,   101,    -1,   103,    -1,   105,   106,    -1,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,    -1,    -1,   131,    -1,    -1,    -1,   135,   136,   137,
     138,   139,    -1,    -1,   142,    -1,    -1,   145,   146,   147,
      -1,    -1,    -1,    -1,    -1,   153,   154,    -1,    -1,    -1,
     158,   159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   277,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   286,    -1,
      -1,    -1,    -1,    -1,    -1,   293,    -1,    -1,    -1,    -1,
      -1,    -1,   300,    -1,    -1,    -1,    -1,    -1,   306,   307,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   315,    -1,    -1,
     318,    -1,   320,    -1,    -1,    -1,   324,    -1,    -1,   327,
     328,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   336,    -1,
      -1,   339,   340,   341,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   364,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   395,    -1,    -1,
     398,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   410,   411,   412,   413,    -1,    -1,   416,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   449,   450,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   516,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   525,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   533,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   541,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   563,    -1,   565,   566,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     598,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   612,   613,   614,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   664,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   672,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   685,   686,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     698,   699,   700,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   754,    -1,    -1,    -1,
      -1,    -1,   760,   761,    -1,    -1,    -1,    -1,    -1,    -1,
     768,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   779,    -1,   781,    -1,    -1,    -1,   785,    -1,   787,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   797,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   815,   816,    -1,
      -1,   819,    -1,    -1,   822,   823,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   874,    -1,   876,   877,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   890,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   904,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   912,   913,    -1,    -1,   916,    -1,
      -1,    -1,    -1,    -1,    -1,   923,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     968,    -1,    -1,    -1,   972,    -1,   974,     0,   976,   977,
       3,    -1,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    -1,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,    -1,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,    -1,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,    -1,    -1,    -1,   168,   169,   170,   171,   172,
     173,   174,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   252,
     253,   254,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,    -1,    -1,
     283,   284
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   294,     0,     3,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    31,
      32,    33,    34,    35,    36,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   168,   169,
     170,   171,   172,   173,   174,   175,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   283,   284,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   282,
     291,   292,   351,   351,   351,   282,   349,   349,   351,   349,
     351,   349,   349,   349,   349,   349,   284,   349,   284,   284,
     282,   349,   284,   349,   284,   282,   349,   284,   284,   284,
     349,   349,   349,   349,   284,   284,   284,   349,   349,   284,
     282,   349,   351,   284,   349,   282,   282,   282,   282,   282,
     282,   282,   284,   284,   284,   349,   284,   284,   349,   284,
     349,   349,   349,   349,   349,   349,   282,   349,   349,   349,
     282,   356,   349,   351,   282,   349,   282,   284,   284,   349,
     282,   282,   282,   284,   284,   284,   284,   349,   349,   282,
     284,   284,   284,   282,   284,   284,   349,   284,   284,   284,
     284,   284,   349,   356,   349,   349,   349,   351,   351,   351,
     351,   351,   349,   349,   349,   349,   351,   349,   351,   351,
     351,   284,   284,   351,   351,   349,   349,   349,   349,   351,
     351,   349,   284,   284,   349,   349,   349,   349,   349,   282,
     282,   284,   349,   284,   282,   349,   349,   349,   284,   351,
     164,   165,   166,   167,   284,   349,   349,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   357,
     357,   284,   349,   349,   281,   282,   291,   292,   355,   282,
     355,   355,   297,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     284,   282,   282,   284,   284,   284,   351,   284,   284,   284,
     284,   284,   351,   284,   351,   284,   349,   284,   284,   284,
     282,   349,   284,   284,   284,   349,   284,   284,   349,   284,
     351,   284,   351,   349,   349,   282,   285,   351,   284,   349,
     349,   349,   351,   282,   284,   349,   351,   284,   349,   349,
     351,   284,   285,   288,   346,   347,   348,   351,   352,   284,
     284,   284,   284,   349,   349,   349,   349,   284,   284,   284,
     284,   284,   284,   282,   284,   349,   284,   284,   284,   282,
     284,   284,   284,   284,   284,   284,   284,   284,   284,   284,
     284,   284,   351,   284,   356,   349,   284,   284,   349,   284,
     284,   284,   284,   284,   284,   284,   351,   349,   349,   349,
     349,   284,   349,   349,   349,   282,   282,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   284,   284,   284,   349,   282,
     284,   349,   284,   351,   349,   282,   351,   285,   353,   351,
     284,   351,   349,   351,   284,   284,   284,   284,   351,   284,
     289,   289,   352,   349,   284,   349,   349,   284,   282,   284,
     351,   351,   349,   351,   284,   284,   284,   351,   349,   349,
     349,   282,   357,   357,   357,   357,   282,   291,   292,   350,
     357,   357,   357,   357,   357,   357,   350,   357,   357,   357,
     357,   357,   350,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   284,   284,   284,   351,   349,   351,   351,   351,
     354,   286,   351,   284,   349,   351,   282,   282,   352,   350,
     349,   349,   284,   351,   351,   282,   284,   284,   349,   349,
     349,   286,   286,   286,   286,   282,   282,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   286,   286,   286,
     286,   286,   286,   286,   286,   286,   286,   351,   284,   351,
     351,   286,   287,   353,   351,   349,   351,   351,   351,   282,
     284,   350,   349,   349,   351,   351,   284,   349,   284,   284,
     357,   350,   357,   350,   350,   350,   349,   350,   349,   350,
     350,   350,   349,   350,   349,   350,   357,   350,   350,   350,
     357,   350,   357,   350,   349,   357,   357,   350,   357,   350,
     357,   350,   357,   350,   282,   351,   351,   351,   287,   351,
     349,   349,   351,   351,   349,   350,   349,   349,   351,   351,
     284,   351,   351,   351,   351,   351,   351,   351,   351,   349,
     350,   349,   349,   351,   351,   351,   282,   351,   284,   351,
     351,   351,   351,   284,   349,   284,   284,   351,   351,   351,
     351,   351,   351,   351,   287,   290,   349,   351,   351,   351,
     351,   284,   349,   349,   349,   351,   351,   282,   351,   351,
     349,   284,   351,   351,   351,   351,   351,   284,   351,   351,
     351,   282,   351,   351,   351,   351,   284,   284,   351,   351,
     351,   351,   351,   282,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   356,   349,
     351,   351,   351,   349,   356,   349,   351,   349,   349,   284
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   293,   294,   294,   295,   295,   295,   295,   295,   296,
     296,   296,   296,   296,   296,   296,   296,   296,   296,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   297,   297,   297,   297,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,   309,   310,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   346,
     347,   348,   349,   350,   350,   350,   351,   351,   351,   352,
     352,   353,   354,   354,   355,   355,   355,   355,   356,   356,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     7,
       6,     6,     2,     3,     6,     9,     3,     3,     4,     3,
       3,     3,     3,     4,     3,     4,     3,     4,     3,     3,
       2,     4,     4,     4,     5,     2,     3,     5,     4,     9,
      15,    11,     7,     6,    21,     3,     3,     3,     3,     2,
       2,     3,     3,     2,     2,     2,     4,     4,     3,     2,
       2,     2,     2,     2,     3,     4,     2,     1,     1,     3,
      35,     2,     3,     5,     3,     4,     2,     2,     5,     2,
       2,     2,     3,     3,     3,     5,     2,     2,     2,     2,
       2,     3,     9,     9,     3,     2,    12,     2,     2,     4,
       2,     2,     2,     3,     3,     3,     3,     3,     4,    15,
       4,     4,     5,    13,     3,     3,     3,     3,     3,     6,
       4,     2,     2,     3,     2,     3,     2,     3,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,    19,
       2,     1,     2,     2,     2,     2,     5,     3,     3,     3,
       3,     2,     2,     2,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     4,     2,     2,     3,     3,     2,
       1,     2,     1,     1,     2,     2,     2,     2,     4,     6,
       6,     6,     6,     4,     6,     6,     6,     6,     6,     6,
       1,     1,     4,     6,     6,     6,     6,     6,     4,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     6,     6,
       6,     6,     6,     6,     6,     6,     6,     6,     1,     1,
       8,     8,     1,     1,     2,     2,     1,     2,     2,     1,
       1,     3,     1,     3,     1,     1,     2,     2,     0,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1
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
        case 58:
#line 510 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				mexit(yyengine, "Exiting as per user's request.", 0);
			}
		}
#line 2485 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 59:
#line 526 "sf-riscv.y" /* yacc.c:1652  */
    {	/*
			 *	Mark given memory as origin of taint
			 */
			if (!yyengine->scanning)
			{
				m_taintmem(yyengine,yyengine->cp, (yyvsp[-5].uval), (yyvsp[-4].uval), (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].uval));

			}
		}
#line 2499 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 60:
#line 539 "sf-riscv.y" /* yacc.c:1652  */
    {
			/*
			 *	Mark given register as an origin of taint
			 */
			if (!yyengine->scanning)
			{
				m_taintreg(yyengine,yyengine->cp, (yyvsp[-4].uval), (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 2513 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 61:
#line 552 "sf-riscv.y" /* yacc.c:1652  */
    {
			/*
			 *	Mark given register as an origin of taint
			 */
			if (!yyengine->scanning)
			{
				m_taintreg(yyengine,yyengine->cp, (yyvsp[-4].uval), (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 2527 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 62:
#line 565 "sf-riscv.y" /* yacc.c:1652  */
    {
			/*
			 *	Give taint of each RISC-V command
			 */
			if (!yyengine->scanning)
			{
				m_riscvdumptaintdistr(yyengine,yyengine->cp);
			}
		}
#line 2541 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 63:
#line 575 "sf-riscv.y" /* yacc.c:1652  */
    {
			/*
			 *	Only give the architecture type:
			 */
			if (!yyengine->scanning)
			{
				m_newnode(yyengine, (yyvsp[-1].str), 0, 0, 0, NULL, 0, 0);
			}
		}
#line 2555 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 64:
#line 585 "sf-riscv.y" /* yacc.c:1652  */
    {
			/*
			 *	Give the architecture type and the (fixed) x/y/z location
			 */
			if (!yyengine->scanning)
			{
				m_newnode(yyengine, (yyvsp[-4].str), (yyvsp[-3].rval)->dval, (yyvsp[-3].rval)->dval, (yyvsp[-3].rval)->dval, NULL, 0, 0);
			}
		}
#line 2569 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 65:
#line 595 "sf-riscv.y" /* yacc.c:1652  */
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
#line 2601 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 66:
#line 623 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				batt_nodeattach(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 2612 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 67:
#line 630 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				batt_printstats(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 2623 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 68:
#line 637 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				batt_newbatt(yyengine, (yyvsp[-2].uval), (yyvsp[-1].dval));
			}
		}
#line 2634 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 69:
#line 644 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				if (SF_DEBUG) mprint(yyengine, NULL, siminfo, 
						"Setting yyengine->batts[%d].ileak to %f\n",
						yyengine->curbatt, (yyvsp[-1].dval));
				yyengine->batts[yyengine->curbatt].ileak = (yyvsp[-1].dval);
			}
		}
#line 2648 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 70:
#line 654 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				if (SF_DEBUG) mprint(yyengine, NULL, siminfo, 
						"Setting yyengine->batts[%d].Cf to %f\n",
						yyengine->curbatt, (yyvsp[-1].dval));
				yyengine->batts[yyengine->curbatt].Cf = (yyvsp[-1].dval);
			}
		}
#line 2662 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 71:
#line 664 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				if (SF_DEBUG) mprint(yyengine, NULL, siminfo, 
					"Setting yyengine->batts[%d].Inominal to %f\n",
					yyengine->curbatt, (yyvsp[-1].dval));
				yyengine->batts[yyengine->curbatt].Inominal = (yyvsp[-1].dval);
			}
		}
#line 2676 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 72:
#line 674 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				if (SF_DEBUG) mprint(yyengine, NULL, siminfo, 
						"Setting yyengine->batts[%d].Rf to %f\n",
						yyengine->curbatt, (yyvsp[-1].dval));
				yyengine->batts[yyengine->curbatt].Rf = (yyvsp[-1].dval);
			}
		}
#line 2690 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 73:
#line 684 "sf-riscv.y" /* yacc.c:1652  */
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
#line 2708 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 74:
#line 698 "sf-riscv.y" /* yacc.c:1652  */
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
#line 2730 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 75:
#line 716 "sf-riscv.y" /* yacc.c:1652  */
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
#line 2748 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 76:
#line 730 "sf-riscv.y" /* yacc.c:1652  */
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
#line 2770 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 77:
#line 748 "sf-riscv.y" /* yacc.c:1652  */
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
#line 2788 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 78:
#line 762 "sf-riscv.y" /* yacc.c:1652  */
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
#line 2810 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 79:
#line 780 "sf-riscv.y" /* yacc.c:1652  */
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
#line 2828 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 80:
#line 794 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_pcbacktrace(yyengine, yyengine->cp);
			}
		}
#line 2839 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 81:
#line 801 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_setbptglobaltime(yyengine, (yyvsp[-1].uval));
			}
		}
#line 2850 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 82:
#line 808 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_setbptcycles(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 2861 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 83:
#line 815 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_setbptinstrs(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 2872 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 84:
#line 822 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_setbptsensorreading(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-1].dval));
			}
		}
#line 2883 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 85:
#line 829 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_bptls(yyengine);
			}
		}
#line 2894 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 86:
#line 836 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_bptdel(yyengine, (yyvsp[-1].uval));
			}
		}
#line 2905 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 87:
#line 843 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_setloc(yyengine, yyengine->cp, (yyvsp[-3].dval), (yyvsp[-2].dval), (yyvsp[-1].dval));
			}
		}
#line 2916 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 88:
#line 850 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				network_setretryalg(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-1].str));
			}
		}
#line 2927 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 89:
#line 857 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_randprint(yyengine, (yyvsp[-7].str), (yyvsp[-6].dval), (yyvsp[-5].dval), (yyvsp[-4].dval), (yyvsp[-3].dval), (yyvsp[-2].dval), (yyvsp[-1].dval));
			}
		}
#line 2938 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 90:
#line 864 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				//m_registerrvar(yyengine->cp, $2, $3, $4, $5, $6, $7,
				//		$8, $9, $10, $11, $12, $13, $14);
			}
		}
#line 2950 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 91:
#line 872 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_initrandtable(yyengine, (yyvsp[-9].str), (yyvsp[-8].str), (yyvsp[-7].dval), (yyvsp[-6].dval), (yyvsp[-5].dval), (yyvsp[-4].dval), (yyvsp[-3].dval), (yyvsp[-2].dval), (yyvsp[-1].dval));
			}
		}
#line 2961 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 92:
#line 879 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				//m_defndist($2, $4, $6);

			}
		}
#line 2973 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 93:
#line 887 "sf-riscv.y" /* yacc.c:1652  */
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
#line 2995 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 94:
#line 905 "sf-riscv.y" /* yacc.c:1652  */
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
#line 3017 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 95:
#line 923 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->ignoredeaths = (yyvsp[-1].uval);
			}
		}
#line 3028 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 96:
#line 930 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->voltscale_alpha = (yyvsp[-1].dval);
			}
		}
#line 3039 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 97:
#line 937 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->voltscale_K = (yyvsp[-1].dval);
			}
		}
#line 3050 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 98:
#line 944 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->voltscale_Vt = (yyvsp[-1].dval);
			}
		}
#line 3061 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 99:
#line 951 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->schedtype = SchedRandom;
			}
		}
#line 3072 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 100:
#line 958 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->schedtype = SchedRoundRobin;
			}
		}
#line 3083 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 101:
#line 965 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->quantum = (yyvsp[-1].uval);
			}
		}
#line 3094 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 102:
#line 972 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->baseid = (yyvsp[-1].uval);
			}
		}
#line 3105 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 103:
#line 979 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_renumbernodes(yyengine);
			}
		}
#line 3116 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 104:
#line 986 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->trip_ustart = musercputimeusecs();
				yyengine->cp->trip_startclk = yyengine->cp->ICLK;
			}
		}
#line 3128 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 105:
#line 994 "sf-riscv.y" /* yacc.c:1652  */
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
#line 3145 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 106:
#line 1007 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				network_netseg2file(yyengine, (yyvsp[-2].uval), (yyvsp[-1].str));
			}
		}
#line 3156 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 107:
#line 1014 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				network_file2netseg(yyengine, (yyvsp[-2].str), (yyvsp[-1].uval));
			}
		}
#line 3167 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 108:
#line 1021 "sf-riscv.y" /* yacc.c:1652  */
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
#line 3183 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 109:
#line 1033 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->settimerintrdelay(yyengine, yyengine->cp, (yyvsp[0].dval));
			}
		}
#line 3194 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 110:
#line 1040 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->phyperiodpsec = (yyvsp[0].dval);
			}
		}
#line 3205 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 111:
#line 1047 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->battperiodpsec = (yyvsp[0].dval);
			}
		}
#line 3216 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 112:
#line 1054 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->netperiodpsec = (yyvsp[0].dval);
			}
		}
#line 3227 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 113:
#line 1061 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->fperiodpsec = (yyvsp[0].dval);
			}
		}
#line 3238 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 114:
#line 1069 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->force_avgpwr = (yyvsp[-1].dval);
				yyengine->cp->force_sleeppwr = (yyvsp[0].dval);
			}
		}
#line 3250 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 115:
#line 1077 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				network_netsegpropmodel(yyengine, (yyvsp[-2].uval), (yyvsp[-1].uval), (yyvsp[0].dval));
			}
		}
#line 3261 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 116:
#line 1084 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->dumpperiodpsec = (yyvsp[0].dval);
			}
		}
#line 3272 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 117:
#line 1091 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_version(yyengine);
			}
		}
#line 3283 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 118:
#line 1098 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				physics_sensorsdbg(yyengine);
			}
		}
#line 3294 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 119:
#line 1106 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				physics_sigsubscr(yyengine, yyengine->cp, (yyvsp[-1].uval), (yyvsp[0].uval));
			}
		}
#line 3305 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 120:
#line 1117 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				physics_newsigsrc(yyengine, (yyvsp[-33].uval), (yyvsp[-32].str), (yyvsp[-31].dval), (yyvsp[-30].dval), (yyvsp[-29].dval), (yyvsp[-28].dval), (yyvsp[-27].dval), (yyvsp[-26].dval), (yyvsp[-25].dval),
					(yyvsp[-24].dval), (yyvsp[-23].dval), (yyvsp[-22].dval), (yyvsp[-21].dval), (yyvsp[-20].dval), (yyvsp[-19].dval), (yyvsp[-18].dval), (yyvsp[-17].dval), (yyvsp[-16].dval), (yyvsp[-15].dval),
					(yyvsp[-14].dval), (yyvsp[-13].dval), (yyvsp[-12].dval), (yyvsp[-11].str), (yyvsp[-10].uval), (yyvsp[-9].dval), (yyvsp[-8].dval), (yyvsp[-7].dval), (yyvsp[-6].uval), (yyvsp[-5].str),
					(yyvsp[-4].uval), (yyvsp[-3].dval), (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3319 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 121:
#line 1127 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				mprint(yyengine, NULL, siminfo,
					"Current directory: %s\n", mgetpwd());
			}
		}
#line 3331 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 122:
#line 1135 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_parseobjdump(yyengine, yyengine->cp, (yyvsp[-1].str));
			}
		}
#line 3342 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 123:
#line 1142 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_dumpall(yyengine, (yyvsp[-3].str), M_OWRITE, (yyvsp[-2].str), (yyvsp[-1].str));
			}
		}
#line 3353 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 124:
#line 1149 "sf-riscv.y" /* yacc.c:1652  */
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
#line 3371 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 125:
#line 1163 "sf-riscv.y" /* yacc.c:1652  */
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
#line 3391 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 126:
#line 1179 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->step = yyengine->cp->cyclestep;
			}
		}
#line 3402 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 127:
#line 1186 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->step = yyengine->cp->faststep;
			}
		}
#line 3413 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 128:
#line 1193 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->cache_init(yyengine, yyengine->cp, (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3424 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 129:
#line 1200 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->cache_deactivate(yyengine, yyengine->cp);
			}
		}
#line 3435 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 130:
#line 1207 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->cache_printstats(yyengine, yyengine->cp);
			}
		}
#line 3446 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 131:
#line 1214 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->dumpregs(yyengine, yyengine->cp);
			}
		}
#line 3457 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 132:
#line 1221 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->dumphist(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 3468 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 133:
#line 1228 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->dumphistpretty(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 3479 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 134:
#line 1235 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->ldhistrandom(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 3490 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 135:
#line 1242 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->addhist(yyengine, yyengine->cp, (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3501 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 136:
#line 1249 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->dumpsysregs(yyengine, yyengine->cp);
			}
		}
#line 3512 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 137:
#line 1256 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->dumppipe(yyengine, yyengine->cp);
			}
		}
#line 3523 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 138:
#line 1263 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->dumpdistribution(yyengine, yyengine->cp);
			}
		}
#line 3534 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 139:
#line 1270 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->resetcpu(yyengine, yyengine->cp);
			}
		}
#line 3545 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 140:
#line 1277 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				mprint(yyengine, yyengine->cp, nodeinfo,
					"Dynamic instruction count = [" UVLONGFMT "], fetched instructions = [" UVLONGFMT "]\n",
					yyengine->cp->dyncnt, yyengine->cp->nfetched);	
			}
		}
#line 3558 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 141:
#line 1286 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->nodetach = (yyvsp[-1].uval);
			}
		}
#line 3569 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 142:
#line 1293 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_addvaluetrace(yyengine, yyengine->cp, (yyvsp[-7].str), (yyvsp[-6].uval), (yyvsp[-5].uval), (yyvsp[-4].uval), (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3580 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 143:
#line 1300 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_delvaluetrace(yyengine, yyengine->cp, (yyvsp[-7].str), (yyvsp[-6].uval), (yyvsp[-5].uval), (yyvsp[-4].uval), (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3591 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 144:
#line 1307 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_readstabs(yyengine, yyengine->cp, (yyvsp[-1].str));
			}
		}
#line 3602 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 145:
#line 1314 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_valuestats(yyengine, yyengine->cp);
			}
		}
#line 3613 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 146:
#line 1321 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_numaregion(yyengine, (yyvsp[-10].str), (yyvsp[-9].uval), (yyvsp[-8].uval), (yyvsp[-7].sval), (yyvsp[-6].sval), (yyvsp[-5].sval), (yyvsp[-4].sval), (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3624 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 147:
#line 1328 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_numastats(yyengine, yyengine->cp);
			}
		}
#line 3635 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 148:
#line 1335 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_numastatsall(yyengine);
			}
		}
#line 3646 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 149:
#line 1342 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_numasetmapid(yyengine, (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3657 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 150:
#line 1349 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->dumptlb(yyengine, yyengine->cp);
			}
		}
#line 3668 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 151:
#line 1356 "sf-riscv.y" /* yacc.c:1652  */
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
#line 3688 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 152:
#line 1372 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				mprint(yyengine, NULL, siminfo,
					"Simulation random seed = %ld\n", yyengine->randseed);
			}
		}
#line 3700 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 153:
#line 1380 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				mprint(yyengine, NULL, siminfo,
					"Resetting random number generation with seed [%ld]\n", (yyvsp[-1].uval));
				yyengine->randseed = mrandominit(yyengine, (yyvsp[-1].uval));
			}
		}
#line 3713 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 154:
#line 1389 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->ENABLE_BATT_LOW_INTR = (yyvsp[-1].uval);
			}
		}
#line 3724 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 155:
#line 1396 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->battery_alert_frac = (yyvsp[-1].dval);
			}
		}
#line 3735 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 156:
#line 1403 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->fail_prob = (yyvsp[-1].dval);
			}
		}
#line 3746 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 157:
#line 1410 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->failure_duration_max = (yyvsp[-1].uval);
			}
		}
#line 3757 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 158:
#line 1417 "sf-riscv.y" /* yacc.c:1652  */
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
#line 3775 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 159:
#line 1432 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				network_netnewseg(yyengine, (yyvsp[-13].uval), (yyvsp[-12].uval), (yyvsp[-11].uval), (yyvsp[-10].uval), (yyvsp[-9].uval), (yyvsp[-8].uval),
					(yyvsp[-7].dval), (yyvsp[-6].dval), (yyvsp[-5].dval), (yyvsp[-4].uval), (yyvsp[-3].dval), (yyvsp[-2].dval), (yyvsp[-1].dval));
			}
		}
#line 3787 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 160:
#line 1440 "sf-riscv.y" /* yacc.c:1652  */
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
#line 3805 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 161:
#line 1454 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				network_netsegnicattach(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3816 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 162:
#line 1461 "sf-riscv.y" /* yacc.c:1652  */
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
#line 3838 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 163:
#line 1479 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				network_netnodenewifc(yyengine, yyengine->cp, (yyvsp[-11].uval), (yyvsp[-10].dval), (yyvsp[-9].dval), (yyvsp[-8].dval),
					(yyvsp[-7].dval), (yyvsp[-6].uval), (yyvsp[-5].dval), (yyvsp[-4].dval), (yyvsp[-3].dval), (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3850 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 164:
#line 1487 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				network_netsegdelete(yyengine, (yyvsp[-1].uval));
			}
		}
#line 3861 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 165:
#line 1494 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				merror(yyengine, "Command \"NODEFAILPROBFN\" unimplemented.");
			}
		}
#line 3872 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 166:
#line 1501 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				merror(yyengine, "Command \"NETSEGFAILPROBFN\" unimplemented.");
			}
		}
#line 3883 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 167:
#line 1508 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_sizemem(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 3894 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 168:
#line 1515 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				pau_init(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 3905 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 169:
#line 1522 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->split(yyengine, yyengine->cp, (yyvsp[-4].uval), (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].str));
			}
		}
#line 3916 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 170:
#line 1529 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_dumpmem(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-1].uval));
			}
		}
#line 3927 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 171:
#line 1536 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->pipelined = 0;
			}
		}
#line 3938 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 172:
#line 1543 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->pipelined = 1;
			}
		}
#line 3949 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 173:
#line 1550 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->superH->ENABLE_CLK_INTR = (yyvsp[-1].uval);
			}
		}
#line 3960 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 174:
#line 1557 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->runnable = 0;
			}
		}
#line 3971 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 175:
#line 1564 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				sfatal(yyengine, yyengine->cp, (yyvsp[-1].str));
			}
		}
#line 3982 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 176:
#line 1571 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->verbose ^= 1;
			}
		}
#line 3993 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 177:
#line 1578 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_run(yyengine, yyengine->cp, (yyvsp[-1].str));
			}

			/*	The 'string' is dynamically allocated, in lex.c		*/
			free((yyvsp[-1].str));
		}
#line 4007 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 178:
#line 1588 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->step(yyengine, yyengine->cp, 0);
			}
		}
#line 4018 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 179:
#line 1595 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				go(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 4029 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 180:
#line 1602 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				loadcmds(yyengine, (yyvsp[-1].str));
			}
		}
#line 4040 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 181:
#line 1609 "sf-riscv.y" /* yacc.c:1652  */
    {	
			if (!yyengine->scanning)
			{
				yyengine->cp->PC = (yyvsp[-1].uval);
			}
		}
#line 4051 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 182:
#line 1616 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				/*	Scale frequency accordingly for provided Vdd	*/
				power_scaledelay(yyengine, yyengine->cp, (yyvsp[-1].dval));
			}
		}
#line 4063 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 183:
#line 1624 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->mem_r_latency = (yyvsp[-1].uval);
			}
		}
#line 4074 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 184:
#line 1631 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->mem_w_latency = (yyvsp[-1].uval);
			}
		}
#line 4085 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 185:
#line 1638 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->flash_r_latency = (yyvsp[-1].uval);
			}
		}
#line 4096 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 186:
#line 1645 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->flash_w_latency = (yyvsp[-1].uval);
			}
		}
#line 4107 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 187:
#line 1652 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				/*	Scale Vdd accordingly for provided frequency	*/
				power_scalevdd(yyengine, yyengine->cp, (yyvsp[-1].dval));
			}
		}
#line 4119 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 188:
#line 1660 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				/*	Set node mass	*/
				massSetNodeMass(yyengine, yyengine->cp, (yyvsp[-1].dval));
			}
		}
#line 4131 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 189:
#line 1668 "sf-riscv.y" /* yacc.c:1652  */
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
#line 4146 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 190:
#line 1679 "sf-riscv.y" /* yacc.c:1652  */
    {
			yyengine->cp->MEMBASE = (yyvsp[0].uval);
			yyengine->cp->TAINTMEMBASE = (yyvsp[0].uval);
		}
#line 4155 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 191:
#line 1684 "sf-riscv.y" /* yacc.c:1652  */
    {
			mprint(yyengine, NULL, siminfo, "Memory base address is %d.",yyengine->cp->MEMBASE);
		}
#line 4163 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 192:
#line 1688 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				help(yyengine);
			}
		}
#line 4174 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 193:
#line 1695 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				man(yyengine, (yyvsp[-1].str));
			}
		}
#line 4185 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 194:
#line 1702 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->flushpipe(yyengine->cp);
			}
		}
#line 4196 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 195:
#line 1709 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->pipeshow = !yyengine->cp->pipeshow;
			}
		}
#line 4207 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 196:
#line 1716 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				savemem(yyengine, yyengine->cp, (yyvsp[-3].uval), (yyvsp[-2].uval), (yyvsp[-1].str));
			}
		}
#line 4218 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 197:
#line 1723 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				load_srec(yyengine, yyengine->cp, (yyvsp[-1].str));
			}
		}
#line 4229 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 198:
#line 1730 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				load_mapfile(yyengine, yyengine->cp, (yyvsp[-1].str));
			}
		}
#line 4240 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 199:
#line 1737 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				cont(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 4251 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 200:
#line 1744 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_sharebus(yyengine, yyengine->cp, (yyvsp[-1].uval));
			}
		}
#line 4262 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 201:
#line 1751 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				mprint(yyengine, NULL, siminfo,
					"CLK = " UVLONGFMT ", ICLK = " UVLONGFMT ", TIME = %E, CYCLETIME = %E\n",
					yyengine->cp->CLK, yyengine->cp->ICLK, yyengine->cp->TIME, 
					yyengine->cp->CYCLETIME);
			}
		}
#line 4276 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 202:
#line 1761 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				power_printstats(yyengine, yyengine->cp);
			}
		}
#line 4287 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 203:
#line 1768 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_locstats(yyengine, yyengine->cp);
			}
		}
#line 4298 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 204:
#line 1775 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_listrvars(yyengine);
			}
		}
#line 4309 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 205:
#line 1783 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_on(yyengine, yyengine->cp);
			}
		}
#line 4320 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 206:
#line 1790 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				m_off(yyengine, yyengine->cp);
			}
		}
#line 4331 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 207:
#line 1797 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				network_netdebug(yyengine, yyengine->cp);
			}
		}
#line 4342 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 208:
#line 1804 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				mprint(yyengine, NULL, siminfo, 
					"WARNING: Trace only works if compiled w/ -MYRMIGKI_DEBUG\n\n");
				yyengine->cp->trace = (yyvsp[-1].uval);
			}
		}
#line 4355 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 209:
#line 1813 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->throttlensec = (yyvsp[-1].uval);
			}
		}
#line 4366 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 210:
#line 1820 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->throttlewin = (yyvsp[-1].uval);
			}
		}
#line 4377 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 211:
#line 1827 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->faultthreshold = (yyvsp[-1].uval);
			}
		}
#line 4388 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 212:
#line 1834 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				yyengine->cp->ENABLE_TOO_MANY_FAULTS = (yyvsp[-1].uval);
			}
		}
#line 4399 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 213:
#line 1841 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				fault_setnodepfun(yyengine, yyengine->cp, (yyvsp[-1].str));
			}
		}
#line 4410 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 214:
#line 1848 "sf-riscv.y" /* yacc.c:1652  */
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
#line 4429 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 215:
#line 1863 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				mmblocksdisplay(yyengine);
			}
		}
#line 4440 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 216:
#line 1870 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				pau_printstats(yyengine, yyengine->cp);
			}
		}
#line 4451 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 217:
#line 1877 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				// TODO: we should account for the cost of the m_on and m_off

				m_off(yyengine, yyengine->cp);
				mnsleep((yyvsp[-1].uval));
				m_on(yyengine, yyengine->cp);
			}
		}
#line 4466 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 218:
#line 1888 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				// TODO: we should account for the cost of the m_on and m_off

				m_off(yyengine, yyengine->cp);
				mnsleep((yyvsp[-1].uval) * 1000000000);
				m_on(yyengine, yyengine->cp);
			}
		}
#line 4481 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 219:
#line 1899 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				/*	For now, nothing fun is done with comments	*/
			}
		}
#line 4492 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 220:
#line 1906 "sf-riscv.y" /* yacc.c:1652  */
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
#line 4507 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 221:
#line 1920 "sf-riscv.y" /* yacc.c:1652  */
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
#line 4535 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 222:
#line 1947 "sf-riscv.y" /* yacc.c:1652  */
    {
		}
#line 4542 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 223:
#line 1953 "sf-riscv.y" /* yacc.c:1652  */
    {
		}
#line 4549 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 224:
#line 1959 "sf-riscv.y" /* yacc.c:1652  */
    {
		}
#line 4556 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 225:
#line 1965 "sf-riscv.y" /* yacc.c:1652  */
    {
		}
#line 4563 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 226:
#line 1970 "sf-riscv.y" /* yacc.c:1652  */
    {
		}
#line 4570 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 227:
#line 1975 "sf-riscv.y" /* yacc.c:1652  */
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
#line 4598 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 228:
#line 2000 "sf-riscv.y" /* yacc.c:1652  */
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
#line 4623 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 229:
#line 2023 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_add(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval), (yyvsp[-4].uval));
				}

				uint32_t tmp = (B00110011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0000 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].uval) << 20)		/*	rs2,	bit 20-24		*/
						| (B00000000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
#line 4666 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 230:
#line 2064 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_addi(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-4].uval), (yyvsp[0].sval));
				}

				uint32_t tmp = (B00010011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0000 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].sval) << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
#line 4708 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 231:
#line 2104 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_and(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval), (yyvsp[-4].uval));
				}

				uint32_t tmp = (B00110011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0111 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].uval) << 20)		/*	rs2,	bit 20-24		*/
						| (B00000000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
#line 4751 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 232:
#line 2145 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_andi(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-4].uval), (yyvsp[0].sval));
				}

				uint32_t tmp = (B00010011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0111 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].sval) << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
#line 4793 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 233:
#line 2185 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_auipc(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].sval));
				}
				uint32_t tmp = B00010111	/*	opcode,	bit 0-6			*/
					| ((yyvsp[-2].uval) << 7)		/*	rd,	bit 7-11		*/
					| ((yyvsp[0].sval) << 12)		/*	imm,	bit 12-31		*/
					;
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
				yyengine->cp->PC += 4;
			}
		}
#line 4831 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 234:
#line 2221 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				uint8_t imm1 = ((yyvsp[0].sval) >> 0) & B1111;
				uint8_t imm5 = ((yyvsp[0].sval) >> 4) & B111111;	/*	Shift the fragements	*/
				uint8_t imm11 = ((yyvsp[0].sval) >> 11) & 1;		/*	so that they are each	*/
				uint8_t imm12 = ((yyvsp[0].sval) >> 12) & 1;		/*	an individual number	*/

				if (!yyengine->cp->pipelined)	
				{
					riscv_beq(yyengine, yyengine->cp, (yyvsp[-4].uval), (yyvsp[-2].uval), imm1, imm5, imm11, imm12);
				}

				uint32_t tmp = (B01100011 << 0)		/*	opcode,	bit 0-6			*/
						| (imm11 << 7)		/*	imm11,	bit 7			*/
						| (imm1 << 8)		/*	imm1-4,	bit 8-11		*/
						| (B0000 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-4].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[-2].uval) << 20)		/*	rs2,	bit 20-24		*/
						| (imm5 << 25)		/*	imm5-10,bit 25-30		*/
						| (imm12 << 31)		/*	imm12,	bit 31			*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
#line 4881 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 235:
#line 2269 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				uint8_t imm1 = ((yyvsp[0].sval) >> 0) & B1111;
				uint8_t imm5 = ((yyvsp[0].sval) >> 4) & B111111;	/*	Shift the fragements	*/
				uint8_t imm11 = ((yyvsp[0].sval) >> 11) & 1;		/*	so that they are each	*/
				uint8_t imm12 = ((yyvsp[0].sval) >> 12) & 1;		/*	an individual number	*/
				if (!yyengine->cp->pipelined)	
				{
					riscv_bne(yyengine, yyengine->cp, (yyvsp[-4].uval), (yyvsp[-2].uval), imm1, imm5, imm11, imm12);
				}
				uint32_t tmp = (B01100011 << 0)		/*	opcode,	bit 0-6			*/
						| (imm11 << 7)		/*	imm11,	bit 7			*/
						| (imm1 << 8)		/*	imm1-4,	bit 8-11		*/
						| (B0001 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-4].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[-2].uval) << 20)		/*	rs2,	bit 20-24		*/
						| (imm5 << 25)		/*	imm5-10,bit 25-30		*/
						| (imm12 << 31)		/*	imm12,	bit 31			*/
						;
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
				yyengine->cp->PC += 4;
			}
		}
#line 4928 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 236:
#line 2314 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{			
				uint8_t imm1 = ((yyvsp[0].sval) >> 0) & B1111;
				uint8_t imm5 = ((yyvsp[0].sval) >> 4) & B111111;	/*	Shift the fragements	*/
				uint8_t imm11 = ((yyvsp[0].sval) >> 11) & 1;		/*	so that they are each	*/
				uint8_t imm12 = ((yyvsp[0].sval) >> 12) & 1;		/*	an individual number	*/
				if (!yyengine->cp->pipelined)	
				{
					riscv_blt(yyengine, yyengine->cp, (yyvsp[-4].uval), (yyvsp[-2].uval), imm1, imm5, imm11, imm12);
				}
				uint32_t tmp = (B01100011 << 0)		/*	opcode,	bit 0-6			*/
						| (imm11 << 7)		/*	imm11,	bit 7			*/
						| (imm1 << 8)		/*	imm1-4,	bit 8-11		*/
						| (B0100 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-4].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[-2].uval) << 20)		/*	rs2,	bit 20-24		*/
						| (imm5 << 25)		/*	imm5-10,bit 25-30		*/
						| (imm12 << 31)		/*	imm12,	bit 31			*/
						;
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
				yyengine->cp->PC += 4;
			}
		}
#line 4975 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 237:
#line 2359 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				uint8_t imm1 = ((yyvsp[0].sval) >> 0) & B1111;
				uint8_t imm5 = ((yyvsp[0].sval) >> 4) & B111111;	/*	Shift the fragements	*/
				uint8_t imm11 = ((yyvsp[0].sval) >> 11) & 1;		/*	so that they are each	*/
				uint8_t imm12 = ((yyvsp[0].sval) >> 12) & 1;		/*	an individual number	*/
				if (!yyengine->cp->pipelined)	
				{
					riscv_bge(yyengine, yyengine->cp, (yyvsp[-4].uval), (yyvsp[-2].uval), imm1, imm5, imm11, imm12);
				}
				uint32_t tmp = (B01100011 << 0)		/*	opcode,	bit 0-6			*/
						| (imm11 << 7)		/*	imm11,	bit 7			*/
						| (imm1 << 8)		/*	imm1-4,	bit 8-11		*/
						| (B0101 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-4].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[-2].uval) << 20)		/*	rs2,	bit 20-24		*/
						| (imm5 << 25)		/*	imm5-10,bit 25-30		*/
						| (imm12 << 31)		/*	imm12,	bit 31			*/
						;
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
				yyengine->cp->PC += 4;
			}
		}
#line 5022 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 238:
#line 2404 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				uint8_t imm1 = ((yyvsp[0].uval) >> 0) & B1111;
				uint8_t imm5 = ((yyvsp[0].uval) >> 4) & B111111;	/*	Shift the fragements	*/
				uint8_t imm11 = ((yyvsp[0].uval) >> 11) & 1;		/*	so that they are each	*/
				uint8_t imm12 = ((yyvsp[0].uval) >> 12) & 1;		/*	an individual number	*/
				if (!yyengine->cp->pipelined)	
				{
					riscv_bltu(yyengine, yyengine->cp, (yyvsp[-4].uval), (yyvsp[-2].uval), imm1, imm5, imm11, imm12);
				}
				uint32_t tmp = (B01100011 << 0)		/*	opcode,	bit 0-6			*/
						| (imm11 << 7)		/*	imm11,	bit 7			*/
						| (imm1 << 8)		/*	imm1-4,	bit 8-11		*/
						| (B0110 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-4].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[-2].uval) << 20)		/*	rs2,	bit 20-24		*/
						| (imm5 << 25)		/*	imm5-10,bit 25-30		*/
						| (imm12 << 31)		/*	imm12,	bit 31			*/
						;
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
				yyengine->cp->PC += 4;
			}
		}
#line 5069 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 239:
#line 2449 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				uint8_t imm1 = ((yyvsp[0].uval) >> 0) & B1111;
				uint8_t imm5 = ((yyvsp[0].uval) >> 4) & B111111;	/*	Shift the fragements	*/
				uint8_t imm11 = ((yyvsp[0].uval) >> 11) & 1;		/*	so that they are each	*/
				uint8_t imm12 = ((yyvsp[0].uval) >> 12) & 1;		/*	an individual number	*/
				if (!yyengine->cp->pipelined)	
				{
					riscv_bgeu(yyengine, yyengine->cp, (yyvsp[-4].uval), (yyvsp[-2].uval), imm1, imm5, imm11, imm12);
				}
				uint32_t tmp = (B01100011 << 0)		/*	opcode,	bit 0-6			*/
						| (imm11 << 7)		/*	imm11,	bit 7			*/
						| (imm1 << 8)		/*	imm1-4,	bit 8-11		*/
						| (B0111 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-4].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[-2].uval) << 20)		/*	rs2,	bit 20-24		*/
						| (imm5 << 25)		/*	imm5-10,bit 25-30		*/
						| (imm12 << 31)		/*	imm12,	bit 31			*/
						;
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
				yyengine->cp->PC += 4;
			}
		}
#line 5116 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 240:
#line 2494 "sf-riscv.y" /* yacc.c:1652  */
    {
			mprint(yyengine, NULL, siminfo, 
					"Not implemented.");
		}
#line 5125 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 241:
#line 2501 "sf-riscv.y" /* yacc.c:1652  */
    {
			mprint(yyengine, NULL, siminfo, 
					"Not implemented.");
		}
#line 5134 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 242:
#line 2508 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo,
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				uint8_t imm1 = ((yyvsp[0].sval) >> 0) & B0000001111111111;
				uint8_t imm11 = ((yyvsp[0].sval) >> 10) & 1;			/*	Shift the fragements	*/
				uint8_t imm12 = ((yyvsp[0].sval) >> 11) & B11111111;		/*	so that they are each	*/
				uint8_t imm20 = ((yyvsp[0].sval) >> 19) & 1;			/*	an individual number	*/
				if (!yyengine->cp->pipelined)	
				{
					riscv_jal(yyengine, yyengine->cp, (yyvsp[-2].uval), imm1, imm11, imm12, imm20);
				}
				uint32_t tmp = (B01101111 << 0)	/*	opcode,		bit 0-6			*/
						| ((yyvsp[-2].uval) << 7)	/*	rd,		bit 7-11		*/
						| (imm12 << 12)	/*	imm12-19,	bit 12-19		*/
						| (imm11 << 20)	/*	imm11,		bit 20			*/
						| (imm1 << 25)	/*	imm1-10,	bit 21-30		*/
						| (imm20 << 31)	/*	imm12,		bit 31			*/
						;
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
				yyengine->cp->PC += 4;
			}
		}
#line 5179 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 243:
#line 2551 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo,
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_jalr(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-4].uval), (yyvsp[0].sval));
				}

				uint32_t tmp = (B01100111 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0000 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].sval) << 20)		/*	imm,	bit 20-31		*/
						;
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
				yyengine->cp->PC += 4;
			}
		}
#line 5220 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 244:
#line 2590 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_lb(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-4].uval), (yyvsp[0].sval));
				}

				uint32_t tmp = (B00000011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0000 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].sval) << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
#line 5262 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 245:
#line 2630 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_lbu(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-4].uval), (yyvsp[0].uval));
				}

				uint32_t tmp = (B00000011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0100 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].uval) << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
#line 5304 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 246:
#line 2670 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_lh(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-4].uval), (yyvsp[0].sval));
				}

				uint32_t tmp = (B00000011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0001 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].sval) << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
#line 5346 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 247:
#line 2710 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_lhu(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-4].uval), (yyvsp[0].uval));
				}

				uint32_t tmp = (B00000011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0101 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].uval) << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
#line 5388 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 248:
#line 2750 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_lui(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].sval));
				}
				uint32_t tmp = B00110111	/*	opcode,	bit 0-6			*/
					| ((yyvsp[-2].uval) << 7)		/*	rd,	bit 7-11		*/
					| ((yyvsp[0].sval) << 12)		/*	imm,	bit 12-31		*/
					;
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
				yyengine->cp->PC += 4;
			}
		}
#line 5426 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 249:
#line 2786 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_lw(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-4].uval), (yyvsp[0].sval));
				}

				uint32_t tmp = (B00000011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0010 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].sval) << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
#line 5468 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 250:
#line 2826 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_or(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval), (yyvsp[-4].uval));
				}

				uint32_t tmp = (B00110011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0110 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].uval) << 20)		/*	rs2,	bit 20-24		*/
						| (B00000000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
#line 5511 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 251:
#line 2867 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_ori(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-4].uval), (yyvsp[0].sval));
				}

				uint32_t tmp = (B00010011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0110 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].sval) << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
#line 5553 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 252:
#line 2907 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				uint8_t imm0 = ((yyvsp[0].sval) >> 0) & B00011111;	/*	Shift the fragements	*/
				uint8_t imm5 = ((yyvsp[0].sval) >> 4) & B01111111;	/*	so that they are each	*/
									/*	an individual number	*/

				if (!yyengine->cp->pipelined)	
				{
					riscv_sb(yyengine, yyengine->cp, (yyvsp[-4].uval), (yyvsp[-2].uval), imm0, imm5);
				}

				uint32_t tmp = (B00100011 << 0)		/*	opcode,	bit 0-6			*/
						| (imm0 << 7)		/*	imm11,	bit 7-11		*/
						| (B0000 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-4].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[-2].uval) << 20)		/*	rs2,	bit 20-24		*/
						| (imm5 << 25)		/*	imm5-10,bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
#line 5600 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 253:
#line 2952 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				uint8_t imm0 = ((yyvsp[0].sval) >> 0) & B00011111;	/*	Shift the fragements	*/
				uint8_t imm5 = ((yyvsp[0].sval) >> 4) & B01111111;	/*	so that they are each	*/
									/*	an individual number	*/

				if (!yyengine->cp->pipelined)	
				{
					riscv_sh(yyengine, yyengine->cp, (yyvsp[-4].uval), (yyvsp[-2].uval), imm0, imm5);
				}

				uint32_t tmp = (B00100011 << 0)		/*	opcode,	bit 0-6			*/
						| (imm0 << 7)		/*	imm11,	bit 7-11		*/
						| (B0001 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-4].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[-2].uval) << 20)		/*	rs2,	bit 20-24		*/
						| (imm5 << 25)		/*	imm5-10,bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
#line 5647 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 254:
#line 2997 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_sll(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval), (yyvsp[-4].uval));
				}

				uint32_t tmp = (B00110011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0001 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].uval) << 20)		/*	rs2,	bit 20-24		*/
						| (B00000000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			 }
		}
#line 5690 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 255:
#line 3038 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_slli(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-4].uval), (yyvsp[0].sval));
				}

				uint32_t tmp = (B00010011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0001 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].sval) << 20)		/*	rs2,	bit 20-24		*/
						| (B00000000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			 }
		}
#line 5733 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 256:
#line 3079 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_slt(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval), (yyvsp[-4].uval));
				}

				uint32_t tmp = (B00110011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0010 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].uval) << 20)		/*	rs2,	bit 20-24		*/
						| (B00000000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			 }
		}
#line 5776 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 257:
#line 3120 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_slti(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-4].uval), (yyvsp[0].sval));
				}

				uint32_t tmp = (B00010011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0010 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].sval) << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
#line 5818 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 258:
#line 3160 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_sltiu(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-4].uval), (yyvsp[0].uval));
				}

				uint32_t tmp = (B00010011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0011 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].uval) << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
#line 5860 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 259:
#line 3200 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_sltu(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval), (yyvsp[-4].uval));
				}

				uint32_t tmp = (B00110011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0011 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].uval) << 20)		/*	rs2,	bit 20-24		*/
						| (B00000000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			 }
		}
#line 5903 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 260:
#line 3241 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_sra(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval), (yyvsp[-4].uval));
				}

				uint32_t tmp = (B00110011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0101 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].uval) << 20)		/*	rs2,	bit 20-24		*/
						| (B00100000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			 }
		}
#line 5946 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 261:
#line 3282 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_srai(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-4].uval), (yyvsp[0].sval));
				}

				uint32_t tmp = (B00010011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0101 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].sval) << 20)		/*	rs2,	bit 20-24		*/
						| (B00100000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			 }
		}
#line 5989 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 262:
#line 3323 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_srl(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval), (yyvsp[-4].uval));
				}

				uint32_t tmp = (B00110011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0101 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].uval) << 20)		/*	rs2,	bit 20-24		*/
						| (B00000000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			 }
		}
#line 6032 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 263:
#line 3364 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_srli(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-4].uval), (yyvsp[0].sval));
				}

				uint32_t tmp = (B00010011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0101 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].sval) << 20)		/*	rs2,	bit 20-24		*/
						| (B00000000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			 }
		}
#line 6075 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 264:
#line 3405 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_sub(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval), (yyvsp[-4].uval));
				}

				uint32_t tmp = (B00110011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0000 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].uval) << 20)		/*	rs2,	bit 20-24		*/
						| (B00100000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			 }
		}
#line 6118 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 265:
#line 3446 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				uint8_t imm0 = ((yyvsp[0].sval) >> 0) & B00011111;	/*	Shift the fragements	*/
				uint8_t imm5 = ((yyvsp[0].sval) >> 4) & B01111111;	/*	so that they are each	*/
									/*	an individual number	*/

				if (!yyengine->cp->pipelined)	
				{
					riscv_sw(yyengine, yyengine->cp, (yyvsp[-4].uval), (yyvsp[-2].uval), imm0, imm5);
				}

				uint32_t tmp = (B00100011 << 0)		/*	opcode,	bit 0-6			*/
						| (imm0 << 7)		/*	imm11,	bit 7-11		*/
						| (B0010 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-4].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[-2].uval) << 20)		/*	rs2,	bit 20-24		*/
						| (imm5 << 25)		/*	imm5-10,bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
#line 6165 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 266:
#line 3491 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_xor(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[0].uval), (yyvsp[-4].uval));
				}

				uint32_t tmp = (B00110011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0100 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].uval) << 20)		/*	rs2,	bit 20-24		*/
						| (B00000000 << 25)	/*	funct7,	bit 25-31		*/
						;

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
				yyengine->cp->PC += 4;
			 }
		}
#line 6208 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 267:
#line 3532 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (yyengine->scanning)
			{
				yyengine->cp->PC += 4;
			}
			else if (yyengine->cp->machinetype != MACHINE_RISCV)
			{
				mprint(yyengine, NULL, siminfo, 
					"Inline assembler is for RV32I(FD) nodes only. Check node type.");
			}
			else
			{
				if (!yyengine->cp->pipelined)	
				{
					riscv_xori(yyengine, yyengine->cp, (yyvsp[-2].uval), (yyvsp[-4].uval), (yyvsp[0].sval));
				}

				uint32_t tmp = (B00010011 << 0)		/*	opcode,	bit 0-6			*/
						| ((yyvsp[-4].uval) << 7)		/*	rd,	bit 7-11		*/
						| (B0100 << 12)		/*	funct3,	bit 12-14		*/
						| ((yyvsp[-2].uval) << 15)		/*	rs1,	bit 15-19		*/
						| ((yyvsp[0].sval) << 20)		/*	imm,	bit 20-31		*/
						;

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
				yyengine->cp->PC += 4;
			}
		}
#line 6250 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 270:
#line 3589 "sf-riscv.y" /* yacc.c:1652  */
    {
			if (!yyengine->scanning)
			{
				//$$->value = m_randgen(yyengine->cp, $3, $4, $5, $6, $7);
			}
		}
#line 6261 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 271:
#line 3598 "sf-riscv.y" /* yacc.c:1652  */
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
#line 6278 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 272:
#line 4193 "sf-riscv.y" /* yacc.c:1652  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.uval) = strtoul((yyvsp[0].str), &ep, 0);
			
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid unsigned immediate data %s.", (yyvsp[0].str));
			}
		}
#line 6294 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 273:
#line 4207 "sf-riscv.y" /* yacc.c:1652  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.sval) = strtol((yyvsp[0].str), &ep, 0);
			
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid signed immediate data %s.", (yyvsp[0].str));
			}
		}
#line 6310 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 274:
#line 4219 "sf-riscv.y" /* yacc.c:1652  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.sval) = -(strtol((yyvsp[0].str), &ep, 0));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid signed immediate data %s.", (yyvsp[0].str));
			}
		}
#line 6326 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 275:
#line 4231 "sf-riscv.y" /* yacc.c:1652  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.sval) = +(strtol((yyvsp[0].str), &ep, 0));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid signed immediate data %s.", (yyvsp[0].str));
			}
		}
#line 6342 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 276:
#line 4245 "sf-riscv.y" /* yacc.c:1652  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.dval) = strtod((yyvsp[0].str), &ep);
			
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid double immediate data %s.", (yyvsp[0].str));
			}
		}
#line 6358 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 277:
#line 4257 "sf-riscv.y" /* yacc.c:1652  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.dval) = -(strtod((yyvsp[0].str), &ep));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid double immediate data %s.", (yyvsp[0].str));
			}
		}
#line 6374 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 278:
#line 4269 "sf-riscv.y" /* yacc.c:1652  */
    {
			char tmp;
			char *ep = &tmp;

			(yyval.dval) = +(strtod((yyvsp[0].str), &ep));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid double immediate data %s.", (yyvsp[0].str));
			}
		}
#line 6390 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 279:
#line 4283 "sf-riscv.y" /* yacc.c:1652  */
    {
			(yyval.rval) = (Rval *) mcalloc(yyengine, 1, sizeof(Rval), "sf.y:rdimm/$$");
			if ((yyval.rval) == NULL)
			{
				sfatal(yyengine, yyengine->cp, "Mcalloc failed");
			}

			(yyval.rval)->dval = (yyvsp[0].dval);
		}
#line 6404 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 281:
#line 4331 "sf-riscv.y" /* yacc.c:1652  */
    {
			(yyval.dlist) = (yyvsp[-1].dlist);
		}
#line 6412 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 282:
#line 4337 "sf-riscv.y" /* yacc.c:1652  */
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
#line 6437 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 283:
#line 4358 "sf-riscv.y" /* yacc.c:1652  */
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
#line 6456 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 284:
#line 4376 "sf-riscv.y" /* yacc.c:1652  */
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
#line 6481 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 285:
#line 4397 "sf-riscv.y" /* yacc.c:1652  */
    {
			char tmp;
			char *ep = &tmp;


			(yyval.sval) = strtol((yyvsp[0].str), &ep, 0);
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid DISP ([%s]). Possibly due to a .comm.", (yyvsp[0].str));
			}
		}
#line 6498 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 286:
#line 4410 "sf-riscv.y" /* yacc.c:1652  */
    {
			char tmp;
			char *ep = &tmp;


			(yyval.sval) = +(strtol((yyvsp[0].str), &ep, 0));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid DISP ([%s]). Possibly due to a .comm.", (yyvsp[0].str));
			}
		}
#line 6515 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 287:
#line 4423 "sf-riscv.y" /* yacc.c:1652  */
    {
			char tmp;
			char *ep = &tmp;


			(yyval.sval) = -(strtol((yyvsp[0].str), &ep, 0));
				
			if (*ep != '\0')
			{
				merror(yyengine, "Invalid DISP ([%s]). Possibly due to a .comm.", (yyvsp[0].str));
			}
		}
#line 6532 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 288:
#line 4438 "sf-riscv.y" /* yacc.c:1652  */
    {
			/*								*/
			/*	To make handling of empty string in T_RUN rule 		*/
			/*	uniform, we malloc a place holder.			*/
			/*								*/
			char *tmp = mmalloc(yyengine, sizeof(char), "opstring rule in shasm.y");
			tmp[0] = '\0';
			(yyval.str) = tmp;
		}
#line 6546 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 289:
#line 4448 "sf-riscv.y" /* yacc.c:1652  */
    {
			(yyval.str) = (yyvsp[0].str);
		}
#line 6554 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 290:
#line 4454 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 0;}
#line 6560 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 291:
#line 4455 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 1;}
#line 6566 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 292:
#line 4456 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 2;}
#line 6572 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 293:
#line 4457 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 3;}
#line 6578 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 294:
#line 4458 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 4;}
#line 6584 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 295:
#line 4459 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 5;}
#line 6590 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 296:
#line 4460 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 6;}
#line 6596 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 297:
#line 4461 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 7;}
#line 6602 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 298:
#line 4462 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 8;}
#line 6608 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 299:
#line 4463 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 9;}
#line 6614 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 300:
#line 4464 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 10;}
#line 6620 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 301:
#line 4465 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 11;}
#line 6626 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 302:
#line 4466 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 12;}
#line 6632 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 303:
#line 4467 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 13;}
#line 6638 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 304:
#line 4468 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 14;}
#line 6644 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 305:
#line 4469 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 15;}
#line 6650 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 306:
#line 4470 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 16;}
#line 6656 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 307:
#line 4471 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 17;}
#line 6662 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 308:
#line 4472 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 18;}
#line 6668 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 309:
#line 4473 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 19;}
#line 6674 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 310:
#line 4474 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 20;}
#line 6680 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 311:
#line 4475 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 21;}
#line 6686 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 312:
#line 4476 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 22;}
#line 6692 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 313:
#line 4477 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 23;}
#line 6698 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 314:
#line 4478 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 24;}
#line 6704 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 315:
#line 4479 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 25;}
#line 6710 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 316:
#line 4480 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 26;}
#line 6716 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 317:
#line 4481 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 27;}
#line 6722 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 318:
#line 4482 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 28;}
#line 6728 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 319:
#line 4483 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 29;}
#line 6734 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 320:
#line 4484 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 30;}
#line 6740 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;

  case 321:
#line 4485 "sf-riscv.y" /* yacc.c:1652  */
    {(yyval.uval) = 31;}
#line 6746 "sf-riscv.tab.c" /* yacc.c:1652  */
    break;


#line 6750 "sf-riscv.tab.c" /* yacc.c:1652  */
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
#line 4521 "sf-riscv.y" /* yacc.c:1918  */


#include "lex-riscv.c"

int
yyerror(char *err)
{
	merror(yyengine, "Invalid command! (for riscv)");
	clearistream(yyengine);

	return 0;
}
