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

#ifndef __MX_H__
#define __MX_H__

#include "osio.h"
#include "osmatch.h"
#include "tp.h"
#include "cgi.h"


#define MXSizeExtension      100
#define MXSizeAddress        16

#define  MXAccessSize        30 
#define  MXDateSize          30


#define MXAppliSizeName      32
#define MXSizeName           100
#define MXSizeDescription    500
#define MXSizeFileName       250


#define MXMaxMessages        50
#define MXMaxClasses         50
#define MXMaxObjects         150



#define MXNumberComInHistory      TPNumberClientInHistory
#define MXMessageHeaderSize       12
#define MXMessageStatutSize       25

#define MXFileProcessing(message)   ((message)->Context->File)
#define MXBufferProcessing(message) ((message)->Context->Buffer)


#define MXBUFVALUESIZE   4096       /* values without file contents       */
#define MXBUFFILESIZE  MAXBUFSIZE


#define STRINGFUNC(x) #x

/* Message CallBack Types */
#define MXONRECV      1
#define MXONSEND      2
#define MXONACK       3
#define MXONSTORE     4
#define MXONREAD      5 
#define MXONWRITE     6 
#define MXONPUT       7 
#define MXONGET       8 

#define MXONCONNECT   9 
#define MXONCLOSE     10 
#define MXONBROKEN    11 
#define MXONRECEIVING 12
#define MXONSENDING   13



/* Message Types */
#define MXNORMAL    0
#define MXACK       1


#define MXSHORT     1
#define MXLONG      2
#define MXWORD      3
#define MXDWORD     4
#define MXBYTE      5
#define MXFILE      6
#define MXSTRING    7
#define MXCHAR      8
#define MXBUFFER    9
#define MXDOUBLE    10


/* del with communication */

#define MXMaxCom    TPMaxClients
#define MXBlock     TRUE
#define MXNoBlock   FALSE

#define MXFROM      0
#define MXTO        1





#define MXSERVER    TPSERVER
#define MXCLIENT    TPCLIENT


#define MXGetComFromMessage(pmessage) (pmessage)->Com
#define MXGetSize(pmessage) (pmessage)->Size
#define MXGETMODE(pmx) (pmx)->Mode;
#define MXIsServer(pmx) ((pmx)->TP->Mode == MXSERVER)
#define MXGetSendSize(pcom) (pcom)->Client->TransferSize
#define MXGetRecSize(pcom) (pcom)->Client->ReceiptSize
#define MXSetTraceProcedure(pcom, proc) (pcom)->TraceProcedure = proc;
#define MXSetComApplicationField(pcom, appfield) (pcom)->ApplicationField = appfield;
#define MXSetConnectingProcedure(pmx, proc, appfield) (pmx)->ConnectingProcedure = proc; \
                                                      (pmx)->ConnectingField = appfield
#define MXGetWithUserOnCom(pcom) (pcom)->Client->WithUserId
#define MXGetWithNameOnCom(pcom) (pcom)->Client->WithName
#define MXGetApplicationField(pcom) (pcom)->ApplicationField
#define MXGetEstablish(pcom)  (pcom)->Client->Connect
#define MXComIsBlocked(pcom) TPClientIsBlocked(pcom->Client)
#define MXGetNbrObjects(pmessage) ListNbElt(pmessage->MessageClass->Objects)
#define MXGetNbrRows(pmessage) pmessage->NbrRows


#define DG_SYS       "DG"  
#define CB2A_SYS     "CB2A" 
#define FTP_SYS      "FTP"                  
#define TCP_SYS      "TCP"
#define MT_SYS       "MT"
#define POP_SYS      "POP"                  
#define SMTP_SYS     "SMTP"                   
#define HTTP_SYS     "HTTP"                   
#define DNS_SYS      "DNS"                   
#define FIOP_SYS     "FIOP"                   
#define DB_SYS       "DB"                 
#define DL_SYS       "SC"                 




#define FTP_SYS_CODE   21
#define TCP_SYS_CODE   0
#define MT_SYS_CODE    2010
#define POP_SYS_CODE   110
#define SMTP_SYS_CODE  25
#define HTTP_SYS_CODE  80
#define DNS_SYS_CODE   53
#define FIOP_SYS_CODE  10000
#define DG_SYS_CODE    30000
#define DB_SYS_CODE    30001
#define DL_SYS_CODE    30002
#define CB2A_SYS_CODE  30003



/*----------------------------------------------------------------------------*/
/* BEGIN DIALOG ENTITIES                                                      */
/*----------------------------------------------------------------------------*/

typedef struct _MXObject
{
    char   Name[MXSizeName];          /* ATTRIBUT may be optional               */
    BYTE   Type;                      /* code INT, WORD, DWORD, STRING,...      */
    WORD   Size;                      /* in case of ARRAYS                      */
    DWORD  Offset;                    /* Offset in the stream we find values    */
    SHORT  InternalType;              /* type for DataBase                      */
    LONG   InternalLength;            /* size for DataBase                      */
    LONG   InternalScale;             /* scale forDataBase                      */
    struct _MXMessageClass* MessageClass;
}MXObject;


typedef struct _MXMessageClass
{
    char   Name[MXSizeName];          /* ATTRIBUT or message name               */
    List*  Objects;                   /* list of MXObjects                      */
    WORD   Code;                      /* code of the message                    */
    DWORD  Size;                      /* check size who knows ?                 */
    List*  ExecuteOutputs;
    List*  ExecuteInputs;
    List*  ExecuteAck;
    BOOL   BlockedOnOutput;
    BOOL   BlockedOnInput;
    struct _MXDialogClass* Class;
}MXMessageClass;


typedef struct _MXDialogClass
{
    char   Name[MXSizeName];          /* ATTRIBUT or dialog name                */
    List*  MessageClasses;            /* list of all messages                   */
    WORD   Code;                      /* code of the class                      */
    struct _MX* MX;
}MXDialogClass;


/*----------------------------------------------------------------------------*/
/* END PHYSICAL ENTITIES                                                      */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* END DIALOG ENTITIES                                                        */
/*----------------------------------------------------------------------------*/




typedef struct _MXComClass
{
    char   Name[MXSizeName];          /* ATTRIBUT or dialog name                */
    List*  Objects;                   /* list of MXObjects                      */
    WORD   Code;                      /* code of the class                      */
    int    Port;
    char   TableName[MXSizeName];
    BYTE   FromToTranscod;            /* which side of com transcoding...       */
    int    Protocol;
    char   FileName[MXSizeName];
    char   FileAttributes[MXSizeName];
    char   UserName[MXSizeName];      /* User Name DataBase                     */
    char   PassWord[MXSizeName];      /* User Name DataBase                     */
    List*  ExecuteOutputs;
    List*  ExecuteInputs;
    List*  ExecuteAck;
    List*  ConnectionProcedures;      /* General Connect Callbacks              */
    List*  EndingProcedures;          /* General Ending  Callbacks              */
}MXComClass;



typedef struct _MX
{
    char   AppliName[MXAppliSizeName]; /* Application Name                      */
    List*  Classes;
    List*  ComClasses;
    List*  Communications;
    int    ComHistoryNumber;          /* number of com in hist                  */
    List*  ComHistory;
    List*  ApplicationProcedures;     /* Application Procedures                 */
    List*  ConnectionProcedures;      /* General Connect  Callbacks             */
    List*  EndingProcedures;          /* General Ending   Callbacks             */
    List*  ExecuteOutputs;
    List*  ExecuteInputs;
    List*  ExecuteAck;
    TP*    TP;                        /* transport protocol                     */
    CGI*   CGI;
    struct _MXManager* Manager;
}MX;


/*  Communication */


typedef struct _MXCom
{
    MX*    MX;
    XF*    XF;
    int    Protocol;
    int    Closed;
    List*  OutputMessages;
    List*  InputMessages;
    List*  AckMessages;
    List*  ActiveInputMessages;
    List*  ActiveOutputMessages;
    time_t CloseTime;                 /* time closing com                        */
    time_t OpenTime;                  /* time opening com                        */
    List*  InputHistory;              /* input messages in history               */
    List*  OutputHistory;             /* output messages in history              */
    List*  ExecuteOutputs;
    List*  ExecuteInputs;
    List*  ExecuteOutputStream;
    List*  ExecuteInputStream;
    List*  ExecuteAck;
    List*  EndingProcedures;          /* related to com CallBacks                */
    void*  InternalField;
    void*  ApplicationField;
    WORD   MessagesId;
    MXComClass* ComClass;
    TPClient* Client;
    int    (*TraceProcedure) (void* pmessage, BYTE type, char* stream, int size);
    struct _MXMessageClass* IOMessageClass;             /* Only for file reading */
}MXCom;




/*----------------------------------------------------------------------------*/
/* A message can be transmitted in several parts each part has the following  */
/* format : <RequestSize1><MessageHeadder1><Stream1>   ...                    */
/* for a message, MessageHeader is the same except the Part Value             */
/* Every Message have a unique identity                                       */
/*----------------------------------------------------------------------------*/


typedef struct _MXMessageHeader
{
    WORD   Identity;                  /*!< identity of the message                 */
    DWORD  Size;                      /*!< size of the whole message               */
    WORD   ClassCode;                 /*!< Class Code                              */
    WORD   MessageCode;               /*!< message Code                            */
    WORD   Part;                      /*!< Message sent in several parts           */
}MXMessageHeader;


typedef struct _MXMessageStatut
{
    BYTE   Type;                      /*!< NORMAL, ACK, STORE                      */
    DWORD  TDate;                     /*!< Transfer Date                           */
    DWORD  THour;                     /*!< Transfer Time                           */
    DWORD  PDate;                     /*!< Put Date                                */
    DWORD  PHour;                     /*!< Put Hour                                */
    DWORD  DDate;                     /*!< Departure Date                          */
    DWORD  DHour;                     /*!< Departure Hour                          */
    DWORD  ADate;                     /*!< Arrival Date in case of ACK             */
    DWORD  AHour;                     /*!< Arrival Hour in case of ACK             */
}MXMessageStatut;

/* Warning : compiling mx.cpp in medium model won't work because we call       */
/* fopen functions which work on near pointers. we should have opening         */
/* file functions that are supported by windows API                            */


typedef struct _MXMessageContext
{
    /* I/O context */
    FILEID* File;                     /*!< this is the case of I/O files           */
    char*  Buffer;                    /*!< this is the case of I/O Buffer          */
    int    Pos;                       /*!< position in the file or in the buffer   */
    char*  Stream;                    /*!< buffer file or buffer stream            */
    int    StreamPos;                 /*!< Position in FileBuffer                  */
    int    StreamSize;                /*!< Size of FileBuffer                      */
    int    ObjectOffset;              /*!< this can be object range in the message */
    int    SizeOffset;                /*!< this is the size offset in the object   */
    DWORD  StillToRead;               /*!< what is left to read for this message   */
}MXMessageContext;


typedef struct _MXMessage
{
    MXDialogClass* Class;             /*!< points to MX Class                      */
    MXMessageClass* MessageClass;     /*!< points to Message Class                 */
    MXMessageContext* Context;        /*!< context means message is  several parts */
    MXCom* Com;                       /*!< To which com it belongs                 */
    void*  InternalField;             /*!< For internal purposes                   */ 
    WORD   Part;                      /*!< part of the message                     */
    char*  Stream;                    /*!< stream contains values of objects       */
    DWORD  StreamSize;                /*!< size of stream                          */
    DWORD* Values;                    /*!< values is an array  to offset in stream */
    DWORD  Offset;                    /*!< offset in the Stream of values          */
    DWORD  Size;                      /*!< Size of the message with it values      */
    BYTE   Complete;                  /*!< whole message is received or sent       */
    WORD   Identity;                  /*!< message Identity                        */
    BYTE   Statut;                    /*!< 0, 1                                    */
    BYTE   Type;                      /*!< NORMAL, ACK, STORE                      */
    DWORD  NbrRows;                   /*!< Nbr of columns in objects of the message*/
    char   EndChar[10];               /*!< End reading special char                */
    DWORD  TDate;                     /*!< Transfer Date                           */
    DWORD  THour;                     /*!< Transfer Time                           */
    DWORD  PDate;                     /*!< Put Date                                */
    DWORD  PHour;                     /*!< Put Hour                                */
    DWORD  DDate;                     /*!< Departure Date                          */
    DWORD  DHour;                     /*!< Departure Hour                          */
    DWORD  ADate;                     /*!< Arrival Date in case of ACK             */
    DWORD  AHour;                     /*!< Arrival Hour in case of ACK             */
}MXMessage;



typedef struct _MXCallBack
{
    void* ApplicationField;
    MXMessageClass* MessageClass;
    int(*ExecuteProcedure) (MXMessage* pmessage, MXCom* pcom, void* papp);
}MXCallBack;


typedef struct _MXApplicationProcedure
{
    void* ApplicationField;
    int(*ExecuteProcedure) (MX* pmx, void* papp);
}MXApplicationProcedure;


typedef struct _MXComCallBack
{
    void*  ApplicationField;
    int  (*ExecuteProcedure) (MXCom* pcom, void* pappfield);
}MXComCallBack;



/*----------------------------------------------------------------------------*/
/*internal dialogs                                                            */


/*----------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif

    extern int        MXInit (MX* pmx, BYTE mode, char* name, char* appliname, int port, char* resourcefile);
    extern void       MXEnd (MX* pmx);

    extern MXDialogClass *  MXCreateDialogClass(MX* pmx, char* name, WORD code);
    extern void       MXFreeDialogClass(MX* pmx, MXDialogClass* pclass);

    extern MXMessageClass* MXCreateMessageClass(MX* pmx, MXDialogClass* pclass, char* name, WORD code, int nb_param, ...);
    extern void       MXFreeMessageClass (MX* pmx, MXMessageClass* pmessageclass);

    extern MXComClass* MXCreateComClass (MX* pmx, char* name, WORD code, int protocol, BYTE fromto, char* tablename);
    extern void       MXFreeComClassFromName (MX* pmx, char* name);
    extern void       MXFreeComClass (MX* pmx, MXComClass* pcomclass);


    extern MXMessage* MXCreateMessage (MX* pmx, char* classname, char* messclassname);
    extern void       MXFreeMessage (MX* pmx, MXMessage* pmessage);


    extern void       MXFreeObject (MX* pmx, MXObject* pobject);

    extern void       MXFreeCom (MX* pmx, MXCom* pcom);

    extern MXObject*  MXAddObjectToMessageClass (MXMessageClass* pmess, char* typeatt, int dim, char* nomatt);
    extern void       MXFreeObjectsFromMessageClass (MXMessageClass* pmessageclass);



    extern int        MXGetPort (MXCom* pcom);
    extern int        MXAddPort (MX* pmx, int port, int protocol);
    extern int        MXLoadResourceFile (MX *pmx, char* filename);
    extern LONG       MXDispatchEvents (MX* pmx, DWORD milliseconds);
    extern int        MXSetValue (MXMessage* pmessage, char* name, int range, void* value);
    extern void*      MXGetValue (MXMessage* pmessage, char* name, int range);

    extern int        MXSetRangeValue (MXMessage* pmessage, int Order, int Range, void* value);
    extern void*      MXGetRangeValue (MXMessage* pmessage, int Order, int range);

    extern char*      MXGetName (MXMessage* pmessage, int Order);
    extern void       MXGetType (MXMessage* pmessage, int Order, char* cType);
    extern SHORT      MXGetInternalType (MXMessage* pmessage, int Order);
    extern LONG       MXGetInternalLength (MXMessage* pmessage, int Order);
    extern LONG       MXGetInternalScale (MXMessage* pmessage, int Order);



    extern void       MXSetMessageStatut (MXMessage* pmessage, BYTE Statut);
    extern void       MXSetMessageDateHour (MXMessage* pmessage, char* date, char* hour);
    extern MXCom*     MXOpenTCP  (MX* pmx, char* machiname, int port, int protocol, char* user, char* appliname, BOOL block);
    extern MXCom*     MXOpenBase (MX* pmx, char* databasename, char* username, char* password, int protocol, BOOL block);
    extern MXCom*     MXOpenFile (MX* pmx, char* filename, char* mode, int protocol, char loc, int size);

    extern int        MXCloseCom (MX* pmx, MXCom* pcom);
    extern int        MXFlushCom (MX* pmx, MXCom* pcom);
    extern MXMessage* MXPutMessage (MXCom* pcom,char* classname, char* messclassname);
    extern int        MXPutThisMessage (MXCom* pcom, MXMessage* pmessage);
    extern int        MXJoinCom (MX* pmx, MXCom* pcom, MXCom* pcom1);
    extern MXCom *    MXGetComFromClient(MX* pmx, TPClient* pclient);
    extern int        MXAddDGCommands (MX* pmx);
    extern int        MXAddFTPCommands (MX* pmx);
    extern int        MXAddPOPCommands (MX* pmx);
    extern int        MXAddSMTPCommands (MX* pmx);
    extern int        MXAddTCPCommands (MX* pmx);
	extern int        MXAddMTCommands (MX* pmx);
    extern int        MXAddDBCommands (MX* pmx);
    extern int        MXAddFIOPCommands (MX* pmx);
    extern int        MXAddHTTPCommands (MX* pmx);
    extern int        MXAddDNSCommands (MX* pmx);
    extern MXMessage* MXCopyMessage (MXMessage* fromessage);
    extern int        MXTranferOutputQueue (MXCom* FromCom, MXCom* ToCom);


    extern MXComClass*MXGetComClassFromName(MX* pmx, char* comclassname);
    extern MXComClass*MXGetComClassFromCode (MX* pmx, WORD Code);
    extern MXDialogClass*   MXGetDialogClassFromName (MX* pmx, char* name);
    extern MXDialogClass*   MXGetDialogClassFromCode (MX* pmx, WORD code);
    extern MXMessageClass* MXGetMessageClassFromName (MXDialogClass* pclass, char* name);
    extern MXMessageClass* MXGetMessageClassFromCodes (MX* pmx, WORD classcode, WORD messagecode);
    extern MXMessageClass* MXGetMessageClassFromNames (MX* pmx, char* classname, char* messname);
    extern MXObject*  MXGetObjectFromName (MXMessageClass* pclassmess, char* name);
    extern int        MXGetProtocolFromName (char* Name);
    extern void       MXSetComProtocol (MX* pmx, MXCom* pcom, int protocol);
    extern MXMessage* MXReadyToSend (MXMessage* pmessage);
    extern MXMessage* MXGetReadyOutputMessage (MXCom* pcom);
    extern MXMessage* MXGetReadyInputMessage (MXCom* pcom);
    extern MXMessage* MXFindIncompleteMessage (List* meslist);
    extern int        MXCheckType2 (char* buf);
    extern void       MXFreeContext (MXMessageContext* pcontext);
    extern MXMessageContext*   MXAllocateContext (void);
    extern DWORD      MXExtendValueBuffer (MXMessage* pmessage, DWORD value);
    extern LONG       MXGetMessageSize (MXMessage* pmessage, BYTE withfiles);
    extern LONG       MXGetMessageClassSize (MXMessageClass* pclassmess);
    extern LONG       MXSetIOMessageClass(MXCom* pcom,MXMessageClass* pmessageclass);
    extern LONG       MXSetIOSize(MXCom* pcom,LONG LineSize);
    extern LONG       MXGetFileSize (char* fromstream, BYTE content);
    extern LONG       MXGetBufferSize (char* fromstream, BYTE content);
    extern int        MXEstablishConnectionOn (TPClient* pclient, void* app);
    extern MXCom*     MXSetCom (MX* pmx, MXComClass* pcomclass, TPClient* pclient, int protocol);
    extern int        MXExecuteApplicationProcedures (TP* ptp, void* app);
    extern int        MXReadMessageStatut (MX* pmx, char* stream, MXMessageStatut* pstatut);
    extern int        MXWriteBufferContent (MX* pmx, MXMessageContext* pcontext, char* stream, char* attributes, int maxcount);
    extern MXMessage* MXFindMessage (List* meslist, WORD Identity);
    extern LONG       MXReadFileContent (MX* pmx, MXMessageContext* pcontext, char* stream, char* attributes, WORD maxcount);
    extern int        MXReadBufferContent (MX* pmx, MXMessageContext* pcontext, char* fromstream, char* attributes, int maxcount);
    extern LONG       MXWriteFileContent (MX* pmx, MXMessageContext* pcontext, char* stream,char* attributes, WORD maxcount);
    extern MXMessage* MXCreateMessageFromCodes(MX* pmx, WORD classcode, WORD messclasscode);
    extern MXMessage* MXAllocMessage (MXDialogClass* pclassmessage, MXMessageClass* pmessageclass);
    extern int        MXInitValues (MXMessage* pmessage);

    extern int        MXSetMode (MX* pmx, char* classname, char* messname, BYTE mode, BOOL Blocked);
    extern LONG       MXSend (MX* pmx, MXCom* pcom, MXMessage* pmessage);
    extern MXMessage* MXRecv (MX* pmx, MXCom* pcom);
    extern void       MXInitMessage (MXMessage* pmessage);
    extern int        MXFreeAndMoveComIn (TPClient* pclient, void* par);
    extern int        MXSetFileValue (MXMessage* pmessage , void* Value);
    extern int        MXSetBufferValue (MXMessage* pmessage,  void* Value);
    extern int        MXSetStringValue (MXMessage* pmessage,  void* Value);

    extern MXApplicationProcedure* MXFindApplicationProcedure (List* executelist, int (*funct)(MX* pmx, void* applicationfield), void* applicationfield);
    extern MXCallBack* MXFindCallBack (MXMessageClass* pmessclass, List* executelist, int (*funct)(MXMessage* pmessage,MXCom* pcom, void* applicationfield), void* applicationfield);
    extern MXCallBack* MXFindGeneralCallBack (List* ExecuteList, int (*funct)(MXMessage* pmessage, MXCom* pcom,void* applicationfield), void* applicationfield);
    extern MXComCallBack* MXFindComCallBack (List* ExecuteList, int (*funct)(MXCom*, void*), void* appfield);

    extern int        MXAddGeneralCallBack (MX* pmx, char* classname, char* messname, BYTE mode, int (*funct)(MXMessage*  pmessage, MXCom* pcom, void* applicationfield), void* applicationfield);
    extern int        MXRemoveGeneralCallBack (MX* pmx, char* classname, char* messname, BYTE mode, int (*funct)(MXMessage* pmessage, MXCom* pcom, void* applicationfield), void* applicationfield);
    extern int        MXAddApplicationProcedure (MX* pmx, int (*funct)(MX* pmx, void* applicationfield), void* applicationfield);
    extern int        MXRemoveApplicationProcedure (MX* pmx, int (*funct)(MX* pmx, void* applicationfield), void* applicationfield);
    extern int        MXAddGeneralConnectCallBack (MX* pmx, BYTE mode, int (*funct)(MXCom* pcom, void* applicationfield), void* applicationfield);
    extern int        MXRemoveGeneralConnectCallBack (MX* pmx, BYTE mode, int (*funct)(MXCom* pcom, void* applicationfield), void* applicationfield);
    extern int        MXAddConnectCallBack (MX* pmx, char* comclass, BYTE mode, int (*funct)(MXCom* pcom, void* applicationfield), void* applicationfield);
    extern int        MXRemoveConnectCallBack (MX* pmx, char* comclass, BYTE mode, int (*funct)(MXCom* pcom, void* applicationfield), void* applicationfield);
    extern int        MXAddCallBack (MX* pmx, char* comclass, char* classname, char* messname, BYTE mode, int (*funct)(MXMessage* pmessage, MXCom* pcom, void* applicationfield), void* applicationfield);
    extern int        MXRemoveCallBack (MX* pmx, char* comclass ,char* classname, char* messname, BYTE mode, int (*funct)(MXMessage* pmessage, MXCom* pcom, void* applicationfield), void* applicationfield);
    extern int        MXAddComCallBack (MX* pmx, MXCom* pcom, char* classname, char* messname, BYTE mode, int (*funct)(MXMessage* pmessage, MXCom* pcom, void* applicationfield), void* applicationfield);
    extern int        MXRemoveComCallBack (MX* pmx, MXCom* pcom, char* classname, char* messname, BYTE mode, int (*funct)(MXMessage*, MXCom* pcom, void* applicationfield), void* applicationfield);
    extern int        MXAddEndingProcedure (MXCom* pcom, int (*funct)(MXCom* pcom, void* applicationfield), void* applicationfield);
    extern int        MXRemoveEndingProcedure (MXCom* pcom, int (*funct)(MXCom* pcom, void* applicationfield), void* applicationfield);

    extern int        MXSetCGIFile (MX* pmx, char* FileName, FILEID* outfile);



#ifdef __cplusplus
}
#endif



#endif /* __MX_H__*/
