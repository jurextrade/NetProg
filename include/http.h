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
#ifndef __HTTP_H__
#define __HTTP_H__


#include "mx.h"



#ifdef __cplusplus
extern "C" {
#endif



    extern LONG         HTTPSend (MX* pmx, MXCom* pcom, MXMessage* pmessage);
    extern MXMessage*   HTTPRecv (MX* pmx, MXCom* pcom);
    extern LONG         HTTPWriteMessage (MX* pmx, MXMessage* pmessage, char* stream, int maxcount, BYTE WithFileContent);
    extern LONG         HTTPSendMessage (TP* ptp, TPClient* pclient, char* buf, int size);
    extern LONG         HTTPReceiveMessage (TP* ptp, TPClient* pclient, char* buf, int count);
    extern LONG         HTTPReadMessage (MX* pmx, MXMessage* pmessage, char* stream, int streamsize, BYTE WithFileContent);
    extern MXMessage*   HTTPCheckAndReturnMessage (MX* pmx, MXCom* pcom, char* stream);

    extern int          HTTPCloseConnection (MXCom* pcom, void* app);
    extern void         HTTPEndOtherMessages (MX* pmx, MXCom* pcom, MXMessage* pmessage);


#ifdef __cplusplus
}
#endif



#endif /* __HTTP_H__*/
