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

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "osdb.h"
#include "mx.h"



#ifdef __cplusplus
extern "C" {
#endif
    extern char*        DBTreatHeader (OSSqlda* psqlda, WORD* size);
    extern int          DBAckProcedure (void* ack, void* par);
    extern int          DBHeaderProcedure (void* sqlda, void* par);
    extern int          DBContentsProcedure (void* sqlda, void* par);

    extern MXMessage*   DBCheckAndReturnMessage (MX* pmx, MXCom* pcom, char* stream);
    extern LONG         DBWriteMessage (MX* pmx, MXMessage* pmessage, char* stream, int maxcount, BYTE WithFileContent);

    extern LONG         DBSendMessage (TP* ptp, TPClient* pclient, char* buf, int size);
    extern LONG         DBReceiveMessage (TP* ptp, TPClient* pclient, char* buf, int count);
    extern LONG         DBSend (MX* pmx, MXCom* pcom, MXMessage* pmessage);
    extern MXMessage*   DBRecv (MX* pmx, MXCom* pcom);
#ifdef __cplusplus
}
#endif

#endif
