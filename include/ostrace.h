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


#ifndef __OSTRACE_H__
#define __OSTRACE_H__

#include <stdarg.h>


#include <time.h>
#include "osio.h"
#include "oscom.h"

#define SZ_BUFFER 600

#define SITE1    "$"
#define DISTANT "$"



#define DEBUT_IV                                 0
#define FIN_IV                                 199

#define DEBUT_GM                               200
#define FIN_GM                                 299

#define DEBUT_DIALOG                           300
#define FIN_DIALOG                             399

#define DEBUT_DB                               400  
#define FIN_DB                                 499


#define DEBUT_OS                              1000


typedef struct _OSPrintProcedure
{
    void* ApplicationField;
    int (*PrintProcedure) (char* buf, void* papp);
} OSPrintProcedure;


extern LONG GlbError  ;
extern LONG GlbTCPError  ;

extern char GlbMessg[SZ_BUFFER+1] ; /* Le message                       */
extern char GlbParam[SZ_BUFFER+1] ; /* Les parametres                   */
extern char GlbInter[SZ_BUFFER+1] ; /* Le message + les parametres      */
extern char GlbLigne[SZ_BUFFER+1] ; /* La ligne complete                */
extern char GlbReper[SZ_BUFFER+1] ; /* Le repertoire des fichiers codes     */

extern IDENTFICH GlbTrace ;

#ifdef __cplusplus
extern "C" {
#endif

    extern void OSSetPrintProcedure (int (*funct)(char* , void*), void* app);

    extern char* OSGetMessg(void) ;
    extern char* OSGetParam(void) ;
    extern char* OSGetInter(void) ;
    extern char* OSGetLigne(void) ;

    extern LONG* OSGetError(void) ;

    extern void  OSSetRepertoireCode(char * chRepertoireCode) ;

    extern IDENTFICH * OSReturnFicTrace(IDENTFICH * ptIdent, char typloc, char * localis, char * ficfname, char * ficftype) ;

    extern char* OSLoadMessage(LONG iIdtErr) ;

    extern char* OSReturnMessage(LONG iIdtErr) ;
    extern char* OSConstructMessage(LONG iIdtErr, char * chParam, ...) ;

    extern LONG OSGlbTraceInFile(void) ;
    extern LONG OSTraceInFile(IDENTFICH * ptIdent, char * chMessage) ;

    extern void OSSetFunctionPrintMessage(void (*PrintProcedure)(char *)) ;
    extern void OSPrintMessage(char * chMessage) ;

    extern void OSSetTraceFile (char* fromname);
    extern LONG OSTraceMessage (char* message);

    extern void OSSetTraceOpenedFile (FILE* file);
    extern LONG OSTraceOpenedMessage (char* message);

    extern void  OSError(char * Name, char * chParam, ...);
    extern void  TCPERROR(int error) ;
    extern char* fgetline(char * chChaine, int iNbr, FILE * ptFic) ;

#ifdef __cplusplus
}
#endif

#endif
