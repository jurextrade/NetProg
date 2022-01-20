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



#ifndef __XF_H__
#define __XF_H__


#include "u_xlat.h"
#include "ostypes.h"
#include "ostrace.h"
#include "oslist.h"


/* the stream of communication between two end points will be : */
/* Little Endian for types and ASCII Char for characters     */


/* different system representation of types */

#define    XFLittleEndian    0x0000
#define    XFBigEndian       0x0001


/* different type of characters*/

#define    XFAscii           0x0000         /* Windows et Unix */
#define    XFEbcdic          0x0001
#define    XFOs2             0x0002

/* Code page categories */

#define XFCPWINDOWS    0
#define XFCPEBCDIC     1
#define XFCPEDITION    2
#define XFCPISO8859    3
#define XFCPAPPLE      4
#define XFCPOTHER      5


typedef struct _XFSymbolRepresentation
{
    char   AsciiRepresentation[256];
    char   InFont[256];
}XFSymbolRepresentation;


typedef struct _XFTableRepresentation
{
    UINT   Category;
    char   Label[250];
    int    CountryCode;
}XFTableRepresentation;



typedef struct _XFTable
{
    char   Name[250];
    int             Code;
    char   From[120];
    char   To[120];
    char*  Table[16];
}XFTable;



typedef struct _XF
{
    XLAT*  Xlat;                   /* table of transcodification   */
    UINT   SystemFlag;             /* System little-e or big-e     */
    UINT   StreamSystemFlag;       /* buffering little-e or big-e  */
    UINT   CharFlag;               /* System char                  */
    UINT   StreamCharFlag;         /* stream char flag             */
    WORD   Code;                   /* code of the class            */
    char FileName[250];            /* File where syntax is defined */
}XF;




/*=====================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

    extern int       XFInit (XF* pxf, UINT xdr, UINT transcode);
    extern int       XFInitSameSystem (XF* pxf);
    extern void      XFEnd (XF* pxf);
    extern XLAT_TBL* XFGetXlatFromName(char* Name);
    extern XLAT_TBL* XFLoadSystemTable(XFTable *ptable);
    extern int       XFGetIndexTableFromName (char* Name);


    extern void  XFError (char* name, int index);

    extern DWORD XFByteRead   (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size);
    extern DWORD XFWordRead   (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size);
    extern DWORD XFCharRead   (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size);
    extern DWORD XFLongRead   (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size);
    extern DWORD XFDWordRead  (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size);
    extern DWORD XFDoubleRead (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size);
    extern DWORD XFStringRead (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size);
    extern DWORD XFFileRead   (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size);
    extern DWORD XFByteWrite  (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size);
    extern DWORD XFCharWrite  (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size);
    extern DWORD XFWordWrite  (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size);
    extern DWORD XFShortWrite (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size);
    extern DWORD XFLongWrite  (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size);
    extern DWORD XFDWordWrite (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size);
    extern DWORD XFDoubleWrite(XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size);
    extern DWORD XFShortRead  (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size);
    extern DWORD XFStringWrite(XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size);
    extern DWORD XFFileWrite  (XF* pxf, char* lpbuffer, LONG* ReadBufferPos, char* BeginPointer, int Size);
    extern BOOL  XFSameSystem (XF* pxf);
    extern void  XFChangeSystemFlag (XF* pxf, UINT xdr, UINT transcode);

#ifdef __cplusplus
}
#endif


#endif /* __XF_H__ */

