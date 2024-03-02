/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "pop.h"
#include "mail.h"

int POPAddFirstReplyMessage (MXCom* pcom)
{
    MXMessage* pmessage;
    int error = 0;

    pmessage = MXCreateMessage (pcom->MX, POP_SYS, "Command_Reply");
    MXSetValue (pmessage, "Value", 1, "OPEN");
    MXSetValue (pmessage, "Code",  1, &error);
    MXSetValue (pmessage, "Par",   1, "");
    pmessage->Com = pcom;
    ListInsert (&pcom->InputMessages, pmessage);
    return 1;
}



void POPTreatOtherMessages (MX* pmx, MXCom* pcom, MXMessage* pmessage)
{
    char buf[200];
    int i = 0;
    MXMessage* pmessage1;
    EMailMessage* Message1;
    List* ToList;


    LONG   Identity =  *(LONG *)  MXGetValue (pmessage, "Identity",  1);
    STRING From      = (STRING)MXGetValue (pmessage, "From",      1);
    STRING To        = (STRING)MXGetValue (pmessage, "To",        1);
    STRING Cc        = (STRING)MXGetValue (pmessage, "Cc",        1);
    STRING Bcc       = (STRING)MXGetValue (pmessage, "Bcc",       1);
    STRING Subject   = (STRING)MXGetValue (pmessage, "Subject",   1);
    STRING Object    = (STRING)MXGetValue (pmessage, "Object",    1);
    STRING Attachment= (STRING)MXGetValue (pmessage, "Attachment",1);


    Message1 = EMailCreateMessage (From, To, Cc, Bcc, Subject, Object);
    EMailPrepareHeader (Message1);
    EMailPrepareBody   (Message1);



    pmessage1 = MXPutMessage (pcom, FTP_SYS, "Command");
    MXSetValue (pmessage1, "Value", 1, "HELO");             /* Helo*/
    MXSetValue (pmessage1, "Par", 1, "friend");



    sprintf (buf, "<%s>", From);                            /* From*/
    pmessage1 = MXPutMessage (pcom, FTP_SYS, "Command");
    MXSetValue (pmessage1, "Value", 1, "MAIL From:");
    MXSetValue (pmessage1, "Par", 1, buf);


    for (i = 0; i < 3; i++)                                 /* To Cc Bcc */
    {
        if (i == 0)       ToList = Message1->ToList;
        else if (i == 1)  ToList = Message1->CCList;
        else              ToList = Message1->BCCList;
        while (ToList)
        {
            EMailRecipient* recipient = (EMailRecipient *)ToList->car;
            sprintf (buf, "<%s>", recipient->Address);
            pmessage1 = MXPutMessage (pcom, FTP_SYS, "Command");
            MXSetValue (pmessage1, "Value", 1, "RCPT TO:");
            MXSetValue (pmessage1, "Par", 1, buf);

            ToList = ToList->cdr;
        }
    }
    pmessage1 = MXPutMessage (pcom, FTP_SYS, "Command");  /* Data Start */
    MXSetValue (pmessage1, "Value", 1, "DATA");
    MXSetValue (pmessage1, "Par", 1, "");


    pmessage1 = MXPutMessage (pcom, FTP_SYS, "Command");  /* Header */
    MXSetValue (pmessage1, "Value", 1, "");
    MXSetValue (pmessage1, "Par", 1, Message1->Header);

    pmessage1 = MXPutMessage (pcom, FTP_SYS, "Command");  /* Body */
    MXSetValue (pmessage1, "Value", 1, "");
    MXSetValue (pmessage1, "Par", 1, Message1->Body);

    pmessage1 = MXPutMessage (pcom, FTP_SYS, "Command");  /* Data End */
    MXSetValue (pmessage1, "Value", 1, "");
    MXSetValue (pmessage1, "Par", 1, "\r\n.");


    pmessage1 = MXPutMessage (pcom, FTP_SYS, "Command");  /* Quit */
    MXSetValue (pmessage1, "Value", 1, "QUIT");
    MXSetValue (pmessage1, "Par", 1, "");


    EMailFreeMessage (Message1);

}



MXMessage* POPCheckAndReturnMessage (MX* pmx, MXCom* pcom, char* stream)
{
    MXMessage* pmessage = NULL;

    if (pcom->InputMessages)
        pmessage = (MXMessage*)pcom->InputMessages->car;
    return pmessage;
}


LONG POPReadMessage (MX* pmx, MXMessage* pmessage, char* stream, int streamsize, BYTE WithFileContent)
{
    LONG i = 0;
    LONG Code = 0;
    char* fromstream;
    MXMessageContext* pcontext = pmessage->Context;
    BOOL FirstTime = FALSE;

    MXObject*  pobject = MXGetObjectFromName (pmessage->MessageClass, "Reply");
    STRING     Value;
    fromstream = stream;

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

    if (pmessage->StreamSize < pmessage->Offset + streamsize)
        if (MXExtendValueBuffer (pmessage, MXBUFVALUESIZE) == 0) return -1;

    Value = MXGetValue (pmessage, "Value", 1);

    if (FirstTime == TRUE)
    {
        pmessage->EndChar[0] = '\n';
        if (OSStrncmp(stream,"-ERR", 4) == 0)
        {
            Code = 1;
            MXSetValue (pmessage, "Code", 1, &Code);
        }
        else
            if (OSStrncmp (Value, "LIST", 4) == 0)
                    pmessage->EndChar[0] = '.';    else
                if (OSStrncmp (Value, "RETR", 4) == 0)
                {
                    EMailMessage* Message = (EMailMessage *)malloc (sizeof (EMailMessage));
                    Message->From  =  NULL;
                    Message->ToList  = NULL;
                    Message->CCList  = NULL;
                    Message->BCCList = NULL;
                    Message->Header  = NULL;
                    Message->MIMEList = NULL;           /* Parts       */
                    Message->HeaderSize = 0;
                    Message->BodySize = 0;
                    Message->Header = NULL;
                    Message->Body = NULL;
                    pcontext->Buffer = (char*)Message;
                    strcpy (pmessage->EndChar, "\r\n\r\n");
                }
        pmessage->Values[(WORD)pobject->Offset] = pmessage->Offset;
    }


    if ((OSStrncmp (Value, "RETR", 4) == 0)  &&
            ((!FirstTime) ||
             (OSStrncmp(stream,"-ERR", 4) != 0)))

    {
        char* pos;
        if (pcontext->StreamSize == 0)
        {
            pcontext->Stream = (char*)malloc (streamsize + 1);
            pcontext->StreamSize  = streamsize + 1;
            memcpy (pcontext->Stream, stream , streamsize);
            pcontext->Stream[streamsize] = 0;
        }
        else
        {
            pcontext->Stream = (char*)realloc (pcontext->Stream, streamsize + pcontext->StreamSize);
            memcpy (pcontext->Stream + pcontext->StreamSize - 1, stream, streamsize);
            pcontext->StreamSize += streamsize;
            pcontext->Stream[pcontext->StreamSize -1] = 0;
        }
        pos = strstr (pcontext->Stream, pmessage->EndChar);
        if (pos)
        {
            if (strcmp (pmessage->EndChar, "\r\n\r\n") == 0) /*header found */
            {
                EMailMessage* mMessage;
                List* List;
                int j = 0;
                int Range;
                i = pos - (pcontext->Stream + pcontext->StreamPos);
                i += 4;


                mMessage = (EMailMessage*)pcontext->Buffer;
                mMessage->Header = malloc (pcontext->StreamPos + i + 1);
                memset (mMessage->Header , 0, pcontext->StreamPos + i + 1);
                memcpy (mMessage->Header, pcontext->Stream, pcontext->StreamPos + i);
                EMailDecodeHeader (mMessage);
                MXSetValue (pmessage, "Subject", 1, mMessage->Subject);
                MXSetValue (pmessage, "From",    1, mMessage->From);
                MXSetValue (pmessage, "Date",    1, mMessage->Date);
                for (j = 1; j <= 3; j++)
                {
                    if (j == 1) List = mMessage->ToList;
                    else if (j == 2) List = mMessage->CCList;
                    else List = mMessage->BCCList;
                    Range = 1;
                    while (List)
                    {
                        char Name[200];
                        EMailRecipient* recipient = (EMailRecipient *)List->car;
                        sprintf (Name, "%s<%s>", recipient->Name, recipient->Address);
                        if (j == 1)
                            MXSetValue (pmessage, "To", Range, Name);
                        else  if (j == 2)
                            MXSetValue (pmessage, "Cc", Range, Name);
                        else
                            MXSetValue (pmessage, "Bcc", Range, Name);
                        Range++;
                        List = List->cdr;
                    }
                }

                strcpy (pmessage->EndChar, "\r\n.\r\n");
                free (pcontext->Stream);
                pcontext->Stream = NULL;
                pcontext->StreamSize = 0;
                pcontext->StreamPos = 0;
                pcontext->Pos += 1;
            }
            else                                             /* body found */
            {
                STRING Body;
                EMailMessage* mMessage;

                i = pos - (pcontext->Stream + pcontext->StreamPos);
                i += 5;

                mMessage = (EMailMessage*)pcontext->Buffer;
                mMessage->Body = malloc (pcontext->StreamPos + i + 1);
                memset (mMessage->Body , 0, pcontext->StreamPos + i + 1);
                memcpy (mMessage->Body, pcontext->Stream, pcontext->StreamPos + i);
                EMailDecodeBody (mMessage);
                MXSetValue (pmessage, "Body", 1, mMessage->Body);
                Body = (STRING)MXGetValue (pmessage, "Body", 1);
                free (pcontext->Stream);
                pcontext->StreamSize = 0;
                pcontext->Stream = NULL;
                pcontext->StreamPos = 0;
                pcontext->Pos += 1;
                if (pcontext->Pos == 2)
                {
                    EMailFreeMessage ((EMailMessage*)pcontext->Buffer);
                    pmessage->Complete = 1;
                }
            }
        }
        else
        {
            i = streamsize;
            pcontext->StreamPos += i;
        }

    }
    else
    {
        while ((i < streamsize) && (fromstream[i] != pmessage->EndChar[0]))  i++;
        if ((fromstream[i] ==  pmessage->EndChar[0]) && (pmessage->EndChar[0] != '.'))
        {
            fromstream[i-1] = 0;
            fromstream[i] = 0;
            i++;
            memcpy (pmessage->Stream + pmessage->Offset, fromstream, i);
            pmessage->Offset += i;
            pmessage->Complete = 1;

            Code = *(LONG *)MXGetValue (pmessage, "Code", 1);

            if ((OSStrncmp (Value, "STAT", 4) == 0)  && (Code == 0))
            {
                LONG m_NumberMail = 0;
                LONG m_TotalSize = 0;
                int First = 1;
                char* p;
                char* p1;

                p1 = MXGetValue (pmessage, "Reply", 1);

                while ((p = strstr (p1, "+OK")) != NULL)
                    p1 = p + 3;

                for (p = p1; *p != '\0'; p++)
                {
                    if (*p == '\t' || *p == ' ')
                    {
                        if (First)
                        {
                            m_NumberMail = atoi(p);
                            MXSetValue (pmessage, "TotalNumber", 1, &m_NumberMail);
                            First = 0;
                        }
                        else
                        {
                            m_TotalSize = atoi(p);
                            MXSetValue (pmessage, "TotalSize", 1,   &m_TotalSize);
                        }
                    }
                }
            }
            else
                if ((OSStrncmp (Value, "LIST", 4) == 0)  && (Code == 0))
                {
                    LONG m_Number = 0;
                    LONG m_Size = 0;
                    LONG m_NumberMail = 0;
                    LONG m_TotalSize = 0;

                    int Range = 1;
                    char* p;

                    p = MXGetValue (pmessage, "Reply", 1);

                    p = strstr (p, "\r\n") + 2;

                    for (p = p; *p != '.'; p++)
                        if (*p == ' ')
                        {
                            m_Size = atoi(p + 1);
                            MXSetValue (pmessage, "Size",   Range,   &m_Size);
                            MXSetValue (pmessage, "Number", Range,   &Range);
                            m_TotalSize += m_Size;
                            Range++;
                            m_NumberMail += 1;
                        }
                    MXSetValue (pmessage, "TotalNumber", 1, &m_NumberMail);
                    MXSetValue (pmessage, "TotalSize", 1,   &m_TotalSize);

                }

        }
        else
        {
            if (pmessage->EndChar[0] == '.')
            {
                pmessage->EndChar[0] = '\n';
                i++;
            }
            memcpy (pmessage->Stream + pmessage->Offset, fromstream, i);
            pmessage->Offset += i;
        }
    }
    MXInitMessage (pmessage);
    return i;
}




LONG POPWriteMessage (MX* pmx, MXMessage* pmessage, char* stream, int maxcount, BYTE WithFileContent)
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


LONG POPSendMessage (TP* ptp, TPClient* pclient, char* buf, int size)
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
    if (POPNotCommandMessage (pmessage))
    {
        POPTreatOtherMessages (pmx, pcom, pmessage);
        ListRemove(&pcom->OutputMessages, pmessage);
        MXFreeMessage (pcom->MX, pmessage);
        /* pick next message */
        return POPSendMessage (ptp, pclient, buf, size);
    }


    TPSetBlockingMode (pcom->Client, pmessage->MessageClass->BlockedOnOutput);

    ret = POPWriteMessage (pmx, pmessage, buf, size, 1);

    /* POP */

    if (ret < 0)
    {
        ListRemove(&pcom->OutputMessages, pmessage);
        MXFreeMessage (pcom->MX, pmessage);
        return -1;
    }
    if (pcom->TraceProcedure)   pcom->TraceProcedure (pmessage, MXONSEND, buf, ret);
    if (pmessage->Complete)
    {
        MXMessage* pmessage1;
        STRING Value   = (STRING)MXGetValue (pmessage, "Value", 1);
        LONG Identity = *(LONG *)MXGetValue (pmessage, "Identity", 1);

        if (OSStrncmp (Value, "LIST", 4) == 0)
            pmessage1 = MXCreateMessage (pmx, POP_SYS , "List_Reply");
        else
            if (OSStrncmp (Value, "STAT", 4) == 0)
                pmessage1 = MXCreateMessage (pmx, POP_SYS , "Stat_Reply");
            else
                if (OSStrncmp (Value, "RETR", 4) == 0)
                    pmessage1 = MXCreateMessage (pmx, POP_SYS , "Retr_Reply");
                else
                    pmessage1 = MXCreateMessage (pmx, POP_SYS , "Command_Reply");

        MXSetValue (pmessage1, "Value", 1, Value);
        MXSetValue (pmessage1, "Identity", 1, &Identity);
        MXSetValue (pmessage1, "Par",   1, MXGetValue (pmessage, "Par", 1));
        pmessage1->Com = pcom;
        ListNewr (&pcom->InputMessages, pmessage1);

        TPSetBlockingMode (pcom->Client, MXNoBlock);
        ListRemove (&pcom->OutputMessages, pmessage);
        if (!(pmessage->Type & MXACK))
            MXFreeMessage (pcom->MX, pmessage);
        else
            ListNewr (&pcom->AckMessages, pmessage);
    }

    return ret;
}

LONG POPSend (MX* pmx, MXCom* pcom, MXMessage* pmessage)
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
    while (1)
    {
        ret = POPWriteMessage (pmx, pmessage, pcom->Client->TransBuffer, pcom->Client->TransSize - 2, 1);

        if (ret < 0)
        {
            TPSetBlockingMode (pcom->Client, FALSE);

            /* A FAIRE  REVOIR LA LIBERATION DES MESSAGES EN BLOQUANT */
            /* ====================================================== */
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
            STRING Value   = (STRING)MXGetValue (pmessage, "Value", 1);
            LONG Identity  = *(LONG *)MXGetValue (pmessage, "Identity", 1);
            if (OSStrncmp (Value, "LIST", 4) == 0)
                pmessage1 = MXCreateMessage (pmx, POP_SYS , "List_Reply");
            else
                if (OSStrncmp (Value, "STAT", 4) == 0)
                    pmessage1 = MXCreateMessage (pmx, POP_SYS , "Stat_Reply");
                else
                    if (OSStrncmp (Value, "RETR", 4) == 0)
                        pmessage1 = MXCreateMessage (pmx, POP_SYS , "Retr_Reply");
                    else
                        pmessage1 = MXCreateMessage (pmx, POP_SYS , "Command_Reply");
            MXSetValue (pmessage1, "Value", 1, Value);
            MXSetValue (pmessage1, "Par",   1, MXGetValue (pmessage, "Par", 1));
            MXSetValue (pmessage1, "Identity", 1, &Identity);
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




LONG POPReceiveMessage (TP* ptp, TPClient* pclient, char* buf, int count)
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
        pmessage = POPCheckAndReturnMessage (pmx, pcom, stream);

        if (!pmessage)
            return -1;

        pmessage->Com = pcom;


        ret = POPReadMessage (pmx, pmessage, stream, count - readcount, 1);
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


MXMessage* POPRecv (MX* pmx, MXCom* pcom)
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
            pmessage = POPCheckAndReturnMessage (pmx, pcom, stream);
            if (!pmessage)
            {
                TPSetBlockingMode (pcom->Client, FALSE);
                return NULL;
            }
            pmessage->Com = pcom;
            if (pcom->TraceProcedure) pcom->TraceProcedure (pmessage, MXONRECV, stream, ret);

            ret = POPReadMessage (pmx, pmessage, stream, ret, 1);
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
    if (!pcom->Closed) TPSetBlockingMode (pcom->Client, FALSE);
    return pmessage;
}


int SYS_POPCommand (MXMessage* pmessage, MXCom* pcom, void* apfield)
{
    STRING Value = (STRING)MXGetValue (pmessage, "Value", 1);
    STRING Par   = (STRING)MXGetValue (pmessage, "Par", 1);

    return 1;
}


int SYS_POPCommand_Reply (MXMessage* pmessage, MXCom* pcom, void* apfield)
{
    LONG   Identity = *(LONG *)MXGetValue (pmessage, "Identity", 1);
    STRING Value  =   (STRING)MXGetValue (pmessage, "Value", 1);
    STRING Par    =   (STRING)MXGetValue (pmessage, "Par", 1);
    LONG   Code   =   *(LONG *)MXGetValue (pmessage, "Code", 1);
    STRING Reply =    (STRING)MXGetValue (pmessage, "Reply", 1);
    return 1;
}






TPClient* POPConnect(TP* ptp, char* ToHost, char* User, char* Password)
{

    TPClient* pclient;
    pclient = TPEstablishConnectionWith (ptp, ToHost, 110, IPPROTO_TCP, NULL, NULL, TRUE);
    if (!pclient)
    {
        sprintf (m_ErrorMessage, "%s", "Server cannot be connected");
        return FALSE;
    }
    else
    {
        char buf [512];
        TPSetClientProtocol (ptp, pclient, IPPROTO_TCP);
        if(POPCheckResponse(pclient, CONNECTION_CHECK)==FALSE)
        {
            TPCloseDownClient (ptp, pclient);
            return NULL;
        }
        sprintf (buf, "USER %s\r\n", User);
        TPWrite(pclient->TP, pclient, buf, (int)strlen (buf));
        if(POPCheckResponse(pclient, USER_CHECK)==FALSE)
        {
            TPCloseDownClient (ptp, pclient);
            return NULL;
        }
        sprintf (buf, "PASS %s\r\n",  Password);
        TPWrite(pclient->TP, pclient, buf, (int)strlen (buf));
        if (POPCheckResponse(pclient, PASSWORD_CHECK)==FALSE)
        {
            TPCloseDownClient (ptp, pclient);
            return NULL;
        }
        return pclient;
    }
}


BOOL POPDisconnect(TPClient* pclient)
{
    char buf [512];

    sprintf (buf, "QUIT \r\n");
    TPWrite(pclient->TP, pclient, buf, (int)strlen (buf));
    if (POPCheckResponse(pclient, QUIT_CHECK)==FALSE)
    {
        TPCloseDownClient (pclient->TP, pclient);
        return FALSE;
    }
    else
        return TRUE;
}



BOOL POPDelete(TPClient* pclient, int MsgNumber)
{
    char buf [512];

    sprintf (buf, "DELE %d\r\n",MsgNumber );
    TPWrite (pclient->TP, pclient, buf, (int)strlen (buf));
    if (POPCheckResponse(pclient, DELETE_CHECK)==FALSE)
    {
        TPCloseDownClient (pclient->TP, pclient);
        return FALSE;
    }
    else
        return TRUE;
}


BOOL POPNoop(TPClient* pclient)
{
    char buf [512];

    sprintf (buf, "NOOP  \r\n");
    TPWrite (pclient->TP, pclient, buf, (int)strlen (buf));
    if (POPCheckResponse(pclient, NOOP_CHECK)==FALSE)
    {
        TPCloseDownClient (pclient->TP, pclient);
        return FALSE;
    }
    else
        return TRUE;
}

/*
int POPGetMessageSize(int MsgNumber) 
{ 
   if(m_SizeOfMsg.GetSize() < MsgNumber+1) 
    return 0; 
   else 
    return m_SizeOfMsg[MsgNumber+1]; 
} 

*/


BOOL POPRetrieve(TPClient* pclient, int MsgNumber)
{
    char buf [512];

    sprintf (buf, "RETR %d\r\n",MsgNumber );
    TPWrite (pclient->TP, pclient, buf, (int)strlen (buf));

    if (POPCheckResponse(pclient, RETR_CHECK)==FALSE)
    {
        TPCloseDownClient (pclient->TP, pclient);
        return FALSE;
    }
    else
        return TRUE;
}


BOOL POPReset(TPClient* pclient)
{
    char buf [512];

    sprintf (buf, "RSET \r\n");
    TPWrite (pclient->TP, pclient, buf, (int)strlen (buf));

    if (POPCheckResponse(pclient, RSET_CHECK)==FALSE)
    {
        TPCloseDownClient (pclient->TP, pclient);
        return FALSE;
    }
    else
        return TRUE;
}

BOOL POPStatistics(TPClient* pclient)
{
    char buf [512];

    sprintf (buf, "STAT \r\n");
    TPWrite (pclient->TP, pclient, buf, (int)strlen (buf));

    if (POPCheckResponse(pclient, STAT_CHECK)==FALSE)
        return FALSE;
    else
        return TRUE;
}


BOOL POPList(TPClient* pclient)
{
    char buf [512];

    sprintf (buf, "LIST  \r\n");
    TPWrite (pclient->TP, pclient, buf, (int)strlen (buf));

    if (POPCheckResponse(pclient, LIST_CHECK)==FALSE)
    {
        TPCloseDownClient (pclient->TP, pclient);
        return FALSE;
    }
    else
        return TRUE;
}


BOOL POPCheckResponse(TPClient* pclient, int ResponseType)
{
    char* buf;
    switch (ResponseType)
    {
    case CONNECTION_CHECK:
        if (TPRead(pclient->TP, pclient) < 0)
            return FALSE;
        buf = pclient->RequestBuffer;
        if (OSStrncmp(buf,"-ERR", 4) == 0)
        {
            sprintf (m_ErrorMessage, "%s", "Bad Connection");
            return FALSE;
        }
        break;
    case USER_CHECK:
        if (TPRead(pclient->TP, pclient) < 0) return FALSE;
        buf = pclient->RequestBuffer;
        if (OSStrncmp(buf,"-ERR", 4) == 0)
        {
            sprintf (m_ErrorMessage, "%s", "Bad User Name");
            return FALSE;
        }
        break;
    case PASSWORD_CHECK:
        if (TPRead(pclient->TP, pclient) < 0) return FALSE;
        buf = pclient->RequestBuffer;

        if (OSStrncmp(buf,"-ERR", 4) == 0)
        {
            sprintf (m_ErrorMessage, "%s", "Bad Password Name");
            return FALSE;
        }
        break;
    case QUIT_CHECK:
        if (TPRead(pclient->TP, pclient) < 0) return FALSE;
        buf = pclient->RequestBuffer;

        if (OSStrncmp(buf,"-ERR", 4) == 0)
        {
            sprintf (m_ErrorMessage, "%s", "Error occured during QUIT");
            return FALSE;
        }
        break;
    case DELETE_CHECK:
        if (TPRead(pclient->TP, pclient) < 0) return FALSE;
        buf = pclient->RequestBuffer;

        if (OSStrncmp(buf,"-ERR", 4) == 0)
        {
            sprintf (m_ErrorMessage, "%s", "Error occured during DELE");
            return FALSE;
        }
        break;
    case RSET_CHECK:
        if (TPRead(pclient->TP, pclient) < 0) return FALSE;
        buf = pclient->RequestBuffer;

        if (OSStrncmp(buf,"-ERR", 4) == 0)
        {
            sprintf (m_ErrorMessage, "%s", "Error occured during RSET");
            return FALSE;
        }
        break;
    case STAT_CHECK:
        if (TPRead(pclient->TP, pclient) < 0) return FALSE;
        buf = pclient->RequestBuffer;

        if (OSStrncmp(buf,"-ERR", 4) == 0)
        {
            sprintf (m_ErrorMessage, "%s", "Error occured during STAT");
            return FALSE;
        }
        else
        {
            char* p;
            char* p1;
            int i = 0;
            int size;
            BOOL EmailNumber = TRUE;

            buf = (char*)malloc (pclient->RequestSize + 1);
            size = pclient->RequestSize;
            memcpy (buf, pclient->RequestBuffer, pclient->RequestSize);
            buf[pclient->RequestSize] = 0;

            while (strstr (buf, "+OK") == NULL)
            {
                if (TPRead(pclient->TP, pclient) < 0)
                {
                    free (buf);
                    return FALSE;
                }
                buf = (char *)realloc (buf, size + pclient->RequestSize + 1);
                memcpy (buf + size, pclient->RequestBuffer, pclient->RequestSize);
                size += pclient->RequestSize;
                buf[size] = 0;
            }

            p1= buf;

            while ((p = strstr (p1, "+OK")) != NULL)
                p1 = p + 3;

            m_NumberMail = 0;
            m_TotalSize = 0;
            for (p = p1; *p != '\0'; p++)
            {
                if (*p == '\t' || *p == ' ')
                {
                    if(EmailNumber == TRUE)
                    {
                        m_NumberMail = atoi(p);
                        EmailNumber = FALSE;
                    }
                    else
                    {
                        m_TotalSize = atoi(p);
                        return TRUE;
                    }
                }
            }
        }
        break;
    case NOOP_CHECK:
        if (TPRead(pclient->TP, pclient) < 0) return FALSE;
        buf = pclient->RequestBuffer;
        if (OSStrncmp(buf,"-ERR", 4) == 0)
        {
            sprintf (m_ErrorMessage, "%s", "Error occured during NOOP");
            return FALSE;
        }
        break;
    case LIST_CHECK:
        if (TPRead(pclient->TP, pclient) < 0) return FALSE;
        buf = pclient->RequestBuffer;

        if (OSStrncmp(buf,"-ERR", 4) == 0)
        {
            sprintf (m_ErrorMessage, "%s", "Error occured during LIST");
            return FALSE;
        }
        else
        {
            char* p;
            int i = 0;
            int size;
            buf = (char*)malloc (pclient->RequestSize + 1);
            size = pclient->RequestSize;
            memcpy (buf, pclient->RequestBuffer, pclient->RequestSize);
            buf[pclient->RequestSize] = 0;

            while (strstr (buf, ".") == NULL)
            {
                if (TPRead(pclient->TP, pclient) < 0)
                {
                    free (buf);
                    return FALSE;
                }
                buf = (char *)realloc (buf, size + pclient->RequestSize + 1);
                memcpy (buf + size, pclient->RequestBuffer, pclient->RequestSize);
                size += pclient->RequestSize;
                buf[size] = 0;
            }

            p = strstr (buf, "\r\n") + 2;

            for (p = p; *p != '.'; p++)
                if (*p == ' ')
                {
                    if (i <  100) m_ListSize[i] = atoi(p + 1);
                    i++;
                }
        }
        break;
    case RETR_CHECK:
        {
            if (TPRead(pclient->TP, pclient) < 0) return FALSE;
            buf = pclient->RequestBuffer;


            if (OSStrncmp(buf,"-ERR", 4) == 0)
            {
                sprintf (m_ErrorMessage, "%s", "Error occured during RETR");
                return FALSE;
            }
            else
            {

            }

        }
        break;
    }
    return TRUE;
}
/*
     DWORD BufferSize;
     char* bufferattributes;
     if (streamsize < pcontext->StillToRead)
     {
        i = streamsize;
     }
     else
     {
        i =  pcontext->StillToRead ;
        pmessage->Complete = 1;
     }
     pcontext->StillToRead -= i;

     bufferattributes = (char*) (pmessage->Stream + pmessage->Values[(WORD)pobject->Offset]);
     OSCopy4(&BufferSize, bufferattributes);
     BufferSize += i;
     memcpy (bufferattributes , (CHAR *)(&BufferSize) , sizeof (DWORD));
     pcontext->Buffer = bufferattributes + sizeof (DWORD) + sizeof (CHAR);
     memcpy ((char *)((DWORD)pcontext->Buffer + pcontext->Pos), fromstream , i);
     pcontext->Pos += i;
     pmessage->Offset += i;
*/


