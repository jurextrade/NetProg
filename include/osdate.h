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


#ifndef __OSDATE_H__
#define __OSDATE_H__

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAXDAYS         488 /*(365 + 122 = sept + oct + nov + dec = 487 + 1)*/
#define MAXMONTHS       16  /*(12 + 4 = sept, ...)*/





extern char* Date[];
extern char* Day[];
extern int NbDays[];



extern int ActualYear;
extern int ActualMonth;
extern int ActualDay;

extern int ActualMonthDay;
extern int ActualWeekDay;      /* day of the week 0 = Dimanche ... 6 */
extern int ActualYearDay;  /* day in the current year from 1 */





extern int BeginDay;
extern int InDay;
extern int InMonth;

extern long ActualDate, ActualHour;


extern char ActualStrDate[200];
extern char ActualStrHour[200];
extern char ActualStrMonth[5];
extern char ActualStrWeek[10];
extern char ActualStrDay[10];




#ifdef __cplusplus
extern "C" {
#endif


    extern int OSGetDayFromDate (int year, int month,int day);
    extern int OSGetMonthFromDate (int year, int month);
    extern int OSGetDateTime (char* SDate, char* STime);
    extern char* OSFormatDate(long date, char* datcha, int WithSep);
    extern char* OSFormatHour(long hour, char* hourcha, int WithSep);
    extern void OSGetDateHour(long* dat, long* hour);

#ifdef __cplusplus
}
#endif



#endif
