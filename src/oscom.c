/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "oscom.h"



/* find first set : returns the descritpor of the first entry*/


/* if there is any bit set in the mask */
#if (MaskCount > 4 )
BOOL OSANYSET (long* src)
{
    int i;
    for (i = 0; i < MaskCount; i++)
        if (src[i]) return TRUE;
    return FALSE;
}
#endif

int OSFFS (long* buf)
{
    int i;

    for (i = 31 ; i >= 0 ; i--)
        if (*buf & (1L << i))
            return i;
    return 0;
}


int FDTRANSLATE (OS* pos, int socket)
{
#ifdef WINDOWS
    while ((pos->FDNEXT < SOCKMAX + 1) && pos->FDTAB[pos->FDNEXT])
        pos->FDNEXT++;
    pos->FDTAB[pos->FDNEXT] = socket;
    return pos->FDNEXT;
#endif
#ifdef __VOS__
    if (pos->MaxSocketNumber < socket)
        pos->MaxSocketNumber = socket;

    return socket - BASE_SOCKET_ID;

#endif
    return 1;
}

void FDREMOVESOCKET (OS* pos, int socket)
{
    if (socket < pos->FDNEXT)
        pos->FDNEXT = socket;
    pos->FDTAB[socket] = 0;
}

int FDRETURNSOCKET (OS* pos, int socket)
{
    int i = 1;
    while (i < SOCKMAX + 1)
        if (pos->FDTAB[i] == socket)
            return i;
        else i++;
    return 0;
}

#ifdef WINDOWS 

int PASCAL FAR __WSAFDIsSet (SOCKET fd, fd_set FAR* set)
{

    int i = set->fd_count;
    while (i--)
        if (set->fd_array[i] == fd)
            return 1;
    return 0;
}


void MSKTOFDSET(OS* pos, long* buf, fd_set* bufd)
{
    int i,j;
    int socket;
    FD_ZERO (bufd);
    if (buf)
        for (j = 0; j < MaskCount; j++)

            for (i = 31 ; i >= 0 ; i--)
                if (buf[j] & (1L << i))
                {
                    socket = FDSOCKET(pos, i + (j << 5));
                    FD_SET (socket, bufd);
                    buf[j] &= ~ (1L << i);
                }
}


void FDSETTOMSK (OS* pos, fd_set* bufd, long* buf)
{
    int socket;
    int i = bufd->fd_count;
    if (buf)
    {
        OSCLEARBITS(buf);
    }
    while (i--)
    {
        socket = FDRETURNSOCKET(pos, bufd->fd_array[i]);
        OSBITSET(buf, socket);
    }
}
#endif

void OSSleep (int time)
{
#ifndef WINDOWS
    /* sleep (time); */
#endif
}

int OSInit (OS* pos)
{
    int i;
#ifdef WINDOWS
    WSADATA wsadata;
    if (WSAStartup(0x101, &wsadata) != 0)
    {
        OSError ("TESYS001", NULL);
        return -1;
    }
#elif defined(OS2)
    if ( sock_init() != 0 )
    {
        OSError ("TESYS001", NULL);
        return -1;
    }
#else
#ifdef _VM_
    int totdesc, inetdesc;
    totdesc = inetdesc = 1022;
    maxdesc (&totdesc, &inetdesc);
#endif
#endif
    pos->FDNEXT = 1;
    for (i=0; i < SOCKMAX; i++)
        pos->FDTAB[i] = 0;
    pos->NbrOpenedSockets = 0;
    pos->NbrClosedSockets = 0;
#ifdef __VOS__
    pos->MaxSocketNumber = 0;
#else
    pos->MaxSocketNumber = SOCKMAX - 1;
#endif
    return 1;
}


void OSEnd (OS* pos)
{
#ifdef WINDOWS
    WSACleanup ();
#endif
}



int OSSetSockOpt (OS* pos, int s, int level, int optname, void* optval, int optlen)
{
#if defined(WINDOWS) || defined (__VOS__)
    s = FDSOCKET(pos, s);
#endif
    return setsockopt (s, level, optname, (char*)optval, optlen);

}


int OSGetSockName (OS* pos, int s, char* netid)
{
    int size, ret;
    struct sockaddr name;
    struct sockaddr_in * Name;

    size = sizeof (name);
#if defined(WINDOWS) || defined (__VOS__)
    s = FDSOCKET(pos, s);
#endif
    if ((ret = getsockname (s, &name, &size)) < 0)
    {
        TCPERROR (errno);
        OSError ("TESYS002", " (%d)", s);
        return -1;
    }
    Name = (struct sockaddr_in*)&name;

    strcpy (netid, inet_ntoa (Name->sin_addr));
    return ret;
}

int OSGetPeerName (OS* pos, int s, int type, char* userid, char* appliname)
{
    int size, ret;
    struct sockaddr name;

    size = sizeof (name);
#if defined(WINDOWS) || defined (__VOS__)
    s = FDSOCKET(pos, s);
#endif
    if ((ret = getpeername (s, &name, &size)) < 0)
    {
        TCPERROR (errno);
        OSError ("TESYS002", " (%d)", s);
        return -1;
    }
    if (type) /* means distant */
    {
        strcpy (userid, inet_ntoa (((struct sockaddr_in*)&name)->sin_addr));
    }
    return ret;
}


int OSAccept (OS* pos, int s)
{
    int request;
    struct sockaddr name;
    DWORD one = 1;
    int size = sizeof (name);

#ifdef SO_LINGER
    int linger[2] = {0, 0};
#endif /* SO_LINGER */


#if defined(WINDOWS) || defined (__VOS__)
    s = FDSOCKET(pos, s);
#endif
    if ((request = accept (s,  (struct sockaddr*)&name, &size)) < 0)
    {
        TCPERROR (errno);
        OSError ("TESYS005", " (%d)", errno);
        return -1;
    }
#if defined(WINDOWS) || defined (__VOS__)
    request = FDTRANSLATE(pos, request);
#endif


    pos->NbrOpenedSockets++;

#ifdef SO_LINGER
    OSSetSockOpt (pos, request, SOL_SOCKET, SO_LINGER, &linger, sizeof (linger));
#endif /* SO_LINGER */

    OSSetSockOpt (pos, request, SOL_SOCKET, SO_KEEPALIVE, &one, sizeof (DWORD));

    if (OSIoctl (pos, request, FIONBIO, &one) < 0)
    {
        TCPERROR (errno);
        OSError ("TESYS007", " (%d)", request);
    }
    return request;
}


int OSIoctl (OS* pos, int s, LONG cmd, void* data)
{

#ifdef WINDOWS
    s = FDSOCKET(pos, s);
    return ioctlsocket (s, cmd, (DWORD*)data);
#elif defined(OS2)
    /* ATTENTION : les commandes utilis�es ici ont toujours pour argument un long
    *             on met donc la taille en dur !
    */
    return ioctl (s, cmd, (char *)data, sizeof(long));
#else
#ifdef __VOS__
    DWORD block;
    OSCopy4(&block, data);
    if (block == 1)  block = 0; else block = 1;
    s = FDSOCKET(pos, s);
    return net_ioctl (s, cmd, (char*)&block);
#else
    return ioctl (s, cmd, (char*)data);
#endif
#endif
}


int OSWrite (OS* pos, int s, char* buf, int len)
{
#ifdef WINDOWS
    s = FDSOCKET(pos, s);
    return send (s, buf, len, MSG_DONTROUTE);
#elif defined(OS2)
    return send (s, buf, len, MSG_DONTROUTE);
#else
  #ifdef __VOS__
    s = FDSOCKET(pos, s);
    return net_write (s, buf, len, 0);
  #else
    return write (s, buf, len);
  #endif
#endif
}


int OSRead (OS* pos, int s, char* buf, int len)
{
#ifdef WINDOWS
    s = FDSOCKET(pos, s);
    return recv (s, buf, len, 0);
#elif defined(OS2)
    return recv (s, buf, len, MSG_DONTROUTE);
#else
  #ifdef __VOS__
    s = FDSOCKET(pos, s);
    return net_read (s, buf, len, 0);
  #else
    return read (s, buf, len);
  #endif
#endif
}



int OSCloseSocket (OS* pos, int s)
{
#ifdef WINDOWS
    int request;
    request = FDSOCKET(pos, s);
    FDREMOVESOCKET (pos, s);
    pos->NbrClosedSockets++;
    return closesocket (request);
#elif defined(OS2)
    pos->NbrClosedSockets++;
    return soclose (s);
#else
  #ifdef __VOS__
    int request;
    request = FDSOCKET(pos, s);
    pos->NbrClosedSockets++;
    return net_close (request);
  #else
    pos->NbrClosedSockets++;
    return close (s);
  #endif
#endif

}


int OSOpenInetSocket(OS* pos, int type, BOOL Block)
{
    struct protoent* protostruct;
    int request = 0;
    int request1;
    DWORD one = 1;

    protostruct = getprotobyname ("icmp");
    protostruct = getprotobyname("ip");
    protostruct = getprotobyname("udp");
    protostruct = getprotobyname("tcp");

    if ((type == IPPROTO_ICMP) || (type == IPPROTO_IP))
        if ((request = socket (AF_INET, SOCK_RAW, protostruct->p_proto)) < 0)
        {
            TCPERROR (errno);
            OSError ("TESYS035",  " (%d)", errno);
            return -1;
        }

    if ((type == IPPROTO_TCP) || (type == IPPROTO_DG)  || (type == IPPROTO_CB2A))
        if ((request = socket (AF_INET, SOCK_STREAM, 0 /*protostruct->p_proto*/)) < 0)
        {
            TCPERROR (errno);
            OSError ("TESYS035",  " (%d)", errno);
            return -1;
        }
    if (type == IPPROTO_UDP)
        if ((request = socket (AF_INET, SOCK_DGRAM, 0 /*protostruct->p_proto*/)) < 0)
        {
            TCPERROR (errno);
            OSError ("TESYS035",  " (%d)", errno);
            return -1;
        }
    if (request == 0) return -1;


#ifdef WINDOWS
    if ((LONG)request == INVALID_SOCKET)
#else
    if ( request >= SOCKMAX )
#endif
    {
        TCPERROR (errno);
        OSError ("TESYS036",  " (%d)", errno);
        return -1;
    }

    pos->NbrOpenedSockets++;

    request1 = request;
#if defined(WINDOWS) || defined (__VOS__)
    request1 = FDTRANSLATE (pos, request);
#endif
    if (Block == FALSE)
    {
#if defined(WINDOWS) || defined (__VOS__)
        if (OSIoctl (pos, request1, FIONBIO, &one) < 0)
#else
        if (OSIoctl (pos, request, FIONBIO, &one) < 0)
#endif
            OSError ("TESYS029",  " (%d)", request);
    }
#if defined(WINDOWS) || defined (__VOS__)
    request = request1;
#endif
    return request;
}



/* binding a socket to a port */

#ifdef TCPCONN
int OSOpenTcpSocket (OS* pos, WORD port)
{
    struct sockaddr_in insock;
    int request;
    int request1;
    int retry;
#ifndef WINDOWS
    DWORD one = 1;
#endif
#ifdef SO_LINGER
    int linger[2] = {0, 0};
#endif /* SO_LINGER */

    if ((request = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
        TCPERROR (errno);
        OSError ("TESYS008", " (%d)", errno);
        return -1;
    }
#ifdef WINDOWS
    if ((LONG)request == INVALID_SOCKET)
#else
    if ( request >= SOCKMAX )
#endif
    {    /*  modifier pour mettre le bon message d'erreur */
        TCPERROR (errno);
        OSError ("TESYS026",  " (%d)", errno);
        return -1;
    }
    pos->NbrOpenedSockets++;

    request1 = request;
#if defined(WINDOWS) || defined (__VOS__)
    request1 = FDTRANSLATE(pos, request);
#endif
#ifndef WINDOWS
#ifdef SO_REUSEADDR      
#ifndef __VOS__
    {
        if (OSSetSockOpt (pos, request1, SOL_SOCKET, SO_REUSEADDR, &one, sizeof (DWORD)))
            OSError ("TESYS009",  " (%d)", request);
    }
#endif
#endif /* SO_REUSEADDR */
#endif 

    memset ((void*)&insock, (int)0, sizeof (insock));
    insock.sin_family = AF_INET;
    insock.sin_port = htons (port);
    insock.sin_addr.s_addr = htonl (INADDR_ANY);
    retry = 20;

    while (bind (request, (struct sockaddr*)&insock, sizeof (insock)))
    {
        if (--retry == 0)
        {
            TCPERROR (errno);
            OSError ("TESYS010",  " (%d)", errno);
            OSCloseSocket (pos, request1);
            return -1;
        }
#ifdef SO_REUSEADDR
        OSSleep (1);
#else
        OSSleep (10);
#endif /* SO_REUSEADDR */
    }  /* eof while */

    if (listen (request, 5))
    {
        TCPERROR (errno);
        OSError ("TESYS012",  " (%d)", errno);
        OSCloseSocket (pos, request1);
        return -1;
    }
#ifdef SO_LINGER
    OSSetSockOpt (pos, request1, SOL_SOCKET, SO_LINGER, &linger, sizeof (linger));
#endif /* SO_LINGER */
#if defined(WINDOWS) || defined (__VOS__)
    request = request1;
#endif

    return request;
}
#endif /* TCPCONN */

/*===========================================================================*/


#ifdef UNIXCONN
static int open_unix_socket (OS* pos, char* path)
{
    int oldmask;
    int request;
    struct sockaddr_un unsock;
    unsock.sun_family = AF_UNIX;
    oldmask = umask(0);
    mkdir ("basecom", 0777);
    chmod ("basecom", 0777);
    strcpy (unsock.sun_path, "basecom");
    strcat (unsock.sun_path, path);
    unlink (unsock.sun_path);
    if ((request = socket (AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        TCPERROR (errno);
        OSError ("TESYS013",  " (%d)", errno);
        return -1;
    }
    else
        if ( request >= SOCKMAX )
        {
            TCPERROR (errno);
            OSError ("TESYS026",  " (%d)", errno);
            return -1;
        }

    pos->NbrOpenedSockets++;

    if (bind (request, (struct sockaddr*)&unsock, strlen (unsock.sun_path)+2))
    {
        TCPERROR (errno);
        OSError ("TESYS014", " (%d)", errno);
        OSCloseSocket (pos, request);
        return -1;
    }

    if (listen (request, 5))
    {
        OSError ("TESYS015", " (%d)", errno);
        OSCloseSocket (pos, request);
        return -1;
    }
    (void)umask(oldmask);
    return request;
}
#endif /* UNIXCONN */

/*===========================================================================*/

#ifdef IUCVCONN
static int open_iucv_socket (OS* pos, char* name, char* appliname)
{
    int request;
    struct sockaddr_iucv iucvsock;
    int retry;
    int l_name, l_appli;
    if ((l_name = strlen (name)) > 8)
        l_name = 8;
    if ((l_appli = strlen (appliname)) > 8)
        l_appli = 8;


    memset ((void*)&iucvsock, (int)0, sizeof (iucvsock));
    strncpy (iucvsock.siucv_nodeid, "        ", 8);
    strncpy (iucvsock.siucv_userid, "        ", 8);
    strncpy (iucvsock.siucv_name, "        ", 8);
    strncpy (iucvsock.siucv_userid, name, l_name);
    strncpy (iucvsock.siucv_name, appliname, l_appli);
    iucvsock.siucv_family = AF_IUCV;


    if ((request = socket (AF_IUCV, SOCK_STREAM, 0)) < 0)
    {
        TCPERROR (errno);
        OSError ("TESYS016", " (%d)", errno);
        return -1;
    }
    else
#ifdef WINDOWS
        if (request == INVALID_SOCKET)
#else
        if ( request >= SOCKMAX )
#endif
        {    /* � modifier pour mettre le bon message d'erreur */
            TCPERROR (errno);
            OSError ("TESYS026", " (%d)", errno);
            return -1;
        }

    NbrOpenedSockets++;

    retry = 20;
    while (bind (request, (struct sockaddr*)&iucvsock, sizeof (iucvsock)))
    {
        if (--retry == 0)
        {
            TCPERROR (errno);
            OSError ("TESYS017", " (%d)", errno);
            OSCloseSocket (request);
            return -1;
        }
    }

    if (listen (request, 5))
    {
        TCPERROR (errno);
        OSError ("TESYS018",  " (%d)", errno);
        OSCloseSocket (request);
        return -1;
    }
    return request;
}

static int connect_iucv_socket (OS* pos, char* name, char* appliname)
{
    int request;
    int one = 1;
    struct sockaddr_iucv iucvsock;
    int retry;
    int l_name, l_appli;

#ifdef SO_LINGER
    int linger[2] = {0, 0};
#endif /* SO_LINGER */


    if ((l_name = strlen (name)) > 8)
        l_name = 8;
    if ((l_appli = strlen (appliname)) > 8)
        l_appli = 8;


    memset ((void*)&iucvsock, (int)0, sizeof (iucvsock));
    strncpy (iucvsock.siucv_nodeid, "        ", 8);
    strncpy (iucvsock.siucv_userid, "        ", 8);
    strncpy (iucvsock.siucv_name, "        ", 8);
    strncpy (iucvsock.siucv_userid, name, l_name);
    strncpy (iucvsock.siucv_name, appliname, l_appli);
    iucvsock.siucv_family = AF_IUCV;


    if ((request = socket (AF_IUCV, SOCK_STREAM, 0)) < 0)
    {
        TCPERROR (errno);
        OSError ("TESYS019",  " (%d)", errno);
        return -1;
    }
    else
#ifdef WINDOWS
        if (request == INVALID_SOCKET)
#else
        if ( request >= SOCKMAX )
#endif
        {    /* � modifier pour mettre le bon message d'erreur */
            TCPERROR (errno);
            OSError ("TESYS026",  " (%d)", errno);
            return -1;
        }
    retry = 20;

    NbrOpenedSockets++;

    while (connect (request, (struct sockaddr*)&iucvsock,
                    sizeof (iucvsock)) < 0)
    {

        if (--retry == 0)
        {
            TCPERROR (errno);
            OSError ("TESYS020",  " (%d)", errno);
            OSCloseSocket (pos, request);
            return -1;
        }
        OSSleep (1);
    }  /* eof while */

#ifdef SO_LINGER
    OSSetSockOpt (pos, request, SOL_SOCKET, SO_LINGER, &linger, sizeof (linger));
#endif /* SO_LINGER */


    if (OSIoctl (pos, request, FIONBIO, &one) < 0)
        OSError ("TESYS022",  " (%d)", request);
    return request;
}


#endif /* IUCVCONN */


/*===========================================================================*/


/* General function */


int OSOpenLocalSocket (OS* pos, char* name, char* appliname)
{
#ifdef WINDOWS
    /*  OSError ("TESYS023", NULL);*/
    return -1;
#endif
#ifdef OS2
    return -1;
#endif
#ifdef UNIXCONN
    return open_unix_socket (pos, name);
#endif
#ifdef IUCVCONN
    return open_iucv_socket (name, appliname);
#endif
    return -1;
}



int OSLocalConnect(OS* pos, char* name, char* appliname)
{
#ifdef WINDOWS
    OSError ("TESYS024", " (%s %s)", name, appliname);
    return -1;
#endif
#ifdef OS2
    OSError ("TESYS024", " (%s %s)", name, appliname);
    return -1;
#endif
#ifdef UNIXCONN
    OSError ("TESYS025", " (%s %s)", name, appliname);
    return -1;
#endif
#ifdef IUCVCONN
    return connect_iucv_socket (pos, name, appliname);
#endif

}



int OSTcpConnect(OS* pos, char* MachinName, WORD port, BOOL Block, BOOL* ConnectOK)
{
    struct sockaddr_in insock;
    struct hostent* phost;
    int request, request1;
    DWORD AdressIP;
    DWORD one = 1;
    int err;

    memset ((void*)&insock, (int)0, sizeof (insock));
    insock.sin_family = AF_INET;
    if ((AdressIP = inet_addr (MachinName)) == INADDR_NONE)
    {
        if ((phost = gethostbyname (MachinName)) == NULL)
            return -1;
        else
        {
            insock.sin_family = phost->h_addrtype;
            OSCopy4(&AdressIP, phost->h_addr);
        }
    }

    if ((request = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
        TCPERROR (errno);
        OSError ("TESYS026",  " (%d)", errno);
        return -1;
    }

#ifdef WINDOWS
    if ((LONG)request == INVALID_SOCKET)
#else
    if ( request >= SOCKMAX )
#endif
    {
        TCPERROR (errno);
        OSError ("TESYS037",  " (%d)", errno);
        return -1;
    }

    /* can i use si_addr ?
    strcpy (insock.sin_zero, "abcde");*/

    insock.sin_port = htons (port);
    insock.sin_addr.s_addr = AdressIP;
    request1 = request;
#if defined(WINDOWS) || defined (__VOS__)
    request1 = FDTRANSLATE (pos, request);
#endif
    if (Block == FALSE)
    {
#if defined(WINDOWS) || defined (__VOS__)
        if (OSIoctl (pos, request1, FIONBIO, &one) < 0)
#else
        if (OSIoctl (pos, request, FIONBIO, &one) < 0)
#endif
            OSError ("TESYS029",  " (%d)", request);
    }
    *ConnectOK = FALSE;

    pos->NbrOpenedSockets++;

    if (connect (request, (struct sockaddr*)&insock, sizeof (insock)) != 0)
    {
        err = errno;
        if ((Block == TRUE) || (err != EWOULDBLOCK))
        {
            TCPERROR (errno);
            OSCloseSocket (pos, request1);
            return -1;
        }
    }
    else  *ConnectOK = TRUE;
#if defined(WINDOWS) || defined (__VOS__)
    request = request1;
#endif
    return request;
}



int OSSelect (OS* pos, int nfds, long* readmsk, long* writemsk,
              long* exceptmsk, struct timeval* timeout)
{
    int ret;
#if defined(WINDOWS) 
    fd_set readfds, writefds, exceptfds;
    MSKTOFDSET(pos, readmsk, &readfds);
    MSKTOFDSET(pos, writemsk, &writefds);
    MSKTOFDSET(pos, exceptmsk, &exceptfds);
    ret = select (nfds, (fd_set*)&readfds, (fd_set*)&writefds,
                  (fd_set*)&exceptfds, timeout);
    FDSETTOMSK(pos, &readfds, readmsk);
    FDSETTOMSK(pos, &writefds, writemsk);
    FDSETTOMSK(pos, &exceptfds, exceptmsk);

#else

    ret = select (nfds, (fd_set*)readmsk, (fd_set*)writemsk,
                  (fd_set*)exceptmsk, timeout);
#endif
    return ret;
}




int OSGiveSocket (OS* pos, int s, char* name)
{
#ifndef IUCVCONN

    OSError ("TESYS030", " (%s)", name);
    return -1;
#else

    struct clientid client;
    int l_name;

    if ((l_name = strlen (name)) > 8)
        l_name = 8;
    memset ((void*)&client, (int)0, sizeof (client));
    client.domain = AF_INET;
    strncpy (client.name, "        ", 8);
    strncpy (client.name, name, l_name);
    strncpy( client.subtaskname, "        ",8);
    return givesocket (s, &client);
#endif
}

int OSTakeSocket (OS* pos, int s, char* name, char* subtaskname)
{
#ifndef IUCVCONN

    OSError ("TESYS031",  " (%s, %s)", name, subtaskname);
    return -1;
#else
    struct clientid client;
    int request;
    DWORD one = 1;
    int l_name, l_subtask;

    if ((l_name = strlen (name)) > 8)
        l_name = 8;
    if ((l_subtask = strlen (subtaskname)) > 8)
        l_subtask = 8;
    memset ((void*)&client, (int)0, sizeof (client));
    client.domain = AF_INET;
    strncpy (client.name, "        ", 8);
    strncpy (client.name, name, l_name);
    strncpy( client.subtaskname, "        ",8);
    strncpy (client.subtaskname, subtaskname, l_subtask);
    request =  takesocket (&client, s);
    if (request < 0) TCPERROR (errno);
    if ((request > 0) && (OSIoctl (pos, request, FIONBIO, &one) < 0))
        OSError ("TESYS032",  " (%d)", request);
    OSSetSockOpt (pos, request, SOL_SOCKET, SO_KEEPALIVE, &one, sizeof (DWORD));
    return request;

#endif
}




int OSRecv (OS* pos, int s, char* buf, int len, char* AddrName)
{
    int result;
    struct sockaddr_in fromaddr;
    int size = sizeof (struct sockaddr_in);
#if defined(WINDOWS) || defined (__VOS__)
    s = FDSOCKET(pos, s);
#endif
    result = recvfrom(s, buf ,len , 0, (struct sockaddr *)&fromaddr, &size);
    if (result <= 0) return result;
    strcpy (AddrName, inet_ntoa (fromaddr.sin_addr));
    return result;
}


int OSSend (OS* pos, int s, char* buf, int len, char* MachinName)
{
    int result;
    struct sockaddr_in insock;
    struct hostent* phost;
    DWORD AdressIP;

    memset ((void*)&insock, (int)0, sizeof (insock));
    insock.sin_family = AF_INET;
    if ((AdressIP = inet_addr (MachinName)) == INADDR_NONE)
    {
        if ((phost = gethostbyname (MachinName)) == NULL)
            return -1;
        else
        {
            insock.sin_family = phost->h_addrtype;
            OSCopy4(&AdressIP, phost->h_addr);
        }
    }
    insock.sin_addr.s_addr = AdressIP;

#if defined(WINDOWS) || defined (__VOS__)
    s = FDSOCKET(pos, s);
#endif
    result = sendto(s, buf ,len , 0, (struct sockaddr *)&insock, sizeof (insock));
    return result;
}


void OSCopy2 (void* word, char* stream)
{
    *(char*)word =stream[0] ;
    *((char*)word + 1) =stream[1];
}


void OSCopy4 (void* dword, char* stream)
{
    *(char*)dword =stream[0] ;
    *((char*)dword + 1) =stream[1];
    *((char*)dword + 2) =stream[2];
    *((char*)dword + 3) =stream[3];
}


void OSCopy8 (void* ddouble, char* stream)
{
    *(char*)ddouble =stream[0] ;
    *((char*)ddouble + 1) =stream[1];
    *((char*)ddouble + 2) =stream[2];
    *((char*)ddouble + 3) =stream[3];
    *((char*)ddouble + 4) =stream[4];
    *((char*)ddouble + 5) =stream[5];
    *((char*)ddouble + 6) =stream[6];
    *((char*)ddouble + 7) =stream[7];
}


int OSGetNameFromAddr (char* AddrName, char* Name)
{
    struct hostent* phost;
    DWORD inetaddr;

    inetaddr = inet_addr (AddrName);

    phost = gethostbyaddr ((char*)(&inetaddr),4 , AF_INET);
    if (phost && (phost-> h_name))
    {
        strcpy (Name, phost->h_name);
        return 1;
    }
    return -1;
}

int OSGetAddrFromName (char* Name, char* AddrName)
{
    DWORD AdressIP;
    struct hostent* phost;
    struct in_addr fromaddr;

    if ((phost = gethostbyname (Name)) == NULL)
        return -1;
    else
    {
        OSCopy4(&AdressIP, phost->h_addr);
        fromaddr.s_addr = AdressIP;
        strcpy (AddrName, inet_ntoa (fromaddr));
    }
    return 1;
}

WORD OSCheckSum (WORD* pPacket, int nBytes)
{
    long sum;
    LONG answer;

    sum = 0L;
    while(nBytes > 1)
    {
        sum += *pPacket++;
        nBytes -= sizeof(WORD);
    }

    if (nBytes)
    {
        sum += *(BYTE *)pPacket;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer= ~sum;
    return ((WORD)answer);
}



int OSGetHostName (char* Name, int NameLength)
{
    return gethostname (Name, NameLength);
}

