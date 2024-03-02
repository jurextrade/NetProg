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

#ifndef __FTP_H__
#define __FTP_H__


#include "mx.h"


typedef struct _FTPActiveConnection
{
    int    Port;                  /* Port Number */
    MXCom* Com;                   /* ftp connection send PORT command */
    int    Done;                  /* connection done on this port */
    char   NetId[250];
    BYTE   Type;                  /* Passive or Active */
}FTPActiveConnection;


#define FTPNotCommandMessage(pmessage) (strcmp (pmessage->MessageClass->Name, "Command") != 0)


#ifdef __cplusplus
extern "C" {
#endif

    extern List* FTPActiveConnections;

    extern MXCom*       FTPConnect(MX* pmx, char* Host, int Port);
    extern int          FTPClose (MXCom* pcom, void* app);
    extern int          FTPSendIdentity (MX* pmx, MXCom* pcom, char* Host, int Port, char* UserName, char* PassWord, char* Acct, char* fwHostName, char* fwUserName, char* fwPassWord,int fwPort,int LogonType);

    extern MXMessage*   FTPRecv (MX* pmx, MXCom* pcom);
    extern LONG         FTPSend (MX* pmx, MXCom* pcom, MXMessage* pmessage);
    extern MXMessage*   FTPSendAndRecvCommand (MX* pmx, MXCom* pcom, char* Value, char* Par);


    extern MXMessage*   FTPCheckAndReturnMessage (MX* pmx, MXCom* pcom, char* stream);

    extern LONG         FTPReadMessage (MX* pmx, MXMessage* pmessage, char* stream, int RequestSize, BYTE WithFileContent);
    extern LONG         FTPReceiveMessage (TP* ptp, TPClient* pclient, char* buf, int count);

    extern LONG         FTPWriteMessage (MX* pmx, MXMessage* pmessage, char* stream, int maxcount, BYTE WithFileContent);
    extern LONG         FTPSendMessage (TP* ptp, TPClient* pclient, char* buf, int size);

    extern int          FTPCloseActiveConnection (MXCom* pcom, void* app);
    extern int          FTPOpenActiveConnection (MXCom* pcom, void* app);

    extern MXMessage*   FTPCheckAndReturnActiveMessage (MX* pmx, MXCom* pcom, char* stream);


    extern LONG         FTPReadFileContent (MX* pmx, MXMessageContext* pcontext, char* stream, char* attributes, WORD maxcount);
    extern LONG         FTPWriteFileContent(MX* pmx, MXMessageContext* pcontext, char* stream, char* attributes, WORD maxcount);

    extern MXMessage*   FTPGetReadyActiveMessage (MXCom* pcom);

    extern LONG         FTPReadActiveMessage (MX* pmx, MXMessage* pmessage, char* stream, int streamsize, BYTE WithFileContent);
    extern LONG         FTPReceiveActiveMessage (TP* ptp, TPClient* pclient, char* buf, int count);

    extern LONG         FTPWriteActiveMessage (MX* pmx, MXMessage* pmessage, char* stream, int maxcount, BYTE WithFileContent);
    extern LONG         FTPSendActiveMessage (TP* ptp, TPClient* pclient, char* buf, int size);

    extern int          FTPGetFile (MXCom* pcom, LONG Identity, char* FileName, char* DestName, CHAR FileType, char* NetId, LONG Port);
    extern int          FTPSendFile (MXCom* pcom, LONG Identity, char* FileName, char* DestName, CHAR FileType, char* NetId, LONG Port);
    extern void         FTPTreatOtherMessages (MX* pmx, MXCom* pcom, MXMessage* pmessage);
    extern void         FTPEndOtherMessages (MX* pmx, MXCom* pcom, MXMessage* pmessage);



    extern void         FTPGetNetworkAndPort (char* Response, char* netid, int* port, BYTE Type);
    extern FTPActiveConnection* FTPFindActiveConnection (MX* pmx, MXCom* pcom, int Port);
    extern FTPActiveConnection* FTPCreateActiveConnection (MX* pmx, MXCom* pcom, int Port, char* NetId, BYTE Type);

    extern int SYS_get (MXMessage* pmessage, MXCom* pcom, void* apfield);
    extern int SYS_put (MXMessage* pmessage, MXCom* pcom, void* apfield);
    extern int SYS_dir (MXMessage* pmessage, MXCom* pcom, void* apfield);
    extern int SYS_Command (MXMessage* pmessage, MXCom* pcom, void* apfield);
    extern int SYS_Command_Reply (MXMessage* pmessage, MXCom* pcom, void* apfield);



#ifdef __cplusplus
}
#endif



#endif /* __FTP_H__*/








