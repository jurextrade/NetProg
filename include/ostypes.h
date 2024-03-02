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


#ifndef __OSTYPES_H__
#define __OSTYPES_H__

#if defined(WIN32) && !defined (WINDOWS)
   #define WINDOWS 
#endif


#ifndef __VOS__
#ifndef BYTE_ORDER
#if (BSD >= 199103)
# include <machine/endian.h>
#else
#ifdef linux
# include <endian.h>
#else
#define LITTLE_ENDIAN 1234    /* least-significant byte first (vax, pc) */
#define BIG_ENDIAN    4321    /* most-significant byte first (IBM, net) */
#define PDP_ENDIAN    3412    /* LSB first in word, MSW first in long (pdp)*/

#if defined(vax) || defined(ns32000) || defined(sun386) || defined(i386) || \
    defined(MIPSEL) || defined(_MIPSEL) || defined(BIT_ZERO_ON_RIGHT) || \
    defined(__alpha__) || defined(__alpha) || \
    defined(WINDOWS) || defined (WIN32) ||defined(OS2) ||  defined(SCO)
  #define LOW_HIGH     LITTLE_ENDIAN
  #define BYTE_ORDER   LITTLE_ENDIAN
#endif

#if defined(sel) || defined(pyr) || defined(mc68000) || defined(sparc) || \
    defined(is68k) || defined(tahoe) || defined(ibm032) || defined(ibm370) || \
    defined(MIPSEB) || defined(_MIPSEB) || defined(_IBMR2) || defined(DGUX) ||\
    defined(apollo) || defined(__convex__) || defined(_CRAY) || \
    defined(__hppa) || defined(__hp9000) || \
    defined(__hp9000s300) || defined(__hp9000s700) || \
    defined (BIT_ZERO_ON_LEFT) || defined(m68k) || \
    defined(_VM_) || defined(UNIX) || defined(AS400) || defined(__VOS__)
  #define HIGH_LOW     BIG_ENDIAN
  #define BYTE_ORDER   BIG_ENDIAN
#endif
#endif /* linux */
#endif /* BSD */
#endif /* BYTE_ORDER */

#if !defined(BYTE_ORDER) || \
    (BYTE_ORDER != BIG_ENDIAN && BYTE_ORDER != LITTLE_ENDIAN && \
    BYTE_ORDER != PDP_ENDIAN)
/* you must determine what the correct bit order is for
 * your compiler - the next line is an intentional error
 * which will force your compiles to bomb until you fix
 * the above macros.
 */
error "Undefined or invalid BYTE_ORDER";
#endif
#else  /* __VOS__*/
  #define LITTLE_ENDIAN 1234    /* least-significant byte first (vax, pc) */
  #define BIG_ENDIAN    4321    /* most-significant byte first (IBM, net) */
  #define PDP_ENDIAN    3412    /* LSB first in word, MSW first in long (pdp)*/
  #define HIGH_LOW     BIG_ENDIAN
  #define BYTE_ORDER   BIG_ENDIAN
#endif /*__VOS__*/

#if defined(WINDOWS) || defined(UNIX) || defined(__VOS__)
  #define CHAR_ASCII
#endif

#if defined(OS2) 
  #define CHAR_OS2
#endif

#if defined(_VM_) || defined(AS400)
  #define CHAR_EBCDIC
#endif

#ifndef XMD_H 
typedef unsigned char  BYTE;       /* 8-bit unsigned entity                   */
typedef int            BOOL;       /* BOOLean (0 or !=0)                      */
#endif                             

typedef unsigned short WORD;       /* 16-bit unsigned number                   */
typedef unsigned int   UINT;       /* machine sized unsigned number (preferred)*/
typedef short          SHORT;      /* 16-bit signed number                     */
typedef long           LONG;       /* 32-bit signed number                     */
typedef unsigned long  DWORD;      /* 32-bit unsigned number                   */
typedef double         DOUBLE;     /* 64-bit signed number                     */
typedef char           CHAR;       /* 8-bit character                          */
typedef char*          STRING;

#define OSCHAR      1
#define INTEGER     2
#define OSDECIMAL 3
#define OSDOUBLE  4
#define OSBIT     5
#define OSFLOAT   6
#define OSLONGVARBINARY 7
#define OSNUMERIC 8
#define OSTIMESTAMP 9    
#define OSTIME    10 
#define OSDATE    11
#define OSVARCHAR 12
#define OSINTEGER 13
#define OSLONGVARCHAR 14 
#define OSREAL    15
#define OSBIGINT  16
#define OSBINARY  17	
#define SMALLINT  18			
#define OSTINYINT 19
#define OSVARBINARY 20

#define OSSHORT    21
#define OSWORD     22
#define OSBYTE     23
#define OSSTRING   24
#define OSBUFFER   25
#define OSFILE     26
#define OSLONG     27
#define OSDWORD    28



#ifdef WINDOWS
#include <windows.h>
#else
 #define FAR
 #define NEAR
#endif

#ifdef NULL
 #undef NULL
 #define NULL 0
#endif


#ifdef TRUE
 #undef TRUE
 #define TRUE 1
#endif

#ifdef FALSE
 #undef FALSE
 #define FALSE 0
#endif


#define KO           1
#define OK           0


#define  FILENAMESIZE   120
#define  ACCESSSIZE      30 
#define  DATESIZE        30
#define  MAXBUFSIZE (1L << 21)



#define OSFICTEXTE  't'             /* Fichier de type Texte                  */
#define OSFICBINAI  'b'             /* Fichier de type Binaire                */          
#define OSFICTXCMP  'x'             /* Fichier de type Texte compresse        */
#define OSFICBICMP  'n'             /* Fichier de type Binaire comp.          */
#define OSFICBITRS  'g'             /* Fichier de type Binaire trancodifie    */

#define TYPEDIRECTORY    1
#define TYPEFILE         0
#define TYPELINK         2

#define  BUFSIZE      4096                                                    
#define  BUFWATERMARK 8192                                                    



typedef struct
{
    CHAR   FileOri[FILENAMESIZE];     /* nom du fichier Origine                 */
    CHAR   FileDest[FILENAMESIZE];    /* nom du fichier Destinataire            */
    CHAR   FileOriLocal;              /* disque ou SFS                          */
    CHAR   FileDestLocal;             /* disque ou SFS                          */
    CHAR   FileType;                  /* Type du fichier                        */
    DWORD  FileSize;                  /* Taille du fichier en octets            */
    LONG   FileError;                 /* Nb d'enregistrements du fichier        */
}FILEPARM;

/* variable ou fixe sur VM ? */

typedef struct
{
    CHAR  Name[FILENAMESIZE];
    BYTE  Type;                       /* Repertory or File                     */
    int   Size;
    CHAR  Access[ACCESSSIZE];
    CHAR  Date[DATESIZE];
}FILEDATA;


typedef struct
{
    DWORD  BufferSize;                 /* Size of buffer                         */
    CHAR  BufferType;                 /* Type du buffer ascii ou binaire        */
    CHAR* BufferContent;
}BUFFERPARM;



#endif /* __OSTYPES_H__ */
