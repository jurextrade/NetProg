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

#ifndef __MAIL_H__
#define __MAIL_H__

#include "mx.h"

#define CONNECTION_CHECK 0
#define HELLO_CHECK      1
#define MAIL_CHECK       2
#define RCPT_CHECK       3
#define DATA_START_CHECK 4
#define DATA_END_CHECK   5
#define QUIT_CHECK       6
#define DATA_CHECK       7

#define USER_CHECK       8 
#define PASSWORD_CHECK   9 
#define DELETE_CHECK     10 
#define RSET_CHECK       11 
#define STAT_CHECK       12 
#define NOOP_CHECK       13 
#define LIST_CHECK       14 
#define RETR_CHECK       15 


#define EMAIL_TO         0
#define EMAIL_CC         1
#define EMAIL_BCC        2
#define EMAIL_FROM       3


#define TEXT_PLAIN              0
#define APPLICATION_OCTETSTREAM 1
#define NEXT_FREE_MIME_CODE     2

#define _7BIT                   0
#define _8BIT                   1
#define BINARY                  2
#define QUOTED_PRINTABLE        3
#define BASE64                  4
#define NEXT_FREE_ENCODING_CODE 5


#define BYTES_TO_READ           54 
#define EMAILBUFSIZE            4096



typedef struct _MimePart
{
    int   Encoding;
    int   ContentType;
    char  Parameters[100];
    BOOL  Path;
    char* Content;
}MimePart;



typedef struct _EmailRecipient
{
    char Address[250];
    char Name[250];
}EMailRecipient;


typedef struct _EMailMessage
{
    char*  From;
    char*  Subject;
    char*  Enveloppe;
    char*  Header;
    char*  Body;
    int    BodySize;
    int    HeaderSize;
    int    CharsPerLine;
    List*  FromList;
	List*  ToList;
    List*  CCList;
    List*  BCCList;
    char   Date[230];
    char   MailerName[230];
    int    Size;
    int    Nbr;
    char   PartBoundary[250];
    char   MIMEText[250];
    char   MIMEContentType[250];
    List*  MIMEList;
}EMailMessage;








#ifdef __cplusplus
extern "C" {
#endif


    extern char m_ErrorMessage[500];
    extern List* RetreiveMessages;
    extern int m_NumberMail;
    extern int m_TotalSize;
    extern int m_ListSize[100];

    extern EMailMessage* EMailCreateMessage (char* from, char* to, char* cc,char* bcc, char* subject, char* body);
    extern void   EMailFreeMessage (EMailMessage* Message);
    extern int    EMailSendMessage (TPClient* pclient, EMailMessage* Message);
    extern EMailRecipient* AddRecipient(EMailMessage* Message, char* EmailAddress, char* FriendlyName, BYTE Type);
    extern BOOL   EMailDecodeHeader(EMailMessage* Message);
    extern void   EMailDecodeBody (EMailMessage* Message);

    extern int    EMailAddMultipleRecipients(EMailMessage* Message, char* szRecipients, BYTE type );
    extern void   EMailPrepareBody (EMailMessage* Message);
    extern void   EMailPrepareHeader (EMailMessage* Message);
    extern int    EMailAddMimePart(EMailMessage* Message, char* nContent, int nContentType, char* szParameters, int nEncoding,  BOOL bPath);
    extern void   EMailAppendMimeParts (EMailMessage* Message);
    extern int    EMailAppendMimePart(EMailMessage* Message, MimePart* Part);
    extern char*  EMailBuildMimeHeader(EMailMessage* Message, MimePart* Part, char* sSubHeader);
    extern int    SendEmail (TP* tp, char* servername, EMailMessage* Message);
    extern int    RetreiveEmail (TP* tp, char* servername, char* name, char* passwd);
    extern char*  Encode(char* szEncoding, int nSize, char* sOutput);


#ifdef __cplusplus
}
#endif

#endif
