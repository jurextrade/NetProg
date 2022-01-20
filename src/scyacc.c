#define _CRT_SECURE_NO_WARNINGS
# line 22 "scyacc.y"
# define SCINTEGER 257
# define SCLONG 258
# define SCFLOAT 259
# define SCBOOLEAN 260
# define SCSTRING 261
# define SCVARIABLE 262
# define SCOPERATOR 263
# define SCOPERATORDEFUN 264
# define SCOPERATORREMOTE 265
# define SCCHAR 266
# define SCTIME 267
# define SCDATE 268
# define SCNULL 269
# define PAR_DEB 270
# define PAR_FIN 271
# define ERR_CHAR 272
# define ERR_INT 273
# define ERR_LONG 274
# define ERR_FLOAT 275
# define ERR_TIME 276
# define ERR_DATE 277
# define ERR_SYNTAX 278
# define ERR_ALLOC 279
# define ERR_NOTFOUND 280

# line 56 "scyacc.y"
    #include "sclex.h"

typedef struct _EntityAction
{
    SCEntity* pentity;
    BYTE actionremote;
}EntityAction;

EntityAction TabEntityAction[NB_ACTIONS_OVERLAPED];

int niveau;
BYTE InStringRemote;
char StringRemote[YYLMAX];
int  InRemoteList;
BYTE InModeDefun;      /* pour la gestion de variables */
List* SCReturnParseSection;

SCSysAction* psysaction_global;
SCSection*   psection_global;
SCSection*   psection_main;
SCEntity*    pentity_global;
SCAction*    paction_global;
SCVariable*  pvariable_global;
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
typedef int yytabelem;
# define YYERRCODE 256

# line 747 "scyacc.y"


void SCAddEntityToList (SCEntity* pentity)
{
    SCSection* psection;
    SCAction*  paction;

    if ((niveau - 1) < 0)
    {
        psection = psection_global;
        ListNewr(&psection->ListEntities, pentity);
        psection->NbEntity++;
    }
    else
    {
        paction = (SCAction *)((SCEntity *)(TabEntityAction[niveau - 1].pentity)->Val);
        ListNewr(&paction->ListParams, pentity);
        paction->NbParam++;
    }
}

SCSection* SCGetSectionFromList(List* listsections, char* name)
{
    SCSection* psection = NULL;

    if (!listsections)
        return NULL;

    while(listsections)
    {
        psection = (SCSection*)listsections->car;

        if ((psection->Name) &&
                (OSStrcmp (psection->Name, name) == 0))
            return psection;
        listsections = listsections->cdr;
    }

    return NULL;
}

SCSection* SCCreateSection(List* listsections, char* name, char* FileName)
{
    SCSection* psection;

    /* on verifie que la section n'existe pas */
    psection = SCGetSectionFromList(listsections, name);
    if (psection)
        return NULL;

    psection = (SCSection *)malloc(sizeof(SCSection));
    if (!psection)
        return NULL;

    psection->Name = (char *)malloc(strlen(name) + 1);
    if (!psection->Name)
    {
        free(psection);
        return NULL;
    }

    strcpy(psection->FileName, FileName);
    strcpy(psection->Name, name);
    psection->LineNumber     = yylineno;
    psection->ListEntities   = NULL;
    psection->ListVariables  = NULL;
    psection->NbEntity       = 0;
    psection->NbVariables    = 0;
    return psection;
}


int SCInitYacc(BYTE WithMain)
{
    int i;
    SCSection* psection;

    niveau          = 0;
    InStringRemote  = 0;
    StringRemote[0] = 0;
    InRemoteList    = 0;
    InModeDefun     = 0;

    for (i = 0; i < NB_ACTIONS_OVERLAPED; i++)
    {
        TabEntityAction[i].pentity      = NULL;
        TabEntityAction[i].actionremote = 0;
    }

    if (WithMain)
    {
        while (SCReturnParseSection)
        {
            psection = (SCSection *)SCReturnParseSection->car;
            ListRemove(&SCReturnParseSection, psection);
            SCFreeSection(psection);
        }
        SCReturnParseSection = NULL;
        psection_main = SCCreateSection(SCReturnParseSection, "MAIN", "INTERACTIVE");
        if (!psection_main)
        {
            Err(ERR_ALLOC, "SCSection MAIN not allocated");
            return -1;
        }
        ListNewr(&SCReturnParseSection, psection_main);
        psection_global = psection_main;
    }
    else psection_global = psection_main;
    return 0;
}

void SCFreeYacc()
{
    int i;
    SCSection* psection;

    for (i = 0; i < NB_ACTIONS_OVERLAPED; i++)
    {
        if (TabEntityAction[i].pentity)
        {
            SCFreeEntity(TabEntityAction[i].pentity);
            TabEntityAction[i].pentity = NULL;
            TabEntityAction[i].actionremote = 0;
        }
    }

    while (SCReturnParseSection)
    {
        psection = (SCSection *)SCReturnParseSection->car;
        ListRemove(&SCReturnParseSection, psection);
        SCFreeSection(psection);
    }

    return;
}

void SCPrintEntity(SCEntity* pentity, int ind)
{
    SCValue* Value;
    char ch = ' ';

    if ((pentity->Type == SCATOM) || (pentity->Type == SCVARIABLE))
    {
        Value = (SCValue *)pentity->Val;
        if (pentity->Type == SCATOM)
        {
            switch (Value->Type)
            {
            case SCNULL :
                printf("%*cSCATOM NULL\n", ind, ch);
                break;
            case SCINTEGER :
                printf("%*cSCATOM INTEGER [%d]\n", ind, ch, (char*)Value->Val);
                break;
            case SCLONG :
                printf("%*cSCATOM LONG [%ld]\n",ind, ch,(char*)Value->Val);
                break;
            case SCFLOAT :
                printf("%*cSCATOM FLOAT [%f]\n",ind, ch,(char*)Value->Val);
                break;
            case SCPOINTER :
                printf("%*cSCATOM POINTER [%p]\n",ind, ch,(char*)Value->Val);
                break;
            case SCBOOLEAN :
                printf("%*cSCATOM BOOLEAN [%d]\n",ind, ch,(int)Value->Val);
                break;
            case SCDATE   :
                printf("%*cSCATOM DATE [%s]\n",ind, ch,(char*)Value->Val);
                break;
            case SCTIME   :
                printf("%*cSCATOM TIME [%s]\n",ind, ch,(char*)Value->Val);
                break;
            case SCSTRING :
                printf("%*cSCATOM STRING [%s]\n",ind, ch,(char*)Value->Val);
                break;
            case SCCHAR   :
                printf("%*cSCATOM CHAR [%s]\n",ind, ch,(char*)Value->Val);
                break;
            default :  break;
            }
        }
        else
            printf("%*cSCVARIABLE [%s]\n", ind, ch,(char*)((SCVariable*)pentity->Val)->Name);
    }

    return;
}

void SCPrintAction(SCAction* paction, int ind)
{
    SCEntity* pentity;
    SCValue* pvalue;
    List* ListParams;
    char ch = ' ';

    printf("%*cSCOPERATOR [%s]\n", ind, ch, (char*)((SCSysAction*)paction->ActionClass)->Name);
    ListParams = paction->ListParams;
    while(ListParams)
    {
        pentity = (SCEntity*)ListParams->car;
        pvalue = (SCValue *)pentity->Val;
        if ((pentity->Type == SCATOM) ||(pentity->Type == SCVARIABLE))
        {
            SCPrintEntity(pentity, ind + 2);
        }
        else
        {
            if (pentity->Type == SCOPERATOR)
            {
                SCPrintAction ((SCAction*)pentity->Val, ind + 2);
            }
        }
        ListParams = ListParams->cdr;
    }

    return;
}

void SCTraceSections (List* lsection)
{
    SCSection* psection;
    SCEntity* pentity;
    SCVariable* pvariable;
    List* ListSections;
    List* ListEntities;
    List* ListVariables;
    int ind = 2;

    printf("------ Debut Trace Arbre ------\n");

    ListSections = lsection;
    while(ListSections)
    {
        psection = (SCSection*)ListSections->car;
        printf("Section [%s] (", psection->Name);

        ListVariables = psection->ListVariables;
        printf(" Nb Var.(%d) ", psection->NbVariables);
        while(ListVariables)
        {
            pvariable = (SCVariable*)ListVariables->car;
            printf(" %s", pvariable->Name);
            ListVariables = ListVariables->cdr;
        }
        printf(")\n");

        ListEntities= psection->ListEntities;
        while(ListEntities)
        {
            pentity = (SCEntity*)ListEntities->car;
            switch (pentity->Type)
            {
            case SCOPERATOR:
                SCPrintAction((SCAction*)pentity->Val, ind);
                break;
            case SCVARIABLE:
            case SCATOM:
                SCPrintEntity(pentity, ind);
                break;
            default: break;
            }
            ListEntities = ListEntities->cdr;
        }
        ListSections = ListSections->cdr;
    }

    printf("------ Fin Trace Arbre ------\n");

    return;
}



yytabelem yyexca[] ={
                        -1, 1,
                        0, -1,
                        -2, 0,
                    };
# define YYNPROD 45
# define YYLAST 212
yytabelem yyact[]={

                      7,     8,     9,    10,    11,    18,    26,    18,    21,    12,
                      13,    14,    15,    31,    61,     7,     8,     9,    10,    11,
                      18,    28,    20,    36,    12,    13,    14,    15,    31,    57,
                      7,     8,     9,    10,    11,    18,     1,    19,     0,    12,
                      13,    14,    15,    16,    56,     7,     8,     9,    10,    11,
                      18,     0,     0,     0,    12,    13,    14,    15,    16,    52,
                      7,     8,     9,    10,    11,    18,     0,     0,     0,    12,
                      13,    14,    15,    31,    39,     7,     8,     9,    10,    11,
                      18,     0,     0,     0,    12,    13,    14,    15,    31,    27,
                      5,     7,     8,     9,    10,    11,    18,    47,     0,     0,
                      12,    13,    14,    15,    16,     7,     8,     9,    10,    11,
                      18,    45,    37,     4,    12,    13,    14,    15,    31,     7,
                      8,     9,    10,    11,    18,    18,    18,    42,    12,    13,
                      14,    15,    16,    18,    50,    35,    18,    24,    22,    25,
                      17,    31,    18,    24,    44,    25,     3,     6,    60,    51,
                      43,     0,     2,     0,    55,     0,     0,    23,    55,     0,
                      0,    46,     0,    34,     0,    30,    33,    38,     0,    32,
                      0,    29,    23,    41,     0,     0,     0,    38,     0,    40,
                      49,     0,     0,     0,     0,     0,    48,    54,     0,     0,
                      0,    54,    59,    53,     0,    49,     0,    53,    58,     0,
                      0,    48,     0,     0,     0,    59,     0,     0,     0,     0,
                      0,    58 };
yytabelem yypact[]={

                       -1000,  -166, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
                       -1000, -1000, -1000, -1000, -1000, -1000,  -126, -1000, -1000,  -264,
                       -182,  -129,  -255, -1000, -1000, -1000,  -136, -1000,  -197, -1000,
                       -1000,  -120,  -138,  -138, -1000,  -152,  -137, -1000, -1000, -1000,
                       -1000, -1000,  -212, -1000, -1000, -1000,  -227,  -242, -1000, -1000,
                       -152, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
                       -257, -1000 };
yytabelem yypgo[]={

                      0,   140,   147,   150,   144,    37,   111,    36,    97,    23,
                      112,    22,    21,     8,   127 };
yytabelem yyr1[]={

                     0,     7,     7,     7,     7,     7,     6,     6,     5,     9,
                     9,     8,     8,     8,     8,     4,     4,     4,     4,    11,
                     11,    13,    14,    14,    14,    14,    14,    14,    12,    12,
                     12,    12,     3,     3,     3,     3,     3,     3,     3,     3,
                     3,     3,     2,    10,     1 };
yytabelem yyr2[]={

                     0,     0,     5,     5,     5,     5,    13,    15,     5,     3,
                     5,     3,     3,     5,     5,     7,     9,    11,    11,     3,
                     3,     3,     3,     3,     3,     5,     5,     5,     3,     3,
                     5,     5,     3,     3,     3,     3,     3,     3,     3,     3,
                     3,     3,     3,     3,     3 };
yytabelem yychk[]={

                      -1000,    -7,    -3,    -4,    -6,   256,    -2,   257,   258,   259,
                      260,   261,   266,   267,   268,   269,   270,    -1,   262,    -5,
                      -11,   -13,   264,    -1,   263,   265,   270,   271,   -12,    -3,
                      -4,   270,    -2,    -4,    -1,   271,    -9,   -10,    -1,   271,
                      -3,    -4,   -14,    -3,    -4,    -6,   -14,    -8,    -3,    -4,
                      271,   -10,   271,    -3,    -4,    -6,   271,   271,    -3,    -4,
                      -8,   271 };
yytabelem yydef[]={

                      1,    -2,     2,     3,     4,     5,    32,    33,    34,    35,
                      36,    37,    38,    39,    40,    41,     0,    42,    44,     0,
                      0,     0,     0,    19,    20,    21,     0,    15,     0,    28,
                      29,     0,     0,     0,     8,     0,     0,     9,    43,    16,
                      30,    31,     0,    22,    23,    24,     0,     0,    11,    12,
                      0,    10,    17,    25,    26,    27,    18,     6,    13,    14,
                      0,     7 };
typedef struct { char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#    define YYDEBUG    0    /* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
    {
        "SCINTEGER",    257,
        "SCLONG",    258,
        "SCFLOAT",    259,
        "SCBOOLEAN",    260,
        "SCSTRING",    261,
        "SCVARIABLE",    262,
        "SCOPERATOR",    263,
        "SCOPERATORDEFUN",    264,
        "SCOPERATORREMOTE",    265,
        "SCCHAR",    266,
        "SCTIME",    267,
        "SCDATE",    268,
        "SCNULL",    269,
        "PAR_DEB",    270,
        "PAR_FIN",    271,
        "ERR_CHAR",    272,
        "ERR_INT",    273,
        "ERR_LONG",    274,
        "ERR_FLOAT",    275,
        "ERR_TIME",    276,
        "ERR_DATE",    277,
        "ERR_SYNTAX",    278,
        "ERR_ALLOC",    279,
        "ERR_NOTFOUND",    280,
        "-unknown-",    -1    /* ends search */
    };

char * yyreds[] =
    {
        "-no such reduction-",
        "program : /* empty */",
        "program : program object",
        "program : program statement",
        "program : program function",
        "program : program error",
        "function : PAR_DEB ope_defun PAR_DEB PAR_FIN funclist PAR_FIN",
        "function : PAR_DEB ope_defun PAR_DEB paramlist PAR_FIN funclist PAR_FIN",
        "ope_defun : SCOPERATORDEFUN object_var_string",
        "paramlist : object_var_local",
        "paramlist : paramlist object_var_local",
        "funclist : object",
        "funclist : statement",
        "funclist : funclist object",
        "funclist : funclist statement",
        "statement : PAR_DEB ope PAR_FIN",
        "statement : PAR_DEB ope statlist PAR_FIN",
        "statement : PAR_DEB ope_remote object_var remotelist PAR_FIN",
        "statement : PAR_DEB ope_remote statement remotelist PAR_FIN",
        "ope : object_var_string",
        "ope : SCOPERATOR",
        "ope_remote : SCOPERATORREMOTE",
        "remotelist : object",
        "remotelist : statement",
        "remotelist : function",
        "remotelist : remotelist object",
        "remotelist : remotelist statement",
        "remotelist : remotelist function",
        "statlist : object",
        "statlist : statement",
        "statlist : statlist object",
        "statlist : statlist statement",
        "object : object_var",
        "object : SCINTEGER",
        "object : SCLONG",
        "object : SCFLOAT",
        "object : SCBOOLEAN",
        "object : SCSTRING",
        "object : SCCHAR",
        "object : SCTIME",
        "object : SCDATE",
        "object : SCNULL",
        "object_var : object_var_string",
        "object_var_local : object_var_string",
        "object_var_string : SCVARIABLE",
    };
#endif /* YYDEBUG */
/* @(#)27       1.7.1.1  com/cmd/lang/yacc/yaccpar, cmdlang, bos320, 9209320f 2/19/92 16:36:02 */
/*
 * COMPONENT_NAME: (CMDLANG) Language Utilities
 *
 * FUNCTIONS: yyparse
 * ORIGINS: 03
 */
/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#ifdef YYSPLIT
#   define YYERROR      return(-2)
#else
#   define YYERROR      goto yyerrlab
#endif
#ifdef YACC_MSG
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif
#include <nl_types.h>
nl_catd catd;
#endif
#define YYACCEPT        return(0)
#define YYABORT         return(1)
#ifndef YACC_MSG
#define YYBACKUP( newtoken, newvalue )\
{\
        if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
        {\
                yyerror( "syntax error - cannot backup" );\
                YYERROR;\
        }\
        yychar = newtoken;\
        yystate = *yyps;\
        yylval = newvalue;\
        goto yynewstate;\
}
#else
#define YYBACKUP( newtoken, newvalue )\
{\
        if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
        {\
                catd=catopen("yacc_user.cat",0);\
                yyerror(catgets(catd,1,1,"syntax error - cannot backup" ));\
                YYERROR;\
        }\
        yychar = newtoken;\
        yystate = *yyps;\
        yylval = newvalue;\
        goto yynewstate;\
}
#endif
#define YYRECOVERING()  (!!yyerrflag)
#ifndef YYDEBUG
#       define YYDEBUG  1       /* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;                    /* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG          (-1000)

#ifdef YYSPLIT
#   define YYSCODE { \
                        extern int (*_yyf[])(); \
                        register int yyret; \
                        if (_yyf[yytmp]) \
                            if ((yyret=(*_yyf[yytmp])()) == -2) \
                                    goto yyerrlab; \
                                else if (yyret>=0) return(yyret); \
                   }
#endif

/*
** global variables used by the parser
*/
YYSTYPE yyv[ YYMAXDEPTH ];      /* value stack */
int yys[ YYMAXDEPTH ];          /* state stack */

YYSTYPE *yypv;                  /* top of value stack */
YYSTYPE *yypvt;                 /* top of value stack for $vars */
int *yyps;                      /* top of state stack */

int yystate;                    /* current state */
int yytmp;                      /* extra var (lasts between blocks) */

int yynerrs;                    /* number of errors */
int yyerrflag;                  /* error recovery flag */
int yychar;                     /* current input token number */

#ifdef __cplusplus
 #ifdef _CPP_IOSTREAMS
  #include <iostream.h>
extern void yyerror (char *); /* error message routine -- iostream version */
 #else
  #include <stdio.h>
extern "C" void yyerror (char *); /* error message routine -- stdio version */
 #endif /* _CPP_IOSTREAMS */
extern "C" int yylex(void);        /* return the next token */
#endif /* __cplusplus */


/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
#ifdef __cplusplus
extern "C"
#endif /* __cplusplus */
    int
    yyparse(SC* psc)
{
    /*
    ** Initialize externals - yyparse may be called more than once
    */
    yypv = &yyv[-1];
    yyps = &yys[-1];
    yystate = 0;
    yytmp = 0;
    yynerrs = 0;
    yyerrflag = 0;
    yychar = -1;
#ifdef YACC_MSG
    catd=catopen("yacc_user.cat",0);
#endif
    goto yystack;
    {
        register YYSTYPE *yy_pv;        /* top of value stack */
        register int *yy_ps;            /* top of state stack */
        register int yy_state;          /* current state */
        register int  yy_n;             /* internal state number info */

        /*
        ** get globals into registers.
        ** branch to here only if YYBACKUP was called.
        */
yynewstate:
        yy_pv = yypv;
        yy_ps = yyps;
        yy_state = yystate;
        goto yy_newstate;

        /*
        ** get globals into registers.
        ** either we just started, or we just finished a reduction
        */
yystack:
        yy_pv = yypv;
        yy_ps = yyps;
        yy_state = yystate;

        /*
        ** top of for (;;) loop while no reductions done
        */
yy_stack:
        /*
        ** put a state and value onto the stacks
        */
#if YYDEBUG
        /*
        ** if debugging, look up token value in list of value vs.
        ** name pairs.  0 and negative (-1) are special values.
        ** Note: linear search is used since time is not a real
        ** consideration while debugging.
        */
        if ( yydebug )
        {
            register int yy_i;

#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
            cout << "State " << yy_state << " token ";
            if ( yychar == 0 )
                cout << "end-of-file" << endl;
            else if ( yychar < 0 )
                cout << "-none-" << endl;
#else
            printf( "State %d, token ", yy_state );
            if ( yychar == 0 )
                printf( "end-of-file\n" );
            else if ( yychar < 0 )
                printf( "-none-\n" );
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
            else
            {
                for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
                        yy_i++ )
                {
                    if ( yytoks[yy_i].t_val == yychar )
                        break;
                }
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                cout << yytoks[yy_i].t_name << endl;
#else
                printf( "%s\n", yytoks[yy_i].t_name );
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
            }
        }
#endif /* YYDEBUG */
        if ( ++yy_ps >= &yys[ YYMAXDEPTH ] )    /* room on stack? */
        {
#ifndef YACC_MSG
            yyerror( "yacc stack overflow" );
#else
            yyerror(catgets(catd,1,2,"yacc stack overflow" ));
#endif
            YYABORT;
        }
        *yy_ps = yy_state;
        *++yy_pv = yyval;

        /*
        ** we have a new state - find out what to do
        */
yy_newstate:
        if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
            goto yydefault;         /* simple state */
#if YYDEBUG
        /*
        ** if debugging, need to mark whether new token grabbed
        */
        yytmp = yychar < 0;
#endif
        if ( ( yychar < 0 ) && ( ( yychar = yylex(psc) ) < 0 ) )
            yychar = 0;             /* reached EOF */
#if YYDEBUG
        if ( yydebug && yytmp )
        {
            register int yy_i;

#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
            cout << "Received token " << endl;
            if ( yychar == 0 )
                cout << "end-of-file" << endl;
            else if ( yychar < 0 )
                cout << "-none-" << endl;
#else
            printf( "Received token " );
            if ( yychar == 0 )
                printf( "end-of-file\n" );
            else if ( yychar < 0 )
                printf( "-none-\n" );
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
            else
            {
                for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
                        yy_i++ )
                {
                    if ( yytoks[yy_i].t_val == yychar )
                        break;
                }
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                cout << yytoks[yy_i].t_name << endl;
#else
                printf( "%s\n", yytoks[yy_i].t_name );
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
            }
        }
#endif /* YYDEBUG */
        if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
            goto yydefault;
        if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )  /*valid shift*/
        {
            yychar = -1;
            yyval = yylval;
            yy_state = yy_n;
            if ( yyerrflag > 0 )
                yyerrflag--;
            goto yy_stack;
        }

yydefault:
        if ( ( yy_n = yydef[ yy_state ] ) == -2 )
        {
#if YYDEBUG
            yytmp = yychar < 0;
#endif
            if ( ( yychar < 0 ) && ( ( yychar = yylex(psc) ) < 0 ) )
                yychar = 0;             /* reached EOF */
#if YYDEBUG
            if ( yydebug && yytmp )
            {
                register int yy_i;

#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                cout << "Received token " << endl;
                if ( yychar == 0 )
                    cout << "end-of-file" << endl;
                else if ( yychar < 0 )
                    cout << "-none-" << endl;
#else
                printf( "Received token " );
                if ( yychar == 0 )
                    printf( "end-of-file\n" );
                else if ( yychar < 0 )
                    printf( "-none-\n" );
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
                else
                {
                    for ( yy_i = 0;
                            yytoks[yy_i].t_val >= 0;
                            yy_i++ )
                    {
                        if ( yytoks[yy_i].t_val
                                == yychar )
                        {
                            break;
                        }
                    }
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                    cout << yytoks[yy_i].t_name << endl;
#else
                    printf( "%s\n", yytoks[yy_i].t_name );
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
                }
            }
#endif /* YYDEBUG */
            /*
            ** look through exception table
            */
            {
                register int *yyxi = yyexca;

                while ( ( *yyxi != -1 ) ||
                        ( yyxi[1] != yy_state ) )
                {
                    yyxi += 2;
                }
                while ( ( *(yyxi += 2) >= 0 ) &&
                        ( *yyxi != yychar ) )
                    ;
                if ( ( yy_n = yyxi[1] ) < 0 )
                    YYACCEPT;
            }
        }

        /*
        ** check for syntax error
        */
        if ( yy_n == 0 )        /* have an error */
        {
            /* no worry about speed here! */
            switch ( yyerrflag )
            {
            case 0:         /* new error */
#ifndef YACC_MSG
                yyerror( "syntax error" );
#else
                yyerror(catgets(catd,1,3,"syntax error" ));
#endif
                goto skip_init;
yyerrlab:
                /*
                ** get globals into registers.
                ** we have a user generated syntax type error
                */
                yy_pv = yypv;
                yy_ps = yyps;
                yy_state = yystate;
                yynerrs++;
skip_init:
            case 1:
            case 2:         /* incompletely recovered error */
                /* try again... */
                yyerrflag = 3;
                /*
                ** find state where "error" is a legal
                ** shift action
                */
                while ( yy_ps >= yys )
                {
                    yy_n = yypact[ *yy_ps ] + YYERRCODE;
                    if ( yy_n >= 0 && yy_n < YYLAST &&
                            yychk[yyact[yy_n]] == YYERRCODE)                                        {
                        /*
                        ** simulate shift of "error"
                        */
                        yy_state = yyact[ yy_n ];
                        goto yy_stack;
                    }
                    /*
                    ** current state has no shift on
                    ** "error", pop stack
                    */
#if YYDEBUG
                    if ( yydebug )
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                        cout << "Error recovery pops state "
                        << (*yy_ps)
                        << ", uncovers state "
                        << yy_ps[-1] << endl;
#else
#       define _POP_ "Error recovery pops state %d, uncovers state %d\n"
                        printf( _POP_, *yy_ps,
                                yy_ps[-1] );
#       undef _POP_
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
#endif
                    yy_ps--;
                    yy_pv--;
                }
                /*
                ** there is no state on stack with "error" as
                ** a valid shift.  give up.
                */
                YYABORT;
            case 3:         /* no shift yet; eat a token */
#if YYDEBUG
                /*
                ** if debugging, look up token in list of
                ** pairs.  0 and negative shouldn't occur,
                ** but since timing doesn't matter when
                ** debugging, it doesn't hurt to leave the
                ** tests here.
                */
                if ( yydebug )
                {
                    register int yy_i;

#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                    cout << "Error recovery discards ";
                    if ( yychar == 0 )
                        cout << "token end-of-file" << endl;
                    else if ( yychar < 0 )
                        cout << "token -none-" << endl;
#else
                    printf( "Error recovery discards " );
                    if ( yychar == 0 )
                        printf( "token end-of-file\n" );
                    else if ( yychar < 0 )
                        printf( "token -none-\n" );
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
                    else
                    {
                        for ( yy_i = 0;
                                yytoks[yy_i].t_val >= 0;
                                yy_i++ )
                        {
                            if ( yytoks[yy_i].t_val
                                    == yychar )
                            {
                                break;
                            }
                        }
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                        cout << "token " <<
                        yytoks[yy_i].t_name <<
                        endl;
#else
                        printf( "token %s\n",
                                yytoks[yy_i].t_name );
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
                    }
                }
#endif /* YYDEBUG */
                if ( yychar == 0 )      /* reached EOF. quit */
                    YYABORT;
                yychar = -1;
                goto yy_newstate;
            }
        }/* end if ( yy_n == 0 ) */
        /*
        ** reduction by production yy_n
        ** put stack tops, etc. so things right after switch
        */
#if YYDEBUG
        /*
        ** if debugging, print the string that is the user's
        ** specification of the reduction which is just about
        ** to be done.
        */
        if ( yydebug )
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
            cout << "Reduce by (" << yy_n << ") \"" <<
            yyreds[ yy_n ] << "\"\n";
#else
            printf( "Reduce by (%d) \"%s\"\n",
                    yy_n, yyreds[ yy_n ] );
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
#endif
        yytmp = yy_n;                   /* value to switch over */
        yypvt = yy_pv;                  /* $vars top of value stack */
        /*
        ** Look in goto table for next state
        ** Sorry about using yy_state here as temporary
        ** register variable, but why not, if it works...
        ** If yyr2[ yy_n ] doesn't have the low order bit
        ** set, then there is no action to be done for
        ** this reduction.  So, no saving & unsaving of
        ** registers done.  The only difference between the
        ** code just after the if and the body of the if is
        ** the goto yy_stack in the body.  This way the test
        ** can be made before the choice of what to do is needed.
        */
        {
            /* length of production doubled with extra bit */
            register int yy_len = yyr2[ yy_n ];

            if ( !( yy_len & 01 ) )
            {
                yy_len >>= 1;
                yyval = ( yy_pv -= yy_len )[1]; /* $$ = $1 */
                yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
                           *( yy_ps -= yy_len ) + 1;
                if ( yy_state >= YYLAST ||
                        yychk[ yy_state =
                                   yyact[ yy_state ] ] != -yy_n )
                {
                    yy_state = yyact[ yypgo[ yy_n ] ];
                }
                goto yy_stack;
            }
            yy_len >>= 1;
            yyval = ( yy_pv -= yy_len )[1]; /* $$ = $1 */
            yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
                       *( yy_ps -= yy_len ) + 1;
            if ( yy_state >= YYLAST ||
                    yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
            {
                yy_state = yyact[ yypgo[ yy_n ] ];
            }
        }
        /* save until reenter driver code */
        yystate = yy_state;
        yyps = yy_ps;
        yypv = yy_pv;
    }
    /*
    ** code supplied by user is placed in this switch
    */

    switch(yytmp){

    case 2:
# line 86 "scyacc.y"
        {
            Trace("--- Yacc --- Object");
            /* add object to MAIN section */
            ListNewr(&psection_main->ListEntities, yypvt[-0].pentity);
            psection_main->NbEntity++;

            SCInitYacc(0);
        } /*NOTREACHED*/ break;
    case 3:
# line 94 "scyacc.y"
        {
            Trace("--- Yacc --- Statement");
            /* add object to MAIN section */
            ListNewr(&psection_main->ListEntities, yypvt[-0].pentity);
            psection_main->NbEntity++;

            SCInitYacc(0);
        } /*NOTREACHED*/ break;
    case 4:
# line 102 "scyacc.y"
        {
            Trace("--- Yacc --- Function");
            /* add function to List */
            ListNewr(&SCReturnParseSection, yypvt[-0].psection);

            /* add entity to MAIN section */
            psysaction_global = SCGetSysAction(psc, "DEFUN");
            if (!psysaction_global)
            {
                Err(ERR_NOTFOUND, "SCSysAction not found");
                YYABORT;
            }
            paction_global = SCAllocAction(psc, psection_global, psysaction_global, psysaction_global->Block);
            if (!paction_global)
            {
                Err(ERR_ALLOC, "SCAction not allocated");
                YYABORT;
            }
            paction_global->LineNumber = yylineno;
            /* on ajoute une string du nom de la fonction */
            pentity_global = SCAllocEntity(psc, (char *)(yypvt[-0].psection->Name), SCSTRING);
            if (!pentity_global)
            {
                Err(ERR_ALLOC, "SCEntity SCSTRING not allocated");
                YYABORT;
            }
            ListNewr(&paction_global->ListParams, pentity_global);
            paction_global->NbParam++;
            pentity_global = SCAllocEntity(psc, paction_global, SCOPERATOR);
            if (!pentity_global)
            {
                Err(ERR_ALLOC, "SCEntity SCOPERATOR not allocated");
                YYABORT;
            }
            ListNewr(&psection_main->ListEntities, pentity_global);
            psection_main->NbEntity++;

            SCInitYacc(0);
        } /*NOTREACHED*/ break;
    case 5:
# line 141 "scyacc.y"
    { Err(ERR_SYNTAX, NULL); yyerrok; yyclearin; YYABORT; } /*NOTREACHED*/ break;
    case 6:
# line 146 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- FUNCTION without paramlist");
                yyval.psection = yypvt[-4].psection;
            }
            else
                InRemoteList++;
        } /*NOTREACHED*/ break;
    case 7:
# line 156 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- FUNCTION with paramlist");
                yyval.psection = yypvt[-5].psection;
            }
            else
                InRemoteList++;
        } /*NOTREACHED*/ break;
    case 8:
# line 169 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- Ope (%s)", yypvt[-0].str);
                psection_global = SCCreateSection(SCReturnParseSection, yypvt[-0].str, psc->CurrentFileName);
                if (!psection_global)
                {
                    Err(ERR_ALLOC, "SCSection not allocated or SCSection just created");
                    YYABORT;
                }
                yyval.psection = psection_global;
                InModeDefun = 1;
            }
        } /*NOTREACHED*/ break;
    case 9:
# line 186 "scyacc.y"
        {
            Trace("--- Yacc --- ADD object_local1 PARAMLIST");
        } /*NOTREACHED*/ break;
    case 10:
# line 190 "scyacc.y"
        {
            Trace("--- Yacc --- ADD object_local PARAMLIST");
        } /*NOTREACHED*/ break;
    case 11:
# line 196 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- ADD object1 FUNCLIST");
                SCAddEntityToList(yypvt[-0].pentity);
            }
            else
                InRemoteList--;
        } /*NOTREACHED*/ break;
    case 12:
# line 205 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- ADD statement1 FUNCLIST");
                SCAddEntityToList(yypvt[-0].pentity);
            }
            else
                InRemoteList--;
        } /*NOTREACHED*/ break;
    case 13:
# line 214 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- ADD object FUNCLIST");
                SCAddEntityToList(yypvt[-0].pentity);
            }
            else
                InRemoteList--;
        } /*NOTREACHED*/ break;
    case 14:
# line 223 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- ADD statement FUNCLIST");
                SCAddEntityToList(yypvt[-0].pentity);
            }
            else
                InRemoteList--;
        } /*NOTREACHED*/ break;
    case 15:
# line 236 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- STATEMENT without list");
                niveau--;
                yyval.pentity = TabEntityAction[niveau].pentity;
                TabEntityAction[niveau].pentity      = NULL;
                TabEntityAction[niveau].actionremote = 0;

                if (((niveau - 1) >= 0) && (TabEntityAction[niveau - 1].actionremote))
                    InStringRemote = 1;
            }
            else
                InRemoteList++;
        } /*NOTREACHED*/ break;
    case 16:
# line 252 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- STATEMENT with list");
                niveau--;
                yyval.pentity = TabEntityAction[niveau].pentity;
                TabEntityAction[niveau].pentity      = NULL;
                TabEntityAction[niveau].actionremote = 0;

                if (((niveau - 1) >= 0) && (TabEntityAction[niveau - 1].actionremote))
                    InStringRemote = 1;
            }
            else
                InRemoteList++;
        } /*NOTREACHED*/ break;
    case 17:
# line 268 "scyacc.y"
        {
            Trace("--- Yacc --- STATEMENT REMOTE [%d]", InStringRemote);
            if (InStringRemote && (InRemoteList == 0))
                InStringRemote = 0;

            if (!InStringRemote)
            {
                /* ne pas oublier de supprimer la derniere parenthese */
                StringRemote[strlen(StringRemote) - 1] = 0;
                Trace("--- Yacc --- STATEMENT REMOTE [%s]", StringRemote);

                niveau--;
                /*  on ajoute l'object_var */
                paction_global = (SCAction *)((SCEntity *)(TabEntityAction[niveau].pentity)->Val);
                ListNewr(&paction_global->ListParams, yypvt[-2].pentity);
                paction_global->NbParam++;
                /* on ajoute la string */
                pentity_global = SCAllocEntity(psc, StringRemote, SCSTRING);
                if (!pentity_global)
                {
                    Err(ERR_ALLOC, "SCEntity SCSTRING not allocated");
                    YYABORT;
                }
                ListNewr(&paction_global->ListParams, pentity_global);
                paction_global->NbParam++;
                StringRemote[0] = 0;

                yyval.pentity = TabEntityAction[niveau].pentity;
                TabEntityAction[niveau].pentity      = NULL;
                TabEntityAction[niveau].actionremote = 0;

                if (((niveau - 1) >= 0) && (TabEntityAction[niveau - 1].actionremote))
                    InStringRemote = 1;
            }
            else
                InRemoteList--;  /* pour decrementer l'object_var */
        } /*NOTREACHED*/ break;
    case 18:
# line 306 "scyacc.y"
        {
            Trace("--- Yacc --- STATEMENT REMOTE [%d]", InStringRemote);
            if (InStringRemote && (InRemoteList == 0))
                InStringRemote = 0;

            if (!InStringRemote)
            {
                /* ne pas oublier de supprimer la derniere parenthese */
                StringRemote[strlen(StringRemote) - 1] = 0;
                Trace("--- Yacc --- STATEMENT REMOTE [%s]", StringRemote);

                niveau--;
                /*  on ajoute le statement */
                paction_global = (SCAction *)((SCEntity *)(TabEntityAction[niveau].pentity)->Val);
                ListNewr(&paction_global->ListParams, yypvt[-2].pentity);
                paction_global->NbParam++;
                /* on ajoute la string */
                pentity_global = SCAllocEntity(psc, StringRemote, SCSTRING);
                if (!pentity_global)
                {
                    Err(ERR_ALLOC, "SCEntity SCSTRING not allocated");
                    YYABORT;
                }
                ListNewr(&paction_global->ListParams, pentity_global);
                paction_global->NbParam++;
                StringRemote[0] = 0;

                yyval.pentity = TabEntityAction[niveau].pentity;
                TabEntityAction[niveau].pentity      = NULL;
                TabEntityAction[niveau].actionremote = 0;


                if (((niveau - 1) >= 0) && (TabEntityAction[niveau - 1].actionremote))
                    InStringRemote = 1;
            }
            else
                InRemoteList--;  /* pour decrementer le statement */
        } /*NOTREACHED*/ break;
    case 19:
# line 347 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- OPE (%s)", yypvt[-0].str);
                psysaction_global = SCGetSysAction(psc, "EXECUTE");
                if (!psysaction_global)
                {
                    Err(ERR_NOTFOUND, "SCSysAction not found");
                    YYABORT;
                }
                paction_global = SCAllocAction(psc, psection_global, psysaction_global, psysaction_global->Block);
                if (!paction_global)
                {
                    Err(ERR_ALLOC, "SCAction not allocated");
                    YYABORT;
                }
                paction_global->LineNumber = yylineno;
                /* on ajoute une string du nom de l'operateur */
                pentity_global = SCAllocEntity(psc, yypvt[-0].str, SCSTRING);
                if (!pentity_global)
                {
                    Err(ERR_ALLOC, "SCEntity SCSTRING not allocated");
                    YYABORT;
                }
                ListNewr(&paction_global->ListParams, pentity_global);
                paction_global->NbParam++;

                pentity_global = SCAllocEntity(psc, paction_global, SCOPERATOR);
                if (!pentity_global)
                {
                    Err(ERR_ALLOC, "SCEntity SCOPERATOR not allocated");
                    YYABORT;
                }
                TabEntityAction[niveau].pentity       = pentity_global;
                TabEntityAction[niveau].actionremote  = 0;
                niveau++;
            }
        } /*NOTREACHED*/ break;
    case 20:
# line 385 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- OPE (%s)", yypvt[-0].str);
                psysaction_global = SCGetSysAction(psc, yypvt[-0].str);
                if (!psysaction_global)
                {
                    Err(ERR_NOTFOUND, "SCSysAction not found");
                    YYABORT;
                }
                paction_global = SCAllocAction(psc, psection_global, psysaction_global, psysaction_global->Block);
                if (!paction_global)
                {
                    Err(ERR_ALLOC, "SCAction not allocated");
                    YYABORT;
                }
                paction_global->LineNumber = yylineno;

                pentity_global = SCAllocEntity(psc, paction_global, SCOPERATOR);
                if (!pentity_global)
                {
                    Err(ERR_ALLOC, "SCEntity SCOPERATOR not allocated");
                    YYABORT;
                }
                TabEntityAction[niveau].pentity       = pentity_global;
                TabEntityAction[niveau].actionremote  = 0;
                niveau++;
            }
        } /*NOTREACHED*/ break;
    case 21:
# line 417 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- OPE REMOTE (%s)", yypvt[-0].str);
                psysaction_global = SCGetSysAction(psc, yypvt[-0].str);
                if (!psysaction_global)
                {
                    Err(ERR_NOTFOUND, "SCSysAction    not found");
                    YYABORT;
                }
                paction_global = SCAllocAction(psc, psection_global, psysaction_global, psysaction_global->Block);
                if (!paction_global)
                {
                    Err(ERR_ALLOC, "SCAction not allocated");
                    YYABORT;
                }
                paction_global->LineNumber = yylineno;

                pentity_global = SCAllocEntity(psc, paction_global, SCOPERATOR);
                if (!pentity_global)
                {
                    Err(ERR_ALLOC, "SCEntity SCOPERATOR not allocated");
                    YYABORT;
                }
                TabEntityAction[niveau].pentity       = pentity_global;
                TabEntityAction[niveau].actionremote  = 1;
                niveau++;
            }
            else
                InRemoteList++;
        } /*NOTREACHED*/ break;
    case 22:
# line 451 "scyacc.y"
        { InRemoteList--;
            Trace("--- Yacc --- ADD object1 REMOTELIST (%d)", InRemoteList);
        } /*NOTREACHED*/ break;
    case 23:
# line 454 "scyacc.y"
        { InRemoteList--;
            Trace("--- Yacc --- ADD statement1 REMOTELIST (%d)", InRemoteList);
        } /*NOTREACHED*/ break;
    case 24:
# line 457 "scyacc.y"
        { InRemoteList--;
            Trace("--- Yacc --- ADD function1 REMOTELIST (%d)", InRemoteList);
        } /*NOTREACHED*/ break;
    case 25:
# line 460 "scyacc.y"
        { InRemoteList--;
            Trace("--- Yacc --- ADD object REMOTELIST (%d)", InRemoteList);
        } /*NOTREACHED*/ break;
    case 26:
# line 463 "scyacc.y"
        { InRemoteList--;
            Trace("--- Yacc --- ADD statement REMOTELIST (%d)", InRemoteList);
        } /*NOTREACHED*/ break;
    case 27:
# line 466 "scyacc.y"
        { InRemoteList--;
            Trace("--- Yacc --- ADD function REMOTELIST (%d)", InRemoteList);
        } /*NOTREACHED*/ break;
    case 28:
# line 472 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- ADD object1 STATLIST");
                SCAddEntityToList(yypvt[-0].pentity);
            }
            else
                InRemoteList--;
        } /*NOTREACHED*/ break;
    case 29:
# line 481 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- ADD statement1 STATLIST");
                SCAddEntityToList(yypvt[-0].pentity);
            }
            else
                InRemoteList--;
        } /*NOTREACHED*/ break;
    case 30:
# line 490 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- ADD object STATLIST");
                SCAddEntityToList(yypvt[-0].pentity);
            }
            else
                InRemoteList--;
        } /*NOTREACHED*/ break;
    case 31:
# line 499 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- ADD statement STATLIST");
                SCAddEntityToList(yypvt[-0].pentity);
            }
            else
                InRemoteList--;
        } /*NOTREACHED*/ break;
    case 32:
# line 511 "scyacc.y"
        {
            yyval.pentity = yypvt[-0].pentity;
        } /*NOTREACHED*/ break;
    case 33:
# line 514 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- SCINTEGER (%s)", yypvt[-0].str);
                pentity_global = SCAllocEntity(psc, yypvt[-0].str, SCINTEGER);
                if (!pentity_global)
                {
                    Err(ERR_ALLOC, "SCEntity SCINTEGER not allocated");
                    YYABORT;
                }
	            pentity_global->LineNumber = yylineno;

                yyval.pentity = pentity_global;
            }
            else
                InRemoteList++;
        } /*NOTREACHED*/ break;
    case 34:
# line 529 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- SCLONG (%s)", yypvt[-0].str);
                pentity_global = SCAllocEntity(psc, yypvt[-0].str, SCLONG);
                if (!pentity_global)
                {
                    Err(ERR_ALLOC, "SCEntity SCLONG not allocated");
                    YYABORT;
                }
	            pentity_global->LineNumber = yylineno;

				yyval.pentity = pentity_global;
            }
            else
                InRemoteList++;
        } /*NOTREACHED*/ break;
    case 35:
# line 544 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- SCFLOAT (%s)", yypvt[-0].str);
                pentity_global = SCAllocEntity(psc, yypvt[-0].str, SCFLOAT);
                if (!pentity_global)
                {
                    Err(ERR_ALLOC, "SCEntity SCFLOAT not allocated");
                    YYABORT;
                }
	            pentity_global->LineNumber = yylineno;

				yyval.pentity = pentity_global;
            }
            else
                InRemoteList++;
        } /*NOTREACHED*/ break;
    case 36:
# line 559 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- SCBOOLEAN (%d)", yypvt[-0].str);
                pentity_global = SCAllocEntity(psc, yypvt[-0].str, SCBOOLEAN);
                if (!pentity_global)
                {
                    Err(ERR_ALLOC, "SCEntity SCBOOLEAN not allocated");
                    YYABORT;
                }
	            pentity_global->LineNumber = yylineno;

				yyval.pentity = pentity_global;
            }
            else
                InRemoteList++;
        } /*NOTREACHED*/ break;
    case 37:
# line 574 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- SCSTRING (%s)", yypvt[-0].str);
                pentity_global = SCAllocEntity(psc, yypvt[-0].str, SCSTRING);
                if (!pentity_global)
                {
                    Err(ERR_ALLOC, "SCEntity SCSTRING not allocated");
                    YYABORT;
                }
	            pentity_global->LineNumber = yylineno;

				yyval.pentity = pentity_global;
            }
            else
                InRemoteList++;
        } /*NOTREACHED*/ break;
    case 38:
# line 589 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- SCCHAR (%s)", yypvt[-0].str);
                pentity_global = SCAllocEntity(psc, yypvt[-0].str, SCCHAR);
                if (!pentity_global)
                {
                    Err(ERR_ALLOC, "SCEntity SCCHAR not allocated");
                    YYABORT;
                }
	            pentity_global->LineNumber = yylineno;

                yyval.pentity = pentity_global;
            }
            else
                InRemoteList++;
        } /*NOTREACHED*/ break;
    case 39:
# line 604 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- SCTIME (%s)", yypvt[-0].str);
                pentity_global = SCAllocEntity(psc, yypvt[-0].str, SCTIME);
                if (!pentity_global)
                {
                    Err(ERR_ALLOC, "SCEntity SCTIME not allocated");
                    YYABORT;
                }
	            pentity_global->LineNumber = yylineno;

				yyval.pentity = pentity_global;
            }
            else
                InRemoteList++;
        } /*NOTREACHED*/ break;
    case 40:
# line 619 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- SCDATE (%s)", yypvt[-0].str);
                pentity_global = SCAllocEntity(psc, yypvt[-0].str, SCDATE);
                if (!pentity_global)
                {
                    Err(ERR_ALLOC, "SCEntity SCDATE not allocated");
                    YYABORT;
                }
	            pentity_global->LineNumber = yylineno;

				yyval.pentity = pentity_global;
            }
            else
                InRemoteList++;
        } /*NOTREACHED*/ break;
    case 41:
# line 634 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- SCNULL (%s)", yypvt[-0].str);
                pentity_global = SCAllocEntity(psc, yypvt[-0].str, SCNULL);
                if (!pentity_global)
                {
                    Err(ERR_ALLOC, "SCEntity SCNULL not allocated");
                    YYABORT;
                }
	            pentity_global->LineNumber = yylineno;

				yyval.pentity = pentity_global;
            }
            else
                InRemoteList++;
        } /*NOTREACHED*/ break;
    case 42:
# line 652 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- SCVARIABLE (%s)", yypvt[-0].str);

                if (InModeDefun)
                {
                    pvariable_global = SCFindVariableInList(psc, (char*)yypvt[-0].str, psection_global->ListVariables);
                    if (!pvariable_global)
                    {
                        pvariable_global = SCFindVariableInList(psc, (char*)yypvt[-0].str, psc->ListVariables);
                        if (!pvariable_global)
                        {
                            pvariable_global = SCFindVariableInList(psc, (char*)yypvt[-0].str, psection_main->ListVariables);
                            if (!pvariable_global)
                            {
                                pentity_global = SCAllocEntity(psc, yypvt[-0].str, SCVARIABLE_BYNAME);
                                if (!pentity_global)
                                {
                                    Err(ERR_ALLOC, "SCEntity SCVARIABLE not allocated");
                                    YYABORT;
                                }
					            pentity_global->LineNumber = yylineno;

                                ListNewr(&psection_main->ListVariables, (SCVariable*)pentity_global->Val);
                                psection_main->NbVariables++;
                            }
                        }
                    }
                }
                else
                {
                    pvariable_global = SCFindVariableInList(psc, (char*)yypvt[-0].str, psc->ListVariables);
                    if (!pvariable_global)
                    {
                        pvariable_global = SCFindVariableInList(psc, (char*)yypvt[-0].str, psection_main->ListVariables);
                        if (!pvariable_global)
                        {
                            pentity_global = SCAllocEntity(psc, yypvt[-0].str, SCVARIABLE_BYNAME);
                            if (!pentity_global)
                            {
                                Err(ERR_ALLOC, "SCEntity SCVARIABLE not allocated");
                                YYABORT;
                            }
				            pentity_global->LineNumber = yylineno;

                            ListNewr(&psection_main->ListVariables, (SCVariable*)pentity_global->Val);
                            psection_main->NbVariables++;
                        }
                    }
                }

                if (pvariable_global)
                {
                    pentity_global = SCAllocEntity(psc, pvariable_global, SCVARIABLE_BYVAL);
                    if (!pentity_global)
                    {
                        Err(ERR_ALLOC, "SCEntity SCVARIABLE not allocated");
                        YYABORT;
                    }
					pentity_global->LineNumber = yylineno;
				}
                
				yyval.pentity = pentity_global;
                if (((niveau - 1) >= 0) && (TabEntityAction[niveau - 1].actionremote))
                    InStringRemote = 1;
            }
            else
                InRemoteList++;
        } /*NOTREACHED*/ break;
    case 43:
# line 720 "scyacc.y"
        {
            if (!InStringRemote)
            {
                Trace("--- Yacc --- SCVARIABLE LOCALE (%s)", yypvt[-0].str);

                if (SCFindVariableInList(psc, (char*)yypvt[-0].str, psection_global->ListVariables))
                {
                    Err(ERR_ALLOC, "SCEntity SCVARIABLE LOCALE does exist");
                    YYABORT;
                }

                pvariable_global = SCCreateVariable(psc, yypvt[-0].str);
                if (!pvariable_global)
                {
                    Err(ERR_ALLOC, "SCVariable LOCALE not allocated");
                    YYABORT;
                }
				pvariable_global->LineNumber = yylineno;
                ListNewr(&psection_global->ListVariables, pvariable_global);
                psection_global->NbVariables++;
            }
        } /*NOTREACHED*/ break;
    case 44:
# line 744 "scyacc.y"
    { yyval.str = yypvt[-0].str; } /*NOTREACHED*/ break;
    }


    goto yystack;           /* reset registers in driver code */
}
