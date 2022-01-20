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

#ifndef __SCYACC__H__
#define __SCYACC__H__

typedef union
{
    int    val_i;
    long   val_l;
    double val_f;
    char*  str;
    struct _SCEntity*  pentity;
    struct _SCSection* psection;
} YYSTYPE;
extern YYSTYPE yylval;
extern int yylineno;
# define SCINTEGER 257
# define SCLONG 258
# define SCFLOAT 259
# define SCBOOLEAN 260
# define SCSTRING 261
# define SCVARIABLE 262
# define SCOPERATOR 263
# define SCOPERATORDEFUN 264
# define SCOPERATORREMOTE 265
# define SCCHAR 266
# define SCTIME 267
# define SCDATE 268
# define SCNULL 269
# define PAR_DEB 270
# define PAR_FIN 271
# define ERR_CHAR 272
# define ERR_INT 273
# define ERR_LONG 274
# define ERR_FLOAT 275
# define ERR_TIME 276
# define ERR_DATE 277
# define ERR_SYNTAX 278
# define ERR_ALLOC 279
# define ERR_NOTFOUND 280

#endif
