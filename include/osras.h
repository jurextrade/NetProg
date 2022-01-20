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

#ifndef __OSRAS_H__
#define __OSRAS_H__

/* You must link RASAPI32.LIB to use RAS under windows */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ostypes.h"


#ifdef _WIN32
#include <ras.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

    extern DWORD OSRasDial(char *lpszEntryName, char *lpszUserName, char *lpszPassword);
    extern DWORD OSRasHangUp(void);
    extern DWORD OSRasGetConnectStatus(void);
    extern DWORD OSRasGetConnectHandle(char *lpszEntryName);
    extern DWORD OSRasError(DWORD rc, char* Buf, WORD size);

#ifdef __cplusplus
}
#endif

#endif

