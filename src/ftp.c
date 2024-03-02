/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "ftp.h"


List* FTPActiveConnections = NULL;
int FTPCurrentPortNumber   = 4360;



void FTPEndOtherMessages (MX* pmx, MXCom* pcom, MXMessage* pmessage)
{
    MXCallBack*  pexecuteprocedure;
    MXCallBack*  pexecutecomprocedure;
    List* plist;

    OSDateHour (&pmessage->ADate, &pmessage->AHour);
    pmessage->Complete = 1;  
    MXInitMessage (pmessage);
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
            if ((!pcom->Client) || (pcom->Closed)) return ;
            pexecutecomprocedure = (MXCallBack *)plist->car;
            plist = plist->cdr;
            if (pexecutecomprocedure->MessageClass == pmessage->MessageClass)
                pexecutecomprocedure->ExecuteProcedure (pmessage, pcom, pexecutecomprocedure->ApplicationField);
        }
    }
    plist = pcom->ExecuteInputs;
    while (plist)
    {
        if ((!pcom->Client) || (pcom->Closed)) return ;
        pexecutecomprocedure = (MXCallBack *)plist->car;
        plist = plist->cdr;
        if (pexecutecomprocedure->MessageClass == pmessage->MessageClass)
            pexecutecomprocedure->ExecuteProcedure (pmessage, pcom, pexecutecomprocedure->ApplicationField);
    }
    MXFreeMessage (pcom->MX, pmessage);
}


void FTPTreatOtherMessages (MX* pmx, MXCom* pcom, MXMessage* pmessage)
{
    int i = 0;
    char Command[250];
    MXMessage* pmessage1;
    STRING VNetId;
    LONG Port;

    VNetId = (STRING)MXGetValue (pmessage, "NetId", 1);
    Port   = *(LONG *)MXGetValue (pmessage, "Port", 1);
    while (VNetId[i] != 0)
    {
        if (VNetId[i] == '.') VNetId[i] = ',';  i++;
    }
    sprintf (Command, "%s,%d,%d", VNetId, Port / 256, Port % 256);

    pmessage1 = MXPutMessage (pcom, FTP_SYS, "Command");
    MXSetValue (pmessage1, "Value", 1, "PORT");
    MXSetValue (pmessage1, "Par", 1, Command);

    if (strcmp (pmessage->MessageClass->Name, "get") == 0)
    {
        STRING FileName;
        CHAR FileType;
        LONG Identity;

        FileName = (STRING)MXGetValue (pmessage, "FileName", 1);
        FileType = *(CHAR*)MXGetValue (pmessage, "FileType", 1);
        Identity = *(LONG*)MXGetValue (pmessage, "Identity", 1);

        pmessage1 = MXPutMessage (pcom, FTP_SYS, "Command");
        MXSetValue (pmessage1, "Value", 1, "TYPE");
        if ((FileType == 'T') || (FileType == 't'))
            MXSetValue (pmessage1, "Par", 1, "A");
        else
            MXSetValue (pmessage1, "Par", 1, "I");
        pmessage1 = MXPutMessage (pcom, FTP_SYS, "Command");
        MXSetValue (pmessage1, "Value",      1, "RETR");
        MXSetValue (pmessage1, "Par",        1, FileName);
        MXSetValue (pmessage1, "Identity",   1, &Identity);
    }
    else
        if (strcmp (pmessage->MessageClass->Name, "put") == 0)
        {
            FILEPARM* File;
            LONG Identity;

            File     = (FILEPARM*)MXGetValue (pmessage, "File", 1);
            Identity = *(LONG*)MXGetValue (pmessage, "Identity", 1);

            pmessage1 = MXPutMessage (pcom, FTP_SYS, "Command");
            MXSetValue (pmessage1, "Value", 1, "TYPE");
            if ((File->FileType == 'T') || (File->FileType == 't'))
                MXSetValue (pmessage1, "Par", 1, "A");
            else
                MXSetValue (pmessage1, "Par", 1, "I");
            pmessage1 = MXPutMessage (pcom, FTP_SYS, "Command");
            MXSetValue (pmessage1, "Value",    1, "STOR");
            MXSetValue (pmessage1, "Par",      1, File->FileDest);
            MXSetValue (pmessage1, "Identity", 1, &Identity);
            free (File);
        }
        else
            if (strcmp (pmessage->MessageClass->Name, "dir") == 0)
            {
                STRING Par;
                LONG Identity;

                Par = (STRING)MXGetValue (pmessage, "Par", 1);
                Identity = *(LONG*)MXGetValue (pmessage, "Identity", 1);

                pmessage1 = MXPutMessage (pcom, FTP_SYS, "Command");
                MXSetValue (pmessage1, "Value",      1, "LIST");
                MXSetValue (pmessage1, "Par",        1, Par);
                MXSetValue (pmessage1, "Identity",   1, &Identity);
            }
}



MXMessage* FTPGetReadyActiveMessage (MXCom* pcom)
{
    MXMessage* pmessage = NULL;
    MXCom* ftpcom = (MXCom*)pcom->InternalField;

    if (pcom->OutputMessages)
        pmessage = MXFindIncompleteMessage (pcom->OutputMessages);
    else
    {
        if (ftpcom->ActiveOutputMessages)
        {
            pmessage = (MXMessage*)ftpcom->ActiveOutputMessages->car;
            ListRemove (&ftpcom->ActiveOutputMessages, pmessage);
            ListNewr (&pcom->OutputMessages, pmessage);
        }
    }
    return pmessage;
}



MXMessage* FTPCheckAndReturnActiveMessage (MX* pmx, MXCom* pcom, char* stream)
{
    MXMessage* pmessage = NULL;
    MXCom* ftpcom = (MXCom*)pcom->InternalField;

    if (pcom->InputMessages)
        pmessage = (MXMessage*)pcom->InputMessages->car;
    else
    {
        if (ftpcom->ActiveInputMessages)
        {
            pmessage = (MXMessage*)ftpcom->ActiveInputMessages->car;
            ListRemove (&ftpcom->ActiveInputMessages, pmessage);
            ListNewr (&pcom->InputMessages, pmessage);
        }
    }
    return pmessage;
}



LONG FTPWriteFileContent (MX* pmx, MXMessageContext* pcontext,
                          char* stream, char* attributes, WORD maxcount)
{
    char* fromfilename;
    char* tofilename;
    char* filebuf;
    WORD size;
    DWORD filesize;
    CHAR filetype = 'b';
    CHAR filelocal;
    int isread = 0;
    LONG from = 0;
    int error = 0;
    LONG readfile = 0;
    BOOL transcod = FALSE;
    fromfilename = attributes;

    size = FILENAMESIZE;
    tofilename = fromfilename + size;

    OSCopy4 (&filesize, tofilename + size + (3 * sizeof (CHAR)));
    /*  filetype = *(fromfilename + (2 * FILENAMESIZE) + (2 * sizeof (CHAR)));*/
    filelocal = *(fromfilename + (2 * FILENAMESIZE));


    if (!pcontext->File)      /* file is not opened  so open it*/
    {
        pcontext->File = OSOpenFile (fromfilename, "r", filetype, filelocal, &error);

        if (!pcontext->File)
        {
            OSError ("TEDAG030", " (%s, %c, %c)", fromfilename, filetype, filelocal);
            return -1;
        }
    }
    /* see if buffer is allocated */
    if (!pcontext->Stream)
    {

        pcontext->Stream = (char*)malloc (MXBUFFILESIZE);
        pcontext->StreamPos = 0;
        if (!pcontext->Stream)
        {
            OSError ("TEDAG031", NULL);
            OSCloseFile (pcontext->File);
            pcontext->File = NULL;
            return -1;
        }
    }

    if (maxcount == 0)       /* I can't read , read next time */
        return 0;

    /* now read in file */
    filebuf = pcontext->Stream + pcontext->StreamPos;

    if (pcontext->StreamPos != 0)
    {
        isread = pcontext->StreamSize - pcontext->StreamPos;

        if (isread > maxcount)   /* this means no place to put evrything */
        {
            isread  = maxcount;
            pcontext->StreamPos += isread;
        }
        else
            pcontext->StreamPos = 0;

        /*  printf ("read from previous buffer : %d whose size is %u\n", isread, pcontext->StreamSize);*/
        memcpy (stream , filebuf, isread);
    }

    while (isread < maxcount)  /* still place to fill buffer */
    {
        from = 0;
        readfile = OSReadFile (pcontext->File, pcontext->Stream, MXBUFFILESIZE, &error);

        /*     printf ("read from file %d\n", readfile);*/
        if (readfile == 0)
        {
            OSCloseFile (pcontext->File);
            pcontext->File = NULL;
            break;
        }
        if ((DWORD) (isread + readfile) > maxcount)
        {
            memcpy (stream + isread , pcontext->Stream, maxcount - isread);
            pcontext->StreamSize = readfile;
            pcontext->StreamPos = maxcount - isread;
            isread = maxcount;
        }
        else
        {
            memcpy (stream + isread , pcontext->Stream, (WORD)readfile);
            isread += readfile;
        }
    }
    return isread;
}




LONG FTPReadFileContent (MX* pmx, MXMessageContext* pcontext, char* stream,
                         char* attributes, WORD maxcount)
{
    char* fromfilename;
    char* tofilename;
    LONG* toerror;
    WORD size;
    int filesize;
    CHAR filetype = 'b';
    CHAR filelocal;
    WORD towrite = 0;
    LONG written, writefile, StillToWrite;
    LONG from = 0;
    int error = 0;
    LONG fileerror;
    BOOL transcod = FALSE;
    fromfilename = attributes;
    size = FILENAMESIZE;
    tofilename = fromfilename + size;

    OSCopy4 (&filesize, tofilename + size + (3 * sizeof (CHAR)));
    /* filetype = *(fromfilename + (2 * FILENAMESIZE) + (2 * sizeof (CHAR)));*/
    filelocal = *(fromfilename + (2 * FILENAMESIZE) + sizeof (CHAR));
    toerror = (LONG*)(fromfilename + (2 * FILENAMESIZE) + (3 * sizeof (CHAR)) + sizeof (DWORD));
    OSCopy4((void*) toerror, (char*)&error);
    OSCopy4(&fileerror, (char*)toerror);



    if (!pcontext->File)      /* file is not opened  so open it*/
    {

        /**** open it alway as binary  */
        pcontext->Pos = 0;
        pcontext->File = OSOpenFile (tofilename, "w", filetype, filelocal, &error);
        if (!pcontext->File)
        {
            OSError ("TEDAG027", " (%s, %c, %c)", tofilename, filetype, filelocal);
            OSCopy4 ((void*)toerror, (char*)&error);
            return -2;
        }
    }
    /* see if buffer is allocated */
    if (!pcontext->Stream)
    {

        pcontext->Stream = (char*)malloc (MXBUFFILESIZE);
        pcontext->StreamPos = 0;
        if (!pcontext->Stream)
        {
            OSError ("TEDAG029", NULL);
            OSCloseFile (pcontext->File);
            pcontext->File = NULL;
            return -1;
        }
    }
    if (maxcount == 0)        /* I can't write , write next time */
        return 0;


    towrite = maxcount;

    /* we use xf in type */
    if (pcontext->StreamSize + towrite > MAXBUFSIZE)
    {
        OSError ("TEDAG054", NULL);    /* disk is full i can't write */
        error = 10005;
        OSCopy4 ((void*)toerror, (char*)&error);
        OSCloseFile (pcontext->File);
        pcontext->File = NULL;
        return -1;
    }
    if (transcod)
        XFCharRead (pmx->TP->XF, pcontext->Stream + pcontext->StreamSize , &from,
                    stream,  towrite );
    else
        memcpy (pcontext->Stream + pcontext->StreamSize , stream , towrite);


    written = 0;
    StillToWrite  = towrite + pcontext->StreamSize;
    while (written < StillToWrite )
    {
        /* now write in file tream */

        writefile = OSWriteFile (pcontext->File, pcontext->Stream +(WORD) written, StillToWrite - written , 0, &error);

        if ((error != 0) || (writefile == 0))
            break;
        written += writefile;
    }

    if ((error != 0) || (written > StillToWrite))
    {
        OSError ("TEDAG028", " (%s)", tofilename);
        OSCloseFile (pcontext->File);
        pcontext->File = NULL;
        return -1;
    }

    if (written < StillToWrite)
    {
        pcontext->StreamSize = (WORD) (StillToWrite - written);
        memcpy (pcontext->Stream , pcontext->Stream + (WORD)written , (WORD) (StillToWrite - written));
    }
    else pcontext->StreamSize = 0;

    pcontext->Pos += towrite;
    return towrite;
}




LONG FTPReadActiveMessage (MX* pmx, MXMessage* pmessage, char* stream, int streamsize, BYTE WithFileContent)
{
    MXObject* pobject;
    BOOL FirstTime = FALSE;
    LONG from = 0;
    BYTE type;
    LONG isread = 0;          /* what is read in partmessage */
    int FromObjectOffset;
    int FromSizeOffset;
    int NbObjects;
    XF* pxf;
    char* fromstream;
    char* bufferattributes;
    char* fileattributes;

    MXMessageClass* pclassmess = pmessage->MessageClass;
    MXMessageContext* pcontext = pmessage->Context;

    int State = 0;
    int i;


    if ((pmessage->Com) && (pmessage->Com->ComClass) && (pmessage->Com->XF))
        pxf = pmessage->Com->XF;
    else pxf = pmx->TP->XF;


    if (!pcontext)
    {
        FirstTime = TRUE;
        pcontext = MXAllocateContext ();
        if (!pcontext)
        {
            return -1;
        }
        pmessage->Context = pcontext;
    }

    fromstream = stream;


    from = 0;
    FromObjectOffset = pcontext->ObjectOffset;
    FromSizeOffset = pcontext->SizeOffset;
    NbObjects = ListNbElt (pclassmess->Objects);

    for (i = FromObjectOffset; i < NbObjects; i++)
    {
        pobject = (MXObject *)ListGetElt (pclassmess->Objects, i);
        type = pobject->Type;
        switch (type)
        {
        case MXSHORT :
            break;
        case MXWORD :
            break;
        case MXLONG :
            break;
        case MXDWORD :
            break;
        case MXBYTE :
            break;
        case MXCHAR :
            break;
        case MXBUFFER :
            {
                DWORD BufferSize;
                if (pmessage->StreamSize < pmessage->Offset + streamsize)
                    if (MXExtendValueBuffer (pmessage, streamsize) == 0) return -1;

                bufferattributes = (char*) (pmessage->Stream + pmessage->Values[(WORD)pobject->Offset]);
                OSCopy4(&BufferSize, bufferattributes);
                BufferSize += streamsize;
                memcpy (bufferattributes , (CHAR *)(&BufferSize) , sizeof (DWORD));

                pcontext->Buffer = bufferattributes + sizeof (DWORD) + sizeof (CHAR);
                memcpy ((char *)(pcontext->Buffer + pcontext->Pos), fromstream , streamsize);
                pcontext->Pos += streamsize;
                pmessage->Offset += streamsize;
            }
            break;
        case MXSTRING :
            break;
        case MXFILE :
            {
                DWORD FileSize;
                fileattributes = (char*) (pmessage->Stream + pmessage->Values[(WORD)pobject->Offset]);

                OSCopy4(&FileSize, fileattributes + (2 * FILENAMESIZE) + (3 * sizeof (CHAR)));
                FileSize += streamsize;
                memcpy (fileattributes + (2 * FILENAMESIZE) + (3 * sizeof (CHAR)), (CHAR *)(&FileSize) , sizeof (DWORD));
                FTPReadFileContent (pmx, pcontext, fromstream,  fileattributes, (WORD)streamsize);
            }
            break;
        default:
            OSError ("TEDAG032",  " (%s)", pmessage->MessageClass->Name);
            return -1;
        }
    }
    return (LONG)(streamsize);
}


LONG FTPReceiveActiveMessage (TP* ptp, TPClient* pclient, char* buf, int count)
{
    int i = 0;
    char* stream;
    MX* pmx;
    MXMessage* pmessage;
    LONG ret;
    LONG readcount = 0;

    MXCom* pcom = (MXCom*)pclient->ApplicationField;
    MXCom* ftpcom = (MXCom*)pcom->InternalField;


    pmx = pcom->MX;

    if ((!pcom) || (!pcom->Client) || (!pclient->RequestBuffer))
        return -1;

    stream = buf;

    pmessage = FTPCheckAndReturnActiveMessage(pmx, pcom, stream);

    if (!pmessage)
        return -1;

    pmessage->Com = pcom;

    if (pcom->TraceProcedure) pcom->TraceProcedure (pmessage, MXONRECV, stream , count);

    ret = FTPReadActiveMessage (pmx, pmessage, stream, count, 1);
    if (ret < 0)
    {
        pmessage->Complete = 1;
    }

    return ret;
}

LONG FTPWriteActiveMessage (MX* pmx, MXMessage* pmessage, char* stream, int maxcount, BYTE WithFileContent)
{
    MXObject* pobject;
    BOOL FirstTime = FALSE;
    LONG from = 0;
    BYTE type;
    BYTE  NoContent = 0;
    int size;
    int written = 0;
    int FromObjectOffset;
    int FromSizeOffset;
    int NbObjects;
    XF* pxf;
    char* tostream;
    char* fileattributes;


    MXMessageClass* pclassmess = pmessage->MessageClass;
    MXMessageContext* pcontext = pmessage->Context;
    int State = 0;
    int j, i;
    MXCom* pcom = pmessage->Com;

    if ((pcom) && (pcom->ComClass) && (pcom->XF))
        pxf = pcom->XF;
    else pxf = pmx->TP->XF;

    /* set part message value */

    pmessage->Part++;

    /* if no context related to this message this means the first time */

    if (!pcontext)
    {
        FirstTime = TRUE;
        pmessage->Size = MXGetMessageSize (pmessage, WithFileContent);
        OSDateHour (&pmessage->DDate, &pmessage->DHour);
        pcontext = MXAllocateContext ();
        if (!pcontext)
        {
            return -1;
        }
        pmessage->Context = pcontext;
        pcontext->StillToRead = pmessage->Size;
    }

    tostream = stream;


    FromObjectOffset = pcontext->ObjectOffset;
    FromSizeOffset = pcontext->SizeOffset;
    NbObjects = ListNbElt (pclassmess->Objects);


    for (i = FromObjectOffset; i < NbObjects; i++)
    {
        pobject = (MXObject *)ListGetElt (pclassmess->Objects, i);
        from = 0;
        type = pobject->Type;
        switch (type) {
        case MXSHORT :
            for (j = FromSizeOffset; j < pobject->Size; j++, from = 0)
            {
                from += sizeof (SHORT);
                pcontext->StillToRead -= sizeof (SHORT);
            }
            break;
        case MXWORD :
            for (j = FromSizeOffset; j < pobject->Size; j++, from = 0)
            {
                from += sizeof (WORD);
                pcontext->StillToRead -= sizeof (WORD);
            }
            break;
        case MXLONG :
            for (j = FromSizeOffset; j < pobject->Size; j++, from = 0)
            {
                from += sizeof (LONG);
                pcontext->StillToRead -= sizeof (LONG);
            }
            break;
        case MXDWORD :
            for (j = FromSizeOffset; j < pobject->Size; j++, from = 0)
            {
                from += sizeof (DWORD);
                pcontext->StillToRead -= sizeof (DWORD);
            }
            break;
        case MXBYTE :
            break;
        case MXCHAR :
            break;
        case MXBUFFER :
            break;
        case MXFILE :
            for (j = FromSizeOffset; j < pobject->Size; j++, from = 0)
            {
                /* skip file attributes */
                size =  (2 * FILENAMESIZE) + (3 * sizeof (CHAR)) + sizeof (DWORD) + sizeof (LONG);
                pcontext->StillToRead -= size;
                fileattributes = (char*) (pmessage->Stream + pmessage->Values[(WORD)pobject->Offset + j]);
                size = FTPWriteFileContent (pmx, pcontext, tostream,  fileattributes, (WORD)(maxcount - written));
                if (size < 0) /* something strange */
                {
                    return -1;
                }
                tostream += size;
                written += size;
                if (MXFileProcessing(pmessage))    /* still to read in file */
                {
                    State = 1;
                    break;
                }
            }
            break;
        case MXSTRING :
            for (j = FromSizeOffset; j < pobject->Size; j++, from = 0)
            {
                size = (int)strlen ((char*) (pmessage->Stream + pmessage->Values[(WORD)pobject->Offset + j])) + 1;
                from += size;
                pcontext->StillToRead -= size;
            }
            break;
        default:
            OSError ("TEDAG035",  " (%s)", pmessage->MessageClass->Name);
            return -1;
        }
        pcontext->SizeOffset = j;
        pcontext->ObjectOffset = i;
        if (State == 1)    /* here we got a break can't write anymore  */
        {
            if (pcontext->StillToRead == 0)            /* something is strange but send it anyway*/
            {

                OSError ("TEDAG036",  " (%s)", pmessage->MessageClass->Name);
                pmessage->Complete = 1;
                pmessage->Part++;
            }
            break;   /* we should return now */
        }
    }

    if (State != 1)              /* evrything is written */
    {
        pmessage->Complete = 1;
    }
    return written ;
}







LONG FTPSendActiveMessage (TP* ptp, TPClient* pclient, char* buf, int size)
{
    LONG ret;
    MX* pmx;

    MXMessage* pmessage;
    MXCallBack* pexecutecomprocedure;
    MXCallBack* pexecuteprocedure;
    List* plist;
    /* now hoping that values are set */
    MXCom* pcom = (MXCom*)pclient->ApplicationField;
    pmx = pcom->MX;


    pmessage = FTPGetReadyActiveMessage (pcom);
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

    ret = FTPWriteActiveMessage (pmx, pmessage, buf, size, 0);


    if (ret < 0)
    {
        ListRemove(&pcom->OutputMessages, pmessage);
        MXFreeMessage (pcom->MX, pmessage);
        return -1;
    }
    if (pcom->TraceProcedure)   pcom->TraceProcedure (pmessage, MXONSEND, buf, ret);
    if (pmessage->Complete)
    {
        pmessage->Complete = 0;  /* let the message goes once before closing */
        if ((ret == 0) && (!MXComIsBlocked (pcom)))
        {
            TPSetBlockingMode (pcom->Client, MXNoBlock);
            ListRemove (&pcom->OutputMessages, pmessage);
            MXFreeMessage (pcom->MX, pmessage);
            MXCloseCom (pcom->MX, pcom);
        }
    }

    return ret;
}



int FTPCloseActiveConnection (MXCom* pcom, void* app)
{
    MXCom* ftpcom = (MXCom*)app;
    MXMessage* pmessage;
    STRING Value;
    int error = 0;
    FTPActiveConnection* ActiveConnection;
    int Port;

    Port = pcom->Client->OnPort->Port;

    if (ActiveConnection = FTPFindActiveConnection (pcom->MX, ftpcom, Port))
        ListRemove (&FTPActiveConnections, ActiveConnection);

    pmessage = FTPCheckAndReturnActiveMessage(pcom->MX, pcom, NULL);

    if (!pmessage)
        return -1;

    /* Treat End of file */
    Value  =   (STRING)MXGetValue (pmessage, "Value", 1);
    if (strcmp (Value, "RETR") == 0)
    {
        MXMessageContext* pcontext = pmessage->Context;
        if (pcontext)
        {
            if (pcontext->StreamSize != 0)
            {
                OSWriteFile (pcontext->File, pcontext->Stream , pcontext->StreamSize , 0, &error);
            }
            OSCloseFile (pcontext->File);
            pcontext->File = NULL;
        }
        else
        {
            //   OSOpenFile (tofilename, "w", filetype, 'D', &error);
            //    OSCloseFile (pcontext->File);
        }
    }

    ListRemove(&pcom->InputMessages, pmessage);
    FTPEndOtherMessages (pcom->MX, ftpcom, pmessage);
    ftpcom->InternalField = NULL;

    return 1;
}



int FTPOpenActiveConnection (MXCom* pcom, void* app)
{
    FTPActiveConnection* ActiveConnection;
    MXCom* ftpcom = (MXCom*)app;
    int Port;

    if (!pcom->Client->OnPort) return 1;
    Port = pcom->Client->OnPort->Port;

    if (ActiveConnection = FTPFindActiveConnection (pcom->MX, ftpcom, Port))
    {
        ActiveConnection->Done = 1;
        MXAddEndingProcedure (pcom, FTPCloseActiveConnection, app);
        pcom->InternalField = ftpcom;
        ftpcom->InternalField = pcom;
        MXSetComApplicationField (pcom, ftpcom->ApplicationField);
        MXSetTraceProcedure (pcom, ftpcom->TraceProcedure);
        pcom->Client->ReceiveFunction = FTPReceiveActiveMessage;
        pcom->Client->SendFunction    = FTPSendActiveMessage;
    }
    return 1;
}


MXMessage* FTPCheckAndReturnMessage (MX* pmx, MXCom* pcom, char* stream)
{
    MXMessage* pmessage = NULL;
    LONG error;
    char serror[10];
    int i = 0;

    while (i < 3)
        serror[i] = stream[i++];
    serror[i] = 0;
    error = atol (serror);

    if ((error == 220) || (error == 150) || (error == 125))
    {
        pmessage = MXCreateMessage (pmx, FTP_SYS, "Command_Reply");
        MXSetValue (pmessage, "Value", 1, "OPEN");
        MXSetValue (pmessage, "Code",  1, &error);
        MXSetValue (pmessage, "Par",   1, "");
        pmessage->Com = pcom;

    }
    else
        if (pcom->InputMessages)
            pmessage = (MXMessage*)pcom->InputMessages->car;
        else
        {
            pmessage = MXCreateMessage (pmx, FTP_SYS, "Command_Reply");
            MXSetValue (pmessage, "Value", 1, "FTP");
            MXSetValue (pmessage, "Par",   1, "");
            pmessage->Com = pcom;

        }
    return pmessage;
}


LONG FTPReadMessage (MX* pmx, MXMessage* pmessage, char* stream, int RequestSize, BYTE WithFileContent)
{
    LONG i = 0;
    int Code = 0;
    char SCode[4];


    while ((i <= RequestSize) && (stream[i] != '\n'))  i++;

    if (stream[i] == '\n')
    {
        stream[i-1] = 0;
        stream[i] = 0;
        i++;
        sprintf (SCode, "%.*s", ((3 > i) ? i : 3), stream);

        Code = atol (SCode);
        MXSetValue (pmessage, "Reply", 1, stream);
        MXSetValue (pmessage, "Code",  1, &Code);

        pmessage->Complete = 1;
    }
    /* A FAIRE si le message est recu partitionne */
    /*============================================*/
    MXInitMessage (pmessage);
    return i;
}




LONG FTPWriteMessage (MX* pmx, MXMessage* pmessage, char* stream, int maxcount, BYTE WithFileContent)
{
    MXObject* pobject;
    BOOL FirstTime = FALSE;
    LONG from = 0;
    BYTE type;
    BYTE  NoContent = 0;
    int size;
    int written = 0;
    int FromObjectOffset;
    int FromSizeOffset;
    int NbObjects;
    XF* pxf;
    char* tostream;


    MXMessageClass* pclassmess = pmessage->MessageClass;
    MXMessageContext* pcontext = pmessage->Context;
    int State = 0;
    int j, i;

    if ((pmessage->Com) && (pmessage->Com->ComClass) && (pmessage->Com->XF))
        pxf = pmessage->Com->XF;
    else pxf = pmx->TP->XF;

    /* set part message value */

    pmessage->Part++;

    /* if no context related to this message this means the first time */
    if (!pcontext)
    {
        FirstTime = TRUE;
        pmessage->Size = MXGetMessageSize (pmessage, WithFileContent);
        OSDateHour (&pmessage->DDate, &pmessage->DHour);
        pcontext = MXAllocateContext ();
        if (!pcontext)
        {
            return -1;
        }
        pmessage->Context = pcontext;
        pcontext->StillToRead = pmessage->Size;  /* Don't send Identity */
    }
    tostream = stream;

    FromObjectOffset = pcontext->ObjectOffset;
    FromSizeOffset = pcontext->SizeOffset;
    NbObjects = ListNbElt (pclassmess->Objects);

    for (i = FromObjectOffset; i < NbObjects; i++)
    {
        pobject = (MXObject *)ListGetElt (pclassmess->Objects, i);
        from = 0;
        type = pobject->Type;
        switch (type)
        {
        case MXSTRING :
            for (j = FromSizeOffset; j < pobject->Size; j++, from = 0)
            {
                char* ftpstream = (char*) (pmessage->Stream + pmessage->Values[(WORD)pobject->Offset + j]);
                if ((written + (int)strlen (ftpstream) + 1) <= maxcount)
                {
                    size = XFStringWrite (pxf, ftpstream, &from, tostream, 1);
                    pcontext->StillToRead -= size;
                    if ((i == 1) || (pmessage->Values[(WORD)pobject->Offset + j] != 0)) /* ignore first string value */
                    {
                        if (strlen (tostream))
                        {
                            written += size;
                            tostream[size - 1] = ' ';
                            tostream += (WORD)size;
                        }
                    }
                }
                else
                {
                    State = 1;
                    break;
                }
            }
            break;
        case MXLONG :
            for (j = FromSizeOffset; j < pobject->Size; j++, from = 0)
            {
                from += sizeof (LONG);
                pcontext->StillToRead -= sizeof (LONG);
            }
            break;
        default:
            OSError ("TEDAG035",  " (%s)", pmessage->MessageClass->Name);
            return -1;
        }
        if (State == 1)    /* here we got a break can't write anymore  */
        {
            if (pcontext->StillToRead == 0)            /* something is strange but send it anyway*/
            {
                OSError ("TEDAG036",  " (%s)", pmessage->MessageClass->Name);
                pmessage->Complete = 1;
                pmessage->Part++;
            }
            pcontext->SizeOffset = j;
            pcontext->ObjectOffset = i;
            break;   /* we should return now */
        }
    }
    if (State != 1)              /* evrything is written */
    {
        pmessage->Complete = 1;  /* add \r\n */

        written += 1;
        tostream[-1] = '\r';
        tostream[0] = '\n';
    }
    MXInitMessage (pmessage);
    return written ;
}



LONG FTPSendMessage (TP* ptp, TPClient* pclient, char* buf, int size)
{
    LONG ret;
    MX* pmx;

    MXMessage* pmessage;
    MXCallBack* pexecutecomprocedure;
    MXCallBack* pexecuteprocedure;
    List* plist;
    /* now hoping that values are set */
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
    if (FTPNotCommandMessage (pmessage))
    {
        FTPTreatOtherMessages (pmx, pcom, pmessage);
        ListRemove(&pcom->OutputMessages, pmessage);
        if (strcmp (pmessage->MessageClass->Name, "put") == 0)
            ListNewr (&pcom->ActiveOutputMessages, pmessage);  /* we are in a sending mode */
        else
            MXFreeMessage (pcom->MX, pmessage);
        /* pick next message */
        return FTPSendMessage (ptp, pclient, buf, size);
    }

    TPSetBlockingMode (pcom->Client, pmessage->MessageClass->BlockedOnOutput);

    ret = FTPWriteMessage (pmx, pmessage, buf, size, 1);

    /* ftp */

    if (ret < 0)
    {
        ListRemove(&pcom->OutputMessages, pmessage);
        MXFreeMessage (pcom->MX, pmessage);
        return -1;
    }
    if (pcom->TraceProcedure)   pcom->TraceProcedure (pmessage, MXONSEND, buf, ret);
    if (pmessage->Complete)
    {
        /* ftp */
        MXMessage* pmessage1;
        LONG Identity;
        Identity = *(LONG*)MXGetValue (pmessage, "Identity", 1);

        pmessage1 = MXCreateMessage (pmx, FTP_SYS , "Command_Reply");
        MXSetValue (pmessage1, "Value", 1, MXGetValue (pmessage, "Value", 1));
        MXSetValue (pmessage1, "Identity", 1, &Identity);
        MXSetValue (pmessage1, "Par",   1, MXGetValue (pmessage, "Par", 1));
        pmessage1->Com = pcom;
        ListNewr (&pcom->InputMessages, pmessage1);
        /* ftp */
        TPSetBlockingMode (pcom->Client, MXNoBlock);
        ListRemove (&pcom->OutputMessages, pmessage);
        if (!(pmessage->Type & MXACK))
            MXFreeMessage (pcom->MX, pmessage);
        else
            ListNewr (&pcom->AckMessages, pmessage);
    }

    return ret;
}

LONG FTPSend (MX* pmx, MXCom* pcom, MXMessage* pmessage)
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


    TPSetBlockingMode (pcom->Client, TRUE);

    if (FTPNotCommandMessage (pmessage))
    {
        int ret = 0;
        LONG Error;
        FTPTreatOtherMessages (pmx, pcom, pmessage);
        ListRemove(&pcom->OutputMessages, pmessage);
        if (strcmp (pmessage->MessageClass->Name, "put") == 0)
            ListNewr (&pcom->ActiveOutputMessages, pmessage);  /* we are in a sending mode */
        /* pick next message */
        while (pcom->OutputMessages)
        {
            MXMessage* message = (MXMessage*)pcom->OutputMessages->car;
            ret += FTPSend (pmx, pcom, message);
            MXFreeMessage (pmx, message);
            if (pcom->OutputMessages)
            {
                message = FTPRecv (pmx, pcom);
                Error = *(LONG*)MXGetValue (message, "Code", 1);
                MXFreeMessage (pmx, message);
            }
        }
        return ret;
    }

    while (1)
    {
        ret = FTPWriteMessage (pmx, pmessage, pcom->Client->TransBuffer, pcom->Client->TransSize - 2, 1);

        if (ret < 0)
        {
            TPSetBlockingMode (pcom->Client, FALSE);
            ListRemove(&pcom->OutputMessages, pmessage);
            return -1;
        }
        if (pcom->Closed) return -1;

        if (pcom->TraceProcedure)  pcom->TraceProcedure (pmessage, MXONSEND,  pcom->Client->TransBuffer, ret);

        ret1 = pcom->Client->WriteFunction (pmx->TP, pcom->Client, pcom->Client->TransBuffer,(int) ret);
        if (ret1 < 0)
        {
            if (!pcom->Client->ClientGone) MXCloseCom (pmx, pcom);
            return -1;
        }
        if (pmessage->Complete)
        {
            MXMessage* pmessage1;
            pmessage1 = MXCreateMessage (pmx, FTP_SYS , "Command_Reply");
            MXSetValue (pmessage1, "Value", 1, MXGetValue (pmessage, "Value", 1));
            MXSetValue (pmessage1, "Par",   1, MXGetValue (pmessage, "Par", 1));
            MXSetValue (pmessage1, "Reply", 1, "");
            pmessage1->Com = pcom;
            ListNewr (&pcom->InputMessages, pmessage1);
            ListRemove (&pcom->OutputMessages, pmessage);
            if (pmessage->Type & MXACK)
                ListNewr (&pcom->AckMessages, pmessage);
            break;
        }
    }
    TPSetBlockingMode (pcom->Client, FALSE);
    return ret1;
}




LONG FTPReceiveMessage (TP* ptp, TPClient* pclient, char* buf, int count)
{
    char* stream;
    MX* pmx;
    MXMessage* pmessage;
    LONG ret = 0;
    LONG readcount = 0;
    List* plist;
    MXCallBack* pexecutecomprocedure;
    MXCallBack* pexecuteprocedure;


    MXCom* pcom = (MXCom*)pclient->ApplicationField;
    pmx = pcom->MX;

    if ((!pcom) || (!pcom->Client) || (!pclient->RequestBuffer))
        return -1;
    stream = buf;


    while (readcount < count)
    {
        pmessage = FTPCheckAndReturnMessage (pmx, pcom, stream);

        if (!pmessage)
            return -1;

        pmessage->Com = pcom;


        ret = FTPReadMessage (pmx, pmessage, stream, count - readcount, 1);
        readcount += ret;
        if (ret < 0)
        {
            pmessage->Complete = 1;     /*finish with this message */
        }

        if (pcom->TraceProcedure)  pcom->TraceProcedure (pmessage, MXONRECV, stream, count);

        TPSetBlockingMode (pcom->Client, pmessage->MessageClass->BlockedOnInput);
        if (pmessage->Complete)
        {

            TPSetBlockingMode (pcom->Client, MXNoBlock);
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
        stream += ret;
    }

    return ret;
}






MXMessage* FTPRecvActive (MX* pmx, MXCom* pcom)
{
    int i = 0;
    char* stream;
    MXMessage* pmessage;
    LONG ret;


    TPSetBlockingMode (pcom->Client, TRUE);
    while (1)
    {
        while ((ret  = pcom->Client->ReadFunction (pmx->TP, pcom->Client)) == 0);
        if (ret < 0)
        {
            if (!pcom->Client->ClientGone) MXCloseCom (pmx, pcom); /* message is freed in FTPTreat..*/
            return NULL;
        }
        

		stream = pcom->Client->RequestBuffer;
        pmessage = FTPCheckAndReturnActiveMessage (pmx, pcom, stream);
        if (!pmessage)
        {
            TPSetBlockingMode (pcom->Client, FALSE);
            return NULL;
        }

		if (ret == 0)
			pmessage->Complete = 1;

		pmessage->Com = pcom;
        if (pcom->TraceProcedure) pcom->TraceProcedure (pmessage, MXONRECV, stream, ret);

        ret = FTPReadActiveMessage (pmx, pmessage, stream, ret, 1);
        if (ret < 0)
        {
            TPSetBlockingMode (pcom->Client, FALSE);
            ListRemove(&pcom->InputMessages, pmessage);
            MXFreeMessage(pcom->MX, pmessage);
            return NULL;
        }
        stream += ret;
        if (pmessage->Complete) break;
    }
    if (!pcom->Closed) TPSetBlockingMode (pcom->Client, FALSE);
    return pmessage;
}


MXMessage* FTPRecv (MX* pmx, MXCom* pcom)
{
    LONG ret = 0;
    LONG readcount = 0;
    char* stream;
    MXMessage* pmessage = NULL;
    List* plist;
    MXCallBack* pexecutecomprocedure;
    MXCallBack* pexecuteprocedure;

    if ((!pcom) || (!pcom->Client) || (pcom->Client->ClientGone))
        return NULL;

    TPSetBlockingMode (pcom->Client, TRUE);
    while (1)
    {
        while ((ret  = pcom->Client->ReadFunction (pmx->TP, pcom->Client)) == 0);
        if (ret < 0)
        {
            if (!pcom->Client->ClientGone) MXCloseCom (pmx, pcom);
            if (pmessage) MXFreeMessage (pcom->MX, pmessage);
            return NULL;
        }
        stream = pcom->Client->RequestBuffer;
        while (readcount < ret)
        {
            pmessage = FTPCheckAndReturnMessage (pmx, pcom, stream);
            if (!pmessage)
            {
                TPSetBlockingMode (pcom->Client, FALSE);
                return NULL;
            }
            pmessage->Com = pcom;
            if (pcom->TraceProcedure) pcom->TraceProcedure (pmessage, MXONRECV, stream, ret);

            ret = FTPReadMessage (pmx, pmessage, stream, ret, 1);
            if (ret < 0)
            {
                TPSetBlockingMode (pcom->Client, FALSE);
                ListRemove(&pcom->InputMessages, pmessage);
                MXFreeMessage(pcom->MX, pmessage);
                return NULL;
            }
            readcount += ret;
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
            }
            stream += ret;
        }
        if (pmessage->Complete) break;
    }
    /*
      {
         LONG   Code   =   (LONG)MXGetValue (pmessage, "Code", 1);
         STRING Reply =   (STRING)MXGetValue (pmessage, "Reply", 1);
         LONG   Identity = (LONG)MXGetValue (pmessage, "Identity", 1);
         STRING Value  =   (STRING)MXGetValue (pmessage, "Value", 1);  
         STRING Par    =   (STRING)MXGetValue (pmessage, "Par", 1);
        
         if ((strcmp (Value, "OPEN") == 0) && (Code == 150))                 
          {                                                                 
             int ret = TPWaitForConnections (pmx->TP);                       
             MXCom* pcom_active = pcom->InternalField;
             MXMessage* pmessage_active =  FTPRecvActive (pmx, pcom_active); 
             MXFreeMessage (pmx, pmessage);                                  
             if (pmessage_active) MXFreeMessage (pmx, pmessage_active);      
             return FTPRecv (pmx, pcom);
          }
       
      }
      
     */
    if (!pcom->Closed) TPSetBlockingMode (pcom->Client, FALSE);
    return pmessage;
}


FTPActiveConnection* FTPFindActiveConnection (MX* pmx, MXCom* pcom, int Port)
{
    List* ActiveConnections = FTPActiveConnections;
    while (ActiveConnections)
    {
        FTPActiveConnection* ActiveConnection = (FTPActiveConnection *)ActiveConnections->car;
        if ((ActiveConnection->Com == pcom) && (ActiveConnection->Port == Port))
            return ActiveConnection;
        ActiveConnections = ActiveConnections->cdr;
    }
    return NULL;
}

FTPActiveConnection* FTPCreateActiveConnection (MX* pmx, MXCom* pcom, int Port, char* NetId, BYTE Type)
{
    FTPActiveConnection* ActiveConnection;
    ActiveConnection = (FTPActiveConnection*)malloc (sizeof (FTPActiveConnection));
    ActiveConnection->Port = Port;
    ActiveConnection->Done = 0;
    ActiveConnection->Com = pcom;
    strcpy (ActiveConnection->NetId, NetId);
    ListNewr (&FTPActiveConnections, ActiveConnection);
    return ActiveConnection;
}


int FTPClose (MXCom* pcom, void* app)
{
    List* ActiveConnections = FTPActiveConnections;
    MXRemoveGeneralConnectCallBack (pcom->MX, MXONCONNECT,  FTPOpenActiveConnection, pcom);

    while (ActiveConnections)
    {
        FTPActiveConnection* ActiveConnection = (FTPActiveConnection *)ActiveConnections->car;
        ActiveConnections = ActiveConnections->cdr;
        if (ActiveConnection->Com == pcom)
        {
            ListRemove (&FTPActiveConnections, ActiveConnection);
            free (ActiveConnection);
        }
    }
    return 1;
}

MXMessage* FTPSendAndRecvCommand (MX* pmx, MXCom* pcom, char* Value, char* Par)
{
    MXMessage* pmessage = MXPutMessage (pcom, FTP_SYS, "Command");
    MXSetValue (pmessage, "Value", 1, Value);
    if (Par)
        MXSetValue (pmessage, "Par",   1, Par);
    else
        MXSetValue (pmessage, "Par",   1, "");


    if(FTPSend(pmx, pcom, pmessage) < 0)
        return NULL;
    if (!(pmessage = FTPRecv(pmx, pcom)))
        return NULL;
    return pmessage;
}


int FTPSendIdentity (MX* pmx, MXCom* pcom, char* Host, int Port, char* UserName, char* PassWord, char* Acct, char* fwHostName, char* fwUserName, char* fwPassWord,int fwPort,int LogonType)
{
    int NUMLOGIN = 9;
    int LO = -2;
    int ER = -1;
    LONG Code;
    char SCode[5];
    int logonseq[9][18] =
        {
            {0, -2,3 ,1, -2,6, 2 ,-2,-1,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},   /* no firewall*/
            {3, 6, 3 ,4, 6, -1,5 ,-1,9, 0, -2,12,1 ,-2,15,2 ,-2,-1},  /* SITE hostname */
            {3, 6, 3 ,4, 6, -1,6 ,-2,9, 1, -2,12,2 ,-2,-1,0 ,0 ,0},   /* USER after logon*/
            {7, 3, 3 ,0, -2,6, 1 ,-2,9, 2, -2,-1,0 ,0 ,0 ,0 ,0 ,0},   /* proxy OPEN */
            {3, 6, 3 ,4, 6, -1,0 ,-2,9, 1, -2,12,2 ,-2,-1,0 ,0 ,0},   /* Transparent*/
            {6, -2,3 ,1, -2,6, 2 ,-2,-1,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},   /* USER with no logon */
            {8, 6, 3 ,4, 6, -1,0 ,-2,9, 1, -2,12,2 ,-2,-1,0 ,0 ,0},   /* USER fireID@remotehost */
            {9, -1,3 ,1, -2,6, 2 ,-2,-1,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0},   /* USER remoteID@remotehost fireID */
            {10,-2,3 ,11,-2,6, 2 ,-2,-1,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0}    /* USER remoteID@fireID@remotehost */
        };
    MXMessage* pmessage;
    int port;
    int logonpoint = 0;
    char temp[500];
    char ToHost[500];
    char HostName[300];

    if ((LogonType < 0) || (LogonType >= NUMLOGIN))
        return -1;

    if(LogonType == 0)
    {
        strcpy (ToHost, Host);
        port = Port;
    }
    else
    {
        strcpy (ToHost, fwHostName);
        port = fwPort;
    }

    if(Port != 21)
        sprintf (HostName, "%s:%d", Host, Port);


    if(!(pmessage = FTPRecv (pmx, pcom)))
    {
        return -1;
    }
    MXFreeMessage (pcom->MX, pmessage);
    while(1)
    {
        pmessage = MXPutMessage (pcom, FTP_SYS, "Command");
        switch(logonseq[LogonType][logonpoint])
        {
        case 0:
            MXSetValue (pmessage, "Value", 1, "USER");
            sprintf (temp, "%s", UserName);
            break;
        case 1:
            MXSetValue (pmessage, "Value", 1, "PASS");
            sprintf (temp, "%s", PassWord);
            break;
        case 2:
            MXSetValue (pmessage, "Value", 1, "ACCT");
            sprintf (temp,  "%s",  Acct);
            break;
        case 3:
            MXSetValue (pmessage, "Value", 1, "USER");
            sprintf (temp,  "%s", fwUserName);
            break;
        case 4:
            MXSetValue (pmessage, "Value", 1, "PASS");
            sprintf (temp,  "%s", fwPassWord);
            break;
        case 5:
            MXSetValue (pmessage, "Value", 1, "SITE");
            sprintf (temp,  "%s", HostName);
            break;
        case 6:
            MXSetValue (pmessage, "Value", 1, "USER");
            sprintf (temp,  "%s@%s", UserName, HostName);
            break;
        case 7:
            MXSetValue (pmessage, "Value", 1, "OPEN");
            sprintf (temp,  "%s", HostName);
            break;
        case 8:
            MXSetValue (pmessage, "Value", 1, "USER");
            sprintf (temp,  "%s@%s", fwUserName, HostName);
            break;
        case 9:
            MXSetValue (pmessage, "Value", 1, "USER");
            sprintf (temp,  "%s@%s %s", UserName, HostName, fwUserName);
            break;
        case 10:
            MXSetValue (pmessage, "Value", 1, "USER");
            sprintf (temp,  "%s@%s@%s", UserName, fwUserName, HostName);
            break;
        case 11:
            MXSetValue (pmessage, "Value", 1, "PASS");
            sprintf (temp,  "%s@%s", PassWord, fwPassWord);
            break;
        }
        MXSetValue (pmessage, "Par", 1, temp);

        if(FTPSend(pmx, pcom, pmessage) < 0)
        {
            return -1;
        }
        if (!(pmessage = FTPRecv(pmx, pcom)))
        {
            return -1;
        }
        Code = *(LONG*)MXGetValue (pmessage, "Code", 1);
        MXFreeMessage (pcom->MX, pmessage);
        sprintf (SCode, "%d", Code);

        if ((SCode[0] != '2' ) && (SCode[0] != '3'))
        {
            return -1;
        }
        logonpoint = logonseq[LogonType][logonpoint + (Code / 100) - 1];
        switch (logonpoint)
        {
        case -1 :     /*ER*/
            return -1;
        case -2 :     /* LO*/
            return 1;
        }
    }
}


int FTPGetFile (MXCom* pcom, LONG Identity, char* FileName, char* DestName, CHAR FileType, char* NetId, LONG Port)
{

    MXMessage* pmessage;
    char Command[20];
    char OnlyFileName[120];
    char LocalRepertory[120];

    strcpy (Command, "GET");

    pmessage = MXPutMessage(pcom, FTP_SYS, "get") ;

    OSGetOnlyName (FileName, OnlyFileName, LocalRepertory, 'D');

    MXSetValue(pmessage, "FileName", 1, FileName) ;
    if (!DestName)
        MXSetValue(pmessage, "DestName", 1, OnlyFileName) ;
    else
        MXSetValue(pmessage, "DestName", 1, DestName) ;

    MXSetValue(pmessage, "FileType",        1, &FileType) ;
    MXSetValue(pmessage, "Identity",        1, &Identity) ;
    MXSetValue(pmessage, "Command",         1, &Command) ;
    if (NetId)
    {
        MXSetValue(pmessage, "NetId",          1, NetId) ;
        MXSetValue(pmessage, "Port",           1, &Port) ;
    }
    else
    {
        MXSetValue(pmessage, "NetId",          1, "") ;
        MXSetValue(pmessage, "Port",           1, &Port) ;
    }
    return 1;
}


int FTPSendFile (MXCom* pcom, LONG Identity, char* FileName, char* DestName, CHAR FileType, char* NetId, LONG Port)
{
    MXMessage* pmessage;
    FILEPARM File;
    char Command[20];
    char OnlyFileName[120];
    char LocalRepertory[120];
    int error = 0;

    strcpy (Command, "PUT");

    pmessage = MXPutMessage(pcom, FTP_SYS, "put") ;

    /* check of file exists otherwise no answer from server */
    if (OSVerifExistence (FileName, 'D', &error) == 0)
        return -1;


    OSGetOnlyName (FileName, OnlyFileName, LocalRepertory, 'D');

    strcpy (File.FileOri,  FileName);

    if (!DestName)
        strcpy (File.FileDest, OnlyFileName);
    else
        strcpy (File.FileDest, DestName);
    File.FileType = FileType;
    File.FileOriLocal = 'D';
    File.FileDestLocal = 'D';

    MXSetValue(pmessage, "File",     1, &File) ;
    MXSetValue(pmessage, "Identity", 1, &Identity) ;
    if (NetId)
    {
        MXSetValue(pmessage, "NetId",          1, NetId) ;
        MXSetValue(pmessage, "Port",           1, &Port) ;
    }
    else
    {
        MXSetValue(pmessage, "NetId",          1, "") ;
        MXSetValue(pmessage, "Port",           1, &Port) ;
    }

    return 1;
}

MXCom* FTPConnect(MX* pmx, char* Host, int Port)
{
    MXCom* pcom;
    pcom = MXOpenTCP (pmx, Host, Port, IPPROTO_FTP, NULL, NULL, TRUE);
    if (!pcom)  return NULL;
    return pcom;
}




int FTPDir (MXCom* pcom, LONG Identity, char* Name, CHAR FileLoc)
{
    /*

      request  = TPListenToTCP (pcom->MX->TP, 4455);
      OSGetSockName (1, netid);
      strcpy (netid, "212.155.201.33");
      i = 0;
      while (netid[i] != 0)
      {
          if (netid[i] == '.') netid[i] = ',';
          i++;
      }
      sprintf (Command, "%s,%d,%d", netid, 17, 103);

      pmessage = MXPutMessage (pcom, FTP_SYS, "Command");
      MXSetValue (pmessage, "Value", 1, "PORT");
      MXSetValue (pmessage, "Par", 1, Command);

      pmessage = MXPutMessage (pcom, FTP_SYS, "Command");
      MXSetValue (pmessage, "Value", 1, "LIST");
      if (Name)
        MXSetValue(pmessage, "Par",            1, Name) ;
      else
        MXSetValue(pmessage, "Par",            1, "") ;

      pmessage_reply = MXCreateMessage(pcom, FTP_SYS, "dir_reply") ;
    */

    return 1;
}

void FTPGetNetworkAndPort (char* Response, char* netid, int* port, BYTE Type)
{
    int i, j, k, hport, lport;
    char hSport[10];
    char lSport[10];
    char EndChar = 0;

    i = 0; j= 0; k = 0;
    if (Type == 0)
    {
        EndChar = ')';
        while (Response[i] != '(') i++;
        i++;
    }
    if (Response[i] == ',') /* No adress is given */
    {
        netid[j] = 0;
        i++;
    }
    else
    {
        while (k < 4)
        {
            if (Response[i] == ',')
            {
                netid[j] = '.';
                k++;
            }
            else
                netid[j] = Response[i];
            i++;
            j++;
        }

        netid[j-1] = 0;
        j = 0;
    }
    while (Response[i] != ',')
    {
        hSport[j] = Response[i];
        i++; j++;
    }
    hSport[j] = 0;
    hport = atoi (hSport);
    j = 0; i++;
    while (Response[i] != EndChar)
    {
        lSport[j] = Response[i];
        i++; j++;
    }
    lSport[j] = 0;
    lport = atoi (lSport);
    *port = hport * 256 + lport;
}


int SYS_get (MXMessage* pmessage, MXCom* pcom, void* apfield)
{
    FILEPARM File;
    MXMessage* pmessage1;
    LONG Error = 0;

    LONG Identity = *(LONG*)MXGetValue (pmessage, "Identity", 1);
    STRING NetId  =   (STRING)MXGetValue (pmessage, "NetId", 1);
    LONG Port     = *(LONG*)MXGetValue (pmessage,   "Port", 1);

    strcpy (File.FileOri,  MXGetValue (pmessage, "FileName", 1));
    strcpy (File.FileDest, MXGetValue (pmessage, "DestName", 1));
    File.FileOriLocal = 'D';
    File.FileDestLocal = 'D';
    File.FileType = *(CHAR*)MXGetValue (pmessage, "FileType", 1);

    pmessage1 = MXCreateMessage (pcom->MX, FTP_SYS, "get_reply");
    MXSetValue (pmessage1, "Value",    1, "RETR");
    MXSetValue (pmessage1, "Identity", 1, &Identity);
    MXSetValue (pmessage1, "File",     1, &File);
    MXSetValue (pmessage1, "NetId",    1, NetId);
    MXSetValue (pmessage1, "Port",     1, &Port);
    MXSetValue (pmessage1, "Error",    1, &Error);
    pmessage1->Com = pcom;


    ListNewr (&pcom->ActiveInputMessages, pmessage1);

    return 1;
}

int SYS_put (MXMessage* pmessage, MXCom* pcom, void* apfield)
{
    MXMessage* pmessage1;
    LONG       Error = 0;
    FILEPARM*  File;

    LONG Identity = *(LONG*)MXGetValue (pmessage, "Identity", 1);
    STRING NetId  =   (STRING)MXGetValue (pmessage, "NetId", 1);
    LONG Port     = *(LONG*)MXGetValue (pmessage,   "Port", 1);

    File = (FILEPARM*)MXGetValue (pmessage, "File", 1);

    pmessage1 = MXCreateMessage (pcom->MX, FTP_SYS, "put_reply");
    MXSetValue (pmessage1, "Value",    1, "STOR");
    MXSetValue (pmessage1, "Identity", 1, &Identity);
    MXSetValue (pmessage1, "FileName", 1, File->FileOri);
    MXSetValue (pmessage1, "DestName", 1, File->FileDest);
    MXSetValue (pmessage1, "Error",    1, &Error);
    MXSetValue (pmessage1, "NetId",    1, NetId);
    MXSetValue (pmessage1, "Port",     1, &Port);
    pmessage1->Com = pcom;
    ListNewr (&pcom->ActiveInputMessages, pmessage1);
    free (File);
    return 1;
}

int SYS_dir (MXMessage* pmessage, MXCom* pcom, void* apfield)
{
    BUFFERPARM Buffer;
    MXMessage* pmessage1;
    LONG       Error = 0;

    STRING Par    =   (STRING)MXGetValue (pmessage, "Par", 1);
    STRING Val    =   (STRING)MXGetValue (pmessage, "Value", 1);
    STRING NetId  =   (STRING)MXGetValue (pmessage, "NetId", 1);
    LONG Port     =   *(LONG *)MXGetValue (pmessage,   "Port", 1);
    LONG Identity =   *(LONG *)MXGetValue (pmessage, "Identity", 1);

    Buffer.BufferType    = 'T';
    Buffer.BufferSize    = 0;
    Buffer.BufferContent = NULL;

    pmessage1 = MXCreateMessage (pcom->MX, FTP_SYS, "dir_reply");
    MXSetValue (pmessage1, "Value",    1, Val);
    MXSetValue (pmessage1, "Par",      1, Par);
    MXSetValue (pmessage1, "Identity", 1, &Identity);
    MXSetValue (pmessage1, "NetId",    1, NetId);
    MXSetValue (pmessage1, "Port",     1, &Port);
    MXSetValue (pmessage1, "Error",    1, &Error);
    MXSetValue (pmessage1, "Buffer",   1, &Buffer);
    pmessage1->Com = pcom;


    ListNewr (&pcom->ActiveInputMessages, pmessage1);

    return 1;
}




int SYS_Command (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    STRING Value = (STRING)MXGetValue (pmessage, "Value", 1);
    STRING Par   = (STRING)MXGetValue (pmessage, "Par", 1);

    if (strcmp (Value, "PORT") == 0)
    {

        char NetId[250];
        char VNetId[250];
        int Port;
        char Command[250];
        FTPGetNetworkAndPort (Par, VNetId, &Port, 1);
        OSGetSockName(pcom->MX->TP->OS, pcom->Client->Pipe->fd, NetId) ;
        /* connection is done on the same machine */
        if ((strcmp (VNetId, "") == 0) || (strcmp (VNetId, NetId) == 0))
        {
            int i = 0;
            if (Port == 0)
            {
                int request  = TPListenToTCP (pcom->MX->TP, FTPCurrentPortNumber, IPPROTO_FTP);
                if (request == -1) return -1;
                Port = FTPCurrentPortNumber;
                FTPCurrentPortNumber++;
            }
            FTPCreateActiveConnection (pcom->MX, pcom, Port, NetId, 1);
            while (NetId[i] != 0)
            {
                if (NetId[i] == '.') NetId[i] = ',';  i++;
            }
            sprintf (Command, "%s,%d,%d", NetId, Port / 256, Port % 256);
            MXSetValue (pmessage, "Par", 1, Command);


        }
    }
    return 1;
}


int SYS_Command_Reply (MXMessage* pmessage, MXCom* pcom, void* apfield)
{
    char SCode[5];
    LONG   Code   = *(LONG*)MXGetValue (pmessage, "Code", 1);
    STRING Reply =   (STRING)MXGetValue (pmessage, "Reply", 1);
    LONG   Identity = *(LONG*)MXGetValue (pmessage, "Identity", 1);
    STRING Value  =   (STRING)MXGetValue (pmessage, "Value", 1);
    STRING Par    =   (STRING)MXGetValue (pmessage, "Par", 1);
    sprintf (SCode, "%d", Code);


    if ((strcmp (Value, "APPE") == 0) ||
            (strcmp (Value, "REIN") == 0) ||
            (strcmp (Value, "LIST") == 0) ||
            (strcmp (Value, "NLST") == 0) ||
            (strcmp (Value, "RETR") == 0) ||
            (strcmp (Value, "STOR") == 0) ||
            (strcmp (Value, "STOU") == 0))
    {
        List* listmessages = pcom->ActiveInputMessages;
        while (listmessages)
        {
            MXMessage* activemessage;
            STRING activevalue;
            LONG activeidentity;
            activemessage  = (MXMessage*)listmessages->car;
            activeidentity = *(LONG*)MXGetValue (activemessage, "Identity", 1);
            activevalue    = (STRING)MXGetValue (activemessage, "Value", 1);
            listmessages = listmessages->cdr;
            if (activeidentity == Identity)
            {
                STRING NetId = MXGetValue (activemessage, "NetId", 1);
                if ((strcmp (NetId, "") != 0) ||
                        (SCode[0] != '2'  && SCode[0] != '3'))
                {

                    MXSetValue (activemessage, "Error", 1, &Code);
                    ListRemove (&pcom->ActiveInputMessages, activemessage);
                    FTPEndOtherMessages (pcom->MX, pcom, activemessage);
                }
            }
        }
        return 1;
    }

    if (strcmp (Value, "PASV") == 0)
    {
        /*
             MXCom* ActiveCom;                              
             FTPActiveConnection* ActiveConnection;
             ActiveConnection = FTPCreateActiveConnection (pcom->MX, pcom, Port, NetId, 0);
             ActiveCom = MXOpenTCP(pcom->MX, NetId, Port, IPPROTO_TCP, NULL, NULL, TRUE);
             if (!ActiveCom) return 1;
             ActiveConnection->Done = 1;
             MXSetComProtocol (pcom->MX, ActiveCom, IPPROTO_TCP);
             MXAddEndingProcedure (ActiveCom, FTPCloseActiveConnection, pcom);
             ActiveCom->InternalField =  pcom;
             MXSetComApplicationField (ActiveCom, pcom->ApplicationField);
             MXSetTraceProcedure (ActiveCom, pcom->TraceProcedure);
             ActiveCom->Client->ReceiveFunction = FTPReceiveActiveMessage;
        */    
    }
    return 1;
}



