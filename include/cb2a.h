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

#ifndef __CB2A_H__
#define __CB2A_H__

#include "mx.h"


#ifdef __cplusplus
extern "C" {
#endif

    extern int        CB2AReadMessageHeader (MX* pmx, char* stream, MXMessageHeader* pheader);
    extern int        CB2AWriteMessageHeader (MX* pmx, MXMessage* pmessage, char* stream, BOOL FirstTime);
    extern LONG       CB2AReadMessage (MX* pmx, MXMessage* pmessage, char* stream, int count, BYTE WithFileContent);
    extern LONG       CB2AWriteMessage (MX* pmx, MXMessage* pmessage, char* stream, int count, BYTE WithFileContent);
    extern LONG       CB2AReceiveMessage (TP* ptp, TPClient* pclient, char* buf, int count);
    extern LONG       CB2ASendMessage (TP* ptp, TPClient* pclient, char* buf, int bufsize);
    extern MXMessage* CB2ARecv (MX* pmx, MXCom* pcom);
    extern LONG       CB2ASend (MX* pmx, MXCom* pcom, MXMessage* pmessage);
    extern MXMessage* CB2ACheckAndReturnMessage (MX* pmx, MXCom* pcom, char* stream);



    /*----------------------------------------------------------------------------*/
    /* BEGIN GENERIC PROCEDURES                                                   */
    /*----------------------------------------------------------------------------*/





#ifdef __cplusplus
}
#endif



#endif /* __CB2A_H__*/
