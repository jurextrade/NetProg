/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "database.h"



char* DBTreatHeader (OSSqlda* psqlda, WORD* size)
{
    WORD     Size = 0;
    WORD     CurrentSize = 0;
    char     Column[100];
    int      i;
    char*    buffer;

    *size = 0;

    buffer = (char*)malloc (100);

    for (i = 0; i < psqlda->ColumnsNumber; i++)
    {
        sprintf (Column, "%03d%04ld%04ld%.*s", psqlda->Description[i]->Type,
                 psqlda->Description[i]->Length,
                 psqlda->Description[i]->ItemLength,
                 (int) psqlda->Description[i]->ItemLength,
                 psqlda->Description[i]->Name);
        CurrentSize = (WORD)12 + (WORD)psqlda->Description[i]->ItemLength;
        buffer = (char *)realloc (buffer, Size + CurrentSize);
        memcpy (buffer + Size , Column, CurrentSize);
        Size += CurrentSize;
    }
    *size = Size;
    return buffer;
}


int DBAckProcedure (void* ack, void* par)
{
    MXMessage*  pmessage;
    OSStatement* pstatement = (OSStatement*)par;
    TPClient*  pclient = (TPClient*)pstatement->UserField;
    MXCom* pcom = (MXCom*)pclient->ApplicationField;
    OSAck*      pack    = (OSAck *)ack;


    pstatement->Status = OSREADACK;
    pmessage = MXCreateMessage (pcom->MX, DB_SYS, "AckStatement");
    MXSetValue (pmessage,  "RowsProcessed",    1, &pack->RowsProcessed);
    MXSetValue (pmessage,  "SqlCode",          1, &pack->SqlCode) ;
    MXSetValue (pmessage,  "SqlErrMsg",        1, pack->SqlErrMsg) ;
    MXSetValue (pmessage,  "Print",            1, &pack->Print) ;
    MXSetValue (pmessage,  "Stop",             1, &pack->Stop) ;
    MXSetValue (pmessage,  "SqlStatement",     1, pstatement->SqlStatement);
    pmessage->Complete = 1;
    ListNewr (&pcom->InputMessages, pmessage);
    pmessage->InternalField = pstatement;
    return 1;
}


int DBHeaderProcedure (void* sqlda, void* par)
{
    MXMessage* pmessage;
    MXMessageClass* pmessageclass;
    MXObject* pobject;
    OSStatement* pstatement = (OSStatement*)par;
    TPClient*  pclient = (TPClient*)pstatement->UserField;
    MXCom* pcom = (MXCom*)pclient->ApplicationField;
    int i;
    OSSqlda*   psqlda = (OSSqlda *)sqlda;

    pstatement->Status = OSREADHEADER;
    if (strcmp (pstatement->Name, "") != 0)
    {
        char columnname[300];
        MXDialogClass* pclass = MXGetDialogClassFromName (pcom->MX, DB_SYS);
        pmessageclass = MXGetMessageClassFromName (pclass, pstatement->Name);
        if (!pmessageclass)
        {
            pmessageclass = MXCreateMessageClass(pclass->MX, pclass, pstatement->Name, 100, 0);
            if (!pmessageclass) return -1;
        }
        MXFreeObjectsFromMessageClass (pmessageclass);
        for (i = 0; i < psqlda->ColumnsNumber; i++)
        {
            sprintf (columnname, "%.*s", (int)psqlda->Description[i]->ItemLength, psqlda->Description[i]->Name);
            pobject = MXAddObjectToMessageClass (pmessageclass, "STRING", pstatement->Query->Every, columnname);
         //   pobject = MXAddObjectToMessageClass (pmessageclass, "STRING", 1, columnname);
            pobject->InternalType   = psqlda->Description[i]->Type;
            pobject->InternalScale  = psqlda->Description[i]->Scale;
            pobject->InternalLength = psqlda->Description[i]->Length - 1;
            if (pcom->MX->CGI->FileName[0])
                CGIAddColumn (pcom->MX->CGI, columnname, (int)psqlda->Description[i]->ItemLength, (int)psqlda->Description[i]->Length, NULL);
        }
        pstatement->MessageClass = pmessageclass;
        pmessage = MXCreateMessage (pcom->MX, DB_SYS, "ResultHeader");
        MXSetValue (pmessage, "ClassName", 1, DB_SYS);
        MXSetValue (pmessage, "QueryName", 1, pstatement->Name);
        ListNewr (&pcom->InputMessages, pmessage);
        pmessage->Complete = 1;
        
    }
    else
    {
        BUFFERPARM Buffer;
        char*      buffer;
        WORD       Size = 0;
        pmessage = MXCreateMessage (pcom->MX, DB_SYS, "ResultHeader");
        buffer = DBTreatHeader (psqlda, &Size);

        Buffer.BufferType    = 'T';
        Buffer.BufferSize    = Size;
        Buffer.BufferContent = buffer ;

        MXSetValue (pmessage, "ColumnsNumber", 1, &psqlda->ColumnsNumber);
        MXSetValue (pmessage, "Buffer",        1, &Buffer);
        ListNewr (&pcom->InputMessages, pmessage);
        pmessage->Complete = 1;

        free (buffer);
    }
    return 1 ;
}


int DBContentsProcedure (void* sqlda, void* par)
{
    MXMessage* pmessage;
    OSStatement* pstatement = (OSStatement*)par;
    TPClient*  pclient = (TPClient*)pstatement->UserField;
    MXCom* pcom = (MXCom*)pclient->ApplicationField;
    OSSqlda* psqlda = (OSSqlda*)sqlda;
    BUFFERPARM Buffer;

    OSContext* pcontext;

    DWORD i = 0;
    char columnname[100];
    int j = 0;
    char* values;
    char* Pvalues;
    int range = 0;


    if (!pstatement) return -1;

    pstatement->Status = OSREADCONTENT;
    pcontext = psqlda->Context[psqlda->ContextNumber-1];
    values = psqlda->Context[psqlda->ContextNumber-1]->Values;

    if (pstatement->MessageClass != NULL)
    {
        if (pcontext->Size != 0)
        {
            pmessage = MXCreateMessage (pcom->MX, DB_SYS, pstatement->Name);
            ListNewr (&pcom->InputMessages, pmessage);
            pmessage->Complete = 1;
            while (i < pcontext->Size)
            {
            
                Pvalues = values;
                for (j = 0; j < psqlda->ColumnsNumber; j++)
                {
                    char* buffer;
                    sprintf (columnname, "%.*s", (int)psqlda->Description[j]->ItemLength, psqlda->Description[j]->Name);
                    buffer = (char*)malloc ((int)psqlda->Description[j]->Length);
                    if (!buffer) return -1;
                    memset (buffer, 0, (int)psqlda->Description[j]->Length);
                    memcpy (buffer, Pvalues, (int)psqlda->Description[j]->Length - 1);
                    MXSetRangeValue (pmessage, j + 1, range + 1, buffer);
                    //MXSetRangeValue (pmessage, j + 1, 1, buffer);
                    free (buffer);
                    if (pcom->MX->CGI->FileName[0])
                        CGIAddValue (pcom->MX->CGI, columnname, Pvalues);
                    Pvalues += (int)psqlda->Description[j]->Length ;
                }
                range++;
                i+= psqlda->size + 1;
                values+= psqlda->size + 1;
            }
            pmessage->NbrRows = range;

        }
    }
    else
    {
        Buffer.BufferType    = 'T';
        Buffer.BufferSize    = psqlda->Context[psqlda->ContextNumber-1]->Size;
        Buffer.BufferContent = psqlda->Context[psqlda->ContextNumber-1]->Values;

        pmessage = MXCreateMessage (pcom->MX, DB_SYS, pstatement->Name);
        MXSetValue (pmessage, "RowsNumber", 1, &psqlda->RowsNumber);
        MXSetValue (pmessage, "Buffer",     1, &Buffer);
        MXSetValue (pmessage, "LastContext",1, &psqlda->LastContext);
        ListNewr (&pcom->InputMessages, pmessage);
        pmessage->Complete = 1;
    }
    return 1;
}

MXMessage* DBCheckAndReturnMessage (MX* pmx, MXCom* pcom, char* stream)
{
    MXMessage* pmessage = NULL;

    if (pcom->InputMessages)
    {
        pmessage = (MXMessage*)pcom->InputMessages->car;
    }
    return pmessage;
}


LONG DBReceiveMessage (TP* ptp, TPClient* pclient, char* buf, int count)
{
    char* stream;
    MX* pmx;
    MXMessage* pmessage;
    LONG ret = 0;
    List* plist;
    MXCallBack* pexecutecomprocedure;
    MXCallBack* pexecuteprocedure;


    MXCom* pcom = (MXCom*)pclient->ApplicationField;
    pmx = pcom->MX;


    if ((!pcom) || (!pcom->Client))
        return -1;


    stream = buf;
    pmessage = DBCheckAndReturnMessage (pmx, pcom, stream);
    if (!pmessage)
        return 1;
    if (pmessage->InternalField)
    {
        PIPE* pipe = pclient->Pipe;
        OSStatement* pstatement = (OSStatement*)pmessage->InternalField;
        ListRemove ((List**)&pipe->Input, pstatement);
        OSFreeStatement (pstatement);
        pmessage->InternalField = NULL;
    }
    pmessage->Com = pcom;

    if (pcom->TraceProcedure)  pcom->TraceProcedure (pmessage, MXONRECV, buf, count);



    TPSetBlockingMode (pcom->Client, pmessage->MessageClass->BlockedOnInput);
    if (pmessage->Complete)
    {

        TPSetBlockingMode (pcom->Client, MXNoBlock);
        ListRemove(&pcom->InputMessages, pmessage);

        /* Commentaire */
        plist = pmx->ExecuteInputs;
        while (plist)
        {
            pexecuteprocedure = (MXCallBack *)plist->car;
            plist = plist->cdr;
            pexecuteprocedure->ExecuteProcedure (pmessage, pcom, pexecuteprocedure->ApplicationField);
        }
        plist = pmessage->MessageClass->ExecuteInputs;
        while (plist)
        {
            pexecuteprocedure = (MXCallBack *)plist->car;
            plist = plist->cdr;
            pexecuteprocedure->ExecuteProcedure (pmessage, pcom, pexecuteprocedure->ApplicationField);
        }
        if (pcom->ComClass)
        {
            plist = pcom->ComClass->ExecuteInputs;
            while (plist)
            {
                if ((!pcom->Client) || (pcom->Closed)) return 1;
                pexecutecomprocedure = (MXCallBack *)plist->car;
                plist = plist->cdr;
                if (pexecutecomprocedure->MessageClass == pmessage->MessageClass)
                    pexecutecomprocedure->ExecuteProcedure (pmessage, pcom, pexecutecomprocedure->ApplicationField);
            }
        }
        plist = pcom->ExecuteInputs;
        while (plist)
        {
            if ((!pcom->Client) || (pcom->Closed)) return 1;
            pexecutecomprocedure = (MXCallBack *)plist->car;
            plist = plist->cdr;
            if (pexecutecomprocedure->MessageClass == pmessage->MessageClass)
                pexecutecomprocedure->ExecuteProcedure (pmessage, pcom, pexecutecomprocedure->ApplicationField);
        }
        MXFreeMessage (pcom->MX, pmessage);
    }
    return ret;
}



LONG DBWriteMessage (MX* pmx, MXMessage* pmessage, char* stream, int maxcount, BYTE WithFileContent)
{
    OSStatement* pstatement;
    OSQuery*     pquery;
    MXCom* pcom = pmessage->Com;



    STRING SqlStatement   =  (char *) MXGetValue (pmessage, "SqlStatement",   1);
    pstatement = OSCreateStatement (NULL, SqlStatement, FALSE);
    if (!pstatement) return -1;

    OSSetProcedurePar(pstatement, pstatement);
    OSSetAckProcedure(pstatement, DBAckProcedure);
    if (strcmp (pmessage->MessageClass->Name, "CommitOrRollback") == 0)
        pstatement->SqlType = DBSQLCOMMIT;

    if (strcmp (pmessage->MessageClass->Name, "ExecuteQuery") != 0)
    {
        if (MXGetValue (pmessage, "Commit",         1))
            pstatement->Commit = *(BYTE *) MXGetValue (pmessage, "Commit",         1);
    }
    else
    {
        STRING Name           = (STRING) MXGetValue (pmessage, "Name",           1);
        DWORD  Every          =  *(DWORD*) MXGetValue (pmessage, "Every",          1);
        DWORD  MaxRows        =  *(DWORD*) MXGetValue (pmessage, "MaxRows",        1);
        pquery = OSCreateQuery (Every, MaxRows);
        if (!pquery )
        {
            OSFreeStatement (pstatement);
            return -1;
        }
        OSSetHeaderProcedure(pquery,   DBHeaderProcedure);
        OSSetContentsProcedure(pquery, DBContentsProcedure);
        OSSetProcedurePar(pquery, pstatement);
        pstatement->Query = pquery;
        strcpy (pstatement->Name, Name);
    }
    ListNewr ((List**)&pcom->Client->Pipe->Output, pstatement);
    pmessage->Complete = 1;
    MXInitMessage (pmessage);
    return 1 ;
}


LONG DBSendMessage (TP* ptp, TPClient* pclient, char* buf, int size)
{
    LONG ret;
    MX* pmx;

    MXMessage* pmessage;
    MXCallBack* pexecutecomprocedure;
    MXCallBack* pexecuteprocedure;
    List* plist;
    /* now hopinng that values are set */
    MXCom* pcom = (MXCom*)pclient->ApplicationField;
    pmx = pcom->MX;


    pmessage = MXGetReadyOutputMessage (pcom);
    if (!pmessage) return 0;
    if (pmessage && (!MXReadyToSend (pmessage))) return -3;

    pmessage->Com = pcom;
    if (!pmessage->Context)
    {

        plist = pmx->ExecuteOutputs;
        while (plist)
        {
            pexecuteprocedure = (MXCallBack *)plist->car;
            plist = plist->cdr;
            pexecuteprocedure->ExecuteProcedure (pmessage, pcom, pexecuteprocedure->ApplicationField);
        }
        plist = pmessage->MessageClass->ExecuteOutputs;
        while (plist)
        {
            pexecuteprocedure = (MXCallBack *)plist->car;
            plist = plist->cdr;
            pexecuteprocedure->ExecuteProcedure (pmessage, pcom, pexecuteprocedure->ApplicationField);
        }
        if (pcom->ComClass)
        {
            plist = pcom->ComClass->ExecuteOutputs;
            while (plist)
            {
                if ((!pcom->Client) || (pcom->Closed)) return 1;
                pexecutecomprocedure = (MXCallBack *)plist->car;
                plist = plist->cdr;
                if (pexecutecomprocedure->MessageClass == pmessage->MessageClass)
                    pexecutecomprocedure->ExecuteProcedure (pmessage, pcom, pexecutecomprocedure->ApplicationField);
            }
        }
        plist = pcom->ExecuteOutputs;
        while (plist)
        {
            if ((!pcom->Client) || (pcom->Closed)) return 1;
            pexecutecomprocedure = (MXCallBack *)plist->car;
            plist = plist->cdr;
            if (pexecutecomprocedure->MessageClass == pmessage->MessageClass)
                pexecutecomprocedure->ExecuteProcedure (pmessage, pcom, pexecutecomprocedure->ApplicationField);
        }
    }

    if (pcom->Closed)
    {
        MXFreeMessage (pcom->MX, pmessage);
        return 1;
    }

    TPSetBlockingMode (pcom->Client, pmessage->MessageClass->BlockedOnOutput);

    ret = DBWriteMessage (pmx, pmessage, buf, size , 0); /*client->TransSize - tp header;*/

    if (ret < 0)
    {
        ListRemove(&pcom->OutputMessages, pmessage);
        MXFreeMessage (pcom->MX, pmessage);
        return -1;
    }
    if (pcom->TraceProcedure) pcom->TraceProcedure (pmessage, MXONSEND, buf, ret);

    if (pmessage->Complete)
    {
        TPSetBlockingMode (pcom->Client, MXNoBlock);
        ListRemove (&pcom->OutputMessages, pmessage);
        if (!(pmessage->Type & MXACK))
            MXFreeMessage (pcom->MX, pmessage);
        else
            ListNewr (&pcom->AckMessages, pmessage);
    }

    return ret;
}



LONG DBSend (MX* pmx, MXCom* pcom, MXMessage* pmessage)
{
    LONG ret, ret1;
    List* plist;
    MXCallBack*  pexecutecomprocedure;
    MXCallBack*  pexecuteprocedure;


    if ((!pcom) || (!pcom->Client) || (pcom->Client->ClientGone))
        return -1;

    if (!pmessage) return -1;

    pmessage->Com = pcom;

    if (!pmessage->Context)
    {
        OSDateHour (&pmessage->DDate, &pmessage->DHour);
        plist = pmx->ExecuteOutputs;
        while (plist)
        {
            pexecuteprocedure = (MXCallBack *)plist->car;
            plist = plist->cdr;
            pexecuteprocedure->ExecuteProcedure (pmessage, pcom, pexecuteprocedure->ApplicationField);
        }

        plist = pmessage->MessageClass->ExecuteOutputs;
        while (plist)
        {
            pexecuteprocedure = (MXCallBack *)plist->car;
            plist = plist->cdr;
            pexecuteprocedure->ExecuteProcedure (pmessage, pcom, pexecuteprocedure->ApplicationField);
        }
        if (pcom->ComClass)
        {
            plist = pcom->ComClass->ExecuteOutputs;
            while (plist)
            {
                if ((!pcom->Client) || (pcom->Closed)) return 1;
                pexecutecomprocedure = (MXCallBack *)plist->car;
                plist = plist->cdr;
                if (pexecutecomprocedure->MessageClass == pmessage->MessageClass)
                    pexecutecomprocedure->ExecuteProcedure (pmessage, pcom, pexecutecomprocedure->ApplicationField);
            }
        }
        plist = pcom->ExecuteOutputs;
        while (plist)
        {
            if ((!pcom->Client) || (pcom->Closed)) return 1;
            pexecutecomprocedure = (MXCallBack *)plist->car;
            plist = plist->cdr;
            if (pexecutecomprocedure->MessageClass == pmessage->MessageClass)
                pexecutecomprocedure->ExecuteProcedure (pmessage, pcom, pexecutecomprocedure->ApplicationField);
        }
    }

    if (pcom->Closed)
    {
        return -1;
    }
    while (1)
    {
        ret = DBWriteMessage (pmx, pmessage, pcom->Client->TransBuffer, pcom->Client->TransSize, 0);

        if (ret < 0)
        {
            ListRemove(&pcom->OutputMessages, pmessage);
            return -1;
        }
        if (pcom->Closed) return -1;

        if (pcom->TraceProcedure) pcom->TraceProcedure (pmessage, MXONSEND, pcom->Client->TransBuffer, ret);

        ret1 = pcom->Client->WriteFunction (pmx->TP, pcom->Client, pcom->Client->TransBuffer,(int) ret);
        if (ret1 < 0)
        {
            if (!pcom->Client->ClientGone) MXCloseCom (pmx, pcom);
            return -1;
        }
        if (pmessage->Complete)
        {
            ListRemove (&pcom->OutputMessages, pmessage);
            break;
        }
    }

    return ret1;
}


MXMessage* DBRecv (MX* pmx, MXCom* pcom)
{
    LONG ret;
    char* stream = NULL;
    MXMessage* pmessage = NULL;
    List* plist;
    MXCallBack* pexecutecomprocedure;
    MXCallBack* pexecuteprocedure;

    if ((!pcom) || (!pcom->Client) || (pcom->Client->ClientGone))
        return NULL;

    while (1)
    {

        if ((pmessage = MXGetReadyInputMessage (pcom)) == NULL)
        {

            ret  = pcom->Client->ReadFunction (pmx->TP, pcom->Client);
            if (ret < 0)
            {
                if (!pcom->Client->ClientGone) MXCloseCom (pmx, pcom);
                return NULL;
            }
            if (ret == 0) return NULL;
            stream = pcom->Client->RequestBuffer;

            pmessage = DBCheckAndReturnMessage (pmx, pcom, stream);
            if (!pmessage)
            {
                return NULL;
            }
        }
        if (pmessage->InternalField)
        {
            PIPE* pipe = pcom->Client->Pipe;
            OSStatement* pstatement = (OSStatement*)pmessage->InternalField;
            ListRemove ((List**)&pipe->Input, pstatement);
            OSFreeStatement (pstatement);
            pmessage->InternalField = NULL;
        }
        pmessage->Com = pcom;
        if (pcom->TraceProcedure) pcom->TraceProcedure (pmessage, MXONRECV, stream, ret);

        if (pmessage->Complete)
        {
            ListRemove(&pcom->InputMessages, pmessage);

            plist = pmx->ExecuteInputs;
            while (plist)
            {
                pexecuteprocedure = (MXCallBack *)plist->car;
                plist = plist->cdr;
                pexecuteprocedure->ExecuteProcedure (pmessage, pcom, pexecuteprocedure->ApplicationField);
            }
            plist = pmessage->MessageClass->ExecuteInputs;
            while (plist)
            {
                pexecuteprocedure = (MXCallBack *)plist->car;
                plist = plist->cdr;
                pexecuteprocedure->ExecuteProcedure (pmessage, pcom, pexecuteprocedure->ApplicationField);
            }
            if (pcom->ComClass)
            {
                plist = pcom->ComClass->ExecuteInputs;
                while (plist)
                {
                    if ((!pcom->Client) || (pcom->Closed)) return NULL;
                    pexecutecomprocedure = (MXCallBack *)plist->car;
                    plist = plist->cdr;
                    if (pexecutecomprocedure->MessageClass == pmessage->MessageClass)
                        pexecutecomprocedure->ExecuteProcedure (pmessage, pcom, pexecutecomprocedure->ApplicationField);
                }
            }
            plist = pcom->ExecuteInputs;
            while (plist)
            {
                if ((!pcom->Client) || (pcom->Closed)) return NULL;
                pexecutecomprocedure = (MXCallBack *)plist->car;
                plist = plist->cdr;
                if (pexecutecomprocedure->MessageClass == pmessage->MessageClass)
                    pexecutecomprocedure->ExecuteProcedure (pmessage, pcom, pexecutecomprocedure->ApplicationField);
            }

            break;
        }
    }
    return pmessage;
}



