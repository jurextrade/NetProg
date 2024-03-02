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


#ifndef __osreslex_h__
#define __osreslex_h__


#include <stdio.h>
#include "osresyac.h"
#include "osio.h"
#include "ostypes.h"
#include "oslist.h"
#include "xf.h"


#define NBFILMAX    20
#define OPEN        1
#define SYNTAX      2
#define CHARACTER   3
#define ALLOCATION  4
#define XLTCAPTION  5 
#define MAXFILE     6 



typedef struct __ListOfFile
{
    UINT NbFile;
    UINT Passed[NBFILMAX];
    UINT LineNumber[NBFILMAX];
    char FileName[NBFILMAX][120];
}ListOfFile;

typedef struct _Argument
{
    char ArguType[10];
    int ArguSize;
    char ArguName[256];
}Argument;

typedef struct _Message
{
    char  MessName[256];
    List* AllArgument;
}Message;


typedef struct _MessageClass
{
    char  MessName[256];
    List* AllArgument;
}MessageClass;



typedef struct _Dialog
{
    int   DialId;
    char  DialName[256];
    List* AllMessageClasses;
}Dialog;


typedef struct _Entity
{
    char   MessName[256];
    List*  AllArgument;
}Entity;


typedef struct _XlatStruct
{
    char   XlatName[250];
    int    XlatId;
    char   FromName[120];
    char   ToName[120];
    char*  Tabxlt[16];
}XlatStruct;


typedef struct _Site
{
    char   Name[256];
    int    Id;
    char   Description[500];
}Site;

typedef struct _Machine
{
    char   Name[256];
    int    Id;
    char   IPAddress[256];
    char   IPName[256];
    char   System[256];
    int    Color;
    char   Localisation[256];
    char   Repertory[256];
    char   Description[500];
}Machine;

typedef struct _LogicalAddress
{
    char   Name[256];
    int    Id;
    char   Localisation[256];
    char   Description[500];
}LogicalAddress;


typedef struct _Property
{
    char   ApplicationName[256];
    int    Id;
    List*  Source;
    List*  Include;
    List*  Make;
    List*  Script;
    List*  Other;
    List*  Procedures;
}Property;

typedef struct _FileArgument
{
    char   Name[500];
    char   Type[30];
}FileArgument;


typedef struct _FolderArgument
{
    char   Name[500];
    char   Extensions[500];
    List*  Files;
}FolderArgument;



typedef struct _ProcArgument
{
    char   Name[500];
    char   Type[30];
    char   Context[30];
    char   Class[200];
    char   MessageClass[200];
    List*  Variables;
}ProcArgument;


typedef struct _VarArgument
{
    char   Name[500];
    char   Type[30];
    char   Init[250];
}VarArgument;


typedef struct _Scenario
{
    char  Name[500];
    int   Id;
    char  FileName[120];
    char  Description[500];
}Scenario;


typedef struct _DataBaseClass
{
    char  Name[500];
    int   Id;
    char  Description[500];
}DataBaseClass;


typedef struct _DataBase
{
    char  Name[500];
    int   Id;
    char  Class[256];
    char  BaseName[256];
    char  Localisation[256];
    char  Description[500];
}DataBase;



typedef struct _JournalClass
{
    char  Name[500];
    int   Id;
    int   Format;
    int   Length;
    List* AllMessage;
    char  Description[500];
}JournalClass;


typedef struct _Journal
{
    char  Name[500];
    int   Id;
    char  Class[256];
    char  Localisation[256];
    char  Description[500];
}Journal;



typedef struct _QueueClass
{
    char  Name[500];
    int   Id;
    char  Description[500];
}QueueClass;


typedef struct _Queue
{
    char  Name[500];
    int   Id;
    char  Class[256];
    char  Localisation[256];
    char  Description[500];
}Queue;



typedef struct _ApplicationClass
{
    char   Name[256];
    int    Id;
    int    SubType;
    char   Type[256];
    int    Color;
    char   Repertory[256];
    char   Description[500];
}ApplicationClass;


typedef struct _Application
{
    char  Name[500];
    int   Id;
    char  Class[256];
    char  Localisation[256];
    char  Repertory[256];
    char  ExecRepertory[256];
    char  ExecCommand[256];
    char  ExecParameters[256];
    char  ReceiveRepertory[256];
    char  SendRepertory[256];
    List* Folders;
}Application;

typedef struct _ConnectionClass
{
    char   Name[256];
    int    Id;
    char   FromAppliName[256];
    char   ToAppliName[256];
    int    PortNumber;
    char   TableName[256];
    char   Protocol[256];
    char   TransferFormat[25];
    char   Description[500];
}ConnectionClass;


typedef struct _Connection
{
    char   Name[256];
    int    Id;
    char   Class[256];
    char   FromAppliName[256];
    char   ToAppliName[256];
    char   UserName[256];
    char   PassWord[256];
    char   FileName[256];
    char   ReadMode[256];
    DWORD  Port;
    char   Description[500];
}Connection;


typedef struct _Remote
{
    char   Name[500];
    int    Id;
    char   OnMachine[256];
    char   UserName[256];
    char   PassWord[256];
    DWORD  Port;
    char   FHostName[256];
    char   FUserName[256];
    char   FPassWord[256];
    DWORD  FPort;
    BYTE   LogonType;                 /* firewall */
    BYTE   TransType;                 /* passive */
    char   Type[50];                  /* ftp, ...*/
    char   DistantRepertory[256];
    char   System[256];
    BOOL   Connected;
}Remote;


typedef struct _GlobalOne
{
    List*  AllDialog;
    List*  AllXlatStruct;
    List*  AllSites;
    List*  AllMachines;
    List*  AllApplicationClasses;
    List*  AllLogicalAddress;
    List*  AllProperties;
    List*  AllApplications;
    List*  AllScenarios;
    List*  AllDataBaseClasses;
    List*  AllDataBases;
    List*  AllJournalClasses;
    List*  AllJournals;
    List*  AllConnections;
    List*  AllConnectionClasses;
    List*  AllRemotes;
    List*  AllQueues;
    List*  AllQueueClasses;
}GlobalOne;



#ifdef __cplusplus
extern "C" {
#endif

    extern char OSStrErr[150];



    extern GlobalOne*      AllocGlobalOneYaccStruct(void);
    extern Dialog*         AllocDialogYaccStruct(char *, int);
    extern Message*        AllocMessageYaccStruct(char *);
    extern MessageClass*   AllocMessageClassYaccStruct(char *);
    extern Argument*       AllocArgumentYaccStruct(char *, int, char *);
    extern ConnectionClass* AllocConnectionClassYaccStruct(char *, int);
    extern Connection*     AllocConnectionYaccStruct(char *Name, int Id);

    extern Site*           AllocSiteYaccStruct(char *Name, int Id);
    extern Machine*        AllocMachineYaccStruct(char *Name, int Id);
    extern LogicalAddress* AllocAddressYaccStruct(char *Name, int Id);
    extern XlatStruct*     AllocXlatYaccStruct(char *, int);
    extern Property*       AllocPropertyYaccStruct (char* Name, int Id);
    extern FileArgument*   AllocFileArgumentYaccStruct(char* Type, char* Name);
    extern ProcArgument*   AllocProcArgumentYaccStruct(char* Name, char* Type, char* Context);
    extern VarArgument*    AllocVarArgumentYaccStruct(char* Name, char* Type, char* Init);
    extern ApplicationClass*    AllocApplicationClassYaccStruct (char *Name, int Id);
    extern Application*    AllocApplicationYaccStruct (char *Name, int Id);
    extern DataBaseClass*  AllocDataBaseClassYaccStruct(char *Name, int Id);
    extern DataBase*       AllocDataBaseYaccStruct(char *Name, int Id);
    extern JournalClass*   AllocJournalClassYaccStruct (char *Name, int Id);
    extern Journal*        AllocJournalYaccStruct (char *Name, int Id);
    extern QueueClass*     AllocQueueClassYaccStruct(char *Name, int Id);
    extern Queue*          AllocQueueYaccStruct(char *Name, int Id);


    extern Scenario*       AllocScenarioYaccStruct(char *Name, int Id);

    extern FolderArgument* AllocFolderArgumentYaccStruct(char *Name, char* Extensions);
    extern Remote*         AllocRemoteYaccStruct(char* Name, int Id);


    extern void            AddFileToFolder (FolderArgument* Fold, FileArgument* Argu);
    extern void            AddFolderToApplication (Application* Prog, FolderArgument* Argu);


    extern void            AddFileToProperty (Property* Pro, FileArgument* Argu);
    extern void            AddVarToProcedure (ProcArgument* Proc, VarArgument* Argu);
    extern void            AddProcToProperty (Property* Pro, ProcArgument* Argu);
    extern void            SetProcMessageClass (ProcArgument* Proc, char* Class, char* MessageClass);


    extern void            FreeArgumentYaccStruct(Argument *);
    extern void            FreeMessageYaccStruct(Message *);
    extern void            FreeMessageClassYaccStruct(MessageClass *);
    extern void            FreeDialogYaccStruct(Dialog *);
    extern void            FreeConnectionClassYaccStruct(ConnectionClass *);
    extern void            FreeConnectionYaccStruct(Connection *);
    extern void            FreeXlatYaccStruct(XlatStruct *);
    extern void            FreeGlobalOneYaccStruct(GlobalOne *);
    extern void            FreePropertyYaccStruct (Property* pPro);

    extern int             SearchDialogDoublons(GlobalOne *);
    extern int             SearchConnectDoublons(GlobalOne *);
    extern int             SearchXlatStructDoublons(GlobalOne *);

    extern void            *zzparse(XF* pxf);
    extern int             OSInitResourceFile (char *);
    extern int             OSInitCurrentResourceFile (char *);
    extern void            OSEndResourceFile (void);
    extern int             ErrorOccurs(int);
    extern int             InitGlob(void);
    extern void            CrashAll(void);
    extern int             OSGiveChar(int);
    extern int             OSSourceFile(void);

#ifdef __cplusplus
}
#endif

#endif
