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
/*-------------------------------------------------------------------------*/
/* TCPCONN    : Les Connexions de type AF_INET sur PC, Unix, Vm, Xerox, .. */
/* UNIXCONN   : Les Connexions de type AF_UNIX                             */
/* IUCVCONN   : Les Connexions de type AF_IUCV                             */
/*-------------------------------------------------------------------------*/
/* WINDOWS    :    Windows Environment                                     */
/* UNIX       :    Unix Environment                                        */
/* _VM_       :    IBM Vm environment                                      */
/* AS400      :    AS400 Environement                                      */
/* __VOS__    :    Stratus Vos Environement                                */
/* SCO        :    Unix SCO Environement                                   */
/*-------------------------------------------------------------------------*/




#ifndef __OSCOM_H__
#define __OSCOM_H__

#include <stdlib.h>
#include <stdio.h>

#include "ostypes.h"
#include "ostrace.h"
#include "osip.h"
/* Standard constants */

#define IPPROTO_FTP             21               
#define IPPROTO_SMTP            25               
#define IPPROTO_DNS             53               
#define IPPROTO_HTTP            80               
#define IPPROTO_POP             110               
#define IPPROTO_BC              100               
#define IPPROTO_DG              30000     
#define IPPROTO_CB2A            30001   
#define IPPROTO_MTT              2010               
#define TCPCONN

#ifndef AF_INET6
#define AF_INET6	            24
#endif              



#ifndef __VOS__
#define LOOP_TIMING             1000
#else                                /*VOS select timing  is not correct */
#define LOOP_TIMING             0   
#endif


#ifdef WINDOWS
  #undef EWOULDBLOCK      
  #define EWOULDBLOCK        WSAEWOULDBLOCK     
 
  #undef EINPROGRESS        
  #define EINPROGRESS        WSAEINPROGRESS     

  #undef EALREADY         
  #define EALREADY           WSAEALREADY        

  #undef ENOTSOCK         
  #define ENOTSOCK           WSAENOTSOCK        

  #undef EDESTADDRREQ     
  #define EDESTADDRREQ       WSAEDESTADDRREQ    

  #undef EMSGSIZE         
  #define EMSGSIZE           WSAEMSGSIZE        

  #undef EPROTOTYPE       
  #define EPROTOTYPE         WSAEPROTOTYPE      

  #undef ENOPROTOOPT      
  #define ENOPROTOOPT        WSAENOPROTOOPT     

  #undef EPROTONOSUPPORT  
  #define EPROTONOSUPPORT    WSAEPROTONOSUPPORT 

  #undef ESOCKTNOSUPPORT  
  #define ESOCKTNOSUPPORT    WSAESOCKTNOSUPPORT 

  #undef EOPNOTSUPP       
  #define EOPNOTSUPP         WSAEOPNOTSUPP      
  #undef EPFNOSUPPORT     
  #define EPFNOSUPPORT       WSAEPFNOSUPPORT    
  #undef EAFNOSUPPORT     
  #define EAFNOSUPPORT       WSAEAFNOSUPPORT    
  #undef EADDRINUSE       
  #define EADDRINUSE         WSAEADDRINUSE      
  #undef EADDRNOTAVAIL    
  #define EADDRNOTAVAIL      WSAEADDRNOTAVAIL   

  #undef ENETDOWN         
  #define ENETDOWN           WSAENETDOWN        

  #undef ENETUNREACH      
  #define ENETUNREACH        WSAENETUNREACH     

  #undef ENETRESET        
  #define ENETRESET          WSAENETRESET       

  #undef ECONNABORTED     
  #define ECONNABORTED       WSAECONNABORTED    

  #undef ECONNRESET       
  #define ECONNRESET         WSAECONNRESET      

  #undef ENOBUFS          
  #define ENOBUFS            WSAENOBUFS         
  #undef EISCONN          
  #define EISCONN            WSAEISCONN         
  #undef ENOTCONN         
  #define ENOTCONN           WSAENOTCONN        

  #undef ESHUTDOWN        
  #define ESHUTDOWN          WSAESHUTDOWN       

  #undef ETOOMANYREFS     
  #define ETOOMANYREFS       WSAETOOMANYREFS    

  #undef ETIMEDOUT        
  #define ETIMEDOUT          WSAETIMEDOUT       

  #undef ECONNREFUSED     
  #define ECONNREFUSED       WSAECONNREFUSED    

  #undef ELOOP            
  #define ELOOP              WSAELOOP           

  #undef ENAMETOOLONG     
  #define ENAMETOOLONG       WSAENAMETOOLONG    

  #undef EHOSTDOWN        
  #define EHOSTDOWN          WSAEHOSTDOWN       

  #undef EHOSTUNREACH     
  #define EHOSTUNREACH       WSAEHOSTUNREACH    

  #undef ENOTEMPTY        
  #define ENOTEMPTY          WSAENOTEMPTY       
  #undef EPROCLIM         
  #define EPROCLIM           WSAEPROCLIM        
  #undef EUSERS           
  #define EUSERS             WSAEUSERS          
  #undef EDQUOT           
  #define EDQUOT             WSAEDQUOT          
  #undef ESTALE           
  #define ESTALE             WSAESTALE          
  #undef EREMOTE          
  #define EREMOTE            WSAEREMOTE         

 #undef EBADF
 #define EBADF ENOTSOCK                                                      
 #undef errno                                                                
 #define errno WSAGetLastError()                                             
 #define FDSOCKET(pos, socket)   pos->FDTAB[socket]
#endif

#ifdef OS2
 #undef errno
 #define errno sock_errno()
#endif




#if !defined(LOW_HIGH) && !defined(HIGH_LOW)
#endif

#if !defined(CHAR_ASCII) && !defined(CHAR_EBCDIC) && !defined(CHAR_OS2)
#endif


#if defined(WINDOWS) || defined(AS400) 
  #define  SOCKMAX   128            /* system dependent */
#elif defined(OS2)
  #define  SOCKMAX   2048           /* system dependent */
#else
  #ifdef __VOS__
   #define SOCKMAX 256
  #else
   #define SOCKMAX 1024
  #endif
#endif

#ifdef __VOS__
 #define FDSOCKET(pos, socket)     (socket + BASE_SOCKET_ID)
#endif



#define  MaskCount     ((SOCKMAX + 31) / 32)

/* OSBITMASK  : shift de l'identifica    teur dans l'élément du tableau      */
/* OSMASKIDX  : rend le rang du tableau correspondant à l'identificateur */
/* OSGETBIT   : rend le mask si il existe                                */
/* OSBITCLEAR : enlever l'identificateur du mask                         */
/* OSBITSET   : mettre l'identificateur dans le mask                     */
/* OSMASKWORD : rend l'élément  du tableau contenant l'identificateur    */



#if (MaskCount == 1)
#define OSBITMASK(i)  (1L << (i))
#define OSMASKIDX(i)  0
#endif

#if (MaskCount > 1)
#define  OSBITMASK(i)  (1L << ((i) & 31))
#define  OSMASKIDX(i)  ((i) >> 5)
#endif

#define  OSMASKWORD(buf, i)  buf[OSMASKIDX(i)]
#define  OSBITSET(buf, i)    OSMASKWORD(buf, i) |= OSBITMASK(i)
#define  OSBITCLEAR(buf,i)   OSMASKWORD(buf, i) &= ~OSBITMASK(i)
#define  OSGETBIT(buf, i)    (OSMASKWORD(buf, i) & OSBITMASK(i))

#if (MaskCount == 1)
#define  OSCOPYBITS(src, dst)           dst[0] = src[0]
#define  OSCLEARBITS(buf)               buf[0] = 0
#define  OSMASKANDSETBITS(dst, b1, b2)  dst[0] = (b1[0] & b2[0])
#define  OSORBITS(dst, b1, b2)          dst[0] = (b1[0] | b2[0])
#define  OSUNSETBITS(dst, b1)           (dst[0] &= ~b1[0])
#define  OSANYSET(src)                  (src[0])
#endif

#if (MaskCount == 2)
#define  OSCOPYBITS(src, dst)           dst[0] = src[0]; dst[1] = src[1]
#define  OSCLEARBITS(buf)               buf[0] = 0;  buf[1] = 0
#define  OSMASKANDSETBITS(dst, b1, b2)  dst[0] = (b1[0] & b2[0]);\
                                      dst[1] = (b1[1] & b2[1])
#define  OSORBITS(dst, b1, b2)          dst[0] = (b1[0] | b2[0]);\
                                      dst[1] = (b1[1] | b2[1])
#define  OSUNSETBITS(dst, b1)           dst[0] &= ~b1[0]; dst[1] &= ~b1[1]
#define   OSANYSET(src)                 (src[0] || src[1])
#endif

#if (MaskCount == 3)
#define  OSCOPYBITS(src, dst)           dst[0] = src[0]; dst[1] = src[1];\
                                      dst[2] = src[2]
#define  OSCLEARBITS(buf)               buf[0] = 0;  buf[1] = 0; buf[2] = 0
#define  OSMASKANDSETBITS(dst, b1, b2)  dst[0] = (b1[0] & b2[0]);\
                                      dst[1] = (b1[1] & b2[1]);\
                                      dst[2] = (b1[2] & b2[2])
#define  OSORBITS(dst, b1, b2)          dst[0] = (b1[0] | b2[0]);\
                                      dst[1] = (b1[1] | b2[1]);\
                                      dst[2] = (b1[2] | b2[2])
#define  OSUNSETBITS(dst, b1)           dst[0] &= ~b1[0]; dst[1] &= ~b1[1];\
                                      dst[2] &= ~b1[2]
#define   OSANYSET(src)                 (src[0] || src[1] || src[2])
#endif

#if (MaskCount == 4)
#define  OSCOPYBITS(src, dst)           dst[0] = src[0]; dst[1] = src[1];\
                                      dst[2] = src[2]; dst[3] = src[3]
#define  OSCLEARBITS(buf)               buf[0] = 0;  buf[1] = 0;\
                                      buf[2] = 0;  buf[3] = 0
#define  OSMASKANDSETBITS(dst, b1, b2)  dst[0] = (b1[0] & b2[0]);\
                                      dst[1] = (b1[1] & b2[1]);\
                                      dst[2] = (b1[2] & b2[2]);\
                                      dst[3] = (b1[3] & b2[3])
#define  OSORBITS(dst, b1, b2)          dst[0] = (b1[0] | b2[0]);\
                                      dst[1] = (b1[1] | b2[1]);\
                                      dst[2] = (b1[2] | b2[2]);\
                                      dst[3] = (b1[3] | b2[3])
#define  OSUNSETBITS(dst, b1)           dst[0] &= ~b1[0]; dst[1] &= ~b1[1];\
                                      dst[2] &= ~b1[2]; dst[3] &= ~b1[3]
#define  OSANYSET(src)                 (src[0] || src[1] || src[2] || src[3])
#endif

#if (MaskCount > 4)
#define  OSCOPYBITS(src, dst)  memcpy ((char *)(dst), (char *)(src),\
                                         MaskCount * sizeof(LONG))
#define  OSCLEARBITS(buf)      memset((char *)(buf), (int)0,  MaskCount * sizeof(LONG))
#define  OSMASKANDSETBITS(dst, b1, b2)\
                { int i;\
                  for (i = 0; i < MaskCount; i++)\
                     dst[i] = (b1[i] & b2[i]);}
#define  OSORBITS(dst, b1, b2)\
                { int i;\
                  for (i = 0; i < MaskCount; i++)\
                     dst[i] = (b1[i] | b2[i]);}
#define  OSUNSETBITS(dst, b1)\
                { int i;\
                  for (i = 0; i < MaskCount; i++)\
                  dst[i] &= ~b1[i];}

#ifdef __cplusplus
extern "C" {
#endif
    extern BOOL OSANYSET (long* src);
#ifdef __cplusplus
}
#endif


#endif

/* Copy in BigEndian */

#define OSGetWord(s, cp) { \
	BYTE *t_cp = (BYTE *)(cp); \
	(s) = ((WORD)t_cp[0] << 8) \
	    | ((WORD)t_cp[1]) \
	    ; \
	(cp) += 2; \
}

#define OSGetDword(l, cp) { \
	BYTE *t_cp = (BYTE *)(cp); \
	(l) = ((DWORD)t_cp[0] << 24) \
	    | ((DWORD)t_cp[1] << 16) \
	    | ((DWORD)t_cp[2] << 8) \
	    | ((DWORD)t_cp[3]) \
	    ; \
	(cp) += 4; \
}

#define OSPutWord(s, cp) { \
	WORD t_s = (WORD)(s); \
	BYTE *t_cp = (BYTE *)(cp); \
	*t_cp++ = (char) (t_s >> 8); \
	*t_cp   = (char) t_s; \
	(cp) += 2; \
}

#define OSPutDword(l, cp) { \
	DWORD t_l = (DWORD)(l); \
	BYTE* t_cp = (BYTE *)(cp); \
	*t_cp++ = (char)(t_l >> 24); \
	*t_cp++ = (char)(t_l >> 16); \
	*t_cp++ = (char)(t_l >> 8); \
	*t_cp   = (char)(t_l); \
	(cp) += 4; \
}


typedef struct _OS
{
    LONG NbrOpenedSockets;
    LONG NbrClosedSockets;
    int MaxSocketNumber;
    int FDNEXT;
    int FDTAB[SOCKMAX+1];
}OS;


#ifdef __cplusplus
extern "C" {
#endif

    extern int  FDTRANSLATE(OS* pos, int socket);
    extern int  OSFFS (long* buf);

    extern int  OSInit(OS* pos);
    extern void OSEnd (OS* pos);
    extern void OSSleep (int time);
    extern int  OSSetSockOpt (OS* pos, int s, int level, int optname, void* optval, int optlen);
    extern int  OSRead       (OS* pos, int s, char* buf, int len);
    extern int  OSWrite      (OS* pos, int s, char* buf, int len);
    extern int  OSCloseSocket(OS* pos, int s);
    extern int  OSGiveSocket (OS* pos, int s, char* name);
    extern int  OSTakeSocket (OS* pos, int s, char* name, char* subtaskname);
    extern int  OSAccept     (OS* pos, int s);
    extern int  OSOpenTcpSocket   (OS* pos, WORD port);
    extern int  OSOpenInetSocket  (OS* pos, int type, BOOL Block);
    extern int  OSOpenLocalSocket (OS* pos, char* name, char* appliname);
    extern int  OSTcpConnect      (OS* pos, char* machinname, WORD port, BOOL Block, BOOL* ConnectOK);
    extern int  OSLocalConnect    (OS* pos, char* name, char* appliname);
    extern int  OSSelect (OS* pos, int nfds, long* readmsk, long* writemsk,
                              long* exceptmsk, struct timeval* timeout);
    extern int  OSIoctl       (OS* pos, int s, LONG cmd, void* data);
    extern int  OSGetPeerName (OS* pos, int s, int type, char* userid, char* appliname);
    extern int  OSGetSockName (OS* pos, int s, char* netid);
    extern int  OSGetHostName (char* Name, int NameLength);


    /* IP functions*/

    extern int  OSSend   (OS* pos, int s, char* buf, int len, char* MachinName);
    extern int  OSRecv   (OS* pos, int s, char* buf, int len, char* MachinName);

    extern int  OSGetNameFromAddr (char* AddrName, char* Name);
    extern int  OSGetAddrFromName (char* Name, char* AddrName);
    extern WORD OSCheckSum (WORD* ptr, int nbytes);
    extern void OSCopy2 (void* word, char* stream);
    extern void OSCopy4 (void* dword, char* stream);
    extern void OSCopy8 (void* ddouble, char* stream);


#ifdef __cplusplus
}
#endif

#endif /* __OSCOM_H__ */

