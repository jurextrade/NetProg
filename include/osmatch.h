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

#ifndef __OSMATCH_H__
#define __OSMATCH_H__


#include "oslist.h"
#include "osio.h"


#define MATCH_PATTERN  6    /* bad pattern */
#define MATCH_LITERAL  5    /* match failure on literal match */
#define MATCH_RANGE    4    /* match failure on [..] construct */
#define MATCH_ABORT    3    /* premature end of text string */
#define MATCH_END      2    /* premature end of pattern string */
#define MATCH_VALID    1    /* valid match */

#define PATTERN_VALID  0    /* valid pattern */
#define PATTERN_ESC   -1    /* literal escape at end of pattern */
#define PATTERN_RANGE -2    /* malformed range in [..] construct */
#define PATTERN_CLOSE -3    /* no end bracket in [..] construct */
#define PATTERN_EMPTY -4    /* [..] contstruct is empty */




#ifdef __cplusplus
extern "C" {
#endif


    extern BOOL  OSSimpleMatch (char* String, char* Pattern);
    extern BOOL  OSMatch(char* String, char* Pattern);
    extern int OSGetFilesFromWildCards (char* AllFileName, List** list, char FileLoc, BOOL WithReper);
    extern int OSReplaceInFile (char* FileName, char* string_orig, char* string_new, char mode, char loc, int* Error);
    extern int OSReplace (char* Name, char* string_orig, char* string_new, char FileLoc, char mode, int recurse, int* Error);
    extern int OSFindString (char* Content, int Size, char* string_orig, int occurence);
    extern int OSExecuteInFile (char* FileName, char* string_orig, char mode, char loc,
                                    int (*funct)(char* file, char* line, int line_pos, int pos, void* appfield), void* appfield, int* Error);
    extern int OSSearchAndExecute (char* Name, char* string_orig, char FileLoc, char mode,
                                       int (*funct)(char* file, char* line, int line_pos, int pos, void* appfield), void* appfield, int* Error);

#ifdef __cplusplus
}
#endif


#endif
