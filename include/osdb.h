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

#ifndef __OSDB_H__
#define __OSDB_H__

#include <setjmp.h>
#include "osio.h"
#include "oslist.h"
#include "ostrace.h"

#ifdef WINDOWS
 #define ODBC                   1 
#endif

#define DBPROTO_ODBC            1021               
#define DBPROTO_ORACLE          1025               
#define DBPROTO_SQLDS           1053               


#define DBSQLUNDEFINED         0
#define DBSQLPROCESS           1              /* SELECT                   */
#define DBSQLEXECUTE           2              /* UPDATE, INSERT or DELETE */
#define DBSQLSHOW              3              /* SHOW                     */
#define DBSQLCONNECT           4              /* CONNECT                  */
#define DBSQLDISCONNECT        5              
#define DBSQLCOMMIT            6

#define OSREADINIT             0
#define OSREADHEADER           1
#define OSREADCONTENT          2
#define OSREADEND              4
#define OSREADACK              3

#define MAXITEMS       200

#define FILEQUERY      'F'
#define BUFFERQUERY    'B'

#define OSSetHeaderProcedure(pt, f)       (pt)->HeaderProcedure   =    (f)
#define OSSetContentsProcedure(pt, f)     (pt)->ContentsProcedure =    (f)
#define OSSetProcedurePar(pt, par)        (pt)->ProcedurePar      =    (par)
#define OSSetAckProcedure(pt, f)          (pt)->AckProcedure      =    (f)


#define OSDEFAULTEVERY         100




typedef struct _OSSqlvar
{
    BYTE        IsNull;                      /* is de colonne Null                 */
    SHORT       Type;                        /* type de la select list sur 3 char  */
    LONG        Length;                      /* colon width                        */
    LONG        Scale;                       /* Item scale                         */
    LONG        ItemLength;                  /* Item colon length                  */
    CHAR*       Name;                        /* colon name                         */
}OSSqlvar;


typedef struct _OSContext
{
    BYTE        StorageType;                  /* file or buffer                    */
    BOOL        Compact;
    IDENTFICH   FileIdent;
    DWORD       Size;
    char*       Values;
}OSContext;


typedef struct _OSSqlda
{
    LONG        ColumnsNumber;              /* actual number of select_list items */
    OSSqlvar**  Description;
    LONG        RowsNumber;                 /* number max of select_list or place-holders*/
    WORD        ContextNumber;              /* number of contents to send */
    OSContext** Context;
    BYTE        LastContext;
    int         (*HeaderProcedure)   (void* psqlda, void* papp);
    int         (*ContentsProcedure) (void* psqlda, void* papp);
    void*       ProcedurePar;
    void*       S_sqlda;                   /* sauvegarde pointeur sqlda oracle, sqlds...*/
    int         size;                      /* line length                               */
    int         Protocol;
}OSSqlda;



typedef struct _OSAck
{
    LONG        SqlCode;
    char        SqlErrMsg[500];
    LONG        RowsProcessed ;
    SHORT       Print;
    SHORT       Stop;
    int         (*AckProcedure) (void* pack, void* papp);
    void*       ProcedurePar;
    void*       Connection;
}OSAck;



/* structure identifiant les char * de retour d'un SHOW */
typedef struct _OSShow
{
    WORD        ContextNumber;              /* number of contents to send */
    OSContext** Context;
    BYTE        LastContext;
    int         (*ContentsProcedure) (void* pshow, void* papp);
    void*       ProcedurePar;
}OSShow;

typedef struct _OSQuery
{
    DWORD     Size;                         /* size of the whole message          */
    DWORD     Every;                        /* number of rows processed           */
    DWORD     MaxRows;                      /* number of rows processed           */
    WORD      ContextNumber;                /* number of contents to send         */
    int       (*HeaderProcedure)   (void* psqlda, void* papp);
    int       (*ContentsProcedure) (void* psqlda, void* papp);
    void*     ProcedurePar;
    OSSqlda*  Contents;
}OSQuery;

/* structure identifiant une requête SQL */


typedef struct _OSStatement
{
    char      Name[300];
    char*     SqlStatement;
    char*     IdentStatic;
    BYTE      SqlType;
    BYTE      Commit;
    BYTE      Status;
    LONG      Identity;
    OSQuery*  Query;
    OSAck*    Ack;
    int       (*AckProcedure) (void* pack, void* papp);
    void*     ProcedurePar;
    void*     UserField;
    void*     MessageClass;
}OSStatement;




#ifdef __cplusplus
extern "C" {
#endif

    extern OSStatement* OSCreateStatement (char* IdentStatic, char* SqlStatement, BYTE Commit);
    extern OSQuery*     OSCreateQuery (DWORD Every, DWORD MaxRows);
    extern void         OSFreeQuery (OSQuery* pquery);
    extern void         OSFreeStatement (OSStatement* pstatement);
    extern OSAck*    OSMallocAck  (void);
    extern OSShow*   OSMallocShow (void);
    extern void      OSFreeShow   (OSShow* pshow);
    extern int       OSAddNewContextToShow (OSShow* pshow);
    extern OSSqlda*  OSMallocSqlda (int NumberItems);
    extern void      OSFreeSqlda   (OSSqlda* psqlda);
    extern int       OSAddNewContextToSqlda (OSSqlda* psqlda, DWORD Every);
    extern int       OSReuseSameContext (OSSqlda* psqlda);
    extern int       OSDisconnect (void* Connection, int protocol, BYTE Commit,
                                       int (*AckProcedure)(void* pack, void* papp),
                                       void* AckProcedurePar);
    extern int       OSCommitOrRollback (void* Connection, int protocol, BYTE Action,
                                             int (*AckProcedure)(void* pack, void* papp),
                                             void* AckProcedurePar);
    extern int       OSConnect (char* DataBaseName, char* UserName, char* PassWord, int protocol,
                                    int (*AckProcedure)(void* pack, void* papp),
                                    void* AckProcedurePar);
    extern int       OSExecute (void* Connection, int protocol, char* SqlStatement, BYTE Commit,
                                    int (*AckProcedure)(void* pack, void* papp),
                                    void* AckProcedurePar);
    extern int       OSSExecute(void* Connection, int protocol, char* SqlStatement, BYTE Commit,
                                    int (*AckProcedure)(void* pack, void* papp),
                                    void* AckProcedurePar,void* content, DWORD ContentSize);
    extern OSSqlda*  OSProcess (void* Connection, int protocol, char* SqlStatement, 
                                    DWORD Every, DWORD MaxRows,
                                    int (*HeaderProcedure)   (void* psqlda, void* papp),
                                    int (*ContentsProcedure) (void* psqlda, void* papp),
                                    void* ProcedurePar,
                                    int (*AckProcedure)(void* pack, void* papp),
                                    void* AckProcedurePar);
    extern int       OSFetch   (OSSqlda* psqlda, int protocol, 
                                    DWORD Every, DWORD MaxRows, DWORD RowsToProcess,
                                    int (*AckProcedure)(void* pack, void* papp),
                                    void* AckProcedurePar);
    extern OSShow*   OSRecupShow  (void* Connection, int protocol, char* SqlStatement, 
                                       int (*ContentsProcedure) (void* pshow, void* papp),
                                       void* ProcedurePar,
                                       int (*AckProcedure)(void* pack, void* papp),
                                       void* AckProcedurePar);
    /*
    extern DWORD OSWriteSqldaHeader (OSSqlda* psqlda, char* Buffer);
    extern OSSqlda* OSReadSqldaHeader (char* Buffer);
    extern void OSTraceSqlda (OSSqlda* psqlda); */

    extern int       OSDefaultAckProc (void* ack, void* par);
    extern int       OSDefaultContentsShowProc  (void* show, void* par);
    extern int       OSDefaultHeaderSqldaProc   (void* sqlda, void* par);
    extern int       OSDefaultContentsSqldaProc (void* sqlda, void* par);

#ifdef ORACLE
    extern OSSqlda* oracle_process (char* SqlStatement, 
                                        DWORD Every,
                                        DWORD MaxRows,
                                        int (*HeaderProcedure)   (void* psqlda, void* papp),
                                        int (*ContentsProcedure) (void* psqlda, void* papp),
                                        void* ProcedurePar,
                                        int (*AckProcedure)(void* pack, void* papp),
                                        void* AckProcedurePar);
    extern void oracle_ack (OSAck* pack);
    extern int  oracle_disconnect (BYTE Commit,
                                       int (*AckProcedure)(void* pack, void* papp),
                                       void* AckProcedurePar);
    extern int  oracle_connect (char* DataBaseName, char* UserName, char* PassWord,
                                    int (*AckProcedure)(void* pack, void* papp),
                                    void* AckProcedurePar);
    extern int  oracle_execute (char* SqlStatement, BYTE Commit,
                                    int (*AckProcedure)(void* pack, void* papp),
                                    void* AckProcedurePar);
    extern int  oracle_commitorrollback (BYTE Action,
                                             int (*AckProcedure)(void* pack, void* papp),
                                             void* AckProcedurePar);

    extern int  oracle_fetch (OSSqlda* psqlda,   
                                  DWORD Every, DWORD MaxRows, DWORD RowsToProcess,
                                  int (*AckProcedure)(void* pack, void* papp),
                                  void* AckProcedurePar);
    extern int oracle_send_sqlda (OSSqlda* psqlda, int i);
    extern void oracle_define_sqlda (OSSqlda* psqlda, int* size,
                                         int* SelectItemNumber,
                                         char* SelectItemName, char* type,
                                         int SelectItemLength);
    extern int oracle_write_selectitem (OSSqlda* psqlda, char** values,
                                            char* SelectItemName, void* val);

#endif
#ifdef SQLDS
    extern OSSqlda* sqlds_process (char* SqlStatement, 
                                       DWORD Every,
                                       DWORD MaxRows,
                                       int (*HeaderProcedure)   (void* psqlda, void* papp),
                                       int (*ContentsProcedure) (void* psqlda, void* papp),
                                       void* ProcedurePar,
                                       int (*AckProcedure)(void* pack, void* papp),
                                       void* AckProcedurePar);
    extern OSShow* sqlds_show     (char* SqlStatement, BYTE StorageType,
                                       int (*ContentsProcedure) (void* pshow, void* papp),
                                       void* ProcedurePar,
                                       int (*AckProcedure)(void* pack, void* papp),
                                       void* AckProcedurePar);
    extern void sqlds_ack (OSAck* pack);
    extern int sqlds_disconnect (BYTE Commit,
                                     int (*AckProcedure)(void* pack, void* papp),
                                     void* AckProcedurePar);
    extern int sqlds_connect (char* DataBaseName, char* UserName, char* PassWord,
                                  int (*AckProcedure)(void* pack, void* papp),
                                  void* AckProcedurePar);
    extern int sqlds_execute (char* SqlStatement, BYTE Commit,
                                  int (*AckProcedure)(void* pack, void* papp),
                                  void* AckProcedurePar);
    extern int sqlds_commitorrollback (BYTE Action,
                                           int (*AckProcedure)(void* pack, void* papp),
                                           void* AckProcedurePar);
    extern int sqlds_fetch (OSSqlda* psqlda, 
                                DWORD Every, DWORD MaxRows, DWORD RowsToProcess,
                                int (*AckProcedure)(void* pack, void* papp),
                                void* AckProcedurePar);
    extern int sqlds_send_sqlda (OSSqlda* psqlda, int i);
    extern void sqlds_define_sqlda (OSSqlda* psqlda, int* size,
                                        int* SelectItemNumber,
                                        char* SelectItemName, char* type,
                                        int SelectItemLength);
    extern int sqlds_write_selectitem (OSSqlda* psqlda, char** values,
                                           char* SelectItemName, void* val);
#endif                                              
#ifdef ODBC
    extern int odbc_connect (char* DataBaseName, char* UserName, char* PassWord,
                                 int (*AckProcedure)(void* pack, void* papp),
                                 void* AckProcedurePar);


    extern OSSqlda* odbc_process (void* Connection, char* SqlStatement, 
                                      DWORD Every,
                                      DWORD MaxRows,
                                      int (*HeaderProcedure)   (void* psqlda, void* papp),
                                      int (*ContentsProcedure) (void* psqlda, void* papp),
                                      void* ProcedurePar,
                                      int (*AckProcedure)(void* pack, void* papp),
                                      void* AckProcedurePar);
    extern int odbc_send_sqlda (OSSqlda* psqlda, int i);
    extern int odbc_fetch (OSSqlda* psqlda, 
                               DWORD Every, DWORD MaxRows, DWORD RowsToProcess,
                               int (*AckProcedure)(void* pack, void* papp),
                               void* AckProcedurePar);
    extern void  odbc_freeS_sqlda (void* p);
    extern int   odbc_execute (void* Connection, char* SqlStatement, BYTE Commit,
                                   int (*AckProcedure)(void* pack, void* papp),
                                   void* AckProcedurePar);

    extern int odbc_sexecute (void* Connection, char* SqlStatement, BYTE Commit,
                                  int (*AckProcedure)(void* pack, void* papp),
                                  void* AckProcedurePar,void* content, DWORD ContentSize);


    extern int odbc_commitorrollback (void* Connection, BYTE Action,
                                          int (*AckProcedure)(void* pack, void* papp),
                                          void* AckProcedurePar);

    extern int odbc_disconnect (void* Connection, BYTE Commit,int (*AckProcedure)(void* pack, void* papp),
                                    void* AckProcedurePar);

#endif

#ifdef __cplusplus
}
#endif

#endif
