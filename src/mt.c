/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS

#include "mt.h"



MXMessage* MTCheckAndReturnMessage (MX* pmx, MXCom* pcom, char* stream)
{
    MXMessage* pmessage = NULL;
	if (pcom->InputMessages)
		pmessage = (MXMessage*)pcom->InputMessages->car;
	else
	{
	    pmessage = MXCreateMessage (pmx, MT_SYS, "Stream");
		ListNewr(&pcom->InputMessages, pmessage);
	}
    return pmessage;
}


LONG MTReceiveMessage (TP* ptp, TPClient* pclient, char* buf, int count)
{
    char* stream;
    MX* pmx;
    MXMessage* pmessage;
    LONG ret = 0;
	int read = count;
    List* plist;
    MXCallBack* pexecutecomprocedure;
    MXCallBack* pexecuteprocedure;


    MXCom* pcom = (MXCom*)pclient->ApplicationField;
    pmx = pcom->MX;


    if ((!pcom) || (!pcom->Client) || (!pclient->RequestBuffer))
        return -1;


    stream = buf;

	
	while (read > 0)                      //Read all 
	{

		pmessage = MTCheckAndReturnMessage (pmx, pcom, stream);
		
		if (!pmessage)
			return -1;

		pmessage->Com = pcom;


		if (pcom->TraceProcedure)  pcom->TraceProcedure (pmessage, MXONRECV, buf, count);

		ret = MTReadMessage (pmx, pmessage, stream , read, 1);
        

		if (ret < 0)
		{
			pmessage->Complete = 1;     /*finish with this message */
		}
		stream += ret;
		read -= ret; 

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
			if (!pcom->Client) return -1;
		}
	}
    return ret;
}


LONG MTSendMessage (TP* ptp, TPClient* pclient, char* buf, int size)
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

    ret = MTWriteMessage (pmx, pmessage, buf, size, 1); /*client->TransSize - tp header;*/

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

LONG MTReadMessage (MX* pmx, MXMessage* pmessage, char* stream, int streamsize, BYTE WithFileContent)
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

    MXMessageClass* pclassmess = pmessage->MessageClass;
    MXMessageContext* pcontext = pmessage->Context;

    int State = 0;
    int i;
    int k = 0; 
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
            case MXBUFFER :
            {
                DWORD BufferSize;
  			    if (pmessage->StreamSize < pmessage->Offset  + streamsize)
                    if (MXExtendValueBuffer (pmessage, streamsize) == 0) 
						 return -1;
                bufferattributes = (char*) (pmessage->Stream + pmessage->Values[(WORD)pobject->Offset]);
                if (MXBufferProcessing (pmessage))
					OSCopy4(&BufferSize, bufferattributes);
				else BufferSize = 0;
                while (k < streamsize && stream[k] != 0 )
					k++;
				if (stream[k] == 0) 
				{
					pmessage->Complete = 1;
				}
                if (k != streamsize) 
				{
					k++; 
				}
				BufferSize += (WORD)k;
				if (BufferSize == 0) 
					return -1;
				memcpy (bufferattributes , (CHAR *)(&BufferSize) , sizeof (DWORD));
				pcontext->Buffer = bufferattributes + sizeof (DWORD) + sizeof (CHAR);
				memcpy ((char *)(pcontext->Buffer + pcontext->Pos), fromstream , k);
				pcontext->Pos += k;
				pmessage->Offset += k;
            }
            break;
            default:
            OSError ("TEDAG032",  " (%s)", pmessage->MessageClass->Name);
            return -1;
        }
    }
    MXInitMessage (pmessage);
    return (LONG)(k);
}



LONG MTWriteMessage (MX* pmx, MXMessage* pmessage, char* stream, int maxcount, BYTE WithFileContent)
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
    char* bufferattributes;



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
        switch (type)
        {
        case MXBUFFER :
            for (j = FromSizeOffset; j < pobject->Size; j++, from = 0)
            {
                bufferattributes = (char*) (pmessage->Stream + pmessage->Values[(WORD)pobject->Offset + j]);
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
    }
    MXInitMessage (pmessage);
    return written ;
}


LONG MTSend (MX* pmx, MXCom* pcom, MXMessage* pmessage)
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
        ret = MTWriteMessage (pmx, pmessage, pcom->Client->TransBuffer, pcom->Client->TransSize - 2, 1);

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

MXMessage* MTRecv (MX* pmx, MXCom* pcom)
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
        while ((ret  = pcom->Client->ReadFunction (pmx->TP, pcom->Client)) == 0);
        if (ret < 0)
        {
            if (!pcom->Client->ClientGone) MXCloseCom (pmx, pcom);
            if (pmessage) MXFreeMessage (pcom->MX, pmessage);
            return NULL;
        }
        stream = pcom->Client->RequestBuffer;

        pmessage = MTCheckAndReturnMessage (pmx, pcom, stream);
        if (!pmessage)
        {
            TPSetBlockingMode (pcom->Client, FALSE);
            return NULL;
        }
        pmessage->Com = pcom;
        if (pcom->TraceProcedure) pcom->TraceProcedure (pmessage, MXONRECV, stream, ret);
        ret = MTReadMessage (pmx, pmessage, stream, ret, 1);
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
