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

#ifndef __OSIO_H__
#define __OSIO_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "ostypes.h"
#include "osdate.h"
#include "oslist.h"
#include "osmatch.h"


#ifdef __VOS__
#include <c_utilities.h>
#endif

#ifdef SCO
 #include <sys/types.h>
 #include <dirent.h>
#elif defined(UNIX)
#ifdef HP_UX
 #include <dirent.h>
#else
  #include <sys/dir.h>
#endif
#endif

#ifdef WINDOWS
  #include <direct.h>
#endif

#define IOPROTO_FTP            -IPPROTO_FTP               
#define IOPROTO_SMTP           -IPPROTO_SMTP              
#define IOPROTO_DNS            -IPPROTO_DNS               
#define IOPROTO_HTTP           -IPPROTO_HTTP              
#define IOPROTO_POP            -IPPROTO_POP                
#define IOPROTO_BC             -IPPROTO_BC                 
#define IOPROTO_STORE          -IPPROTO_DG                   
#define IOPROTO_MTT            -IPPROTO_MTT                   
#define IOPROTO_FIOP           10000


#define LOCREPER  'R'               /* Stockage sur r{pertoire SFS            */
#define LOCDISQU  'D'               /* Stockage sur mini-disque CMS           */
#define LOCBANDE  'B'               /* Stockage sur bande       CMS           */
#define LOCVIRER  'V'               /* on ne trouve pas                       */

#define LNGNOMFI  8                 /* longueur du nom de fichier  sur le host*/
#define LNGLOCAL  35                /* longueur de la zone de localisation fichier*/
#define LNGNOMIC  120               /* longueur du fichier sur micro          */



typedef struct
{
    char   typloc          ;          /* Type de la localisation                */
    char   localis[LNGLOCAL];         /* Repertoire ou mini disque              */
}TYPLOCAL ;



typedef struct
{
    TYPLOCAL zloc;
    char ficfname[LNGNOMFI];          /* Fname du fichier                       */
    char ficftype[LNGNOMFI];          /* Ftype du fichier                       */
}IDENTFICH ;


typedef struct
{
    char* Buffer;
    char* BufPtr;
    int   BufCnt;
    int   LenLastReq;
    int   Size;
    int   Count;
}CHANNEL;




typedef struct
{
    void*   File;
    char    Type;                     /* text or binary                         */
    char    Format;                   /* fix or variable for later use          */
    char    Disc;                     /* letter for VMS                         */
    char    Loc;                      /* R or D   SFS or Disc  form VMS         */
    int     LineSize;                 /* read line size                         */
}FILEID;


typedef struct
{
    int      fd;                      /* socket                                 */
    FILEID*  fileid;                  /* file                                   */
    void*    connection;              /* database                               */
    CHANNEL* Input;
    CHANNEL* Output;
    BYTE     AuthorId;
}PIPE;



/*----------------------------------------------------------------------------*/



#ifdef __cplusplus
extern "C" {
#endif


    extern FILEID* OSOpenFile (char* filename, char* mode , char type,  char loc, int* error);
    extern int     OSCloseFile (FILEID* file);
    extern LONG    OSGetFileSize (char* filename, char type, char loc, int* error);
    extern int     OSRenameFile (char* filename, char* tofilename, char loc, int* error);
    extern int     OSRemoveFile (char* filename, char loc, int* error);
    extern LONG    OSReadFile (FILEID* fileid, char* buffer, LONG size, int* error);
    extern LONG    OSWriteFile (FILEID* fileid, char* buffer, LONG size, int pos, int* error);
    extern void    OSSeekFile (FILEID* file, long offset, int whence);
    extern int     OSVerifExistence (char* filename, char loc, int* error);
    extern void    OSGetFileName (IDENTFICH* identfile, char* filename);
    extern char*   OSGetOnlyName (char* fromname, char* filename, char* Repertory, char FileLoc);
    extern int     OSCopyFile (char* filename, char* newfilename, char loc, int* error);
    extern int     OSChangeDir (char* NewDir, char FileLoc, int* error);
    extern int     OSMakeDir (char* Dir, char FileLoc, int* error);
    extern int     OSRemoveDir (char* Dir, char FileLoc, int* error);
    extern char*   OSGetDir (char* Dir, char FileLoc, int* error);
    extern int     OSDir (char* Name, char FileLoc, BUFFERPARM* Buffer);
    extern List*   OSList (char* Name, char FileLoc);

    extern void    OSDateHour (DWORD* dat, DWORD* hour);
    extern char*   OSGetFileNameExtension (char* extension, char* filename);
    extern int     OSGetSFSFormatFromReper (char* directory, char* sfsdir);
    extern int     OSGetCMSFormatFromFileName (char* filename, char* sfsname, char* sfstype, char* sfsloc);
    extern int     OSGetSFSFormatFromFileName (char* filename, char* sfsname, char* sfstype);
    extern int     OSTakeOffBackpoints (char* directory, char* sfsdir);
    extern int     OSSetFileName (char* destfilename, char* repertory, char* filename, char FileLoc);
    extern int     OSStrcmp(char* string1, char* string2);
    extern int     OSStrncmp(char* string1, char* string2, int nbcar);
    extern int     OSStrEqNc(char* s1, char* s2);


    extern char*   OSFormateHour(long hour, char* hourcha, BOOL WithSep);
    extern char*   OSFormateDate(long date, char* datcha, BOOL WithSep);
    extern DWORD   OSIncrementHour (DWORD hour);

    extern PIPE*    OSAllocatePipe (void);
    extern CHANNEL* OSAllocateChannel (void);

    extern void      OSFreePipe (PIPE* ppipe);
    extern void      OSFreeChannel (CHANNEL* pchannel);

#ifdef __cplusplus
}
#endif



#endif /* __OSIO_H__*/


