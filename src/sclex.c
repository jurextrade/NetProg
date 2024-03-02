#define _CRT_SECURE_NO_WARNINGS
# include <stdio.h>
# include <stddef.h>
# include <locale.h>
# include <stdlib.h>
# define U(x) ((x)&0377)
# define NCH 256
# define NLSTATE yyprevious=YYNEWLINE
# define BEGIN yybgin = yysvec + 1 +
# define INITIAL 0
# define YYLERR yysvec
# define YYSTATE (yyestate-yysvec-1)
# define YYOPTIM 1
# define YYLMAX 200

#ifdef __cplusplus
int yylook(void);
extern "C" int yywrap(void), yyless(int), yyreject(void);
#endif /* __cplusplus */

#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
# include <iostream.h>
# define output(c) (*yyout) << ((unsigned char) c)
# define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):yyin->get())==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
# define ECHO (*yyout) << yytext
istream *yyin = &cin;
ostream *yyout = &cout;
#else
# define output(c) putc(c,yyout)
# define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
# define ECHO fprintf(yyout, "%S",yywtext)
FILE *yyin = NULL, *yyout = NULL;
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
# define unput(c) {yytchar= (c);if(yytchar=='\n')yylineno--;*yysptr++=yytchar;}
# define yymore() (yymorfg=1)
# define REJECT { nstr = yyreject(); goto yyfussy;}
# define yysetlocale() setlocale(LC_ALL,"")
# define wreturn(r) return(yywreturn(r))
# define winput() yywinput()
# define wunput(c) yywunput(c)
# define woutput(c) yywoutput(c)
int yyleng; extern unsigned char yytext[];
int yywleng; extern wchar_t yywtext[];
int yymorfg = 0;
int yymbcurmax = -1;
int __once_yylex = 1;
extern unsigned char *yysptr, yysbuf[];
int yytchar;
extern int yylineno;
struct yywork;
struct yysvf {
    struct yywork *yystoff;
    struct yysvf *yyother;
    int *yystops;};
struct yysvf *yyestate;
extern struct yysvf yysvec[], *yybgin;
#include <stdio.h>
#include <stdarg.h>
#include "sclex.h"

#undef  YY_INTERACTIVE
#define YY_INTERACTIVE 0

#undef input
typedef int (*Input) ();
Input input;
#define SetFunction(pf1,pf2)   pf1 = pf2

SC* GlobalSC;

char StrErr[150];

char *entry;
int  ind_entry = 0;

static int yywrap(void);

static char buf[YYLMAX];
static char str_tmp[25];
char *pt_str;
int  length_str;
int yyback(int *p, int m);
# if YYHSIZE
int yyhlook(int c, int v);
int yymlook(int c);
# endif /*YYHSIZE*/
# if YYXSIZE
int yyxlook (int c, int v);
#endif /*YYXSIZE*/
int yywinput();
void yywoutput(int c);
void yywunput(int c);
int yywreturn(int r);
int yyinput();
void yyoutput(int c);
void yyunput(int c);
int yymbinput();
void yymboutput(int c);
void yymbunput(int c);
int yymbreturn(int x);
# define YYNEWLINE 10
#ifdef __cplusplus
extern "C"
#endif /*__cplusplus */
    int yylex(SC* psc){
    int nstr; extern int yyprevious;
    if (__once_yylex) {
        yysetlocale();
#if !(__cplusplus && _CPP_IOSTREAMS)
        if (yyin == NULL) yyin = stdin;
        if (yyout == NULL) yyout = stdout;
#endif /* !(__cplusplus && _CPP_IOSTREAMS) */
        __once_yylex = 0; }
    GlobalSC = psc;
    if(yymbcurmax<=0) yymbcurmax=1;
    while((nstr = yylook()) >= 0)
yyfussy: switch(nstr){
        case 0:
            if(yywrap()) return(0); break;
        case 1:
        { Trace("--- Lex --- commentaire"); }
            /*NOTREACHED*/ break;
        case 2:
            {
                LONG from = 0;
                XFStringRead(psc->MX->TP->XF , buf, &from, (char*)yytext, 1);
                Trace("--- Lex --- string(%s)", buf);
                if (InStringRemote) sprintf(StringRemote + strlen(StringRemote)," %s", buf);
                /* on supprime les " */
                pt_str     = buf;
                length_str = strlen(buf);
                strncpy (buf, pt_str + 1, length_str - 2);
                buf[length_str - 2] = 0;
                yylval.str = buf;
                return SCSTRING;
            }
            /*NOTREACHED*/ break;
        case 3:
            {
                LONG from = 0;
                XFStringRead(psc->MX->TP->XF , buf, &from, (char*)yytext, 1);
                Trace("--- Lex --- char(%s)", buf);
                if (InStringRemote) sprintf(StringRemote + strlen(StringRemote)," %s", buf);
                /* on supprime les ' */
                pt_str     = buf;
                length_str = strlen(buf);
                strncpy (buf, pt_str + 1, length_str - 2);
                buf[length_str - 2] = 0;
                yylval.str = buf;
                return SCCHAR;
            }
            /*NOTREACHED*/ break;
        case 4:
            {
                LONG from = 0;
                XFStringRead(psc->MX->TP->XF , buf, &from, (char*)yytext, 1);
                yylval.val_i = atoi(buf);
                sprintf(str_tmp,"%d",yylval.val_i);
                Trace("--- Lex --- yytext (%s) val_i (%d) str_tmp (%s)", buf, yylval.val_i, str_tmp);
                /* on verifie que l'on n'a pas dépassé la taille max d'un 'int' */
                if (strcmp(buf, str_tmp) != 0)
                {
                    Err(ERR_INT, NULL);
                    return ERR_INT;
                }
                else
                {
                    yylval.str = buf;

                    if (InStringRemote) sprintf(StringRemote + strlen(StringRemote)," %s", buf);
                    return SCINTEGER;
                }
            }
            /*NOTREACHED*/ break;
        case 5:
            {
                LONG from = 0;
                XFStringRead(psc->MX->TP->XF , buf, &from, (char*)yytext, 1);
                buf[strlen(buf) -1] = 0;	/* on supprime le L */
                yylval.val_l = atol(buf);
                sprintf(str_tmp,"%ld",yylval.val_l);
                Trace("--- Lex --- yytext (%s) val_l (%ld) str_tmp (%s)", buf, yylval.val_l, str_tmp);
                /* on vérifie que l'on n'a pas dépassé la taille max d'un 'long' */
                if (strcmp(buf, str_tmp) != 0)
                {
                    Err(ERR_LONG, NULL);
                    return ERR_LONG;
                }
                else
                {
                    yylval.str = buf;

                    if (InStringRemote) sprintf(StringRemote + strlen(StringRemote)," %sL", buf);
                    return SCLONG;
                }
            }
            /*NOTREACHED*/ break;
        case 6:
            {
                LONG from = 0;
                XFStringRead(psc->MX->TP->XF , buf, &from, (char*)yytext, 1);
                yylval.val_f = atof(buf);
                sprintf(str_tmp,"%3.3f",yylval.val_f);
                Trace("--- Lex --- yytext (%s) val_f (%3.3f) str_tmp (%s)", buf, yylval.val_f, str_tmp);
            //    if (strcmp(buf, str_tmp) != 0)
             //   {
             //       Err(ERR_FLOAT, NULL);
               //     return ERR_FLOAT;
             //   }
             //   else
                {
                    yylval.str = buf;

                    if (InStringRemote) sprintf(StringRemote + strlen(StringRemote)," %s", buf);
                    return SCFLOAT;
                }
            }
            /*NOTREACHED*/ break;
        case 7:
            {
                LONG from = 0;
                XFStringRead(psc->MX->TP->XF , buf, &from, (char*)yytext, 1);
                yylval.str = 0;
                if (InStringRemote) sprintf(StringRemote + strlen(StringRemote)," %s", buf);
                return SCNULL;
            }
            /*NOTREACHED*/ break;
        case 8:
            {
                LONG from = 0;
                XFStringRead(psc->MX->TP->XF , buf, &from, (char*)yytext, 1);
                yylval.val_i = 1;
                yylval.str = buf;
				if (InStringRemote) sprintf(StringRemote + strlen(StringRemote)," %s", buf);
                return SCBOOLEAN;
            }
            /*NOTREACHED*/ break;
        case 9:
            {
                LONG from = 0;
                XFStringRead(psc->MX->TP->XF , buf, &from, (char*)yytext, 1);
                yylval.val_i = 0;
                yylval.str = buf;
				if (InStringRemote) sprintf(StringRemote + strlen(StringRemote)," %s", buf);
                return SCBOOLEAN;
            }
            /*NOTREACHED*/ break;
        case 10:
            {
                LONG from = 0;
                XFStringRead(psc->MX->TP->XF , buf, &from, (char*)yytext, 1);
                if (SCCtrlTime(buf) < 0)
                { Err(ERR_TIME, NULL); return ERR_TIME;}
                Trace("--- Lex --- time(%s)", buf);
                yylval.str = buf;
                if (InStringRemote) sprintf(StringRemote + strlen(StringRemote)," %s", buf);
                return SCTIME;
            }
            /*NOTREACHED*/ break;
        case 11:
            {
                LONG from = 0;
                XFStringRead(psc->MX->TP->XF , buf, &from, (char*)yytext, 1);
                if (SCCtrlDate(buf) < 0)
                { Err(ERR_DATE, NULL); return ERR_DATE;}
                Trace("--- Lex --- date(%s)", buf);
                yylval.str = buf;
                if (InStringRemote) sprintf(StringRemote + strlen(StringRemote)," %s", buf);
                return SCDATE;
            }
            /*NOTREACHED*/ break;
        case 12:
            {
                LONG from = 0;
                XFStringRead(psc->MX->TP->XF , buf, &from, (char*)yytext, 1);
                Trace("--- Lex --- VAR (%s)", buf);
                yylval.str = buf;
                if (SCGetSysAction(psc, buf))
                {
                    if (InStringRemote) sprintf(StringRemote + strlen(StringRemote),"%s", buf);
                    if
                    (OSStrncmp(buf, "DEFUN", 5) == 0)
                        return SCOPERATORDEFUN;
                    if
                    (OSStrncmp(buf, "REMOTE", 6) == 0)
                        return SCOPERATORREMOTE;
                    else
                        return SCOPERATOR;
                }
                else
                {
                    if (InStringRemote) sprintf(StringRemote + strlen(StringRemote)," %s", buf);
                    return SCVARIABLE;
                }
            }
            /*NOTREACHED*/ break;
        case 13:
            { if (InStringRemote) strcat(StringRemote, "(");
                return(PAR_DEB);
            }
            /*NOTREACHED*/ break;
        case 14:
            { if (InStringRemote) strcat(StringRemote, ")");
                return(PAR_FIN);
            }
            /*NOTREACHED*/ break;
        case 15:
            ;
            /*NOTREACHED*/ break;
        case 16:
        { Err(ERR_CHAR, NULL); return(ERR_CHAR);}
            /*NOTREACHED*/ break;
        case -1:
            break;
        default:
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
            (*yyout) << "bad switch yylook " << nstr;
#else
            fprintf(yyout,"bad switch yylook %d",nstr);
#endif /* defined(__cplusplus) && defined(_CPP_IOSTREAMS)*/
        } return(0); }
/* end of yylex */
static int yywrap(void)
{
    return(1);
}

int SCGiveChar (int x)
{
    LONG pos = 0;
    char lpbuf;
    char car = (char)x;

    if (x <= 12) return(x);

    /*  printf("(%d- %c-",x,x); */
    XFCharWrite (GlobalSC->MX->TP->XF, &car, &pos, &lpbuf, 1);
    /*  printf("%d)",lpbuf); */
    return lpbuf;
}

int input_file()
{
    return (((yytchar=yysptr>yysbuf?U(*--yysptr):SCGiveChar(getc(yyin)))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar);

}

int input_string()
{
    return (((yytchar=yysptr>yysbuf?U(*--yysptr):SCGiveChar(entry[ind_entry++]))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar);
}

void Err(int IdErr, char *fmt, ...)
{
    char str_err[100];
    va_list va;

    if ( fmt != NULL )
    {
        str_err[0] = 0;
        va_start(va, fmt);
        vsprintf(str_err, fmt, va);
        va_end(va);
    }

    if (*StrErr == 0)
        /* sprintf(StrErr, "***** Erreur (%d) line (%d) str (%s)", IdErr, yylineno, str_err); */
       // sprintf(StrErr, "[File : %s][line : %d] Erreur (%d) str (%s)", GlobalSC->CurrentFileName, yylineno, IdErr, str_err);
	    sprintf(StrErr, "[line : %d] Error (%d) str (%s)", yylineno, IdErr, str_err);

    SCFreeYacc();
    return;
}

void Trace(char *fmt, ...)
{
    char str_msg[YYLMAX + 100];
    va_list va;

    if ( fmt != NULL )
    {
        str_msg[0] = 0;
        va_start(va, fmt);
        vsprintf(str_msg, fmt, va);
        va_end(va);
    }

    /*   printf("%s\n", str_msg);*/
    return;
}

void yyerror(char *fmt, ...)
{
    char str_err[100];
    va_list va;

    if ( fmt != NULL )
    {
        str_err[0] = 0;
        va_start(va, fmt);
        vsprintf(str_err, fmt, va);
        va_end(va);
    }

    Err(0, str_err);
    return;
}

int SCInitEntryWithFile(FILE* pFile)
{
    yyin = pFile;
    yysptr = yysbuf;
    yylineno = 1;

    SetFunction(input, input_file);
    StrErr[0] = 0;

    if (SCInitYacc(1) < 0)
        return(-1);

    return(0);
}

int SCInitEntryWithString(char* pentry)
{
    ind_entry = 0;
    entry = pentry;
    yysptr = yysbuf;
    yylineno = 1;

    SetFunction(input, input_string);
    StrErr[0] = 0;

    if (SCInitYacc(1) < 0)
        return(-1);

    return(0);
}

int SCCtrlTime(char* Buf)
{
    DWORD time, heure, minute, seconde;
    char timechar[7]; /* pour conversion HH:MM:SS -> HHMMSS */

    timechar[0] = Buf[0];
    timechar[1] = Buf[1];
    timechar[2] = Buf[3];
    timechar[3] = Buf[4];
    timechar[4] = Buf[6];
    timechar[5] = Buf[7];
    timechar[6] = 0;

    time = (DWORD) atol(timechar);

    heure   = time/10000;
    minute  = (time - heure*10000)/100;
    seconde = time - (heure*10000 + minute*100);

    if (heure<0 || heure>24)
        return(-1);

    if (minute<0 || minute>59)
        return(-1);

    if (seconde<0 || seconde>59)
        return(-1);

    return(0);
}

int SCCtrlDate(char* Buf)
{
    DWORD date, siecle, mois, jour;
    static short jours[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    char datechar[9];  /* pour conversion JJ/MM/AAAA -> AAAAMMJJ */
    ldiv_t  div_result;
   #define MINDATE 19010101
   #define MAXDATE 20501231

    datechar[0] = Buf[6];
    datechar[1] = Buf[7];
    datechar[2] = Buf[8];
    datechar[3] = Buf[9];
    datechar[4] = Buf[3];
    datechar[5] = Buf[4];
    datechar[6] = Buf[0];
    datechar[7] = Buf[1];
    datechar[8] = 0;

    date = (DWORD) atol(datechar);
    if (date < MINDATE || date > MAXDATE)
        return(-1);

    siecle = date/10000 ;
    mois   = (date - siecle*10000)/100;
    jour   = date - (siecle*10000 + mois*100);

    div_result = ldiv(siecle, 4);
    if (div_result.rem == 0) jours[2]=29;
    else jours[2]=28;

    if (mois<1 || mois>12)
        return(-1);

    if (jour<1 || jour>(DWORD)jours[mois])
        return(-1);

    return(0);
}
int yyvstop[] = {
                    0,

                    16,
                    0,

                    15,
                    16,
                    0,

                    15,
                    0,

                    16,
                    0,

                    16,
                    0,

                    13,
                    16,
                    0,

                    14,
                    16,
                    0,

                    16,
                    0,

                    4,
                    16,
                    0,

                    16,
                    0,

                    12,
                    16,
                    0,

                    12,
                    16,
                    0,

                    12,
                    16,
                    0,

                    12,
                    16,
                    0,

                    12,
                    16,
                    0,

                    2,
                    0,

                    3,
                    0,

                    4,
                    0,

                    4,
                    0,

                    5,
                    0,

                    1,
                    0,

                    1,
                    0,

                    12,
                    0,

                    12,
                    0,

                    12,
                    0,

                    12,
                    0,

                    12,
                    0,

                    4,
                    0,

                    6,
                    0,

                    4,
                    0,

                    12,
                    0,

                    12,
                    0,

                    12,
                    0,

                    6,
                    0,

                    4,
                    0,

                    12,
                    0,

                    7,
                    12,
                    0,

                    8,
                    12,
                    0,

                    6,
                    0,

                    6,
                    0,

                    9,
                    12,
                    0,

                    6,
                    0,

                    10,
                    0,

                    11,
                    0,
                    0};
# define YYTYPE unsigned char
struct yywork { YYTYPE verify, advance; } yycrank[] = {
            0,0,	0,0,	1,3,	0,0,
            0,0,	0,0,	0,0,	0,0,
            0,0,	0,0,	1,4,	1,5,
            0,0,	0,0,	1,4,	4,5,
            4,5,	0,0,	0,0,	4,5,
            0,0,	0,0,	0,0,	0,0,
            0,0,	0,0,	0,0,	0,0,
            0,0,	0,0,	0,0,	0,0,
            0,0,	0,0,	0,0,	1,6,
            0,0,	0,0,	4,5,	0,0,
            1,7,	1,8,	1,9,	2,8,
            2,9,	0,0,	1,10,	1,3,
            1,3,	1,11,	1,11,	53,57,
            0,0,	0,0,	0,0,	0,0,
            0,0,	0,0,	0,0,	54,58,
            1,12,	1,13,	1,3,	13,29,
            0,0,	0,0,	1,14,	1,14,
            15,31,	0,0,	1,14,	1,15,
            41,49,	47,55,	0,0,	0,0,
            0,0,	1,14,	31,39,	1,16,
            6,18,	32,40,	40,48,	1,14,
            1,14,	1,17,	1,14,	16,32,
            6,18,	6,18,	17,33,	33,41,
            6,18,	39,47,	0,0,	0,0,
            0,0,	50,43,	0,0,	0,0,
            0,0,	0,0,	1,14,	10,22,
            10,22,	10,22,	10,22,	10,22,
            10,22,	10,22,	10,22,	10,22,
            10,22,	6,19,	0,0,	0,0,
            0,0,	0,0,	6,18,	0,0,
            0,0,	0,0,	0,0,	0,0,
            6,18,	6,18,	6,18,	6,18,
            6,18,	50,43,	0,0,	0,0,
            0,0,	0,0,	0,0,	0,0,
            0,0,	0,0,	6,18,	6,18,
            6,18,	0,0,	0,0,	0,0,
            6,18,	6,18,	0,0,	0,0,
            6,18,	6,18,	0,0,	0,0,
            0,0,	0,0,	0,0,	6,18,
            0,0,	6,18,	7,20,	0,0,
            0,0,	6,18,	6,18,	6,18,
            6,18,	0,0,	7,20,	7,20,
            0,0,	0,0,	7,20,	0,0,
            0,0,	0,0,	0,0,	0,0,
            0,0,	0,0,	0,0,	11,23,
            6,18,	11,24,	11,24,	11,24,
            11,24,	11,24,	11,24,	11,24,
            11,24,	11,24,	11,24,	7,20,
            0,0,	0,0,	0,0,	0,0,
            7,21,	0,0,	0,0,	0,0,
            0,0,	0,0,	7,20,	7,20,
            7,20,	7,20,	7,20,	0,0,
            0,0,	11,25,	0,0,	0,0,
            0,0,	0,0,	0,0,	0,0,
            7,20,	7,20,	7,20,	0,0,
            0,0,	0,0,	7,20,	7,20,
            0,0,	0,0,	7,20,	7,20,
            0,0,	0,0,	0,0,	0,0,
            0,0,	7,20,	0,0,	7,20,
            12,26,	0,0,	0,0,	7,20,
            7,20,	7,20,	7,20,	0,0,
            12,26,	12,27,	0,0,	0,0,
            12,28,	0,0,	0,0,	0,0,
            0,0,	0,0,	0,0,	0,0,
            0,0,	0,0,	7,20,	22,34,
            22,34,	22,34,	22,34,	22,34,
            22,34,	22,34,	22,34,	22,34,
            22,34,	12,26,	0,0,	0,0,
            0,0,	0,0,	12,26,	0,0,
            0,0,	0,0,	0,0,	0,0,
            12,26,	12,26,	12,26,	12,28,
            12,26,	0,0,	0,0,	0,0,
            0,0,	0,0,	0,0,	0,0,
            0,0,	0,0,	12,26,	12,26,
            12,26,	0,0,	0,0,	0,0,
            12,26,	12,26,	0,0,	0,0,
            12,26,	12,26,	0,0,	0,0,
            0,0,	0,0,	0,0,	12,26,
            0,0,	12,26,	0,0,	0,0,
            0,0,	12,26,	12,26,	12,26,
            12,26,	14,30,	14,30,	14,30,
            14,30,	14,30,	14,30,	14,30,
            14,30,	14,30,	14,30,	0,0,
            0,0,	0,0,	0,0,	0,0,
            12,26,	0,0,	14,30,	14,30,
            14,30,	14,30,	14,30,	14,30,
            14,30,	14,30,	14,30,	14,30,
            14,30,	14,30,	14,30,	14,30,
            14,30,	14,30,	14,30,	14,30,
            14,30,	14,30,	14,30,	14,30,
            14,30,	14,30,	14,30,	14,30,
            0,0,	0,0,	0,0,	0,0,
            14,30,	0,0,	14,30,	14,30,
            14,30,	14,30,	14,30,	14,30,
            14,30,	14,30,	14,30,	14,30,
            14,30,	14,30,	14,30,	14,30,
            14,30,	14,30,	14,30,	14,30,
            14,30,	14,30,	14,30,	14,30,
            14,30,	14,30,	14,30,	14,30,
            23,35,	23,35,	23,35,	23,35,
            23,35,	23,35,	23,35,	23,35,
            23,35,	23,35,	24,23,	24,36,
            24,37,	24,37,	24,37,	24,37,
            24,37,	24,37,	24,37,	24,37,
            24,37,	24,37,	24,38,	34,37,
            34,37,	34,37,	34,37,	34,37,
            34,37,	34,37,	34,37,	34,37,
            34,37,	0,0,	0,0,	0,0,
            0,0,	0,0,	0,0,	0,0,
            24,25,	35,42,	35,42,	35,42,
            35,42,	35,42,	35,42,	35,42,
            35,42,	35,42,	35,42,	36,44,
            36,44,	36,44,	36,44,	36,44,
            36,44,	36,44,	36,44,	36,44,
            36,44,	0,0,	35,43,	37,45,
            37,45,	37,45,	37,45,	37,45,
            37,45,	37,45,	37,45,	37,45,
            37,45,	38,46,	38,46,	38,46,
            38,46,	38,46,	38,46,	38,46,
            38,46,	38,46,	38,46,	42,50,
            42,50,	42,50,	42,50,	42,50,
            42,50,	42,50,	42,50,	42,50,
            42,50,	43,51,	35,43,	0,0,
            43,52,	43,52,	43,52,	43,52,
            43,52,	43,52,	43,52,	43,52,
            43,52,	43,52,	44,53,	44,53,
            44,53,	44,53,	44,53,	44,53,
            44,53,	44,53,	44,53,	44,53,
            45,45,	45,45,	45,45,	45,45,
            45,45,	45,45,	45,45,	45,45,
            45,45,	45,45,	46,54,	46,54,
            46,54,	46,54,	46,54,	46,54,
            46,54,	46,54,	46,54,	46,54,
            0,0,	0,0,	0,0,	0,0,
            0,0,	0,0,	0,0,	0,0,
            45,25,	51,52,	51,52,	51,52,
            51,52,	51,52,	51,52,	51,52,
            51,52,	51,52,	51,52,	52,56,
            52,56,	52,56,	52,56,	52,56,
            52,56,	52,56,	52,56,	52,56,
            52,56,	57,59,	57,59,	57,59,
            57,59,	57,59,	57,59,	57,59,
            57,59,	57,59,	57,59,	58,60,
            58,60,	58,60,	58,60,	58,60,
            58,60,	58,60,	58,60,	58,60,
            58,60,	59,61,	59,61,	59,61,
            59,61,	59,61,	59,61,	59,61,
            59,61,	59,61,	59,61,	60,62,
            60,62,	60,62,	60,62,	60,62,
            60,62,	60,62,	60,62,	60,62,
            60,62,	61,63,	61,63,	61,63,
            61,63,	61,63,	61,63,	61,63,
            61,63,	61,63,	61,63,	63,64,
            63,64,	63,64,	63,64,	63,64,
            63,64,	63,64,	63,64,	63,64,
            63,64,	0,0};
struct yysvf yysvec[] = {
                            0,	0,	0,
                            yycrank+-1,	0,		0,
                            yycrank+-3,	yysvec+1,	0,
                            yycrank+0,	0,		yyvstop+1,
                            yycrank+6,	0,		yyvstop+3,
                            yycrank+0,	yysvec+4,	yyvstop+6,
                            yycrank+-79,	0,		yyvstop+8,
                            yycrank+-157,	0,		yyvstop+10,
                            yycrank+0,	0,		yyvstop+12,
                            yycrank+0,	0,		yyvstop+15,
                            yycrank+55,	0,		yyvstop+18,
                            yycrank+133,	0,		yyvstop+20,
                            yycrank+-235,	0,		yyvstop+23,
                            yycrank+2,	0,		yyvstop+25,
                            yycrank+273,	0,		yyvstop+28,
                            yycrank+3,	yysvec+14,	yyvstop+31,
                            yycrank+2,	yysvec+14,	yyvstop+34,
                            yycrank+8,	yysvec+14,	yyvstop+37,
                            yycrank+0,	yysvec+6,	0,
                            yycrank+0,	0,		yyvstop+40,
                            yycrank+0,	yysvec+7,	0,
                            yycrank+0,	0,		yyvstop+42,
                            yycrank+211,	yysvec+11,	yyvstop+44,
                            yycrank+348,	0,		0,
                            yycrank+360,	0,		yyvstop+46,
                            yycrank+0,	0,		yyvstop+48,
                            yycrank+0,	yysvec+12,	0,
                            yycrank+0,	0,		yyvstop+50,
                            yycrank+0,	yysvec+12,	yyvstop+52,
                            yycrank+0,	yysvec+13,	yyvstop+54,
                            yycrank+0,	yysvec+14,	yyvstop+56,
                            yycrank+2,	yysvec+14,	yyvstop+58,
                            yycrank+5,	yysvec+14,	yyvstop+60,
                            yycrank+6,	yysvec+14,	yyvstop+62,
                            yycrank+371,	yysvec+11,	yyvstop+64,
                            yycrank+389,	0,		yyvstop+66,
                            yycrank+399,	0,		0,
                            yycrank+411,	yysvec+11,	yyvstop+68,
                            yycrank+421,	0,		0,
                            yycrank+10,	yysvec+14,	yyvstop+70,
                            yycrank+6,	yysvec+14,	yyvstop+72,
                            yycrank+3,	yysvec+14,	yyvstop+74,
                            yycrank+431,	yysvec+35,	yyvstop+76,
                            yycrank+444,	0,		0,
                            yycrank+454,	0,		0,
                            yycrank+464,	0,		yyvstop+78,
                            yycrank+474,	0,		0,
                            yycrank+4,	yysvec+14,	yyvstop+80,
                            yycrank+0,	yysvec+14,	yyvstop+82,
                            yycrank+0,	yysvec+14,	yyvstop+85,
                            yycrank+28,	0,		yyvstop+88,
                            yycrank+493,	0,		0,
                            yycrank+503,	0,		yyvstop+90,
                            yycrank+4,	0,		0,
                            yycrank+1,	0,		0,
                            yycrank+0,	yysvec+14,	yyvstop+92,
                            yycrank+0,	0,		yyvstop+95,
                            yycrank+513,	0,		0,
                            yycrank+523,	0,		0,
                            yycrank+533,	0,		0,
                            yycrank+543,	0,		0,
                            yycrank+553,	0,		0,
                            yycrank+0,	0,		yyvstop+97,
                            yycrank+563,	0,		0,
                            yycrank+0,	0,		yyvstop+99,
                            0,	0,	0};
struct yywork *yytop = yycrank+620;
struct yysvf *yybgin = yysvec+1;
unsigned char yymatch[] = {
                              00  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
                              01  ,011 ,012 ,01  ,01  ,015 ,01  ,01  ,
                              01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
                              01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
                              011 ,01  ,'"' ,01  ,01  ,01  ,01  ,047 ,
                              01  ,01  ,01  ,01  ,01  ,'-' ,'.' ,'/' ,
                              '0' ,'1' ,'1' ,'1' ,'1' ,'1' ,'1' ,'1' ,
                              '1' ,'1' ,01  ,';' ,'<' ,'=' ,'<' ,01  ,
                              01  ,'A' ,'B' ,'B' ,'B' ,'E' ,'F' ,'B' ,
                              'B' ,'B' ,'B' ,'B' ,'L' ,'B' ,'N' ,'B' ,
                              'B' ,'B' ,'R' ,'S' ,'T' ,'U' ,'B' ,'B' ,
                              'B' ,'B' ,'B' ,01  ,01  ,01  ,01  ,'B' ,
                              01  ,'B' ,'B' ,'B' ,'B' ,'e' ,'B' ,'B' ,
                              'B' ,'B' ,'B' ,'B' ,'B' ,'B' ,'B' ,'B' ,
                              'B' ,'B' ,'B' ,'B' ,'B' ,'B' ,'B' ,'B' ,
                              'B' ,'B' ,'B' ,01  ,01  ,01  ,01  ,01  ,
                              01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
                              01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
                              01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
                              01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
                              01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
                              01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
                              01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
                              01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
                              01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
                              01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
                              01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
                              01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
                              01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
                              01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
                              01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
                              01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
                              0};
unsigned char yyextra[] = {
                              0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,
                              0};
/* @(#)44 1.9.1.2 com/lib/l/ncform, libl, bos320, 9240320 9/28/92 09:13:13";*/
/*
 * COMPONENT_NAME: (LIBL) Lex Libraries
 *
 * FUNCTIONS: yylook, yyhlook, yyback, yyinput, yyoutput, yyunput,
 *            yymbinput, yymboutput, yymbunput, yymbreturn
 *            yywinput, yywoutput, yywunput, yywreturn
 *
 * ORIGINS: 03
 *
*/

int yylineno =1;

# define YYU(x)         x
# define NLSTATE        yyprevious=YYNEWLINE

# define YYCCLDOT       0x0001

unsigned char   yytext[YYLMAX];
wchar_t         yywtext[YYLMAX];
wchar_t         yywbuf[YYLMAX];

unsigned char   yytbuf[YYLMAX];
unsigned char   *yytbp;
int             yytbi;
wchar_t         yytwc;

struct yysvf    *yylstate [YYLMAX], **yylsp, **yyolsp;
unsigned char   yysbuf[YYLMAX];
unsigned char   *yysptr = yysbuf;
int             *yyfnd;
extern struct yysvf *yyestate;
int             yyprevious = YYNEWLINE;

# ifdef LEXDEBUG
int             debug;
#ifdef __cplusplus
#ifdef _CPP_IOSTREAMS
void            allprint(wchar_t);
void            sprint  (wchar_t *);
#else
extern "C" void allprint(wchar_t);
extern "C" void sprint  (wchar_t *);
#endif
#endif
# endif

yylook()
{
    register struct yysvf *yystate, **lsp;
    register struct yywork *yyt;
    struct yysvf    *yyz;
    int             yywch, yyfirst, yyw;
    struct yywork   *yyr;
    wchar_t         *yywlastch;
    /*
     * start off machines
     */
# ifdef LEXDEBUG
    debug = 1;
# endif

    yyfirst=1;
    if (!yymorfg)
        yywlastch = yywtext;
    else
    {
        yymorfg=0;
        yywlastch = &(yywtext[yywleng]);
    }

    for(;;)
    {
        lsp = yylstate;
        yyestate = yystate = yybgin;
        if (yyprevious==YYNEWLINE)
            yystate++;
        for (;;)
        {
# ifdef LEXDEBUG
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
            if(debug) (*yyout) << "state " << yystate-yysvec-1 << endl;
#else
            if(debug) fprintf(yyout,"state %d\n",yystate-yysvec-1);
#endif /* #if defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
# endif
            yyt = yystate->yystoff;
            if(yyt == yycrank && !yyfirst)      /* may not be any transitions */
            {
                yyz = yystate->yyother;
                if(yyz == 0)
                    break;
                if(yyz->yystoff == yycrank)
                    break;
            }
            yywch = winput();
            *yywlastch++ = yywch;

            yyfirst=0;
tryagain:
# ifdef LEXDEBUG
            if(debug)
            {
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                (*yyout) << "char ";
                allprint (yywch);
                (*yyout) << endl;
#else
                fprintf(yyout,"char ");
                allprint(yywch);
                putchar('\n');
#endif /* #if defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
            }
# endif
            yyr = yyt;
            if ( (int)yyt > (int)yycrank)
            {
                if (yywch < NCH)                /* 8-bit */
                {
                    yyt = yyr + yywch;
                    if (yyt <= yytop && yyt->verify+yysvec == yystate)
                    {
                        if(yyt->advance+yysvec == YYLERR) /* error transitions */
                        {
                            wunput(*--yywlastch);
                            break;
                        }
                        *lsp++ = yystate = yyt->advance+yysvec;
                        goto contin;
                    }
                }
# if YYHSIZE
                else                            /* wide */
                {
                    if (yyw = yyhlook(yywch,yystate-yysvec))
                    {
                        if (yywcrank[yyw].wadvance+yysvec==YYLERR)
                        {
                            wunput(*--yywlastch);
                            break;
                        }
                        *lsp++ = yystate = yywcrank[yyw].wadvance+yysvec;
                        goto contin;
                    }
                }
# endif /*YYHSIZE*/
            }
# ifdef YYOPTIM
            else if ((int)yyt < (int)yycrank)   /* r < yycrank */
            {
                yyt = yyr = yycrank+(yycrank-yyt);
# ifdef LEXDEBUG
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                if(debug) (*yyout) << "compressed state" << endl;
#else
                if(debug) fprintf(yyout,"compressed state\n");
#endif /* #if defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
# endif
                if (yywch < NCH)                /* 8-bit */
                {
                    yyt = yyt + yywch;
                    if(yyt <= yytop && yyt->verify+yysvec == yystate)
                    {
                        if(yyt->advance+yysvec == YYLERR) /* error transitions */
                        {
                            wunput(*--yywlastch);
                            break;
                        }
                        *lsp++ = yystate = yyt->advance+yysvec;
                        goto contin;
                    }
                    yyt = yyr + YYU(SCGiveChar(yymatch[yywch]));
# ifdef LEXDEBUG
                    if(debug)
                    {
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                        (*yyout) << "try fall back character ";
                        allprint(YYU(yymatch[yywch]));
                        (*yyout) << endl;
#else
                        fprintf(yyout,"try fall back character ");
                        allprint(YYU(yymatch[yywch]));
                        putchar('\n');
#endif /* #if defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
                    }
# endif
                    if(yyt <= yytop && yyt->verify+yysvec == yystate)
                    {
                        if(yyt->advance+yysvec == YYLERR) /* error transition */
                        {
                            wunput(*--yywlastch);
                            break;
                        }
                        *lsp++ = yystate = yyt->advance+yysvec;
                        goto contin;
                    }
                }
# if YYHSIZE
                else                            /* wide */
                {
                    if (yyw = yyhlook(yywch,yystate-yysvec))
                    {
                        if (yywcrank[yyw].wadvance+yysvec==YYLERR)
                        {
                            wunput(*--yywlastch);
                            break;
                        }
                        *lsp++ = yystate = yywcrank[yyw].wadvance+yysvec;
                        goto contin;
                    }
                    if (yyw = yymlook(yywch))   /* find the target char */
                    {
                        if (yyw = yyhlook(yyw,yystate-yysvec))
                        {
                            if (yywcrank[yyw].wadvance+yysvec == YYLERR)
                            {
                                wunput(*--yywlastch);
                                break;
                            }
                            *lsp++ = yystate = yywcrank[yyw].wadvance+yysvec;
                            goto contin;
                        }
                    }
                }
# endif /*YYHSIZE*/
# if YYXSIZE
# ifdef MFDEBUG
                if (debug)
                {
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                    (*yyout) << "try extended ccl on char ";
                    allprint(YYU(yywch));
                    (*yyout) << '\n';
#else
                    fprintf (yyout, "try extended ccl on char ");
                    allprint(YYU(yywch));
                    putchar('\n');
#endif /* #if defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
                }
# endif
                yyw = yyxlook(yywch,yystate-yysvec);
                if (yyw != -1)
                {
                    if (yyxccl[yyw].advance+yysvec == YYLERR)
                    {
                        wunput(*--yywlastch);
                        break;
                    }
                    *lsp++ = yystate = yyxccl[yyw].advance+yysvec;
                    goto contin;
                }
# endif /*YYXSIZE*/
            } /* r < yycrank */
            if ((yystate = yystate->yyother) && (yyt= yystate->yystoff) != yycrank)
            {
# ifdef LEXDEBUG
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                if(debug) (*yyout) << "fall back to state "
                    << yystate-yysvec-1 << endl;
#else
                if(debug) fprintf(yyout,"fall back to state %d\n",yystate-yysvec-1);
#endif /* #if defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
# endif
                goto tryagain;
            }
# endif /*YYOPTIM*/
            else
            {
                wunput(*--yywlastch);
                break;
            }
contin:
# ifdef LEXDEBUG
            if(debug)
            {
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                (*yyout) << "state " << yystate-yysvec-1 << " char ";
                allprint(yywch);
                (*yyout) << endl;
#else
                fprintf(yyout,"state %d char ",yystate-yysvec-1);
                allprint(yywch);
                putchar('\n');
#endif /* #if defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
            }
# endif
            ; /* contin: */
        } /* for(;;) */
# ifdef LEXDEBUG
        if(debug)
        {
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
            (*yyout) << "stopped at " << (int) (*(lsp-1)-yysvec-1) << " with ";
            allprint(yywch);
            (*yyout) << endl;
#else
            fprintf(yyout,"stopped at %d with ",*(lsp-1)-yysvec-1);
            allprint(yywch);
            putchar('\n');
#endif /* #if defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
        }
# endif
        while (lsp-- > yylstate)
        {
            *yywlastch-- = 0;
            if (*lsp != 0 && (yyfnd= (*lsp)->yystops) && *yyfnd > 0)
            {
                yyolsp = lsp;
                if(yyextra[*yyfnd])             /* must backup */
                {
                    while(yyback((*lsp)->yystops,-*yyfnd) != 1 && lsp > yylstate)
                    {
                        lsp--;
                        wunput(*yywlastch--);
                    }
                }
                yyprevious = YYU(*yywlastch);
                yylsp = lsp;
                yywleng = yywlastch-yywtext+1;
                yywtext[yywleng] = 0;
# ifdef LEXDEBUG
                if(debug)
                {
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
                    (*yyout) << "\nmatch ";
                    sprint(yywtext);
                    (*yyout) << " action " << *yyfnd << endl;
#else
                    fprintf(yyout,"\nmatch ");
                    sprint(yywtext);
                    fprintf(yyout," action %d\n",*yyfnd);
#endif /* #if defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
                }
# endif

                wreturn(*yyfnd++);
            }
            wunput(*yywlastch);
        }
        if (yywtext[0] == 0  /* && feof(yyin) */)
        {
            yysptr=yysbuf;
            wreturn(0);
        }
        yyprevious = yywtext[0] = winput();
        if (yyprevious>0)
            woutput(yyprevious);
        yywlastch=yywtext;
# ifdef LEXDEBUG
#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
        if(debug) cout << endl;
#else
        if(debug) putchar('\n');
#endif /* #if defined(__cplusplus) && defined(_CPP_IOSTREAMS) */
# endif
    } /* for (;;) */
    wreturn(0);
}
int yyback(int *p, int m)
{
    if (p==0) return(0);
    while (*p)
    {
        if (*p++ == m)
            return(1);
    }
    return(0);
}
# if YYHSIZE
/*
 * wide chars are hashed in yywcrank, return the index of the found element, or
 * zero
 */
int yyhlook(int c, int v)
{
    int             n;

    n = (c % (YYHSIZE - 1)) + 1;
    for (;;)
    {
        if ((yywcrank[n].wch==c)&&(yywcrank[n].wverify==v))
            return(n);
        n = yywcrank[n].wnext;
        if (n==0)
            return(0);
    }
}
# endif /*YYHSIZE*/
# if YYHSIZE
/*
 * wide CCL chars are hashed in yywmatch, return the character to map to or
 * zero if there are none;  each char occurs only once in the match table.
 */
int
yymlook(int c)
{
# if YYMSIZE
    int             n;

    n = (c % (YYMSIZE - 1)) + 1;

    while (n!=0)
    {
        if (yywmatch[n].wch == c)
            return(yywmatch[n].wmatch);
        n = yywmatch[n].wnext;
    }
# endif /*YYMSIZE*/

    return(0);
}
# endif /*YYHSIZE*/
# if YYXSIZE
/*
 * general CCLs are described in yyxccl; return the index of the first
 * acceptable CCL that matches verification, -1 if none found.
 */
int
yyxlook (int c, int v)
{
    register int i;

    if (!c)                                     /* c==0 => EOF */
        return (-1);

    for (i = 0; (i < YYXSIZE) && (yyxccl[i].verify < v); i++)
        ; /*Empty*/

    for ( ; (i < YYXSIZE) && (yyxccl[i].verify == v); i++)
    {
        switch (yyxccl[i].type)
        {
        case YYCCLDOT:  return((c=='\n')?-1:i);
        }
    }
    return (-1);
}
#endif
/*
 * The following are the multi-byte renditions of input, unput, and
 * output.  They are referenced through the winput, wunput, and woutput macros
 * resepectively.
 *
 * A (-1) is returned if the character input is invalid.
 */
#ifdef __cplusplus
#include <mbstr.h>
#endif /*  __cplusplus */
int yywinput()
{
#define mbsinvalid(pt) 1
    yytbi=0;
    do
    {
        yytbuf[yytbi++]=input();
        yytbuf[yytbi]=0;
    } while(mbsinvalid((const char *) yytbuf)&&(yytbi<yymbcurmax));

    if (mbtowc(&yytwc,(const char *)yytbuf,yymbcurmax)<0)
        return (-1);

    return ((int)yytwc);
}

void yywoutput(int c)
{
    int result;
 #if defined (_VM_) || defined(AS400)
    result = 1;
 #else
    result = wctomb((char*)yytbuf, (wchar_t)c);
 #endif
    for (yytbi = result, yytbp = yytbuf; yytbi > 0; yytbi--)
        output(*yytbp++);
}
void yywunput(int c)
{
    int result;
 #if defined (_VM_) || defined(AS400)
    result = 1;
 #else
    result = wctomb((char*)yytbuf, (wchar_t)c);
 #endif
    for (yytbi = result - 1; yytbi >= 0; yytbi--)
        unput(yytbuf[yytbi]);
}
/*
 * Convert yywtext to yytext prior to returning from yylook.  It is referenced
 * through the wreturn macro.
 */
int yywreturn(int r)
             {
 #if defined (_VM_) || defined(AS400)
                 int i;
                 for (i=0; i < YYLMAX; i++)
                     yytext[i] = yywtext[i];
 #else
                 yyleng = wcstombs((char*)yytext, yywtext, YYLMAX);
                 if (yyleng < 0)
                 {  yyleng = 0;
                     yytext[0] = 0;
                 }
 #endif
                 return(r);
             }
             /*
              * the following are only used in the lex library
              */
             int yyinput()
             {
                 return(input());
             }
             void yyoutput(int c)
             {
                 output(c);
             }
             void yyunput(int c)
             {
                 unput(c);
             }
             int yymbinput()
             {
                 return(winput());
             }
             void yymboutput(int c)
             {
                 woutput(c);
             }
             void yymbunput(int c)
             {
                 wunput(c);
             }
             int yymbreturn(int x)
                           {
                               wreturn (x);
                           }
