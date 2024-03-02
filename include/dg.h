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

#ifndef __DG_H__
#define __DG_H__

#include "mx.h"


#ifdef __cplusplus
extern "C" {
#endif

    extern int        DGReadMessageHeader (MX* pmx, char* stream, MXMessageHeader* pheader);
    extern int        DGWriteMessageHeader (MX* pmx, MXMessage* pmessage, char* stream, BOOL FirstTime);
    extern LONG       DGReadMessage (MX* pmx, MXMessage* pmessage, char* stream, int count, BYTE WithFileContent);
    extern LONG       DGWriteMessage (MX* pmx, MXMessage* pmessage, char* stream, int count, BYTE WithFileContent);
    extern LONG       DGReceiveMessage (TP* ptp, TPClient* pclient, char* buf, int count);
    extern LONG       DGSendMessage (TP* ptp, TPClient* pclient, char* buf, int bufsize);
    extern MXMessage* DGRecv (MX* pmx, MXCom* pcom);
    extern LONG       DGSend (MX* pmx, MXCom* pcom, MXMessage* pmessage);
    extern MXMessage* DGCheckAndReturnMessage (MX* pmx, MXCom* pcom, char* stream);



    /*----------------------------------------------------------------------------*/
    /* BEGIN GENERIC PROCEDURES                                                   */
    /*----------------------------------------------------------------------------*/
    extern int        DGSendFiles (MXCom* pcom, LONG Identity, char* FileNames, char* DistantRepertory, CHAR FileType,
                                       CHAR LocalFileLoc, CHAR DistantFileLoc) ;
    extern int        DGSendFile  (MXCom* pcom, LONG Identity, char* FileName, char* DestName, CHAR FileType,
                                       CHAR LocalFileLoc, CHAR DistantFileLoc);
    extern int        DGGetFiles  (MXCom* pcom, LONG Identity, char* FileNames, char* LocalRepertory, CHAR FileType,
                                       CHAR LocalFileLoc, CHAR DistantFileLoc);
    extern int        DGGetFile   (MXCom* pcom, LONG Identity, char* FileName, char* DestName, CHAR FileType,
                                       CHAR LocalFileLoc, CHAR DistantFileLoc);
    extern int        DGRemoveFiles (MXCom* pcom, LONG Identity, char* FileNames, CHAR FileLoc);
    extern int        DGRenameFile(MXCom* pcom, LONG Identity, char* OldName, char* NewName, CHAR FileLoc);
    extern int        DGChangeDir (MXCom* pcom, LONG Identity, char* NewDir, char FileLoc);
    extern int        DGMakeDir   (MXCom* pcom, LONG Identity, char* Dir, char FileLoc);
    extern int        DGRemoveDir (MXCom* pcom, LONG Identity, char* Dir, char FileLoc);
    extern int        DGGetDir    (MXCom* pcom, LONG Identity, char FileLoc);
    extern int        DGPutError  (MXCom* pcom, LONG Identity, LONG Error, char * Param,BOOL Stop);
    extern void       DGSendClassConnection (MXCom* pcom, STRING Name, WORD Code, int Port, STRING TableName, BYTE FromTo);
    extern void       DGReplyClassConnection (MXCom* pcom, STRING Name, LONG Error);
    extern int        DGGetList   (MXCom* pcom, LONG Identity, char* FileName, CHAR LocalFileLoc);
    extern int        DGDir       (MXCom* pcom, LONG Identity, char* Name, CHAR FileLoc);


    /*----------------------------------------------------------------------------*/
    /* END GENERIC PROCEDURES                                                     */
    /*----------------------------------------------------------------------------*/

    extern int SYS_PutError (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int SYS_AckMessage (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int SYS_ReplyClassConnection (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int SYS_SendClassConnection (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int SYS_Command (MXMessage* pmessage, MXCom* pcom, void* apfield);
    extern int SYS_Command_Reply (MXMessage* pmessage, MXCom* pcom, void* apfield);
    extern int SYS_RenameFile (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int SYS_RemoveFiles (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int SYS_GetFile(MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int SYS_GetFiles (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int SYS_SendFile (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int SYS_List(MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int SYS_Dir(MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int SYS_MakeDir (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int SYS_RemoveDir (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int SYS_GetDir (MXMessage* pmessage, MXCom* pcom, void* appfield);
    extern int SYS_ChangeDir (MXMessage* pmessage, MXCom* pcom, void* appfield);

#ifdef __cplusplus
}
#endif



#endif /* __DG_H__*/
