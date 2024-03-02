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


#ifndef __OSDRAW_H__
#define __OSDRAW_H__         

#include "ostypes.h"     


#define OSBLACKBRUSH  BLACK_BRUSH
#define OSMAXFONTS    10
#define OSFONT8       0
#define OSFONT10      1
#define OSFONT12      2
#define OSFONT14      3
#define OSFONT16      4
               

#define OSBOLDFONT8   5
#define OSBOLDFONT10  6
#define OSBOLDFONT12  7 
#define OSBOLDFONT14  8
#define OSBOLDFONT16  9
#define OSBOLD        10




extern HFONT OSAllocatedFonts[OSMAXFONTS];


typedef struct  _OSGC
{
  int GC;
}OSGC;


typedef struct _OSDC
{
  void* DC;
}OSDC;


typedef struct _OSCRect
{
  LONG 	  Left;
  LONG	  Top;
  LONG	  Right;
  LONG	  Bottom;
}OSRect;



#ifdef __cplusplus
extern "C" {
#endif 

extern void OSReturnFont (LONG dc, int Font);
extern void OSDrawLine (int first_x,int first_y,int last_x,int last_y, LONG dc);
extern void OSDrawEllipse (OSRect* rect, LONG dc);
extern void OSDrawRectangle (OSRect* rect, LONG dc);
extern void OSFillRectangle (OSRect* rect, LONG dc, DWORD pattern);
extern void OSInvertRectangle (OSRect* prect, LONG dc);
extern void OSDrawText (char* s, int Font, LONG dc, OSRect* rect, int format);
extern void OSGetWindowSize (HWND win,  OSRect* rect);
extern void OSGetTextExtent (char* text, int Font, LONG dc, LONG* width, LONG* height);
extern void OSDrawInit (void);
extern void OSDrawEnd (void);
		
#ifdef __cplusplus
}
#endif

#endif
