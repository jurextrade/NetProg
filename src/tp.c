/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS

#include "tp.h"

int TPInit (TP* ptp, char* name, char* appliname)
{
    int i;
    XF* pxf;

    OS* pos = (OS*)malloc (sizeof (OS));
    if (!pos)
    {
        OSError ("TEDAG001", NULL);
        return -1;
    }
    if (OSInit (pos) < 0)
    {
        free (pos);
        OSError ("TEDAG001", NULL);
        return -1;
    }
    ptp->OS = pos;


    /* SYNTAX */
    pxf = (XF*)malloc (sizeof (XF));
    if (!pxf)
    {
        OSError ("TEDAG001", NULL);
        return -1;
    }
    if (XFInit (pxf, XFLittleEndian, XFAscii) < 0)
    {
        OSError ("TEDAG002", NULL);
    }
    ptp->XF = pxf;

    /* BASECOM */
    OSCLEARBITS(ptp->AllSockets);
    OSCLEARBITS(ptp->AllClients);
    OSCLEARBITS(ptp->AllFileClients);
    OSCLEARBITS(ptp->AllBaseClients);
    OSCLEARBITS(ptp->LastSelectMask);
    OSCLEARBITS(ptp->ClientsWithInput);
    OSCLEARBITS(ptp->ClientsWithOutput);
    OSCLEARBITS(ptp->ClientsWithFileOutput);
    OSCLEARBITS(ptp->ClientsWithFileInput);
    OSCLEARBITS(ptp->ClientsWithBaseOutput);
    OSCLEARBITS(ptp->ClientsWithBaseInput);
    OSCLEARBITS(ptp->OutputPending);
    OSCLEARBITS(ptp->InputPending);
    OSCLEARBITS(ptp->SavedAllClients);
    OSCLEARBITS(ptp->SavedAllSockets);
    OSCLEARBITS(ptp->SavedClientsWithInput);
    OSCLEARBITS(ptp->SavedClientsWithOutput);
    OSCLEARBITS(ptp->SavedClientsWithFileInput);
    OSCLEARBITS(ptp->SavedClientsWithFileOutput);
    OSCLEARBITS(ptp->SavedClientsWithBaseInput);
    OSCLEARBITS(ptp->SavedClientsWithBaseOutput);
    OSCLEARBITS(ptp->ClientsWriteBlocked);
    OSCLEARBITS(ptp->WellKnownConnections );
    OSCLEARBITS(ptp->LocalListening );
    OSCLEARBITS(ptp->DistantListening );
    OSCLEARBITS(ptp->ConnectionPending );
    ptp->CurrentMaxClients = ptp->NextFreeClientId = 0;
    ptp->AnyClientsWriteBlocked = ptp->GrabInProgress = FALSE;
    ptp->ApplicationProcedures = NULL;
    ptp->ConnectionProcedures = NULL;
    ptp->History = NULL;
    ptp->ClientHistoryNumber = 0;

    for (i = 0; i < SOCKMAX; i++)
    {
        ptp->ConnectionTranslation[i] = 0;
        ptp->Clients[i] = NULL;
    }
    if (name)
        strcpy (ptp->Name, name);
    else strcpy (ptp->Name, "");

    if (appliname)
        strcpy (ptp->AppliName, appliname);
    else strcpy (ptp->AppliName, "");

    ptp->Mode = TPCLIENT;
    ptp->ListeningPorts = NULL;
    TPAddApplicationProcedure (ptp, TPSendIfAnyOutput, NULL);
    TPAddApplicationProcedure (ptp, TPRecvIfAnyInput, NULL);
    return 1;
}


TPPort* TPCreatePort (TP* ptp, int socket, int port)
{
    TPPort* pport = (TPPort *)malloc (sizeof (TPPort));
    pport->Port = port;
    pport->fd = socket;
    pport->Clients = NULL;
    return pport;
}

int TPFindFdFromPort (TP* ptp, int port)
{
    List* LPorts = ptp->ListeningPorts;
    TPPort* pport;

    while (LPorts)
    {
        pport = (TPPort *)LPorts->car;
        if (pport->Port == port) return pport->fd;
        LPorts = LPorts->cdr;
    }
    return -1;
}

TPPort* TPFindPortFromFd (TP* ptp, int listenid)
{
    List* LPorts = ptp->ListeningPorts;
    TPPort* pport;

    while (LPorts)
    {
        pport = (TPPort *)LPorts->car;
        if (pport->fd == listenid) return pport;
        LPorts = LPorts->cdr;
    }
    return NULL;
}


int TPListenToLocal (TP* ptp)
{
    int request;
    if ((request = OSOpenLocalSocket (ptp->OS, ptp->Name, ptp->AppliName)) < 0)
        return TP_NOK;
    OSBITSET(ptp->LocalListening    , request);
    OSBITSET(ptp->WellKnownConnections, request);
    OSBITSET(ptp->AllSockets, request);
    return request;
}


int TPListenToTCP (TP* ptp, int Port, int Protocol)
{
    int request;
    TPPort* pport;
    if ((request = OSOpenTcpSocket (ptp->OS, (WORD)Port)) < 0)
        return TP_NOK;

    pport = TPCreatePort (ptp, request, Port);
    pport->Protocol = Protocol;
    ListNewr (&ptp->ListeningPorts, pport);
    ptp->Mode = TPSERVER;

    OSBITSET(ptp->DistantListening    , request);
    OSBITSET(ptp->WellKnownConnections, request);
    OSBITSET(ptp->AllSockets, request);
    return request;
}


void TPConnectionOK (TP* ptp, TPClient* pclient)
{
    int request = pclient->Pipe->fd;
    DWORD one = 1;
    List* plist;
    TPConnectionProcedure* ConnectionProc;

#ifdef SO_LINGER
    int linger[2] = {0, 0};
    OSSetSockOpt (ptp->OS, request, SOL_SOCKET, SO_LINGER, &linger, sizeof (linger));
#endif
    if (OSIoctl (ptp->OS, request, FIONBIO, &one) < 0)
        OSError ("TESYS029", "socket : %d", request);

    plist = ptp->ConnectionProcedures;
    while (plist)
    {
        ConnectionProc = (TPConnectionProcedure *)plist->car;
        ConnectionProc->ExecuteProcedure(pclient,ConnectionProc->ApplicationField);
        plist = plist->cdr;
    }
    if ((pclient->Protocol != IPPROTO_ICMP) &&
            (pclient->Protocol != IPPROTO_IP))
    {
        pclient->Connect = TRUE;
        OSGetPeerName (ptp->OS, request, pclient->Type, pclient->WithUserId, pclient->WithName);
    }
}


TPClient* TPOpenFile (TP* ptp, char* filename, char* mode, int protocol, char loc, int size)
{
    FILEID* fileid;
    TPClient* pclient;
    List* plist;
    TPConnectionProcedure* ConnectionProc;
    int error = 0;

    fileid = OSOpenFile (filename, mode , 'b', loc, &error);
    if (!fileid)
        return NULL;
    fileid->LineSize = size;
    pclient = TPNextAvailableClient (ptp);
    if (!pclient)
    {
        OSCloseFile (fileid);
        return NULL;
    }
    TPSetClientProtocol (ptp, pclient, protocol) ;
    pclient->Type = 10000;
    pclient->Connect = TRUE;
    pclient->ErrorIndex = 0;
    pclient->CommunicationTime = time (NULL);
    pclient->Pipe->fileid = fileid;
    if (ptp->GrabInProgress)
        OSBITSET(ptp->SavedClientsWithFileOutput, pclient->Index + 1);
    else
        OSBITSET(ptp->ClientsWithFileOutput, pclient->Index + 1);
    OSBITSET(ptp->AllFileClients, pclient->Index + 1);


    plist = ptp->ConnectionProcedures;
    while (plist)
    {
        ConnectionProc = (TPConnectionProcedure *)plist->car;
        ConnectionProc->ExecuteProcedure(pclient,ConnectionProc->ApplicationField);
        plist = plist->cdr;
    }

    return pclient;
}



TPClient* TPOpenBase (TP* ptp, char* databasename, char* username, char* password, int protocol)
{
    TPClient*    pclient;
    int          connection;
    List*        plist;
    TPConnectionProcedure* ConnectionProc;

    if ((connection = OSConnect (databasename, username, password, protocol, NULL, NULL)) <= 0)
    {
        return NULL;
    }

    pclient = TPNextAvailableClient (ptp);
    if (!pclient)
    {
        OSDisconnect ((void*)connection, pclient->Protocol, 0, NULL, NULL);
        return NULL;
    }
    TPSetClientProtocol (ptp, pclient, protocol) ;
    pclient->Type       = 10000;
    pclient->Connect    = TRUE; 
    pclient->ErrorIndex = 0;
    pclient->CommunicationTime = time (NULL);
    pclient->Pipe->connection = (void*)connection;
    if (ptp->GrabInProgress)
        OSBITSET(ptp->SavedClientsWithBaseOutput, pclient->Index + 1);
    else
        OSBITSET(ptp->ClientsWithBaseOutput, pclient->Index + 1);
    OSBITSET(ptp->AllBaseClients, pclient->Index + 1);

    plist = ptp->ConnectionProcedures;
    while (plist)
    {
        ConnectionProc = (TPConnectionProcedure *)plist->car;
        ConnectionProc->ExecuteProcedure(pclient,ConnectionProc->ApplicationField);
        plist = plist->cdr;
    }
    return pclient;
}



TPClient* TPEstablishConnectionWith (TP* ptp, char* machinname, int port, int protocol,
                                     char* user, char* appli, BOOL Block)
{
    int request;
    BOOL FlagOK = FALSE;
    TPClient* pclient;


    if (user)
        request = OSLocalConnect (ptp->OS, user, appli);
    else
        request = OSTcpConnect (ptp->OS, machinname, (WORD)port, Block, &FlagOK); /* FlagOk means connecion ok for non blocking socket*/
    if (request < 0)
        return NULL;

    pclient = TPNextAvailableClient (ptp);
    if (!pclient)
    {
        OSCloseSocket (ptp->OS, request);
        return NULL;
    }

    TPSetClientProtocol (ptp, pclient, protocol) ;
    pclient->Connect = FlagOK;
    pclient->ErrorIndex = 0;
    pclient->CommunicationTime = time (NULL);
    pclient->Pipe->fd = request;
    ptp->ConnectionTranslation[request] = pclient->Index;

    if (ptp->GrabInProgress)
    {
        OSBITSET(ptp->SavedAllClients, request);
        OSBITSET(ptp->SavedAllSockets, request);
        OSBITSET(ptp->SavedClientsWithOutput, request);
    }
    else
    {
        OSBITSET(ptp->AllClients, request);
        OSBITSET(ptp->AllSockets, request);
        OSBITSET(ptp->ClientsWithOutput, request);
    }
    /* pour les connexions non bnloquantes */
    if (pclient->Connect == TRUE)
        TPConnectionOK (ptp, pclient);
    else
    {
        OSBITSET(ptp->ClientsWriteBlocked, request);
        OSBITCLEAR(ptp->ClientsWithOutput, request);
        ptp->AnyClientsWriteBlocked = TRUE;
    }
    return pclient;
}



TPClient* TPCreateInetClient (TP* ptp, int type)
{
    int request;
    TPClient* pclient;

    request = OSOpenInetSocket (ptp->OS, type, FALSE);
    if (request < 0)
        return NULL;
    pclient = TPCreateClientWithFd (ptp, request);
    if (!pclient)
    {
        OSCloseSocket (ptp->OS, request);
        return NULL;
    }
    TPSetClientProtocol (ptp, pclient, type)      ;
    TPConnectionOK (ptp, pclient);
    return pclient;
}



void TPSetClientProtocol (TP* ptp, TPClient* pclient, int type)
{
    switch (type)
    {
    case IPPROTO_ICMP :
        pclient->Protocol          = IPPROTO_ICMP;
        pclient->ReadFunction      = TPReadIcmpFromClient;
        pclient->WriteFunction     = TPWriteIcmpToClient;
        break;
    case IPPROTO_IP :
        pclient->Protocol          = IPPROTO_IP;
        pclient->ReadFunction      = TPReadIpFromClient;
        pclient->WriteFunction     = TPWriteIpToClient;
        break;
    case IPPROTO_UDP :
        pclient->Protocol          = IPPROTO_UDP;
        pclient->ReadFunction      = TPReadUdpFromClient;
        pclient->WriteFunction     = TPWriteUdpToClient;
        break;
    case IPPROTO_FTP :
        pclient->Protocol          = IPPROTO_FTP;
        pclient->ReadFunction      = TPReadTcpFromClient;
        pclient->WriteFunction     = TPWriteTcpToClient;
        break;
    case IPPROTO_TCP :
        pclient->Protocol          = IPPROTO_TCP;
        pclient->ReadFunction      = TPReadTcpFromClient;
        pclient->WriteFunction     = TPWriteTcpToClient;
        break;
    case IPPROTO_MTT :
        pclient->Protocol          = IPPROTO_MTT;
        pclient->ReadFunction      = TPReadTcpFromClient;
        pclient->WriteFunction     = TPWriteTcpToClient;
        break;
	case IPPROTO_HTTP :
        pclient->Protocol          = IPPROTO_HTTP;
        pclient->ReadFunction      = TPReadTcpFromClient;
        pclient->WriteFunction     = TPWriteTcpToClient;
        break;
    case IPPROTO_DNS :
        pclient->Protocol          = IPPROTO_DNS;
        pclient->ReadFunction      = TPReadDNSFromClient;
        pclient->WriteFunction     = TPWriteDNSToClient;
        break;
    case IPPROTO_BC :
        pclient->Protocol          = IPPROTO_BC;
        pclient->ReadFunction      = TPReadRequestFromClient;
        pclient->WriteFunction     = TPWriteRequestToClient;
        break;
    case IPPROTO_DG :
        pclient->Protocol          = IPPROTO_DG;
        pclient->ReadFunction      = TPReadRequestFromClient;
        pclient->WriteFunction     = TPWriteRequestToClient;
        break;
    case IPPROTO_CB2A :
        pclient->Protocol          = IPPROTO_CB2A;
        pclient->ReadFunction      = TPReadTcpFromClient;
        pclient->WriteFunction     = TPWriteTcpToClient;
        break;
    case IPPROTO_SMTP :
        pclient->Protocol          = IPPROTO_SMTP;
        pclient->ReadFunction      = TPReadTcpFromClient;
        pclient->WriteFunction     = TPWriteTcpToClient;
        break;
    case IPPROTO_POP :
        pclient->Protocol          = IPPROTO_POP;
        pclient->ReadFunction      = TPReadTcpFromClient;
        pclient->WriteFunction     = TPWriteTcpToClient;
        break;
    case IOPROTO_STORE :
        pclient->Protocol          = IOPROTO_STORE;
        pclient->ReadFunction      = TPReadRequestFromFile;
        pclient->WriteFunction     = TPWriteRequestToFile;
        break;
    case IOPROTO_FIOP :
        pclient->Protocol          = IOPROTO_FIOP;
        pclient->ReadFunction      = TPReadStreamFromFile;
        pclient->WriteFunction     = TPWriteStreamToFile;
        break;
    case DBPROTO_ORACLE :
    case DBPROTO_ODBC :
    case DBPROTO_SQLDS :
        pclient->Protocol          = type;
        pclient->WriteFunction     = TPWriteStreamToBase;
        pclient->ReadFunction      = TPReadStreamFromBase;
        break;

    }
    return;
}



TPClient* TPCreateClientWithFd (TP* ptp, int fd)
{
    TPClient* pclient;
    int i;
    if (fd < 0)
        return NULL;

    for (i = 0; i < ptp->CurrentMaxClients; i++)
        if (ptp->Clients[i])
            if (ptp->Clients[i]->Pipe->fd == fd) return NULL;

    pclient = TPNextAvailableClient (ptp);
    if (!pclient)
        return NULL;
    pclient->Type = 0;
    pclient->ErrorIndex = 0;
    pclient->CommunicationTime = time (NULL);
    pclient->Pipe->fd = fd;
    ptp->ConnectionTranslation[fd] = pclient->Index;

    if (ptp->GrabInProgress)
    {
        OSBITSET(ptp->SavedAllClients, fd);
        OSBITSET(ptp->SavedAllSockets, fd);
        OSBITSET(ptp->SavedClientsWithOutput, fd);
    }
    else
    {
        OSBITSET(ptp->AllClients, fd);
        OSBITSET(ptp->AllSockets, fd);
        OSBITSET(ptp->ClientsWithOutput, fd);
    }
    return pclient;
}

TPClient* TPCreateClientWithFileId (TP* ptp, FILEID* fileid)
{
    TPClient* pclient;
    int i;
    if (fileid == NULL)
        return NULL;

    for (i = 0; i < ptp->CurrentMaxClients; i++)
        if (ptp->Clients[i])
            if (ptp->Clients[i]->Pipe->fileid == fileid) return NULL;

    pclient = TPNextAvailableClient (ptp);
    if (!pclient)
        return NULL;
    pclient->Type = 0;
    pclient->ErrorIndex = 0;
    pclient->CommunicationTime = time (NULL);
    pclient->Pipe->fileid = fileid;
    if (ptp->GrabInProgress)
        OSBITSET(ptp->SavedClientsWithFileOutput, pclient->Index + 1);
    else
        OSBITSET(ptp->ClientsWithFileOutput, pclient->Index + 1);
    OSBITSET(ptp->AllFileClients, pclient->Index + 1);

    return pclient;
}


BOOL TPDistant(TP *ptp, int s)
{

    if (OSGETBIT(ptp->DistantListening, s))
        return TRUE;
    else
        return FALSE;
}

BOOL TPLocal(TP *ptp, int s)
{

    if (OSGETBIT(ptp->LocalListening, s))
        return TRUE;
    else
        return FALSE;
}


TPClient* TPEstablishConnectionOn (TP* ptp, int listenid)
{
    int request;
    TPClient* pclient;
    TPPort* pport;
    List* plist;
    TPConnectionProcedure* ConnectionProc;

    OSBITCLEAR(ptp->ConnectionPending , listenid);

    if ((request = OSAccept (ptp->OS, listenid)) < 0)
        return NULL;

    pclient = TPNextAvailableClient (ptp);
    if (!pclient)
    {
        OSCloseSocket (ptp->OS, request);
        return NULL;
    }

    pclient->Protocol = IPPROTO_DG;
    pclient->ErrorIndex = 0;
    pclient->CommunicationTime = time (NULL);

    OSGetPeerName (ptp->OS, request, pclient->Type, pclient->WithUserId, pclient->WithName);
    /* in case of local get clientid and put it in TPClient */

    pclient->Pipe->fd = request;
    ptp->ConnectionTranslation[request] = pclient->Index;


    if (ptp->GrabInProgress)
    {
        OSBITSET(ptp->SavedAllClients, request);
        OSBITSET(ptp->SavedAllSockets, request);
        OSBITSET(ptp->SavedClientsWithOutput, request);
    }
    else
    {
        OSBITSET(ptp->AllClients, request);
        OSBITSET(ptp->AllSockets, request);
        OSBITSET(ptp->ClientsWithOutput, request);
    }



    if (TPDistant(ptp, listenid))
    {
        pclient->Type = 1;
        pport = TPFindPortFromFd (ptp, listenid);
        if (pport) ListNewr (&pport->Clients, pclient);
        pclient->OnPort = pport;
        TPSetClientProtocol (ptp, pclient, pport->Protocol);
        plist = ptp->ConnectionProcedures;
        while (plist)
        {
            ConnectionProc = (TPConnectionProcedure *)plist->car;
            ConnectionProc->ExecuteProcedure(pclient,ConnectionProc->ApplicationField);
            plist = plist->cdr;
        }
    }
    else  pclient->Type = 0;

    return pclient;
}

int TPEstablishAllNewConnections (TP* ptp)
{

    int curcon;
    int ntplient = 0;
    int i;

    for (i = 0; i < MaskCount; i++)
    {
        if (ptp->ConnectionPending[i])
        {
            curcon = OSFFS (&ptp->ConnectionPending[i]);
            ptp->ConnectionPending[i] &= ~(1L << curcon);
            if (TPEstablishConnectionOn (ptp, curcon)) ntplient++;
        }
    }
    return ntplient;
}

int TPGiveConnection (TP* ptp, TPClient* pclient, char* name)
{
    int ret;
    ret = OSGiveSocket (ptp->OS, pclient->Pipe->fd, name);
    pclient->ClientGone = TRUE;
    return ret;
}


TPClient* TPTakeConnection (TP* ptp, char* name,
                            char* subtaskname, int fromdescriptor)
{
    TPClient* pclient;
    int request =  OSTakeSocket (ptp->OS, fromdescriptor, name, subtaskname);
    if (request < 0)
    {
        OSError ("TEBAS001", NULL);
        return NULL;
    }

    pclient = TPNextAvailableClient (ptp);
    if (!pclient)
    {
        OSCloseSocket (ptp->OS, request);
        return NULL;
    }

    pclient->Type = 1;
    pclient->ErrorIndex = 0;
    OSGetPeerName (ptp->OS, request, pclient->Type, pclient->WithUserId, pclient->WithName);
    pclient->Pipe->fd = request;
    ptp->ConnectionTranslation[request] = pclient->Index;

    if (ptp->GrabInProgress)
    {
        OSBITSET(ptp->SavedAllClients, request);
        OSBITSET(ptp->SavedAllSockets, request);
        OSBITSET(ptp->SavedClientsWithOutput, request);
    }
    else
    {
        OSBITSET(ptp->AllClients, request);
        OSBITSET(ptp->AllSockets, request);
        OSBITSET(ptp->ClientsWithOutput, request);

    }
    return pclient;
}


void TPCloseDownConnection (TP* ptp, TPClient* pclient)
{
    PIPE* pcom = pclient->Pipe;
    int connection = pcom->fd;


    if (pclient->Pipe->fileid)
    {
        OSCloseFile (pclient->Pipe->fileid);
        OSFreePipe (pcom);
        pclient->Pipe = NULL;
        OSBITCLEAR(ptp->ClientsWithFileInput,  pclient->Index + 1);
        OSBITCLEAR(ptp->ClientsWithFileOutput, pclient->Index + 1);
        OSBITCLEAR(ptp->AllFileClients,  pclient->Index + 1);
        if (ptp->GrabInProgress)
        {
            OSBITCLEAR(ptp->SavedClientsWithFileInput,  pclient->Index + 1);
            OSBITCLEAR(ptp->SavedClientsWithFileOutput, pclient->Index + 1);
        }
        return;
    }
    if (pclient->Pipe->connection)
    {
        OSDisconnect (pclient->Pipe->connection, pclient->Protocol, FALSE, NULL, NULL);
        OSFreePipe (pcom);
        pclient->Pipe = NULL;
        OSBITCLEAR(ptp->ClientsWithBaseInput,  pclient->Index + 1);
        OSBITCLEAR(ptp->ClientsWithBaseOutput, pclient->Index + 1);
        OSBITCLEAR(ptp->AllBaseClients,  pclient->Index + 1);
        if (ptp->GrabInProgress)
        {
            OSBITCLEAR(ptp->SavedClientsWithBaseInput,  pclient->Index + 1);
            OSBITCLEAR(ptp->SavedClientsWithBaseOutput, pclient->Index + 1);
        }
        return;
    }
    if (pcom->Output && pcom->Output->Count)
    {
        TPFlushClient (ptp, pclient);
    }
    OSCloseSocket (ptp->OS, connection);

    if (pclient->OnPort)
    {
        ListRemove (&pclient->OnPort->Clients, pclient);
    }

    ptp->ConnectionTranslation[pcom->fd] = 0;

    OSBITCLEAR(ptp->AllSockets, connection);
    OSBITCLEAR(ptp->AllClients, connection);
    OSBITCLEAR(ptp->ClientsWithInput, connection);
    OSBITCLEAR(ptp->ClientsWithOutput, connection);
    if (ptp->GrabInProgress)
    {
        OSBITCLEAR(ptp->SavedAllSockets, connection);
        OSBITCLEAR(ptp->SavedAllClients, connection);
        OSBITCLEAR(ptp->SavedClientsWithInput, connection);
        OSBITCLEAR(ptp->SavedClientsWithOutput, connection);
    }
    OSBITCLEAR(ptp->ClientsWriteBlocked, connection);
    if (!OSANYSET(ptp->ClientsWriteBlocked))
        ptp->AnyClientsWriteBlocked = FALSE;
    OSBITCLEAR(ptp->OutputPending, connection);
    OSBITCLEAR(ptp->InputPending,  connection);
}


int TPCheckConnection (TP* ptp, TPClient* pclient)
{
    struct timeval notime;
    int ret;
    long tmask[MaskCount];
    int connection = pclient->Pipe->fd;


    notime.tv_sec = 0;
    notime.tv_usec = 0;
    OSCLEARBITS(tmask);
    OSBITSET(tmask, connection);

    ret =  OSSelect (ptp->OS, connection + 1, tmask, NULL, NULL, &notime);
    if (ret < 0)
        return TP_NOK;
    else
        return TP_OK;
}


void TPCloseDamagedConnections (TP* ptp)
{

    struct timeval notime;
    int i, ret, curoff, curclient;
    long mask = 0;
    long tmask[MaskCount];
    notime.tv_sec = 0;
    notime.tv_usec = 0;

    for (i =0; i < MaskCount; i++)
    {
        mask  = ptp->AllClients[i];
        while (mask)
        {
            curoff = OSFFS(&mask);
            curclient = curoff + (i << 5);
            OSCLEARBITS(tmask);
            OSBITSET(tmask, curclient);
            ret =  OSSelect (ptp->OS, curclient + 1, tmask, NULL,NULL, &notime);
            if (ret < 0)
                TPCloseDownClient (ptp,
                                   ptp->Clients[ptp->ConnectionTranslation[curclient]]);
            mask &= ~(1L << curoff);
        }
    }
}


void TPCloseDownClient (TP* ptp, TPClient* pclient)
{
    List* plist;
    TPEndingProcedure* EndingProc;
    pclient->ClientGone = TRUE;


    plist = pclient->EndingProcedures;
    while (plist)
    {
        EndingProc = (TPEndingProcedure *)plist->car;
        EndingProc->EndingProcedure(pclient, EndingProc->ApplicationField);
        plist = plist->cdr;
    }

    /*
      if (pclient->EndingProcedure)
       pclient->EndingProcedure(pclient->ApplicationField);
     */
    TPCloseDownConnection (ptp, pclient);
    if (pclient->Index < ptp->NextFreeClientId)
        ptp->NextFreeClientId = pclient->Index;
    ptp->Clients[pclient->Index] = NULL;

    if (pclient->TransBuffer)
        free ((BYTE*)pclient->TransBuffer);
    pclient->TransBuffer = NULL;



    while ((ptp->CurrentMaxClients > 0) && (!ptp->Clients[ptp->CurrentMaxClients - 1]))
        ptp->CurrentMaxClients--;


    ListNewr (&ptp->History, pclient);
    ptp->ClientHistoryNumber++;
    if (ptp->ClientHistoryNumber > TPNumberClientInHistory)
    {
        TPClient* pclient = (TPClient *) ptp->History->car;
        ListRemove (&ptp->History, pclient);
        free (pclient);
        (ptp->ClientHistoryNumber)--;
    }
}


void TPKillAllClients (TP* ptp)
{
    int i;
    for (i = 0; i < ptp->CurrentMaxClients; i++)
        if (ptp->Clients[i])
            TPCloseDownClient (ptp, ptp->Clients[i]);
}



void TPOnlyListenToOneClient (TP* ptp, TPClient* pclient)
{
    int connection = pclient->Pipe->fd;
    if (!ptp->GrabInProgress)
    {
        OSCOPYBITS(ptp->ClientsWithInput, ptp->SavedClientsWithInput);
        OSCOPYBITS(ptp->ClientsWithFileInput, ptp->SavedClientsWithFileInput);

        if (pclient->Pipe->fd)
        {
            OSBITCLEAR(ptp->SavedClientsWithInput, connection);
            if (OSGETBIT(ptp->ClientsWithInput, connection))
            {
                OSCLEARBITS(ptp->ClientsWithInput);
                OSBITSET(ptp->ClientsWithInput, connection);
            }
            else
                OSCLEARBITS(ptp->ClientsWithInput);
        }
        else
            if (pclient->Pipe->fileid)
            {
                OSBITCLEAR(ptp->SavedClientsWithFileInput,  pclient->Index + 1);
                if (OSGETBIT(ptp->ClientsWithFileInput, pclient->Index + 1))
                {
                    OSCLEARBITS(ptp->ClientsWithFileInput);
                    OSBITSET(ptp->ClientsWithFileInput, pclient->Index + 1);
                }
                else
                    OSCLEARBITS(ptp->ClientsWithFileInput);
            }
            else
                if (pclient->Pipe->connection)
                {
                    OSBITCLEAR(ptp->SavedClientsWithBaseInput,  pclient->Index + 1);
                    if (OSGETBIT(ptp->ClientsWithBaseInput, pclient->Index + 1))
                    {
                        OSCLEARBITS(ptp->ClientsWithBaseInput);
                        OSBITSET(ptp->ClientsWithBaseInput, pclient->Index + 1);
                    }
                    else
                        OSCLEARBITS(ptp->ClientsWithBaseInput);
                }
        OSCOPYBITS(ptp->AllSockets, ptp->SavedAllSockets);
        OSCOPYBITS(ptp->AllClients, ptp->SavedAllClients);
        OSUNSETBITS(ptp->AllSockets, ptp->AllClients);
        OSCLEARBITS(ptp->AllClients);

        if (pclient->Pipe->fileid == NULL)
        {
            OSBITSET(ptp->AllSockets, connection);
            OSBITSET(ptp->AllClients, connection);
        }
        ptp->GrabInProgress = pclient->Index;
    }
}


void TPListenToAllClients (TP* ptp)
{
    if (ptp->GrabInProgress)
    {
        OSORBITS(ptp->AllSockets, ptp->AllSockets, ptp->SavedAllSockets);
        OSORBITS(ptp->AllClients, ptp->AllClients, ptp->SavedAllClients);
        OSORBITS(ptp->ClientsWithInput, ptp->ClientsWithInput,
                 ptp->SavedClientsWithInput);
        OSORBITS(ptp->ClientsWithOutput, ptp->ClientsWithOutput,
                 ptp->SavedClientsWithOutput);
        OSORBITS(ptp->ClientsWithFileInput, ptp->ClientsWithFileInput,
                 ptp->SavedClientsWithFileInput);
        OSORBITS(ptp->ClientsWithFileOutput, ptp->ClientsWithFileOutput,
                 ptp->SavedClientsWithOutput);
        ptp->GrabInProgress = 0;
    }
}

static long IgnoredClientsWithInput[MaskCount];
static long IgnoredSavedClientsWithInput[MaskCount];

static long IgnoredClientsWithFileInput[MaskCount];
static long IgnoredSavedClientsWithFileInput[MaskCount];

static long IgnoredClientsWithBaseInput[MaskCount];
static long IgnoredSavedClientsWithBaseInput[MaskCount];



void TPIgnoreClient (TP* ptp, TPClient* pclient)
{
    int connection = pclient->Pipe->fd;
    if (pclient->Pipe->fd)
    {

        if (OSGETBIT(ptp->ClientsWithInput, connection))
            OSBITSET(IgnoredClientsWithInput, connection);
        else
            OSBITCLEAR(IgnoredClientsWithInput, connection);
        OSBITCLEAR(ptp->ClientsWithInput, connection);
        OSBITCLEAR(ptp->ClientsWithOutput, connection);
        OSBITCLEAR(ptp->AllSockets, connection);
        OSBITCLEAR (ptp->AllClients,connection);
        if (ptp->GrabInProgress)
        {
            if (OSGETBIT(ptp->SavedClientsWithInput, connection))
                OSBITSET(IgnoredSavedClientsWithInput, connection);
            else
                OSBITCLEAR(IgnoredSavedClientsWithInput, connection);

            OSBITCLEAR(ptp->SavedClientsWithInput, connection);
            OSBITCLEAR(ptp->SavedClientsWithOutput, connection);
            OSBITCLEAR(ptp->SavedAllSockets, connection);
            OSBITCLEAR(ptp->SavedAllClients, connection);
        }
    }
    else
        if (pclient->Pipe->fileid)
        {
            if (OSGETBIT(ptp->ClientsWithFileInput, pclient->Index + 1))
                OSBITSET(IgnoredClientsWithFileInput, pclient->Index + 1);
            else
                OSBITCLEAR(IgnoredClientsWithFileInput, pclient->Index + 1);
            OSBITCLEAR(ptp->ClientsWithFileInput, pclient->Index + 1);
            OSBITCLEAR(ptp->ClientsWithFileOutput, pclient->Index + 1);
            if (ptp->GrabInProgress)
            {
                if (OSGETBIT(ptp->SavedClientsWithFileInput, pclient->Index + 1))
                    OSBITSET(IgnoredSavedClientsWithFileInput, pclient->Index + 1);
                else
                    OSBITCLEAR(IgnoredSavedClientsWithFileInput, pclient->Index + 1);

                OSBITCLEAR(ptp->SavedClientsWithFileInput, pclient->Index + 1);
                OSBITCLEAR(ptp->SavedClientsWithFileOutput, pclient->Index + 1);
            }
        }
        else
            if (pclient->Pipe->connection)
            {
                if (OSGETBIT(ptp->ClientsWithBaseInput, pclient->Index + 1))
                    OSBITSET(IgnoredClientsWithBaseInput, pclient->Index + 1);
                else
                    OSBITCLEAR(IgnoredClientsWithBaseInput, pclient->Index + 1);
                OSBITCLEAR(ptp->ClientsWithBaseInput, pclient->Index + 1);
                OSBITCLEAR(ptp->ClientsWithBaseOutput, pclient->Index + 1);
                if (ptp->GrabInProgress)
                {
                    if (OSGETBIT(ptp->SavedClientsWithBaseInput, pclient->Index + 1))
                        OSBITSET(IgnoredSavedClientsWithBaseInput, pclient->Index + 1);
                    else
                        OSBITCLEAR(IgnoredSavedClientsWithBaseInput, pclient->Index + 1);

                    OSBITCLEAR(ptp->SavedClientsWithBaseInput, pclient->Index + 1);
                    OSBITCLEAR(ptp->SavedClientsWithBaseOutput, pclient->Index + 1);
                }
            }
}

void TPAttendClient (TP* ptp, TPClient* pclient)
{
    int connection = pclient->Pipe->fd;
    if (!ptp->GrabInProgress || ptp->GrabInProgress == pclient->Index)
    {
        if (pclient->Pipe->fd)
        {
            OSBITSET(ptp->AllClients, connection);
            OSBITSET(ptp->AllSockets, connection);
            if (OSGETBIT(IgnoredClientsWithInput, connection))
                OSBITSET(ptp->ClientsWithInput, connection);
        }
        else
            if (pclient->Pipe->fileid)
            {
                if (OSGETBIT(IgnoredClientsWithFileInput, pclient->Index + 1))
                    OSBITSET(ptp->ClientsWithFileInput, pclient->Index + 1);
            }
            else
                if (pclient->Pipe->connection)
                {
                    if (OSGETBIT(IgnoredClientsWithBaseInput, pclient->Index + 1))
                        OSBITSET(ptp->ClientsWithBaseInput, pclient->Index + 1);
                }

    }
    else
    {
        if (pclient->Pipe->fd)
        {
            OSBITSET(ptp->SavedAllClients, connection);
            OSBITSET(ptp->SavedAllSockets, connection);
            if (OSGETBIT(IgnoredSavedClientsWithInput, connection))
                OSBITSET(ptp->SavedClientsWithInput, connection);
        }
        else
            if (pclient->Pipe->fileid)
            {
                if (OSGETBIT(IgnoredSavedClientsWithFileInput, pclient->Index + 1))
                    OSBITSET(ptp->SavedClientsWithFileInput, pclient->Index + 1);
            }
            else
                if (pclient->Pipe->connection)
                {
                    if (OSGETBIT(IgnoredSavedClientsWithBaseInput, pclient->Index + 1))
                        OSBITSET(ptp->SavedClientsWithBaseInput, pclient->Index + 1);
                }
    }

}


void  TPSetReadFunction (TP* ptp, TPClient* pclient, int (*proc) (TP* ptp, TPClient* pclient))
{
    pclient->ReadFunction  = proc;
}

void  TPSetWriteFunction (TP* ptp, TPClient* pclient, int (*proc) (TP* ptp, TPClient* pclient,
                          char* buf, int Count))
{
    pclient->WriteFunction  = proc;
}



void  TPSetReceiveFunction (TP* ptp, TPClient* pclient, LONG (*proc) (TP* ptp, TPClient* pclient, char* buf, int bufsize))
{
    pclient->ReceiveFunction  = proc;
}

void  TPSetSendFunction (TP* ptp, TPClient* pclient, LONG (*proc) (TP* ptp, TPClient* pclient, char* buf, int count))
{
    pclient->SendFunction  = proc;
}




TPClient* TPNextAvailableClient (TP* ptp)
{
    TPClient* pclient;
    PIPE* pcom;
    int i;
    char* buffer;


    i = ptp->NextFreeClientId;
    if (i == TPMaxClients)
    {
        OSError ("TEBAS002", NULL);
        return  NULL;
    }
    pcom = OSAllocatePipe ();
    if (!pcom)
    {
        return  NULL;
    }
    pclient = (TPClient*)malloc (sizeof (TPClient));
    if (!pclient)
    {
        free ((BYTE *)pcom);
        OSError ("TEBAS002", NULL);
        return NULL;
    }

    ptp->Clients[i]            = pclient;
    pclient->WithUserId[0]     = pclient->WithName[0] = 0;
    pclient->Pipe              = pcom;
    pclient->Index             = i;
    pclient->ClientGone        = FALSE;
    pclient->NoClientException = TRUE;
    pclient->RequestBuffer     = NULL;
    pclient->EndingProcedures  = NULL;
    pclient->ApplicationField  = NULL;
    pclient->UserField         = NULL;
    pclient->ReceiptSize       = 0;
    pclient->TransferSize      = 0;
    pclient->Blocked           = FALSE;
    pclient->Connect           = TRUE;
    pclient->OnPort            = NULL;

    TPSetClientProtocol (ptp, pclient, IPPROTO_BC);
    pclient->SendFunction      = TPDefaultSend;
    pclient->ReceiveFunction   = TPDefaultReceive;

    buffer = (char*)malloc (TPBUFOUTPUTSIZE);
    if (!buffer)
    {
        free ((BYTE*)pcom);
        return NULL;
    }
    pclient->TransSize = TPBUFOUTPUTSIZE;
    pclient->TransBuffer = buffer;
    pclient->TP = ptp;


    if (i == ptp->CurrentMaxClients) (ptp->CurrentMaxClients)++;
    while ((ptp->NextFreeClientId < TPMaxClients) &&
            ptp->Clients[ptp->NextFreeClientId])
        (ptp->NextFreeClientId)++;


    return pclient;
}

int TPSetBlockingMode (TPClient* pclient, BOOL blocking)
{

    if ((pclient->Pipe->fileid) ||
            (pclient->Pipe->connection))
        return 0;

    if ((blocking) && (!pclient->Blocked))
    {
        DWORD zero = 0;
        pclient->Blocked = TRUE;
        return OSIoctl (pclient->TP->OS, pclient->Pipe->fd, FIONBIO, &zero);
    }
    if ((!blocking) && pclient->Blocked)
    {
        DWORD one = 1;
        pclient->Blocked = FALSE;
        return OSIoctl (pclient->TP->OS, pclient->Pipe->fd, FIONBIO, &one);

    }
    return 0;
}

int TPWaitForSomething (TP* ptp, DWORD microseconds)
{
    int err;
    struct timeval waittime;
    struct timeval* tp;
    int i, givenclient;
    ldiv_t lt;
    long clientsWritable[MaskCount];
    long clientsException[MaskCount];
    long clientsNotConnected[MaskCount];
    tp = NULL;

    waittime.tv_sec = 0;
    waittime.tv_usec = 0;

    if (microseconds > 0 )
    {
        if (microseconds < 1000000)
            waittime.tv_usec =  microseconds;
        else
        {
            lt = ldiv (microseconds, 1000000);
            waittime.tv_usec = lt.rem;
            waittime.tv_sec = lt.quot;
        }
        tp = &waittime;
    }
    OSCOPYBITS(ptp->AllSockets, clientsException);
    OSCOPYBITS(ptp->AllSockets, ptp->LastSelectMask);
    OSCOPYBITS(ptp->AllClients, ptp->ClientsWithOutput);
    OSCOPYBITS(ptp->AllFileClients, ptp->ClientsWithFileOutput);
    if (ptp->AnyClientsWriteBlocked)
    {
        OSUNSETBITS(ptp->ClientsWithOutput, ptp->ClientsWriteBlocked);
        OSCOPYBITS(ptp->ClientsWriteBlocked, clientsWritable);
        i = OSSelect (ptp->OS, ptp->OS->MaxSocketNumber + 1, ptp->LastSelectMask, clientsWritable, clientsException, tp);
    }
    else i = OSSelect (ptp->OS, ptp->OS->MaxSocketNumber + 1, ptp->LastSelectMask, NULL, clientsException, tp);

    if (i <= 0)
    {
        OSCLEARBITS(ptp->LastSelectMask);
        OSCLEARBITS(clientsException);
        OSCLEARBITS(clientsWritable);
        if (i < 0)
            if ((err = errno) == EBADF)
            {
                TPCloseDamagedConnections (ptp);
                if (!OSANYSET(ptp->AllClients))
                    return 0;
            }
            else
            {
                if ((err == ETIMEDOUT) || (err == EWOULDBLOCK))
                    i = 0;
                else
                {
                    TCPERROR (errno);
                    OSError ("TEBAS004", "socket error : %d", errno);
                    return -1;
                }
            }
    }
    OSMASKANDSETBITS(ptp->ConnectionPending, ptp->LastSelectMask,  ptp->WellKnownConnections);
    OSMASKANDSETBITS (ptp->InputPending, ptp->LastSelectMask, ptp->AllClients);

    while (OSANYSET(clientsException))
    {
        if ((givenclient = TPGetNextDescriptor (ptp, clientsException)) > 0)
        {
            if ((ptp->Clients[ptp->ConnectionTranslation[givenclient]])->Connect == FALSE)
                OSError ("TETCP024", NULL);
            TPCloseDownClient (ptp, ptp->Clients[ptp->ConnectionTranslation[givenclient]]);
        }
    }

    if (ptp->AnyClientsWriteBlocked && OSANYSET(clientsWritable))
    {
        /* il faut voir si c'est une connexion non bloquante */
        OSCOPYBITS(clientsWritable, clientsNotConnected);
        while (OSANYSET(clientsNotConnected))
        {
            if ((givenclient = TPGetNextDescriptor (ptp, clientsNotConnected)) > 0)
                if ((ptp->Clients[ptp->ConnectionTranslation[givenclient]])->Connect == FALSE)
                    TPConnectionOK (ptp, ptp->Clients[ptp->ConnectionTranslation[givenclient]]);
        }

        OSCOPYBITS(clientsWritable, ptp->OutputPending);
        OSUNSETBITS(ptp->ClientsWriteBlocked, clientsWritable);
        OSORBITS(ptp->ClientsWithOutput, ptp->ClientsWithOutput, clientsWritable);
        if (!OSANYSET(ptp->ClientsWriteBlocked))
            ptp->AnyClientsWriteBlocked = FALSE;
    }
    return i;
}

/* clients with output not send */

int TPFlushClient (TP* ptp, TPClient* pclient)
{
    if (pclient->ClientGone) return -1;
    return pclient->WriteFunction (ptp, pclient, NULL, 0);
}



BOOL TPExecuteApplicationProcedures (TP* ptp)
{

    List* plist;
    BOOL DontBlock;
    TPApplicationProcedure* AppProc;

    DontBlock = FALSE;
    plist = ptp->ApplicationProcedures;
    while (plist)
    {
        AppProc = (TPApplicationProcedure *)plist->car;
        if (AppProc->ExecuteProcedure(ptp, AppProc->ApplicationField) != 0)
            DontBlock = TRUE;
        plist = plist->cdr;
    }
    return DontBlock;
}



LONG TPDefaultSend (TP* ptp, TPClient* pclient, char* buf, int count)
{
    return 0;
}

LONG TPDefaultReceive (TP* ptp, TPClient* pclient, char* buf, int count)
{
    return 1;
}


int TPSendIfAnyOutput (TP* ptp, void* par)
{
    long ClientsWithOutput[MaskCount];
    TPClient* pclient;
    int TimeToYield = 5;
    LONG returnvalue = 0;

    if (TPClientsWithOutput(ptp))
    {
        OSCOPYBITS(ptp->ClientsWithOutput, ClientsWithOutput);
        while ((OSANYSET(ClientsWithOutput)) &&
                ((pclient = TPGetClient(ptp, TPGetNextDescriptor(ptp, ClientsWithOutput))) != NULL))
        {
            OSBITCLEAR(ptp->ClientsWithOutput, pclient->Pipe->fd);
            if ((returnvalue = TPDispatchOutput (ptp, pclient, TimeToYield)) < 0)
                if ((returnvalue != -3) && (!TPIsServer(ptp)) && (TPNoMoreClient(ptp))) break;
        }
    }

    if (TPClientsWithFileOutput(ptp))
    {
        OSCOPYBITS(ptp->ClientsWithFileOutput, ClientsWithOutput);
        while ((OSANYSET(ClientsWithOutput)) &&
                ((pclient = ptp->Clients[TPGetNextDescriptor(ptp, ClientsWithOutput) - 1]) != NULL))
        {
            OSBITCLEAR(ptp->ClientsWithFileOutput, pclient->Index + 1);
            if ((returnvalue = TPDispatchOutput (ptp, pclient, TimeToYield)) < 0)
                if ((returnvalue != -3) && (!TPIsServer(ptp)) && (TPNoMoreClient(ptp))) break;
        }
    }
    if (TPClientsWithBaseOutput(ptp))
    {
        OSCOPYBITS(ptp->ClientsWithBaseOutput, ClientsWithOutput);
        while ((OSANYSET(ClientsWithOutput)) &&
                ((pclient = ptp->Clients[TPGetNextDescriptor(ptp, ClientsWithOutput) - 1]) != NULL))
        {
            OSBITCLEAR(ptp->ClientsWithBaseOutput, pclient->Index + 1);
            if ((returnvalue = TPDispatchOutput (ptp, pclient, TimeToYield)) < 0)
                if ((returnvalue != -3) && (!TPIsServer(ptp)) && (TPNoMoreClient(ptp))) break;
        }
    }
    if ((returnvalue == -3) ||
            (TPClientsWithOutput (ptp)) ||
            (TPClientsWithFileOutput (ptp)) ||
            (TPClientsWithBaseOutput (ptp)))
        return 1;
    else
        return 0;
}


int TPRecvIfAnyInput (TP* ptp, void* par)
{
    long ClientsWithInput[MaskCount];
    TPClient* pclient;
    int TimeToYield = 5;
    LONG returnvalue = 0;

    if (TPClientsWithBaseInput(ptp))
    {
        OSCOPYBITS(ptp->ClientsWithBaseInput, ClientsWithInput);
        while ((OSANYSET(ClientsWithInput)) &&
                ((pclient = ptp->Clients[TPGetNextDescriptor(ptp, ClientsWithInput) - 1]) != NULL))
        {
            if ((returnvalue = TPDispatchInput (ptp, pclient, TimeToYield)) < 0)
                if ((returnvalue != -3) && (!TPIsServer(ptp)) && (TPNoMoreClient(ptp))) break;
        }
    }
    if ((returnvalue == -3) ||
            (TPClientsWithBaseInput (ptp)))
        return 1;
    else
        return 0;
}


/*
  return value < 0 :
    -1 : error in writing message but communication can persist 
    -2 : error in comm the communication is closed
  return value = 0 (evrything is written)
  return value > 0 (Ewouldblock)
*/

LONG TPFlushClientOutput (TP* ptp, TPClient* pclient)
{
    int ret = 0;
    LONG ret1 = 0;
    LONG written = 0;

    if (pclient->ClientGone) return 0;
    if (pclient->Pipe->fileid == NULL)
    {
        while (TPClientWithOutput(pclient))
        {
            OSBITCLEAR(ptp->ClientsWithOutput, pclient->Pipe->fd);
            if ((ret = pclient->SendFunction(ptp, pclient, pclient->TransBuffer, pclient->TransSize)) <= 0)
                break;
            if (pclient->ClientGone) return 0;
            ret1 = pclient->WriteFunction (ptp, pclient, pclient->TransBuffer, (int)ret);
            if (ret1 > 0)
                written += ret1;
            if ((ret1 < 0) || (ret1 < ret))
                break;
        }
    }
    else
    {
        while (TPClientWithFileOutput(pclient))
        {
            OSBITCLEAR(ptp->ClientsWithFileOutput, pclient->Index + 1);
            if ((ret = pclient->SendFunction(ptp, pclient, pclient->TransBuffer, pclient->TransSize)) <= 0)
                break;
            if (pclient->ClientGone) return 0;
            ret1 = pclient->WriteFunction (ptp, pclient, pclient->TransBuffer, (int)ret);
            if (ret1 > 0)
                written += ret1;
            if ((ret1 < 0) || (ret1 < ret))
                break;
        }

    }
    if (ret1 < 0)
    {
        if (!pclient->ClientGone) TPCloseDownClient (ptp, pclient);
        return -2;
    }

    if (ret < 0) return -1;
    if (ret == 0) return written;
    return written;
}

/*return value < 0 :
    -1 : error in message but comm can persist 
    -2 : error in comm we should close communication 
    -3 : there is blockinq write on com
  return value = 0 we can't write anymore on comm (either notihing to write or blocking mode)
  return value > 0 we can write but time yield */




LONG TPDispatchOutput (TP* ptp, TPClient* pclient, int TimeToYield)
{
    LONG  ret;
    LONG  ret1 = 0;
    int i;

    for (i = 0; i < TimeToYield; i++)
    {

        if ((ret = pclient->SendFunction(ptp, pclient, pclient->TransBuffer, pclient->TransSize)) <= 0)
            break;  /* if it returns 0 it means output is not set anymore in Dispatch */


        if (pclient->ClientGone) return 0;
        ret1 = pclient->WriteFunction (ptp, pclient, pclient->TransBuffer, (int)ret);
        if ((ret1 < 0) || (ret1 < ret))
            break;
    }
    if (ret1 < 0)
    {
        if (!pclient->ClientGone) TPCloseDownClient (ptp, pclient);
        return -2;
    }
    if (ret == -3) return -3;
    if (ret < 0) return -1;
    if ((ret == 0) || (ret1 < ret)) return 0;
    return ret;
}

/* return -2 if communication is to be closed */
/* returns -1 if an error occurs in message    */
/* returns > 0 if evrything is OK             */
/* returns 0 if stop reading */

LONG TPDispatchInput (TP* ptp, TPClient* pclient, int TimeToYield)
{
    int ret;
    LONG ret1;
    int i;


    for (i = 0; i < TimeToYield; i++)
    {
        ret = pclient->ReadFunction (ptp, pclient);
        if (ret <= 0)
            break;

        else ret1 = pclient->ReceiveFunction (ptp, pclient, pclient->RequestBuffer, ret);
        /* IF COM IS CLOSED BY A MESSAGE RECEPTION it closes the client OK...*/
        if (pclient->ClientGone) return 0;
    }
    if (ret < 0)
    {
        if (!pclient->ClientGone) TPCloseDownClient (ptp, pclient);
        return -2;
    }
    if (ret == 0) return ret;
    if (ret1 < 0) return -1;
    return ret1;
}

LONG TPDispatchEvents (TP* ptp, DWORD microseconds)
{
    TPClient* pclient;
    LONG ret = 0;
    BOOL Every = TRUE;
    int DontBlock;
    int connection;
    DWORD time = microseconds;
    int TimeToYield = 5;

    while (Every)
    {
        if (microseconds) Every = FALSE;

        DontBlock = TPExecuteApplicationProcedures (ptp);


        if ((!TPIsServer(ptp)) && (TPNoMoreClient(ptp)))
            break;

        if (!microseconds)
            if (DontBlock)  time = 1;                   /* 0 means blocking mode   */
            else            time = microseconds;       /* return to blocking mode;*/

        if (TPIsServer(ptp) || OSANYSET(ptp->AllClients))
        {
            if ((ret = TPWaitForSomething (ptp, time)) <= 0)
                if (((ret == 0) && (microseconds != 0)) ||
                        (ret < 0))
                    break;

            if (TPIsServer(ptp))
            {
                while (TPConnectionPending(ptp))
                {
                    connection = TPNextConnectionPending(ptp) ;
                    pclient = TPEstablishConnectionOn(ptp, connection) ;
                }
            }
        }
        while ((TPInputPending (ptp)) && ((pclient = TPNextInputPending (ptp)) != NULL))
        {
            if ((ret = TPDispatchInput (ptp, pclient, TimeToYield)) < 0)
                if ((!TPIsServer(ptp)) && (TPNoMoreClient(ptp)))
                    break;
        }
        while ((TPClientsWithInput(ptp)) && ((pclient = TPNextClientWithInput(ptp)) != NULL))
        {
            if ((ret = TPDispatchInput (ptp, pclient, TimeToYield)) < 0)
                if ((!TPIsServer(ptp)) && (TPNoMoreClient(ptp)))
                    break;
        }
        while ((TPOutputPending(ptp)) && ((pclient = TPNextOutputPending(ptp)) != NULL))
        {
            if ((ret = TPFlushClient(ptp, pclient)) < 0)
                TPCloseDownClient (ptp, pclient);
            if ((!TPIsServer(ptp)) && (TPNoMoreClient(ptp)))
                break;
        }
        if ((!TPIsServer(ptp)) && (TPNoMoreClient(ptp)))
            break;
    }
    return ret;
}


int TPWaitForConnections (TP* ptp)
{
    TPClient* pclient;
    int connection;
    int ret;

    if (TPIsServer(ptp) || OSANYSET(ptp->AllClients))
    {
        if ((ret = TPWaitForSomething (ptp, 0)) <= 0)
            if (ret < 0)
                return -1;

        if (TPIsServer(ptp))
        {
            while (TPConnectionPending(ptp))
            {
                connection = TPNextConnectionPending(ptp) ;
                pclient = TPEstablishConnectionOn(ptp, connection) ;
            }
        }
    }
    return 1;
}


/* take the first descritor in a mask */
int TPGetNextDescriptor (TP* ptp, long* mask)
{
    int i, curclient;
    for (i = 0; i < MaskCount; i++)
    {
        if (mask[i])
        {
            curclient = OSFFS (&mask[i]);
            mask[i] &= ~(1L << curclient);
            curclient += (i << 5);
            return curclient;
        }
    }
    return 0;
}

int TPNextConnectionPending (TP* ptp)
{
    int listener,i;

    for (i = 0; i < MaskCount; i++)
    {
        if (ptp->ConnectionPending[i])
        {
            listener = OSFFS (&ptp->ConnectionPending[i]);
            ptp->ConnectionPending[i] &= ~(1L << listener);
            listener += (i << 5);
            return listener;
        }
    }
    return 0;
}


void TPCloseListeningConnections (TP* ptp)
{
    int curcon,i;

    for (i = 0; i < MaskCount; i++)
    {
        if (ptp->WellKnownConnections[i])
        {
            curcon = OSFFS (&ptp->WellKnownConnections[i]);
            ptp->WellKnownConnections[i] &= ~(1L << curcon);
            ptp->AllSockets[i] &= ~(1L << curcon);
            OSCloseSocket (ptp->OS, curcon);
        }
    }
}


void TPEnd (TP* ptp)
{
    TPPort* pport;
    TPApplicationProcedure* pappprocedure;
    TPConnectionProcedure* pexecuteprocedure;
    TPEndingProcedure* pendingprocedure;

    TPKillAllClients (ptp);
    TPCloseListeningConnections (ptp);

    while (ptp->History)
    {

        TPClient* pclient = (TPClient *) ptp->History->car;
        ListRemove (&ptp->History, pclient);
        while (pclient->EndingProcedures)
        {
            pendingprocedure = (TPEndingProcedure*)pclient->EndingProcedures->car;
            ListRemove (&pclient->EndingProcedures, pendingprocedure);
            free (pendingprocedure);
        }
        free (pclient);
    }

    while (ptp->ListeningPorts)
    {
        pport = (TPPort *)ptp->ListeningPorts->car;
        ListRemove (&ptp->ListeningPorts, pport);
        free (pport);
    }

    while (ptp->ApplicationProcedures)
    {
        pappprocedure = (TPApplicationProcedure *)ptp->ApplicationProcedures->car;
        ListRemove (&ptp->ApplicationProcedures, pappprocedure);
        free (pappprocedure);
    }

    while (ptp->ConnectionProcedures)
    {
        pexecuteprocedure = (TPConnectionProcedure *)ptp->ConnectionProcedures->car;
        ListRemove (&ptp->ConnectionProcedures, pexecuteprocedure);
        free (pexecuteprocedure);
    }

    XFEnd (ptp->XF);
    free ((BYTE*)ptp->XF);
    OSEnd (ptp->OS);
    free ((BYTE*)ptp->OS);
}


TPBuf* TPAllocBuf (char* buf, int count)
{
    TPBuf* receivebuffer = (TPBuf *)malloc (sizeof (TPBuf));
    receivebuffer->Buf = (char*)malloc (count);
    receivebuffer->Count= count;
    memcpy (receivebuffer->Buf, buf, count);
    return receivebuffer;
}

int TPCloseJoinClients (TPClient* pclient, void* app)
{
    LONG ret;
    LONG ret1;
    TP* ptp = (TP*)app;

    TPClient* pclient1 = (TPClient*)pclient->PointerField;


    if (!pclient1) return 1;

    while (pclient1->UserField)
    {
        ret = TPSendBuf (ptp, pclient1, pclient1->TransBuffer, pclient1->TransSize);
        ret1 = TPWrite (ptp, pclient1,  pclient1->TransBuffer  ,ret);
        if (ret1 != ret)
            printf ( "IIIIIIIIII`\n");
    }
    pclient1->PointerField = NULL;
    TPCloseDownClient (ptp, pclient1);
    return 1;
}




int TPJoinClients (TP* ptp, TPClient* pclient, TPClient* pclient1)
{
    if ((!pclient) || (!pclient1)) return -1;

    TPSetClientProtocol (ptp, pclient, IPPROTO_TCP)      ;
    TPSetReceiveFunction (ptp, pclient,  TPReceiveBuf);
    TPSetSendFunction (ptp, pclient,     TPSendBuf);

    TPSetPointerField (pclient, pclient1);

    TPSetClientProtocol (ptp, pclient1, IPPROTO_TCP)      ;
    TPSetReceiveFunction (ptp, pclient1, TPReceiveBuf);
    TPSetSendFunction (ptp, pclient1,    TPSendBuf);
    TPSetPointerField (pclient1, pclient);
    TPAddEndingProcedure (pclient, TPCloseJoinClients , ptp);
    TPAddEndingProcedure (pclient1,TPCloseJoinClients, ptp);
    return 1;
}


LONG TPReceiveBuf (TP* ptp, TPClient* pclient, char* buf, int count)
{
    TPBuf* receivebuffer;
    TPClient* pclient1 = (TPClient*)pclient->PointerField;

    receivebuffer = TPAllocBuf (buf, count);
    ListNewr (&pclient1->UserField, receivebuffer);

    return 1;
}


LONG TPSendBuf (TP* ptp, TPClient* pclient, char* buf, int count)
{

    TPBuf* receivebuffer;
    TPClient* pclient1 = (TPClient*)pclient->PointerField;
    int Count;
    if (pclient->UserField)
    {
        receivebuffer = (TPBuf *)pclient->UserField->car;
        memcpy (buf, receivebuffer->Buf, receivebuffer->Count);
        ListRemove (&pclient->UserField, receivebuffer);
        Count = receivebuffer->Count;
        free (receivebuffer->Buf);
        free (receivebuffer);
        return Count;
    }
    return 0;
}


int TPLoadResourceFile (TP *ptp, char *Filename)
{
    GlobalOne *Glob;
    XlatStruct *pXlat;

    List* AllXlatStruct;

    if (!OSInitResourceFile(Filename))
    {
        OSError("TEOS0001LEX", OSStrErr);
        return 0;
    }

    Glob = (GlobalOne *)zzparse(ptp->XF);
    if (!Glob)
    {
        OSError("TEOS0001LEX", OSStrErr);
        OSEndResourceFile();
        return 0;
    }

    AllXlatStruct = Glob->AllXlatStruct;
    while(AllXlatStruct)
    {
        pXlat = (XlatStruct *)AllXlatStruct->car;
        XFLoadSystemTable(ptp->XF, (XFTable *)pXlat);
        AllXlatStruct = AllXlatStruct->cdr;
    }

    FreeGlobalOneYaccStruct(Glob);
    OSEndResourceFile();
    return 1;
}

/* TP INPUT OUTPUT */

/* if = 0 request is not complete */
/* if > 0 */
/*     --returns the length of the request , if another request */
/*     is also read ClientsWithInput is set :this means that select can */
/*     block for the next call so we should treat these clients before */
/*     going in TPWaitForSomething !!!*/
/* if < 0 we should CloseDownClient */

int TPReadRequestFromClient (TP* ptp, TPClient* pclient)
{
    PIPE* pcom;
    CHANNEL* pinput;
    TPRequest* prequest;
    int gotnow, result;
    int needed;
    pcom = pclient->Pipe;
    pinput = pcom->Input;
    OSBITCLEAR(ptp->InputPending, pcom->fd);
    if (!pinput)
    {
        pinput = OSAllocateChannel ();
        if (!pinput)
            return -1;
        pcom->Input = pinput;
    }
    pinput->BufPtr += pinput->LenLastReq;  /* begin of request */
    prequest = (TPRequest*)pinput->BufPtr;

    /* gotnow = what is already been read of this request */

    gotnow = pinput->BufCnt + (int) (pinput->Buffer - pinput->BufPtr);
    if ((gotnow < sizeof (TPRequest)) ||
            (gotnow < (needed = TPGetRequestLength ((char*)prequest))))
    {
        pinput->LenLastReq = 0;
        if ((gotnow < sizeof (TPRequest)) || (needed == 0))
            needed = sizeof (TPRequest);  /* needed can be = 0 ? */
        else if (needed > MAXBUFSIZE)
        {
            OSError ("TEBAS007", NULL);
            return -1;
        }
        if ((gotnow == 0) ||
                ((pinput->BufPtr - pinput->Buffer + needed) > pinput->Size))
        {
            if ((gotnow > 0) && (pinput->BufPtr != pinput->Buffer))
                memcpy (pinput->Buffer, pinput->BufPtr, gotnow);
            if (needed > pinput->Size)
            {
                char* ibuf;
                ibuf = (char*)realloc(pinput->Buffer, needed);
                if (!ibuf)
                {
                    OSError ("TEBAS008", NULL);
                    return -1;
                }
                pinput->Size = needed;
                pinput->Buffer = ibuf;
            }
            pinput->BufPtr = pinput->Buffer;
            pinput->BufCnt = gotnow;
        }
        /* ready to read */
        result = OSRead (ptp->OS, pcom->fd, pinput->Buffer + pinput->BufCnt,
                         pinput->Size - pinput->BufCnt);

        if (result <= 0)
        {
            if ((result < 0) && (errno == EWOULDBLOCK))
            {
                /* no input for this client */
                OSBITCLEAR(ptp->ClientsWithInput, pcom->fd);
                return 0;
            }
            TCPERROR (errno);
            return -1;
        }
        pclient->ReceiptSize += result;
        pinput->BufCnt += result;
        gotnow += result;
        /* free space after huge request */
        if ((pinput->Size > BUFWATERMARK) &&
                (pinput->BufCnt < BUFSIZE) && (needed < BUFSIZE))
        {
            char* ibuf;
            ibuf = (char*)realloc (pinput->Buffer, BUFSIZE);
            if (ibuf)
            {
                pinput->Size = BUFSIZE;
                pinput->Buffer = ibuf;
                pinput->BufPtr = ibuf + pinput->BufCnt - gotnow;
            }
        }
        /* request can be completely read if client sends it complete */
        prequest = (TPRequest*)pinput->BufPtr;
        if ((gotnow < sizeof (TPRequest)) ||
                (gotnow < (needed = TPGetRequestLength ((char*)prequest))))
        {
            OSBITCLEAR(ptp->ClientsWithInput, pcom->fd);
            return 0;
        }
    }
    if (needed == 0)  needed = sizeof (TPRequest);
    /* at least one request has been read */
    pinput->LenLastReq = needed;

    if (gotnow >= needed + (int)sizeof (TPRequest))
    {
        prequest = (TPRequest*)(pinput->BufPtr + needed);
        if (gotnow >= needed + TPGetRequestLength ((char*)prequest))
            /* still a request in the buffer */
            OSBITSET(ptp->ClientsWithInput, pcom->fd);
        else
            OSBITCLEAR(ptp->ClientsWithInput, pcom->fd);
    }
    else
    {
        if (gotnow == needed)
            OSBITCLEAR(ptp->ClientsWithInput, pcom->fd);
    }
    pclient->RequestBuffer = (char*)pinput->BufPtr;
    pclient->RequestSize = needed;
    return (int)needed;
}



int TPWriteRequestToClient (TP* ptp, TPClient* pclient, char* buf, int Count)
{
    PIPE* pcom;
    CHANNEL* poutput;
    int result, gotnow, needed, written, count, towrite;
    pcom = pclient->Pipe;
    poutput = pcom->Output;
    written = 0;

    if (Count == 0)
        count = 0;
    else count = Count + sizeof (TPRequest);
    if (!poutput)
    {
        poutput = OSAllocateChannel ();
        if (!poutput)
            return -1;
        pcom->Output = poutput;
    }
    gotnow = poutput->BufCnt + (int)(poutput->Buffer - poutput->BufPtr);
    needed = count + gotnow;   /* 2 means request word for size */


    if ((gotnow != 0) && (poutput->Buffer != poutput->BufPtr))
        memcpy (poutput->Buffer, poutput->BufPtr, gotnow);


    if (needed > poutput->Size)
    {
        char* obuf;
        obuf = (char*)realloc (poutput->Buffer, needed);
        if (!obuf)
        {
            OSError ("TEBAS009", NULL);
            return -1;
        }
        poutput->Buffer = obuf;
        poutput->Size = needed;
    }
    poutput->BufPtr = poutput->Buffer;
    poutput->BufCnt = gotnow;

    if (count)
    {
        memcpy (poutput->Buffer + poutput->BufCnt + 2 , buf, count - 2);
        TPSetRequestLength (poutput->Buffer + poutput->BufCnt, (WORD)count);
    }

    poutput->BufCnt += count;
    gotnow += count;
    towrite = gotnow;
    while (written < gotnow)
    {
        result = OSWrite (ptp->OS, pcom->fd, poutput->BufPtr, towrite);

        if (result < 0)
        {
            if (errno == EWOULDBLOCK)
            {
                OSBITSET(ptp->ClientsWriteBlocked, pcom->fd);
                OSBITCLEAR(ptp->ClientsWithOutput, pcom->fd);
                ptp->AnyClientsWriteBlocked = TRUE;
                return written;
            }
            else
            {
                TCPERROR (errno);
                return -1;
            }
        }
        else
        {
            pclient->TransferSize += result;
            written += result;
            poutput->BufPtr += result;
            towrite -= result;
        }
    }

    if (ptp->AnyClientsWriteBlocked)         /* maybe no use ! */
    {
        OSBITCLEAR(ptp->ClientsWriteBlocked, pcom->fd);
        if (!OSANYSET(ptp->ClientsWriteBlocked))
            ptp->AnyClientsWriteBlocked = FALSE;
    }
    OSBITSET(ptp->ClientsWithOutput, pcom->fd);
    if (poutput->Size > BUFSIZE)
    {
        char* obuf;
        obuf = (char*)realloc (poutput->Buffer, BUFSIZE);
        if (obuf)
        {
            poutput->Size = BUFSIZE;
            poutput->Buffer = obuf;
            poutput->BufCnt = 0;
            poutput->BufPtr = obuf;
        }
    }
    return written;
}






/* blocking mode */


int TPRead (TP* ptp, TPClient* pclient)
{
    int ret;

    TPSetBlockingMode (pclient, TRUE);
    ret  = pclient->ReadFunction(ptp, pclient);
    TPSetBlockingMode (pclient, FALSE);
    if (ret < 0)
    {
        TCPERROR (errno);
        return -1;
    }
    return ret;
}



int TPWrite (TP* ptp, TPClient* pclient, char* buffer , int size)
{
    int ret;
    TPSetBlockingMode (pclient, TRUE);
    ret  = pclient->WriteFunction (ptp, pclient, buffer, size);
    TPSetBlockingMode (pclient, FALSE);
    if (ret < 0)
    {
        TCPERROR (errno);
        return -1;
    }
    else return ret;
}


/*--------------------------------------------------------------*/
/* RAW INPUT OUTPUT */
/*--------------------------------------------------------------*/


int TPReadIpFromClient (TP* ptp, TPClient* pclient)
{
    char AddrName[100];
    PIPE* pcom;
    CHANNEL* pinput;
    char* prequest;
    int gotnow, result;
    int needed;
    pcom = pclient->Pipe;
    pinput = pcom->Input;
    OSBITCLEAR(ptp->InputPending, pcom->fd);
    if (!pinput)
    {
        pinput = OSAllocateChannel ();
        if (!pinput)
            return -1;
        pcom->Input = pinput;
    }
    pinput->BufPtr += pinput->LenLastReq;  /* begin of IP */
    prequest = pinput->BufPtr;

    /* gotnow = what is already been read of this IP */

    gotnow = pinput->BufCnt + (int) (pinput->Buffer - pinput->BufPtr);
    if ((gotnow < sizeof (DWORD)) ||   /* ip header length*/
            (gotnow < (needed = TPGetIpLength (prequest))))
    {
        pinput->LenLastReq = 0;
        if ((gotnow < sizeof (DWORD)) || (needed == 0))
            needed = sizeof (DWORD);  /* needed can be = 0 ? */
        else if (needed > MAXBUFSIZE)
        {
            OSError ("TEBAS007", NULL);
            return -1;
        }
        if ((gotnow == 0) ||
                ((pinput->BufPtr - pinput->Buffer + needed) > pinput->Size))
        {
            if ((gotnow > 0) && (pinput->BufPtr != pinput->Buffer))
                memcpy (pinput->Buffer, pinput->BufPtr, gotnow);
            if (needed > pinput->Size)
            {
                char* ibuf;
                ibuf = (char*)realloc(pinput->Buffer, needed);
                if (!ibuf)
                {
                    OSError ("TEBAS008", NULL);
                    return -1;
                }
                pinput->Size = needed;
                pinput->Buffer = ibuf;
            }
            pinput->BufPtr = pinput->Buffer;
            pinput->BufCnt = gotnow;
        }
        /* ready to read */
        result = OSRecv (ptp->OS, pcom->fd, pinput->Buffer + pinput->BufCnt,
                         pinput->Size - pinput->BufCnt, AddrName);

        if (result <= 0)
        {
            if ((result < 0) && (errno == EWOULDBLOCK))
            {
                /* no input for this client */
                OSBITCLEAR(ptp->ClientsWithInput, pcom->fd);
                return 0;
            }
            TCPERROR (errno);
            return -1;
        }
        strcpy (pclient->WithUserId, AddrName);
        //OSGetNameFromAddr (pclient->WithUserId, pclient->WithName);
        pclient->ReceiptSize += result;
        pinput->BufCnt += result;
        gotnow += result;
        /* free space after huge request */
        if ((pinput->Size > BUFWATERMARK) &&
                (pinput->BufCnt < BUFSIZE) && (needed < BUFSIZE))
        {
            char* ibuf;
            ibuf = (char*)realloc (pinput->Buffer, BUFSIZE);
            if (ibuf)
            {
                pinput->Size = BUFSIZE;
                pinput->Buffer = ibuf;
                pinput->BufPtr = ibuf + pinput->BufCnt - gotnow;
            }
        }
        /* request can be completely read if client sends it complete */
        prequest = pinput->BufPtr;
        if ((gotnow < sizeof (DWORD)) ||
                (gotnow < (needed = TPGetIpLength (prequest))))
        {
            OSBITCLEAR(ptp->ClientsWithInput, pcom->fd);
            return 0;
        }
    }
    if (needed == 0)  needed =sizeof  (DWORD);
    /* at least one IP has been read */
    pinput->LenLastReq = needed;

    if (gotnow >= needed + (int)sizeof (DWORD))
    {
        prequest = (char*)(pinput->BufPtr + needed);
        if (gotnow >= needed + TPGetIpLength (prequest))
            /* still an IP  in the buffer */
            OSBITSET(ptp->ClientsWithInput, pcom->fd);
        else
            OSBITCLEAR(ptp->ClientsWithInput, pcom->fd);
    }
    else
    {
        if (gotnow == needed)
            OSBITCLEAR(ptp->ClientsWithInput, pcom->fd);
    }
    pclient->RequestBuffer = (char*)pinput->BufPtr;
    return (int)needed;
}


int TPWriteIpToClient (TP* ptp, TPClient* pclient, char* buf, int Count)
{

    PIPE* pcom;
    CHANNEL* poutput;
    int result, gotnow, needed, written, towrite;
    pcom = pclient->Pipe;
    poutput = pcom->Output;
    written = 0;

    if (!poutput)
    {
        poutput = OSAllocateChannel ();
        if (!poutput)
            return -1;
        pcom->Output = poutput;
    }
    gotnow = poutput->BufCnt + (int)(poutput->Buffer - poutput->BufPtr);
    needed = Count + gotnow;   /* 2 means request word for size */


    if ((gotnow != 0) && (poutput->Buffer != poutput->BufPtr))
        memcpy (poutput->Buffer, poutput->BufPtr, gotnow);


    if (needed > poutput->Size)
    {
        char* obuf;
        obuf = (char*)realloc (poutput->Buffer, needed);
        if (!obuf)
        {
            OSError ("TEBAS009", NULL);
            return -1;
        }
        poutput->Buffer = obuf;
        poutput->Size = needed;
    }
    poutput->BufPtr = poutput->Buffer;
    poutput->BufCnt = gotnow;

    if (Count)
    {
        memcpy (poutput->Buffer + poutput->BufCnt , buf, Count);
        TPSetIpLength (poutput->Buffer + poutput->BufCnt, (WORD)Count);
    }


    poutput->BufCnt += Count;
    gotnow += Count;
    towrite = gotnow;
    while (written < gotnow)
    {
        result = OSSend (ptp->OS, pcom->fd, poutput->BufPtr, towrite, pclient->WithUserId);

        if (result < 0)
        {
            if (errno == EWOULDBLOCK)
            {
                OSBITSET(ptp->ClientsWriteBlocked, pcom->fd);
                OSBITCLEAR(ptp->ClientsWithOutput, pcom->fd);
                ptp->AnyClientsWriteBlocked = TRUE;
                return written;
            }
            else
            {
                TCPERROR (errno);
                return -1;
            }
        }
        else
        {

            pclient->TransferSize += result;
            written += result;
            poutput->BufPtr += result;
            towrite -= result;
        }
    }

    if (ptp->AnyClientsWriteBlocked)         /* maybe no use ! */
    {
        OSBITCLEAR(ptp->ClientsWriteBlocked, pcom->fd);
        if (!OSANYSET(ptp->ClientsWriteBlocked))
            ptp->AnyClientsWriteBlocked = FALSE;
    }
    OSBITSET(ptp->ClientsWithOutput, pcom->fd);
    if (poutput->Size > BUFSIZE)
    {
        char* obuf;
        obuf = (char*)realloc (poutput->Buffer, BUFSIZE);
        if (obuf)
        {
            poutput->Size = BUFSIZE;
            poutput->Buffer = obuf;
            poutput->BufCnt = 0;
            poutput->BufPtr = obuf;
        }
    }
    return written;
}

/* the icmp  is encapsulated in an IP. This is done by the socket layer ,
    we assume that the buffer read begins with an IP header */


int TPReadIcmpFromClient (TP* ptp, TPClient* pclient)
{
    return TPReadIpFromClient (ptp, pclient);
}

/*  the icmp  is encapsulated in an IP. This is done by the socket layer ,
    we assume that the buffer begins with an icmp header  
    the checksum is done here */


int TPWriteIcmpToClient (TP* ptp, TPClient* pclient, char* buf, int Count)
{
    PIPE* pcom;
    CHANNEL* poutput;
    int result, gotnow, needed, written, towrite;
    pcom = pclient->Pipe;
    poutput = pcom->Output;
    written = 0;

    if (!poutput)
    {
        poutput = OSAllocateChannel ();
        if (!poutput)
            return -1;
        pcom->Output = poutput;
    }
    gotnow = poutput->BufCnt + (int)(poutput->Buffer - poutput->BufPtr);
    needed = Count + gotnow;   /* 2 means request word for size */


    if ((gotnow != 0) && (poutput->Buffer != poutput->BufPtr))
        memcpy (poutput->Buffer, poutput->BufPtr, gotnow);


    if (needed > poutput->Size)
    {
        char* obuf;
        obuf = (char*)realloc (poutput->Buffer, needed);
        if (!obuf)
        {
            OSError ("TEBAS009", NULL);
            return -1;
        }
        poutput->Buffer = obuf;
        poutput->Size = needed;
    }
    poutput->BufPtr = poutput->Buffer;
    poutput->BufCnt = gotnow;

    if (Count)
    {
        memcpy (poutput->Buffer + poutput->BufCnt , buf, Count);
        TPSetIcmpLength (poutput->Buffer + poutput->BufCnt, (WORD)Count);
    }


    poutput->BufCnt += Count;
    gotnow += Count;
    towrite = gotnow;
    while (written < gotnow)
    {
        result = OSSend (ptp->OS, pcom->fd, poutput->BufPtr, towrite, pclient->WithUserId);

        if (result < 0)
        {
            if (errno == EWOULDBLOCK)
            {
                OSBITSET(ptp->ClientsWriteBlocked, pcom->fd);
                OSBITCLEAR(ptp->ClientsWithOutput, pcom->fd);
                ptp->AnyClientsWriteBlocked = TRUE;
                return written;
            }
            else
            {
                TCPERROR (errno);
                return -1;
            }
        }
        else
        {

            pclient->TransferSize += result;
            written += result;
            poutput->BufPtr += result;
            towrite -= result;
        }
    }

    if (ptp->AnyClientsWriteBlocked)         /* maybe no use ! */
    {
        OSBITCLEAR(ptp->ClientsWriteBlocked, pcom->fd);
        if (!OSANYSET(ptp->ClientsWriteBlocked))
            ptp->AnyClientsWriteBlocked = FALSE;
    }
    OSBITSET(ptp->ClientsWithOutput, pcom->fd);
    if (poutput->Size > BUFSIZE)
    {
        char* obuf;
        obuf = (char*)realloc (poutput->Buffer, BUFSIZE);
        if (obuf)
        {
            poutput->Size = BUFSIZE;
            poutput->Buffer = obuf;
            poutput->BufCnt = 0;
            poutput->BufPtr = obuf;
        }
    }
    return written;
}



int TPReadUdpFromClient (TP* ptp, TPClient* pclient)
{
    return 1;
}

int TPWriteUdpToClient (TP* ptp, TPClient* pclient, char* buf, int Count)
{
    return 1;
}




int TPReadTcpFromClient (TP* ptp, TPClient* pclient)
{
    CHANNEL* pinput;
    PIPE* pcom;
    int result, gotnow;
    pcom = pclient->Pipe;
    pinput = pcom->Input;

    OSBITCLEAR(ptp->InputPending, pcom->fd);


    if (!pinput)
    {
        pinput = OSAllocateChannel ();
        if (!pinput)
            return -1;
        pcom->Input = pinput;
    }

    pinput->BufPtr += pinput->LenLastReq;  /* begin of request */
    gotnow = pinput->BufCnt + (int) (pinput->Buffer - pinput->BufPtr);

    if (gotnow > 0)  result = gotnow;
    else
    {
        pinput->BufPtr = pinput->Buffer;
        pinput->BufCnt = 0;
        result = OSRead (ptp->OS, pcom->fd, pinput->Buffer,  pinput->Size);

        if (result <= 0)
        {
            if ((result < 0) && (errno == EWOULDBLOCK))
            {
                /* no input for this client */
                OSBITCLEAR(ptp->ClientsWithInput, pcom->fd);
                return 0;
            }
            TCPERROR (errno);
            return -1;
        }
    }
    pclient->ReceiptSize += result;

    pinput->BufCnt = result;

    pinput->LenLastReq = result;

    OSBITCLEAR(ptp->ClientsWithInput, pcom->fd);

    pclient->RequestBuffer = (char*)pinput->BufPtr;
    pclient->RequestSize = result;
    return (int)result;
}




int TPWriteTcpToClient (TP* ptp, TPClient* pclient, char* buf, int Count)
{

    PIPE* pcom;
    CHANNEL* poutput;
    int result, gotnow, needed, written, count, towrite;
    pcom = pclient->Pipe;
    poutput = pcom->Output;
    written = 0;

    if (Count == 0)  count = 0;
    else count = Count;
    if (!poutput)
    {
        poutput = OSAllocateChannel ();
        if (!poutput)
            return -1;
        pcom->Output = poutput;
    }
    gotnow = poutput->BufCnt + (int)(poutput->Buffer - poutput->BufPtr);
    needed = count + gotnow;   /* 2 means request word for size */


    if ((gotnow != 0) && (poutput->Buffer != poutput->BufPtr))
        memcpy (poutput->Buffer, poutput->BufPtr, gotnow);


    if (needed > poutput->Size)
    {
        char* obuf;
        obuf = (char*)realloc (poutput->Buffer, needed);
        if (!obuf)
        {
            OSError ("TEBAS009", NULL);
            return -1;
        }
        poutput->Buffer = obuf;
        poutput->Size = needed;
    }

    poutput->BufPtr = poutput->Buffer;
    poutput->BufCnt = gotnow;

    if (count)
    {
        memcpy (poutput->Buffer + poutput->BufCnt , buf, count);
    }

    poutput->BufCnt += count;
    gotnow += count;
    towrite = gotnow;
    while (written < gotnow)
    {
        result = OSWrite (ptp->OS, pcom->fd, poutput->BufPtr, towrite);

        if (result < 0)
        {
            if (errno == EWOULDBLOCK)
            {
                OSBITSET(ptp->ClientsWriteBlocked, pcom->fd);
                OSBITCLEAR(ptp->ClientsWithOutput, pcom->fd);
                ptp->AnyClientsWriteBlocked = TRUE;
                return written;
            }
            else
            {
                TCPERROR (errno);
                return -1;
            }
        }
        else
        {
            pclient->TransferSize += result;
            written += result;
            poutput->BufPtr += result;
            towrite -= result;
        }
    }

    if (ptp->AnyClientsWriteBlocked)         /* maybe no use ! */
    {
        OSBITCLEAR(ptp->ClientsWriteBlocked, pcom->fd);
        if (!OSANYSET(ptp->ClientsWriteBlocked))
            ptp->AnyClientsWriteBlocked = FALSE;
    }
    OSBITSET(ptp->ClientsWithOutput, pcom->fd);
    if (poutput->Size > BUFSIZE)
    {
        char* obuf;
        obuf = (char*)realloc (poutput->Buffer, BUFSIZE);
        if (obuf)
        {
            poutput->Size = BUFSIZE;
            poutput->Buffer = obuf;
            poutput->BufCnt = 0;
            poutput->BufPtr = obuf;
        }
    }
    return written;

}


/*---------------------------------------------------------*/
/* related to TP                                           */
/* always in little endian in BC stream                    */
/*---------------------------------------------------------*/


WORD TPGetRequestLength (char* buffer)
{

#ifdef LOW_HIGH
    return (*(WORD*)buffer);
#else
    {
        char buf[2];
        buf[0] = *(buffer + 1);
        buf[1] = *buffer;
        return (*(WORD*)buf);
    }
#endif
}

void TPSetRequestLength (char* buffer, WORD size)
{
#ifdef LOW_HIGH
    OSCopy2 (buffer, (char*)&size);
#else
    {
        char* buf = (char *)&size;
        buffer[0] = buf[1];
        buffer[1] = buf[0];
    }
#endif
}

/*---------------------------------------------------------*/
/* related to IP                                           */
/* always in big endian in the stream                      */
/* UNIX the length is without the header length !!!....    */
/*---------------------------------------------------------*/

WORD TPGetIpLength (char* buffer)
{
    WORD size;
    OSCopy2 (&size, (buffer + 2));
 #ifdef UNIX
    return (htons(size) + TPGetIpHeaderLength (buffer));
 #else
    return htons(size);
 #endif
}

WORD TPGetIpHeaderLength (char* buffer)
{
    WORD HeaderSize=(BYTE)buffer[0] & 15;
    HeaderSize*=4;
    return HeaderSize;
}


void TPSetIpLength (char* buffer, WORD size)
{
    WORD chk_sum = OSCheckSum ((WORD *) buffer, (int)size);
    (*(WORD*)(buffer + 2))  = chk_sum;
}

void TPSetIpHeaderLength (char* buffer, WORD headersize)
{

    /* version is always 4 */
#ifdef LOW_HIGH
struct iph {BYTE ip_hl:4, ip_v:4;};
#else
struct iph {BYTE ip_v:4,  ip_hl:4;};
#endif
    struct iph ip;
    ip.ip_v = 4;
    ip.ip_hl = (headersize >> 2);
    buffer[0] = *((BYTE *)&ip);
}


/*---------------------------------------------------------*/
/* related to ICMP                                         */
/* the ICMP length is the IP length minus the              */
/* IP Header length                                        */
/*---------------------------------------------------------*/


WORD TPGetIcmpLength (char* buffer)
{
    return (TPGetIpLength (buffer) - TPGetIpHeaderLength (buffer));
}

/* only thing to do is to put the correct checksum */

void TPSetIcmpLength (char* buffer, WORD size)
{
    WORD chk_sum = OSCheckSum ((WORD *) buffer, (int)size);
    (*(WORD*)(buffer + 2))  = chk_sum;
}





BYTE TPGetIpProtocol (char* buffer)
{
    return ((BYTE)(*(buffer + 9)));
}

void TPSetIpProtocol (char* buffer, BYTE protocol)
{
    buffer[10] = protocol;
}


/*return always in HIGH_LOW , existing functionalities
  do the translations*/
DWORD TPGetIpSourceAddress (char* buffer)
{
    DWORD IpAdress;
    OSCopy4(&IpAdress, (buffer + 12));
    return IpAdress;
}

DWORD TPGetIpDestinationAddress (char* buffer)
{
    DWORD IpAdress;
    OSCopy4(&IpAdress, (buffer + 16));
    return IpAdress;

}

void TPSetIpSourceAddress (char* buffer, DWORD Address)
{
    OSCopy4((buffer + 12), (char*)&Address);
}

void TPSetIpDestinationAddress (char* buffer, DWORD Address)
{
    OSCopy4((buffer + 16), (char*)&Address);
}


WORD TPGetTcpSourcePort (char* buffer)
{
    WORD SourcePort;
    OSCopy2 (&SourcePort, buffer);
    return htons(SourcePort);
}

void TPSetTcpSourcePort (char* buffer, WORD SourcePort)
{
    WORD net_SourcePort = htons (SourcePort);
    OSCopy2(buffer, (char*)&net_SourcePort);
}


WORD TPGetTcpDestinationPort (char* buffer)
{
    WORD SourcePort;
    OSCopy2 (&SourcePort, buffer + 2);
    return htons(SourcePort);
}

void TPSetTcpDestinationPort (char* buffer, WORD DestPort)
{
    WORD net_DestPort = htons (DestPort);
    OSCopy2((buffer + 2), (char*)&net_DestPort);
}




int TPAddApplicationProcedure (TP* ptp, int (*funct)(TP*, void*), void* applicationfield)
{
    TPApplicationProcedure* executeprocedure;

    executeprocedure = (TPApplicationProcedure *) malloc (sizeof (TPApplicationProcedure));
    if (!executeprocedure)   return -1;
    executeprocedure->ApplicationField = applicationfield;
    executeprocedure->ExecuteProcedure = funct;
    ListInsert (&ptp->ApplicationProcedures, executeprocedure);
    return 0;
}


int TPRemoveApplicationProcedure (TP* ptp, int (*funct)(TP*, void*), void* applicationfield)
{
    TPApplicationProcedure* executeprocedure;

    if ((executeprocedure = TPFindApplicationProcedure (ptp->ApplicationProcedures, funct)) == NULL)
        return -1;
    ListRemove (&ptp->ApplicationProcedures, executeprocedure);
    free (executeprocedure);
    return 1;
}



TPApplicationProcedure* TPFindApplicationProcedure (List* ExecuteList, int (*funct)(TP*, void*))
{
    int found = 0;
    List* plist = ExecuteList;
    TPApplicationProcedure* pexecuteprocedure = NULL;


    while ((found == 0) && (plist != NULL))
    {
        pexecuteprocedure = (TPApplicationProcedure*)plist->car;

        if (pexecuteprocedure->ExecuteProcedure == funct)   found = 1;
        else plist = plist->cdr;
    }

    return pexecuteprocedure;
}



int TPAddConnectionProcedure (TP* ptp, int (*funct)(TPClient*, void*), void* applicationfield)
{
    TPConnectionProcedure* executeprocedure;

    executeprocedure = (TPConnectionProcedure *) malloc (sizeof (TPConnectionProcedure));
    if (!executeprocedure)   return -1;
    executeprocedure->ApplicationField = applicationfield;
    executeprocedure->ExecuteProcedure = funct;
    ListInsert (&ptp->ConnectionProcedures, executeprocedure);
    return 0;
}



int TPRemoveConnectionProcedure (TP* ptp, int (*funct)(TPClient*, void*), void* applicationfield)
{
    TPConnectionProcedure* executeprocedure;

    if ((executeprocedure = TPFindConnectionProcedure (ptp->ConnectionProcedures, funct)) == NULL)
        return -1;
    ListRemove (&ptp->ConnectionProcedures, executeprocedure);
    free (executeprocedure);
    return 1;
}


TPConnectionProcedure* TPFindConnectionProcedure (List* ExecuteList, int (*funct)(TPClient*, void*))
{
    int found = 0;
    List* plist = ExecuteList;
    TPConnectionProcedure* pexecuteprocedure = NULL;


    while ((found == 0) && (plist != NULL))
    {
        pexecuteprocedure = (TPConnectionProcedure*)plist->car;

        if (pexecuteprocedure->ExecuteProcedure == funct)   found = 1;
        else plist = plist->cdr;
    }

    return pexecuteprocedure;
}


int TPAddEndingProcedure (TPClient* pclient, int (*funct)(TPClient* , void*), void* appfield)
{
    TPEndingProcedure* endingprocedure;

    if (!pclient) return -1;

    if ((endingprocedure = TPFindEndingProcedure (pclient->EndingProcedures, funct, appfield)) != NULL)
        return 1;

    endingprocedure = (TPEndingProcedure *) malloc (sizeof (TPEndingProcedure));
    if (!endingprocedure)  return -1;

    endingprocedure->ApplicationField = appfield;
    endingprocedure->EndingProcedure = funct;

    ListInsert (&pclient->EndingProcedures, endingprocedure);
    return 1;

}

TPEndingProcedure* TPFindEndingProcedure (List* ExecuteList, int (*funct)(TPClient*, void*), void* appfield)
{
    int found = 0;
    List* plist = ExecuteList;
    TPEndingProcedure* pendingprocedure = NULL;

    while ((found == 0) && (plist != NULL))
    {
        pendingprocedure = (TPEndingProcedure*)plist->car;

        if ((pendingprocedure->EndingProcedure == funct)  &&
                (pendingprocedure->ApplicationField == appfield))
            found = 1;
        else plist = plist->cdr;
    }

    return pendingprocedure;
}


int TPRemoveEndingProcedure (TPClient* pclient, int (*funct)(TPClient* ,void*), void* appfield)
{
    TPEndingProcedure* endingprocedure;
    if ((endingprocedure = TPFindEndingProcedure (pclient->EndingProcedures, funct, appfield)) == NULL)
        return 1;

    ListRemove (&pclient->EndingProcedures, endingprocedure);
    free (endingprocedure);
    return 1;
}




int TPSetApplicationField(TPClient* pclient, void* pfield)
{
    if (!pclient) return -1;
    pclient->ApplicationField = pfield;
    return 1;
}



int TPSetEndingProcedure(TPClient* pclient, int (*funct) (TPClient*, void*))
{
    if (!pclient) return -1;
    TPAddEndingProcedure (pclient, funct, pclient->ApplicationField);
    return 1;
}



/*-------------------------------------------------------------------*/
/* DNS */
/*-------------------------------------------------------------------*/


WORD TPGetDNSLength (char* buffer)
{

#ifdef HIGH_LOW
    return (*(WORD*)buffer);
#else
    {
        char buf[2];
        buf[0] = *(buffer + 1);
        buf[1] = *buffer;
        return (*(WORD*)buf);
    }
#endif
}

void TPSetDNSLength (char* buffer, WORD size)
{
#ifdef HIGH_LOW
    OSCopy2 (buffer, (char*)&size);
#else
    {
        char* buf = (char *)&size;
        buffer[0] = buf[1];
        buffer[1] = buf[0];
    }
#endif
}



int TPReadDNSFromClient (TP* ptp, TPClient* pclient)
{
    PIPE* pcom;
    CHANNEL* pinput;
    TPRequest* prequest;
    int gotnow, result;
    int needed;
    pcom = pclient->Pipe;
    pinput = pcom->Input;
    OSBITCLEAR(ptp->InputPending, pcom->fd);
    if (!pinput)
    {
        pinput = OSAllocateChannel ();
        if (!pinput)
            return -1;
        pcom->Input = pinput;
    }
    pinput->BufPtr += pinput->LenLastReq;  /* begin of request */
    prequest = (TPRequest*)pinput->BufPtr;

    /* gotnow = what is already been read of this request */

    gotnow = pinput->BufCnt + (int) (pinput->Buffer - pinput->BufPtr);
    if ((gotnow < sizeof (TPRequest)) ||
            (gotnow < (needed = TPGetDNSLength ((char*)prequest) + 2)))
    {
        pinput->LenLastReq = 0;
        if ((gotnow < sizeof (TPRequest)) || (needed == 0))
            needed = sizeof (TPRequest);  /* needed can be = 0 ? */
        else if (needed > MAXBUFSIZE)
        {
            OSError ("TEBAS007", NULL);
            return -1;
        }
        if ((gotnow == 0) ||
                ((pinput->BufPtr - pinput->Buffer + needed) > pinput->Size))
        {
            if ((gotnow > 0) && (pinput->BufPtr != pinput->Buffer))
                memcpy (pinput->Buffer, pinput->BufPtr, gotnow);
            if (needed > pinput->Size)
            {
                char* ibuf;
                ibuf = (char*)realloc(pinput->Buffer, needed);
                if (!ibuf)
                {
                    OSError ("TEBAS008", NULL);
                    return -1;
                }
                pinput->Size = needed;
                pinput->Buffer = ibuf;
            }
            pinput->BufPtr = pinput->Buffer;
            pinput->BufCnt = gotnow;
        }
        /* ready to read */
        result = OSRead (ptp->OS, pcom->fd, pinput->Buffer + pinput->BufCnt,
                         pinput->Size - pinput->BufCnt);

        if (result <= 0)
        {
            if ((result < 0) && (errno == EWOULDBLOCK))
            {
                /* no input for this client */
                OSBITCLEAR(ptp->ClientsWithInput, pcom->fd);
                return 0;
            }
            TCPERROR (errno);
            return -1;
        }
        pclient->ReceiptSize += result;
        pinput->BufCnt += result;
        gotnow += result;
        /* free space after huge request */
        if ((pinput->Size > BUFWATERMARK) &&
                (pinput->BufCnt < BUFSIZE) && (needed < BUFSIZE))
        {
            char* ibuf;
            ibuf = (char*)realloc (pinput->Buffer, BUFSIZE);
            if (ibuf)
            {
                pinput->Size = BUFSIZE;
                pinput->Buffer = ibuf;
                pinput->BufPtr = ibuf + pinput->BufCnt - gotnow;
            }
        }
        /* request can be completely read if client sends it complete */
        prequest = (TPRequest*)pinput->BufPtr;
        if ((gotnow < sizeof (TPRequest)) ||
                (gotnow < (needed = TPGetDNSLength ((char*)prequest) + 2)))
        {
            OSBITCLEAR(ptp->ClientsWithInput, pcom->fd);
            return 0;
        }
    }
    if (needed == 0)  needed = sizeof (TPRequest);
    /* at least one request has been read */
    pinput->LenLastReq = needed;

    if (gotnow >= needed + (int)sizeof (TPRequest))
    {
        prequest = (TPRequest*)(pinput->BufPtr + needed);
        if (gotnow >= needed + TPGetDNSLength ((char*)prequest) + 2)
            /* still a request in the buffer */
            OSBITSET(ptp->ClientsWithInput, pcom->fd);
        else
            OSBITCLEAR(ptp->ClientsWithInput, pcom->fd);
    }
    else
    {
        if (gotnow == needed)
            OSBITCLEAR(ptp->ClientsWithInput, pcom->fd);
    }
    pclient->RequestBuffer = (char*)pinput->BufPtr;
    pclient->RequestSize = needed;
    return (int)needed;
}




int TPWriteDNSToClient (TP* ptp, TPClient* pclient, char* buf, int Count)
{
    PIPE* pcom;
    CHANNEL* poutput;
    int result, gotnow, needed, written, count, towrite;
    pcom = pclient->Pipe;
    poutput = pcom->Output;
    written = 0;

    if (Count == 0)
        count = 0;
    else count = Count + sizeof (TPRequest);
    if (!poutput)
    {
        poutput = OSAllocateChannel ();
        if (!poutput)
            return -1;
        pcom->Output = poutput;
    }
    gotnow = poutput->BufCnt + (int)(poutput->Buffer - poutput->BufPtr);
    needed = count + gotnow;   /* 2 means request word for size */


    if ((gotnow != 0) && (poutput->Buffer != poutput->BufPtr))
        memcpy (poutput->Buffer, poutput->BufPtr, gotnow);


    if (needed > poutput->Size)
    {
        char* obuf;
        obuf = (char*)realloc (poutput->Buffer, needed);
        if (!obuf)
        {
            OSError ("TEBAS009", NULL);
            return -1;
        }
        poutput->Buffer = obuf;
        poutput->Size = needed;
    }
    poutput->BufPtr = poutput->Buffer;
    poutput->BufCnt = gotnow;


    if (count)
    {
        memcpy (poutput->Buffer + poutput->BufCnt + 2 , buf, count - 2);
        TPSetDNSLength (poutput->Buffer + poutput->BufCnt, (WORD)(count - 2));
    }
    poutput->BufCnt += count;
    gotnow += count;
    towrite = gotnow;
    while (written < gotnow)
    {
        result = OSWrite (ptp->OS, pcom->fd, poutput->BufPtr, towrite);
        if (result < 0)
        {
            if (errno == EWOULDBLOCK)
            {
                OSBITSET(ptp->ClientsWriteBlocked, pcom->fd);
                OSBITCLEAR(ptp->ClientsWithOutput, pcom->fd);
                ptp->AnyClientsWriteBlocked = TRUE;
                return written;
            }
            else
            {
                TCPERROR (errno);
                return -1;
            }
        }
        else
        {
            pclient->TransferSize += result;
            written += result;
            poutput->BufPtr += result;
            towrite -= result;
        }
    }

    if (ptp->AnyClientsWriteBlocked)         /* maybe no use ! */
    {
        OSBITCLEAR(ptp->ClientsWriteBlocked, pcom->fd);
        if (!OSANYSET(ptp->ClientsWriteBlocked))
            ptp->AnyClientsWriteBlocked = FALSE;
    }
    OSBITSET(ptp->ClientsWithOutput, pcom->fd);
    if (poutput->Size > BUFSIZE)
    {
        char* obuf;
        obuf = (char*)realloc (poutput->Buffer, BUFSIZE);
        if (obuf)
        {
            poutput->Size = BUFSIZE;
            poutput->Buffer = obuf;
            poutput->BufCnt = 0;
            poutput->BufPtr = obuf;
        }
    }
    return written;
}

int TPReadRequestFromFile (TP* ptp, TPClient* pclient)
{
    PIPE* pcom;
    CHANNEL* pinput;
    TPRequest* prequest;
    int gotnow, result;
    int error = 0;
    int needed;
    pcom = pclient->Pipe;
    pinput = pcom->Input;

    if (!pinput)
    {
        pinput = OSAllocateChannel ();
        if (!pinput)
            return -1;
        pcom->Input = pinput;
    }
    pinput->BufPtr += pinput->LenLastReq;  /* begin of request */
    prequest = (TPRequest*)pinput->BufPtr;

    /* gotnow = what is already been read of this request */

    gotnow = pinput->BufCnt + (int) (pinput->Buffer - pinput->BufPtr);
    if ((gotnow < sizeof (TPRequest)) ||
            (gotnow < (needed = TPGetRequestLength ((char*)prequest))))
    {
        pinput->LenLastReq = 0;
        if ((gotnow < sizeof (TPRequest)) || (needed == 0))
            needed = sizeof (TPRequest);  /* needed can be = 0 ? */
        else if (needed > MAXBUFSIZE)
        {
            return -1;
        }
        if ((gotnow == 0) ||
                ((pinput->BufPtr - pinput->Buffer + needed) > pinput->Size))
        {
            if ((gotnow > 0) && (pinput->BufPtr != pinput->Buffer))
                memcpy (pinput->Buffer, pinput->BufPtr, gotnow);
            if (needed > pinput->Size)
            {
                char* ibuf;
                ibuf = (char*)realloc(pinput->Buffer, needed);
                if (!ibuf)
                {
                    return -1;
                }
                pinput->Size = needed;
                pinput->Buffer = ibuf;
            }
            pinput->BufPtr = pinput->Buffer;
            pinput->BufCnt = gotnow;
        }
        /* ready to read */
        result = (int)OSReadFile (pcom->fileid, pinput->Buffer + pinput->BufCnt,
                                  pinput->Size - pinput->BufCnt,  &error);

        if (result < 0)
        {
            return -1;
        }
        pinput->BufCnt += result;
        gotnow += result;
        /* free space after huge request */
        if ((pinput->Size > BUFWATERMARK) &&
                (pinput->BufCnt < BUFSIZE) && (needed < BUFSIZE))
        {
            char* ibuf;
            ibuf = (char*)realloc (pinput->Buffer, BUFSIZE);
            if (ibuf)
            {
                pinput->Size = BUFSIZE;
                pinput->Buffer = ibuf;
                pinput->BufPtr = ibuf + pinput->BufCnt - gotnow;
            }
        }
        /* request can be completely read if client sends it complete */
        prequest = (TPRequest*)pinput->BufPtr;
        if ((gotnow < sizeof (TPRequest)) ||
                (gotnow < (needed = TPGetRequestLength ((char*)prequest))))
        {
            return 0;
        }
    }
    if (needed == 0)  needed = sizeof (TPRequest);
    /* at least one request has been read */
    pinput->LenLastReq = needed;

    pclient->RequestBuffer = (char*)pinput->BufPtr;
    pclient->RequestSize = needed;
    return (int)needed;
}



int TPWriteRequestToFile (TP* ptp, TPClient* pclient, char* buf, int Count)
{
    PIPE* pcom;
    CHANNEL* poutput;
    int error = 0;
    int result, gotnow, needed, written, count, towrite;
    pcom = pclient->Pipe;
    poutput = pcom->Output;
    written = 0;
    if (Count == 0)
        count = 0;
    else count = Count + sizeof (TPRequest);
    if (!poutput)
    {
        poutput = OSAllocateChannel ();
        if (!poutput)
            return -1;
        pcom->Output = poutput;
    }
    gotnow = poutput->BufCnt + (int)(poutput->Buffer - poutput->BufPtr);
    needed = count + gotnow;   /* 2 means request word for size */


    if ((gotnow != 0) && (poutput->Buffer != poutput->BufPtr))
        memcpy (poutput->Buffer, poutput->BufPtr, gotnow);

    OSBITSET(ptp->ClientsWithFileOutput, pclient->Index + 1);

    if (needed > poutput->Size)
    {
        char* obuf;
        obuf = (char*)realloc (poutput->Buffer, needed);
        if (!obuf)
        {
            return -1;
        }
        poutput->Buffer = obuf;
        poutput->Size = needed;
    }
    poutput->BufPtr = poutput->Buffer;
    poutput->BufCnt = gotnow;

    if (count)
    {
        memcpy (poutput->Buffer + poutput->BufCnt + 2 , buf, count - 2);
        TPSetRequestLength (poutput->Buffer + poutput->BufCnt, (WORD)count);
    }


    poutput->BufCnt += count;
    gotnow += count;
    towrite = gotnow;
    while (written < gotnow)
    {
        result = (int)OSWriteFile (pcom->fileid, poutput->BufPtr, towrite,  0,  &error);
        if (result < 0)
        {
            return -1;
        }
        else
        {
            written += result;
            poutput->BufPtr += result;
            towrite -= result;
        }
    }

    if (poutput->Size > BUFSIZE)
    {
        char* obuf;
        obuf = (char*)realloc (poutput->Buffer, BUFSIZE);
        if (obuf)
        {
            poutput->Size = BUFSIZE;
            poutput->Buffer = obuf;
            poutput->BufCnt = 0;
            poutput->BufPtr = obuf;
        }
    }
    return written;
}





int TPReadStreamFromFile (TP* ptp, TPClient* pclient)
{
    PIPE* pcom;
    CHANNEL* pinput;
    TPRequest* prequest;
    int gotnow, result;
    int error = 0;
    int needed;
    pcom = pclient->Pipe;
    pinput = pcom->Input;

    if (!pinput)
    {
        pinput = OSAllocateChannel ();
        if (!pinput)
            return -1;
        pcom->Input = pinput;
    }
    pinput->BufPtr += pinput->LenLastReq;  /* begin of request */
    prequest = (TPRequest*)pinput->BufPtr;

    /* gotnow = what is already been read of this request */

    gotnow = pinput->BufCnt + (int) (pinput->Buffer - pinput->BufPtr);
    if (gotnow < (needed = pcom->fileid->LineSize))
    {
        pinput->LenLastReq = 0;
        if (needed > MAXBUFSIZE)
        {
            return -1;
        }
        if ((gotnow == 0) ||
                ((pinput->BufPtr - pinput->Buffer + needed) > pinput->Size))
        {
            if ((gotnow > 0) && (pinput->BufPtr != pinput->Buffer))
                memcpy (pinput->Buffer, pinput->BufPtr, gotnow);
            if (needed > pinput->Size)
            {
                char* ibuf;
                ibuf = (char*)realloc(pinput->Buffer, needed);
                if (!ibuf)
                {
                    return -1;
                }
                pinput->Size = needed;
                pinput->Buffer = ibuf;
            }
            pinput->BufPtr = pinput->Buffer;
            pinput->BufCnt = gotnow;
        }
        /* ready to read */

        result = (int)OSReadFile (pcom->fileid, pinput->Buffer + pinput->BufCnt,
                                  pinput->Size - pinput->BufCnt,  &error);

        if (result < 0)
        {
            return -1;
        }
        pinput->BufCnt += result;
        gotnow += result;
        /* free space after huge request */
        if ((pinput->Size > BUFWATERMARK) &&
                (pinput->BufCnt < BUFSIZE) && (needed < BUFSIZE))
        {
            char* ibuf;
            ibuf = (char*)realloc (pinput->Buffer, BUFSIZE);
            if (ibuf)
            {
                pinput->Size = BUFSIZE;
                pinput->Buffer = ibuf;
                pinput->BufPtr = ibuf + pinput->BufCnt - gotnow;
            }
        }
        /* request can be completely read if client sends it complete */
        prequest = (TPRequest*)pinput->BufPtr;
        if (gotnow < (needed = pcom->fileid->LineSize))
        {
            return 0;
        }
    }
    /* at least one request has been read */
    pinput->LenLastReq = needed;

    pclient->RequestBuffer = (char*)pinput->BufPtr;
    pclient->RequestSize = needed;
    return (int)needed;
}



int TPWriteStreamToFile (TP* ptp, TPClient* pclient, char* buf, int Count)
{
    PIPE* pcom;
    CHANNEL* poutput;
    int error = 0;
    int result, gotnow, needed, written, count, towrite;
    pcom = pclient->Pipe;
    poutput = pcom->Output;
    written = 0;
    if (Count == 0)
        count = 0;
    else count = Count;
    if (!poutput)
    {
        poutput = OSAllocateChannel ();
        if (!poutput)
            return -1;
        pcom->Output = poutput;
    }
    gotnow = poutput->BufCnt + (int)(poutput->Buffer - poutput->BufPtr);
    needed = count + gotnow;   /* 2 means request word for size */


    if ((gotnow != 0) && (poutput->Buffer != poutput->BufPtr))
        memcpy (poutput->Buffer, poutput->BufPtr, gotnow);


    if (needed > poutput->Size)
    {
        char* obuf;
        obuf = (char*)realloc (poutput->Buffer, needed);
        if (!obuf)
        {
            return -1;
        }
        poutput->Buffer = obuf;
        poutput->Size = needed;
    }
    poutput->BufPtr = poutput->Buffer;
    poutput->BufCnt = gotnow;

    if (count)
    {
        memcpy (poutput->Buffer + poutput->BufCnt, buf, count);
    }


    poutput->BufCnt += count;
    gotnow += count;
    towrite = gotnow;
    while (written < gotnow)
    {
        result = (int)OSWriteFile (pcom->fileid, poutput->BufPtr, towrite,  0,  &error);
        if (result < 0)
        {
            return -1;
        }
        else
        {
            written += result;
            poutput->BufPtr += result;
            towrite -= result;
        }
    }

    OSBITSET(ptp->ClientsWithFileOutput, pclient->Index + 1);
    if (poutput->Size > BUFSIZE)
    {
        char* obuf;
        obuf = (char*)realloc (poutput->Buffer, BUFSIZE);
        if (obuf)
        {
            poutput->Size = BUFSIZE;
            poutput->Buffer = obuf;
            poutput->BufCnt = 0;
            poutput->BufPtr = obuf;
        }
    }
    return written;
}


int TPReadStreamFromBase (TP* ptp, TPClient* pclient)
{
    PIPE* pcom = pclient->Pipe;
    OSStatement* pstatement;
    OSQuery* pquery;
    int ReturnFetch = 1;

    if (!pcom->Input) 
    {
 //       OSBITCLEAR(ptp->ClientsWithBaseInput, pclient->Index + 1);
        return 0; 
    }

    pstatement = (OSStatement*)((List*)pcom->Input)->car;
    pquery = pstatement->Query;

    if (!pquery)
    {
       // ListRemove ((List**)&pcom->Input, pstatement);
        //OSFreeStatement (pstatement);
    }
    else
    {
        if (pstatement->Status == OSREADHEADER)
            pstatement->Status = OSREADCONTENT;
        if (pstatement->Status == OSREADCONTENT)
        {
            ReturnFetch =  OSFetch (pquery->Contents,
                                    pclient->Protocol,
                                    pquery->Every,
                                    pquery->MaxRows,
                                    pquery->Every,
                                    pstatement->AckProcedure,
                                    pstatement->ProcedurePar);
        }
       /* else
            if (pstatement->Status == OSREADACK)
                pstatement->Status = OSREADEND;
            else
            if (pstatement->Status == OSREADEND)
            {
                ListRemove ((List**)&pcom->Input, pstatement);
                OSFreeStatement (pstatement);

            }
            */
    }
    if (!pcom->Input)
            OSBITCLEAR(ptp->ClientsWithBaseInput, pclient->Index + 1);
    return 1;
}



int TPWriteStreamToBase (TP* ptp, TPClient* pclient, char* buf, int Count)
{
    PIPE* pcom = pclient->Pipe;
    OSStatement* pstatement;

    if (!pcom->Output) return 0;

    OSBITSET(ptp->ClientsWithBaseOutput, pclient->Index + 1);
    pstatement = (OSStatement*)((List*)pcom->Output)->car;
    pstatement->UserField = pclient;


    ListRemove ((List**)&pcom->Output, pstatement);
    ListNewr ((List**)&pcom->Input, pstatement);
    OSBITSET(ptp->ClientsWithBaseInput, pclient->Index + 1);

    if (pstatement->Query != NULL)
    {
        OSQuery* pquery = pstatement->Query;
        pquery->Contents = OSProcess (pcom->connection,
                                      pclient->Protocol,
                                      pstatement->SqlStatement,
                                      pquery->Every,
                                      pquery->MaxRows,
                                      pquery->HeaderProcedure,
                                      pquery->ContentsProcedure,
                                      pquery->ProcedurePar,
                                      pstatement->AckProcedure,
                                      pstatement->ProcedurePar);
         return 1;
    }
    if (pstatement->SqlType == DBSQLCOMMIT)
    {

        OSCommitOrRollback (pcom->connection,
                            pclient->Protocol,
                            pstatement->Commit,
                            pstatement->AckProcedure,
                            pstatement->ProcedurePar);
    //    ListRemove ((List**)&pcom->Input, pstatement);
    //    OSFreeStatement (pstatement);
        return 1; 

    }
    else
    {
        OSExecute (pcom->connection,
                   pclient->Protocol,
                   pstatement->SqlStatement,
                   pstatement->Commit,
                   pstatement->AckProcedure,
                   pstatement->ProcedurePar);

 //       ListRemove ((List**)&pcom->Input, pstatement);
  //      OSFreeStatement (pstatement);
        return 1;
    }
    return 1;
}



