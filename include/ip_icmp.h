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



#ifndef IP_ICMPHEADER
#define IP_ICMPHEADER

struct icmp
{
    BYTE   icmp_type;
    BYTE   icmp_code;
    WORD   icmp_cksum;
    WORD   icmp_id;
    WORD   icmp_seq;
    char   icmp_data[1];
};

#define SIZE_ICMP_HDR   8
#define SIZE_TIME_DATA  8


#ifdef WINDOWS
struct ip
{
BYTE   
l:4,                /* header length */
ip_v:4;                 /* version */
    BYTE   ip_tos;                 /* type of service */
    short  ip_len;                 /* total length */
    WORD   ip_id;                  /* identification */
    short  ip_off;                 /* fragment offset field */
    BYTE   ip_ttl;                 /* time to live */
    BYTE   ip_p;                   /* protocol */
    WORD   ip_sum;                 /* checksum */
    struct in_addr ip_src,ip_dst;  /* source and dest address */
};
#endif

#define   ICMP_ECHOREPLY          0    /* echo reply */
#define   ICMP_UNREACH            3    /* dest unreachable, codes: */
#define   ICMP_UNREACH_NET        0    /* bad net */
#define   ICMP_UNREACH_HOST       1    /* bad host */
#define   ICMP_UNREACH_PROTOCOL   2    /* bad protocol */
#define   ICMP_UNREACH_PORT       3    /* bad port */
#define   ICMP_UNREACH_NEEDFRAG   4    /* IP_DF caused drop */
#define   ICMP_UNREACH_SRCFAIL    5    /* src route failed */
#define   ICMP_SOURCEQUENCH       4    /* packet lost, slow down */
#define   ICMP_REDIRECT           5    /* shorter route, codes: */
#define   ICMP_REDIRECT_NET       0    /* for network */
#define   ICMP_REDIRECT_HOST      1    /* for host */
#define   ICMP_REDIRECT_TOSNET    2    /* for tos and net */
#define   ICMP_REDIRECT_TOSHOST   3    /* for tos and host */
#define   ICMP_ECHO               8    /* echo service */
#define   ICMP_TIMXCEED           11   /* time exceeded, code: */
#define   ICMP_TIMXCEED_INTRANS   0    /* ttl==0 in transit */
#define   ICMP_TIMXCEED_REASS     1    /* ttl==0 in reass */
#define   ICMP_PARAMPROB          12   /* ip header bad */
#define   ICMP_TSTAMP             13   /* timestamp request */
#define   ICMP_TSTAMPREPLY        14   /* timestamp reply */
#define   ICMP_IREQ               15   /* information request */
#define   ICMP_IREQREPLY          16   /* information reply */
#define   ICMP_MASKREQ            17   /* address mask request */
#define   ICMP_MASKREPLY          18   /* address mask reply */

#define ICMP_MAXTYPE              18

#define ICMP_MINLEN     8                               /* abs minimum */
#define ICMP_TSLEN      (8 + 3 * sizeof (n_time))       /* timestamp */
#define ICMP_MASKLEN    12                              /* address mask */
#define ICMP_ADVLENMIN  (8 + sizeof (struct ip) + 8)    /* min */
#define ICMP_ADVLEN(p)  (8 + ((p)->icmp_ip.ip_hl << 2) + 8)

#define STNORM          0

/* Definition of the lowest telnet byte following an IAC byte */
#define LOW_TEL_OPT 236

#define TEL_EOF     236
#define SUSP        237
#define ABORT       238

#define SE          240
#define NOP         241
#define DM          242
#define BREAK       243
#define IP          244
#define AO          245
#define AYT         246
#define EC          247
#define EL          248
#define GOAHEAD     249
#define SB          250
#define WILL        251
#define WONT        252
#define DO          253
#define DONT        254
#define IAC         255

#endif

