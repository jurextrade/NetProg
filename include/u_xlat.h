/*-------------------------------------------------------------------------*/
/*                    NETPROG (Network Programming)                        */
/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */  
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*                                                                         */
/* This program is free software; you can redistribute it and/or modify it */
/* under the terms of the GNU General Public License as published by the   */
/* Free Software Foundation; either version 2 of the License, or (at your  */ 
/* option) any later  version.                                             */
/*                                                                         */
/* This program is distributed in the hope that it will be useful, but     */
/* WITHOUT ANY WARRANTY; without even the implied warranty of              */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU General */
/* Public License for more details.                                        */
/*                                                                         */
/* You should have received a copy of the GNU General Public License along */
/* with this program; if not, write to the Free Software Foundation, Inc., */
/* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA                   */
/* Copy of GNU General Public License at: http://www.gnu.org/              */
/*                                                                         */
/* Source code home page: http://sourceforge.net/projects/netprog/         */
/* Contact author at: Gabriel_Jureidini@yahoo.com                          */
/*-------------------------------------------------------------------------*/


#ifndef __U_XLAT_H
#define __U_XLAT_H

#include "ostypes.h"
#include "oslist.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>



/* les codes erreurs sont negatifs */


#define NOMEM             -1     /* plus de memoire sur un malloc */
#define S_NOMEM                  "Plus assez de memoire disponible"
#define INVALID           -2     /* objet ou param¨tre invalide   */
#define NOTFOUND          -3     /* recherche infructueuse        */
#define NOTEXIST          -4     /* objet inexistant              */
#define ALREADY_EXISTS    -5     /* objet dŒjã existant           */

#define NOOPEN            -10    /* ouverture fichier impossible  */
#define S_NOOPEN                   "Ouverture fichier impossible"
#define NOOPENREAD        -11    /* idem si ouverture en lecture  */
#define NOOPENWRITE       -12    /* ...                           */
#define NOOPENAPPEND      -14
#define NOOPENUPDATE      -15
#define ENDFILE           -16

#define NOREAD            -20    /* lecture fichier impossible    */
#define NOWRITE           -21    /* ecriture fichier impossible   */

#define UNEXPECTED_EOF    -30    /* fin de fichier inattendue     */
#define S_UNEXPECTED_EOF           "Fin de fichier inattendue"



#define S_FILL  1
#define S_TRUNC 2
#define S_COLOR 4

#define COLORCAR '@'
#define MAXSTR 255
#define TOMINALLOC   12     /* taille du plus petit bloc alloue (16-4) */
#define TOMAXALLOC  508     /* limite la taille d'un bloc (512-4)      */
#define MAXALLOC  65530L    /* taille max d'une page, garde une marge */
#define ToSize(Tab) ((Tab)->nobj)
#define ToSizeObj(Tab) ((Tab)->size)
#define ToObj(Tab,r) ((r)>=ToSize(Tab) || (r) < 0 ? NULL : \
                     ((Tab)->first)+(r)*(Tab)->size)

#define ToLastObj(Tab) ToObj(Tab,ToSize(Tab)-1)
#define ToTab(Tab) ((Tab)->first)

#define DEFAULT_CHAR_CODE "ASCII"
#if defined(_VM_) || defined(AS400)
#undef  DEFAULT_CHAR_CODE
#define DEFAULT_CHAR_CODE "EBCDIC" /* jeu de car. par défaut */
#endif

#define XLAT_MAXBUF 80
#define MAXCODE     80                 /* nb max de caractères dans le nom du code */
#define MXlatc(x,c) (x == NULL ? c : (x)->Tab->Tables[(x)->FromTo][(unsigned char)c])
#define MUnXlatc(x,c) (x==NULL?c:(x)->Tab->Tables[not((x)->FromTo)][(unsigned char)c])
#define not(i) ((i) ? 0 : 1)           /* macro xor */
#define LoadXlatTable(n) LdXlTbl(n)

#define ReadSGnrl RdSGnrl
#define ReadS(f, s) ReadSGnrl( f, s, MAXSTR, 0)
#define ReadSF(f, s, max) ReadSGnrl( f, s, max, S_FILL )
#define ReadST(f, s, max) ReadSGnrl( f, s, max, S_TRUNC )
#define ReadSM(f, s, max) ReadSGnrl( f, s, max, 0 )

#define ToIniErrMem ToIniErr
#define ToExecArg ToExArg
#define ToDelObj ToDeObj
#define ToDelLastObj(Tab) ToDelObj(Tab, ToSize(Tab)-1);
#define ToTakeObj ToTakObj
#define ToAddSeveral ToAddSvl
#define ToAddObj(Tab, Obj) ToInsert( Tab, ToSize(Tab), Obj )






typedef struct _deftabobj
{
    int nobj;              /* Nombre courant d'objets dans la table   */
    size_t size;           /* taille d'un objet                       */
    char *first;           /* Adresse du premier objet de la table    */
    int nrealloc;          /* Intervalle de rŒallocation de la table  */
    void (* errmem)(void); /* Fonction ã appeler en cas de dŒbordement*/
}TABOBJ;

typedef int (*TOEXEC)(void *);

typedef int (*TOEXECARG)(void *, void *);



typedef struct _XlatTable
{
    char Name[250];
    char From[MAXCODE+1];     /* code jeu de caractères avant */
    char To[MAXCODE+1];       /* code jeu de caractères après */
    int IsBiject;             /* = 1 si table bijective*/
    char Tables[2][256];      /* tables de transformation directe et inverse*/
}XLAT_TBL;



typedef struct _XLat
{
    XLAT_TBL* Tab;            /* table à utiliser */
    int FromTo;               /* flag inversion */
}XLAT;



#ifdef __cplusplus
extern "C" {
#endif

    extern List* TabXlat;

    extern XLAT_TBL*  LdXlTbl( char* nomfic );
    extern XLAT*  InitXlat( char* in, char* out);
    extern char*  Xlat( XLAT* x, char* dest, char* src, size_t len);
    extern char*  UnXlat( XLAT* x, char* dest, char* src, size_t len);
    extern char*  Xlats( XLAT* x, char* str);
    extern char*  UnXlats( XLAT* x, char* str);
    extern char   Xlatc(XLAT* x , char c);
    extern char   UnXlatc(XLAT* x, char c);
    extern void   EndXlat(XLAT* x);
    extern void   FreeXlat(void);



    extern char*  stripe( char *);
    extern char*  strcpyv( char *, ...);
    extern char*  strcatv( char *, ...);
    extern char*  strfcpy( char *s1, const char *s2 , int n);
    extern char*  strmcpy( char *s1, char *s2, int max);

    extern void   ToIniErr(TABOBJ *tab, void (*ToErrMem)(void));
    extern size_t RdSGnrl(FILE *f, char *s, int max, int option);
    extern size_t WriteS(FILE *f, char *s);

    extern void     (*ToGenErrMem)(void);
    extern int      ToExec( TABOBJ *Tab, TOEXEC fct );
    extern int      ToExArg( TABOBJ *Tab, TOEXECARG fct, void * ArgQcq );
    extern int      ToDeObj( TABOBJ *Tab, int Rang );
    extern void     ToDelAdr( TABOBJ ** PtrTab ); /* PtrTab est mis ã NULL */
    extern void     ToDel( TABOBJ *Tab );
    extern void*    ToAddSvl( TABOBJ *t, int n);
    extern void*    ToTakObj( TABOBJ *Tab, int Rang );
    extern void*    ToCopy(TABOBJ *dest, TABOBJ *source);
    extern void*    ToInsert( TABOBJ *Tab, int Rang, void *Obj  );
    extern TABOBJ*  ToMk( int MaxObj, size_t Size );



#ifdef __cplusplus
}
#endif

#endif    /*__U_XLAT_H*/
