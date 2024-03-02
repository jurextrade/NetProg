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

#ifndef __osresyac_h__
#define __osresyac_h__

typedef union
{
    int Valint;
    char *Valvar;
    char *Valtyp;
} ZZSTYPE;


extern ZZSTYPE zzlval;
# define DIGIT 257
# define DIMDIGIT 258
# define VAR 259
# define LOCALVAR 260
# define BIGVAR 261
# define NFILE 262
# define IDP 263
# define LINEHEX 264
# define COMMENT 265
# define ANUM 266
# define PGCONTEXT 267
# define TYPE 268
# define RESTYPE 269
# define PFILETYPE 270
# define EVENT 271
# define EVENTSR 272
# define RESSYSTEM 273
# define YESNO 274
# define PASS 275
# define SITE 276
# define MACHINE 277
# define LA 278
# define APPLICATIONCLASS 279
# define APPLICATION 280
# define CONNECTIONCLASS 281
# define CONNECTION 282
# define PROPERTIES 283
# define DEB 284
# define FIN 285
# define DATABASECLASS 286
# define DATABASE 287
# define JOURNALCLASS 288
# define JOURNAL 289
# define QUEUECLASS 290
# define QUEUE 291
# define SCENARIO 292
# define LENGTH 293
# define MESSAGE 294
# define MESSAGECLASS 295
# define OUVRE 296
# define FERME 297
# define FROM 298
# define TO 299
# define PORT 300
# define TABLE 301
# define PROTOCOL 302
# define TABXLT 303
# define STRFILES 304
# define TRANSFERT 305
# define COLOR 306
# define IPADDRESS 307
# define IPNAME 308
# define DESCRIPTION 309
# define APPTYPE 310
# define SYSTEM 311
# define PFILE 312
# define PPROCEDURE 313
# define PEQUAL 314
# define DIALOGCLASS 315
# define XLTCLASS 316
# define CHARERROR 317
# define MAXREACH 318
# define ONMACHINE 319
# define USERNAME 320
# define BASENAME 321
# define PASSWORD 322
# define FHOSTNAME 323
# define FUSERNAME 324
# define FPASSWORD 325
# define FPORT 326
# define LOGONTYPE 327
# define TRANSTYPE 328
# define REPERTORY 329
# define CONNECTED 330
# define REMOTE 331
# define SUBTYPE 332
# define CLASS 333
# define MODE 334
# define LOCALISATION 335
# define EXECREPERTORY 336
# define LOCALREPERTORY 337
# define RECEIVEREPERTORY 338
# define SENDREPERTORY 339
# define EXECCOMMAND 340
# define EXECPARAMETERS 341
# define FOLDER 342
# define EXTENSION 343
#endif
