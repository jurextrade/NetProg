/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


/**
*  NetProg 
*  NetProg is intended to all people wanting to program applications through the network and which must be carried out on various operating systems.
*  They ask a knowledge of the C language.
*  
*  Which type of applications ?
*  
*  The field of application is very large, from a simple application of file transfers between two machines to applications needing a complex protocol of data exchange 
*  in synchronous or asynchronous mode.
*  
*  Positioning
*  
*  This product is single because it makes it possible to cover all types and needs for communications : synchronous or asynchronous 
*  based on a homogeneous architecture of événementiel type.
*  
*  Strong Points
*  
*  Profit of 80 % of time and means in developments of communicating applications multi - platforms on TCP / IP.API multi - platforms : UNIX, Windows, IBM, … Transcoding of characters.Newspaper of transfers allowing to index the transfers carried out with mechanisms of recovery.Interpreted language allowing instantaneous programming of orders distributed on the various machines.Connections to data bases.Synchronous or asynchronous communication.Programming of low level : definition of new protocols.
*  
*  The programmer is freed from the heavy tasks related to the development of client - server applications programming on TCP / IP, portability, synchronizations, transcoding, …
*  
*  It is particularly useful for programming of applications type Web servers which can handle a large number or connections.
*/

#define _CRT_SECURE_NO_WARNINGS
#include "mx.h"
#include "mxmanager.h"
#include "dg.h"
#include "tcp.h"
#include "ftp.h"
#include "mt.h"
#include "../include/http.h"
#include "dns.h"
#include "fiop.h"
#include "smtp.h"
#include "pop.h"
#include "database.h"
#include "cb2a.h"

/**
* This function is used to initialize th MX manager.
* This is the first function to call in your program, it allocates all resources related to MX.

* If mode is MXCLIENT, the MX manager does not accept income connections, but the application can connect to other applications.
*    In this mode MXDispatchEvents() called with the parameter miliseconds equal to 0, doesn’t return as long as there is still opened connections in the application.
*    When all connections are closed MXDispatchEvents returns

* If mode is MXSERVER the MX manager accepts all income connections on the ports added with MXAddPort function.
*    In this mode MXDispatchEvents called with the parameter miliseconds to 0, blocks indefinetly port corresponds to initial listening port with IPPROTO_DG protocol.
*    Giving a port in the function is equivalent to MXAddPort (port, IPPROTO_DG)
* appliname is the name of your application, can be NULL
* name is tne name of your program, can be NULL
* 
* \param pmx the MX manager
* \param mode the mode of initialisation (MXSERVER, MXCLIENT)
* \param name the name of the program if using local connections
* \param appliname  the name of the application
* \param port the listening port if mode is MXSERVER
* \param resourcefile the resource file to loa
* \sa MXDispatchEvents(), MXEnd()
* \return if error return -1 else returns > 0
*/
int MXInit (MX* pmx, BYTE mode, char* name, char* appliname, int port, char* ResourceFile)
{
    int ret;
    TP* ptp;
    CGI* pcgi;
    char machinename[MXSizeName];
    char machineaddress[MXSizeName];

    memset (pmx, 0, sizeof (MX));

    ptp = (TP*)malloc (sizeof (TP));
    if (!ptp) return -1;
    ret =   TPInit (ptp, name, appliname);
    if (ret < 0)
    {
        free (ptp);
        return ret;
    }
    pcgi = (CGI*)malloc (sizeof (CGI));
    if (!pcgi) return -1;

    ret = CGIInit (pcgi, NULL, NULL);

    pmx->CGI              = pcgi;
    pmx->TP               = ptp;
    pmx->TP->Mode         = mode;
    pmx->ComHistory       = NULL;
    pmx->ComHistoryNumber = 0;
    pmx->Communications   = NULL;
    pmx->Classes          = NULL;
    pmx->ComClasses       = NULL;
    pmx->ApplicationProcedures = NULL;
    pmx->ConnectionProcedures = NULL;
    pmx->EndingProcedures = NULL;
    pmx->ExecuteInputs    = NULL;
    pmx->ExecuteOutputs   = NULL;
    pmx->ExecuteAck       = NULL;
    if (mode == MXSERVER)
    {
        if (name) ret = TPListenToLocal (ptp);
        ret = MXAddPort (pmx, port, IPPROTO_DG);
    }
    TPAddApplicationProcedure (ptp, MXExecuteApplicationProcedures, pmx);
    TPAddConnectionProcedure (ptp, MXEstablishConnectionOn, pmx);
    MXAddDGCommands  (pmx);
    MXAddFTPCommands (pmx);
    MXAddSMTPCommands (pmx);
    MXAddPOPCommands (pmx);
    MXAddTCPCommands (pmx);
	MXAddMTCommands (pmx);
    MXAddFIOPCommands(pmx);
    MXAddHTTPCommands(pmx);
    MXAddDNSCommands (pmx);
    MXAddDBCommands (pmx);

    pmx->Manager = (MXManager*)malloc (sizeof (MXManager));
    memset (pmx->Manager, 0, sizeof (MXManager));

    if (ResourceFile) MXLoadResourceFile (pmx, ResourceFile);

    if (appliname)
    {
        strncpy (pmx->AppliName, appliname, (strlen (appliname) >= MXAppliSizeName) ? (MXAppliSizeName - 1) : strlen (appliname));
    }

    if (OSGetHostName (machinename, MXSizeName - 1) != -1)
    {
        OSGetAddrFromName (machinename, machineaddress);
        strcpy (pmx->Manager->LocalMachineName, machinename);
        strcpy (pmx->Manager->LocalAddress , machineaddress);
    }


    return ret;
}

/**
* This function terminates the MX session of an application.
* Usually it is called at the end of the program before exiting.
* It frees all resources, closes all the connections.
* 
* \param pmx the MX manager
* \sa MXInit()
* \return no return value
*/

void MXEnd(MX* pmx)
{
    List* list;
    MXApplicationProcedure* pexecuteprocedure;
    MXComCallBack* pconnectprocedure;
    MXCallBack* pgeneralprocedure;

    if (!pmx) return;
    if (pmx->Manager) free(pmx->Manager);

    list = pmx->Classes;

    if (pmx->CGI)
    {
        CGIReturn(pmx->CGI, pmx->CGI->Out);
        CGIEnd(pmx->CGI);
        free((BYTE*)pmx->CGI);
    }
    if (pmx->TP)
    {
        TPEnd(pmx->TP);
        free((BYTE*)pmx->TP);
    }

    while (pmx->Classes)
    {

        MXDialogClass* pclass = (MXDialogClass*)pmx->Classes->car;
        ListRemove(&pmx->Classes, pclass);
        MXFreeDialogClass(pmx, pclass);
    }

    while (pmx->ComClasses)
    {

        MXComClass* pcomclass = (MXComClass*)pmx->ComClasses->car;
        ListRemove(&pmx->ComClasses, pcomclass);
        MXFreeComClass(pmx, pcomclass);
    }
    while (pmx->ComHistory)
    {

        MXCom* pcom = (MXCom*)pmx->ComHistory->car;
        ListRemove(&pmx->ComHistory, pcom);
        MXFreeCom(pmx, pcom);
    }

    while (pmx->ApplicationProcedures)
    {
        pexecuteprocedure = (MXApplicationProcedure*)pmx->ApplicationProcedures->car;
        ListRemove(&pmx->ApplicationProcedures, pexecuteprocedure);
        free(pexecuteprocedure);
    }
    while (pmx->ConnectionProcedures)
    {
        pconnectprocedure = (MXComCallBack*)pmx->ConnectionProcedures->car;
        ListRemove(&pmx->ConnectionProcedures, pconnectprocedure);
        free(pconnectprocedure);
    }

    while (pmx->EndingProcedures)
    {
        pconnectprocedure = (MXComCallBack*)pmx->EndingProcedures->car;
        ListRemove(&pmx->EndingProcedures, pconnectprocedure);
        free(pconnectprocedure);
    }
    while (pmx->ExecuteInputs)
    {
        pgeneralprocedure = (MXCallBack*)pmx->ExecuteInputs->car;
        ListRemove(&pmx->ExecuteInputs, pgeneralprocedure);
        free(pgeneralprocedure);
    }
    while (pmx->ExecuteOutputs)
    {
        pgeneralprocedure = (MXCallBack*)pmx->ExecuteOutputs->car;
        ListRemove(&pmx->ExecuteOutputs, pgeneralprocedure);
        free(pgeneralprocedure);
    }

    while (pmx->ExecuteAck)
    {
        pgeneralprocedure = (MXCallBack*)pmx->ExecuteAck->car;
        ListRemove(&pmx->ExecuteAck, pgeneralprocedure);
        free(pgeneralprocedure);
    }


    pmx = NULL;
    /* should free all messages in history */
}


int MXSetCGIFile (MX* pmx, char* FileName, FILEID* fileout)
{

    if (!FileName) return -1;
    if (fileout) pmx->CGI->Out = fileout->File;
    return (CGIParseFile (pmx->CGI, FileName));
}


int MXAddPort (MX* pmx, int Port, int Protocol)
{

    pmx->TP->Mode = MXSERVER;
    return TPListenToTCP (pmx->TP, Port, Protocol);
}


int MXGetPort (MXCom* pcom)
{
    if (!pcom->Client->OnPort) return -1;
    return pcom->Client->OnPort->Port;
}


int MXSetValue (MXMessage* pmessage, char* Name, int Range, void* Value)
{
    int  size = 0;
    MXObject*  pobject;
    MXMessageClass* pclassmess = pmessage->MessageClass;
    char* tostream = pmessage->Stream + pmessage->Offset;

    pobject = MXGetObjectFromName (pclassmess, Name);

    if (!pobject)
    {
        OSError ("TEDAG013", " (%s)", Name);
        return -1;
    }

    if ((Range > pobject->Size) || (Range < 1))
    {
        OSError ("TEDAG014", " (%d)", Range);
        return -1;
    }

    switch (pobject->Type) {
        case MXSHORT :
            size = sizeof (SHORT);
            pmessage->Values[(WORD)pobject->Offset + (Range - 1)] = pmessage->Offset;
            if (Value) memcpy (tostream, (char*)Value, size);
            break;
        case MXWORD :
            size = sizeof (WORD);
            pmessage->Values[(WORD)pobject->Offset + (Range - 1)] = pmessage->Offset;
            if (Value) memcpy (tostream, (char*)Value, size);
            break;
        case MXDWORD :
        case MXLONG :
            size = sizeof (DWORD);
            pmessage->Values[(WORD)pobject->Offset + (Range - 1)] = pmessage->Offset;
            if (Value) memcpy (tostream, (char*)Value, size);
            break;
        case MXDOUBLE :
            size = sizeof (DOUBLE);
            pmessage->Values[(WORD)pobject->Offset + (Range - 1)] = pmessage->Offset;
            if (Value) memcpy (tostream, (char*)Value, size);
            break;
        case MXBYTE :
            size = sizeof (BYTE);
            pmessage->Values[(WORD)pobject->Offset + (Range - 1)] = pmessage->Offset;
            if (Value) memcpy (tostream, (char*)Value, size);
            break;
        case MXCHAR :
            size = pobject->Size;
            pmessage->Values[(WORD)pobject->Offset] = pmessage->Offset;
            if (Value) memcpy (tostream, (char*)Value, size);
            break;
        case MXFILE :
            pmessage->Values[(WORD)pobject->Offset + (Range - 1)] = pmessage->Offset;
            size = MXSetFileValue(pmessage,  (char*)Value);
            break;
        case MXBUFFER :
            pmessage->Values[(WORD)pobject->Offset + (Range - 1)] = pmessage->Offset;
            size = MXSetBufferValue(pmessage, (char*)Value);
            if (size < 0) return -1;
            break;
        case MXSTRING :
            pmessage->Values[(WORD)pobject->Offset + (Range - 1)] = pmessage->Offset;
            size = MXSetStringValue(pmessage, (char*)Value);
            if (size < 0) return -1;
            /*if (Value)
                   {
                     strcpy (tostream , (char*)Value);
                     size = strlen ((char*)Value) + 1;
                   }*/
            break;
        default:
            OSError ("TEDAG0015", NULL);
            return -1;

    }
    pmessage->Offset += size;
    return size;
}


void* MXGetValue (MXMessage* pmessage, char* Name, int Range)
{
    int size;
    MXObject*  pobject;
    char* stream;
    FILEPARM* file;
    BUFFERPARM* buffer;

    pobject = MXGetObjectFromName (pmessage->MessageClass, Name);
    if (!pobject)
    {
        OSError ("TEDAG016", " (%s)", Name);
        return NULL;
    }

    if ((Range > pobject->Size) || (Range < 1))
    {
        OSError ("TEDAG017", " (%d)", Range);
        return NULL;
    }

    /* no range is possible for char */
    if (pobject->Type == MXCHAR) Range = 1;

    /* return value can be null */

    stream = (pmessage->Stream + pmessage->Values[(int)pobject->Offset + (Range - 1)]);
    switch (pobject->Type) {
        case MXFILE:
            /* special treatment for FILEPARM Structure to resolve alignment */
            file = (FILEPARM*)malloc(sizeof(FILEPARM));
            if (!file) return NULL;

            size = FILENAMESIZE;
            strncpy(file->FileOri, stream, size);
            stream += size;
            strncpy(file->FileDest, stream, size);
            stream += size;
            file->FileOriLocal = *(CHAR*)stream;
            stream += sizeof(CHAR);
            file->FileDestLocal = *(CHAR*)stream;
            stream += sizeof(CHAR);
            file->FileType = *(CHAR*)stream;
            stream += sizeof(CHAR);

            OSCopy4(&file->FileSize, stream);
            stream += sizeof(DWORD);
            OSCopy4(&file->FileError, stream);
            stream += sizeof(LONG);
            return (void*)file;
            break;
        case MXBUFFER:
            /* special treatment for BUFFERPARM Structure to resolve alignment */
            buffer = (BUFFERPARM*)malloc(sizeof(BUFFERPARM));
            if (!buffer) return NULL;

            OSCopy4(&buffer->BufferSize, stream);
            stream += sizeof(DWORD);
            buffer->BufferType = *(CHAR*)stream;
            stream += sizeof(CHAR);

            buffer->BufferContent = (void*)malloc(buffer->BufferSize);
            if (!buffer->BufferContent)
                return (void*)buffer;
            memcpy(buffer->BufferContent, stream, buffer->BufferSize);
            return (void*)buffer;
            break;
        case MXWORD:
        case MXSHORT:
        case MXDWORD:
        case MXLONG:
        case MXDOUBLE:
            return (void*)stream;
            break;
        default:
            return (void*)(pmessage->Stream + pmessage->Values[(int)pobject->Offset + (Range - 1)]);
            break;
    }
}


/*
  return value < 0 :
    -1 : error in writing messages but communication can persist 
    -2 : error in comm the communication is closed
  return value = 0 (evrything is written)
  return value > 0 (Ewouldblock)
*/

int MXFlushCom (MX* pmx, MXCom* pcom)
{
    TPClient* pclient;
    if (!pcom) return 1;
    pclient = pcom->Client;

    if (pclient)
        return (int)TPFlushClientOutput (pmx->TP, pclient);

    return -2;

}


int MXCloseCom (MX* pmx, MXCom* pcom)
{

    TPClient* pclient;
    if (!pcom) return 1;
    pclient = pcom->Client;


    if (pclient)
        TPCloseDownClient (pmx->TP, pclient);

    return 1;
}


MXCom* MXOpenTCP (MX* pmx, char* machiname, int port, int protocol, char* user, char* appliname, BOOL block)
{
    TPClient* pclient;
    MXCom* pcom;

    if ((protocol !=  IPPROTO_ICMP) &&
            (protocol !=  IPPROTO_IP)   &&
            (protocol !=  IPPROTO_TCP)  &&
            (protocol !=  IPPROTO_MTT)  &&
            (protocol !=  IPPROTO_DG)   &&
	        (protocol !=  IPPROTO_CB2A) &&
            (protocol !=  IPPROTO_UDP)  &&
            (protocol !=  IPPROTO_DNS)  &&
            (protocol !=  IPPROTO_HTTP) &&
            (protocol !=  IPPROTO_FTP)  &&
            (protocol !=  IPPROTO_SMTP) &&
            (protocol !=  IPPROTO_POP)  &&
            (protocol !=  IPPROTO_BC))
        return NULL;

    pclient = TPEstablishConnectionWith (pmx->TP, machiname,  port, protocol, user, appliname, block);
    if (!pclient)   return NULL;

    pcom = MXGetComFromClient (pmx, pclient);
    if (!pcom)
        pcom = MXSetCom (pmx, NULL, pclient, protocol);

    if (pcom && (protocol == IPPROTO_FTP))
    {
        MXAddGeneralConnectCallBack (pmx, MXONCONNECT, FTPOpenActiveConnection ,pcom);
        MXAddEndingProcedure (pcom, FTPClose, NULL);
        MXAddComCallBack(pmx,  pcom, FTP_SYS,  "Command" ,       MXONSEND,   SYS_Command,       NULL);
        MXAddComCallBack(pmx,  pcom, FTP_SYS,  "Command_Reply" , MXONRECV,   SYS_Command_Reply, NULL);
        MXAddComCallBack(pmx,  pcom, FTP_SYS,  "put"           , MXONSEND,   SYS_put,           NULL);
        MXAddComCallBack(pmx,  pcom, FTP_SYS,  "get"           , MXONSEND,   SYS_get,           NULL);
        MXAddComCallBack(pmx,  pcom, FTP_SYS,  "dir"           , MXONSEND,   SYS_dir,           NULL);
    }
    else
        if (pcom && (protocol == IPPROTO_SMTP))
        {
            MXAddComCallBack(pmx,  pcom, SMTP_SYS,  "Command" ,       MXONSEND,   SYS_SMTPCommand,       NULL);
            MXAddComCallBack(pmx,  pcom, SMTP_SYS,  "Command_Reply" , MXONRECV,   SYS_SMTPCommand_Reply, NULL);
            SMTPAddFirstReplyMessage (pcom);
        }
        else
            if (pcom && (protocol == IPPROTO_HTTP))
            {
                MXAddEndingProcedure (pcom, HTTPCloseConnection, NULL);
            }
            else
                if (pcom && (protocol == IPPROTO_POP))
                {
                    MXAddComCallBack(pmx,  pcom, POP_SYS,  "Command" ,       MXONSEND,   SYS_POPCommand,       NULL);
                    MXAddComCallBack(pmx,  pcom, POP_SYS,  "Command_Reply" , MXONRECV,   SYS_POPCommand_Reply, NULL);
                    POPAddFirstReplyMessage (pcom);
                }


    return pcom;
}




MXCom* MXOpenBase (MX* pmx, char* databasename, char* username, char* password, int protocol, BOOL block)
{
    TPClient* pclient;
    MXCom* pcom;

    if ((protocol !=  DBPROTO_ORACLE) &&
            (protocol !=  DBPROTO_ODBC) &&
            (protocol !=  DBPROTO_SQLDS)
       )
        return NULL;
    pclient = TPOpenBase (pmx->TP, databasename, username, password,  protocol);
    if (!pclient)   return NULL;
    pcom = MXGetComFromClient (pmx, pclient);
    if (!pcom)
        pcom = MXSetCom (pmx, NULL, pclient, protocol);

    return pcom;
}


MXCom* MXOpenFile (MX* pmx, char* filename, char* mode, int protocol, char loc, int size)
{
    TPClient* pclient;
    MXCom* pcom;

    int error = 0;


    if ((protocol !=  IOPROTO_STORE) &&
        (protocol !=  IOPROTO_FIOP))
        return NULL;
    pclient = TPOpenFile (pmx->TP, filename, mode, protocol, loc, size);
    if (!pclient)   return NULL;

    pcom = MXGetComFromClient (pmx, pclient);
    if (!pcom)
        pcom = MXSetCom (pmx, NULL, pclient, protocol);

    return pcom;
}



int MXJoinCom (MX* pmx, MXCom* pcom, MXCom* pcom1)
{
    return TPJoinClients (pmx->TP, pcom->Client, pcom1->Client);
}



LONG MXDispatchEvents (MX* pmx, DWORD milliseconds)
{
    TP* ptp = pmx->TP;
    return TPDispatchEvents (ptp, milliseconds);
}


MXMessage* MXRecv (MX* pmx, MXCom* pcom)
{
    MXMessage* pmessage = NULL;
    switch (pcom->Protocol)
    {
    case IPPROTO_TCP :
        pmessage = TCPRecv (pmx, pcom);
        break;
    case IPPROTO_MTT :
        pmessage = MTRecv (pmx, pcom);
        break;
    case IPPROTO_DNS :
        return TCPRecv (pmx, pcom);
        break;
    case IPPROTO_HTTP:
        return HTTPRecv (pmx, pcom);
        break;
    case IPPROTO_FTP :
        pmessage =  FTPRecv (pmx, pcom);
        break;
    case IPPROTO_SMTP :
        pmessage =  SMTPRecv (pmx, pcom);
        break;
    case IPPROTO_POP :
        pmessage =  POPRecv (pmx, pcom);
        break;
    case IPPROTO_DG :
    case IOPROTO_STORE :
        pmessage =  DGRecv (pmx, pcom);
        break;
    case IPPROTO_CB2A :
        pmessage =  CB2ARecv (pmx, pcom);
        break;
    case IOPROTO_FIOP :
        pmessage =  FIOPRecv (pmx, pcom);
        break;
    case DBPROTO_ODBC :
    case DBPROTO_ORACLE :
        return DBRecv (pmx, pcom);
        break;
    }
    return pmessage;
}


LONG MXSend (MX* pmx, MXCom* pcom, MXMessage* pmessage)
{
    pmessage->Complete = 0;
    switch (pcom->Protocol)
    {
    case IPPROTO_TCP :
        return TCPSend (pmx, pcom, pmessage);
        break;
    case IPPROTO_MTT :
        return MTSend (pmx, pcom, pmessage);
        break;
    case IPPROTO_DNS :
        return TCPSend (pmx, pcom, pmessage);
        break;
    case IPPROTO_HTTP:
        return HTTPSend (pmx, pcom, pmessage);
        break;
    case IPPROTO_FTP :
        return FTPSend (pmx, pcom, pmessage);
        break;
    case IPPROTO_SMTP :
        return SMTPSend (pmx, pcom, pmessage);
        break;
    case IPPROTO_POP :
        return POPSend (pmx, pcom, pmessage);
        break;
    case IPPROTO_DG :
    case IOPROTO_STORE :
        return DGSend (pmx, pcom, pmessage);
        break;
    case IPPROTO_CB2A :
        return CB2ASend (pmx, pcom, pmessage);
        break;
    case IOPROTO_FIOP :
        return FIOPSend (pmx, pcom, pmessage);
        break;
    case DBPROTO_ODBC :
    case DBPROTO_ORACLE :
        return DBSend (pmx, pcom, pmessage);
        break;

    }
    return -1;
}

void MXSetComProtocol (MX* pmx, MXCom* pcom, int type)
{
    TPSetClientProtocol (pmx->TP, pcom->Client, type);
    pcom->Protocol = type;
    switch (type)
    {
    case IPPROTO_ICMP :
        TPSetReceiveFunction (pmx->TP, pcom->Client, TPReceiveBuf);
        TPSetSendFunction    (pmx->TP, pcom->Client, TPSendBuf);
        break;
    case IPPROTO_IP :
        TPSetReceiveFunction (pmx->TP, pcom->Client, TPReceiveBuf);
        TPSetSendFunction    (pmx->TP, pcom->Client, TPSendBuf);
        break;
    case IPPROTO_UDP :
        TPSetReceiveFunction (pmx->TP, pcom->Client, TPReceiveBuf);
        TPSetSendFunction    (pmx->TP, pcom->Client, TPSendBuf);
        break;
    case IPPROTO_FTP :
        TPSetReceiveFunction (pmx->TP, pcom->Client, FTPReceiveMessage);
        TPSetSendFunction    (pmx->TP, pcom->Client, FTPSendMessage);
        break;
    case IPPROTO_TCP :
        TPSetReceiveFunction (pmx->TP, pcom->Client, TCPReceiveMessage);
        TPSetSendFunction    (pmx->TP, pcom->Client, TCPSendMessage);
        break;
    case IPPROTO_MTT :
        TPSetReceiveFunction (pmx->TP, pcom->Client, MTReceiveMessage);
        TPSetSendFunction    (pmx->TP, pcom->Client, MTSendMessage);
        break;
	case IPPROTO_HTTP :
        TPSetReceiveFunction (pmx->TP, pcom->Client, HTTPReceiveMessage);
        TPSetSendFunction    (pmx->TP, pcom->Client, HTTPSendMessage);
        break;
    case IPPROTO_DNS :
        TPSetReceiveFunction (pmx->TP, pcom->Client, DNSReceiveMessage);
        TPSetSendFunction    (pmx->TP, pcom->Client, DNSSendMessage);
        break;
    case IPPROTO_BC :
        TPSetReceiveFunction (pmx->TP, pcom->Client, TPReceiveBuf);
        TPSetSendFunction    (pmx->TP, pcom->Client, TPSendBuf);
        break;
    case IPPROTO_DG :
    case IOPROTO_STORE :
        TPSetReceiveFunction (pmx->TP, pcom->Client, DGReceiveMessage);
        TPSetSendFunction    (pmx->TP, pcom->Client, DGSendMessage);
        break;
	case IPPROTO_CB2A :
        TPSetReceiveFunction (pmx->TP, pcom->Client, CB2AReceiveMessage);
        TPSetSendFunction    (pmx->TP, pcom->Client, CB2ASendMessage);
        break;
    case IPPROTO_SMTP :
        TPSetReceiveFunction (pmx->TP, pcom->Client, SMTPReceiveMessage);
        TPSetSendFunction    (pmx->TP, pcom->Client, SMTPSendMessage);
        break;
    case IPPROTO_POP :
        TPSetReceiveFunction (pmx->TP, pcom->Client, POPReceiveMessage);
        TPSetSendFunction    (pmx->TP, pcom->Client, POPSendMessage);
        break;
    case IOPROTO_FIOP :
        TPSetReceiveFunction (pmx->TP, pcom->Client, FIOPReceiveMessage);
        TPSetSendFunction    (pmx->TP, pcom->Client, FIOPSendMessage);
        break;
    case DBPROTO_ORACLE :
    case DBPROTO_ODBC :
    case DBPROTO_SQLDS :
        TPSetReceiveFunction (pmx->TP, pcom->Client, DBReceiveMessage);
        TPSetSendFunction    (pmx->TP, pcom->Client, DBSendMessage);
        break;


    }
    return;
}


/*----------------------------------------------------------------------------*/
int MXAddDGCommands (MX* pmx)
{

    MXDialogClass * pclass;


    pclass = MXCreateDialogClass(pmx, DG_SYS, DG_SYS_CODE);

    MXCreateMessageClass(pmx, pclass, "SendFile",  1, 5, 
						"STRING", 1, "Command",
						"LONG",   1, "Identity",
						"SHORT",  1, "NbFiles",
						"SHORT",  1, "NbFile",
						"FILE",   1, "File");

    MXCreateMessageClass(pmx, pclass, "GetFiles",    2, 7, 
						"STRING", 1, "Command",
						"LONG",   1, "Identity",
						"STRING", 1, "FileNames",
						"STRING", 1, "LocalRepertory",
						"CHAR",   1, "FileType",
						"CHAR",   1, "LocalFileLoc",
						"CHAR",   1, "DistantFileLoc");
    MXCreateMessageClass(pmx, pclass, "GetFile",      3, 7, 
                        "STRING", 1, "Command",
                        "LONG",   1, "Identity",
                        "STRING", 1, "FileName",
                        "STRING", 1, "DestName",
                        "CHAR",   1, "FileType",
                        "CHAR",   1, "LocalFileLoc",
                        "CHAR",   1, "DistantFileLoc");

    MXCreateMessageClass(pmx, pclass, "RemoveFiles", 4, 4, 
                        "STRING", 1, "Command",
                        "LONG",   1, "Identity",
                        "STRING", 1, "FileNames",
                        "CHAR",   1, "FileLoc");
    MXCreateMessageClass(pmx, pclass, "RenameFile",  5, 5,
                        "STRING", 1, "Command",
                        "LONG",   1, "Identity",
                        "STRING", 1, "OldName",
                        "STRING", 1, "NewName",
                        "CHAR",   1, "FileLoc");
    MXCreateMessageClass(pmx, pclass, "FileReply",   6, 6,
                        "STRING", 1, "Command",
                        "LONG",   1, "Identity",
                        "STRING", 1, "FileName",
                        "SHORT",  1, "NbFiles",
                        "SHORT",  1, "NbFile",
                        "LONG",   1, "Error");


    MXCreateMessageClass(pmx, pclass, "ChangeDir",   7, 4, 
                        "STRING", 1, "Command",
                        "LONG",   1, "Identity",
                        "STRING", 1, "NewDir",
                        "CHAR",   1, "FileLoc");

    MXCreateMessageClass(pmx, pclass, "MakeDir",     8, 4, 
                        "STRING", 1, "Command",
                        "LONG",   1, "Identity",
                        "STRING", 1, "Dir",
                        "CHAR",   1, "FileLoc");

    MXCreateMessageClass(pmx, pclass, "RemoveDir",   9, 4, 
                        "STRING", 1, "Command",
                        "LONG",   1, "Identity",
                        "STRING", 1, "Dir",
                        "CHAR",   1, "FileLoc");

    MXCreateMessageClass(pmx, pclass, "GetDir",      10, 3, 
                        "STRING", 1, "Command",
                        "LONG",   1, "Identity",
                        "CHAR",   1, "FileLoc");

    MXCreateMessageClass(pmx, pclass, "DirReply",    11, 4, 
                        "STRING", 1, "Command",
                        "LONG",   1, "Identity",
                        "STRING", 1, "Dir",
                        "LONG",   1, "Error");

    MXCreateMessageClass(pmx, pclass, "PutError",    12, 4, 
                        "STRING", 1, "Param",
                        "LONG",   1, "Identity",
                        "SHORT",  1, "Stop",
                        "LONG",   1, "Error");

    MXCreateMessageClass(pmx, pclass, "AckMessage",          13, 4, 
                        "LONG",   1, "Identity",
                        "DWORD",  1, "Date",
                        "DWORD",  1, "Hour",
                        "LONG",   1, "Error");

    MXCreateMessageClass(pmx, pclass, "SendClassConnection", 14, 5, 
                        "STRING",   1, "Name",
                        "WORD",     1, "Port",
                        "WORD",     1, "Code",
                        "STRING",   1, "TableName",
                        "BYTE",    1, "FromTo");

    MXCreateMessageClass(pmx, pclass, "ReplyClassConnection",15, 2, 
                        "STRING",   1, "Name",
                        "LONG",     1, "Error");


    MXCreateMessageClass(pmx, pclass, "List",                16, 4, 
                        "STRING", 1, "Command",
                        "LONG",   1, "Identity",
                        "STRING", 1, "Name",
                        "CHAR",   1, "FileLoc");

    MXCreateMessageClass(pmx, pclass, "ListReply",           17, 9, 
                        "STRING", 1, "Command",
                        "LONG",   1, "Identity",
                        "STRING", 1, "Name",
                        "STRING", 1, "Access",
                        "STRING", 1, "Date",
                        "BYTE",   1, "Type",
                        "LONG",   1, "Size",
                        "SHORT",  1, "NbFiles",
                        "SHORT",  1, "NbFile");

    MXCreateMessageClass(pmx, pclass, "Dir",                 18, 4, 
                        "STRING", 1, "Command",
                        "LONG",   1, "Identity",
                        "STRING", 1, "Name",
                        "CHAR",   1, "FileLoc");

    MXCreateMessageClass(pmx, pclass, "GDirReply",           19, 3, 
                        "STRING", 1, "Command",
                        "LONG",   1, "Identity",
                        "BUFFER", 1, "Buffer");

    MXAddGeneralCallBack(pmx,  DG_SYS, "SendClassConnection", MXONRECV, SYS_SendClassConnection, NULL);
    MXAddGeneralCallBack(pmx,  DG_SYS, "ReplyClassConnection", MXONRECV, SYS_ReplyClassConnection, NULL);
    MXAddGeneralCallBack(pmx,  DG_SYS, "AckMessage"      , MXONRECV, SYS_AckMessage,      NULL);
    MXAddGeneralCallBack(pmx,  DG_SYS, "GetFiles"        , MXONRECV, SYS_GetFiles,        NULL);
    MXAddGeneralCallBack(pmx,  DG_SYS, "GetFile"         , MXONRECV, SYS_GetFile,         NULL);
    MXAddGeneralCallBack(pmx,  DG_SYS, "SendFile"        , MXONRECV, SYS_SendFile,        NULL);
    MXAddGeneralCallBack(pmx,  DG_SYS, "List"            , MXONRECV, SYS_List,            NULL);
    MXAddGeneralCallBack(pmx,  DG_SYS, "Dir"             , MXONRECV, SYS_Dir,             NULL);



    MXAddGeneralCallBack(pmx,  DG_SYS, "RemoveFiles"     , MXONRECV, SYS_RemoveFiles,     NULL);
    MXAddGeneralCallBack(pmx,  DG_SYS, "RenameFile"      , MXONRECV, SYS_RenameFile,      NULL);
    MXAddGeneralCallBack(pmx,  DG_SYS, "ChangeDir"       , MXONRECV, SYS_ChangeDir,       NULL);
    MXAddGeneralCallBack(pmx,  DG_SYS, "GetDir"          , MXONRECV, SYS_GetDir,          NULL);
    MXAddGeneralCallBack(pmx,  DG_SYS, "MakeDir"         , MXONRECV, SYS_MakeDir,         NULL);
    MXAddGeneralCallBack(pmx,  DG_SYS, "RemoveDir"       , MXONRECV, SYS_RemoveDir,       NULL);
    MXAddGeneralCallBack(pmx,  DG_SYS, "PutError"        , MXONRECV, SYS_PutError,        NULL);
    return 1;
}

int MXAddFTPCommands (MX* pmx)
{

    MXDialogClass * pclass;


    pclass = MXCreateDialogClass(pmx, FTP_SYS, FTP_SYS_CODE);
    MXCreateMessageClass(pmx, pclass, "Command",              1 , 3, "LONG",   1, "Identity",
                         "STRING", 1, "Value",
                         "STRING", 1, "Par");

    MXCreateMessageClass(pmx, pclass, "Command_Reply",        2,  5, "LONG",   1, "Identity",
                         "STRING", 1, "Value",
                         "STRING", 1, "Par",
                         "LONG",   1, "Code",
                         "STRING", 1, "Reply");

    /* active connection */

    MXCreateMessageClass(pmx, pclass, "dir",                  3, 5,  "LONG",   1, "Identity",
                         "STRING", 1, "Value",
                         "STRING", 1, "Par",
                         "STRING", 1, "NetId",
                         "LONG",   1, "Port" );

    MXCreateMessageClass(pmx, pclass, "dir_reply",            4, 7,  "LONG",   1, "Identity",
                         "STRING", 1, "Value",
                         "STRING", 1, "Par",
                         "LONG",   1, "Error",
                         "STRING", 1, "NetId",
                         "LONG",   1, "Port",
                         "BUFFER", 1, "Buffer");

    MXCreateMessageClass(pmx, pclass, "get",                  5, 6,  "LONG",   1, "Identity",
                         "STRING", 1, "FileName",
                         "STRING", 1, "DestName",
                         "CHAR",   1, "FileType",
                         "STRING", 1, "NetId",
                         "LONG",   1, "Port" );
    MXCreateMessageClass(pmx, pclass, "get_reply",            7, 6,  "LONG",   1, "Identity",
                         "STRING", 1, "Value",
                         "LONG",   1, "Error",
                         "STRING", 1, "NetId",
                         "LONG",   1, "Port",
                         "FILE",   1, "File");


    MXCreateMessageClass(pmx, pclass, "put",                  6, 4,  "LONG",   1, "Identity",
                         "FILE",   1, "File",
                         "STRING", 1, "NetId",
                         "LONG",   1, "Port" );

    MXCreateMessageClass(pmx, pclass, "put_reply",            8, 7,  "LONG",   1, "Identity",
                         "STRING", 1, "Value",
                         "LONG",   1, "Error",
                         "STRING", 1, "FileName",
                         "STRING", 1, "DestName",
                         "STRING", 1, "NetId",
                         "LONG",   1, "Port" );
    return 1;
}
int MXAddSMTPCommands (MX* pmx)
{

    MXDialogClass * pclass;


    pclass = MXCreateDialogClass(pmx, SMTP_SYS, SMTP_SYS_CODE);
    MXCreateMessageClass(pmx, pclass, "Command",              1 , 3, "LONG",   1, "Identity",
                         "STRING", 1, "Value",
                         "STRING", 1, "Par");

    MXCreateMessageClass(pmx, pclass, "Command_Reply",        2,  5, "LONG",   1, "Identity",
                         "STRING", 1, "Value",
                         "STRING", 1, "Par",
                         "LONG",   1, "Code",
                         "STRING", 1, "Reply");

    MXCreateMessageClass(pmx, pclass, "Send",                  3, 8, "LONG",   1, "Identity",
                         "STRING", 1, "From",
                         "STRING", 1, "To",
                         "STRING", 1, "Cc",
                         "STRING", 1, "Bcc",
                         "STRING", 1, "Object",
                         "STRING", 1, "Subject",
                         "STRING", 1, "Attachment");
    return 1;
}


int MXAddPOPCommands (MX* pmx)
{

    MXDialogClass * pclass;

    pclass = MXCreateDialogClass(pmx, POP_SYS, POP_SYS_CODE);
    MXCreateMessageClass(pmx, pclass, "Command",              1 , 3, "LONG",   1, "Identity",
                         "STRING", 1, "Value",
                         "STRING", 1, "Par");

    MXCreateMessageClass(pmx, pclass, "Command_Reply",        2,  5, "LONG",   1, "Identity",
                         "STRING", 1, "Value",
                         "STRING", 1, "Par",
                         "LONG",   1, "Code",
                         "STRING", 1, "Reply");

    MXCreateMessageClass(pmx, pclass, "List_Reply",           3,  9, "LONG",   1, "Identity",
                         "STRING", 1, "Value",
                         "STRING", 1, "Par",
                         "STRING", 1, "Reply",
                         "LONG",   1, "Code",
                         "LONG",   60, "Number",
                         "LONG",   60, "Size",
                         "LONG",   1, "TotalNumber",
                         "LONG",   1, "TotalSize");




    MXCreateMessageClass(pmx, pclass, "Stat_Reply",           4,  7, "LONG",   1, "Identity",
                         "STRING", 1, "Value",
                         "STRING", 1, "Par",
                         "STRING", 1, "Reply",
                         "LONG",   1, "Code",
                         "LONG",   1, "TotalNumber",
                         "LONG",   1, "TotalSize");

    MXCreateMessageClass(pmx, pclass, "Retr_Reply",           5,  13,"LONG",   1, "Identity",
                         "STRING", 1, "Value",
                         "STRING", 1, "Par",
                         "STRING", 1, "Reply",
                         "LONG",   1, "Code",
                         "LONG",   1, "Size",
                         "STRING", 1, "Date",
                         "STRING", 1, "From",
                         "STRING", 200, "To",
                         "STRING", 200, "Cc",
                         "STRING", 200, "Bcc",
                         "STRING", 1, "Subject",
                         "STRING", 1, "Body",
                         "STRING", 1, "Attachment");


    return 1;
}

int MXAddFIOPCommands (MX* pmx)
{
    MXDialogClass * pclass;

    pclass = MXCreateDialogClass(pmx, FIOP_SYS, FIOP_SYS_CODE);
    return 1;
}

int MXAddTCPCommands (MX* pmx)
{

    MXDialogClass * pclass;

    pclass = MXCreateDialogClass(pmx, TCP_SYS, TCP_SYS_CODE);

    MXCreateMessageClass(pmx, pclass, "Stream", 1, 1, "BUFFER", 1, "Buffer");

    return 1;
}

int MXAddMTCommands (MX* pmx)
{

    MXDialogClass * pclass;

    pclass = MXCreateDialogClass(pmx, MT_SYS, MT_SYS_CODE);

    MXCreateMessageClass(pmx, pclass, "Stream",               1, 1,  "BUFFER", 1, "Buffer");

    return 1;
}


int MXAddDNSCommands (MX* pmx)
{

    MXDialogClass * pclass;

    pclass = MXCreateDialogClass(pmx, DNS_SYS, DNS_SYS_CODE);

    MXCreateMessageClass(pmx, pclass, "Query",                 1, 6, "LONG",   1, "Identity",
                         "WORD",   1, "Operation",
                         "WORD",   1, "QClass",
                         "WORD",   1, "QType",
                         "STRING", 1, "Domain",
                         "BUFFER", 1, "RecordData");


    MXCreateMessageClass(pmx, pclass, "Reply",                 1, 13,"LONG",   1, "Identity",
                         "WORD",   1, "Operation",
                         "WORD",   1, "QClass",
                         "WORD",   1, "QType",
                         "STRING", 1, "Domain",
                         "WORD",   1, "Code",
                         "WORD",   1, "AnswerCount",
                         "WORD",   1, "AuthorityCount",
                         "WORD",   1, "AdditionalCount",
                         "WORD",   60,"Section",
                         "STRING", 60,"Name",
                         "WORD",   60,"Type",
                         "STRING", 60,"Value");

    return 1;
}


int MXAddDBCommands (MX* pmx)
{
    MXDialogClass * pclass;

    pclass = MXCreateDialogClass(pmx, DB_SYS, DB_SYS_CODE);


    MXCreateMessageClass(pmx, pclass, "ExecuteQuery",       1, 5,    "LONG",    1, "Identity",
                         "STRING",  1, "Name",
                         "STRING",  1, "SqlStatement",
                         "DWORD",   1, "Every",
                         "DWORD",   1, "MaxRows",
                         "STRING",  1, "IdentStatic");


    MXCreateMessageClass(pmx, pclass, "ExecuteUpdate",       2, 3,   "STRING",  1, "IdentStatic",
                         "STRING",  1, "SqlStatement",
                         "BYTE",    1, "Commit");

    MXCreateMessageClass(pmx, pclass, "ResultHeader",        3, 2,   "STRING",    1, "ClassName",
                         "STRING",  1, "QueryName");

    MXCreateMessageClass(pmx, pclass, "ResultSet",           4, 3,   "LONG",    1, "RowsNumber",
                         "BUFFER",  1, "Buffer",
                         "BYTE",    1, "LastContext");

    MXCreateMessageClass(pmx, pclass, "AckStatement",        5, 7,   "LONG",    1, "Identity",
                         "LONG",    1, "RowsProcessed",
                         "LONG",    1, "SqlCode",
                         "STRING",  1, "SqlErrMsg",
                         "STRING",  1, "SqlStatement",
                         "SHORT",   1, "Print",
                         "SHORT",   1, "Stop");


    MXCreateMessageClass(pmx, pclass, "DisconnectClient",    6, 1,   "BYTE",       1, "Commit");

    MXCreateMessageClass(pmx, pclass, "CommitOrRollback",    7, 1,   "BYTE",       1, "Commit");

    MXCreateMessageClass(pmx, pclass, "CancelStatement",     8, 0);

    MXCreateMessageClass(pmx, pclass, "AskShow",             9, 2,  "STRING",  1, "SqlStatement",
                         "BYTE",       1, "StorageType");

    MXCreateMessageClass(pmx, pclass, "ReceiveShowBuffer",   10, 2,  "BUFFER",  1, "Buffer",
                         "BYTE",       1, "LastContext");
    return 1;
}


int MXAddHTTPCommands (MX* pmx)
{

    MXDialogClass * pclass;
    MXMessageClass* pmessageClass;


    pclass = MXCreateDialogClass(pmx, HTTP_SYS, HTTP_SYS_CODE);

    pmessageClass =
        MXCreateMessageClass(pmx, pclass, "Request",              1, 29,
                            "STRING",   1,     "Request-Line",
                            "STRING",   1,     "Cache-Control",
                            "STRING",   1,     "Connection",
                            "STRING",   1,     "Date",
                            "STRING",   1,     "Pragma",
                            "STRING",   1,     "Trailer",
                            "STRING",   1,     "Transfer-Encoding",
                            "STRING",   1,     "Upgrade",
                            "STRING",   1,     "Via",
                            "STRING",   1,     "Warning",
                            "STRING",   1,     "Accept",
                            "STRING",   1,     "Accept-Charset",
                            "STRING",   1,     "Accept-Encoding",
                            "STRING",   1,     "Accept-Language",
                            "STRING",   1,     "Authorization",
                            "STRING",   1,     "Expect",
                            "STRING",   1,     "From",
                            "STRING",   1,     "Host",
                            "STRING",   1,     "If-Match",
                            "STRING",   1,     "If-Modified-Since",
                            "STRING",   1,     "If-None-Match",
                            "STRING",   1,     "If-Range",
                            "STRING",   1,     "If-Unmodified-Since",
                            "STRING",   1,     "Max-Forwards",
                            "STRING",   1,     "Proxy-Authorzation",
                            "STRING",   1,     "Range",
                            "STRING",   1,     "Referer",
                            "STRING",   1,     "TE",
                            "STRING",   1,     "User-Agent");

    MXAddObjectToMessageClass  (pmessageClass, "STRING",   1,     "Allow");
    MXAddObjectToMessageClass  (pmessageClass, "STRING",   1,     "Content-Encoding");
    MXAddObjectToMessageClass  (pmessageClass, "STRING",   1,     "Content-Language");
    MXAddObjectToMessageClass  (pmessageClass, "STRING",   1,     "Content-Length");
    MXAddObjectToMessageClass  (pmessageClass, "STRING",   1,     "Content-Location");
    MXAddObjectToMessageClass  (pmessageClass, "STRING",   1,     "Content-MD5");
    MXAddObjectToMessageClass  (pmessageClass, "STRING",   1,     "Content-Range");
    MXAddObjectToMessageClass  (pmessageClass, "STRING",   1,     "Content-Type");
    MXAddObjectToMessageClass  (pmessageClass, "STRING",   1,     "Expires");
    MXAddObjectToMessageClass  (pmessageClass, "STRING",   1,     "Last-Modified");
    MXAddObjectToMessageClass  (pmessageClass, "STRING",   1,     "Proxy-Connection");
    MXAddObjectToMessageClass  (pmessageClass, "STRING",   1,     "Cookie");
    MXAddObjectToMessageClass  (pmessageClass, "BUFFER",   1,     "Content");


    MXCreateMessageClass(pmx, pclass, "Response",              2, 31,
                        "STRING",   1,     "Status-Line",
                        "STRING",   1,     "Cache-Control",
                        "STRING",   1,     "Connection",
                        "STRING",   1,     "Date",
                        "STRING",   1,     "Pragma",
                        "STRING",   1,     "Trailer",
                        "STRING",   1,     "Transfer-Encoding",
                        "STRING",   1,     "Upgrade",
                        "STRING",   1,     "Via",
                        "STRING",   1,     "Warning",
                        "STRING",   1,     "Accept-Ranges",
                        "STRING",   1,     "Age",
                        "STRING",   1,     "Etag",
                        "STRING",   1,     "Location",
                        "STRING",   1,     "Proxy-Connection",
                        "STRING",   1,     "Retry-After",
                        "STRING",   1,     "Server",
                        "STRING",   1,     "Vary",
                        "STRING",   1,     "WWW-Authenticate",
                        "STRING",   1,     "Allow",
                        "STRING",   1,     "Content-Encoding",
                        "STRING",   1,     "Content-Language",
                        "STRING",   1,     "Content-Length",
                        "STRING",   1,     "Content-Location",
                        "STRING",   1,     "Content-MD5",
                        "STRING",   1,     "Content-Range",
                        "STRING",   1,     "Content-Type",
                        "STRING",   1,     "Expires",
                        "STRING",   1,     "Last-Modified",
                        "STRING",   1,     "P3P",
                        "BUFFER",   1,     "Content");

    return 1;
}



int MXLoadResourceFile (MX *pmx, char *Filename)
{
    int MesId = 0;
    GlobalOne*   Glob;
    Dialog*      Dial;
    MessageClass*     Mess;
    Argument*    Argu;
    ConnectionClass*     Conn;
    XlatStruct*  XL;
    MXDialogClass*     pclass;
    MXMessageClass* pmessclass;
    List*        AllDialog;
    List*        AllMessage;
    List*        AllArgument;
    List*        AllXlatStruct;
    List*        AllJournalClasses;
    List*        AllConnect;
    List*        AllApplicationClasses;
    List*        AllDataBaseClasses;
    List*        AllQueues;
    List*        AllSites;
    List*        AllMachines;
    List*        AllLads;
    List*        AllConnections;
    List*        AllApplications;
    List*        AllDataBases;
    List*        AllJournals;

    BYTE         FromTo = MXFROM;
	BYTE		 type ;

    if (!OSInitResourceFile(Filename))
    {
        OSError("TEOS0001LEX", OSStrErr);
        return -1;
    }
    Glob = (GlobalOne *)zzparse(pmx->TP->XF);
    if (!Glob)
    {
        OSError("TEOS0001LEX", OSStrErr);
        OSEndResourceFile();
        return -1;
    }
    AllDialog = Glob->AllDialog;
    while(AllDialog)
    {
        Dial = (Dialog *)AllDialog->car;

        /*Traitement d'initialisation... */
        MesId = 0;
        pclass = MXCreateDialogClass(pmx, Dial->DialName, (WORD)Dial->DialId);
        AllMessage = Dial->AllMessageClasses;
        while(AllMessage)
        {
            Mess = (MessageClass *)AllMessage->car;

            MesId++;
            pmessclass = MXCreateMessageClass(pmx, pclass, Mess->MessName, (WORD)MesId, 0);
            AllArgument = Mess->AllArgument;
            while(AllArgument)
            {
                Argu = (Argument *)AllArgument->car;
                MXAddObjectToMessageClass (pmessclass, Argu->ArguType, Argu->ArguSize, Argu->ArguName);
                AllArgument = AllArgument->cdr;
            }
            AllMessage = AllMessage->cdr;
        }
        AllDialog = AllDialog->cdr;
    }
    AllXlatStruct = Glob->AllXlatStruct;
    while(AllXlatStruct)
    {
        XL = (XlatStruct *)AllXlatStruct->car;
        XFLoadSystemTable(pmx->TP->XF, (XFTable *)XL);
        AllXlatStruct = AllXlatStruct->cdr;
    }
    AllSites = Glob->AllSites;
    while (AllSites)
    {
        Site*        site;
        MXSite*      pmxsite;

        site = (Site *)AllSites->car;
        pmxsite = MXCreateSite (pmx, site->Name, (WORD)site->Id);
        AllSites = AllSites->cdr;
    }
    AllMachines = Glob->AllMachines;
    while (AllMachines)
    {
        MXMachine*   pmxmachine;
        Machine* machine = (Machine *)AllMachines->car;
        BYTE CodeSystem = MXGetCodeFromSystem (machine->System);
        char EntityName[250];

        pmxmachine = MXCreateMachine (pmx, machine->Name, (WORD)machine->Id, CodeSystem, machine->IPAddress, machine->IPName);
        MXAddMachineToSite (pmx,(MXSite*)MXGetEntityFromString (pmx, machine->Localisation, MXSITE, EntityName),pmxmachine);
        AllMachines = AllMachines->cdr;
    }

    AllLads = Glob->AllLogicalAddress;
    while (AllLads)
    {
        char EntityName[250];
        LogicalAddress* lad = (LogicalAddress *)AllLads->car;
        MXLadClass* pmxladclass = MXCreateLadClass (pmx, lad->Name, (WORD)lad->Id);
        MXLad* pmxlad = MXCreateLad (pmx, pmxladclass, lad->Name, (WORD)lad->Id,(MXMachine*)MXGetEntityFromString (pmx, lad->Localisation, MXMACHINE, EntityName));
        AllLads = AllLads->cdr;
    }

    AllJournalClasses = Glob->AllJournalClasses;
    while(AllJournalClasses)
    {
        JournalClass*        storage = (JournalClass *)AllJournalClasses->car;
        MXJournalClass* pmxjournalclass;

        MesId = 0;
        pmxjournalclass = MXCreateJournalClass (pmx, storage->Name,  (WORD)storage->Id, (BYTE)storage->Format);
        AllMessage = storage->AllMessage;
        pclass = MXGetDialogClassFromName (pmx, FIOP_SYS);
        while(AllMessage)
        {
            Mess = (MessageClass *)AllMessage->car;
            MesId++;
            pmessclass = MXCreateMessageClass(pmx, pclass, Mess->MessName, (WORD)MesId, 0);
            AllArgument = Mess->AllArgument;
            while(AllArgument)
            {
                Argu = (Argument *)AllArgument->car;
                MXAddObjectToMessageClass (pmessclass, Argu->ArguType, Argu->ArguSize, Argu->ArguName);
                AllArgument = AllArgument->cdr;
            }
            AllMessage = AllMessage->cdr;
            ListNewr (&pmxjournalclass->MessageClasses, pmessclass);
        }
        pmxjournalclass->Length = storage->Length;
        AllJournalClasses = AllJournalClasses->cdr;
    }

    AllJournals = Glob->AllJournals;
    while(AllJournals)
    {
        char EntityName[250];
        Journal*    journal;
        MXJournal* pmxjournal;
        MXJournalClass* pmxjournalclass;

        journal = (Journal *)AllJournals->car;
        pmxjournalclass = MXGetJournalClassFromName (pmx, journal->Class);
        if (pmxjournalclass)
        {
            MXLad* pmxlad = (MXLad*)MXGetEntityFromString (pmx, journal->Localisation, MXLAD, EntityName);
            MXMachine* pmxmachine = (MXMachine*)MXGetEntityFromString (pmx, journal->Localisation, MXMACHINE, EntityName);

            if (pmxmachine)
                pmxjournal = MXCreateJournal (pmx, pmxjournalclass, journal->Name,  (WORD)journal->Id, pmxmachine, MXONMACHINE);
            else
                pmxjournal = MXCreateJournal (pmx, pmxjournalclass, journal->Name,  (WORD)journal->Id, pmxlad, MXONLAD);
        }
        AllJournals = AllJournals->cdr;
    }


    AllDataBaseClasses = Glob->AllDataBaseClasses;
    while(AllDataBaseClasses)
    {
        DataBaseClass*    databaseclass;
        MXDataBaseClass* pmxdatabaseclass;

        databaseclass = (DataBaseClass *)AllDataBaseClasses->car;
        pmxdatabaseclass = MXCreateDataBaseClass (pmx, databaseclass->Name,  (WORD)databaseclass->Id, 0);
        AllDataBaseClasses = AllDataBaseClasses->cdr;
    }
    AllDataBases = Glob->AllDataBases;
    while(AllDataBases)
    {
        char EntityName[250];
        DataBase*    database;
        MXDataBase* pmxdatabase;
        MXDataBaseClass*    pmxdatabaseclass;

        database = (DataBase *)AllDataBases->car;
        pmxdatabaseclass = MXGetDataBaseClassFromName (pmx, database->Class);
        if (pmxdatabaseclass)
        {
            MXLad* pmxlad = (MXLad*)MXGetEntityFromString (pmx, database->Localisation, MXLAD, EntityName);
            MXMachine* pmxmachine = (MXMachine*)MXGetEntityFromString (pmx, database->Localisation, MXMACHINE, EntityName);

            if (pmxmachine)
                pmxdatabase = MXCreateDataBase (pmx, pmxdatabaseclass, database->Name,  (WORD)database->Id, pmxmachine, MXONMACHINE);
            else
                pmxdatabase = MXCreateDataBase (pmx, pmxdatabaseclass, database->Name,  (WORD)database->Id, pmxlad, MXONLAD);
            strcpy (pmxdatabase->BaseName, database->BaseName);
        }
        AllDataBases = AllDataBases->cdr;
    }


    AllQueues = Glob->AllQueues;
    while(AllQueues)
    {
        Queue*       queue;
        MXQueueClass* pmxqueueclass;

        queue = (Queue *)AllQueues->car;
        pmxqueueclass = MXCreateQueueClass (pmx, queue->Name,  (WORD)queue->Id, 0);
        AllQueues = AllQueues->cdr;
    }


    AllApplicationClasses = Glob->AllApplicationClasses;
    while (AllApplicationClasses)
    {
        ApplicationClass* application = (ApplicationClass *)AllApplicationClasses->car;
		MXApplicationClass* pmxapplicationclass;

        if (OSStrcmp (application->Type, "SERVER") == 0) type = MXSERVER; else type = MXCLIENT;
		
		pmxapplicationclass = MXCreateApplicationClass (pmx, application->Name,  (WORD)application->Id, type);
        if (application->Repertory[0] != 0)
        {
            memcpy (pmxapplicationclass->Repertory, application->Repertory + 1, strlen(application->Repertory) - 2);
            pmxapplicationclass->Repertory[strlen(application->Repertory) - 2] = 0;
        }
        AllApplicationClasses = AllApplicationClasses->cdr;
    }
    AllConnect = Glob->AllConnectionClasses;
    while(AllConnect)
    {
        Conn = (ConnectionClass *)AllConnect->car;
        FromTo = (strcmp (Conn->TransferFormat, "FROM") == 0) ? MXFROM : MXTO;
        MXCreateConnectionClass (pmx, Conn->Name, (WORD)Conn->Id, MXGetProtocolFromName(Conn->Protocol), FromTo, Conn->TableName, Conn->FromAppliName, Conn->ToAppliName);
        AllConnect = AllConnect->cdr;
    }
    AllApplications = Glob->AllApplications;
    while (AllApplications)
    {
        char EntityName[250];
        List* Folders;
        MXApplication* pmxapplication;
        Application* program = (Application *)AllApplications->car;

        MXApplicationClass* pmxapplicationclass = MXGetApplicationClassFromName (pmx, program->Class);
        MXLad* pmxlad = (MXLad*)MXGetEntityFromString (pmx, program->Localisation, MXLAD, EntityName);
        MXMachine* pmxmachine = (MXMachine*)MXGetEntityFromString (pmx, program->Localisation, MXMACHINE, EntityName);

        if (!pmxapplicationclass) continue;
        else
            if (pmxmachine)
                pmxapplication= MXCreateApplication (pmx, pmxapplicationclass,  program->Name, (WORD)program->Id, pmxmachine, MXONMACHINE);
            else
                if (pmxlad)
                    pmxapplication= MXCreateApplication (pmx, pmxapplicationclass,  program->Name, (WORD)program->Id, pmxlad,     MXONLAD);
                else continue;
        if (program->Repertory[0] != 0)
            memcpy (pmxapplication->Repertory,       program->Repertory + 1,        strlen(program->Repertory) - 2);
        if (program->ExecRepertory[0] != 0)
            memcpy (pmxapplication->ExecRepertory,   program->ExecRepertory + 1,    strlen(program->ExecRepertory) - 2);
        if (program->ExecCommand[0] != 0)
            memcpy (pmxapplication->ExecCommand,     program->ExecCommand + 1,      strlen(program->ExecCommand) - 2);
        if (program->ExecParameters[0] != 0)
            memcpy (pmxapplication->ExecParameters,  program->ExecParameters + 1,   strlen(program->ExecParameters) - 2);
        if (program->ReceiveRepertory[0] != 0)
            memcpy (pmxapplication->ReceiveRepertory,program->ReceiveRepertory + 1, strlen(program->ReceiveRepertory) - 2);
        if (program->SendRepertory[0] != 0)
            memcpy (pmxapplication->SendRepertory,   program->SendRepertory + 1,    strlen(program->SendRepertory) - 2);
        Folders = program->Folders;
        while (Folders)
        {
            FolderArgument* pFolder = (FolderArgument*)Folders->car;
            MXFolder* pmxfolder = MXGetFolderFromName (pmx, pmxapplication, pFolder->Name);
            List* list = pFolder->Files;
            if (!pmxfolder)
                pmxfolder = MXCreateFolder (pmx, pmxapplication, pFolder->Name, pFolder->Extensions);
            while (list)
            {
                FileArgument* pfile = (FileArgument*)list->car;
                pfile->Name[strlen (pfile->Name) - 1] = 0;
                MXCreateFile (pmx, pmxfolder, pfile->Name + 1);
                list = list->cdr;
            }
            Folders = Folders->cdr;
        }
        AllApplications = AllApplications->cdr;
    }
    AllConnections = Glob->AllConnections;
    while(AllConnections)
    {   
           Connection* connection = (Connection *)AllConnections->car;
           MXConnectionClass* pmxconnectionclass = MXGetConnectionClassFromName (pmx, connection->Class);
           if (pmxconnectionclass)
           {
               MXApplication* fromappli = MXGetApplicationFromName (pmx, connection->FromAppliName);
               MXConnection* pmxconnection = MXCreateConnection (pmx, connection->Name, (WORD)connection->Id, pmxconnectionclass, connection->FromAppliName, connection->ToAppliName);
               if (!pmxconnection) continue;
               strcpy (pmxconnection->PassWord, connection->PassWord);
               strcpy (pmxconnection->UserName, connection->UserName);
               strcpy (pmxconnection->ReadMode, connection->ReadMode);
               strcpy (pmxconnection->FileName, connection->FileName + 1);
               pmxconnection->FileName[strlen(pmxconnection->FileName) - 1] = 0;
               pmxconnection->Port = connection->Port;
           }
        AllConnections = AllConnections->cdr;
    }
    FreeGlobalOneYaccStruct(Glob);
    OSEndResourceFile();
    return 1;
}

