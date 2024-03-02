#define _CRT_SECURE_NO_WARNINGS
# line 29 "osresyac.y"
  #include "osreslex.h"

GlobalOne*       TheOne;
Dialog*          Dial;
MessageClass*    MessClass;
Argument*        Argu;
Machine*         Mach;
ConnectionClass* ConnClass;
Connection*      Conn;
ApplicationClass* AppliClass;
Application*     Prog;
XlatStruct*      VarXlat;
Site*            Sit;
Scenario*        Sce;
DataBaseClass*   Data;
DataBase*        iData;
JournalClass*    Stor;
Journal*         iStor;
QueueClass*      Que;
Queue*           iQue;
LogicalAddress*  La;
Property*        Pro;
FolderArgument*  FolderArgu;
FileArgument*    FileArgu;
ProcArgument*    ProcArgu;
VarArgument*     VarArgu;
Remote*          remote;
int Verif;
char Buffer[256];
int Errind = SYNTAX;

# line 61 "osresyac.y"
#ifdef __cplusplus
#  include <stdio.h>
#  include <yacc.h>
#endif	/* __cplusplus */ 
#define zzclearin zzchar = -1
#define zzerrok zzerrflag = 0
extern int zzchar;
#ifndef ZZMAXDEPTH
#define ZZMAXDEPTH 150
#endif

/* __ZZSCLASS defines the scoping/storage class for global objects
 * that are NOT renamed by the -p option.  By default these names
 * are going to be 'static' so that multi-definition errors
 * will not occur with multiple parsers.
 * If you want (unsupported) access to internal names you need
 * to define this to be null so it implies 'extern' scope.
 * This should not be used in conjunction with -p.
 */
#ifndef __ZZSCLASS
# define __ZZSCLASS static
#endif
ZZSTYPE zzlval;
__ZZSCLASS ZZSTYPE zzval;
typedef int zztabelem;
# define ZZERRCODE 256

# line 702 "osresyac.y"


/*******************************************************************************
            DEFINITION OF THE FUNCTIONS
*******************************************************************************/
zzerror (char *s)
{
    ErrorOccurs(Errind);
}


int InitGlob()
{
    if (!TheOne)
    {
        TheOne = AllocGlobalOneYaccStruct();
        if (!TheOne) return 0;
    }
    return 1;
}

void CrashAll()
{
    if (TheOne)
        FreeGlobalOneYaccStruct(TheOne);
    return;
}

GlobalOne *AllocGlobalOneYaccStruct()
{
    GlobalOne *Glob;

    Glob = (GlobalOne *)malloc(sizeof(GlobalOne));

    if (!Glob) return(NULL);
    memset (Glob, 0, sizeof (GlobalOne));
    return((GlobalOne *)Glob);
}


Dialog *AllocDialogYaccStruct(char *Name, int Id)
{
    Dialog *Dial;

    Dial = (Dialog *)malloc(sizeof(Dialog));
    if (!Dial) return(NULL);

    memset(Dial->DialName, '\0', 256);
    memcpy(Dial->DialName, Name + 1, strlen(Name) - 2);
    Dial->DialId = Id;
    Dial->AllMessageClasses = NULL;

    return Dial;
}



Property* AllocPropertyYaccStruct (char* Name, int Id)
{
    Property* Pro;
    Pro = (Property *)malloc (sizeof (Property));
    if (!Pro) return NULL;
    memset (Pro, 0, sizeof (Property));
    memcpy(Pro->ApplicationName, Name + 1, strlen(Name) - 2);
    Pro->Id = Id;
    return Pro;
}

FileArgument* AllocFileArgumentYaccStruct(char* Name, char* Type)
{
    FileArgument* FileArgu;
    FileArgu = (FileArgument*)malloc (sizeof (FileArgument));
    if (!FileArgu)  return NULL;
    memset (FileArgu, 0, sizeof (FileArgument));
    strcpy (FileArgu->Type, Type);
    strcpy (FileArgu->Name, Name);
    return FileArgu;
}

ProcArgument* AllocProcArgumentYaccStruct(char* Name, char* Type, char* Context)
{
    ProcArgument* ProcArgu;
    ProcArgu = (ProcArgument*)malloc (sizeof (ProcArgument));
    if (!ProcArgu)  return NULL;
    memset (ProcArgu, 0, sizeof (ProcArgument));
    strcpy (ProcArgu->Name, Name);
    strcpy (ProcArgu->Type, Type);
    strcpy (ProcArgu->Context, Context);
    return ProcArgu;
}

VarArgument* AllocVarArgumentYaccStruct(char* Name, char* Type, char* Init)
{
    VarArgument* VarArgu;
    VarArgu = (VarArgument*)malloc (sizeof (VarArgument));
    if (!VarArgu)  return NULL;
    memset (VarArgu, 0, sizeof (VarArgument));
    strcpy (VarArgu->Type, Type);
    strcpy (VarArgu->Name, Name);
    strcpy (VarArgu->Init, Init);
    return VarArgu;
}



void AddFileToFolder (FolderArgument* Fold, FileArgument* Argu)
{
    ListNewr (&Fold->Files, Argu);
}

void AddFileToProperty (Property* Pro, FileArgument* Argu)
{
    char* Type = Argu->Type;
    if (strcmp (Type, "SOURCE") == 0)
        ListNewr (&Pro->Source, Argu);
    if (strcmp (Type, "INCLUDE") == 0)
        ListNewr (&Pro->Include, Argu);
    if (strcmp (Type, "SCRIPT") == 0)
        ListNewr (&Pro->Script, Argu);
    if (strcmp (Type, "MAKE") == 0)
        ListNewr (&Pro->Make, Argu);
    if (strcmp (Type, "OTHER") == 0)
        ListNewr (&Pro->Other, Argu);
}

void AddFolderToApplication (Application* Prog, FolderArgument* Argu)
{
    ListNewr (&Prog->Folders, Argu);
}



void AddProcToProperty (Property* Pro, ProcArgument* Argu)
{
    ListNewr (&Pro->Procedures, Argu);
}

void AddVarToProcedure (ProcArgument* Proc, VarArgument* Argu)
{
    ListNewr (&Proc->Variables, Argu);
}

void SetProcMessageClass (ProcArgument* Proc, char* Class, char* MessageClass)
{
    strcpy (Proc->Class, Class);
    strcpy (Proc->MessageClass, MessageClass);
}


MessageClass *AllocMessageClassYaccStruct(char *Name)
{
    MessageClass *Mess;

    Mess = (MessageClass *)malloc(sizeof(MessageClass));
    if (!Mess) return(NULL);

    memset(Mess->MessName, '\0', 256);
    memcpy(Mess->MessName, Name, strlen(Name));
    Mess->AllArgument = NULL;
    return((MessageClass *)Mess);
}


Message *AllocMessageYaccStruct(char *Name)
{
    Message *Mess;

    Mess = (Message *)malloc(sizeof(Message));
    if (!Mess) return(NULL);

    memset(Mess->MessName, '\0', 256);
    memcpy(Mess->MessName, Name, strlen(Name));
    Mess->AllArgument = NULL;

    return((Message *)Mess);
}

Argument *AllocArgumentYaccStruct(char *Type, int Size, char *Name)
{
    Argument *Argu = NULL;

    Argu = (Argument *)malloc(sizeof(Argument));
    if (!Argu) return(NULL);

    memset(Argu->ArguType, '\0', 10);
    memset(Argu->ArguName, '\0', 256);

    memcpy(Argu->ArguType, Type, strlen(Type));
    Argu->ArguSize = Size;
    memcpy(Argu->ArguName, Name, strlen(Name));

    return((Argument *)Argu);
}


ConnectionClass *AllocConnectionClassYaccStruct(char *Name, int Id)
{
    ConnectionClass *Conn;

    Conn = (ConnectionClass *)malloc(sizeof(ConnectionClass));
    if (!Conn) return(NULL);
    memset (Conn, 0, sizeof (ConnectionClass));
    memcpy(Conn->Name, Name + 1, strlen(Name) - 2);
    Conn->Id = Id;
    return Conn;
}


Connection *AllocConnectionYaccStruct(char *Name, int Id)
{
    Connection *Conn;

    Conn = (Connection *)malloc(sizeof(Connection));
    if (!Conn) return(NULL);
    memset (Conn, 0, sizeof (Connection));
    memcpy(Conn->Name, Name + 1, strlen(Name) - 2);
    Conn->Id = Id;
    return Conn;
}





Remote *AllocRemoteYaccStruct(char *Name, int Id)
{
    Remote* Conn;

    Conn = (Remote *)malloc(sizeof(Remote));
    if (!Conn) return(NULL);
    memset (Conn, 0, sizeof (Remote));
    memcpy(Conn->Name, Name + 1,  strlen(Name) - 2);
    Conn->Id = Id;
    Conn->Port = 21;
    Conn->Connected = 0;
    return Conn;
}

ApplicationClass* AllocApplicationClassYaccStruct(char *Name, int Id)
{
    ApplicationClass* appli;
    appli = (ApplicationClass *)malloc(sizeof(ApplicationClass));
    if (!appli) return(NULL);
    memset (appli, 0, sizeof (ApplicationClass));
    memcpy(appli->Name, Name + 1,  strlen(Name) - 2);
    appli->Id = Id;
    return appli;
}


Application* AllocApplicationYaccStruct(char *Name, int Id)
{
    Application* Pro;

    Pro = (Application *)malloc(sizeof(Application));
    if (!Pro) return(NULL);
    memset (Pro, 0, sizeof (Application));
    memcpy(Pro->Name, Name + 1,  strlen(Name) - 2);
    Pro->Id = Id;
    return Pro;
}

FolderArgument *AllocFolderArgumentYaccStruct(char *Name, char* Extensions)
{
    FolderArgument* Fold;

    Fold = (FolderArgument *)malloc(sizeof(FolderArgument));
    if (!Fold) return(NULL);
    memset (Fold, 0, sizeof (FolderArgument));
    memcpy(Fold->Name, Name + 1,  strlen(Name) - 2);
    memcpy(Fold->Extensions, Extensions,  strlen(Extensions));
    return Fold;
}


Scenario* AllocScenarioYaccStruct(char *Name, int Id)
{
    Scenario* scenario;

    scenario = (Scenario *)malloc(sizeof(Scenario));
    if (!scenario) return(NULL);
    memset (scenario, 0, sizeof (Scenario));
    memcpy(scenario->Name, Name + 1,  strlen(Name) - 2);
    scenario->Id = Id;
    return scenario;
}

Queue* AllocQueueYaccStruct(char *Name, int Id)
{
    Queue* queue;

    queue = (Queue *)malloc(sizeof(Queue));
    if (!queue) return(NULL);
    memset (queue, 0, sizeof (Queue));
    memcpy(queue->Name, Name + 1,  strlen(Name) - 2);
    queue->Id = Id;
    return queue;
}



QueueClass* AllocQueueClassYaccStruct(char *Name, int Id)
{
    QueueClass* queueclass;

    queueclass = (QueueClass *)malloc(sizeof(QueueClass));
    if (!queueclass) return(NULL);
    memset (queueclass, 0, sizeof (QueueClass));
    memcpy(queueclass->Name, Name + 1,  strlen(Name) - 2);
    queueclass->Id = Id;
    return queueclass;
}





DataBaseClass* AllocDataBaseClassYaccStruct(char *Name, int Id)
{
    DataBaseClass* database;

    database = (DataBaseClass *)malloc(sizeof(DataBaseClass));
    if (!database) return(NULL);
    memset (database, 0, sizeof (DataBaseClass));
    memcpy(database->Name, Name + 1,  strlen(Name) - 2);
    database->Id = Id;
    return database;
}

DataBase* AllocDataBaseYaccStruct(char *Name, int Id)
{
    DataBase* database;

    database = (DataBase *)malloc(sizeof(DataBase));
    if (!database) return(NULL);
    memset (database, 0, sizeof (DataBase));
    memcpy(database->Name, Name + 1,  strlen(Name) - 2);
    database->Id = Id;
    return database;
}




JournalClass*  AllocJournalClassYaccStruct (char *Name, int Id)
{
    JournalClass* journal;

    journal = (JournalClass *)malloc(sizeof(JournalClass));
    if (!journal) return(NULL);
    memset (journal, 0, sizeof (JournalClass));
    memcpy(journal->Name, Name + 1,  strlen(Name) - 2);
    journal->AllMessage = NULL;
    journal->Format = 0;
    journal->Length = 0;
    journal->Id = Id;
    return journal;
}


Journal*  AllocJournalYaccStruct (char *Name, int Id)
{
    Journal* journal;

    journal = (Journal *)malloc(sizeof(Journal));
    if (!journal) return(NULL);
    memset (journal, 0, sizeof (Journal));
    memcpy(journal->Name, Name + 1,  strlen(Name) - 2);
    journal->Id = Id;
    return journal;
}







Site *AllocSiteYaccStruct(char *Name, int Id)
{
    Site* site;

    site = (Site *)malloc(sizeof(Site));
    if (!site) return(NULL);
    memset (site, 0, sizeof (Site));
    memcpy(site->Name, Name + 1,  strlen(Name) - 2);
    site->Id = Id;
    return site;
}

Machine *AllocMachineYaccStruct(char *Name, int Id)
{
    Machine* machine;

    machine = (Machine *)malloc(sizeof(Machine));
    if (!machine) return(NULL);
    memset (machine, 0, sizeof (Machine));
    memcpy(machine->Name, Name + 1,  strlen(Name) - 2);
    machine->Id = Id;
    return machine;
}

LogicalAddress *AllocAddressYaccStruct(char *Name, int Id)
{
    LogicalAddress* la;

    la = (LogicalAddress *)malloc(sizeof(LogicalAddress));
    if (!la) return(NULL);
    memset (la, 0, sizeof (LogicalAddress));
    memcpy(la->Name, Name + 1,  strlen(Name) - 2);
    la->Id = Id;
    return la;
}



XlatStruct *AllocXlatYaccStruct(char *Name, int Id)
{
    int i;
    XlatStruct *VarXlat;

    VarXlat = (XlatStruct *)malloc(sizeof(XlatStruct));
    if (!VarXlat) return(NULL);

    memset(VarXlat->XlatName, '\0', 256);
    memcpy(VarXlat->XlatName, Name + 1, strlen(Name) - 2);
    VarXlat->XlatId = Id;
    memset(VarXlat->FromName, '\0', 120);
    memset(VarXlat->ToName, '\0', 120);
    for(i=0; i<16; i++)
    {
        VarXlat->Tabxlt[i] = (char *)malloc(33);
        memset((char *)VarXlat->Tabxlt[i], '\0', 33);
    }

    return((XlatStruct *)VarXlat);
}

void FreeArgumentYaccStruct(Argument *pArg)
{
    if (pArg)
        free(pArg);
    return;
}

void FreeMessageClassYaccStruct(MessageClass *pMess)
{
    Argument *Argu = NULL;

    if (pMess)
    {
        while(pMess->AllArgument)
        {
            Argu = (Argument *)pMess->AllArgument->car;
            ListRemove(&pMess->AllArgument, Argu);
            FreeArgumentYaccStruct(Argu);
        }
        free(pMess);
    }
    return;
}



void FreeMessageYaccStruct(Message *pMess)
{
    Argument *Argu = NULL;

    if (pMess)
    {
        while(pMess->AllArgument)
        {
            Argu = (Argument *)pMess->AllArgument->car;
            ListRemove(&pMess->AllArgument, Argu);
            FreeArgumentYaccStruct(Argu);
        }
        free(pMess);
    }
    return;
}


void FreePropertyYaccStruct (Property* pPro)
{
    FileArgument* Argu = NULL;

    while(pPro->Source)
    {
        Argu = (FileArgument *)pPro->Source->car;
        ListRemove(&pPro->Source, Argu);
        free (Argu);
    }
    while(pPro->Include)
    {
        Argu = (FileArgument *)pPro->Include->car;
        ListRemove(&pPro->Include, Argu);
        free (Argu);
    }
    while(pPro->Script)
    {
        Argu = (FileArgument *)pPro->Script->car;
        ListRemove(&pPro->Script, Argu);
        free (Argu);
    }
    while(pPro->Make)
    {
        Argu = (FileArgument *)pPro->Make->car;
        ListRemove(&pPro->Make, Argu);
        free (Argu);
    }
    while(pPro->Other)
    {
        Argu = (FileArgument *)pPro->Other->car;
        ListRemove(&pPro->Other, Argu);
        free (Argu);
    }
    while(pPro->Procedures)
    {

        ProcArgument* ProcArgu = (ProcArgument *)pPro->Procedures->car;
        ListRemove(&pPro->Procedures, ProcArgu);
        while (ProcArgu->Variables)
        {
            VarArgument* VarArgu = (VarArgument *)ProcArgu->Variables->car;
            ListRemove(&ProcArgu->Variables, VarArgu);
            free (VarArgu);
        }
        free (Argu);
    }

}


void FreeDialogYaccStruct(Dialog *pDial)
{
    MessageClass *Mess;

    if (pDial)
    {
        while(pDial->AllMessageClasses)
        {
            Mess = (MessageClass *)pDial->AllMessageClasses->car;
            ListRemove(&pDial->AllMessageClasses, Mess);
            FreeMessageClassYaccStruct(Mess);
        }
        free(pDial);
    }
    return;
}

void FreeConnectionClassYaccStruct(ConnectionClass *pConn)
{
    if (pConn) free(pConn);
    return;
}

void FreeConnectionYaccStruct(Connection* pConn)
{
    if (pConn) free(pConn);
    return;
}


void FreeXlatYaccStruct(XlatStruct *pXlat)
{
    if (pXlat) free(pXlat);
    return;
}

void FreeGlobalOneYaccStruct(GlobalOne *pGlob)
{
    Dialog*   Dial;
    ConnectionClass* ConnClass;
    Connection*      Conn;
    XlatStruct* VarXlat;

    if (pGlob)
    {
        while(pGlob->AllDialog)
        {
            Dial = (Dialog *)pGlob->AllDialog->car;
            ListRemove(&pGlob->AllDialog, Dial);
            FreeDialogYaccStruct(Dial);
        }
        while(pGlob->AllConnectionClasses)
        {
            ConnClass = (ConnectionClass *)pGlob->AllConnectionClasses->car;
            ListRemove(&pGlob->AllConnectionClasses, ConnClass);
            FreeConnectionClassYaccStruct(ConnClass);
        }
        while(pGlob->AllConnections)
        {
            Conn = (Connection *)pGlob->AllConnections->car;
            ListRemove(&pGlob->AllConnections, Conn);
            FreeConnectionYaccStruct(Conn);
        }

        while(pGlob->AllXlatStruct)
        {
            VarXlat = (XlatStruct *)pGlob->AllXlatStruct->car;
            ListRemove(&pGlob->AllXlatStruct, VarXlat);
            FreeXlatYaccStruct(VarXlat);
        }
        while (pGlob->AllProperties)
        {
            Property* Pro = (Property*)pGlob->AllProperties->car;
            ListRemove (&pGlob->AllProperties, Pro);
            free (Pro);
        }
        while (pGlob->AllApplications)
        {
            Application* Prog = (Application*)pGlob->AllApplications->car;
            ListRemove (&pGlob->AllApplications, Prog);
            free (Prog);
        }
        while (pGlob->AllScenarios)
        {
            Scenario* Sce = (Scenario*)pGlob->AllScenarios->car;
            ListRemove (&pGlob->AllScenarios, Sce);
            free (Sce);
        }
        while (pGlob->AllDataBaseClasses)
        {
            DataBaseClass* Data = (DataBaseClass*)pGlob->AllDataBaseClasses->car;
            ListRemove (&pGlob->AllDataBaseClasses, Data);
            free (Data);
        }
        while (pGlob->AllDataBases)
        {
            DataBase* Data = (DataBase*)pGlob->AllDataBases->car;
            ListRemove (&pGlob->AllDataBases, Data);
            free (Data);
        }
        while (pGlob->AllQueueClasses)
        {
            QueueClass* queue = (QueueClass*)pGlob->AllQueueClasses->car;
            ListRemove (&pGlob->AllQueueClasses, queue);
            free (queue);
        }

        while (pGlob->AllQueues)
        {
            Queue* queue = (Queue*)pGlob->AllQueues->car;
            ListRemove (&pGlob->AllQueues, queue);
            free (queue);
        }

        while (pGlob->AllJournalClasses)
        {
            JournalClass* Sto = (JournalClass*)pGlob->AllJournalClasses->car;
            ListRemove (&pGlob->AllJournalClasses, Sto);
            free (Sto);
        }

        while (pGlob->AllJournals)
        {
            Journal* iSto = (Journal*)pGlob->AllJournals->car;
            ListRemove (&pGlob->AllJournals, iSto);
            free (iSto);
        }



        free(pGlob);
        TheOne = NULL;
    }
    return;
}

int SearchDialogDoublons(GlobalOne *Glob)
{
    List *Glb1, *Glb2;
    Dialog *Dial1, *Dial2;

    Glb1 = Glob->AllDialog;
    while(Glb1)
    {
        Dial1 = (Dialog *)Glb1->car;
        Glb2 = Glb1->cdr;

        while(Glb2)
        {
            Dial2 = (Dialog *)Glb2->car;

            if (strcmp(Dial1->DialName, Dial2->DialName) == 0) return(0);
            if (Dial1->DialId == Dial2->DialId) return(0);
            Glb2 = Glb2->cdr;
        }
        Glb1 = Glb1->cdr;
    }
    return(1);
}

int SearchConnectDoublons(GlobalOne *Glob)
{
    List *Glb1, *Glb2;
    ConnectionClass *Conn1, *Conn2;

    Glb1 = Glob->AllConnectionClasses;
    while(Glb1)
    {
        Conn1 = (ConnectionClass *)Glb1->car;
        Glb2 = Glb1->cdr;

        while(Glb2)
        {
            Conn2 = (ConnectionClass *)Glb2->car;

            if (strcmp(Conn1->Name, Conn2->Name) == 0) return(0);
            if (Conn1->Id == Conn2->Id) return(0);
            Glb2 = Glb2->cdr;
        }
        Glb1 = Glb1->cdr;
    }
    return(1);
}

int SearchXlatStructDoublons(GlobalOne *Glob)
{
    List *Glb1, *Glb2;
    XlatStruct *VarXlat1, *VarXlat2;

    Glb1 = Glob->AllXlatStruct;
    while(Glb1)
    {
        VarXlat1 = (XlatStruct *)Glb1->car;
        Glb2 = Glb1->cdr;

        while(Glb2)
        {
            VarXlat2 = (XlatStruct *)Glb2->car;

            if (strcmp(VarXlat1->XlatName, VarXlat2->XlatName) == 0) return(0);
            if (VarXlat1->XlatId == VarXlat2->XlatId) return(0);
            Glb2 = Glb2->cdr;
        }
        Glb1 = Glb1->cdr;
    }
    return(1);
}

__ZZSCLASS zztabelem zzexca[] ={
                                   -1, 1,
                                   0, -1,
                                   -2, 0,
                               };
# define ZZNPROD 334
# define ZZLAST 599
__ZZSCLASS zztabelem zzact[]={

                                 471,   462,   452,   414,   382,    58,   352,    25,    26,    28,
                                 27,    29,    22,    23,    30,   463,   453,    32,    33,    34,
                                 35,    36,    37,    31,   410,    25,    26,    28,    27,    29,
                                 22,    23,    30,   379,   442,    32,    33,    34,    35,    36,
                                 37,    31,   430,   341,   305,   269,    21,    24,    20,   263,
                                 227,   215,   157,   439,   190,   184,   176,   160,   142,   290,
                                 421,   436,    38,   338,    21,    24,    57,   294,   485,   481,
                                 475,   458,   467,   324,   446,   468,   369,   320,   251,   330,
                                 38,   273,    70,    62,   193,   229,   415,   162,   478,   472,
                                 426,   170,   335,   255,   180,   357,   154,   449,   406,   399,
                                 376,   310,   346,   317,   314,   245,   223,   309,   181,   181,
                                 187,   287,   230,   167,   163,   167,   173,   148,   151,   373,
                                 219,   366,   283,   327,   278,   388,   383,   403,   392,    67,
                                 59,   400,   401,   281,   208,   200,   362,   388,   353,   362,
                                 325,   353,   325,    68,    60,   204,   145,   139,   465,   444,
                                 349,   344,   322,   136,   416,   312,   276,   257,   487,   437,
                                 271,   267,   194,   265,   412,   394,   387,   385,   361,   360,
                                 351,   323,   195,   261,   253,   249,   247,   243,   225,   221,
                                 217,   213,   211,   206,   202,   198,   133,   131,   129,   127,
                                 125,   123,   121,   119,   117,   115,   113,   111,   109,   107,
                                 105,   103,   101,    99,   374,   241,   240,   296,   233,   220,
                                 242,   216,   460,   428,   424,   408,   384,   355,   354,   350,
                                 303,   292,   266,   248,   212,   371,   333,   236,   393,   367,
                                 260,   237,   483,   479,   454,   447,   443,   440,   431,   411,
                                 380,   377,   343,   342,   298,   246,   464,   347,   331,   328,
                                 318,   315,   295,   288,   282,   279,   274,   270,   264,   252,
                                 228,   224,   205,   201,   134,   132,   130,   128,   126,   124,
                                 122,   120,   118,   116,   114,   112,   110,   108,   106,   104,
                                 102,   100,   418,   417,   396,   395,   389,   390,   363,   364,
                                 284,   209,   285,   210,   476,   473,   469,   459,   456,   450,
                                 434,   433,   423,   419,   404,   397,   383,   370,   358,   336,
                                 321,   259,   306,   302,   301,   300,   197,   486,   482,   427,
                                 422,   407,   339,   311,   291,    98,    96,    94,    92,    90,
                                 88,    86,    84,    82,    80,    78,    76,    74,    72,    66,
                                 64,   258,   234,   256,   196,   179,   165,   164,    18,    17,
                                 56,    55,    15,    16,    54,    53,   386,    13,    14,    52,
                                 51,    12,    19,    50,    49,    11,    10,    48,    47,     9,
                                 8,    46,    45,     7,     6,    44,    43,     5,     4,    42,
                                 41,     3,     2,    40,    39,   332,   413,   207,   144,   143,
                                 69,   448,   438,   425,   402,   368,   359,   329,   280,   203,
                                 141,   140,    65,   484,   480,   474,   466,   457,   445,   435,
                                 420,   391,   356,   319,   272,   348,   192,   432,   191,    97,
                                 455,   381,   299,   235,   239,   238,   166,   161,    81,   451,
                                 441,   429,   409,   378,   340,   293,   226,   159,   158,    79,
                                 375,   337,   289,   218,   153,   152,    75,   313,   262,   183,
                                 182,    91,   178,   177,   461,    89,   345,   304,   250,   175,
                                 174,    87,   172,   470,   171,    85,   316,   268,   189,   188,
                                 95,   477,   186,   185,    93,   244,   169,   168,    83,   405,
                                 372,   334,   286,   214,   150,   149,    73,   222,   156,   155,
                                 77,   147,   146,    71,   398,   365,   326,   277,   199,   138,
                                 137,    63,   135,    61,     1,     0,     0,     0,   232,   231,
                                 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
                                 0,     0,     0,     0,   254,     0,     0,     0,     0,     0,
                                 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
                                 275,     0,     0,     0,     0,     0,     0,     0,     0,     0,
                                 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
                                 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
                                 0,     0,     0,     0,   297,     0,     0,     0,     0,     0,
                                 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
                                 0,     0,     0,     0,     0,     0,     0,   308,   307 };
__ZZSCLASS zztabelem zzpact[]={

                                  -269,  -251, -3000, -3000, -3000, -3000, -3000, -3000, -3000, -3000,
                                  -3000, -3000, -3000, -3000, -3000, -3000, -3000, -3000, -3000, -3000,
                                  -3000,  -174,    83,    82,  -175,    81,    80,    79,    78,    77,
                                  76,    75,    74,    73,    72,    71,    70,    69,    68, -3000,
                                  -3000, -3000, -3000, -3000, -3000, -3000, -3000, -3000, -3000, -3000,
                                  -3000, -3000, -3000, -3000, -3000, -3000, -3000, -3000, -3000, -3000,
                                  -3000,   -81,    20,   -82,    19,   -83,    18, -3000, -3000,   -84,
                                  17,   -85,    16,   -86,    15,   -87,    14,   -88,    13,   -89,
                                  12,   -90,    11,   -91,    10,   -92,     9,   -93,     8,   -94,
                                  7,   -95,     6,   -96,     5,   -97,     4,   -98,     3,  -142,
                                  -3000,  -151, -3000,  -275, -3000,  -152, -3000,  -192, -3000,  -189,
                                  -3000,  -214, -3000,  -283, -3000,  -276, -3000,  -198, -3000,  -221,
                                  -3000,  -193, -3000,  -277, -3000,  -201, -3000,  -278, -3000,  -199,
                                  -3000,  -279, -3000,  -235, -3000,  -123,    57,  -100,  -164,     2,
                                  -101,  -153,     1,  -102,  -165,    34,  -103, -3000,   -41,  -104,
                                  -284,   -55,  -105,  -186,   -60,  -106,  -203,     0,  -107,  -285,
                                  -1,  -200, -3000,   -62,   -32, -3000,   -66,   -57,  -108,  -204,
                                  -17,  -109, -3000,   -42,  -110,  -243,    -2,  -111,  -202,  -136,
                                  52,   -33,  -112,  -286,    -3,  -122, -3000,   -43,  -124,  -290,
                                  -4,  -125,  -239,    -5, -3000,    57,  -140, -3000, -3000,  -177,
                                  -6, -3000, -3000,  -166,    -7, -3000, -3000,  -181,    33, -3000,
                                  -3000, -3000, -3000, -3000,  -197,    -8, -3000, -3000,  -273,    67,
                                  -3000, -3000, -3000,   -44, -3000, -3000,  -262,    -9, -3000, -3000,
                                  -63,   -32, -3000,   -18, -3000,    56, -3000, -3000, -3000, -3000,
                                  55,    54, -3000, -3000, -3000,   -45, -3000, -3000, -3000, -3000,
                                  -291,    53, -3000, -3000,  -136,    52,  -208,    66,  -141, -3000,
                                  -3000, -3000,  -205,   -10, -3000, -3000, -3000, -3000,  -206,   -11,
                                  -3000, -3000,  -245,    51, -3000,  -144,  -126,  -179,   -12, -3000,
                                  -241,   -13, -3000,   -38, -3000, -3000,  -219,    50, -3000,  -266,
                                  65, -3000, -3000,  -293,   -19, -3000,   -20, -3000, -3000,  -145,
                                  -3000, -3000, -3000, -3000,  -207,   -14, -3000,  -208,  -146, -3000,
                                  -46, -3000,  -127, -3000,   -47, -3000, -3000,   -48, -3000,  -215,
                                  49, -3000,  -128, -3000,  -129,    30,  -184,   -34, -3000,  -246,
                                  48, -3000,   -39, -3000,  -187,   -69, -3000,  -209,   -21, -3000,
                                  -307,   -22, -3000, -3000,    47, -3000,   -49, -3000, -3000,  -130,
                                  -3000, -3000,  -131,    28, -3000, -3000,  -172,   -35, -3000,  -132,
                                  -3000, -3000,    26,    46, -3000,  -210,  -167, -3000,  -173,    45,
                                  -3000, -3000,  -211,    64, -3000, -3000,   -50, -3000,  -317,   -23,
                                  -3000,  -133,  -228, -3000, -3000, -3000,  -143, -3000,    24,    44,
                                  -3000,  -270,    63, -3000, -3000, -3000,    43, -3000, -3000,   -51,
                                  -3000, -3000,  -222,    62, -3000, -3000,   -52, -3000, -3000,  -296,
                                  -24, -3000, -3000,  -228, -3000,    42, -3000, -3000,    41, -3000,
                                  -268,  -115, -3000, -3000, -3000,  -281,   -25, -3000, -3000,  -305,
                                  -26, -3000, -3000,  -147, -3000,  -249,   -27, -3000,  -212,    40,
                                  -3000,  -326,   -28, -3000,    39,  -253,    38, -3000, -3000,   -53,
                                  -3000,  -326,  -328,   -15, -3000,  -149, -3000,  -250,    37, -3000,
                                  -3000,  -328,  -223,    36, -3000, -3000,  -256,    35, -3000, -3000,
                                  -223,  -224,   -29, -3000,  -258,    61, -3000,  -224,   -30, -3000,
                                  -260,    60, -3000, -3000, -3000,  -117, -3000, -3000 };
__ZZSCLASS zztabelem zzpgo[]={

                                 0,   504,   382,   381,   378,   377,   374,   373,   370,   369,
                                 366,   365,   362,   361,   358,   357,   353,   352,   349,   348,
                                 503,   502,   344,    73,   501,   500,   499,   498,   497,   496,
                                 495,   494,   493,   492,   491,   490,   489,   488,   487,   486,
                                 485,   484,   483,   482,   481,   480,   479,   478,   477,   476,
                                 475,   474,   473,   472,   470,   469,   468,   467,   466,   465,
                                 464,   462,   461,   460,   459,   458,   457,   456,   455,   453,
                                 452,   345,   343,   107,   341,     6,   451,   450,   449,   448,
                                 447,   446,   445,   444,   443,   442,   441,   440,   439,   438,
                                 437,   436,   435,   434,   433,   432,   431,   430,   429,     2,
                                 1,     0,   428,   427,   347,   342,   346,   426,   425,   424,
                                 423,   422,   421,     4,     3,   420,   419,   418,   416,   414,
                                 413,   412,   411,   410,   409,   408,   407,   406,   405,   404,
                                 403,   402,   401,   400,   399,   398,   397,   395,   394,   393,
                                 392,   391,   390,   389,   388,   387,   385 };
__ZZSCLASS zztabelem zzr1[]={

                                0,     1,     1,     1,     1,     1,     1,     1,     1,     1,
                                1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
                                1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
                                1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
                                2,     2,     2,    20,    21,    21,    21,    21,    22,    23,
                                23,    23,    23,     3,    24,    25,    28,    28,    28,    29,
                                30,    30,    30,    30,    26,    26,    26,    27,    27,    27,
                                31,    31,    31,     6,    32,    33,    34,    34,    34,     9,
                                35,    36,    37,    37,    37,    38,    38,    38,     7,    39,
                                40,    42,    42,    42,    41,    41,    41,    43,    43,    43,
                                46,    46,    46,    45,    45,    45,    44,    44,    44,    13,
                                47,    48,    49,    49,    49,    50,    50,    50,    18,    51,
                                52,    53,    53,    53,    19,    54,    55,    56,    57,    57,
                                57,    58,    58,    58,    15,    59,    60,    61,    61,    61,
                                14,    62,    63,    64,    65,    65,    65,    66,    66,    66,
                                67,    67,    67,    17,    68,    69,    69,    70,    70,    70,
                                70,    74,    75,    75,    75,    75,    71,    71,    72,    72,
                                73,    73,    73,    16,    76,    77,    78,    79,    79,    79,
                                80,    80,    80,     8,    81,    82,    83,    83,    83,    85,
                                85,    85,    86,    86,    86,    84,    84,    84,    87,    87,
                                87,    10,    88,    89,    89,    90,    91,    91,    91,    92,
                                92,    92,    93,    93,    93,    94,    94,    94,    95,    95,
                                95,    96,    96,    96,    97,    97,    97,    98,    98,    98,
                                98,    99,   100,   101,   101,    11,    11,   102,   103,   103,
                                103,   103,   103,   103,   104,   106,   107,   108,   109,   105,
                                110,   110,   111,   112,   112,   113,   114,   115,   115,    12,
                                116,   117,   118,   119,   120,   121,   121,   121,   122,   122,
                                122,   123,   123,   123,   124,   124,   124,   125,   125,   125,
                                126,   126,   126,   127,   127,   127,   128,   128,   128,   130,
                                130,   130,   129,   129,   129,     4,   131,   132,   133,   134,
                                134,   134,   135,   135,   135,   136,   136,   136,   137,   137,
                                137,   138,   138,   138,   139,   139,   139,   140,   140,   140,
                                141,   141,   141,     5,     5,     5,   142,   143,   144,   144,
                                145,   145,   146,   146 };
__ZZSCLASS zztabelem zzr2[]={

                                0,     4,     4,     4,     4,     4,     4,     4,     4,     4,
                                4,     4,     4,     4,     4,     4,     4,     4,     4,     5,
                                2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
                                2,     2,     2,     2,     2,     2,     2,     2,     3,     5,
                                4,     5,    11,     5,    13,    11,     9,    11,     3,     7,
                                9,     7,     5,    11,     5,    12,     5,     2,     0,     5,
                                5,     5,     2,     0,     5,     2,     0,     5,     2,     0,
                                5,     2,     0,    11,     5,     2,     5,     2,     0,    11,
                                5,     4,     5,     2,     0,     5,     2,     0,    11,     5,
                                12,     5,     2,     0,     5,     2,     0,     5,     2,     0,
                                5,     2,     0,     5,     2,     0,     5,     2,     0,    11,
                                5,     4,     5,     2,     0,     5,     2,     0,    11,     5,
                                2,     5,     2,     0,    11,     5,     6,     5,     5,     2,
                                0,     5,     2,     0,    11,     5,     2,     5,     2,     0,
                                11,     5,     8,     5,     5,     2,     0,     5,     2,     0,
                                5,     2,     0,    11,     5,     8,     6,    11,    13,     9,
                                11,     3,     7,     9,     7,     5,     5,     0,     5,     0,
                                5,     2,     0,    11,     5,     6,     5,     5,     2,     0,
                                5,     2,     0,    11,     5,    10,     5,     2,     0,     5,
                                2,     0,     5,     2,     0,     5,     2,     0,     5,     2,
                                0,    11,     5,    18,    16,     5,     5,     2,     0,     5,
                                2,     0,     5,     2,     0,     5,     2,     0,     5,     2,
                                0,     5,     2,     0,     5,     2,     0,     8,     6,     6,
                                4,     5,     5,     7,     5,    11,     9,     5,     9,     7,
                                6,     4,     2,     4,     4,     5,     5,     5,     5,    11,
                                3,     3,     3,     6,     4,     3,    11,     3,     0,    11,
                                5,    26,     5,     5,     5,     5,     2,     0,     5,     2,
                                0,     5,     2,     0,     5,     2,     0,     5,     2,     0,
                                5,     2,     0,     5,     2,     0,     5,     2,     0,     5,
                                2,     0,     5,     2,     0,    11,     5,    18,     5,     5,
                                2,     0,     5,     2,     0,     5,     2,     0,     5,     2,
                                0,     5,     2,     0,     5,     2,     0,     5,     2,     0,
                                5,     2,     0,     4,     5,    11,     5,     9,     5,     5,
                                5,     5,     5,     3 };
__ZZSCLASS zztabelem zzchk[]={

                                 -3000,    -1,    -2,    -3,    -4,    -5,    -6,    -7,    -8,    -9,
                                 -10,   -11,   -13,   -15,   -14,   -17,   -16,   -18,   -19,   -12,
                                 317,   315,   281,   282,   316,   276,   277,   279,   278,   280,
                                 283,   292,   286,   287,   288,   289,   290,   291,   331,    -2,
                                 -3,    -4,    -5,    -6,    -7,    -8,    -9,   -10,   -11,   -12,
                                 -13,   -14,   -15,   -16,   -17,   -18,   -19,   317,   256,   304,
                                 318,   -20,   257,   -24,   257,  -131,   257,   304,   318,  -142,
                                 257,   -32,   257,   -39,   257,   -81,   257,   -35,   257,   -88,
                                 257,  -102,   257,   -47,   257,   -59,   257,   -62,   257,   -68,
                                 257,   -76,   257,   -51,   257,   -54,   257,  -116,   257,   284,
                                 261,   284,   261,   284,   261,   284,   261,   284,   261,   284,
                                 261,   284,   261,   284,   261,   284,   261,   284,   261,   284,
                                 261,   284,   261,   284,   261,   284,   261,   284,   261,   284,
                                 261,   284,   261,   284,   261,   -21,   295,   -25,   -26,   298,
                                 -132,  -133,   333,  -143,  -144,   298,   -33,   -34,   309,   -40,
                                 -41,   307,   -82,   -83,   310,   -36,   -37,   335,   -89,   -90,
                                 333,  -103,   285,   312,  -104,  -106,  -107,   313,   -48,   -49,
                                 312,   -60,   -61,   309,   -63,   -64,   333,   -69,   -70,   -71,
                                 295,   310,   -77,   -78,   333,   -52,   -53,   309,   -55,   -56,
                                 333,  -117,  -118,   319,   285,   295,   -22,   259,   285,   -27,
                                 299,   261,   285,  -134,   298,   261,   285,  -145,   299,   257,
                                 259,   285,   265,   285,   -42,   335,   266,   285,   -84,   306,
                                 269,   285,   -38,   309,   261,   285,   -91,   335,   261,   285,
                                 312,  -104,  -106,   270,  -105,  -110,   259,   263,  -108,  -109,
                                 272,   271,   267,   285,   -50,   309,   262,   285,   265,   285,
                                 -65,   321,   261,   285,   -71,   295,   -72,   293,   -74,   259,
                                 263,   285,   -79,   335,   261,   285,   265,   285,   -57,   335,
                                 261,   285,  -119,   320,   261,   -22,   296,   -28,   301,   261,
                                 -135,   299,   261,   303,   257,   259,   -43,   308,   261,   -85,
                                 332,   257,   265,   -92,   329,   261,   270,  -105,   262,  -111,
                                 259,   259,   259,   265,   -66,   335,   259,   -72,   -74,   -73,
                                 309,   257,   296,   -80,   309,   261,   -58,   309,   261,  -120,
                                 322,   259,   296,   297,   -23,   268,   -29,   302,   261,  -136,
                                 320,   261,  -146,   264,   -44,   311,   259,   -86,   329,   257,
                                 -93,   336,   262,   262,   296,   -67,   309,   261,   -73,   296,
                                 265,   297,   -75,   268,   265,   265,  -121,   310,   259,   -23,
                                 297,   297,   268,   258,   259,   -30,   305,   263,  -137,   322,
                                 259,   264,   -45,   306,   273,   -87,   309,   262,   -94,   340,
                                 262,  -112,  -113,   259,   265,   297,   -75,   297,   268,   258,
                                 259,  -122,   300,   263,   297,   259,   258,   259,   -31,   309,
                                 298,   299,  -138,   300,   259,   -46,   309,   257,   265,   -95,
                                 341,   262,   297,  -113,  -114,   314,   297,   259,   258,   259,
                                 -123,   330,   257,   259,   265,  -139,   312,   257,   265,   -96,
                                 338,   262,  -114,   259,   259,  -124,   329,   274,  -140,   334,
                                 262,   -97,   339,   262,   296,  -125,   323,   262,  -141,   309,
                                 259,   -98,   -99,   342,   262,  -115,   259,  -126,   324,   259,
                                 265,   -99,  -100,   343,   261,   297,  -127,   322,   325,   259,
                                 -100,  -101,   312,   259,  -128,   326,   259,  -101,   312,   262,
                                 -129,   327,   257,   262,  -130,   328,   257,   275 };
__ZZSCLASS zztabelem zzdef[]={

                                 0,    -2,    20,    21,    22,    23,    24,    25,    26,    27,
                                 28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
                                 38,     0,     0,     0,     0,     0,     0,     0,     0,     0,
                                 0,     0,     0,     0,     0,     0,     0,     0,     0,     1,
                                 2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
                                 12,    13,    14,    15,    16,    17,    18,    19,    39,    40,
                                 41,     0,     0,     0,     0,     0,     0,   323,   324,     0,
                                 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
                                 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
                                 0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
                                 43,    66,    54,     0,   296,     0,   326,    78,    74,    96,
                                 89,   188,   184,    84,    80,     0,   202,     0,   237,   114,
                                 110,   139,   135,     0,   141,   167,   154,     0,   174,   123,
                                 119,     0,   125,     0,   260,     0,     0,     0,    69,    65,
                                 0,   301,     0,     0,     0,     0,     0,    75,    77,     0,
                                 93,    95,     0,   197,   187,     0,    87,    83,     0,   208,
                                 0,     0,   236,     0,     0,   242,     0,     0,     0,   117,
                                 113,     0,   136,   138,     0,   146,     0,     0,   167,   169,
                                 0,     0,     0,   179,     0,     0,   120,   122,     0,   130,
                                 0,     0,     0,     0,    42,     0,     0,    48,    53,    58,
                                 68,    64,   295,   304,   300,   298,   325,     0,     0,   328,
                                 329,    73,    76,    88,    99,    92,    94,   183,   191,   196,
                                 186,    79,    81,    86,    82,   201,   211,   207,   205,   235,
                                 0,     0,   243,     0,   241,     0,   250,   251,   244,   245,
                                 0,     0,   246,   109,   111,   116,   112,   134,   137,   140,
                                 149,   145,   143,   153,   169,     0,   172,     0,     0,   161,
                                 166,   173,   182,   178,   176,   118,   121,   124,   133,   129,
                                 127,   259,     0,     0,   262,     0,     0,     0,    57,    67,
                                 307,   303,   299,     0,   330,   331,   108,    98,    91,   194,
                                 190,   195,    85,   214,   210,   206,     0,   240,   239,     0,
                                 252,   247,   248,   115,   152,   148,   144,   172,     0,   156,
                                 171,   168,     0,   175,   181,   177,   126,   132,   128,   267,
                                 0,   263,     0,    46,     0,     0,    63,     0,    56,   310,
                                 306,   302,   327,   333,   105,   107,    97,   200,   193,   189,
                                 217,   213,   209,   238,     0,   142,   151,   147,   155,     0,
                                 170,   159,     0,     0,   180,   131,   270,   266,   264,     0,
                                 45,    47,     0,     0,    52,    72,    62,    59,   313,   309,
                                 305,   332,   102,   104,   106,   185,   199,   192,   220,   216,
                                 212,     0,     0,   255,   150,   157,     0,   160,     0,     0,
                                 165,   273,   269,   265,    44,    49,     0,    51,    55,    71,
                                 60,    61,   316,   312,   308,    90,   101,   103,   198,   223,
                                 219,   215,   249,     0,   254,     0,   158,   162,     0,   164,
                                 276,   272,   268,    50,    70,   319,   315,   311,   100,   226,
                                 222,   218,   253,     0,   163,   279,   275,   271,   322,   318,
                                 314,   204,   225,   221,   258,   282,   278,   274,   297,   321,
                                 317,   203,     0,     0,   224,     0,   257,   285,   281,   277,
                                 320,     0,   230,     0,   231,   256,   288,     0,   284,   280,
                                 228,   229,     0,   232,   294,   287,   283,   227,     0,   234,
                                 291,   293,   286,   233,   261,   290,   292,   289 };
typedef struct { char *t_name; int t_val; } zztoktype;
#ifndef ZZDEBUG
#	define ZZDEBUG	0	/* don't allow debugging */
#endif

#if ZZDEBUG

__ZZSCLASS zztoktype zztoks[] =
    {
        "DIGIT",	257,
        "DIMDIGIT",	258,
        "VAR",	259,
        "LOCALVAR",	260,
        "BIGVAR",	261,
        "NFILE",	262,
        "IDP",	263,
        "LINEHEX",	264,
        "COMMENT",	265,
        "ANUM",	266,
        "PGCONTEXT",	267,
        "TYPE",	268,
        "RESTYPE",	269,
        "PFILETYPE",	270,
        "EVENT",	271,
        "EVENTSR",	272,
        "RESSYSTEM",	273,
        "YESNO",	274,
        "PASS",	275,
        "SITE",	276,
        "MACHINE",	277,
        "LA",	278,
        "APPLICATIONCLASS",	279,
        "APPLICATION",	280,
        "CONNECTIONCLASS",	281,
        "CONNECTION",	282,
        "PROPERTIES",	283,
        "DEB",	284,
        "FIN",	285,
        "DATABASECLASS",	286,
        "DATABASE",	287,
        "JOURNALCLASS",	288,
        "JOURNAL",	289,
        "QUEUECLASS",	290,
        "QUEUE",	291,
        "SCENARIO",	292,
        "LENGTH",	293,
        "MESSAGE",	294,
        "MESSAGECLASS",	295,
        "OUVRE",	296,
        "FERME",	297,
        "FROM",	298,
        "TO",	299,
        "PORT",	300,
        "TABLE",	301,
        "PROTOCOL",	302,
        "TABXLT",	303,
        "STRFILES",	304,
        "TRANSFERT",	305,
        "COLOR",	306,
        "IPADDRESS",	307,
        "IPNAME",	308,
        "DESCRIPTION",	309,
        "APPTYPE",	310,
        "SYSTEM",	311,
        "PFILE",	312,
        "PPROCEDURE",	313,
        "PEQUAL",	314,
        "DIALOGCLASS",	315,
        "XLTCLASS",	316,
        "CHARERROR",	317,
        "MAXREACH",	318,
        "ONMACHINE",	319,
        "USERNAME",	320,
        "BASENAME",	321,
        "PASSWORD",	322,
        "FHOSTNAME",	323,
        "FUSERNAME",	324,
        "FPASSWORD",	325,
        "FPORT",	326,
        "LOGONTYPE",	327,
        "TRANSTYPE",	328,
        "REPERTORY",	329,
        "CONNECTED",	330,
        "REMOTE",	331,
        "SUBTYPE",	332,
        "CLASS",	333,
        "MODE",	334,
        "LOCALISATION",	335,
        "EXECREPERTORY",	336,
        "LOCALREPERTORY",	337,
        "RECEIVEREPERTORY",	338,
        "SENDREPERTORY",	339,
        "EXECCOMMAND",	340,
        "EXECPARAMETERS",	341,
        "FOLDER",	342,
        "EXTENSION",	343,
        "-unknown-",	-1	/* ends search */
    };

__ZZSCLASS char * zzreds[] =
    {
        "-no such reduction-",
        "Principal : Principal multdialog",
        "Principal : Principal multconclass",
        "Principal : Principal multcon",
        "Principal : Principal multstx",
        "Principal : Principal multsite",
        "Principal : Principal multmachine",
        "Principal : Principal multappliclass",
        "Principal : Principal multla",
        "Principal : Principal multappli",
        "Principal : Principal multproperties",
        "Principal : Principal multremote",
        "Principal : Principal multscenario",
        "Principal : Principal multdatabase",
        "Principal : Principal multdatabaseclass",
        "Principal : Principal multjournal",
        "Principal : Principal multjournalclass",
        "Principal : Principal multqueueclass",
        "Principal : Principal multqueue",
        "Principal : Principal CHARERROR",
        "Principal : multdialog",
        "Principal : multconclass",
        "Principal : multcon",
        "Principal : multstx",
        "Principal : multsite",
        "Principal : multmachine",
        "Principal : multappliclass",
        "Principal : multla",
        "Principal : multappli",
        "Principal : multproperties",
        "Principal : multscenario",
        "Principal : multdatabaseclass",
        "Principal : multdatabase",
        "Principal : multjournalclass",
        "Principal : multjournal",
        "Principal : multqueueclass",
        "Principal : multqueue",
        "Principal : multremote",
        "Principal : CHARERROR",
        "Principal : Principal error",
        "multdialog : DIALOGCLASS STRFILES",
        "multdialog : DIALOGCLASS MAXREACH",
        "multdialog : DIALOGCLASS DName DEB simplemessag FIN",
        "DName : DIGIT BIGVAR",
        "simplemessag : simplemessag MESSAGECLASS messname OUVRE messargu FERME",
        "simplemessag : simplemessag MESSAGECLASS messname OUVRE FERME",
        "simplemessag : MESSAGECLASS messname OUVRE FERME",
        "simplemessag : MESSAGECLASS messname OUVRE messargu FERME",
        "messname : VAR",
        "messargu : messargu TYPE VAR",
        "messargu : messargu TYPE DIMDIGIT VAR",
        "messargu : TYPE DIMDIGIT VAR",
        "messargu : TYPE VAR",
        "multconclass : CONNECTIONCLASS CName DEB simpleport FIN",
        "CName : DIGIT BIGVAR",
        "simpleport : fromcon tocon table protocol transfert Cdescription",
        "table : TABLE BIGVAR",
        "table : TABLE",
        "table : /* empty */",
        "protocol : PROTOCOL IDP",
        "transfert : TRANSFERT FROM",
        "transfert : TRANSFERT TO",
        "transfert : TRANSFERT",
        "transfert : /* empty */",
        "fromcon : FROM BIGVAR",
        "fromcon : FROM",
        "fromcon : /* empty */",
        "tocon : TO BIGVAR",
        "tocon : TO",
        "tocon : /* empty */",
        "Cdescription : DESCRIPTION COMMENT",
        "Cdescription : DESCRIPTION",
        "Cdescription : /* empty */",
        "multsite : SITE SName DEB Sbody FIN",
        "SName : DIGIT BIGVAR",
        "Sbody : Sdescription",
        "Sdescription : DESCRIPTION COMMENT",
        "Sdescription : DESCRIPTION",
        "Sdescription : /* empty */",
        "multla : LA LName DEB Lbody FIN",
        "LName : DIGIT BIGVAR",
        "Lbody : Llocalisation Ldescription",
        "Llocalisation : LOCALISATION BIGVAR",
        "Llocalisation : LOCALISATION",
        "Llocalisation : /* empty */",
        "Ldescription : DESCRIPTION COMMENT",
        "Ldescription : DESCRIPTION",
        "Ldescription : /* empty */",
        "multmachine : MACHINE MName DEB Mbody FIN",
        "MName : DIGIT BIGVAR",
        "Mbody : Mip Mlocalisation Mipname Msystem MColor Mdescription",
        "Mlocalisation : LOCALISATION BIGVAR",
        "Mlocalisation : LOCALISATION",
        "Mlocalisation : /* empty */",
        "Mip : IPADDRESS ANUM",
        "Mip : IPADDRESS",
        "Mip : /* empty */",
        "Mipname : IPNAME VAR",
        "Mipname : IPNAME",
        "Mipname : /* empty */",
        "Mdescription : DESCRIPTION COMMENT",
        "Mdescription : DESCRIPTION",
        "Mdescription : /* empty */",
        "MColor : COLOR DIGIT",
        "MColor : COLOR",
        "MColor : /* empty */",
        "Msystem : SYSTEM RESSYSTEM",
        "Msystem : SYSTEM",
        "Msystem : /* empty */",
        "multscenario : SCENARIO SCEName DEB SCEbody FIN",
        "SCEName : DIGIT BIGVAR",
        "SCEbody : SCEfile SCEdescription",
        "SCEfile : PFILE NFILE",
        "SCEfile : PFILE",
        "SCEfile : /* empty */",
        "SCEdescription : DESCRIPTION COMMENT",
        "SCEdescription : DESCRIPTION",
        "SCEdescription : /* empty */",
        "multqueueclass : QUEUECLASS queueName DEB queuebody FIN",
        "queueName : DIGIT BIGVAR",
        "queuebody : queuedescription",
        "queuedescription : DESCRIPTION COMMENT",
        "queuedescription : DESCRIPTION",
        "queuedescription : /* empty */",
        "multqueue : QUEUE iqueueName DEB iqueuebody FIN",
        "iqueueName : DIGIT BIGVAR",
        "iqueuebody : iqueueclass iqueuelocalisation iqueuedescription",
        "iqueueclass : CLASS BIGVAR",
        "iqueuelocalisation : LOCALISATION BIGVAR",
        "iqueuelocalisation : LOCALISATION",
        "iqueuelocalisation : /* empty */",
        "iqueuedescription : DESCRIPTION COMMENT",
        "iqueuedescription : DESCRIPTION",
        "iqueuedescription : /* empty */",
        "multdatabaseclass : DATABASECLASS dataName DEB databody FIN",
        "dataName : DIGIT BIGVAR",
        "databody : datadescription",
        "datadescription : DESCRIPTION COMMENT",
        "datadescription : DESCRIPTION",
        "datadescription : /* empty */",
        "multdatabase : DATABASE idataName DEB idatabody FIN",
        "idataName : DIGIT BIGVAR",
        "idatabody : idataclass ibasename idatalocalisation idatadescription",
        "idataclass : CLASS BIGVAR",
        "ibasename : BASENAME VAR",
        "ibasename : BASENAME",
        "ibasename : /* empty */",
        "idatalocalisation : LOCALISATION BIGVAR",
        "idatalocalisation : LOCALISATION",
        "idatalocalisation : /* empty */",
        "idatadescription : DESCRIPTION COMMENT",
        "idatadescription : DESCRIPTION",
        "idatadescription : /* empty */",
        "multjournalclass : JOURNALCLASS stoName DEB stobody FIN",
        "stoName : DIGIT BIGVAR",
        "stobody : stomessag stotype stolength stodescription",
        "stobody : stotype stolength stodescription",
        "stomessag : stomessag MESSAGECLASS stomessname OUVRE FERME",
        "stomessag : stomessag MESSAGECLASS stomessname OUVRE stomessargu FERME",
        "stomessag : MESSAGECLASS stomessname OUVRE FERME",
        "stomessag : MESSAGECLASS stomessname OUVRE stomessargu FERME",
        "stomessname : VAR",
        "stomessargu : stomessargu TYPE VAR",
        "stomessargu : stomessargu TYPE DIMDIGIT VAR",
        "stomessargu : TYPE DIMDIGIT VAR",
        "stomessargu : TYPE VAR",
        "stotype : APPTYPE IDP",
        "stotype : /* empty */",
        "stolength : LENGTH DIGIT",
        "stolength : /* empty */",
        "stodescription : DESCRIPTION COMMENT",
        "stodescription : DESCRIPTION",
        "stodescription : /* empty */",
        "multjournal : JOURNAL journalName DEB journalbody FIN",
        "journalName : DIGIT BIGVAR",
        "journalbody : journalclass journallocalisation journaldescription",
        "journalclass : CLASS BIGVAR",
        "journallocalisation : LOCALISATION BIGVAR",
        "journallocalisation : LOCALISATION",
        "journallocalisation : /* empty */",
        "journaldescription : DESCRIPTION COMMENT",
        "journaldescription : DESCRIPTION",
        "journaldescription : /* empty */",
        "multappliclass : APPLICATIONCLASS AName DEB Abody FIN",
        "AName : DIGIT BIGVAR",
        "Abody : Atype AColor Asubtype Arepertory Adescription",
        "Atype : APPTYPE RESTYPE",
        "Atype : APPTYPE",
        "Atype : /* empty */",
        "Asubtype : SUBTYPE DIGIT",
        "Asubtype : SUBTYPE",
        "Asubtype : /* empty */",
        "Arepertory : REPERTORY NFILE",
        "Arepertory : REPERTORY",
        "Arepertory : /* empty */",
        "AColor : COLOR DIGIT",
        "AColor : COLOR",
        "AColor : /* empty */",
        "Adescription : DESCRIPTION COMMENT",
        "Adescription : DESCRIPTION",
        "Adescription : /* empty */",
        "multappli : APPLICATION PrName DEB Prbody FIN",
        "PrName : DIGIT BIGVAR",
        "Prbody : Pronapplication Prlocalisation Prrepertory Prexecrepertory Prexeccommand Prexecparameters Prrecrepertory Prsendrepertory PrFolders",
        "Prbody : Pronapplication Prlocalisation Prrepertory Prexecrepertory Prexeccommand Prexecparameters Prrecrepertory Prsendrepertory",
        "Pronapplication : CLASS BIGVAR",
        "Prlocalisation : LOCALISATION BIGVAR",
        "Prlocalisation : LOCALISATION",
        "Prlocalisation : /* empty */",
        "Prrepertory : REPERTORY NFILE",
        "Prrepertory : REPERTORY",
        "Prrepertory : /* empty */",
        "Prexecrepertory : EXECREPERTORY NFILE",
        "Prexecrepertory : EXECREPERTORY",
        "Prexecrepertory : /* empty */",
        "Prexeccommand : EXECCOMMAND NFILE",
        "Prexeccommand : EXECCOMMAND",
        "Prexeccommand : /* empty */",
        "Prexecparameters : EXECPARAMETERS NFILE",
        "Prexecparameters : EXECPARAMETERS",
        "Prexecparameters : /* empty */",
        "Prrecrepertory : RECEIVEREPERTORY NFILE",
        "Prrecrepertory : RECEIVEREPERTORY",
        "Prrecrepertory : /* empty */",
        "Prsendrepertory : SENDREPERTORY NFILE",
        "Prsendrepertory : SENDREPERTORY",
        "Prsendrepertory : /* empty */",
        "PrFolders : PrFolders PrFolder PrExtension PrFiles",
        "PrFolders : PrFolders PrFolder PrExtension",
        "PrFolders : PrFolder PrExtension PrFiles",
        "PrFolders : PrFolder PrExtension",
        "PrFolder : FOLDER BIGVAR",
        "PrExtension : EXTENSION VAR",
        "PrFiles : PrFiles PFILE NFILE",
        "PrFiles : PFILE NFILE",
        "multproperties : PROPERTIES PName DEB Pbody FIN",
        "multproperties : PROPERTIES PName DEB FIN",
        "PName : DIGIT BIGVAR",
        "Pbody : Pbody PFILE PFILETYPE NFILE",
        "Pbody : PFILE PFILETYPE NFILE",
        "Pbody : Pbody PRSProcedure PVariables",
        "Pbody : PRSProcedure PVariables",
        "Pbody : PProcedure",
        "Pbody : Pbody PProcedure",
        "PRSProcedure : PContext PSREvent",
        "PProcedure : PContext PEvent",
        "PContext : PPROCEDURE PGCONTEXT",
        "PSREvent : EVENTSR VAR",
        "PEvent : EVENT VAR",
        "PVariables : PClass PMessageClass OUVRE PVar FERME",
        "PClass : VAR",
        "PClass : IDP",
        "PMessageClass : VAR",
        "PVar : PVar PTypeVar PVarInstance",
        "PVar : PTypeVar PVarInstance",
        "PTypeVar : VAR",
        "PVarInstance : PEQUAL VAR OUVRE PInitVar FERME",
        "PInitVar : VAR",
        "PInitVar : /* empty */",
        "multremote : REMOTE RName DEB Rbody FIN",
        "RName : DIGIT BIGVAR",
        "Rbody : Ronmachine Rusername Rpassword Rtype Rport Rconnected Rrepertory Rfhostname Rfusername Rfpassword Rfport Rlogontype Rtranstype",
        "Ronmachine : ONMACHINE BIGVAR",
        "Rusername : USERNAME VAR",
        "Rpassword : PASSWORD VAR",
        "Rtype : APPTYPE IDP",
        "Rtype : APPTYPE",
        "Rtype : /* empty */",
        "Rport : PORT DIGIT",
        "Rport : PORT",
        "Rport : /* empty */",
        "Rconnected : CONNECTED YESNO",
        "Rconnected : CONNECTED",
        "Rconnected : /* empty */",
        "Rrepertory : REPERTORY NFILE",
        "Rrepertory : REPERTORY",
        "Rrepertory : /* empty */",
        "Rfhostname : FHOSTNAME VAR",
        "Rfhostname : FHOSTNAME",
        "Rfhostname : /* empty */",
        "Rfusername : FUSERNAME VAR",
        "Rfusername : FUSERNAME",
        "Rfusername : /* empty */",
        "Rfpassword : PASSWORD VAR",
        "Rfpassword : FPASSWORD",
        "Rfpassword : /* empty */",
        "Rfport : FPORT DIGIT",
        "Rfport : FPORT",
        "Rfport : /* empty */",
        "Rtranstype : TRANSTYPE PASS",
        "Rtranstype : TRANSTYPE",
        "Rtranstype : /* empty */",
        "Rlogontype : LOGONTYPE DIGIT",
        "Rlogontype : LOGONTYPE",
        "Rlogontype : /* empty */",
        "multcon : CONNECTION CIName DEB CIsimpleport FIN",
        "CIName : DIGIT BIGVAR",
        "CIsimpleport : CIclass CIfromcon CItocon CIusername CIpassword CIport CIfilename CIreadmode CIdescription",
        "CIclass : CLASS BIGVAR",
        "CIfromcon : FROM BIGVAR",
        "CIfromcon : FROM",
        "CIfromcon : /* empty */",
        "CItocon : TO BIGVAR",
        "CItocon : TO",
        "CItocon : /* empty */",
        "CIusername : USERNAME VAR",
        "CIusername : USERNAME",
        "CIusername : /* empty */",
        "CIpassword : PASSWORD VAR",
        "CIpassword : PASSWORD",
        "CIpassword : /* empty */",
        "CIport : PORT DIGIT",
        "CIport : PORT",
        "CIport : /* empty */",
        "CIfilename : PFILE NFILE",
        "CIfilename : PFILE",
        "CIfilename : /* empty */",
        "CIreadmode : MODE VAR",
        "CIreadmode : MODE",
        "CIreadmode : /* empty */",
        "CIdescription : DESCRIPTION COMMENT",
        "CIdescription : DESCRIPTION",
        "CIdescription : /* empty */",
        "multstx : XLTCLASS STRFILES",
        "multstx : XLTCLASS MAXREACH",
        "multstx : XLTCLASS XName DEB simplestx FIN",
        "XName : DIGIT BIGVAR",
        "simplestx : fromstx tostx TABXLT tablehex",
        "fromstx : FROM DIGIT",
        "fromstx : FROM VAR",
        "tostx : TO DIGIT",
        "tostx : TO VAR",
        "tablehex : tablehex LINEHEX",
        "tablehex : LINEHEX",
    };
#endif /* ZZDEBUG */
#define ZZFLAG  (-3000)
/* @(#) $Revision: 70.7 $ */

/*
** Skeleton parser driver for yacc output
*/

#if defined(NLS) && !defined(NL_SETN)
#include <msgbuf.h>
#endif

#ifndef nl_msg
#define nl_msg(i,s) (s)
#endif

/*
** yacc user known macros and defines
*/
#define ZZERROR		goto zzerrlab

#ifndef __RUNTIME_ZZMAXDEPTH
#define ZZACCEPT	return((GlobalOne *)TheOne)
#define ZZABORT		return(NULL)
#else
#define ZZACCEPT	{free_stacks(); return((GlobalOne*)TheOne);}
#define ZZABORT		{free_stacks(); return(NULL);}
#endif

#define ZZBACKUP( newtoken, newvalue )\
{\
	if ( zzchar >= 0 || ( zzr2[ zztmp ] >> 1 ) != 1 )\
	{\
		zzerror( (nl_msg(30001,"syntax error - cannot backup")) );\
		goto zzerrlab;\
	}\
	zzchar = newtoken;\
	zzstate = *zzps;\
	zzlval = newvalue;\
	goto zznewstate;\
}
#define ZZRECOVERING()	(!!zzerrflag)
#ifndef ZZDEBUG
#	define ZZDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/

#ifdef ZZREENTRANT
__thread int zzdebug;		/* set to 1 to get debugging */
#else
int zzdebug;			/* set to 1 to get debugging */
#endif

/*
** driver internal defines
*/
/* define for ZZFLAG now generated by yacc program. */
/*#define ZZFLAG		(FLAGVAL)*/

/*
** global variables used by the parser
*/
# ifndef __RUNTIME_ZZMAXDEPTH

#   ifdef ZZREENTRANT
__thread __ZZSCLASS ZZSTYPE zzv[ ZZMAXDEPTH ];  /* value stack */
__thread __ZZSCLASS int zzs[ ZZMAXDEPTH ];      /* state stack */
#   else
__ZZSCLASS ZZSTYPE zzv[ ZZMAXDEPTH ];           /* value stack */
__ZZSCLASS int zzs[ ZZMAXDEPTH ];               /* state stack */
#   endif

# else

#  ifdef ZZREENTRANT
__thread __ZZSCLASS ZZSTYPE *zzv;               /* pointer to malloc'ed stack st
value stack */
__thread __ZZSCLASS int *zzs;                   /* pointer to malloc'ed stack st
ack */
#  else
__ZZSCLASS ZZSTYPE *zzv;                        /* pointer to malloc'ed value st
ack */
__ZZSCLASS int *zzs;                            /* pointer to malloc'ed stack stack */
#  endif

#if defined(__STDC__) || defined (__cplusplus)
#include <stdlib.h>
#else
extern char *malloc();
extern char *realloc();
extern void free();
#endif /* __STDC__ or __cplusplus */


static int allocate_stacks();
static void free_stacks();
# ifndef ZZINCREMENT
# define ZZINCREMENT (ZZMAXDEPTH/2) + 10
# endif
# endif	/* __RUNTIME_ZZMAXDEPTH */
long  zzmaxdepth = ZZMAXDEPTH;


#ifdef ZZREENTRANT

__thread __ZZSCLASS ZZSTYPE *zzpv;      /* top of value stack */
__thread __ZZSCLASS int *zzps;          /* top of state stack */

__thread __ZZSCLASS int zzstate;        /* current state */
__thread __ZZSCLASS int zztmp;          /* extra var (lasts between blocks) */

__thread int zznerrs;                   /* number of errors */
__thread __ZZSCLASS int zzerrflag;      /* error recovery flag */
__thread int zzchar;                    /* current input token number */

__thread extern int zzinit_key;           /* init TLS data once */
extern int TLS_INIT;

#else

__ZZSCLASS ZZSTYPE *zzpv;               /* top of value stack */
__ZZSCLASS int *zzps;                   /* top of state stack */

__ZZSCLASS int zzstate;                 /* current state */
__ZZSCLASS int zztmp;                   /* extra var (lasts between blocks) */

int zznerrs;                            /* number of errors */
__ZZSCLASS int zzerrflag;               /* error recovery flag */
int zzchar;
#endif


/*
** zzparse - return 0 if worked, 1 if syntax error not recovered from
l*/
void*
zzparse(XF* pxf)
{
    register ZZSTYPE *zzpvt;	/* top of value stack for $vars */


    /*
    ** Initialize externals - zzparse may be called more than once
    */

#ifdef ZZREENTRANT
    if (zzinit_key != TLS_INIT)
    {
        zzinit_key = TLS_INIT;
        zzinit_tls();
    }
#endif

    /*
    ** Initialize externals - zzparse may be called more than once
    */
# ifdef __RUNTIME_ZZMAXDEPTH
    if (allocate_stacks()) ZZABORT;
# endif
    zzpv = &zzv[-1];
    zzps = &zzs[-1];
    zzstate = 0;
    zztmp = 0;
    zznerrs = 0;
    zzerrflag = 0;
    zzchar = -1;

    goto zzstack;
    {
        register ZZSTYPE *zz_pv;	/* top of value stack */
        register int *zz_ps;		/* top of state stack */
        register int zz_state;		/* current state */
        register int  zz_n;		/* internal state number info */

        /*
        ** get globals into registers.
        ** branch to here only if ZZBACKUP was called.
        */
zznewstate:
        zz_pv = zzpv;
        zz_ps = zzps;
        zz_state = zzstate;
        goto zz_newstate;

        /*
        ** get globals into registers.
        ** either we just started, or we just finished a reduction
        */
zzstack:
        zz_pv = zzpv;
        zz_ps = zzps;
        zz_state = zzstate;

        /*
        ** top of for (;;) loop while no reductions done
        */
zz_stack:
        /*
        ** put a state and value onto the stacks
        */
#if ZZDEBUG
        /*
        ** if debugging, look up token value in list of value vs.
        ** name pairs.  0 and negative (-1) are special values.
        ** Note: linear search is used since time is not a real
        ** consideration while debugging.
        */
        if ( zzdebug )
        {
            register int zz_i;

            printf( "State %d, token ", zz_state );
            if ( zzchar == 0 )
                printf( "end-of-file\n" );
            else if ( zzchar < 0 )
                printf( "-none-\n" );
            else
            {
                for ( zz_i = 0; zztoks[zz_i].t_val >= 0;
                        zz_i++ )
                {
                    if ( zztoks[zz_i].t_val == zzchar )
                        break;
                }
                printf( "%s\n", zztoks[zz_i].t_name );
            }
        }
#endif /* ZZDEBUG */
        if ( ++zz_ps >= &zzs[ zzmaxdepth ] )	/* room on stack? */
        {
# ifndef __RUNTIME_ZZMAXDEPTH
            zzerror( (nl_msg(30002,"yacc stack overflow")) );
            ZZABORT;
# else
            /* save old stack bases to recalculate pointers */
            ZZSTYPE * zzv_old = zzv;
            int * zzs_old = zzs;
            zzmaxdepth += ZZINCREMENT;
            zzs = (int *) realloc(zzs, zzmaxdepth * sizeof(int));
            zzv = (ZZSTYPE *) realloc(zzv, zzmaxdepth * sizeof(ZZSTYPE));
            if (zzs==0 || zzv==0) {
                zzerror( (nl_msg(30002,"yacc stack overflow")) );
                ZZABORT;
            }
            /* Reset pointers into stack */
            zz_ps = (zz_ps - zzs_old) + zzs;
            zzps = (zzps - zzs_old) + zzs;
            zz_pv = (zz_pv - zzv_old) + zzv;
            zzpv = (zzpv - zzv_old) + zzv;
# endif

        }
        *zz_ps = zz_state;
        *++zz_pv = zzval;

        /*
        ** we have a new state - find out what to do
        */
zz_newstate:
        if ( ( zz_n = zzpact[ zz_state ] ) <= ZZFLAG )
            goto zzdefault;		/* simple state */
#if ZZDEBUG
        /*
        ** if debugging, need to mark whether new token grabbed
        */
        zztmp = zzchar < 0;
#endif
        if ( ( zzchar < 0 ) && ( ( zzchar = zzlex(pxf) ) < 0 ) )
            zzchar = 0;		/* reached EOF */
#if ZZDEBUG
        if ( zzdebug && zztmp )
        {
            register int zz_i;

            printf( "Received token " );
            if ( zzchar == 0 )
                printf( "end-of-file\n" );
            else if ( zzchar < 0 )
                printf( "-none-\n" );
            else
            {
                for ( zz_i = 0; zztoks[zz_i].t_val >= 0;
                        zz_i++ )
                {
                    if ( zztoks[zz_i].t_val == zzchar )
                        break;
                }
                printf( "%s\n", zztoks[zz_i].t_name );
            }
        }
#endif /* ZZDEBUG */
        if ( ( ( zz_n += zzchar ) < 0 ) || ( zz_n >= ZZLAST ) )
            goto zzdefault;
        if ( zzchk[ zz_n = zzact[ zz_n ] ] == zzchar )	/*valid shift*/
        {
            zzchar = -1;
            zzval = zzlval;
            zz_state = zz_n;
            if ( zzerrflag > 0 )
                zzerrflag--;
            goto zz_stack;
        }

zzdefault:
        if ( ( zz_n = zzdef[ zz_state ] ) == -2 )
        {
#if ZZDEBUG
            zztmp = zzchar < 0;
#endif
            if ( ( zzchar < 0 ) && ( ( zzchar = zzlex(pxf) ) < 0 ) )
                zzchar = 0;		/* reached EOF */
#if ZZDEBUG
            if ( zzdebug && zztmp )
            {
                register int zz_i;

                printf( "Received token " );
                if ( zzchar == 0 )
                    printf( "end-of-file\n" );
                else if ( zzchar < 0 )
                    printf( "-none-\n" );
                else
                {
                    for ( zz_i = 0;
                            zztoks[zz_i].t_val >= 0;
                            zz_i++ )
                    {
                        if ( zztoks[zz_i].t_val
                                == zzchar )
                        {
                            break;
                        }
                    }
                    printf( "%s\n", zztoks[zz_i].t_name );
                }
            }
#endif /* ZZDEBUG */
            /*
            ** look through exception table
            */
            {
                register int *zzxi = zzexca;

                while ( ( *zzxi != -1 ) ||
                        ( zzxi[1] != zz_state ) )
                {
                    zzxi += 2;
                }
                while ( ( *(zzxi += 2) >= 0 ) &&
                        ( *zzxi != zzchar ) )
                    ;
                if ( ( zz_n = zzxi[1] ) < 0 )
                    ZZACCEPT;
            }
        }

        /*
        ** check for syntax error
        */
        if ( zz_n == 0 )	/* have an error */
        {
            /* no worry about speed here! */
            switch ( zzerrflag )
            {
            case 0:		/* new error */
                zzerror( (nl_msg(30003,"syntax error")) );
                zznerrs++;
                goto skip_init;
zzerrlab:
                /*
                ** get globals into registers.
                ** we have a user generated syntax type error
                */
                zz_pv = zzpv;
                zz_ps = zzps;
                zz_state = zzstate;
                zznerrs++;
skip_init:
            case 1:
            case 2:		/* incompletely recovered error */
                /* try again... */
                zzerrflag = 3;
                /*
                ** find state where "error" is a legal
                ** shift action
                */
                while ( zz_ps >= zzs )
                {
                    zz_n = zzpact[ *zz_ps ] + ZZERRCODE;
                    if ( zz_n >= 0 && zz_n < ZZLAST &&
                            zzchk[zzact[zz_n]] == ZZERRCODE)					{
                        /*
                        ** simulate shift of "error"
                        */
                        zz_state = zzact[ zz_n ];
                        goto zz_stack;
                    }
                    /*
                    ** current state has no shift on
                    ** "error", pop stack
                    */
#if ZZDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
                    if ( zzdebug )
                        printf( _POP_, *zz_ps,
                                zz_ps[-1] );
#	undef _POP_
#endif
                    zz_ps--;
                    zz_pv--;
                }
                /*
                ** there is no state on stack with "error" as
                ** a valid shift.  give up.
                */
                ZZABORT;
            case 3:		/* no shift yet; eat a token */
#if ZZDEBUG
                /*
                ** if debugging, look up token in list of
                ** pairs.  0 and negative shouldn't occur,
                ** but since timing doesn't matter when
                ** debugging, it doesn't hurt to leave the
                ** tests here.
                */
                if ( zzdebug )
                {
                    register int zz_i;

                    printf( "Error recovery discards " );
                    if ( zzchar == 0 )
                        printf( "token end-of-file\n" );
                    else if ( zzchar < 0 )
                        printf( "token -none-\n" );
                    else
                    {
                        for ( zz_i = 0;
                                zztoks[zz_i].t_val >= 0;
                                zz_i++ )
                        {
                            if ( zztoks[zz_i].t_val
                                    == zzchar )
                            {
                                break;
                            }
                        }
                        printf( "token %s\n",
                                zztoks[zz_i].t_name );
                    }
                }
#endif /* ZZDEBUG */
                if ( zzchar == 0 )	/* reached EOF. quit */
                    ZZABORT;
                zzchar = -1;
                goto zz_newstate;
            }
        }/* end if ( zz_n == 0 ) */
        /*
        ** reduction by production zz_n
        ** put stack tops, etc. so things right after switch
        */
#if ZZDEBUG
        /*
        ** if debugging, print the string that is the user's
        ** specification of the reduction which is just about
        ** to be done.
        */
        if ( zzdebug )
            printf( "Reduce by (%d) \"%s\"\n",
                    zz_n, zzreds[ zz_n ] );
#endif
        zztmp = zz_n;			/* value to switch over */
        zzpvt = zz_pv;			/* $vars top of value stack */
        /*
        ** Look in goto table for next state
        ** Sorry about using zz_state here as temporary
        ** register variable, but why not, if it works...
        ** If zzr2[ zz_n ] doesn't have the low order bit
        ** set, then there is no action to be done for
        ** this reduction.  So, no saving & unsaving of
        ** registers done.  The only difference between the
        ** code just after the if and the body of the if is
        ** the goto zz_stack in the body.  This way the test
        ** can be made before the choice of what to do is needed.
        */
        {
            /* length of production doubled with extra bit */
            register int zz_len = zzr2[ zz_n ];

            if ( !( zz_len & 01 ) )
            {
                zz_len >>= 1;
                zzval = ( zz_pv -= zz_len )[1];	/* $$ = $1 */
                zz_state = zzpgo[ zz_n = zzr1[ zz_n ] ] +
                           *( zz_ps -= zz_len ) + 1;
                if ( zz_state >= ZZLAST ||
                        zzchk[ zz_state =
                                   zzact[ zz_state ] ] != -zz_n )
                {
                    zz_state = zzact[ zzpgo[ zz_n ] ];
                }
                goto zz_stack;
            }
            zz_len >>= 1;
            zzval = ( zz_pv -= zz_len )[1];	/* $$ = $1 */
            zz_state = zzpgo[ zz_n = zzr1[ zz_n ] ] +
                       *( zz_ps -= zz_len ) + 1;
            if ( zz_state >= ZZLAST ||
                    zzchk[ zz_state = zzact[ zz_state ] ] != -zz_n )
            {
                zz_state = zzact[ zzpgo[ zz_n ] ];
            }
        }
        /* save until reenter driver code */
        zzstate = zz_state;
        zzps = zz_ps;
        zzpv = zz_pv;
    }
    /*
    ** code supplied by user is placed in this switch
    */
    switch( zztmp )
    {

    case 19:
# line 124 "osresyac.y"
    { ErrorOccurs(CHARACTER); ZZERROR; } break;
    case 38:
# line 143 "osresyac.y"
    { ErrorOccurs(CHARACTER); ZZERROR; } break;
    case 39:
# line 144 "osresyac.y"
    { ErrorOccurs(Errind); ZZERROR; } break;
    case 41:
# line 153 "osresyac.y"
    { Errind = MAXFILE; ZZERROR; } break;
    case 42:
# line 155 "osresyac.y"
    { ListNewr(&TheOne->AllDialog, Dial); } break;
    case 43:
# line 157 "osresyac.y"
        {
            Dial = AllocDialogYaccStruct(zzpvt[-0].Valvar, zzpvt[-1].Valint);
            if (!Dial) { Errind = ALLOCATION; ZZERROR; }
        } break;
    case 44:
# line 162 "osresyac.y"
    { ListNewr(&Dial->AllMessageClasses, MessClass); } break;
    case 45:
# line 164 "osresyac.y"
    {ListNewr(&Dial->AllMessageClasses, MessClass); } break;
    case 46:
# line 166 "osresyac.y"
    {ListNewr(&Dial->AllMessageClasses, MessClass); } break;
    case 47:
# line 168 "osresyac.y"
    {ListNewr(&Dial->AllMessageClasses, MessClass); } break;
    case 48:
# line 170 "osresyac.y"
        {
            MessClass = AllocMessageClassYaccStruct(zzpvt[-0].Valvar);
            if (!MessClass) { Errind = ALLOCATION; ZZERROR; }
        } break;
    case 49:
# line 176 "osresyac.y"
        {
			Argument* Argu;
            Argu = AllocArgumentYaccStruct(zzpvt[-1].Valtyp, 1, zzpvt[-0].Valvar);
            if (!Argu) { Errind = ALLOCATION; ZZERROR; }
            ListNewr(&MessClass->AllArgument, Argu);
        } break;
    case 50:
# line 182 "osresyac.y"
        {
			Argument* Argu;
            Argu = AllocArgumentYaccStruct(zzpvt[-2].Valtyp, zzpvt[-1].Valint, zzpvt[-0].Valvar);
            if (!Argu) { Errind = ALLOCATION; ZZERROR; }
            ListNewr(&MessClass->AllArgument, Argu);
        } break;
    case 51:
# line 188 "osresyac.y"
        {
			Argument* Argu;
            Argu = AllocArgumentYaccStruct(zzpvt[-2].Valtyp, zzpvt[-1].Valint, zzpvt[-0].Valvar);
            if (!Argu) { Errind = ALLOCATION; ZZERROR; }
            ListNewr(&MessClass->AllArgument, Argu);
        } break;
    case 52:
# line 194 "osresyac.y"
        {
			Argument* Argu;
            Argu = AllocArgumentYaccStruct(zzpvt[-1].Valtyp, 1, zzpvt[-0].Valvar);
            if (!Argu) { Errind = ALLOCATION; ZZERROR; }
            ListNewr(&MessClass->AllArgument, Argu);
        } break;
    case 53:
# line 205 "osresyac.y"
    { ListNewr(&TheOne->AllConnectionClasses, ConnClass); } break;
    case 54:
# line 207 "osresyac.y"
        {
            ConnClass = AllocConnectionClassYaccStruct(zzpvt[-0].Valvar, zzpvt[-1].Valint);
            if (!ConnClass) { Errind = ALLOCATION; ZZERROR; }
        } break;
    case 56:
# line 212 "osresyac.y"
    { memcpy(ConnClass->TableName, zzpvt[-0].Valvar + 1, strlen (zzpvt[-0].Valvar) - 2); } break;
    case 59:
# line 214 "osresyac.y"
    { strcpy(ConnClass->Protocol, zzpvt[-0].Valvar); } break;
    case 60:
# line 215 "osresyac.y"
    { strcpy(ConnClass->TransferFormat, "FROM"); } break;
    case 61:
# line 216 "osresyac.y"
    { strcpy(ConnClass->TransferFormat, "TO"); } break;
    case 64:
# line 218 "osresyac.y"
    { memcpy(ConnClass->FromAppliName, zzpvt[-0].Valvar + 1, strlen (zzpvt[-0].Valvar) - 2); } break;
    case 67:
# line 220 "osresyac.y"
    { memcpy(ConnClass->ToAppliName, zzpvt[-0].Valvar + 1, strlen (zzpvt[-0].Valvar) - 2); } break;
    case 70:
# line 222 "osresyac.y"
        {strcpy(ConnClass->Description, zzpvt[-0].Valvar + 1);
            ConnClass->Description[strlen(ConnClass->Description) -1] = 0;} break;
    case 73:
# line 235 "osresyac.y"
    { ListNewr(&TheOne->AllSites, Sit); } break;
    case 74:
# line 237 "osresyac.y"
        {
            Sit = AllocSiteYaccStruct(zzpvt[-0].Valvar, zzpvt[-1].Valint);
            if (!Sit) { Errind = ALLOCATION; ZZERROR; }
        } break;
    case 76:
# line 242 "osresyac.y"
        {strcpy(Sit->Description, zzpvt[-0].Valvar + 1);
            Sit->Description[strlen(Sit->Description) -1] = 0;} break;
    case 79:
# line 250 "osresyac.y"
    { ListNewr(&TheOne->AllLogicalAddress, La); } break;
    case 80:
# line 252 "osresyac.y"
        {
            La = AllocAddressYaccStruct(zzpvt[-0].Valvar, zzpvt[-1].Valint);
            if (!La) { Errind = ALLOCATION; ZZERROR; }
        } break;
    case 82:
# line 257 "osresyac.y"
    {memcpy (La->Localisation,  zzpvt[-0].Valvar + 1, strlen (zzpvt[-0].Valvar) -2);} break;
    case 85:
# line 259 "osresyac.y"
        {strcpy(La->Description, zzpvt[-0].Valvar + 1);
            La->Description[strlen(La->Description) -1] = 0;} break;
    case 88:
# line 268 "osresyac.y"
    { ListNewr(&TheOne->AllMachines, Mach); } break;
    case 89:
# line 270 "osresyac.y"
        {
            Mach = AllocMachineYaccStruct(zzpvt[-0].Valvar, zzpvt[-1].Valint);
            if (!Mach) { Errind = ALLOCATION; ZZERROR; }
        } break;
    case 91:
# line 275 "osresyac.y"
    {memcpy (Mach->Localisation,  zzpvt[-0].Valvar + 1, strlen (zzpvt[-0].Valvar) -2);} break;
    case 94:
# line 277 "osresyac.y"
    { strcpy(Mach->IPAddress, zzpvt[-0].Valvar);} break;
    case 97:
# line 279 "osresyac.y"
    { strcpy(Mach->IPName, zzpvt[-0].Valvar);} break;
    case 100:
# line 281 "osresyac.y"
        { strcpy(Mach->Description, zzpvt[-0].Valvar + 1);
            Mach->Description[strlen(Mach->Description) -1] = 0;} break;
    case 103:
# line 285 "osresyac.y"
    { Mach->Color = zzpvt[-0].Valint;} break;
    case 106:
# line 287 "osresyac.y"
    {strcpy (Mach->System, zzpvt[-0].Valtyp);} break;
    case 109:
# line 297 "osresyac.y"
    { ListNewr(&TheOne->AllScenarios, Sce); } break;
    case 110:
# line 299 "osresyac.y"
        {
            Sce = AllocScenarioYaccStruct(zzpvt[-0].Valvar, zzpvt[-1].Valint);
            if (!Sce) { Errind = ALLOCATION; ZZERROR; }
        } break;
    case 112:
# line 304 "osresyac.y"
    {strcpy (Sce->FileName,  zzpvt[-0].Valvar);} break;
    case 115:
# line 306 "osresyac.y"
        {strcpy(Sce->Description, zzpvt[-0].Valvar + 1);
            Sce->Description[strlen(Sce->Description) -1] = 0;} break;
    case 118:
# line 316 "osresyac.y"
    { ListNewr(&TheOne->AllQueueClasses, Que); } break;
    case 119:
# line 318 "osresyac.y"
        {
            Que = AllocQueueClassYaccStruct(zzpvt[-0].Valvar, zzpvt[-1].Valint);
            if (!Que) { Errind = ALLOCATION; ZZERROR; }
        } break;
    case 121:
# line 323 "osresyac.y"
        {strcpy(Que->Description, zzpvt[-0].Valvar + 1);
            Que->Description[strlen(Que->Description) -1] = 0;} break;
    case 124:
# line 333 "osresyac.y"
    { ListNewr(&TheOne->AllQueues, iQue); } break;
    case 125:
# line 335 "osresyac.y"
        {
            iQue = AllocQueueYaccStruct(zzpvt[-0].Valvar, zzpvt[-1].Valint);
            if (!Que) { Errind = ALLOCATION; ZZERROR; }
        } break;
    case 127:
# line 340 "osresyac.y"
    {memcpy (iQue->Class, zzpvt[-0].Valvar + 1, strlen (zzpvt[-0].Valvar) -2);} break;
    case 128:
# line 341 "osresyac.y"
    {memcpy (iQue->Localisation,  zzpvt[-0].Valvar + 1, strlen (zzpvt[-0].Valvar) -2);} break;
    case 131:
# line 343 "osresyac.y"
        {strcpy(iQue->Description, zzpvt[-0].Valvar + 1);
            iQue->Description[strlen(iQue->Description) -1] = 0;} break;
    case 134:
# line 353 "osresyac.y"
    { ListNewr(&TheOne->AllDataBaseClasses, Data); } break;
    case 135:
# line 355 "osresyac.y"
        {
            Data = AllocDataBaseClassYaccStruct(zzpvt[-0].Valvar, zzpvt[-1].Valint);
            if (!Data) { Errind = ALLOCATION; ZZERROR; }
        } break;
    case 137:
# line 360 "osresyac.y"
        {strcpy(Data->Description, zzpvt[-0].Valvar + 1);
            Data->Description[strlen(Data->Description) -1] = 0;} break;
    case 140:
# line 370 "osresyac.y"
    { ListNewr(&TheOne->AllDataBases, iData); } break;
    case 141:
# line 372 "osresyac.y"
        {
            iData = AllocDataBaseYaccStruct(zzpvt[-0].Valvar, zzpvt[-1].Valint);
            if (!Data) { Errind = ALLOCATION; ZZERROR; }
        } break;
    case 143:
# line 377 "osresyac.y"
    {memcpy (iData->Class, zzpvt[-0].Valvar + 1, strlen (zzpvt[-0].Valvar) -2);} break;
    case 144:
# line 378 "osresyac.y"
    {strcpy (iData->BaseName,  zzpvt[-0].Valvar);} break;
    case 147:
# line 379 "osresyac.y"
    {memcpy (iData->Localisation,  zzpvt[-0].Valvar + 1, strlen (zzpvt[-0].Valvar) -2);} break;
    case 150:
# line 381 "osresyac.y"
        {strcpy(iData->Description, zzpvt[-0].Valvar + 1);
            iData->Description[strlen(iData->Description) -1] = 0;} break;
    case 153:
# line 392 "osresyac.y"
    { ListNewr(&TheOne->AllJournalClasses, Stor); } break;
    case 154:
# line 394 "osresyac.y"
        {
            Stor = AllocJournalClassYaccStruct(zzpvt[-0].Valvar, zzpvt[-1].Valint);
            if (!Stor) { Errind = ALLOCATION; ZZERROR; }
        } break;
    case 157:
# line 401 "osresyac.y"
    {ListNewr(&Stor->AllMessage, MessClass); } break;
    case 158:
# line 403 "osresyac.y"
    {ListNewr(&Stor->AllMessage, MessClass); } break;
    case 159:
# line 405 "osresyac.y"
    {ListNewr(&Stor->AllMessage, MessClass); } break;
    case 160:
# line 407 "osresyac.y"
    {ListNewr(&Stor->AllMessage, MessClass); } break;
    case 161:
# line 409 "osresyac.y"
        {
            MessClass = AllocMessageClassYaccStruct(zzpvt[-0].Valvar);
            if (!MessClass) { Errind = ALLOCATION; ZZERROR; }
        } break;
    case 162:
# line 414 "osresyac.y"
        {
            Argu = AllocArgumentYaccStruct(zzpvt[-1].Valtyp, 1, zzpvt[-0].Valvar);
            if (!Argu) { Errind = ALLOCATION; ZZERROR; }
            ListNewr(&MessClass->AllArgument, Argu);
        } break;
    case 163:
# line 420 "osresyac.y"
        {
            Argu = AllocArgumentYaccStruct(zzpvt[-2].Valtyp, zzpvt[-1].Valint, zzpvt[-0].Valvar);
            if (!Argu) { Errind = ALLOCATION; ZZERROR; }
            ListNewr(&MessClass->AllArgument, Argu);
        } break;
    case 164:
# line 426 "osresyac.y"
        {
            Argu = AllocArgumentYaccStruct(zzpvt[-2].Valtyp, zzpvt[-1].Valint, zzpvt[-0].Valvar);
            if (!Argu) { Errind = ALLOCATION; ZZERROR; }
            ListNewr(&MessClass->AllArgument, Argu);
        } break;
    case 165:
# line 432 "osresyac.y"
        {
            Argu = AllocArgumentYaccStruct(zzpvt[-1].Valtyp, 1, zzpvt[-0].Valvar);
            if (!Argu) { Errind = ALLOCATION; ZZERROR; }
            ListNewr(&MessClass->AllArgument, Argu);
        } break;
    case 166:
# line 437 "osresyac.y"
    { Stor->Format = ((strcmp (zzpvt[-0].Valvar, "STORE") == 0) ? 1 : 0);} break;
    case 168:
# line 438 "osresyac.y"
    { Stor->Length = zzpvt[-0].Valint;} break;
    case 170:
# line 439 "osresyac.y"
        {strcpy(Stor->Description, zzpvt[-0].Valvar + 1);
            Stor->Description[strlen(Stor->Description) -1] = 0;} break;
    case 173:
# line 448 "osresyac.y"
    { ListNewr(&TheOne->AllJournals, iStor); } break;
    case 174:
# line 450 "osresyac.y"
        {
            iStor = AllocJournalYaccStruct(zzpvt[-0].Valvar, zzpvt[-1].Valint);
            if (!iStor) { Errind = ALLOCATION; ZZERROR; }
        } break;
    case 176:
# line 455 "osresyac.y"
    {memcpy (iStor->Class, zzpvt[-0].Valvar + 1, strlen (zzpvt[-0].Valvar) -2);} break;
    case 177:
# line 456 "osresyac.y"
    {memcpy (iStor->Localisation,  zzpvt[-0].Valvar + 1, strlen (zzpvt[-0].Valvar) -2);} break;
    case 180:
# line 458 "osresyac.y"
        {strcpy(iStor->Description, zzpvt[-0].Valvar + 1);
            iStor->Description[strlen(iStor->Description) -1] = 0;} break;
    case 183:
# line 467 "osresyac.y"
    { ListNewr(&TheOne->AllApplicationClasses, AppliClass); } break;
    case 184:
# line 469 "osresyac.y"
        {
            AppliClass = AllocApplicationClassYaccStruct(zzpvt[-0].Valvar, zzpvt[-1].Valint);
            if (!AppliClass) { Errind = ALLOCATION; ZZERROR; }
        } break;
    case 186:
# line 474 "osresyac.y"
    {strcpy (AppliClass->Type, zzpvt[-0].Valtyp);} break;
    case 189:
# line 477 "osresyac.y"
    {AppliClass->SubType = zzpvt[-0].Valint;} break;
    case 192:
# line 479 "osresyac.y"
    {strcpy (AppliClass->Repertory,  zzpvt[-0].Valvar);} break;
    case 195:
# line 482 "osresyac.y"
    { AppliClass->Color = zzpvt[-0].Valint;} break;
    case 198:
# line 484 "osresyac.y"
        {strcpy(AppliClass->Description, zzpvt[-0].Valvar + 1);
            AppliClass->Description[strlen(AppliClass->Description) -1] = 0;} break;
    case 201:
# line 494 "osresyac.y"
    { ListNewr(&TheOne->AllApplications, Prog); } break;
    case 202:
# line 496 "osresyac.y"
        {
            Prog = AllocApplicationYaccStruct(zzpvt[-0].Valvar, zzpvt[-1].Valint);
            if (!Prog) { Errind = ALLOCATION; ZZERROR; }
        } break;
    case 205:
# line 502 "osresyac.y"
    {memcpy (Prog->Class, zzpvt[-0].Valvar + 1, strlen (zzpvt[-0].Valvar) -2);} break;
    case 206:
# line 503 "osresyac.y"
    {memcpy (Prog->Localisation,  zzpvt[-0].Valvar + 1, strlen (zzpvt[-0].Valvar) -2);} break;
    case 209:
# line 505 "osresyac.y"
    {strcpy (Prog->Repertory,  zzpvt[-0].Valvar);} break;
    case 212:
# line 507 "osresyac.y"
    {strcpy (Prog->ExecRepertory, zzpvt[-0].Valvar);} break;
    case 215:
# line 509 "osresyac.y"
    { strcpy (Prog->ExecCommand, zzpvt[-0].Valvar); } break;
    case 218:
# line 511 "osresyac.y"
    {strcpy (Prog->ExecParameters, zzpvt[-0].Valvar);} break;
    case 221:
# line 513 "osresyac.y"
    {strcpy (Prog->ReceiveRepertory,  zzpvt[-0].Valvar);} break;
    case 224:
# line 515 "osresyac.y"
    {strcpy (Prog->SendRepertory, zzpvt[-0].Valvar);} break;
    case 231:
# line 522 "osresyac.y"
        {
            FolderArgu = AllocFolderArgumentYaccStruct(zzpvt[-0].Valvar, "");
            if (!FolderArgu) { Errind = ALLOCATION; ZZERROR; }
            AddFolderToApplication (Prog, FolderArgu);
        } break;
    case 232:
# line 528 "osresyac.y"
        {
            strcpy (FolderArgu->Extensions , zzpvt[-0].Valvar);
        } break;
    case 233:
# line 532 "osresyac.y"
        {
            FileArgu = AllocFileArgumentYaccStruct(zzpvt[-0].Valvar, "OTHER");
            if (!FileArgu) { Errind = ALLOCATION; ZZERROR; }
            AddFileToFolder (FolderArgu, FileArgu);
        } break;
    case 234:
# line 538 "osresyac.y"
        {
            FileArgu = AllocFileArgumentYaccStruct(zzpvt[-0].Valvar, "OTHER");
            if (!FileArgu) { Errind = ALLOCATION; ZZERROR; }
            AddFileToFolder (FolderArgu, FileArgu);
        } break;
    case 235:
# line 548 "osresyac.y"
    { ListNewr(&TheOne->AllProperties, Pro); } break;
    case 236:
# line 550 "osresyac.y"
    { ListNewr(&TheOne->AllProperties, Pro); } break;
    case 237:
# line 552 "osresyac.y"
        {
            Pro = AllocPropertyYaccStruct(zzpvt[-0].Valvar, zzpvt[-1].Valint);
            if (!Pro) { Errind = ALLOCATION; ZZERROR; }
        } break;
    case 238:
# line 557 "osresyac.y"
        {
            FileArgu = AllocFileArgumentYaccStruct(zzpvt[-0].Valvar, zzpvt[-1].Valtyp);
            if (!FileArgu) { Errind = ALLOCATION; ZZERROR; }
            AddFileToProperty (Pro, FileArgu);
        } break;
    case 239:
# line 563 "osresyac.y"
        {
            FileArgu = AllocFileArgumentYaccStruct(zzpvt[-0].Valvar, zzpvt[-1].Valtyp);
            if (!FileArgu) { Errind = ALLOCATION; ZZERROR; }
            AddFileToProperty (Pro, FileArgu);
        } break;
    case 245:
# line 574 "osresyac.y"
        {
        } break;
    case 246:
# line 577 "osresyac.y"
        {
            ProcArgu = AllocProcArgumentYaccStruct("","",zzpvt[-0].Valtyp);
            if (!ProcArgu) { Errind = ALLOCATION; ZZERROR; }
            AddProcToProperty (Pro, ProcArgu);
        } break;
    case 247:
# line 583 "osresyac.y"
        {
            strcpy (ProcArgu->Type, zzpvt[-1].Valtyp);
            strcpy (ProcArgu->Name, zzpvt[-0].Valvar);
        } break;
    case 248:
# line 588 "osresyac.y"
        {
            strcpy (ProcArgu->Type, zzpvt[-1].Valtyp);
            strcpy (ProcArgu->Name, zzpvt[-0].Valvar);
        } break;
    case 249:
# line 593 "osresyac.y"
        {
        } break;
    case 250:
# line 595 "osresyac.y"
    { strcpy (ProcArgu->Class, zzpvt[-0].Valvar);} break;
    case 251:
# line 596 "osresyac.y"
    { strcpy (ProcArgu->Class, zzpvt[-0].Valvar);} break;
    case 252:
# line 597 "osresyac.y"
    { strcpy (ProcArgu->MessageClass, zzpvt[-0].Valvar);} break;
    case 255:
# line 601 "osresyac.y"
        {
            VarArgu = AllocVarArgumentYaccStruct("", zzpvt[-0].Valvar, "");
            if (!VarArgu) { Errind = ALLOCATION; ZZERROR; }
            AddVarToProcedure (ProcArgu, VarArgu);
        } break;
    case 256:
# line 607 "osresyac.y"
        {
            strcpy (VarArgu->Name, zzpvt[-3].Valvar);
        } break;
    case 257:
# line 610 "osresyac.y"
    {strcpy (VarArgu->Init, zzpvt[-0].Valvar);} break;
    case 259:
# line 619 "osresyac.y"
    { ListNewr(&TheOne->AllRemotes, remote); } break;
    case 260:
# line 621 "osresyac.y"
        {
            remote = AllocRemoteYaccStruct(zzpvt[-0].Valvar, zzpvt[-1].Valint);
            if (!remote) { Errind = ALLOCATION; ZZERROR; }
        } break;
    case 262:
# line 626 "osresyac.y"
    {memcpy (remote->OnMachine, zzpvt[-0].Valvar + 1, strlen (zzpvt[-0].Valvar) - 2);} break;
    case 263:
# line 627 "osresyac.y"
    {strcpy (remote->UserName,  zzpvt[-0].Valvar);} break;
    case 264:
# line 628 "osresyac.y"
    {strcpy (remote->PassWord,  zzpvt[-0].Valvar);} break;
    case 265:
# line 629 "osresyac.y"
    {strcpy (remote->Type, zzpvt[-0].Valvar);} break;
    case 268:
# line 630 "osresyac.y"
    { remote->Port = zzpvt[-0].Valint; } break;
    case 271:
# line 631 "osresyac.y"
    {remote->Connected = (strcmp (zzpvt[-0].Valtyp, "YES") ? 1 : 0);} break;
    case 274:
# line 632 "osresyac.y"
    {strcpy (remote->DistantRepertory,  zzpvt[-0].Valvar);} break;
    case 277:
# line 633 "osresyac.y"
    {strcpy (remote->FHostName, zzpvt[-0].Valvar);} break;
    case 280:
# line 634 "osresyac.y"
    {strcpy (remote->FUserName,  zzpvt[-0].Valvar);} break;
    case 283:
# line 635 "osresyac.y"
    {strcpy (remote->FPassWord,  zzpvt[-0].Valvar);} break;
    case 286:
# line 636 "osresyac.y"
    { remote->FPort = zzpvt[-0].Valint; } break;
    case 289:
# line 637 "osresyac.y"
    {remote->TransType = ((strcmp (zzpvt[-0].Valtyp, "PASSIVE") == 0) ? 1 : 0);} break;
    case 292:
# line 638 "osresyac.y"
    {remote->LogonType = zzpvt[-0].Valint;} break;
    case 295:
# line 644 "osresyac.y"
    { ListNewr(&TheOne->AllConnections, Conn); } break;
    case 296:
# line 646 "osresyac.y"
        {
            Conn = AllocConnectionYaccStruct(zzpvt[-0].Valvar, zzpvt[-1].Valint);
            if (!Conn) { Errind = ALLOCATION; ZZERROR; }
        } break;
    case 298:
# line 651 "osresyac.y"
    {memcpy (Conn->Class, zzpvt[-0].Valvar + 1, strlen (zzpvt[-0].Valvar) -2);} break;
    case 299:
# line 652 "osresyac.y"
    { memcpy(Conn->FromAppliName, zzpvt[-0].Valvar + 1, strlen (zzpvt[-0].Valvar) - 2); } break;
    case 302:
# line 654 "osresyac.y"
    { memcpy(Conn->ToAppliName, zzpvt[-0].Valvar + 1, strlen (zzpvt[-0].Valvar) - 2); } break;
    case 305:
# line 656 "osresyac.y"
    {strcpy (Conn->UserName,  zzpvt[-0].Valvar);} break;
    case 308:
# line 657 "osresyac.y"
    {strcpy (Conn->PassWord,  zzpvt[-0].Valvar);} break;
    case 311:
# line 658 "osresyac.y"
    { Conn->Port = zzpvt[-0].Valint; } break;
    case 314:
# line 659 "osresyac.y"
    { strcpy (Conn->FileName,  zzpvt[-0].Valvar);} break;
    case 317:
# line 660 "osresyac.y"
    {strcpy (Conn->ReadMode,  zzpvt[-0].Valvar);} break;
    case 320:
# line 661 "osresyac.y"
        {strcpy(Conn->Description, zzpvt[-0].Valvar + 1);
            Conn->Description[strlen(Conn->Description) -1] = 0;} break;
    case 324:
# line 672 "osresyac.y"
    { Errind = MAXFILE; ZZERROR; } break;
    case 325:
# line 674 "osresyac.y"
    { ListNewr(&TheOne->AllXlatStruct, VarXlat); } break;
    case 326:
# line 676 "osresyac.y"
        {
            VarXlat = AllocXlatYaccStruct(zzpvt[-0].Valvar, zzpvt[-1].Valint);
            if (!VarXlat) {Errind = ALLOCATION; ZZERROR; }
        } break;
    case 327:
# line 681 "osresyac.y"
        {
            if (Verif != 16) { Verif = 0; Errind = XLTCAPTION; ZZERROR; }
            Verif = 0;
        } break;
    case 328:
# line 685 "osresyac.y"
    { sprintf(VarXlat->FromName, "%d\0", zzpvt[-0].Valint); } break;
    case 329:
# line 686 "osresyac.y"
    { strcpy(VarXlat->FromName, zzpvt[-0].Valvar); } break;
    case 330:
# line 687 "osresyac.y"
    { sprintf(VarXlat->ToName, "%d\0", zzpvt[-0].Valint); } break;
    case 331:
# line 688 "osresyac.y"
    { strcpy(VarXlat->ToName, zzpvt[-0].Valvar); } break;
    case 332:
# line 690 "osresyac.y"
        {
            if (Verif >= 16) { Verif = 0; Errind = XLTCAPTION; ZZERROR; }
            strcpy((char *)VarXlat->Tabxlt[Verif], zzpvt[-0].Valvar);
            Verif++;
        } break;
    case 333:
# line 696 "osresyac.y"
        {
            if (Verif >= 16) { Verif = 0; Errind = XLTCAPTION; ZZERROR; }
            strcpy((char *)VarXlat->Tabxlt[Verif], zzpvt[-0].Valvar);
            Verif++;
        } break;
    }
    goto zzstack;		/* reset registers in driver code */
}

# ifdef __RUNTIME_ZZMAXDEPTH

static int allocate_stacks() {
    /* allocate the zzs and zzv stacks */
    zzs = (int *) malloc(zzmaxdepth * sizeof(int));
    zzv = (ZZSTYPE *) malloc(zzmaxdepth * sizeof(ZZSTYPE));

    if (zzs==0 || zzv==0) {
        zzerror( (nl_msg(30004,"unable to allocate space for yacc stacks")) );
        return(1);
    }
    else return(0);

}


static void free_stacks() {
    if (zzs!=0) free((char *) zzs);
    if (zzv!=0) free((char *) zzv);
}

# endif  /* defined(__RUNTIME_ZZMAXDEPTH) */

