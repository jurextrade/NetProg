/***************************************************************/
/* Ces modifications sont a apporter au source OSRESLEX.C      */
/*                                                             */
/* existant : int yymorfg;                                     */
/* modif    : int yymorfg = 0;                                 */
/*                                                             */
/* existant : if(yymbcurmax<=0) yymbcurmax=MB_CUR_MAX;         */
/* modif    : if(yymbcurmax<=0) yymbcurmax = 1;                */
/*                                                             */
/* existant : yyt = yyr + YYU(yymatch[yywch]);                 */
/* modif    : yyt = yyr + YYU(OSGiveChar(yymatch[yywch]));     */
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
/* ajout du paramètres XF* pxf à la fonction yylex()           */
/*         yylex(XF* pxf)                                      */
/* et de l'affectation GlobalStx = pxf au début de yylex()     */
/*                                                             */
/*     transformation des variables et des fonctions de type   */
/* yy$var et YY$var en respectivement zz$var et ZZ$var         */
/*                                                             */
/***************************************************************/

%o40000
%a26000
%k3000
%n1000
%p5000
%e2000
%{
#include <stdio.h>
#include <stdarg.h>
#include "osreslex.h"

#undef input
#define input OSSourceFile
#undef YYLMAX
#define YYLMAX 256 

char typbuf[256];
char varbuf[256];
char Name[120];
char PrecName[120];
char *varptr;
int Line = 0;

char OSStrErr[150];

ListOfFile PtrTab;
FILEID *FileStr;

XF *GlobalStx;

static int yywrap(void);
static char buf[YYLMAX];
%}


/* Les variables */
com         [;].*[\n]
            
digit       [0-9]+
var         [a-zA-Z][.a-zA-Z0-9_-]*
bigvar      [\*].*[\*]
comment     [$].*[\n]
onelocalvar {bigvar}
localvar    {bigvar}
anum        {digit}[.]{digit}[.]{digit}[.]{digit}
file        [/a-zA-Z0-9%][ a-zA-Z0-9[/_:.\\#>%-]*
strfiles    ["]{file}["]
nfile       [[]{file}[]]
dimdig      [[]{digit}[]]
            
hex         [0-9a-fA-F]
linehex     ({hex}{32})


dialogclass [D][I][A][L][O][G][C][L][A][S][S]
messageclass [M][E][S][S][A][G][E][C][L][A][S][S]
type1       [S][T][R][I][N][G]|[W][O][R][D]|[D][W][O][R][D]|[F][I][L][E]
type2       [C][H][A][R]|[B][Y][T][E]|[S][H][O][R][T]|[B][U][F][F][E][R]
type3       [L][O][N][G]|[D][O][U][B][L][E]
type        {type1}|{type2}|{type3}
length      [L][E][N][G][T][H]


idp1 [I][C][M][P]|[D][G]|[T][P]|[U][D][P]|[T][C][P]|[I][P]|[F][T][P]|[P][O][P]|[S][M][T][P]|[D][L]|[T][E][L][N][E][T]
idp2 [S][T][O][R][E]|[F][I][X][E][D]|[O][D][B][C]|[H][T][T][P]|[O][R][A][C][L][E]|
idp  {idp1}|{idp2}


queue       [Q][U][E][U][E]
queueclass  [Q][U][E][U][E][C][L][A][S][S]

xltclass    [X][L][T][C][L][A][S][S]
from        [F][R][O][M]
to          [T][O]
tabxlt      [T][A][B][X][L][T]
readwrite   [R][E][A][D][W][R][I][T][E]
la          [L][A]

appliclass       [A][P][P][L][I][C][A][T][I][O][N][C][L][A][S][S]
databaseclass    [D][A][T][A][B][A][S][E][C][L][A][S][S]
journalclass     [J][O][U][R][N][A][L][C][L][A][S][S]
connectionclass  [C][O][N][N][E][C][T][I][O][N][C][L][A][S][S]


application      [A][P][P][L][I][C][A][T][I][O][N]
database         [D][A][T][A][B][A][S][E]
journal          [J][O][U][R][N][A][L]
connection       [C][O][N][N][E][C][T][I][O][N]
message     [M][E][S][S][A][G][E]


apptype     [T][Y][P][E]
restype     [C][L][I][E][N][T]|[S][E][R][V][E][R]
port        [P][O][R][T]
protocol    [P][R][O][T][O][C][O][L]
transfert   [T][R][A][N][S][F][E][R][F][O][R][M][A][T]
table       [T][A][B][L][E]
scenario    [S][C][E][N][A][R][I][O]
site        [S][I][T][E]
machine     [M][A][C][H][I][N][E]
ip          [I][P]
ipname      [I][P][N][A][M][E]
system      [S][Y][S][T][E][M]
ressystem   [W][I][N][3][2]|[U][N][I][X]|[V][M]|[A][S][4][0][0]|[V][O][S]|[H][P][-][U][X]|[O][S][2]

repertory        [R][E][P][E][R][T][O][R][Y] 
execrepertory    [E][X][E][C][R][E][P][E][R][T][O][R][Y] 
localrepertory   [L][O][C][A][L][R][E][P][E][R][T][O][R][Y] 
receiverepertory [R][E][C][E][I][V][E][R][E][P][E][R][T][O][R][Y] 
sendrepertory    [S][E][N][D][R][E][P][E][R][T][O][R][Y] 
execcommand      [E][X][E][C][C][O][M][M][A][N][D]
execparameters   [E][X][E][C][P][A][R][A][M][E][T][E][R][S]
folder           [F][O][L][D][E][R]
extension        [E][X][T][E][N][S][I][O][N]
pfile            [F][I][L][E][N][A][M][E]
mode             [M][O][D][E]         


remote           [R][E][M][O][T][E]
fport            [F][P][O][R][T]
description      [D][E][S][C][R][I][P][T][I][O][N]
color            [C][O][L][O][R]
class            [C][L][A][S][S]
localisation     [L][O][C][A][L][I][S][A][T][I][O][N]


subtype    [S][U][B][T][Y][P][E]
properties [P][R][O][P][E][R][T][I][E][S]
pfiletype  [S][O][U][R][C][E]|[M][A][K][E]|[I][N][C][L][U][D][E]|[S][C][R][I][P][T]|[O][T][H][E][R]

pprocedure [P][R][O][C][E][D][U][R][E]
eventsr    [O][N][S][E][N][D]|[O][N][R][E][C][V]
event      [O][N][C][L][O][S][E]|[O][N][C][O][N][N][E][C][T]|[O][N][B][R][O][K][E][N]|[O][N][A][C][K]
context    [C]|[S][C]

onmachine  [O][N][M][A][C][H][I][N][E]
basename   [B][A][S][E][N][A][M][E]
username   [U][S][E][R][N][A][M][E]
password   [P][A][S][S][W][O][R][D]
fhostname  [F][H][O][S][T][N][A][M][E]
fusername  [F][U][S][E][R][N][A][M][E]
fpassword  [F][P][A][S][S][W][O][R][D]
fport      [F][P][O][R][T]         
logontype  [L][O][G][O][N][T][Y][P][E]                
transtype  [T][R][A][N][S][T][Y][P][E]                 
connected  [C][O][N][N][E][C][T][E][D]
account    [A][C][C][O][U][N][T]

pass       [A][C][T][I][V][E]|[P][A][S][S][I][V][E]
yesno      [Y][E][S]|[N][O]

deb       [B][E][G][I][N]
fin       [E][N][D]
ouvre     [(]
ferme     [)]
equal     [=]
retour    [ \t\n]


%%

{retour} ;

{com}     ;
{queue}            {  return (QUEUE);           }
{application}      {  return (APPLICATION);     }
{length}           {  return (LENGTH);          }
{extension}        {  return (EXTENSION);       }
{class}            {  return (CLASS);   }
{localisation}     {  return (LOCALISATION);    }
{execrepertory}    {  return (EXECREPERTORY);   }
{localrepertory}   {  return (LOCALREPERTORY);  }
{receiverepertory} {  return (RECEIVEREPERTORY);}
{sendrepertory}    {  return (SENDREPERTORY);   }
{execcommand}      {  return (EXECCOMMAND);     }
{execparameters}   {  return (EXECPARAMETERS);  } 
{subtype}          {  return (SUBTYPE);         }
{remote}           {  return (REMOTE);          }
{onmachine}        {  return (ONMACHINE);       }
{username}         {  return (USERNAME);        }
{basename}         {  return (BASENAME);        }
{password}         {  return (PASSWORD);        }
{fhostname}        {  return (FHOSTNAME);       }
{fusername}        {  return (FUSERNAME);       }
{fpassword}        {  return (FPASSWORD);       }
{fport}            {  return (FPORT);           }
{logontype}        {  return (LOGONTYPE);       }
{transtype}        {  return (TRANSTYPE);       }
{repertory}        {  return (REPERTORY);       }
{connected}        {  return (CONNECTED);       }
{folder}           {  return (FOLDER);          }
{pfile}            {  return (PFILE);           }
{equal}            {  return (PEQUAL);          }
{color}            {  return (COLOR);           }
{pprocedure}       {  return (PPROCEDURE);      }
{properties}       {  return (PROPERTIES);      }
{dialogclass}      {  return (DIALOGCLASS);     }
{message}          {  return (MESSAGE);         }
{messageclass}     {  return (MESSAGECLASS);    }
{xltclass}         {  return (XLTCLASS);        }
{site}             {  return (SITE);            }
{machine}          {  return (MACHINE);         }
{connection}       {  return (CONNECTION);      }
{database}         {  return (DATABASE);        }
{journal}          {  return (JOURNAL);         }
{connectionclass}  {  return (CONNECTIONCLASS); }
{appliclass}       {  return (APPLICATIONCLASS);}
{databaseclass}    {  return (DATABASECLASS);   }
{journalclass}     {  return (JOURNALCLASS);    }
{queueclass}       {  return (QUEUECLASS);      }
{la}               {  return (LA);              }
{system}           {  return (SYSTEM);          }
{mode}             {  return (MODE);            }
{port}             {  return (PORT);            }
{table}            {  return (TABLE);           }
{tabxlt}           {  return (TABXLT);          }
{from}             {  return (FROM);            }
{to}               {  return (TO);              }
{transfert}        {  return (TRANSFERT);       }
{protocol}         {  return (PROTOCOL);        }
{ip}               {  return (IPADDRESS);       }
{ipname}           {  return (IPNAME);          }
{description}      {  return (DESCRIPTION);     }
{apptype}          {  return (APPTYPE);         }
{deb}              {  return (DEB);             }
{fin}              {  return (FIN);             }
{ouvre}            {  return (OUVRE);           }
{ferme}            {  return (FERME);           }
{scenario}         {  return (SCENARIO);        }
{pass}        {  LONG from = 0;                                     
                 XFStringRead(pxf, typbuf, &from, (char*)yytext, 1);
                 yylval.Valtyp = typbuf;                            
                 return (PASS);     }

{yesno}       {  LONG from = 0;                                     
                 XFStringRead(pxf, typbuf, &from, (char*)yytext, 1);
                 yylval.Valtyp = typbuf;                            
                 return (YESNO);     }
{event}       { LONG from = 0;
                XFStringRead(pxf, typbuf, &from, (char*)yytext, 1);
                yylval.Valtyp = typbuf;
                return(EVENT);         }
{eventsr}     { LONG from = 0;
                XFStringRead(pxf, typbuf, &from, (char*)yytext, 1);
                yylval.Valtyp = typbuf;
                return(EVENTSR);       }
{context}     { LONG from = 0;
                XFStringRead(pxf, typbuf, &from, (char*)yytext, 1);
                yylval.Valtyp = typbuf;
                return(PGCONTEXT);     }
{pfiletype}   { LONG from = 0;
                XFStringRead(pxf, typbuf, &from, (char*)yytext, 1);
                yylval.Valtyp = typbuf;
                return(PFILETYPE);     }
{type}        { LONG from = 0;
                XFStringRead(pxf, typbuf, &from, (char*)yytext, 1);
                yylval.Valtyp = typbuf;
                return(TYPE); }
{restype}     { LONG from = 0;
                XFStringRead(pxf, typbuf, &from, (char*)yytext, 1);
                yylval.Valtyp = typbuf;
                return(RESTYPE);       }
{ressystem}   { LONG from = 0;
                XFStringRead(pxf, typbuf, &from, (char*)yytext, 1);
                yylval.Valtyp = typbuf;
                return(RESSYSTEM);     }
{idp}         {  LONG from = 0;
                 XFStringRead(pxf, varbuf, &from, (char*)yytext, 1);
                 yylval.Valvar = varbuf;
                 return(IDP);          }
{strfiles}    {  LONG from = 0;
                 XFStringRead(pxf, varbuf, &from, (char*)yytext, 1);
                 if (PtrTab.NbFile == NBFILMAX)
                 return(MAXREACH);
                 PtrTab.LineNumber[PtrTab.NbFile] = zzlineno;
                 varptr = strtok(varbuf, "\"");
                 strcpy((char *)PtrTab.FileName[PtrTab.NbFile], varptr);
                 PtrTab.Passed[PtrTab.NbFile] = 0;
                 PtrTab.NbFile++;
                 return(STRFILES);     }
{dimdig}      {  LONG from = 0;
                 XFStringRead(pxf, varbuf, &from, (char*)yytext, 1);
                 varptr = strtok(varbuf, "][");
                 yylval.Valint = atoi(varptr);
                 return(DIMDIGIT);     }
{linehex}     {  LONG from = 0;
                 XFStringRead(pxf, varbuf, &from, (char*)yytext, 1);
                 yylval.Valvar = varbuf;
                 return(LINEHEX);      }
{anum}        {  LONG from = 0;
                 XFStringRead(pxf, varbuf, &from, (char*)yytext, 1);
                 yylval.Valvar = varbuf;
                 return(ANUM); }
{comment}     {  LONG from = 0;
                 XFStringRead(pxf, varbuf, &from, (char*)yytext, 1);
                 yylval.Valvar = varbuf;
                 return(COMMENT); }
{digit}       {  LONG from = 0;
                 XFStringRead(pxf, buf, &from, (char*)yytext, 1);
                 yylval.Valint = atoi(buf);
                 return(DIGIT); }
{var}         {  LONG from = 0;
                 XFStringRead(pxf, varbuf, &from, (char*)yytext, 1);
                 yylval.Valvar = varbuf;
                 return(VAR); }
{bigvar}      {  LONG from = 0;
                 XFStringRead(pxf, varbuf, &from, (char*)yytext, 1);
                 yylval.Valvar = varbuf;
                 return(BIGVAR); }

{localvar}     { LONG from = 0;
                 XFStringRead(pxf, varbuf, &from, (char*)yytext, 1);
                 yylval.Valvar = varbuf;
                 return(LOCALVAR); }

{nfile}       {  LONG from = 0;
                 XFStringRead(pxf, varbuf, &from, (char*)yytext, 1);
                 yylval.Valvar = varbuf;
                 return(NFILE); }
.             {  return(CHARERROR); }
%%

static int yywrap(void)
{
  int i;

  if (FileStr)
  {
     OSCloseFile(FileStr);
     FileStr = NULL;
  }

  for (i=0 ; i<PtrTab.NbFile ; i++)
  {
    if (PtrTab.Passed[i] == 0)
    {
        PtrTab.Passed[i] = 1;
        if (i > 0) 
         strcpy(PrecName, (char*)PtrTab.FileName[i-1]);
        else
         strcpy(PrecName, (char*)PtrTab.FileName[i]);
        Line = PtrTab.LineNumber[i];
        if (!OSInitResourceFile((char *)PtrTab.FileName[i]))
        {
            ErrorOccurs(OPEN);
            return(1);
        }
        return(0);
    } 
  }
  return(1);
}

int OSInitResourceFile(char *FileName)
{
   int err;
  
   strcpy(Name, FileName);
   if (!InitGlob()) { ErrorOccurs(ALLOCATION); return 0; }
  
   FileStr = OSOpenFile(FileName, "r", 't', LOCDISQU, &err);
   if (!FileStr) { ErrorOccurs(OPEN); return 0; } 
  
  
   PtrTab.LineNumber[PtrTab.NbFile] = 0;
   varptr = strtok(varbuf, "\"");
   strcpy((char *)PtrTab.FileName[PtrTab.NbFile], Name);
  
   PtrTab.Passed[PtrTab.NbFile] = 1;
   PtrTab.NbFile++;
  
   zzlineno = 1;
   zzin = FileStr->File;
  
   return(1);
}

int OSInitCurrentResourceFile(char *FileName)
{
  int err;

  strcpy(Name, FileName);

  FileStr = OSOpenFile(FileName, "r", 't', LOCDISQU, &err);
  if (!FileStr) return 0;

  zzlineno = 1;
  zzin = FileStr->File;

  return(1);
}

void OSEndResourceFile()
{
  int i;
  
  for (i=0 ; i<PtrTab.NbFile ; i++)
  {
      PtrTab.Passed[i] = 0;
      PtrTab.LineNumber[i] = 0;
      memset((char *)PtrTab.FileName[i], '\0', 120);
  }
  Line = 0;
  PtrTab.NbFile = 0;
  
  if (FileStr)
  {
     OSCloseFile(FileStr);
     FileStr = NULL;
  }
  
  yysptr = yysbuf;
  yylineno = 1;
  yyin = NULL;
}

int ErrorOccurs(int Type)
{

  CrashAll();
  switch(Type)
  {
      case OPEN:
        sprintf(OSStrErr, "[File : %s][line : %d] Impossible d'ouvrir le fichier %s\n", PrecName, Line, Name);
      break;
      case MAXFILE:
        sprintf(OSStrErr, "[File : %s][line : %d] Nombre Maximum de fichiers atteint...\n", PrecName, yylineno);
      break;
      case SYNTAX:
        sprintf(OSStrErr, "[File : %s][line : %d] Erreur de syntaxe\n", Name, yylineno);
      break;
      case CHARACTER:
        sprintf(OSStrErr, "[File : %s][line : %d] Caractère Inconnu\n", Name, yylineno);
      break;
      case ALLOCATION:
        sprintf(OSStrErr, "[File : %s][line : %d] Mémoire Insuffisante...\n", Name, yylineno);
      break;
      case XLTCAPTION:
        sprintf(OSStrErr, "[File : %s][line : %d] Table de Translation incorrecte\n", Name, yylineno);
      break;
      default:
        sprintf(OSStrErr, "[File : %s][line : %d] Une erreur non référencée vient de se produire\n", Name, yylineno);
      break;
  }
  return(1);
}

int OSGiveChar(int x)
{
  LONG Pos = 0;
  char lpbuf;
  char car = (char)x;
  
  if (x <= 12) return(x);
  XFCharWrite (GlobalStx, &car, &Pos, &lpbuf, 1);
  
  return (lpbuf);
}

int OSSourceFile()
{
  return(((yytchar=yysptr>yysbuf?U(*--yysptr):OSGiveChar(getc(yyin)))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar);
}
