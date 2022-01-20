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

#ifndef __SCLEX_H__
#define __SCLEX_H__

#if defined(__cplusplus) && defined(_CPP_IOSTREAMS)
#include <iostream.h>
#endif

#include <stdio.h>
#include <stddef.h>
#include <locale.h>
#include <stdlib.h>
#include <stdarg.h>
#include "sc.h"



#undef  YYLMAX
#define YYLMAX  50000
#define NB_ACTIONS_OVERLAPED 20

extern int yylineno;

extern char StrErr[150];
extern BYTE InStringRemote;
extern char StringRemote[YYLMAX];
extern List* SCReturnParseSection;


#ifdef __cplusplus
extern "C" {
#endif
    extern int          yylex();
    extern int          yyparse (SC* psc);
    extern void         yyerror(char *fmt, ...);
    extern int          SCInitEntryWithFile(FILE *pFile);
    extern int          SCInitEntryWithString(char *pentry);
    extern int          SCInitEntryWithFile(FILE *pFile);
    extern int          SCInitEntryWithString(char *pentry);
    extern int          SCInitYacc(BYTE WithMain);
    extern void         SCFreeYacc();
    extern void         SCTraceSections(List* lsection);

    extern void         Err(int IdErr , char *fmt, ...);
    extern void         Trace(char *fmt, ...);



#ifdef __cplusplus
}
#endif


#endif
