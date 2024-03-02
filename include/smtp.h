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
#ifndef __SMTP_H__
#define __SMTP_H__


#include "mx.h"


#define SMTPNotCommandMessage(pmessage) (strcmp (pmessage->MessageClass->Name, "Command") != 0)



#ifdef __cplusplus
extern "C" {
#endif


    extern MXMessage*   SMTPRecv (MX* pmx, MXCom* pcom);
    extern LONG         SMTPSend (MX* pmx, MXCom* pcom, MXMessage* pmessage);
    extern MXMessage*   SMTPCheckAndReturnMessage (MX* pmx, MXCom* pcom, char* stream);
    extern void         SMTPTreatOtherMessages (MX* pmx, MXCom* pcom, MXMessage* pmessage);

    extern LONG         SMTPReadMessage (MX* pmx, MXMessage* pmessage, char* stream, int RequestSize, BYTE WithFileContent);
    extern LONG         SMTPReceiveMessage (TP* ptp, TPClient* pclient, char* buf, int count);

    extern LONG         SMTPWriteMessage (MX* pmx, MXMessage* pmessage, char* stream, int maxcount, BYTE WithFileContent);
    extern LONG         SMTPSendMessage (TP* ptp, TPClient* pclient, char* buf, int size);

    extern int          SMTPAddFirstReplyMessage (MXCom* pcom);


    extern int SYS_SMTPCommand (MXMessage* pmessage, MXCom* pcom, void* apfield);
    extern int SYS_SMTPCommand_Reply (MXMessage* pmessage, MXCom* pcom, void* apfield);


    extern BOOL         SMTPCheckResponse(TPClient* pclient, int Type);
    extern TPClient*    SMTPConnect(TP* ptp, char* ToHost);
    extern BOOL         SMTPHelo (TPClient* pclient);
    extern BOOL         SMTPQuit(TPClient* pclient);
    extern BOOL         SMTPMail(TPClient* pclient, char* from);
    extern BOOL         SMTPSetTo(TPClient* pclient, char* to);
    extern BOOL         SMTPData(TPClient* pclient, char* Header, char* Body);


#ifdef __cplusplus
}
#endif



#endif /* __SMTP_H__*/

