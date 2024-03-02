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

#ifndef __MXMANAGER_H__
#define __MXMANAGER_H__


#include "mx.h"



#define MXONMACHINE          0
#define MXONLAD              1

#define MXSTORE              1
#define MXFIOP               0

/* SYSTEM */

#define MXSYSTEMUNIX         1
#define MXSYSTEMWIN32        2
#define MXSYSTEMVM           3
#define MXSYSTEMAS400        4
#define MXSYSTEMVOS          5 
#define MXSYSTEMOS2          6
#define MXSYSTEMHPUX         7

/* CARTO */
#define MXSITE               1
#define MXLAD                2
#define MXMACHINE            3
#define MXAPPLICATION        4


/* APPLICATION TYPE */
#define MXPASSIF             0 
#define MXCONNECTABLE        1
#define MXEXTERNAL           2

/* FILE TYPE */
#define MXPROJFILE           0
#define MXSOURCEFILE         1
#define MXDIALOGFILE         2
#define MXSCRIPTFILE         3 
#define MXMAKEFILE           4
#define MXTRANSCODFILE       5
#define MXINCLUDEFILE        6
#define MXOTHERFILE          7


#define MXPROJEXTENSION      "vnad"
#define MXCEXTENSION         "c"
#define MXCPLUSEXTENSION     "cpp"
#define MXINCLUDEEXTENSION   "h"
#define MXDIALOGEXTENSION    "mx"
#define MXSCRIPTEXTENSION    "sc"
#define MXTRANSCODEXTENSION  "xf"
#define MXRESOURCEEXTENSION  "irc"
#define MXENTITYEXTENSION    "pj"
#define MXFILEEXTENSION      "fv"
#define MXMAKEEXTENSION      "mk"




typedef struct _MXManager
{
    List*  Sites;
    List*  Machines;
    List*  Lads;

    List*  Applications;
    List*  DataBases;
    List*  Journals;
    List*  Queues;
    List*  Connections;

    List*  LadClasses;
    List*  ApplicationClasses;
    List*  DataBaseClasses;
    List*  QueueClasses;
    List*  JournalClasses;
    List*  ConnectionClasses;
    char   LocalMachineName[MXSizeName];
    char   LocalAddress[MXSizeName];
}MXManager;


typedef struct _MXLadClass
{
    char   Name[MXSizeName];        /* Name of the Site */
    WORD   Code;
    List*  Lads;
}MXLadClass;


typedef struct _MXApplicationClass
{
    char   Name[MXSizeName];
    WORD   Code;
    BYTE   Type;                          /* Connectable, External, Internal */
    List*  Applications;
    char   Repertory[MXSizeFileName];
}MXApplicationClass;


typedef struct _MXJournalClass
{
    char   Name[MXSizeName];
    WORD   Code;                          /* code of the class                  */
    BYTE   Type;                          /* system or file                     */
    int    Format;
    int    Length;
    List*  Journals;
    List*  MessageClasses;
}MXJournalClass;


typedef struct _MXDataBaseClass
{
    char   Name[MXSizeName];
    WORD   Code;                    /* code of the class                        */
    BYTE   Type;                    /* system or file                           */
    List*  DataBases;
}MXDataBaseClass;


typedef struct _MXQueueClass
{
    char   Name[MXSizeName];
    WORD   Code;                    /* code of the class                        */
    BYTE   Type;                    /* system or file                           */
    List*  Queues;
    List*  Machines;
}MXQueueClass;


/*----------------------------------------------------------------------------*/
/* BEGIN PHYSICAL ENTITIES                                                    */
/*----------------------------------------------------------------------------*/


typedef struct _MXSite
{
    char        Name[MXSizeName];   /* Name of the Site                         */
    WORD        Code;
    List*       Machines;           /* All machines                             */
}MXSite;


typedef struct _MXMachine
{
    char   Name[MXSizeName];        /* Name of the Machine                      */
    WORD   Code;
    BYTE   System;                  /* Operating System of the machine          */
    BOOL   SupportTCP;              /* Is machine supporting TCP or visible     */
    char   AddrName[MXSizeName];    /* Adressof the Machine                     */
    char   AdressIP[MXSizeAddress]; /* machine IP Adress                        */
    List*  Lads;                    /* All centers                              */
    List*  Applications;
    List*  DataBases;
    List*  Journals;
    List*  Queues;
    List*  Connections;             /* ftp connections, sc, ...                 */
    MXSite* Site;
}MXMachine;

typedef struct _MXLad
{
    char   Name[MXSizeName];        /* Name of the Site                         */
    WORD   Code;
    BYTE   System;                  /* Operating System of the machine          */
    List*  Applications;
    List*  DataBases;
    List*  Journals;
    List*  Queues;
    MXMachine*  Machine;
    MXLadClass* Class;
}MXLad;


typedef struct _MXApplication
{
    char   Name[MXSizeName];        /* Name of the Machine */
    WORD   Code;
    BYTE   Type;                    /* Type of application, connected,...       */
    char   Repertory[MXSizeFileName];  /* where source files are */
    char   ExecRepertory[MXSizeFileName];
    char   ExecCommand[MXSizeFileName];
    char   ExecParameters[MXSizeFileName];
    char   ReceiveRepertory[MXSizeFileName];
    char   SendRepertory[MXSizeFileName];
    List*  Folders;
    MXLad* Lad;
    MXMachine*  Machine;
    MXApplicationClass* Class;
}MXApplication;



typedef struct _MXJournal
{
    char   Name[MXSizeName];           /* Name of the Machine                   */
    WORD   Code;
    BYTE   Type;                       /* Type of application, connected,...    */
    char   Repertory[MXSizeFileName];  /* where source files are                */
    MXJournalClass* Class;
    MXLad* Lad;
    MXMachine*  Machine;
}MXJournal;


typedef struct _MXDataBase
{
    char    Name[MXSizeName];           /* Name of the Machine                  */
    WORD    Code;
    char    BaseName[MXSizeName];       /* Name of the base instance            */
    BYTE    Type;                       /* Type of application, connected,...   */
    MXDataBaseClass* Class;
    MXLad*  Lad;
    MXMachine*  Machine;
}MXDataBase;


typedef struct _MXQueue
{
    char    Name[MXSizeName];          /* Name of the Machine                    */
    WORD    Code;
    BYTE    Type;                      /* Type of application, connected,...     */
    MXQueueClass* Class;
    MXLad*  Lad;
    MXMachine*  Machine;
}MXQueue;

typedef struct _MXConnectionClass
{
    char   Name[MXSizeName];          /* ATTRIBUT or dialog name                */
    WORD   Code;                      /* code of the class                      */
    List*  Objects;                   /* list of MXObjects                      */
    char   TableName[MXSizeName];
    BYTE   FromToTranscod;            /* which side of com transcoding...       */
    int    Protocol;
    char   FileName[MXSizeName];
    char   FileAttributes[MXSizeName];
    char   UserName[MXSizeName];      /* User Name DataBase                     */
    char   PassWord[MXSizeName];      /* User Name DataBase                     */
    MXComClass* ComClass;
    MXApplicationClass* FromApplicationClass;
    void*  ToEntityClass;             /* Application, database, queue, journal  */
    List*  Connections;
}MXConnectionClass;


typedef struct _MXConnectionPar
{
    char        UserName[MXSizeFileName];
    char        PassWord[MXSizeFileName];
    DWORD       Port;
    char        FHostName[MXSizeFileName];
    char        FUserName[MXSizeFileName];
    char        FPassWord[MXSizeFileName];
    DWORD       FPort;
    BYTE        LogonType;               /* firewall */
    BYTE        TransType;               /* passive */
}MXConnectionPar;


typedef struct _MXConnection
{
    char        Name[MXSizeName];
    WORD        Code;
    char        UserName[MXSizeName];
    char        PassWord[MXSizeName];
    char        FileName[MXSizeFileName];
    int         Port;
    char        ReadMode[MXSizeName];
    BOOL        Connected;
    MXConnectionPar* Parameters;
    char        DistantRepertory[MXSizeFileName];
    char        System[MXSizeName];
    struct _MXFileData* FileData;
    MXApplication* FromApplication;
    void*       ToEntity;
    MXCom*      Com;
    MXConnectionClass* Class;
}MXConnection;




/*----------------------------------------------------------------------------*/
/* BEGIN FOLDER FILES                                                         */
/*----------------------------------------------------------------------------*/

typedef struct _MXFileData
{
    CHAR          Name[MXSizeName];
    CHAR          OnlyName[MXSizeName];
    BYTE          Type;                    /* Repertory or File                     */
    int           Size;
    CHAR          Access[MXAccessSize];
    CHAR          Date[MXDateSize];
    List*         Children;
    int           Expanded;                /* 1 opened already */
    int           InvalidName;
    struct _MXFileData*   Parent;
    MXConnection* Connection;
}MXFileData;



typedef struct _MXFolder
{
    char        Name[MXSizeName];        /* Name of the folder */
    BYTE        Type;                    /*script, dialog, c,...*/
    char        Extension[MXSizeExtension];
    char        Repertory[MXSizeFileName];
    List*       Files;                   /* file or dialog class */
}MXFolder;


typedef struct _MXFile
{
    char        Name[MXSizeName];        /* Name of the application */
    BYTE        Type;                    /*script, dialog, c,...*/
    MXFolder*   Folder;
    MXApplication*  Application;
}MXFile;


#ifdef __cplusplus
extern "C" {
#endif


    extern MXSite*    MXGetSiteFromName (MX* pmx, char* sitename);
    extern MXSite*    MXGetSiteFromCode (MX* pmx, WORD code);
    extern MXSite*    MXCreateSite (MX* pmx, char* sitename, WORD code);
    extern void       MXFreeSite (MX* pmx, MXSite* psite);

    extern MXMachine* MXAddMachineToSite (MX* pmx, MXSite* psite, MXMachine* pmachine);

    extern MXMachine* MXGetMachineFromName (MX* pmx, char* machname);
    extern MXMachine* MXGetMachineFromCode (MX* pmx, WORD code);
    extern MXMachine* MXGetMachineFromAddress (MX* pmx, char* IPAddress);
    extern MXMachine* MXCreateMachine (MX* pmx, char* machinename, WORD code, BYTE system, char* AddressIP, char* dname);
    extern void       MXFreeMachine (MX* pmx, MXMachine* pmachine);

    extern MXLadClass* MXGetLadClassFromName (MX* pmx, char* ladclassname);
    extern MXLadClass* MXGetLadClassFromCode (MX* pmx, WORD code);
    extern MXLadClass* MXCreateLadClass (MX* pmx, char* ladclassname, WORD code);
    extern void        MXFreeLadClass (MX* pmx, MXLadClass* pladclass);

    extern MXLad*     MXGetLadFromName (MX* pmx, char* ladname);
    extern MXLad*     MXGetLadFromCode (MX* pmx, WORD code);
    extern MXLad*     MXCreateLad (MX* pmx, MXLadClass* pclass, char* ladname, WORD code,MXMachine* pmachine);
    extern void       MXFreeLad (MX* pmx, MXLad* plad);

    extern MXApplicationClass* MXGetApplicationClassFromName (MX* pmx, char* appliname);
    extern MXApplicationClass* MXGetApplicationClassFromCode (MX* pmx, WORD code);
    extern MXApplicationClass* MXCreateApplicationClass (MX* pmx, char* appliclassname, WORD code, BYTE type);
    extern void                MXFreeApplicationClass (MX* pmx, MXApplicationClass* pappliclass);

    extern MXDataBaseClass* MXGetDataBaseClassFromName (MX* pmx, char* classname);
    extern MXDataBaseClass* MXGetDataBaseClassFromCode (MX* pmx, WORD code);
    extern MXDataBaseClass* MXCreateDataBaseClass  (MX* pmx, char* classname, WORD code, BYTE type);
    extern void             MXFreeDataBaseClass (MX* pmx, MXDataBaseClass* pdatabaseclass);

    extern MXQueueClass* MXGetQueueClassFromName (MX* pmx, char* classname);
    extern MXQueueClass* MXGetQueueClassFromCode (MX* pmx, WORD code);
    extern MXQueueClass* MXCreateQueueClass  (MX* pmx, char* classname, WORD code, BYTE type);
    extern void          MXFreeQueueClass (MX* pmx, MXQueueClass* pqueueclass);

    extern MXJournalClass* MXGetJournalClassFromName (MX* pmx, char* classname);
    extern MXJournalClass* MXGetJournalClassFromCode (MX* pmx, WORD code);
    extern MXJournalClass* MXCreateJournalClass  (MX* pmx, char* classname, WORD code, BYTE type);
    extern void            MXFreeJournalClass (MX* pmx, MXJournalClass* pjournalclass);

    extern MXApplication* MXGetApplicationFromName (MX* pmx, char* appliname);
    extern MXApplication* MXGetApplicationFromCode (MX* pmx, WORD code);
    extern MXApplication* MXCreateApplication (MX* pmx, MXApplicationClass* pclass, char* name, WORD code,  void* OnEntity, BYTE type);
    extern void           MXFreeApplication (MX* pmx, MXApplication* pappli);

    extern MXDataBase* MXGetDataBaseFromName (MX* pmx, char* name);
    extern MXDataBase* MXGetDataBaseFromCode (MX* pmx, WORD code);
    extern MXDataBase* MXCreateDataBase (MX* pmx, MXDataBaseClass* pclass, char* name, WORD code, void* OnEntity, BYTE type);
    extern void        MXFreeDataBase (MX* pmx, MXDataBase* pdatabase);

    extern MXQueue* MXGetQueueFromName (MX* pmx, char* name);
    extern MXQueue* MXGetQueueFromCode (MX* pmx, WORD code);
    extern MXQueue* MXCreateQueue (MX* pmx, MXQueueClass* pclass, char* name, WORD code, void* OnEntity, BYTE type);
    extern void     MXFreeQueue (MX* pmx, MXQueue* pqueue);

    extern MXJournal* MXGetJournalFromName (MX* pmx, char* name);
    extern MXJournal* MXGetJournalFromCode (MX* pmx, WORD code);
    extern MXJournal* MXCreateJournal (MX* pmx, MXJournalClass* pclass, char* name, WORD code, void* OnEntity, BYTE type);
    extern void       MXFreeJournal (MX* pmx, MXJournal* pjournal);

    extern MXConnectionClass* MXGetConnectionClassFromName (MX* pmx, char* Name);
    extern MXConnectionClass* MXGetConnectionClassFromCode (MX* pmx, WORD code);
    extern MXConnectionClass* MXCreateConnectionClass (MX* pmx, char* name, WORD code, int protocol, BYTE FromTo, char* TableName, char* fromclassname, char* toclassname);


    extern MXConnection* MXGetConnectionFromName (MX* pMX, char* Name);
    extern MXConnection* MXGetConnectionFromCode (MX* pMX, WORD code);
    extern MXConnection* MXCreateConnection (MX* pmx, char* name, WORD code, MXConnectionClass* pclass, char* fromname, char* toname);



    extern MXFolder*  MXGetFolderFromName (MX* pmx, MXApplication* pappli, char* Name);
    extern MXFolder*  MXGetFolderFromName (MX* pmx, MXApplication* pappli, char* Name);
    extern MXFolder*  MXCreateFolder (MX* pmx, MXApplication* pappli, char* Name, char* Extension);
    extern void       MXFreeFolder (MX* pmx, MXFolder* pfolder);

    extern MXFile*    MXGetFileFromName (MX* pmx,  char* filename, MXFolder* pfolder);
    extern MXFile*    MXCreateFile (MX* pmx, MXFolder* folder, char* f_filename);
    extern void       MXFreeFile (MX* pmx, MXFile* pfile);





    extern char*      MXGetSystemFromCode (BYTE system, char* Name);
    extern BYTE       MXGetCodeFromSystem (char* Name);
    extern void*      MXGetEntityFromString (MX* pmx, char* s, BYTE Type, char* EntityName);

    extern MXCom*     MXConnect (MX* pmx, char* ConnectionName);
    extern void       MXDisconnect (MX* pmx, char* ConnectionName);

    extern MXMessage* MXManagePutMessage (MX* pmx, char* connectionname, char* classname, char* messageclassname);
    extern MXMessage* MXManageSend (MX* pmx, char* connectionname, char* classname, char* messageclassname);

#ifdef __cplusplus
}
#endif



#endif /* __MXMANAGER_H__*/
