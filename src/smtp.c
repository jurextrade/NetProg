/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/

#define _CRT_SECURE_NO_WARNINGS

#include "smtp.h"
#include "mail.h"

int SMTPAddFirstReplyMessage (MXCom* pcom)
{
    MXMessage* pmessage;
    int error = 0;

    pmessage = MXCreateMessage (pcom->MX, SMTP_SYS, "Command_Reply");
    MXSetValue (pmessage, "Value", 1, "OPEN");
    MXSetValue (pmessage, "Code",  1, &error);
    MXSetValue (pmessage, "Par",   1, "");
    pmessage->Com = pcom;
    ListInsert (&pcom->InputMessages, pmessage);
    return 1;
}

void SMTPTreatOtherMessages (MX* pmx, MXCom* pcom, MXMessage* pmessage)
{
    char buf[200];
    int i = 0;
    MXMessage* pmessage1;
    EMailMessage* Message1;
    List* RecList;
	EMailRecipient* recipient;

    LONG   Identity = *(LONG*)MXGetValue (pmessage, "Identity",  1);
    STRING From      = (STRING)MXGetValue (pmessage, "From",      1);
    STRING To        = (STRING)MXGetValue (pmessage, "To",        1);
    STRING Cc        = (STRING)MXGetValue (pmessage, "Cc",        1);
    STRING Bcc       = (STRING)MXGetValue (pmessage, "Bcc",       1);
    STRING Subject   = (STRING)MXGetValue (pmessage, "Subject",   1);
    STRING Object    = (STRING)MXGetValue (pmessage, "Object",    1);
    STRING Attachment= (STRING)MXGetValue (pmessage, "Attachment",1);


    Message1 = EMailCreateMessage (From, To, Cc, Bcc, Subject, Object);

    if (strcmp (Attachment, "") != 0)
        EMailAddMimePart(Message1, Attachment, APPLICATION_OCTETSTREAM,"" ,BASE64, FALSE );


    EMailPrepareHeader (Message1);
    EMailPrepareBody   (Message1);



    pmessage1 = MXPutMessage (pcom, SMTP_SYS, "Command");
    if (pmessage1)
    {
        MXSetValue (pmessage1, "Value", 1, "HELO");             /* Helo*/
        MXSetValue (pmessage1, "Par", 1, "friend");
    }


	RecList = Message1->FromList;
    recipient= (EMailRecipient *)RecList->car;   /* From*/
 	sprintf (buf, "<%s>", recipient->Address);     



    pmessage1 = MXPutMessage (pcom, SMTP_SYS, "Command");
    if (pmessage1)
    {
        MXSetValue (pmessage1, "Value", 1, "MAIL FROM:");
        MXSetValue (pmessage1, "Par", 1, buf);
    }

    for (i = 0; i < 3; i++)                                 /* To Cc Bcc */
    {
        if (i == 0)       RecList = Message1->ToList;
        else 
		if (i == 1)		  RecList = Message1->CCList;
        else              RecList = Message1->BCCList;
        
		
		while (RecList)
        {
            recipient = (EMailRecipient *)RecList->car;
			sprintf (buf, "<%s>", recipient->Address);
            pmessage1 = MXPutMessage (pcom, SMTP_SYS, "Command");
            if (pmessage1)
            {
                MXSetValue (pmessage1, "Value", 1, "RCPT TO:");
                MXSetValue (pmessage1, "Par", 1, buf);
            }
            RecList = RecList->cdr;
        }
    }
    pmessage1 = MXPutMessage (pcom, SMTP_SYS, "Command");  /* Data Start */
    if (pmessage1)
    {
        MXSetValue (pmessage1, "Value", 1, "DATA");
        MXSetValue (pmessage1, "Par", 1, "");
    }
	
    pmessage1 = MXPutMessage (pcom, SMTP_SYS, "Command");  /* Header */
    if (pmessage1)
    {
        MXSetValue (pmessage1, "Value", 1, "");
        MXSetValue (pmessage1, "Par", 1, Message1->Header);
    }
    pmessage1 = MXPutMessage (pcom, SMTP_SYS, "Command");  /* Body */
    if (pmessage1)
    {
        MXSetValue (pmessage1, "Value", 1, "");
        MXSetValue (pmessage1, "Par", 1, Message1->Body);
    }

    pmessage1 = MXPutMessage (pcom, SMTP_SYS, "Command");  /* Data End */
    if (pmessage1)
    {
        MXSetValue (pmessage1, "Value", 1, "");
        MXSetValue (pmessage1, "Par", 1, "\r\n.");
    }

    pmessage1 = MXPutMessage (pcom, SMTP_SYS, "Command");  /* Quit */
    if (pmessage1)
    {
        MXSetValue (pmessage1, "Value", 1, "QUIT");
        MXSetValue (pmessage1, "Par", 1, "");
    }

    EMailFreeMessage (Message1);

}



MXMessage* SMTPCheckAndReturnMessage (MX* pmx, MXCom* pcom, char* stream)
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
        pmessage = MXCreateMessage (pmx, SMTP_SYS, "Command_Reply");
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
            pmessage = MXCreateMessage (pmx, SMTP_SYS, "Command_Reply");
            MXSetValue (pmessage, "Value", 1, "SMTP");
            MXSetValue (pmessage, "Par",   1, "");
            pmessage->Com = pcom;

        }
    return pmessage;
}


LONG SMTPReadMessage (MX* pmx, MXMessage* pmessage, char* stream, int RequestSize, BYTE WithFileContent)
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




LONG SMTPWriteMessage (MX* pmx, MXMessage* pmessage, char* stream, int maxcount, BYTE WithFileContent)
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


LONG SMTPSendMessage (TP* ptp, TPClient* pclient, char* buf, int size)
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
    if (SMTPNotCommandMessage (pmessage))
    {
        SMTPTreatOtherMessages (pmx, pcom, pmessage);
        ListRemove(&pcom->OutputMessages, pmessage);
        MXFreeMessage (pcom->MX, pmessage);
        /* pick next message */
        return SMTPSendMessage (ptp, pclient, buf, size);
    }


    TPSetBlockingMode (pcom->Client, pmessage->MessageClass->BlockedOnOutput);

    ret = SMTPWriteMessage (pmx, pmessage, buf, size, 1);

    /* smtp */

    if (ret < 0)
    {
        ListRemove(&pcom->OutputMessages, pmessage);
        MXFreeMessage (pcom->MX, pmessage);
        return -1;
    }
    if (pcom->TraceProcedure)   pcom->TraceProcedure (pmessage, MXONSEND, buf, ret);
    if (pmessage->Complete)
    {

        if (strcmp (MXGetValue (pmessage, "Value", 1), "") != 0)
        {
            MXMessage* pmessage1;
            LONG Identity = *(LONG*)MXGetValue (pmessage, "Identity", 1);
            pmessage1 = MXCreateMessage (pmx, SMTP_SYS , "Command_Reply");
            MXSetValue (pmessage1, "Value", 1, MXGetValue (pmessage, "Value", 1));
            MXSetValue (pmessage1, "Identity", 1, &Identity);
            MXSetValue (pmessage1, "Par",   1, MXGetValue (pmessage, "Par", 1));
            pmessage1->Com = pcom;
            ListNewr (&pcom->InputMessages, pmessage1);
        }

        TPSetBlockingMode (pcom->Client, MXNoBlock);
        ListRemove (&pcom->OutputMessages, pmessage);
        if (!(pmessage->Type & MXACK))
            MXFreeMessage (pcom->MX, pmessage);
        else
            ListNewr (&pcom->AckMessages, pmessage);
    }

    return ret;
}

LONG SMTPSend (MX* pmx, MXCom* pcom, MXMessage* pmessage)
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
        ret = SMTPWriteMessage (pmx, pmessage, pcom->Client->TransBuffer, pcom->Client->TransSize - 2, 1);

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

            if (strcmp (MXGetValue (pmessage, "Value", 1), "") != 0)
            {
                MXMessage* pmessage1;
                LONG Identity= *(LONG*)MXGetValue (pmessage, "Identity", 1);
                pmessage1 = MXCreateMessage (pmx, SMTP_SYS , "Command_Reply");
                MXSetValue (pmessage1, "Value", 1, MXGetValue (pmessage, "Value", 1));
                MXSetValue (pmessage1, "Par",   1, MXGetValue (pmessage, "Par", 1));
                MXSetValue (pmessage1, "Identity", 1, &Identity);
                MXSetValue (pmessage1, "Reply", 1, "");
                pmessage1->Com = pcom;
                ListNewr (&pcom->InputMessages, pmessage1);
            }
            ListRemove (&pcom->OutputMessages, pmessage);
            if (pmessage->Type & MXACK)
                ListNewr (&pcom->AckMessages, pmessage);
            break;
        }
    }
    TPSetBlockingMode (pcom->Client, FALSE);
    return ret1;
}




LONG SMTPReceiveMessage (TP* ptp, TPClient* pclient, char* buf, int count)
{
    char* stream;
    MX* pmx;
    MXMessage* pmessage;
    LONG ret;
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
        pmessage = SMTPCheckAndReturnMessage (pmx, pcom, stream);

        if (!pmessage)
            return -1;

        pmessage->Com = pcom;


        ret = SMTPReadMessage (pmx, pmessage, stream, count - readcount, 1);
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


MXMessage* SMTPRecv (MX* pmx, MXCom* pcom)
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
            pmessage = SMTPCheckAndReturnMessage (pmx, pcom, stream);
            if (!pmessage)
            {
                TPSetBlockingMode (pcom->Client, FALSE);
                return NULL;
            }
            pmessage->Com = pcom;
            if (pcom->TraceProcedure) pcom->TraceProcedure (pmessage, MXONRECV, stream, ret);

            ret = SMTPReadMessage (pmx, pmessage, stream, ret, 1);
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


int SYS_SMTPCommand (MXMessage* pmessage, MXCom* pcom, void* apfield)
{
    STRING Value = (STRING)MXGetValue (pmessage, "Value", 1);
    STRING Par   = (STRING)MXGetValue (pmessage, "Par", 1);

    return 1;
}


int SYS_SMTPCommand_Reply (MXMessage* pmessage, MXCom* pcom, void* apfield)
{
    LONG   Identity = *(LONG*)MXGetValue (pmessage, "Identity", 1);
    STRING Value  =   (STRING)MXGetValue (pmessage, "Value", 1);
    STRING Par    =   (STRING)MXGetValue (pmessage, "Par", 1);
    LONG   Code   = *(LONG*)MXGetValue (pmessage, "Code", 1);
    STRING Reply =    (STRING)MXGetValue (pmessage, "Reply", 1);
    return 1;
}



BOOL SMTPCheckResponse(TPClient* pclient, int Type)
{
    char* buf;
    char temp[4];
    int temp2;

    if (TPRead(pclient->TP, pclient) < 0) return FALSE;
    buf = pclient->RequestBuffer;
    buf[pclient->RequestSize] = 0;

    strncpy(temp, buf, 3);
    temp[3] = 0;

    temp2 = atoi(temp);

    switch (Type)
    {
    case CONNECTION_CHECK:
        if (temp2 != 220)
        {
            sprintf (m_ErrorMessage, "%s", buf);
            return FALSE;
        }
        break;
    case HELLO_CHECK:
        if (temp2 != 250)
        {
            sprintf (m_ErrorMessage, "%s", buf);
            return FALSE;
        }
        break;
    case MAIL_CHECK:
        if (temp2 != 250)
        {
            sprintf (m_ErrorMessage, "%s", buf);
            return FALSE;
        }
        break;
    case RCPT_CHECK:
        if (temp2 != 250)
        {
            sprintf (m_ErrorMessage, "%s", buf);
            return FALSE;
        }
        break;
    case DATA_START_CHECK:
        if (temp2 != 354)
        {
            sprintf (m_ErrorMessage, "%s", buf);
            return FALSE;
        }
        break;
    case DATA_END_CHECK:
        if (temp2 != 250)
        {
            sprintf (m_ErrorMessage, "%s", buf);
            return FALSE;
        }
        break;
    case QUIT_CHECK:
        if (temp2 != 221)
        {
            sprintf (m_ErrorMessage, "%s", buf);
            return FALSE;
        }
        break;
    case DATA_CHECK:
        if (temp2 != 354)
        {
            sprintf (m_ErrorMessage, "%s", buf);
            return FALSE;
        }
        break;
    }
    return TRUE;
}



TPClient* SMTPConnect(TP* ptp, char* ToHost)
{
    TPClient* pclient;
    pclient = TPEstablishConnectionWith (ptp, ToHost, 25, IPPROTO_TCP, NULL, NULL, TRUE);
    if (!pclient)
    {
        sprintf (m_ErrorMessage, "%s",  "Server cannot be connected");
        return NULL;
    }
    else
    {
        TPSetClientProtocol (ptp, pclient, IPPROTO_TCP);
        if(SMTPCheckResponse(pclient, CONNECTION_CHECK)==FALSE)
        {
            TPCloseDownClient (ptp, pclient);
            return NULL;
        }
    }
    return pclient;
}


BOOL SMTPHelo (TPClient* pclient)
{
    char buf[256];
    char local_host[200];

    gethostname( local_host, 200 );
    sprintf (buf, "HELO %s\r\n", local_host);
    TPWrite (pclient->TP, pclient, buf, (int)strlen (buf));
    if (SMTPCheckResponse(pclient, HELLO_CHECK)==FALSE)
    {
        TPCloseDownClient (pclient->TP, pclient);
        return FALSE;
    }
    return TRUE;
}

BOOL SMTPQuit(TPClient* pclient)
{
    char buf[256];

    sprintf (buf, "QUIT \r\n");
    TPWrite(pclient->TP, pclient, buf, (int)strlen (buf));
    if (SMTPCheckResponse(pclient, QUIT_CHECK)==FALSE)
    {
        TPCloseDownClient (pclient->TP, pclient);
        return FALSE;
    }
    else
        return TRUE;
}


BOOL SMTPMail(TPClient* pclient, char* from)
{
    char buf[256];

    sprintf (buf, "MAIL From:<%s>\r\n", from);
    TPWrite(pclient->TP, pclient, buf, (int)strlen (buf));
    if (SMTPCheckResponse(pclient, MAIL_CHECK)==FALSE)
    {
        TPCloseDownClient (pclient->TP, pclient);
        return FALSE;
    }
    else
        return TRUE;
}



BOOL SMTPSetTo(TPClient* pclient, char* to)
{
    char buf[256];
    sprintf (buf, "RCPT TO:<%s>\r\n", to);
    TPWrite(pclient->TP, pclient, buf, (int)strlen (buf));
    if (SMTPCheckResponse(pclient, RCPT_CHECK)==FALSE)
    {
        TPCloseDownClient (pclient->TP, pclient);
        return FALSE;
    }
    else
        return TRUE;

}


BOOL SMTPData(TPClient* pclient, char* Header, char* Body)
{
    char buf[512];

    sprintf (buf, "DATA\r\n");

    TPWrite(pclient->TP, pclient, buf, (int)strlen (buf));
    if (SMTPCheckResponse(pclient, DATA_CHECK)==FALSE)
    {
        TPCloseDownClient (pclient->TP, pclient);
        return FALSE;
    }
    else
    {

        TPWrite (pclient->TP, pclient, Header, (int)strlen (Header));
        /*sTemp = cook_body( msg );*/

        TPWrite (pclient->TP, pclient, Body, (int)strlen (Body));

        sprintf (buf, "\r\n.\r\n");        /* end message */
        TPWrite (pclient->TP, pclient,buf, (int)strlen (buf));

        if (SMTPCheckResponse(pclient, DATA_END_CHECK)==FALSE)
        {
            TPCloseDownClient (pclient->TP, pclient);
            return FALSE;
        }
    }

    return TRUE;
}

/*
 This is a little inefficient because it beings a search
 at the beginning of the string each time. This was
 the only thing I could think of that handled ALL variations.
 In particular, the sequence "\r\n.\r\n.\r\n" is troublesome. 
 (Even CStringEx's FindReplace wouldn't handle that situation
 with the global flag set.)
*/

/*

CString SMTPCookBody(CMailMessage * msg)
{
  ASSERT( msg != NULL );
  CString sTemp;
  CString sCooked = _T( "" );
  LPTSTR szBad = _T( "\r\n.\r\n" );
  LPTSTR szGood = _T( "\r\n..\r\n" );
  int nPos;
  int nStart = 0;
  int nBadLength = strlen( szBad );
  sTemp = msg->m_sBody;
  if( sTemp.Left( 3 ) == _T( ".\r\n" ) )
      sTemp = _T( "." ) + sTemp;
  while( (nPos = sTemp.Find( szBad )) > -1 )
  {
      sCooked = sTemp.Mid( nStart, nPos );
      sCooked += szGood;
      sTemp = sCooked + sTemp.Right( sTemp.GetLength() - (nPos + nBadLength) );
  }
  return sTemp;
}

*/


/* =====================================================================================*/
/* END SMTP Functions ==================================================================*/
/* =====================================================================================*/

