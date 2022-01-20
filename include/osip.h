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

#ifndef __OSIP_H__
#define __OSIP_H__

#define IPCOM
#define ICMPCOM

#ifdef _VM_
 #define IUCVCONN  1
#endif

#ifdef UNIX
#ifndef SCO 
 #define UNIXCONN  1
#endif
#endif


#ifdef WINDOWS
  #include <winsock.h>
#elif defined(OS2)
 #define BSD_SELECT
 #include <types.h>
 #include <sys/socket.h>
 #include <sys/ioctl.h>
 #include <sys/select.h>
 #include <netinet/in.h>
 #include <netdb.h>
#endif 

#ifdef _VM_
 #ifndef XMD_H
 #include <bsdtypes.h>
 #include <socket.h>
 #include <saiucv.h>
 #include <ioctl.h>
 #include <in.h>
 #include <manifest.h>
 #include <bsdtime.h>
 #include <errno.h>
 #include <tcperrno.h>
 #include <netdb.h>
 #include <inet.h>
#endif
#endif

#ifdef __VOS__
 #include <tcp_types.h>
 #include <socket.h>
 #include <tcp_socket.h>
 #include <ioctl.h>
 #include <ifreq.h>
 #include <in.h>
 #include <uio.h>
 #include <tcp_errno.h>
 #include <nameser.h>
 #include <netdb.h>
 #include <resolv.h>
#endif

#ifdef UNIX
#include <sys/types.h>
#include <sys/socket.h>
#ifdef UNIXCONN
#include <sys/un.h>
#endif
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#ifndef HP_UX
#include <sys/select.h>
#endif
#include <netdb.h>
#include <sys/ioctl.h>
#include <errno.h>
#endif


#ifdef WINDOWS
 #include "ip_icmp.h"
#elif defined(OS2)
#endif 
#ifdef _VM_
 #include "ip_icmp.h"
#endif

#ifdef __VOS__
 #include "ip_icmp.h"
#endif



#ifdef UNIX
 #include <netinet/in_systm.h>
 #include <netinet/ip.h>
 #include <netinet/ip_icmp.h>
#endif


#ifdef AS400
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <iconv.h> 
#include <qtqiconv.h>


#endif

#endif          

