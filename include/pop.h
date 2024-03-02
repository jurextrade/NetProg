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

#ifndef __POP_H__
#define __POP_H__


#include "mx.h"



#define POPNotCommandMessage(pmessage) (strcmp (pmessage->MessageClass->Name, "Command") != 0)


#ifdef __cplusplus
extern "C" {
#endif


    extern MXMessage*   POPRecv (MX* pmx, MXCom* pcom);
    extern LONG         POPSend (MX* pmx, MXCom* pcom, MXMessage* pmessage);
    extern MXMessage*   POPCheckAndReturnMessage (MX* pmx, MXCom* pcom, char* stream);
    extern void         POPTreatOtherMessages (MX* pmx, MXCom* pcom, MXMessage* pmessage);

    extern LONG         POPReadMessage (MX* pmx, MXMessage* pmessage, char* stream, int RequestSize, BYTE WithFileContent);
    extern LONG         POPReceiveMessage (TP* ptp, TPClient* pclient, char* buf, int count);

    extern LONG         POPWriteMessage (MX* pmx, MXMessage* pmessage, char* stream, int maxcount, BYTE WithFileContent);
    extern LONG         POPSendMessage (TP* ptp, TPClient* pclient, char* buf, int size);

    extern int          POPAddFirstReplyMessage (MXCom* pcom);

    extern int SYS_POPCommand (MXMessage* pmessage, MXCom* pcom, void* apfield);
    extern int SYS_POPCommand_Reply (MXMessage* pmessage, MXCom* pcom, void* apfield);

    extern BOOL         POPCheckResponse(TPClient* pclient, int ResponseType);
    extern TPClient*    POPConnect(TP* ptp, char* ToHost, char* User, char* Password);
    extern BOOL         POPDisconnect(TPClient* pclient);
    extern BOOL         POPDelete(TPClient* pclient, int MsgNumber);
    extern BOOL         POPNoop(TPClient* pclient);
    extern BOOL         POPRetrieve(TPClient* pclient, int MsgNumber);
    extern BOOL         POPReset(TPClient* pclient);
    extern BOOL         POPStatistics(TPClient* pclient);
    extern BOOL         POPList(TPClient* pclient);

#ifdef __cplusplus
}
#endif



#endif /* __POP_H__*/

