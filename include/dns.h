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

#ifndef __DNS_H__
#define __DNS_H__


#include "mx.h"

#define DNSMessageHeaderSize     12   /* #/bytes of fixed data in header   */
#define DNSMessageQuerySize      4    /* #/bytes of fixed data in query    */
#define DNSMessageRecordSize     10   /* #/bytes of fixed data in r record */

#define  NOERROR        0       /* no error */
#define  SUCCESS		0
#define  TIME_OUT		-1
#define  NO_INFO		-2
#define  NONAUTH		-4
#define  NO_RESPONSE	-5
#define  DNS_IGNORE      -6

#ifdef ERROR
#undef ERROR
#endif
#define  ERROR			-3


#define INADDRSZ	4		/* IPv4 T_A */
#define IN6ADDRSZ	16		/* IPv6 T_AAAA */



/*
 * Type values for resources and queries
 */
#define T_A         1        /* host address                 */
#define T_NS        2        /* authoritative server         */
#define T_MD        3        /* mail destination             */
#define T_MF        4        /* mail forwarder               */
#define T_CNAME     5        /* canonical name               */
#define T_SOA       6        /* start of authority zone      */
#define T_MB        7        /* mailbox domain name          */
#define T_MG        8        /* mail group member            */
#define T_MR        9        /* mail rename name             */
#define T_NULL     10        /* null resource record         */
#define T_WKS      11        /* well known service           */
#define T_PTR      12        /* domain name pointer          */
#define T_HINFO    13        /* host information             */
#define T_MINFO    14        /* mailbox information          */
#define T_MX       15        /* mail routing information     */
#define T_TXT      16        /* text strings                 */
#define T_RP       17        /* responsible person           */
#define T_AFSDB    18        /* AFS cell database            */
#define T_X25      19        /* X_25 calling address         */
#define T_ISDN     20        /* ISDN calling address         */
#define T_RT       21        /* router                       */
#define T_NSAP     22        /* NSAP address                 */
#define T_NSAP_PTR 23        /* reverse NSAP lookup (deprecated) */
#define T_SIG      24        /* security signature           */
#define T_KEY      25        /* security key                 */
#define T_PX       26        /* X.400 mail mapping           */
#define T_GPOS     27        /* geographical position (withdrawn) */
#define T_AAAA     28        /* IP6 Address                  */
#define T_LOC      29        /* Location Information         */
#define T_NXT      30        /* Next Valid Name in Zone      */
#define T_EID      31        /* Endpoint identifier          */
#define T_NIMLOC   32        /* Nimrod locator               */
#define T_SRV      33        /* Server selection             */
#define T_ATMA     34        /* ATM Address                  */
#define T_NAPTR    35        /* Naming Authority PoinTeR     */
/* non standard */
#define T_UINFO    100        /* user (finger) information   */
#define T_UID      101        /* user ID                     */
#define T_GID      102        /* group ID                    */
#define T_UNSPEC   103        /* Unspecified format (binary data) */


/* Query type values which do not appear in resource records */
#define T_IXFR     251        /* incremental zone transfer   */
#define T_AXFR     252        /* transfer zone of authority  */
#define T_MAILB    253        /* transfer mailbox records    */
#define T_MAILA    254        /* transfer mail agent records */
#define T_ANY      255        /* wildcard match              */

#define C_IN		1		/* the arpa internet */
#define C_CHAOS		3		/* for chaos net (MIT) */
#define C_HS		4		/* for Hesiod name server (MIT) (XXX) */
/* Query class values which do not appear in resource records */
#define C_ANY		255		/* wildcard match */




/*
 * Currently defined opcodes
 */
#define QUERY           0x0 /* standard query */
#define IQUERY          0x1 /* inverse query */
#define STATUS          0x2 /* nameserver status query */
#define NS_NOTIFY_OP    0x4 /* notify secondary of SOA change */




typedef struct _DNSMessageHeader
{
unsigned    id:16;               /* query identification number   */
#ifdef HIGH_LOW
    /* fields in third byte          */
unsigned   qr:1;                /* response flag                 */
unsigned  opcode:4;          /* purpose of message            */
unsigned  aa:1;                /* authoritive answer            */
unsigned  tc:1;                /* truncated message             */
unsigned  rd:1;                /* recursion desired             */
    /* fields in fourth byte         */
unsigned  ra:1;                /* recursion available           */
unsigned  unused:1;            /* unused bits MBZ as of 4.9.3a3)*/
unsigned  ad:1;                /* authentic data from named     */
unsigned  cd:1;                /* checking disabled by resolver */
unsigned  rcode :4;            /* response code                 */
#endif
#ifdef LOW_HIGH
    /* fields in third byte          */
unsigned   rd:1;                /* recursion desired             */
unsigned   tc:1;                /* truncated message             */
unsigned   aa:1;                /* authoritive answer            */
unsigned   opcode:4;          /* purpose of message            */
unsigned   qr:1;                /* response flag                 */
    /* fields in fourth byte         */
unsigned   rcode:4;           /* response code                 */
unsigned   cd:1;                /* checking disabled by resolver */
unsigned   ad:1;                /* authentic data from named     */
unsigned   unused:1;            /* unused bits MBZ as of 4.9.3a3)*/
unsigned   ra:1;                /* recursion available           */
#endif
unsigned  qdcount:16;           /* number of question entries    */
unsigned  ancount:16;           /* number of answer entries      */
unsigned  nscount:16;           /* number of authority entries   */
unsigned  arcount:16;           /* number of resource entries    */
}DNSMessageHeader;



typedef struct _DNSResourceSymbols
{
    int     Code;
    char*   Name;
    char*   HumanName;
}DNSResourceSymbols;





#ifdef __cplusplus
extern "C" {
#endif

    extern DNSResourceSymbols DNSClassSyms[];
    extern DNSResourceSymbols DNSTypeSyms[];


    extern char*        Print_cdname_sub (char* cp, char* msg, char* eom, char* value, int format);
    extern char*        Print_cdname (char* cp, char* msg, char* eom, char* value);
    extern char*        Print_cdname2 (char* cp, char* msg, char* eom, char* value);

    extern char*        DNSCodeToName (DNSResourceSymbols* syms, int code, int* success);
    extern char*        DNSCodeToHumanName (DNSResourceSymbols *syms, int code, int* success);
    extern char*        DNSGetNameFromType(int type);
    extern char*        DNSGetNameFromClass(int Class);



    extern int          OSStripDomain (char* string, char* domain);
    extern int          OSGetDomainName (char* string, char* name);

    extern int          DNSComp(char* src, char* dst, int dstsiz, char** dnptrs, char** lastdnptr);
    extern int          DNSExpand(char *msg, char *eom, char *src,char *dst, int dstsiz);
    extern int          DNSSkipName(char* ptr, char* eom);

    extern LONG         DNSSend (MX* pmx, MXCom* pcom, MXMessage* pmessage);
    extern LONG         DNSWriteMessage (MX* pmx, MXMessage* pmessage, char* stream, int maxcount, BYTE WithFileContent);
    extern LONG         DNSSendMessage (TP* ptp, TPClient* pclient, char* buf, int size);
    extern LONG         DNSReceiveMessage (TP* ptp, TPClient* pclient, char* buf, int count);
    extern LONG         DNSReadMessage (MX* pmx, MXMessage* pmessage, char* stream, int streamsize, BYTE WithFileContent);
    extern MXMessage*   DNSCheckAndReturnMessage (MX* pmx, MXCom* pcom, char* stream);


#ifdef __cplusplus
}
#endif

#endif
