/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "cb2a.h"




MXMessage* CB2AReturnMessageFromStream (MX* pmx, char* stream)
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

    CB2AReadMessageHeader(pmx, stream, &header);

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



MXMessage* CB2ACheckAndReturnMessage (MX* pmx, MXCom* pcom, char* stream)
{

    WORD RequestSize;
    DWORD MessageSize;
    MXMessageHeader header;
    MXMessageStatut statut;
    MXMessage* pmessage;

    RequestSize = TPGetRequestLength (stream);      /* TP dependent */

    /* check if request size contains at least the message header */

    if (RequestSize < (sizeof(DOUBLE) + sizeof (WORD))) // bitmap + type of message
    {
        OSError ("TEDAG020", " (%d)", RequestSize);
        return NULL;
    }

    /* pheader points to the machine independant structure */

    CB2AReadMessageHeader(pmx, stream, &header);
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




LONG CB2AReceiveMessage (TP* ptp, TPClient* pclient, char* buf, int count)
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
    pmessage = CB2ACheckAndReturnMessage (pmx, pcom, stream);
    if (!pmessage)
        return -1;

    pmessage->Com = pcom;

    if (pcom->TraceProcedure)  pcom->TraceProcedure (pmessage, MXONRECV, buf, count);


    ret = CB2AReadMessage (pmx, pmessage, stream, count, (BYTE)((pcom->Protocol == IOPROTO_STORE) ? 0 : 1));

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


LONG CB2ASendMessage (TP* ptp, TPClient* pclient, char* buf, int size)
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

    ret = CB2AWriteMessage (pmx, pmessage, buf, size - 2, (BYTE)((pcom->Protocol == IOPROTO_STORE) ? 0 : 1)); /*client->TransSize - tp header;*/

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

MXMessage* CB2ARecv (MX* pmx, MXCom* pcom)
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

        pmessage = CB2ACheckAndReturnMessage (pmx, pcom, stream);
        if (!pmessage)
        {
            TPSetBlockingMode (pcom->Client, FALSE);
            return NULL;
        }
        pmessage->Com = pcom;
        if (pcom->TraceProcedure) pcom->TraceProcedure (pmessage, MXONRECV, stream, ret);
        ret = CB2AReadMessage (pmx, pmessage, stream, ret, (BYTE)((pcom->Protocol == IOPROTO_STORE) ? 0 : 1));
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




LONG CB2ASend (MX* pmx, MXCom* pcom, MXMessage* pmessage)
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
        ret = CB2AWriteMessage (pmx, pmessage, pcom->Client->TransBuffer, pcom->Client->TransSize - 2, (BYTE)((pcom->Protocol == IOPROTO_STORE) ? 0 : 1));

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



int CB2AReadMessageHeader (MX* pmx, char* stream, MXMessageHeader* pheader)
{

    LONG from = 0;
    XF* pxf = pmx->TP->XF;
    char* fromstream = stream;     

	fromstream += XFWordRead (pxf, (char*)(&pheader->MessageCode), &from, fromstream, 1);
    from = 0;
    fromstream += XFDoubleRead (pxf, (char*)(&pheader->Part), &from, fromstream, 1);
    
	return (sizeof(WORD) + sizeof(DOUBLE))	;
}

int CB2AWriteMessageHeader (MX* pmx, MXMessage* pmessage, char* stream, BOOL FirstTime)
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




LONG CB2AReadMessage (MX* pmx, MXMessage* pmessage, char* stream, int streamsize, BYTE WithFileContent)
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


LONG CB2AWriteMessage (MX* pmx, MXMessage* pmessage, char* stream, int maxcount, BYTE WithFileContent)
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

    written = CB2AWriteMessageHeader(pmx, pmessage, stream, FirstTime);
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


