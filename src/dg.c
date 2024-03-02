/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "dg.h"


void DGSendClassConnection (MXCom* pcom, STRING Name, WORD Code, int Port, STRING TableName, BYTE FromTo)
{
    MXMessage* pmessage;
    WORD wPort = (WORD)Port;
    pmessage = MXPutMessage (pcom, DG_SYS, "SendClassConnection");
    MXSetValue(pmessage, "Name",  1, Name) ;
    MXSetValue(pmessage, "Code",  1, &Code) ;
    MXSetValue(pmessage, "Port",   1, &wPort) ;
    MXSetValue(pmessage, "TableName", 1, TableName);
    MXSetValue(pmessage, "FromTo", 1, &FromTo);
    DGSend (pcom->MX, pcom, pmessage);
}

void DGReplyClassConnection (MXCom* pcom, STRING Name, LONG Error)
{
    MXMessage* pmessage;
    pmessage = MXPutMessage (pcom, DG_SYS, "ReplyClassConnection");
    MXSetValue(pmessage, "Name",  1, Name) ;
    MXSetValue(pmessage, "Error",   1, &Error) ;
    DGSend (pcom->MX, pcom, pmessage);
}

void DGPutAck (MXCom* pcom, LONG Identity, DWORD Date, DWORD Hour, LONG Error)
{
    MXMessage* pmessage;
    pmessage = MXPutMessage (pcom, DG_SYS, "AckMessage");
    MXSetValue(pmessage, "Date",     1, &Date) ;
    MXSetValue(pmessage, "Hour",  1, &Hour) ;
    MXSetValue(pmessage, "Identity",   1, &Identity) ;
    MXSetValue(pmessage, "Error",   1, &Error) ;
}

MXMessage* DGReturnMessageFromStream (MX* pmx, char* stream)
{

    WORD RequestSize;
    DWORD MessageSize;
    MXMessageHeader header;
    MXMessageStatut statut;
    MXMessage* pmessage;

    RequestSize = TPGetRequestLength (stream);      /* TP dependent */

    /* check if request size contains at least the message header */

    if (RequestSize < (MXMessageHeaderSize + MXMessageStatutSize + sizeof (WORD)))
    {
        OSError ("TEDAG020", " (%d)", RequestSize);
        return NULL;
    }

    /* pheader points to the machine independant structure */

    DGReadMessageHeader(pmx, stream, &header);

    MessageSize = header.Size;

    pmessage =  MXCreateMessageFromCodes (pmx,  header.ClassCode,  header.MessageCode);
    if (!pmessage)
        return NULL;
    else
    {
        MXReadMessageStatut (pmx, stream, &statut);
        pmessage->Size = MessageSize;
        pmessage->Part = header.Part;
        pmessage->Identity = header.Identity;
        pmessage->Type  = statut.Type;
        pmessage->ADate = statut.ADate;
        pmessage->AHour = statut.AHour;
        pmessage->TDate = statut.TDate;
        pmessage->THour = statut.THour;
        pmessage->PDate = statut.PDate;
        pmessage->PHour = statut.PHour;
        pmessage->DDate = statut.DDate;
        pmessage->DHour = statut.DHour;

    }
    return pmessage;

}



MXMessage* DGCheckAndReturnMessage (MX* pmx, MXCom* pcom, char* stream)
{

    WORD RequestSize;
    DWORD MessageSize;
    MXMessageHeader header;
    MXMessageStatut statut;
    MXMessage* pmessage;

    RequestSize = TPGetRequestLength (stream);      /* TP dependent */

    /* check if request size contains at least the message header */

    if (RequestSize < (MXMessageHeaderSize + sizeof (WORD)))
    {
        OSError ("TEDAG020", " (%d)", RequestSize);
        return NULL;
    }

    /* pheader points to the machine independant structure */

    DGReadMessageHeader(pmx, stream, &header);
    MessageSize = header.Size;

    /* check if message structure exists with same identity */

    if (pcom->InputMessages)
    {

        pmessage = MXFindMessage (pcom->InputMessages, header.Identity);
        if (pmessage)
        {
            pmessage->Part = header.Part;
            return pmessage;
        }
    }
    else
        if (header.Part != 1)
            return NULL;                          /*Ignore message I remove it
                   because i can't open file 
                   so the other parts have no sense
                   that stopped with error */

    pmessage =  MXCreateMessageFromCodes (pmx,  header.ClassCode,  header.MessageCode);
    /* check messages with no parameters */
    if (!pmessage)
        return NULL;
    else
    {
        MXReadMessageStatut (pmx, stream, &statut);
        pmessage->Size = MessageSize;
        pmessage->Part = header.Part;
        pmessage->Identity = header.Identity;
        pmessage->Type  = statut.Type;
        pmessage->TDate = statut.TDate;
        pmessage->THour = statut.THour;
        pmessage->PDate = statut.PDate;
        pmessage->PHour = statut.PHour;
        pmessage->DDate = statut.DDate;
        pmessage->DHour = statut.DHour;
        ListNewr (&pcom->InputMessages, pmessage);
    }
    return pmessage;
}




LONG DGReceiveMessage (TP* ptp, TPClient* pclient, char* buf, int count)
{
    char* stream;
    MX* pmx;
    MXMessage* pmessage;
    LONG ret;
    List* plist;
    MXCallBack* pexecutecomprocedure;
    MXCallBack* pexecuteprocedure;


    MXCom* pcom = (MXCom*)pclient->ApplicationField;
    pmx = pcom->MX;


    if ((!pcom) || (!pcom->Client) || (!pclient->RequestBuffer))
        return -1;


    stream = buf;
    pmessage = DGCheckAndReturnMessage (pmx, pcom, stream);
    if (!pmessage)
        return -1;

    pmessage->Com = pcom;

    if (pcom->TraceProcedure)  pcom->TraceProcedure (pmessage, MXONRECV, buf, count);


    ret = DGReadMessage (pmx, pmessage, stream, count, (BYTE)((pcom->Protocol == IOPROTO_STORE) ? 0 : 1));

    if (ret < 0)
    {
        pmessage->Complete = 1;     /*finish with this message */
    }

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


LONG DGSendMessage (TP* ptp, TPClient* pclient, char* buf, int size)
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

    ret = DGWriteMessage (pmx, pmessage, buf, size - 2, (BYTE)((pcom->Protocol == IOPROTO_STORE) ? 0 : 1)); /*client->TransSize - tp header;*/

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

/* blocking mode */


/* blocking mode */

MXMessage* DGRecv (MX* pmx, MXCom* pcom)
{
    LONG ret;
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
        while (((ret  = pcom->Client->ReadFunction (pmx->TP, pcom->Client)) == 0) &&
                (pcom->Protocol != IOPROTO_STORE));
        if (ret < 0)
        {
            if (!pcom->Client->ClientGone) MXCloseCom (pmx, pcom);
            if (pmessage) MXFreeMessage (pcom->MX, pmessage);
            return NULL;
        }
        if (ret == 0) return pmessage;

        stream = pcom->Client->RequestBuffer;

        pmessage = DGCheckAndReturnMessage (pmx, pcom, stream);
        if (!pmessage)
        {
            TPSetBlockingMode (pcom->Client, FALSE);
            return NULL;
        }
        pmessage->Com = pcom;
        if (pcom->TraceProcedure) pcom->TraceProcedure (pmessage, MXONRECV, stream, ret);
        ret = DGReadMessage (pmx, pmessage, stream, ret, (BYTE)((pcom->Protocol == IOPROTO_STORE) ? 0 : 1));
        if (ret < 0)
        {
            TPSetBlockingMode (pcom->Client, FALSE);
            ListRemove(&pcom->InputMessages, pmessage);
            MXFreeMessage(pcom->MX, pmessage);
            return NULL;
        }
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
    if (!pcom->Closed) TPSetBlockingMode (pcom->Client, FALSE);
    return pmessage;
}




LONG DGSend (MX* pmx, MXCom* pcom, MXMessage* pmessage)
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
        ret = DGWriteMessage (pmx, pmessage, pcom->Client->TransBuffer, pcom->Client->TransSize - 2, (BYTE)((pcom->Protocol == IOPROTO_STORE) ? 0 : 1));

        if (ret < 0)
        {
            TPSetBlockingMode (pcom->Client, FALSE);
            /*!!!!!! A REVOIR LA LIBERATION DES MESSAGES EN BLOQUANT */
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
            if (pmessage->Type & MXACK)
                ListNewr (&pcom->AckMessages, pmessage);
            break;
        }
    }
    TPSetBlockingMode (pcom->Client, FALSE);

    return ret1;
}



int DGReadMessageHeader (MX* pmx, char* stream, MXMessageHeader* pheader)
{

    LONG from = 0;
    XF* pxf = pmx->TP->XF;
    char* fromstream = stream + sizeof (WORD);     /*skip size of request */

    fromstream += XFWordRead (pxf, (char*)(&pheader->Identity), &from, fromstream, 1);
    from = 0;
    fromstream += XFDWordRead (pxf, (char*)(&pheader->Size), &from, fromstream, 1);
    from = 0;
    fromstream += XFWordRead (pxf, (char*)(&pheader->ClassCode), &from, fromstream, 1);
    from = 0;
    fromstream += XFWordRead (pxf, (char*)(&pheader->MessageCode), &from, fromstream, 1);
    from = 0;
    fromstream += XFWordRead (pxf, (char*)(&pheader->Part), &from, fromstream, 1);
    return MXMessageHeaderSize;
}

int DGWriteMessageHeader (MX* pmx, MXMessage* pmessage, char* stream, BOOL FirstTime)
{
    LONG from = 0;
    XF* pxf = pmx->TP->XF;
    MXDialogClass* pclass = pmessage->Class;
    MXMessageClass* pclassmess = pmessage->MessageClass;
    char* pheader = stream;


    pheader += XFWordWrite (pxf, (char*)(&pmessage->Identity) , &from, (char*) pheader , 1);
    from = 0;
    pheader += XFDWordWrite (pxf, (char*)(&pmessage->Size) , &from, (char*) pheader , 1);
    from = 0;
    pheader += XFWordWrite (pxf, (char*)(&pclass->Code) , &from, (char*) pheader , 1);
    from = 0;
    pheader += XFWordWrite (pxf, (char*)(&pclassmess->Code), &from, (char*)pheader , 1);
    from = 0;
    pheader += XFWordWrite (pxf, (char*)(&pmessage->Part), &from, (char*)pheader , 1);
    if (FirstTime)
    {
        /*Dates & hours */
        from = 0;
        pheader += XFByteWrite (pxf, (char*)(&pmessage->Type), &from, (char*)pheader , 1);
        from = 0;
        pheader += XFDWordWrite (pxf, (char*)(&pmessage->TDate), &from, (char*)pheader , 1);
        from = 0;
        pheader += XFDWordWrite (pxf, (char*)(&pmessage->THour), &from, (char*)pheader , 1);
        from = 0;
        pheader += XFDWordWrite (pxf, (char*)(&pmessage->PDate), &from, (char*)pheader , 1);
        from = 0;
        pheader += XFDWordWrite (pxf, (char*)(&pmessage->PHour), &from, (char*)pheader , 1);
        from = 0;
        pheader += XFDWordWrite (pxf, (char*)(&pmessage->DDate), &from, (char*)pheader , 1);
        from = 0;
        pheader += XFDWordWrite (pxf, (char*)(&pmessage->DHour), &from, (char*)pheader , 1);
    }
    if (FirstTime)
        return MXMessageHeaderSize + MXMessageStatutSize;
    else
        return MXMessageHeaderSize;

}


/*--------------------------------------------------------------------*/
/* Functions related to buffer */
/*--------------------------------------------------------------------*/




LONG DGReadMessage (MX* pmx, MXMessage* pmessage, char* stream, int streamsize, BYTE WithFileContent)
{
    MXObject* pobject;
    BOOL FirstTime = FALSE;
    WORD MessageHeaderSize = MXMessageHeaderSize;
    LONG from = 0;
    BYTE type;
    LONG size;
    LONG isread = 0;          /* what is read in partmessage */
    int FromObjectOffset;
    int FromSizeOffset;
    int NbObjects;
    XF* pxf;
    char* Stream;
    char* fromstream;
    char* fileattributes;
    char* bufferattributes;
    WORD RequestSize;
    WORD PartMessageSize;

    MXMessageClass* pclassmess = pmessage->MessageClass;
    MXMessageContext* pcontext = pmessage->Context;

    int State = 0;
    int j, i;
    MXCom* pcom = pmessage->Com;

    if ((pcom) && (pcom->ComClass) && (pcom->XF))
        pxf = pcom->XF;
    else pxf = pmx->TP->XF;

    RequestSize = TPGetRequestLength (stream);      /* TP dependent */


    /* if no context related to this message this means the first time we go */
    /* in this message with parameters                                       */

    if (!pcontext)
    {
        FirstTime = TRUE;
        MessageHeaderSize += MXMessageStatutSize;
        pcontext = MXAllocateContext ();
        if (!pcontext)
        {
            return -1;
        }
        pmessage->Context = pcontext;
        pcontext->StillToRead = pmessage->Size;
    }

    PartMessageSize = RequestSize - (MessageHeaderSize + sizeof (WORD));

    /* now compare StillToRead with request content */


    fromstream = stream + MessageHeaderSize + sizeof (WORD);

    if (pcontext->StillToRead < PartMessageSize)  /* this means something wrong in message */
    {
        /* Try to rectify the request */
        PartMessageSize = (WORD)pcontext->StillToRead;    /* we force steping in state 1 */
    }

    from = 0;
    FromObjectOffset = pcontext->ObjectOffset;
    FromSizeOffset = pcontext->SizeOffset;
    NbObjects = ListNbElt (pclassmess->Objects);

    for (i = FromObjectOffset; i < NbObjects; i++)
    {
        pobject = (MXObject *)ListGetElt (pclassmess->Objects, i);
        type = pobject->Type;
        switch (type) {
        case MXSHORT :
            if (pmessage->StreamSize < pmessage->Offset + from + pobject->Size * sizeof (SHORT))
                if (MXExtendValueBuffer (pmessage, MXBUFVALUESIZE) == 0) return -1;
            Stream = pmessage->Stream + pmessage->Offset;
            for (j = FromSizeOffset; j < pobject->Size; j++)
            {
                pmessage->Values[(int)pobject->Offset + j] = pmessage->Offset + from;
                size = XFShortRead (pxf, Stream, &from, fromstream, 1);
                fromstream += size;
                isread += size;
                pcontext->StillToRead -= size;
                if (isread == PartMessageSize )
                {
                    State = 1;
                    break;
                }
            }
            break;
        case MXWORD :
            if (pmessage->StreamSize < pmessage->Offset + from + pobject->Size * sizeof (WORD))
                if (MXExtendValueBuffer (pmessage, MXBUFVALUESIZE) == 0) return -1;
            Stream = pmessage->Stream + pmessage->Offset;

            for (j = FromSizeOffset; j < pobject->Size; j++)
            {
                pmessage->Values[(int)pobject->Offset + j] = pmessage->Offset + from;
                size = XFWordRead (pxf, Stream, &from, fromstream, 1);
                fromstream += size;
                isread += size;
                pcontext->StillToRead -= size;
                if (isread == PartMessageSize )
                {
                    State = 1;
                    break;
                }
            }
            break;
        case MXLONG :
            if (pmessage->StreamSize < pmessage->Offset + from + pobject->Size * sizeof (LONG))
                if (MXExtendValueBuffer (pmessage, MXBUFVALUESIZE) == 0) return -1;
            Stream = pmessage->Stream + pmessage->Offset;
            for (j = FromSizeOffset; j < pobject->Size; j++)
            {

                pmessage->Values[(int)pobject->Offset + j] = (WORD)(pmessage->Offset + from);
                size = XFLongRead (pxf, Stream, &from, fromstream, 1);
                fromstream += size;
                isread += size;
                pcontext->StillToRead -= size;
                if (isread == PartMessageSize )
                {
                    State = 1;
                    break;
                }
            }
            break;
        case MXDWORD :
            if (pmessage->StreamSize < pmessage->Offset + from + pobject->Size * sizeof (DWORD))
                if (MXExtendValueBuffer (pmessage, MXBUFVALUESIZE) == 0) return -1;
            Stream = pmessage->Stream + pmessage->Offset;
            for (j = FromSizeOffset; j < pobject->Size; j++)
            {

                pmessage->Values[(int)pobject->Offset + j] = pmessage->Offset + from;
                size = XFDWordRead (pxf, Stream, &from, fromstream, 1);
                fromstream += size;
                isread += size;
                pcontext->StillToRead -= size;
                if (isread == PartMessageSize )
                {
                    State = 1;
                    break;
                }
            }
            break;
        case MXDOUBLE :
            if (pmessage->StreamSize < pmessage->Offset + from + pobject->Size * sizeof (DOUBLE))
                if (MXExtendValueBuffer (pmessage, MXBUFVALUESIZE) == 0) return -1;
            Stream = pmessage->Stream + pmessage->Offset;
            for (j = FromSizeOffset; j < pobject->Size; j++)
            {

                pmessage->Values[(int)pobject->Offset + j] = pmessage->Offset + from;
                size = XFDoubleRead (pxf, Stream, &from, fromstream, 1);
                fromstream += size;
                isread += size;
                pcontext->StillToRead -= size;
                if (isread == PartMessageSize )
                {
                    State = 1;
                    break;
                }
            }
            break;

        case MXBYTE :
            if (pmessage->StreamSize < pmessage->Offset + from + pobject->Size)
                if (MXExtendValueBuffer (pmessage, MXBUFVALUESIZE) == 0) return -1;
            Stream = pmessage->Stream + pmessage->Offset;
            for (j = FromSizeOffset; j < pobject->Size; j++)
            {

                pmessage->Values[(int)pobject->Offset + j] = (WORD)(pmessage->Offset + from);
                size = XFByteRead (pxf, Stream, &from, fromstream, 1);
                fromstream += size;
                isread += size;
                pcontext->StillToRead -= size;
                if (isread == PartMessageSize )
                {
                    State = 1;
                    break;
                }
            }
            break;
        case MXCHAR :
            if (pmessage->StreamSize < pmessage->Offset + from + pobject->Size)
                if (MXExtendValueBuffer (pmessage, MXBUFVALUESIZE) == 0) return -1;
            Stream = pmessage->Stream + pmessage->Offset;
            pmessage->Values[(int)pobject->Offset] = pmessage->Offset + from;
            size = XFCharRead (pxf, Stream, &from, fromstream, pobject->Size);
            fromstream += size;
            isread += size;
            pcontext->StillToRead -= size;
            if (isread == PartMessageSize )
            {
                State = 1;
                break;
            }
            break;
        case MXFILE :
            for (j = FromSizeOffset; j < pobject->Size; j++)
            {
                if (!WithFileContent || (!MXFileProcessing (pmessage)))
                {
                    int FileStructSize = (2 * FILENAMESIZE) + (3 * sizeof (CHAR)) + sizeof (DWORD) + sizeof (LONG);
                    /* first time we goes in file */
                    if (pmessage->StreamSize < pmessage->Offset + from + FileStructSize)
                        if (MXExtendValueBuffer (pmessage, MXBUFVALUESIZE) == 0) return -1;
                    Stream = pmessage->Stream + pmessage->Offset;

                    pmessage->Values[(int)pobject->Offset + j] = pmessage->Offset + from;
                    size = XFFileRead (pxf, Stream, &from, fromstream, 1);
                    fromstream += size;
                    isread += size;
                    pcontext->StillToRead -= size;
                    if (isread == PartMessageSize )
                    {
                        State = 1;
                        break;
                    }
                }
                if (WithFileContent)
                {
                    fileattributes = (char*) (pmessage->Stream +
                                              pmessage->Values[(int)pobject->Offset + j]);
                    size = MXReadFileContent (pmx, pcontext, fromstream,  fileattributes, (WORD)(PartMessageSize - isread));
                    if (size < 0) /* something strange */
                    {
                        /*We should stop reading message */
                        return -1;
                    }
                    fromstream += size;
                    isread += size;
                    pcontext->StillToRead -= size;
                    if (isread == PartMessageSize)
                    {
                        State = 1;
                        break;
                    }
                }
            }
            break;
        case MXBUFFER :
            for (j = FromSizeOffset; j < pobject->Size; j++)
            {
                if (!MXBufferProcessing (pmessage))
                {
                    DWORD BufSize;
                    /* first time we goes in buffer */
                    if (pmessage->StreamSize < pmessage->Offset + from + sizeof (DWORD) + sizeof (CHAR))
                        if (MXExtendValueBuffer (pmessage, MXBUFVALUESIZE) == 0) return -1;
                    Stream = pmessage->Stream + pmessage->Offset;

                    pmessage->Values[(int)pobject->Offset + j] = pmessage->Offset + from;
                    size = XFDWordRead (pxf, Stream, &from, fromstream, 1);
                    fromstream += size;
                    isread += size;
                    pcontext->StillToRead -= size;
                    size = XFCharRead (pxf, Stream, &from, fromstream, 1);
                    fromstream += size;
                    isread += size;
                    pcontext->StillToRead -= size;

                    OSCopy4 (&BufSize, Stream + from - 5);
                    size = BufSize;
                    if (pmessage->StreamSize < pmessage->Offset + from + size)
                        if (MXExtendValueBuffer (pmessage, size) == 0) return -1;
                    Stream = pmessage->Stream + pmessage->Offset;
                    if (isread == PartMessageSize )
                    {
                        State = 1;
                        break;
                    }
                }


                bufferattributes = (char*) (pmessage->Stream +
                                            pmessage->Values[(int)pobject->Offset + j]);
                size = MXReadBufferContent (pmx, pcontext, fromstream,  bufferattributes, PartMessageSize - isread);
                if (size < 0) /* something strange */
                {
                    /*We should stop reading message */
                    return -1;
                }
                fromstream += size;
                isread += size;
                from += size;
                pcontext->StillToRead -= size;
                if (isread == PartMessageSize)
                {
                    State = 1;
                    break;
                }
            }
            break;
        case MXSTRING :
            for (j = FromSizeOffset; j < pobject->Size; j++)
            {
                if (pmessage->StreamSize < pmessage->Offset + from + strlen (fromstream))
                    if (MXExtendValueBuffer (pmessage, MXBUFVALUESIZE) == 0) return -1;
                Stream = pmessage->Stream + pmessage->Offset;
                pmessage->Values[(int)pobject->Offset + j] = pmessage->Offset + from;
                size = XFStringRead (pxf, Stream, &from, fromstream, 1);
                fromstream += size;
                isread += size;
                pcontext->StillToRead -= size;
                if (isread == PartMessageSize )
                {
                    State = 1;
                    break;
                }
            }
            break;
        default:
            OSError ("TEDAG032",  " (%s)", pmessage->MessageClass->Name);
            return -1;
        }
        if (State == 1)    /* here we got a break partmessage is read  */
        {
            if (pcontext->StillToRead == 0)            /*everything is read */
            {
                OSDateHour (&pmessage->ADate, &pmessage->AHour);
                pmessage->Complete = 1;
                /*!!!!!!!!!!!SEND ACK */
                if (pmessage->Type & MXACK)
                    DGPutAck (pmessage->Com, (DWORD)pmessage->Identity, pmessage->ADate, pmessage->AHour, 0);
            }
            if (PartMessageSize != (fromstream - (stream + MessageHeaderSize + sizeof (WORD))))
            {

                OSError ("TEDAG033", " (%s)", pmessage->MessageClass->Name);
                return -1;
            }
            if ((!MXFileProcessing (pmessage))  && (!MXBufferProcessing (pmessage)))
                if (j == (pobject->Size - 1))
                {
                    pcontext->SizeOffset = 0;
                    pcontext->ObjectOffset = i + 1;
                }
                else pcontext->SizeOffset = j + 1;
            else
            {
                pcontext->SizeOffset = j;
                pcontext->ObjectOffset = i;
            }
            break;               /* we should go out */
        }
    }
    if (State != 1)   /* we reach the end but PartMessageSize is not equal to what is read */
    {
        /* this is possible if message doesn't have attributes  so check */

        if (!(pmessage->Size == 0))
            OSError ("TEDAG033",  " (%s)", pmessage->MessageClass->Name);
        pmessage->Complete = 1;
    }  /* we sent it anyhow */
    pmessage->Offset += (WORD)from;
    MXInitMessage (pmessage);
    return (LONG)(fromstream - stream);
}




/*--------------------------------------------------------------------*/
/* here message structure exists we know its size & the request size  */
/* if the message has not a context it means this is the first time   */
/* we goes in : create a context read the request size and the        */
/* size. We have at least the request size in stream resolved by TP   */
/* A request contains at least the Header Message Size                */
/* A request can not contain two messages....                         */
/*--------------------------------------------------------------------*/


LONG DGWriteMessage (MX* pmx, MXMessage* pmessage, char* stream, int maxcount, BYTE WithFileContent)
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
    char* bufferattributes;


    MXMessageClass* pclassmess = pmessage->MessageClass;
    MXMessageContext* pcontext = pmessage->Context;
    int State = 0;
    int j, i;
    MXCom* pcom = pmessage->Com;

    if ((pcom) && (pcom->ComClass) && (pcom->XF))
        pxf = pcom->XF;
    else pxf = pmx->TP->XF;


    /* check if buf size contains at least the message header */

    if (maxcount < (MXMessageHeaderSize))
    {
        OSError ("TEDAG034",  " (%s)", pclassmess->Name);
        return -1;
    }

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

    /* pheader points to the machine independant structure. At this point*/
    /* all pmessage attributes are good to be send */

    written = DGWriteMessageHeader(pmx, pmessage, stream, FirstTime);
    tostream = stream + written;

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
                if ((written + (int)sizeof (SHORT)) <= maxcount)
                {
                    size = XFShortWrite (pxf, (char*) (pmessage->Stream +
                                                       pmessage->Values[(int)pobject->Offset + j]), &from, tostream, 1);
                    tostream += size;
                    pcontext->StillToRead -= size;
                    written += size;
                }
                else
                {
                    State = 1;
                    break;
                }
            }
            break;
        case MXWORD :
            for (j = FromSizeOffset; j < pobject->Size; j++, from = 0)
            {
                if ((written + (int)sizeof (WORD)) <= maxcount)
                {
                    size = XFWordWrite (pxf, (char*) (pmessage->Stream +
                                                      pmessage->Values[(int)pobject->Offset + j]), &from, tostream, 1);
                    tostream += size;
                    pcontext->StillToRead -= size;
                    written += size;
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
                if ((written + (int)sizeof (LONG)) <= maxcount)
                {
                    size = XFLongWrite (pxf, (char*) (pmessage->Stream +
                                                      pmessage->Values[(int)pobject->Offset + j]), &from, tostream, 1);
                    tostream += size;
                    pcontext->StillToRead -= size;
                    written += size;
                }
                else
                {
                    State = 1;
                    break;
                }
            }
            break;
        case MXDWORD :
            for (j = FromSizeOffset; j < pobject->Size; j++, from = 0)
            {
                if ((written + (int)sizeof (DWORD)) <= maxcount)
                {
                    size = XFDWordWrite (pxf, (char*) (pmessage->Stream +
                                                       pmessage->Values[(int)pobject->Offset + j]), &from, tostream, 1);
                    tostream += size;
                    pcontext->StillToRead -= size;
                    written += size;
                }
                else
                {
                    State = 1;
                    break;
                }
            }
            break;
        case MXDOUBLE :
            for (j = FromSizeOffset; j < pobject->Size; j++, from = 0)
            {
                if ((written + (int)sizeof (DOUBLE)) <= maxcount)
                {
                    size = XFDoubleWrite (pxf, (char*) (pmessage->Stream +
                                                        pmessage->Values[(int)pobject->Offset + j]), &from, tostream, 1);
                    tostream += size;
                    pcontext->StillToRead -= size;
                    written += size;
                }
                else
                {
                    State = 1;
                    break;
                }
            }
            break;
        case MXBYTE :
            for (j = FromSizeOffset; j < pobject->Size; j++, from = 0)
            {
                if ((written + (int)sizeof (BYTE)) <= maxcount)
                {
                    size = XFByteWrite (pxf, (char*) (pmessage->Stream +
                                                      pmessage->Values[(int)pobject->Offset + j]), &from, tostream, 1);
                    tostream += size;
                    pcontext->StillToRead -= size;
                    written += size;
                }
                else
                {
                    State = 1;
                    break;
                }
            }
            break;
        case MXCHAR :
            if ((written + (pobject->Size * (int)sizeof (BYTE))) <= maxcount)
            {
                size = XFCharWrite (pxf, (char*) (pmessage->Stream +
                                                  pmessage->Values[(int)pobject->Offset]), &from, tostream, pobject->Size);
                tostream += size;
                pcontext->StillToRead -= size;
                written += size;
            }
            else
            {
                State = 1;
                break;
            }
            break;
        case MXBUFFER :
            for (j = FromSizeOffset; j < pobject->Size; j++, from = 0)
            {
                if (!MXBufferProcessing (pmessage))
                {

                    /* first time we goes in file */
                    if (written + (int)sizeof (DWORD) + (int)sizeof (CHAR) <= maxcount)
                    {
                        size = XFDWordWrite (pxf, (char*) (pmessage->Stream +
                                                           pmessage->Values[(int)pobject->Offset + j]), &from, tostream, 1);
                        tostream += size;
                        pcontext->StillToRead -= size;
                        written += size;
                        size = XFCharWrite (pxf, (char*) (pmessage->Stream +
                                                          pmessage->Values[(int)pobject->Offset + j]), &from, tostream, 1);
                        tostream += size;
                        pcontext->StillToRead -= size;
                        written += size;
                    }
                    else
                    {
                        State = 1;
                        break;
                    }
                }
                bufferattributes = (char*) (pmessage->Stream + pmessage->Values[(int)pobject->Offset + j]);
                size = MXWriteBufferContent (pmx, pcontext, tostream,  bufferattributes, maxcount - written);
                if (size < 0) /* something strange */
                {
                    /*We should stop the message */
                    return -1;
                }
                /* if I am here it means I can't write anymore  or buffer is read completely*/
                tostream += size;
                pcontext->StillToRead -= size;
                written += size;
                if (MXBufferProcessing(pmessage))    /* still to read in buffer */
                {
                    State = 1;
                    break;
                }
            }
            break;
        case MXFILE :
            for (j = FromSizeOffset; j < pobject->Size; j++, from = 0)
            {
                if (!WithFileContent || (!MXFileProcessing (pmessage)))
                {

                    /* first time we goes in file */
                    if ((written + MXGetFileSize ((char*) (pmessage->Stream +
                                                           pmessage->Values[(int)pobject->Offset + j]), 0)) <= maxcount)
                    {
                        size = XFFileWrite (pxf, (char*) (pmessage->Stream +
                                                          pmessage->Values[(int)pobject->Offset + j]), &from, tostream, 1);
                        tostream += size;
                        pcontext->StillToRead -= size;
                        written += size;
                    }
                    else
                    {
                        State = 1;
                        break;
                    }
                }
                if (WithFileContent)
                {
                    fileattributes = (char*) (pmessage->Stream + pmessage->Values[(int)pobject->Offset + j]);
                    size = MXWriteFileContent (pmx, pcontext, tostream,  fileattributes, (WORD)(maxcount - written));
                    if (size < 0) /* something strange */
                    {
                        /*We should stop the message */
                        return -1;
                    }
                    /* if I am here it means I can't write anymore  or file is read completely*/
                    tostream += size;
                    pcontext->StillToRead -= size;
                    written += size;
                    if (MXFileProcessing(pmessage))    /* still to read in file */
                    {
                        State = 1;
                        break;
                    }
                }
            }
            break;
        case MXSTRING :
            for (j = FromSizeOffset; j < pobject->Size; j++, from = 0)
            {
                if ((written + (int)strlen ((char*) (pmessage->Stream +
                                                     pmessage->Values[(int)pobject->Offset + j])) + 1) <= maxcount)
                {
                    size = XFStringWrite (pxf, (char*) (pmessage->Stream +
                                                        pmessage->Values[(int)pobject->Offset + j]), &from, tostream, 1);
                    tostream += size;
                    pcontext->StillToRead -= size;
                    written += size;
                }
                else
                {
                    State = 1;
                    break;
                }
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
        pmessage->Complete = 1;
    }
    MXInitMessage (pmessage);
    return written ;
}




/*----------------------------------------------------------------------------*/
/* un repertoire dans osio dans la structure IDENTFICH finit toujours par le  */
/* caract�re s�parateur                                                       */
/* dans ce fichier il n'y est pas toujours !!! � revoir                       */
/* DIALOGUE SYSTEME eST SUR LE CODE 30000                                     */
/* FAIRE LE REPERTOIRE SFS                                                    */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/

int DGSendFiles (MXCom* pcom, LONG Identity, char* FileNames, char* DistantRepertory,CHAR FileType, CHAR LocalFileLoc, CHAR DistantFileLoc)
{
    List* l;
    List* ListFiles = NULL;
    char LocalRepertory[120];
    char FileName[120];
    int Error = 0;
    FILEPARM File ;
    MXMessage* pmessage ;
    char Command[20];
    SHORT NbFiles = 0;
    SHORT NbFile = 0;
    strcpy (Command, "MPUT");

    File.FileOriLocal  = LocalFileLoc ;
    File.FileDestLocal = DistantFileLoc ;
    File.FileType      = FileType ;

    OSGetOnlyName (FileNames, FileName, LocalRepertory, LocalFileLoc);

    if (strcmp (LocalRepertory, "") == 0)  OSGetDir (LocalRepertory, LocalFileLoc, &Error);

    NbFiles = OSGetFilesFromWildCards (FileNames, &ListFiles, LocalFileLoc, FALSE);

    if (ListFiles)
    {
        l = ListFiles;
        while (l)
        {
            NbFile++;
            OSSetFileName (File.FileOri,  LocalRepertory,   ((FILEDATA *)l->car)->Name, LocalFileLoc);
            OSSetFileName (File.FileDest, DistantRepertory, ((FILEDATA *)l->car)->Name, DistantFileLoc);
            pmessage = MXPutMessage(pcom, DG_SYS, "SendFile") ;
            MXSetValue(pmessage, "File",     1, &File) ;
            MXSetValue(pmessage, "NbFiles",  1, &NbFiles) ;
            MXSetValue(pmessage, "NbFile",   1, &NbFile) ;
            MXSetValue(pmessage, "Identity", 1, &Identity) ;
            MXSetValue(pmessage, "Command",  1, &Command) ;

            l = l->cdr;
        }
    }
    else
    {
        pmessage = MXPutMessage(pcom, DG_SYS, "SendFile") ;
        strcpy (File.FileOri,  "\\");
        strcpy (File.FileDest, "");
        MXSetValue(pmessage, "File",     1, &File) ;
        MXSetValue(pmessage, "NbFiles",  1, &NbFiles) ;
        MXSetValue(pmessage, "NbFile",   1, &NbFile) ;
        MXSetValue(pmessage, "Identity", 1, &Identity) ;
        MXSetValue(pmessage, "Command",  1, &Command) ;
    }

    while (ListFiles)
    {
        free ((char*)ListFiles->car);
        ListRemove (&ListFiles, (char*)ListFiles->car);
    }

    return 1 ;
}



/*----------------------------------------------------------------------------*/


int DGSendFile (MXCom* pcom, LONG Identity, char* FileName, char* DestName, CHAR FileType, CHAR LocalFileLoc, CHAR DistantFileLoc)
{
    SHORT NbFiles = 1;
    SHORT NbFile  = 1;
    int Error = 0;
    FILEPARM File ;
    char LocalRepertory[120];
    char OnlyFileName[120];
    MXMessage* pmessage;

    char Command[20];
    strcpy (Command, "PUT");

    File.FileOriLocal  = LocalFileLoc ;
    File.FileDestLocal = DistantFileLoc ;
    File.FileType      = FileType ;

    OSGetOnlyName (FileName, OnlyFileName, LocalRepertory, LocalFileLoc);
    if (strcmp (LocalRepertory, "") == 0)    OSGetDir (LocalRepertory, LocalFileLoc, &Error);

    OSSetFileName (File.FileOri, LocalRepertory, OnlyFileName, LocalFileLoc);

    if (!DestName)
        sprintf (File.FileDest,  "%s", OnlyFileName);
    else
        sprintf (File.FileDest,  "%s",  DestName);

    pmessage = MXPutMessage(pcom, DG_SYS, "SendFile") ;
    pmessage->Type = MXACK;
    MXSetValue(pmessage, "File",     1, &File) ;
    MXSetValue(pmessage, "NbFiles",  1, &NbFiles) ;
    MXSetValue(pmessage, "NbFile",   1, &NbFile) ;
    MXSetValue(pmessage, "Identity", 1, &Identity) ;
    MXSetValue(pmessage, "Command",  1, &Command) ;

    return 1;
}

/*----------------------------------------------------------------------------*/


int DGGetFiles (MXCom* pcom, LONG Identity, char* FileNames, char* LocalRepertory, CHAR FileType, CHAR LocalFileLoc, CHAR DistantFileLoc)
{

    char Repertory[120];
    int Error = 0;
    char Command[20];

    MXMessage* pmessage = MXPutMessage(pcom, DG_SYS, "GetFiles") ;

    strcpy (Command, "MGET");

    MXSetValue(pmessage, "FileNames", 1, FileNames) ;
    if (!LocalRepertory)
    {
        OSGetDir (Repertory, LocalFileLoc, &Error);
        MXSetValue(pmessage, "LocalRepertory", 1,Repertory ) ;
    }
    else
        MXSetValue(pmessage, "LocalRepertory", 1, LocalRepertory) ;
    MXSetValue(pmessage, "FileType",        1, &FileType) ;
    MXSetValue(pmessage, "DistantFileLoc",  1, &DistantFileLoc) ;
    MXSetValue(pmessage, "LocalFileLoc",    1, &LocalFileLoc) ;
    MXSetValue(pmessage, "Identity",        1, &Identity) ;
    MXSetValue(pmessage, "Command",         1, &Command) ;

    return 1;
}

/*----------------------------------------------------------------------------*/

int DGGetFile (MXCom* pcom, LONG Identity, char* FileName, char* DestName, CHAR FileType, CHAR LocalFileLoc, CHAR DistantFileLoc)
{

    MXMessage* pmessage = MXPutMessage(pcom, DG_SYS, "GetFile") ;
    char Command[20];
    char OnlyFileName[120];
    char LocalRepertory[120];
    strcpy (Command, "GET");


    OSGetOnlyName (FileName, OnlyFileName, LocalRepertory, LocalFileLoc);


    MXSetValue(pmessage, "FileName", 1, FileName) ;
    if (!DestName)
        MXSetValue(pmessage, "DestName", 1, OnlyFileName) ;
    else
        MXSetValue(pmessage, "DestName", 1, DestName) ;

    MXSetValue(pmessage, "FileType", 1, &FileType) ;
    MXSetValue(pmessage, "DistantFileLoc",  1, &DistantFileLoc) ;
    MXSetValue(pmessage, "LocalFileLoc",    1, &LocalFileLoc) ;
    MXSetValue(pmessage, "Identity",        1, &Identity) ;
    MXSetValue(pmessage, "Command",         1, &Command) ;

    return 1;
}


/*----------------------------------------------------------------------------*/


int DGGetList (MXCom* pcom, LONG Identity, char* Name, CHAR FileLoc)
{

    MXMessage* pmessage = MXPutMessage(pcom, DG_SYS, "List") ;
    char Command[20];
    strcpy (Command, "LIST");

    if (Name)
        MXSetValue(pmessage, "Name",            1, Name) ;
    else
        MXSetValue(pmessage, "Name",            1, "") ;

    MXSetValue(pmessage, "FileLoc",         1, &FileLoc) ;
    MXSetValue(pmessage, "Identity",        1, &Identity) ;
    MXSetValue(pmessage, "Command",         1, &Command) ;

    return 1;
}


int DGDir (MXCom* pcom, LONG Identity, char* Name, CHAR FileLoc)
{

    MXMessage* pmessage = MXPutMessage(pcom, DG_SYS, "Dir") ;
    char Command[20];
    strcpy (Command, "DIR");

    if (Name)
        MXSetValue(pmessage, "Name",            1, Name) ;
    else
        MXSetValue(pmessage, "Name",            1, "") ;

    MXSetValue(pmessage, "FileLoc",         1, &FileLoc) ;
    MXSetValue(pmessage, "Identity",        1, &Identity) ;
    MXSetValue(pmessage, "Command",         1, &Command) ;

    return 1;
}

/*----------------------------------------------------------------------------*/

int DGRemoveFiles (MXCom* pcom, LONG Identity, char* FileNames, CHAR FileLoc)
{

    char Command[20];
    MXMessage* pmessage = MXPutMessage(pcom, DG_SYS, "RemoveFiles") ;

    strcpy (Command, "RDELETE");

    MXSetValue(pmessage, "FileNames", 1, FileNames) ;
    MXSetValue(pmessage, "FileLoc",   1, &FileLoc) ;
    MXSetValue(pmessage, "Identity", 1, &Identity) ;
    MXSetValue(pmessage, "Command",  1, &Command) ;

    return 1;
}


/*----------------------------------------------------------------------------*/

int DGRenameFile (MXCom* pcom, LONG Identity, char* OldName, char* NewName, CHAR FileLoc)
{

    char Command[20];

    MXMessage* pmessage = MXPutMessage(pcom, DG_SYS, "RenameFile") ;
    strcpy (Command, "RENAME");

    MXSetValue(pmessage, "OldName", 1, OldName) ;
    MXSetValue(pmessage, "NewName", 1, NewName) ;
    MXSetValue(pmessage, "FileLoc", 1, &FileLoc) ;
    MXSetValue(pmessage, "Identity", 1, &Identity) ;
    MXSetValue(pmessage, "Command",  1, &Command) ;

    return 1;
}


/*----------------------------------------------------------------------------*/


int DGChangeDir (MXCom* pcom, LONG Identity, char* NewDir, char FileLoc)
{

    char Command[20];
    MXMessage* pmessage = MXPutMessage(pcom, DG_SYS, "ChangeDir") ;
    strcpy (Command, "RCD");

    MXSetValue(pmessage, "NewDir",  1, NewDir) ;
    MXSetValue(pmessage, "FileLoc", 1, &FileLoc) ;
    MXSetValue(pmessage, "Identity", 1, &Identity) ;
    MXSetValue(pmessage, "Command",  1, &Command) ;

    return 1;
}


/*----------------------------------------------------------------------------*/


int DGMakeDir (MXCom* pcom, LONG Identity, char* Dir, char FileLoc)
{

    char Command[20];
    MXMessage* pmessage = MXPutMessage(pcom, DG_SYS, "MakeDir") ;
    strcpy (Command, "RMKDIR");

    MXSetValue(pmessage, "Dir",     1, Dir) ;
    MXSetValue(pmessage, "FileLoc", 1, &FileLoc) ;
    MXSetValue(pmessage, "Identity", 1, &Identity) ;
    MXSetValue(pmessage, "Command",  1, &Command) ;

    return 1;
}

/*----------------------------------------------------------------------------*/

int DGRemoveDir (MXCom* pcom, LONG Identity, char* Dir, char FileLoc)
{
    char Command[20];
    MXMessage* pmessage = MXPutMessage(pcom, DG_SYS, "RemoveDir") ;
    strcpy (Command, "RRMDIR");

    MXSetValue(pmessage, "Dir",     1, Dir) ;
    MXSetValue(pmessage, "FileLoc", 1, &FileLoc) ;
    MXSetValue(pmessage, "Identity", 1, &Identity) ;
    MXSetValue(pmessage, "Command",  1, &Command) ;

    return 1;
}

/*----------------------------------------------------------------------------*/

int DGGetDir (MXCom* pcom, LONG Identity, char FileLoc)
{
    char Command[20];
    MXMessage* pmessage = MXPutMessage(pcom, DG_SYS, "GetDir") ;
    strcpy (Command, "RPWD");

    MXSetValue(pmessage, "FileLoc", 1, &FileLoc) ;
    MXSetValue(pmessage, "Identity", 1, &Identity) ;
    MXSetValue(pmessage, "Command",  1, &Command) ;

    return 1;
}
int DGPutError (MXCom* pcom, LONG Identity, LONG Error, char * Param,BOOL Stop)
{
    SHORT stop = Stop ? 1 : 0 ;
    MXMessage * pmessage ;

    pmessage = MXPutMessage(pcom, DG_SYS, "PutError") ;

    MXSetValue(pmessage, "Error", 1,  &Error) ;
    MXSetValue(pmessage, "Param" , 1, Param) ;
    MXSetValue(pmessage, "Stop" , 1, &stop) ;
    MXSetValue(pmessage, "Identity" , 1, &Identity) ;
    return 1;
}


int SYS_PutError (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    LONG Error ;
    LONG Identity ;
    char * Param ;
    SHORT Stop ;


    Identity = *(LONG*)MXGetValue(pmessage, "Identity" , 1) ;
    Error    = *(LONG*)MXGetValue(pmessage, "Error" , 1) ;
    Param    =  (char  *)MXGetValue(pmessage, "Param" , 1) ;
    Stop     = *(SHORT*)MXGetValue(pmessage, "Stop" , 1) ;

    return 1 ;
}

int SYS_AckMessage (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    LONG Error ;
    LONG Identity ;
    DWORD Date, Hour;
    MXMessage* pmessage1;
    List* plist;
    MXCallBack* pexecuteprocedure;

    Identity = *(LONG *)MXGetValue(pmessage, "Identity" , 1) ;
    Error = *(LONG *)MXGetValue(pmessage, "Error" , 1) ;
    Date =  *(DWORD *)MXGetValue(pmessage,"Date" , 1) ;
    Hour = *(DWORD *)MXGetValue(pmessage, "Hour" , 1) ;

    pmessage1 = MXFindMessage (pcom->AckMessages, (WORD)Identity);
    if (!pmessage1) return 1;
    pmessage1->ADate  = Date;
    pmessage1->AHour  = Hour;
    pmessage1->Statut = 1;
    /* !!!!!!!!!!!!! execute les call back associer et liberer si MXSTORE non positio*/
    plist = pmessage1->MessageClass->ExecuteAck;
    while (plist)
    {
        pexecuteprocedure = (MXCallBack *)plist->car;
        plist = plist->cdr;
        pexecuteprocedure->ExecuteProcedure (pmessage1, pcom, pexecuteprocedure->ApplicationField);
    }

    return 1 ;
}


int SYS_ReplyClassConnection (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    XL* pxlat = NULL;
    LONG Error;
    STRING ClassName;
    List* plist;
    MXComClass* pcomclass;
    MXComCallBack* ConnectionProc;


    ClassName =  (STRING)MXGetValue (pmessage,  "Name",       1) ;
    Error = *(LONG *) MXGetValue (pmessage,  "Error",       1) ;

    if (Error == -1)
    {
        OSError ("TEDAG051", " (%s)", ClassName);
        MXCloseCom (pcom->MX, pcom);
        return 1;
    }
    pcomclass = MXGetComClassFromName(pcom->MX, ClassName);
    if (!pcomclass)
    {
        OSError ("TEDAG051", " (%s)", ClassName);
        MXCloseCom (pcom->MX, pcom);
        return 1;
    }
    /*
    if ((strcmp (pcomclass->TableName, "") == 0) ||
            ((XLTable = XFGetXlatFromName(pcomclass->TableName)) == NULL))
        pcom->XF = NULL;
    else
    {
        pxf = (XF *)malloc (sizeof (XF));
        if (!pxf) return 1;
        pxf->SystemFlag = pcom->MX->TP->XF->SystemFlag;
        pxf->StreamSystemFlag = pcom->MX->TP->XF->StreamSystemFlag;

        if (pcomclass->FromToTranscod == MXFROM)
        {
            pxlat = (XL *)malloc(sizeof(XL));
            pxlat->FromTo = pcomclass->FromToTranscod;
            pxlat->Tab = XLTable;
        }
        pxf->XL = pxlat;
        pcom->XF = pxf;
    }
    */
    pcom->ComClass = pcomclass;
    plist = pcomclass->ConnectionProcedures;
    while (plist)
    {
        ConnectionProc = (MXComCallBack *)plist->car;
        plist = plist->cdr;
        ConnectionProc->ExecuteProcedure(pcom, ConnectionProc->ApplicationField);
    }
    return 1;
}

int SYS_SendClassConnection (MXMessage* pmessage, MXCom* pcom, void* appfield)
{

    XL* pxlat = NULL;

    STRING ClassName;
    STRING TableName;
    WORD Port, Code;
    BYTE FromTo;
    MXComClass* pcomclass;
    List* plist;
    MXComCallBack* ConnectionProc;
    LONG Error = 0;

    ClassName =  (STRING) MXGetValue (pmessage,  "Name",       1) ;
    Port  =      *(WORD*) MXGetValue (pmessage,  "Port",       1) ;
    Code  =      *(WORD*) MXGetValue (pmessage,  "Code",       1) ;
    TableName =  (STRING) MXGetValue (pmessage,  "TableName",  1) ;
    FromTo =     *(BYTE*) MXGetValue (pmessage,  "FromTo" ,    1) ;

    pcomclass = MXGetComClassFromName(pcom->MX, ClassName);
    if ((!pcomclass) ||
            (pcomclass->Port != Port) ||
            (pcomclass->FromToTranscod != FromTo) ||
            (pcomclass->Code != Code) ||
            (strcmp (pcomclass->TableName, TableName) != 0))
    {
        Error = -1;
        DGReplyClassConnection (pcom, ClassName, Error);
        OSError ("TEDAG051", " (%s)", ClassName);
        return 1;
    }
/*    
    if ((strcmp (pcomclass->TableName, "") == 0) ||
            ((XLTable = XFGetXlatFromName(pcomclass->TableName)) == NULL))
        pcom->XF = NULL;
    else
    {
        pxf = (XF *)malloc (sizeof (XF));
        if (!pxf) return 1;
        pxf->SystemFlag = pcom->MX->TP->XF->SystemFlag;
        pxf->StreamSystemFlag = pcom->MX->TP->XF->StreamSystemFlag;

        if (pcomclass->FromToTranscod == MXTO)
        {
            pxlat = (XL *)malloc(sizeof(XL));
            pxlat->FromTo = pcomclass->FromToTranscod;
            pxlat->Tab = XLTable;
        }
        pxf->XL = pxlat;
        pcom->XF = pxf;
    }
*/    
    DGReplyClassConnection (pcom, ClassName, Error);
    pcom->ComClass = pcomclass;
    plist = pcomclass->ConnectionProcedures;
    while (plist)
    {
        ConnectionProc = (MXComCallBack *)plist->car;
        plist = plist->cdr;
        ConnectionProc->ExecuteProcedure(pcom, ConnectionProc->ApplicationField);
    }
    return 1;
}



int SYS_RenameFile (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    char OldName[120];
    char NewName[120];
    char FileLoc;
    LONG Identity;
    int Error;
    SHORT NbFile = 1;
    SHORT NbFiles = 1;
    char Command[20];
    LONG error;


    FileLoc         = *(BYTE *)MXGetValue (pmessage,  "FileLoc"  ,     1) ;
    strcpy (OldName,  (char*)MXGetValue (pmessage,  "OldName",  1)) ;
    strcpy (NewName,  (char*)MXGetValue (pmessage,  "NewName",  1)) ;
    strcpy (Command,  (char*)MXGetValue (pmessage,  "Command",  1)) ;
    Identity        = *(LONG *) MXGetValue (pmessage,  "Identity" ,     1) ;


    OSRenameFile (OldName, NewName, FileLoc, &Error);

    error = (LONG)Error;
    pmessage = MXPutMessage(pcom, DG_SYS, "FileReply") ;
    MXSetValue(pmessage, "FileName", 1, OldName) ;
    MXSetValue(pmessage, "Error", 1, &error) ;
    MXSetValue(pmessage, "NbFiles",  1, &NbFiles) ;
    MXSetValue(pmessage, "NbFile",   1, &NbFile) ;
    MXSetValue(pmessage, "Identity", 1, &Identity) ;
    MXSetValue(pmessage, "Command",  1, &Command) ;

    return 1;
}

int SYS_RemoveFiles (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    List* l;
    List* ListFiles = NULL;
    char FileLoc;
    char FileNames[120];
    int Error = 0;
    LONG error;
    SHORT NbFiles  = 0;
    SHORT NbFile = 0;
    LONG Identity;
    char Command[20];

    strcpy (FileNames, (char*) MXGetValue (pmessage,  "FileNames", 1)) ;
    FileLoc = *(BYTE *)MXGetValue (pmessage,  "FileLoc"  ,     1) ;

    NbFiles = OSGetFilesFromWildCards (FileNames, &ListFiles, FileLoc, TRUE);

    if (!ListFiles) return 1;

    l = ListFiles;
    while (l)
    {
        NbFile++;
        OSRemoveFile (((FILEDATA*)l->car)->Name, FileLoc, &Error);
        error = (LONG)Error;
        pmessage = MXPutMessage(pcom, DG_SYS, "FileReply") ;
        MXSetValue(pmessage, "FileName", 1, ((FILEDATA *)l->car)->Name) ;
        MXSetValue(pmessage, "Error", 1, &error) ;
        MXSetValue(pmessage, "NbFiles",  1, &NbFiles) ;
        MXSetValue(pmessage, "NbFile",   1, &NbFile) ;
        MXSetValue(pmessage, "Identity", 1, &Identity) ;
        MXSetValue(pmessage, "Command",  1, &Command) ;
        l = l->cdr;
    }

    while (ListFiles)
    {
        free (ListFiles->car);
        ListRemove (&ListFiles, ListFiles->car);
    }

    return 1 ;
}

int SYS_GetFile(MXMessage* pmessage, MXCom* pcom, void* appfield)
{

    SHORT NbFiles = 1;
    SHORT NbFile =  1;
    LONG Identity;
    int Error = 0;
    char Command[20];
    char FileName[350];
    char LocalRepertory[120];
    char OnlyFileName[120];

    FILEPARM File ;
    char FileType, DistantFileLoc, LocalFileLoc;



    /* il faut voir si le nom FileName contient un repertoire */


    FileType        = *(BYTE *)MXGetValue (pmessage , "FileType",      1);
    LocalFileLoc    = *(BYTE *)MXGetValue (pmessage , "LocalFileLoc",  1);
    DistantFileLoc  = *(BYTE *)MXGetValue (pmessage , "DistantFileLoc",1);
    Identity        = *(LONG *) MXGetValue (pmessage,  "Identity" ,     1) ;
    strcpy (Command,     (char*)MXGetValue (pmessage,  "Command"  ,     1)) ;

    strcpy(FileName, (char*)  MXGetValue (pmessage,  "FileName",  1)) ;


    OSGetOnlyName (FileName, OnlyFileName, LocalRepertory, DistantFileLoc);
    if (strcmp (LocalRepertory, "") == 0)    OSGetDir (LocalRepertory, DistantFileLoc, &Error);

    OSSetFileName (File.FileOri, LocalRepertory, OnlyFileName, DistantFileLoc);
    strcpy(File.FileDest, (char*)  MXGetValue (pmessage,  "DestName",  1)) ;

    File.FileDestLocal = LocalFileLoc ;
    File.FileOriLocal  = DistantFileLoc ;
    File.FileType      = FileType;

    pmessage = MXPutMessage(pcom, DG_SYS, "SendFile") ;
    MXSetValue(pmessage, "File", 1, &File) ;
    MXSetValue(pmessage, "NbFiles",  1, &NbFiles) ;
    MXSetValue(pmessage, "NbFile",   1, &NbFile) ;
    MXSetValue(pmessage, "Identity", 1, &Identity) ;
    MXSetValue(pmessage, "Command",  1, &Command) ;


    return 1 ;
}

int SYS_GetFiles (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    char FileNames[120];
    char DistantRepertory[120];
    char FileType, DistantFileLoc, LocalFileLoc;
    LONG Identity;
    char Command[20];


    strcpy (FileNames,        (char*) MXGetValue (pmessage,  "FileNames"  ,     1)) ;
    strcpy (DistantRepertory, (char*) MXGetValue (pmessage,  "LocalRepertory" , 1)) ;
    FileType        = *(BYTE *)MXGetValue (pmessage , "FileType"       , 1);
    LocalFileLoc    = *(BYTE *)MXGetValue (pmessage , "LocalFileLoc"    , 1);
    DistantFileLoc  = *(BYTE *)MXGetValue (pmessage , "DistantFileLoc" , 1);
    Identity        = *(LONG *) MXGetValue (pmessage,  "Identity" ,     1) ;
    strcpy (Command,    (char*)MXGetValue (pmessage,  "Command"  ,     1)) ;

    DGSendFiles (pcom, Identity, FileNames, DistantRepertory, FileType, DistantFileLoc, LocalFileLoc);
    return 1;
}

int SYS_SendFile (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    FILEPARM* File;
    LONG Error;
    char Command[20];
    SHORT NbFiles = 0;
    SHORT NbFile = 0;
    LONG Identity;

    File     =  (FILEPARM*) MXGetValue (pmessage,  "File"  ,        1) ;
    NbFiles  = *(SHORT*)MXGetValue (pmessage,  "NbFiles",       1) ;
    NbFile   = *(SHORT*)MXGetValue (pmessage,  "NbFile"  ,      1) ;
    Identity = *(LONG*) MXGetValue (pmessage,  "Identity" ,     1) ;
    strcpy (Command,  MXGetValue (pmessage,  "Command"  ,     1)) ;

    Error = (LONG)File->FileError;
    pmessage = MXPutMessage(pcom, DG_SYS, "FileReply") ;
    MXSetValue(pmessage, "FileName", 1,   File->FileOri) ;
    MXSetValue(pmessage,  "Command", 1 ,  Command);
    MXSetValue(pmessage,  "Error",   1 ,  &Error ) ;
    MXSetValue(pmessage,  "Identity",1 ,  &Identity);
    MXSetValue(pmessage,  "NbFiles", 1 ,  &NbFiles);
    MXSetValue(pmessage,  "NbFile",  1 ,  &NbFile);
    free (File);
    return 1;
}


int SYS_List(MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    LONG Identity;
    int Error = 0;
    List* ListFiles = NULL;
    STRING Command;
    STRING Name;
    char LocalRepertory[120];
    char OnlyFileName[120];
    SHORT NbrFiles;
    SHORT NbrFile = 1;
    CHAR FileLoc;


    Name      = (STRING)MXGetValue (pmessage, "Name",       1);
    Identity  = *(LONG *) MXGetValue (pmessage,  "Identity" ,     1) ;
    Command   = (STRING)MXGetValue (pmessage,  "Command"  ,     1) ;
    FileLoc   = *(CHAR *)MXGetValue (pmessage,  "FileLoc"  ,     1) ;


    if (strlen (Name) == 0)
        NbrFiles =  OSGetFilesFromWildCards ("*", &ListFiles, FileLoc, FALSE);
    else
    {
        OSGetOnlyName (Name, OnlyFileName, LocalRepertory, FileLoc);
        if (strcmp (OnlyFileName, Name) == 0)
            NbrFiles =  OSGetFilesFromWildCards (Name, &ListFiles, FileLoc, FALSE);
        else
            NbrFiles =  OSGetFilesFromWildCards (Name, &ListFiles, FileLoc, TRUE);
    }
    while (ListFiles)
    {
        FILEDATA* filedata = (FILEDATA *)ListFiles->car;
        pmessage = MXPutMessage(pcom, DG_SYS, "ListReply") ;
        MXSetValue(pmessage, "Name",      1, filedata->Name) ;
        MXSetValue(pmessage, "Access",    1, filedata->Access) ;
        MXSetValue(pmessage, "Date",      1, filedata->Date) ;
        MXSetValue(pmessage, "Type",      1, &filedata->Type) ;
        MXSetValue(pmessage, "Size",      1, &filedata->Size) ;
        MXSetValue(pmessage, "NbFiles",   1, &NbrFiles) ;
        MXSetValue(pmessage, "NbFile",    1, &NbrFile) ;
        MXSetValue(pmessage, "Identity", 1,  &Identity) ;
        MXSetValue(pmessage, "Command",   1, Command) ;
        NbrFile++;
        free (ListFiles->car);
        ListRemove (&ListFiles, ListFiles->car);
    }
    return 1 ;
}


int SYS_Dir(MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    LONG Identity;
    int Error = 0;
    List* ListFiles = NULL;
    STRING Command;
    STRING Name;
    char LocalRepertory[120];
    char OnlyFileName[120];
    SHORT NbrFiles;
    CHAR FileLoc;
    BUFFERPARM Buffer;
    char FileLine[300];
    char* Buf = 0;
    DWORD BufSize = 0;

    FileLoc   = *(CHAR *)MXGetValue (pmessage,  "FileLoc"  ,     1);
    Name      = (STRING) MXGetValue (pmessage,  "Name",          1);
    Identity  = *(LONG *)   MXGetValue (pmessage,  "Identity" ,     1);
    Command   = (STRING) MXGetValue (pmessage,  "Command"  ,     1);


    if (strlen (Name) == 0)
        NbrFiles =  OSGetFilesFromWildCards ("*", &ListFiles, FileLoc, FALSE);
    else
    {
        OSGetOnlyName (Name, OnlyFileName, LocalRepertory, FileLoc);
        if (strcmp (OnlyFileName, Name) == 0)
            NbrFiles =  OSGetFilesFromWildCards (Name, &ListFiles, FileLoc, FALSE);
        else
            NbrFiles =  OSGetFilesFromWildCards (Name, &ListFiles, FileLoc, TRUE);
    }

    if (ListFiles)
    {
        Buf = (char*)malloc (MXBUFVALUESIZE);
        Buf[0] = 0;
    }

    while (ListFiles)
    {
        FILEDATA* filedata = (FILEDATA *)ListFiles->car;
        sprintf (FileLine, "%s %s %d\r\n",  filedata->Name,   ((filedata->Type == 1) ? "<Dir>" : ((filedata->Type == 2) ? "<Link>" : "")), filedata->Size);
        strcat (Buf, FileLine);
        BufSize += (int)strlen (FileLine);
        free (ListFiles->car);
        ListRemove (&ListFiles, ListFiles->car);
    }
    Buffer.BufferType    = 'T';
    Buffer.BufferSize    = BufSize;
    Buffer.BufferContent = Buf;

    if (ListFiles)  free (Buf);

    pmessage = MXPutMessage(pcom, DG_SYS, "GDirReply") ;
    MXSetValue(pmessage, "Identity",  1, &Identity) ;
    MXSetValue(pmessage, "Command",   1, Command) ;
    MXSetValue (pmessage, "Buffer",   1, &Buffer);
    return 1 ;
}



int SYS_MakeDir (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    char Dir[120];
    char CurrentDir[120];
    char FileLoc;
    char Command[20];
    LONG Identity;
    int Error;
    LONG error;


    strcpy (Dir,      (char*)  MXGetValue (pmessage,  "Dir"  ,     1)) ;
    strcpy (Command,  (char*)  MXGetValue (pmessage,  "Command",  1)) ;
    Identity        = *(LONG *) MXGetValue (pmessage,  "Identity" ,     1) ;
    FileLoc         = *(BYTE *)MXGetValue (pmessage , "FileLoc"    , 1);

    OSMakeDir (Dir, FileLoc, &Error);

    error = (LONG)Error;

    OSGetDir (CurrentDir, FileLoc, &Error);

    pmessage = MXPutMessage(pcom, DG_SYS, "DirReply") ;
    MXSetValue(pmessage, "Dir",      1, CurrentDir) ;
    MXSetValue(pmessage, "Error",    1, &error) ;
    MXSetValue(pmessage, "Identity", 1, &Identity) ;
    MXSetValue(pmessage, "Command",  1, &Command) ;

    return 1;
}


int SYS_RemoveDir (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    char Dir[120];
    char CurrentDir[120];
    char Command[20];
    LONG Identity;
    char FileLoc;
    int Error;
    LONG error;


    strcpy (Dir,      (char*) MXGetValue (pmessage,   "Dir"  ,        1)) ;
    strcpy (Command,  (char*)MXGetValue (pmessage,    "Command",      1)) ;
    Identity        = *(LONG *) MXGetValue (pmessage,  "Identity" ,    1) ;
    FileLoc         = *(BYTE *)MXGetValue (pmessage , "FileLoc"    ,  1);

    OSRemoveDir (Dir, FileLoc, &Error);
    error = (LONG)Error;
    OSGetDir (CurrentDir, FileLoc, &Error);

    pmessage = MXPutMessage(pcom, DG_SYS, "DirReply") ;
    MXSetValue(pmessage, "Dir",      1, CurrentDir) ;
    MXSetValue(pmessage, "Error",    1, &error) ;
    MXSetValue(pmessage, "Identity", 1, &Identity) ;
    MXSetValue(pmessage, "Command",  1, &Command) ;

    return 1;
}


int SYS_GetDir (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    char CurrentDir[120];
    char FileLoc;
    char Command[20];
    LONG Identity;
    LONG error = 0;
    int Error = 0;


    FileLoc         = *(BYTE *)MXGetValue (pmessage , "FileLoc"    , 1);
    strcpy (Command,  (char*)MXGetValue (pmessage,    "Command",      1)) ;
    Identity        = *(LONG *) MXGetValue (pmessage,  "Identity" ,    1) ;
    OSGetDir (CurrentDir, FileLoc, &Error);
    error = (LONG)Error;

    pmessage = MXPutMessage(pcom, DG_SYS, "DirReply") ;
    MXSetValue(pmessage, "Dir",      1, CurrentDir) ;
    MXSetValue(pmessage, "Error",    1, &error) ;
    MXSetValue(pmessage, "Identity", 1, &Identity) ;
    MXSetValue(pmessage, "Command",  1, &Command) ;
    return 1;
}




int SYS_ChangeDir (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    char NewDir[120];
    char CurrentDir[120];
    char FileLoc;
    char Command[20];
    LONG Identity;
    int Error;
    LONG error;


    strcpy (NewDir,   (char*) MXGetValue (pmessage,  "NewDir"  ,     1)) ;
    strcpy (Command,  (char*)MXGetValue (pmessage,  "Command",  1)) ;
    Identity        = *(LONG *) MXGetValue (pmessage,  "Identity" ,     1) ;
    FileLoc         = *(BYTE *)MXGetValue (pmessage , "FileLoc"    , 1);

    OSChangeDir (NewDir, FileLoc, &Error);


    error = (LONG)Error;
    OSGetDir (CurrentDir, FileLoc, &Error);

    pmessage = MXPutMessage(pcom, DG_SYS, "DirReply") ;
    MXSetValue(pmessage, "Dir",      1, CurrentDir) ;
    MXSetValue(pmessage, "Error",    1, &error) ;
    MXSetValue(pmessage, "Identity", 1, &Identity) ;
    MXSetValue(pmessage, "Command",  1, &Command) ;

    return 1;
}




/*----------------------------------------------------------------------------*/


