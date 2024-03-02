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

#ifndef __TP_H__
#define __TP_H__

#include "oscom.h"
#include "osio.h"
#include "oslist.h"
#include "osreslex.h"
#include "osdb.h"

#include "xf.h"
#include <time.h>




#define TPMaxClients SOCKMAX
#define TPNumberClientInHistory     10



#define TPOutputPending(ptp)     (OSANYSET((ptp)->OutputPending))
#define TPInputPending(ptp)      (OSANYSET((ptp)->InputPending))
#define TPConnectionPending(ptp) (OSANYSET((ptp)->ConnectionPending))
#define TPClientsWithInput(ptp)  (OSANYSET((ptp)->ClientsWithInput))
#define TPClientsWithOutput(ptp) (OSANYSET((ptp)->ClientsWithOutput))
#define TPClientsWithFileInput(ptp)  (OSANYSET((ptp)->ClientsWithFileInput))
#define TPClientsWithFileOutput(ptp) (OSANYSET((ptp)->ClientsWithFileOutput))
#define TPClientsWithBaseInput(ptp)  (OSANYSET((ptp)->ClientsWithBaseInput))
#define TPClientsWithBaseOutput(ptp) (OSANYSET((ptp)->ClientsWithBaseOutput))
#define TPGetClient(ptp, s)      ((ptp)->Clients[(ptp)->ConnectionTranslation[s]])

#define TPNextClientWithOutput(ptp)  TPGetClient(ptp, TPGetNextDescriptor(ptp, (ptp)->ClientsWithOutput))
#define TPNextClientWithInput(ptp)   TPGetClient(ptp, TPGetNextDescriptor(ptp, (ptp)->ClientsWithInput))
#define TPNextInputPending(ptp)      TPGetClient(ptp, TPGetNextDescriptor(ptp, (ptp)->InputPending))
#define TPNextOutputPending(ptp)     TPGetClient(ptp, TPGetNextDescriptor(ptp, (ptp)->OutputPending))
#define TPNoMoreTCPClient(ptp)(!OSANYSET((ptp)->AllClients)) 

#define TPNoMoreClient(ptp)(!OSANYSET((ptp)->AllClients) && \
                            !OSANYSET((ptp)->AllFileClients) && \
                            !OSANYSET((ptp)->AllBaseClients))

#define TPClientIsBlocked(pclient) (OSGETBIT(pclient->TP->ClientsWriteBlocked, pclient->Pipe->fd) ? 1 : 0)
#define TPClientWithOutput(pclient) (OSGETBIT(pclient->TP->ClientsWithOutput, pclient->Pipe->fd) ? 1 : 0)
#define TPClientWithFileOutput(pclient) (OSGETBIT(pclient->TP->ClientsWithFileOutput, pclient->Index + 1) ? 1 : 0)
#define TPClientWithBaseOutput(pclient) (OSGETBIT(pclient->TP->ClientsWithBaseOutput, pclient->Index + 1) ? 1 : 0)






#define TPGetUserField(pclient) (pclient)->UserField
#define TPSetUserField(pclient,pfield) ((pclient)->UserField = pfield)
#define TPGetApplicationField(pclient) (pclient)->ApplicationField
#define TPSetPointerField(pclient,pfield) ((pclient)->PointerField = pfield)
#define TPIsServer(ptp) ((ptp)->Mode == TPSERVER)



#define TP_OK             0
#define TP_NOK           -1

#define TPSERVER          1
#define TPCLIENT          0


#define TPBUFOUTPUTSIZE    BUFSIZE         /* max size of transmission  TP - 2    */



typedef struct _TPRequest
{
    WORD Size;
}TPRequest;


typedef struct _TPPort
{
    int   fd;
    int   Port;
    int   Protocol;
    List* Clients;
}TPPort;


typedef struct _TPbuf
{
    char* Buf;
    int   Count;
}TPBuf;






typedef struct _TP
{
    long WellKnownConnections[MaskCount];          /*!< All Listening sockets          */
    long LocalListening[MaskCount];                /*!< local listening sockets        */
    long DistantListening[MaskCount];              /*!< distant listening sockets      */
    long ConnectionPending[MaskCount];             /*!< Any connection pending ?       */
    long AllSockets[MaskCount];                    /*!< all sockets listen & comm      */
    long AllClients[MaskCount];                    /*!< all comm                       */
    long AllFileClients[MaskCount];                /*!< all files                      */
    long AllBaseClients[MaskCount];                /*!< all bases                      */
    long LastSelectMask[MaskCount];                /*!< mask for listen & read         */
    long ClientsWithInput[MaskCount];              /*!< still input not processed      */
    long ClientsWithOutput[MaskCount];             /*!< still output not processed     */
    long ClientsWithFileInput[MaskCount];          /*!< still file input not processed */
    long ClientsWithFileOutput[MaskCount];         /*!< still file output not processed*/
    long ClientsWithBaseInput[MaskCount];          /*!< still base input not processed */
    long ClientsWithBaseOutput[MaskCount];         /*!< still base output not processed*/
    long ClientsWriteBlocked[MaskCount];           /*!< all comm  blocked in output    */
    long OutputPending[MaskCount];                 /*!< all comm not blocked in output */
    long InputPending[MaskCount];                  /*!< all comm with input not read   */
    long SavedAllClients[MaskCount];
    long SavedAllSockets[MaskCount];
    long SavedClientsWithInput[MaskCount];
    long SavedClientsWithOutput[MaskCount];
    long SavedClientsWithFileInput[MaskCount];
    long SavedClientsWithFileOutput[MaskCount];
    long SavedClientsWithBaseInput[MaskCount];
    long SavedClientsWithBaseOutput[MaskCount];
    int  CurrentMaxClients;
    int  NextFreeClientId;
    BOOL GrabInProgress;
    BOOL AnyClientsWriteBlocked;                   /*!< any comm blocked in output ?   */
    struct _TPClient* Clients[TPMaxClients];       /*!< all clients struct             */
    int  ConnectionTranslation[TPMaxClients];
    char Name[32];
    char AppliName[32];
    BYTE Mode;                                     /*!< server or client               */
    List*  History;
    int    ClientHistoryNumber;                    /*!< number of client in hist       */
    List* ListeningPorts;
    List*  ApplicationProcedures;
    List*  ConnectionProcedures;
    XF*   XF;                                       /*!< syntax                         */
    OS*   OS;
}TP;



typedef struct _TPClient
{
    TP*              TP;
    int              Index;
    int              Type;                         /*!< local or distant or file      */
    int              ClientGone;
    int              NoClientException;
    int              ErrorIndex;                   /*!< zero means no error            */
    char             WithUserId[16];               /*!< the peer adress                */
    char             WithName[50];
    PIPE*            Pipe;
    char*            RequestBuffer;                /*!< the last request received      */
    WORD             RequestSize;
    char*            TransBuffer;
    WORD             TransSize;
    void*            ApplicationField;             /*!< connect to a higher level      */
    List*            EndingProcedures;
    int              (*ReadFunction) (struct _TP* ptp,  struct _TPClient* pclient);
    int              (*WriteFunction)(struct _TP* ptp,  struct _TPClient* pclient, char* buf, int Count);
    LONG             (*SendFunction)(struct _TP* ptp,  struct _TPClient* pclient, char* buf, int bufsize);
    LONG             (*ReceiveFunction)(struct _TP* ptp,  struct _TPClient* pclient, char* buf, int Count);
    DWORD            ReceiptSize;
    DWORD            TransferSize;
    time_t           CommunicationTime;
    BOOL             Blocked;                      /*!< client is in blocking mode     */
    BOOL             Connect;                      /*!< should be TCP                  */
    int              Protocol;
    TPPort*          OnPort;                       /*!< on which port it was connected */
    List*            UserField;                    /*!< user field for programming     */
    void*            PointerField;                 /*!< user field for programming     */
}TPClient;



typedef struct _TPApplicationProcedure
{
    void* ApplicationField;
    int(*ExecuteProcedure) (TP* ptp, void* papp);
}TPApplicationProcedure;


typedef struct _TPConnectionProcedure
{
    void* ApplicationField;
    int(*ExecuteProcedure) (TPClient* pclient, void* papp);
}TPConnectionProcedure;


typedef struct _TPEndingProcedure
{
    void* ApplicationField;
    int(*EndingProcedure) (TPClient* pcom, void* papp); /* close call this */
}TPEndingProcedure;





#ifdef __cplusplus
extern "C" {
#endif


    extern int       TPInit (TP* tpp, char* name, char* appliname);
    extern int       TPSendIfAnyOutput (TP* ptp, void* par) ;
    extern int       TPRecvIfAnyInput  (TP* ptp, void* par) ;
    extern BOOL      TPExecuteApplicationProcedures (TP* ptp);



    extern int       TPListenToLocal (TP* tpp);
    extern int       TPListenToTCP (TP* tpp, int Port, int protocol);

    extern TPPort*   TPFindPortFromFd (TP* ptp, int listenid);

    extern TPClient* TPEstablishConnectionWith (TP* tpp, char* machinname, int port, int protocol, char* user, char* appli, BOOL Block);
    extern TPClient* TPOpenFile (TP* ptp, char* filename, char* mode, int protocol, char loc, int Size);
    extern TPClient* TPOpenBase (TP* ptp, char* databasename, char* username, char* password, int protocol);

    extern TPClient* TPCreateClientWithFd (TP* ptp, int fd);
    extern TPClient* TPCreateClientWithFileId (TP* ptp, FILEID* fileid);
    extern TPClient* TPEstablishConnectionOn (TP* ptp, int listenid);
    extern TPClient* TPTakeConnection (TP* ptp, char* name, char* subtaskname, int from);
    extern TPClient* TPNextAvailableClient (TP* ptp);
    extern TPClient* TPCreateInetClient (TP*ptp, int type);
    extern int       TPEstablishAllNewConnections (TP* ptp);
    extern int       TPGiveConnection (TP* ptp, TPClient* pclient, char* name);
    extern void      TPCloseDownConnection (TP* ptp, TPClient* pclient);
    extern void      TPCloseDownClient (TP* ptp, TPClient* pclient);
    extern void      TPKillAllClients (TP* ptp);
    extern void      TPCloseDamagedConnections (TP* ptp);
    extern int       TPCheckConnection (TP* ptp, TPClient* pclient);
    extern void      TPOnlyListenToOneClient (TP* ptp, TPClient* pclient);
    extern void      TPListenToAllClients (TP* ptp);
    extern void      TPIgnoreClient (TP* ptp, TPClient* pclient);
    extern void      TPAttendClient (TP* ptp, TPClient* pclient);
    extern int       TPFlushClient (TP* ptp, TPClient* pclient);
    extern int       TPWaitForSomething (TP* ptp, DWORD milliseconds);
    extern int       TPWaitForConnections (TP* ptp);
    extern void      TPCloseListeningConnections (TP* ptp);

    extern int       TPGetNextDescriptor (TP* ptp, long* mask);
    extern int       TPNextConnectionPending (TP* ptp);

    extern void      TPSetClientProtocol (TP* ptp, TPClient* pclient, int protocol);

    extern int       TPReadRequestFromClient (TP* ptp, TPClient* pclient);
    extern int       TPWriteRequestToClient (TP* ptp, TPClient* pclient, char* buf, int size);
    extern int       TPReadIcmpFromClient (TP* ptp, TPClient* pclient);
    extern int       TPWriteIcmpToClient (TP* ptp, TPClient* pclient, char* buf, int size);
    extern int       TPReadIpFromClient (TP* ptp, TPClient* pclient);
    extern int       TPWriteIpToClient (TP* ptp, TPClient* pclient, char* buf, int size);
    extern int       TPReadUdpFromClient (TP* ptp, TPClient* pclient);
    extern int       TPWriteUdpToClient (TP* ptp, TPClient* pclient, char* buf, int size);
    extern int       TPReadTcpFromClient (TP* ptp, TPClient* pclient);
    extern int       TPWriteTcpToClient (TP* ptp, TPClient* pclient, char* buf, int size);
    extern int       TPReadDNSFromClient (TP* ptp, TPClient* pclient);
    extern int       TPWriteDNSToClient (TP* ptp, TPClient* pclient, char* buf, int size);
    extern int       TPReadRequestFromFile (TP* ptp, TPClient* pclient);
    extern int       TPWriteRequestToFile (TP* ptp, TPClient* pclient, char* buf, int size);
    extern int       TPReadStreamFromFile (TP* ptp, TPClient* pclient);
    extern int       TPWriteStreamToFile (TP* ptp, TPClient* pclient, char* buf, int size);
    extern int       TPWriteStreamToBase (TP* ptp, TPClient* pclient, char* buf, int size);
    extern int       TPReadStreamFromFile(TP* ptp, TPClient* pclient);
    extern int       TPReadStreamFromBase(TP* ptp, TPClient* pclient);


    extern int       TPRead (TP* ptp, TPClient* pclient);
    extern int       TPWrite (TP* ptp, TPClient* pclient, char* buffer , int size);
    extern LONG      TPDispatchOutput (TP* ptp, TPClient* pclient, int TimeToYield);
    extern LONG      TPFlushClientOutput (TP* ptp, TPClient* pclient);
    extern LONG      TPDispatchInput (TP* ptp, TPClient* pclient, int TimeToYield);
    extern LONG      TPDispatchEvents (TP* ptp, DWORD milliseconds);

    extern LONG      TPDefaultSend (TP* ptp, TPClient* pclient, char* buf, int bufsize);
    extern LONG      TPDefaultReceive (TP* ptp, TPClient* pclient, char* buf, int count);

    extern LONG      TPReceiveBuf (TP* ptp, TPClient* pclient, char* buf, int count);
    extern LONG      TPSendBuf (TP* ptp, TPClient* pclient, char* buf, int count);

    extern TPBuf*    TPAllocBuf (char* buf, int count);
    extern int       TPJoinClients (TP* ptp, TPClient* pclient, TPClient* pclient1);




    extern int       TPSetBlockingMode (TPClient* pclient, BOOL blocking);
    extern void      TPConnectionOK (TP* ptp, TPClient* pclient);
    extern void      TPSetReadFunction (TP* ptp, TPClient* pclient,int (*proc) (TP* ptp, TPClient* pclient));
    extern void      TPSetWriteFunction (TP* ptp, TPClient* pclient, int (*proc) (TP* ptp, TPClient* pclient,char* buf, int Count));

    extern void      TPSetSendFunction (TP* ptp, TPClient* pclient,LONG (*proc) (TP* ptp, TPClient* pclient, char* buf, int size));
    extern void      TPSetReceiveFunction (TP* ptp, TPClient* pclient, LONG (*proc) (TP* ptp, TPClient* pclient,char* buf, int Count));



    extern WORD      TPGetRequestLength (char* buffer);
    extern WORD      TPGetIpLength (char* buffer);
    extern WORD      TPGetIpHeaderLength (char* buffer);

    extern void      TPSetRequestLength (char* buffer, WORD size);
    extern void      TPSetIpLength (char* buffer, WORD size);
    extern void      TPSetIpHeaderLength (char* buffer, WORD size);

    extern WORD      TPGetIcmpLength (char* buffer);
    extern void      TPSetIcmpLength (char* buffer, WORD size);

    extern WORD      TPGetDNSLength (char* buffer);
    extern void      TPSetDNSLength (char* buffer, WORD size);


    extern BYTE      TPGetIpProtocol (char* buffer);
    extern void      TPSetIpProtocol (char* buffer, BYTE protocol);


    extern DWORD     TPGetIpSourceAddress (char* buffer);
    extern DWORD     TPGetIpDestinationAddress (char* buffer);

    extern void      TPSetIpSourceAddress (char* buffer, DWORD ipadress);
    extern void      TPSetIpDestinationAddress (char* buffer, DWORD ipadress);


    extern WORD      TPGetTcpSourcePort (char* buffer);
    extern void      TPSetTcpSourcePort (char* buffer, WORD SourcePort);

    extern WORD      TPGetTcpDestinationPort (char* buffer);
    extern void      TPSetTcpDestinationPort (char* buffer, WORD DestPort);

    extern int       TPAddApplicationProcedure (TP* ptp, int (*funct)(TP*, void*), void* applicationfield);
    extern int       TPRemoveApplicationProcedure (TP* ptp, int (*funct)(TP*, void*), void* applicationfield);

    extern TPApplicationProcedure* TPFindApplicationProcedure (List* ExecuteList, int (*funct)(TP*, void*));
    extern int       TPAddConnectionProcedure (TP* ptp, int (*funct)(TPClient*, void*), void* applicationfield);
    extern int       TPRemoveConnectionProcedure (TP* ptp, int (*funct)(TPClient*, void*), void* applicationfield);
    extern TPConnectionProcedure* TPFindConnectionProcedure (List* ExecuteList, int (*funct)(TPClient*, void*));
    extern int       TPRemoveEndingProcedure (TPClient* pcmlient, int (*funct)(TPClient* ,void*), void* appfield);
    extern TPEndingProcedure* TPFindEndingProcedure (List* ExecuteList, int (*funct)(TPClient*, void*), void* appfield);
    extern int       TPAddEndingProcedure (TPClient* pclient, int (*funct)(TPClient* , void*), void* appfield);
    extern int       TPSetEndingProcedure(TPClient* pclient, int (*funct) (TPClient*, void*));
    extern int       TPSetApplicationField(TPClient* pclient, void* pfield);
    extern LONG      TPReceiveBuf (TP* ptp, TPClient* pclient, char* buf, int count);
    extern LONG      TPSendBuf (TP* ptp, TPClient* pclient, char* buf, int count);



    extern int       TPLoadResourceFile(TP* ptp, char* Filename);

    extern void      TPEnd (TP* ptp);


#ifdef __cplusplus
}
#endif



#endif /* __TP_H__*/

