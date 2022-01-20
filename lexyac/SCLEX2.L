/***************************************************************/
/* Ces modifications sont a apporter au source SCLEX.C         */
/*                                                             */
/* existant : int yymorfg;                                     */
/* modif    : int yymorfg = 0;                                 */
/*                                                             */
/* existant : if(yymbcurmax<=0) yymbcurmax=MB_CUR_MAX;         */
/* modif    : if(yymbcurmax<=0) yymbcurmax = 1;                */
/*                                                             */
/* existant : yyt = yyr + YYU(yymatch[yywch]);                 */
/* modif    : yyt = yyr + YYU(SCGiveChar(yymatch[yywch]));     */
/*                                                             */
/* dans la fonction yywinput() ajouter :                       */
/*            #define mbsinvalid(pt)  1                        */
/*                                                             */
/* la fonction yywoutput(int c) devient :                      */
/* void yywoutput(int c)                                       */
/* {                                                           */
/*    int result;                                              */
/* #ifdef _VM_                                                 */
/*    result = 1;                                              */
/* #else                                                       */
/*    result = wctomb((char*)yytbuf, (wchar_t)c);              */
/* #endif                                                      */
/*    for (yytbi = result, yytbp = yytbuf; yytbi > 0; yytbi--) */
/*      output(*yytbp++);                                      */
/* }                                                           */
/*                                                             */
/* la fonction yywunput(int c) devient :                       */
/* void yywunput(int c)                                        */
/* {                                                           */
/*    int result;                                              */
/* #ifdef _VM_                                                 */
/*    result = 1;                                              */
/* #else                                                       */
/*    result = wctomb((char*)yytbuf, (wchar_t)c);              */
/* #endif                                                      */
/*    for (yytbi = result - 1; yytbi >= 0; yytbi--)            */
/*      unput(yytbuf[yytbi]);                                  */
/* }                                                           */
/*                                                             */
/* la fonction yywreturn(int r) devient :                      */
/* int yywreturn(int r)                                        */
/* {                                                           */
/* #ifdef _VM_                                                 */
/*    int i;                                                   */
/*    for (i=0; i < YYLMAX; i++)                               */
/*       yytext[i] = yywtext[i];                               */
/* #else                                                       */
/*    yyleng = wcstombs((char*)yytext, yywtext, YYLMAX);       */
/*    if (yyleng < 0)                                          */
/*    {  yyleng = 0;                                           */
/*       yytext[0] = 0;                                        */
/*    }                                                        */
/* #endif                                                      */
/*    return(r);                                               */
/* }                                                           */
/*                                                             */
/* ajout du paramètres SC* psc à la fonction yylex()           */
/*         yylex(SC* psc)                                      */
/* et de l'affectation GlobalSC = psc au début de yylex()      */
/***************************************************************/


/* definition section */

sign   [-]
exp    [eE]
dec    [0-9]
var1   [a-zA-Z_][a-zA-Z0-9_]*
var2   [<>][=]*
com    [;].*([\n]|[\r]|[0])
string ["][^"]*["]
char   ['][^']*[']
null   [N][U][L][L]
true   [T][R][U][E]
false  [F][A][L][S][E]

dd     ({dec}{2})
dddd   ({dec}{4})
int    {sign}?{dec}+
long   {sign}?{dec}+[L]
float  ({sign}?({dec}{1,3})[.]({dec}{1,3})({exp}{sign}?({dec}{1,2}))?)
time   {dd}:{dd}:{dd}
date   {dd}[/]{dd}[/]{dddd}
var    {var1}|{var2} 

/* declaration section, external to the lexical analyser */
%{
#include <stdio.h>
#include <stdarg.h>
#include "sclex.h"

#undef  YY_INTERACTIVE
#define YY_INTERACTIVE 0

#undef input
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
%}

/* translation section */
%%
{com}                { Trace("--- Lex --- commentaire"); }
{string}             {
                       LONG from = 0;
                      XFStringRead(psc->DG->BC->Stx , buf, &from, (char*)yytextt, 1);
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
{char}               {
                       LONG from = 0;
                      XFStringRead(psc->DG->BC->Stx , buf, &from, (char*)yytextt, 1);
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
{int}                {
                       LONG from = 0;
                      XFStringRead(psc->DG->BC->Stx , buf, &from, (char*)yytextt, 1);
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
{long}               {
                       LONG from = 0;
                      XFStringRead(psc->DG->BC->Stx , buf, &from, (char*)yytext, 1);
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
{float}              {
                       LONG from = 0;
                      XFStringRead(psc->DG->BC->Stx , buf, &from, (char*)yytext, 1);
                       yylval.val_f = atof(buf);
                       sprintf(str_tmp,"%3.3f",yylval.val_f);
                       Trace("--- Lex --- yytext (%s) val_f (%3.3f) str_tmp (%s)", buf, yylval.val_f, str_tmp);
                       if (strcmp(buf, str_tmp) != 0)
                       {
                         Err(ERR_FLOAT, NULL);
                         return ERR_FLOAT;
                       }
                       else
                       {
                         yylval.str = buf;

                         if (InStringRemote) sprintf(StringRemote + strlen(StringRemote)," %s", buf);
                         return SCFLOAT;
                       }
                     }
{null}               {
                       LONG from = 0;
                      XFStringRead(psc->DG->BC->Stx , buf, &from, (char*)yytextt, 1);
                       yylval.str = 0;
                       if (InStringRemote) sprintf(StringRemote + strlen(StringRemote)," %s", buf);
                       return SCNULL;
                     }
{true}               {
                       LONG from = 0;
                      XFStringRead(psc->DG->BC->Stx , buf, &from, (char*)yytextt, 1);
                       yylval.val_i = 1;
                       if (InStringRemote) sprintf(StringRemote + strlen(StringRemote)," %s", buf);
                       return SCBOOLEAN;
                     }
{false}              {
                       LONG from = 0;
                      XFStringRead(psc->DG->BC->Stx , buf, &from, (char*)yytextt, 1);
                       yylval.val_i = 0;
                       if (InStringRemote) sprintf(StringRemote + strlen(StringRemote)," %s", buf);
                       return SCBOOLEAN;
                     }
{time}               {
                       LONG from = 0;
                      XFStringRead(psc->DG->BC->Stx , buf, &from, (char*)yytextt, 1);
                       if (SCCtrlTime(buf) < 0)
                       { Err(ERR_TIME, NULL); return ERR_TIME;}
                       Trace("--- Lex --- time(%s)", buf);
                       yylval.str = buf;
                       if (InStringRemote) sprintf(StringRemote + strlen(StringRemote)," %s", buf);
                       return SCTIME;
                     }
{date}               {
                       LONG from = 0;
                      XFStringRead(psc->DG->BC->Stx , buf, &from, (char*)yytextt, 1);
                       if (SCCtrlDate(buf) < 0)
                       { Err(ERR_DATE, NULL); return ERR_DATE;}
                       Trace("--- Lex --- date(%s)", buf);
                       yylval.str = buf;
                       if (InStringRemote) sprintf(StringRemote + strlen(StringRemote)," %s", buf);
                       return SCDATE;
                     }
{var}                {
                       LONG from = 0;
                      XFStringRead(psc->DG->BC->Stx , buf, &from, (char*)yytextt, 1);
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

"("                  { if (InStringRemote) strcat(StringRemote, "(");
                       return(PAR_DEB);
                     }
")"                  { if (InStringRemote) strcat(StringRemote, ")");
                       return(PAR_FIN);
                     }
[ \n\t\r]+           ;
.                    { Err(ERR_CHAR, NULL); return(ERR_CHAR);}

%%
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
  XFCharWrite (GlobalSC->DG->BC->Stx, &car, &pos, &lpbuf, 1);
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
	 sprintf(StrErr, "[File : %s][line : %d] Erreur (%d) str (%s)", GlobalSC->CurrentFileName, yylineno, IdErr, str_err);

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

   printf("%s\n", str_msg);
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
