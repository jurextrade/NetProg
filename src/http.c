/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "../include/http.h"

void HTTPEndOtherMessages (MX* pmx, MXCom* pcom, MXMessage* pmessage)
{
    MXCallBack*  pexecuteprocedure;
    MXCallBack*  pexecutecomprocedure;
    List* plist;

    OSDateHour (&pmessage->ADate, &pmessage->AHour);
    pmessage->Complete = 1;  /* In case we don't have the length of the Response or Request */
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






LONG HTTPWriteMessage (MX* pmx, MXMessage* pmessage, char* stream, int maxcount, BYTE WithFileContent)
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
        case MXSTRING :
            for (j = FromSizeOffset; j < pobject->Size; j++, from = 0)
            {
                LONG fromzero = 0;
                char* httpstream = (char*) (pmessage->Stream + pmessage->Values[(WORD)pobject->Offset + j]);
                if ((written + (int)strlen (httpstream) + (int)strlen (pobject->Name) + 2 + 2) <= maxcount)
                {

                    if (strlen (httpstream) == 0)  /* no value for tag */
                    {
                        pcontext->StillToRead -= 1;
                    }
                    else
                    {
                        if ((strcmp (pobject->Name, "Request-Line")  != 0) &&
                            (strcmp (pobject->Name, "Status-Line") != 0))
                        {
                            size = XFStringWrite (pxf, pobject->Name, &fromzero, tostream, 1);
                            tostream +=size - 1;
                            written += size - 1;
                            fromzero = 0;
                            size = XFStringWrite   (pxf,  ":",  &fromzero, tostream, 1);
                            tostream += size - 1;
                            written += size - 1;
                        }
                        size = XFStringWrite (pxf, httpstream, &from, tostream, 1);
                        pcontext->StillToRead -= size;
                        written += size - 1;
                        tostream += size - 1;    /* don't add 0 at end */
                        fromzero = 0;
                        XFStringWrite (pxf, "\r\n", &fromzero, tostream, 1);
                        written += 2;
                        tostream += 2;
                    }
                }
                else
                {
                    State = 1;
                    break;
                }
            }
            break;
        case MXBUFFER :
            for (j = FromSizeOffset; j < pobject->Size; j++, from = 0)
            {
				LONG fromzero = 0;

                if (!MXBufferProcessing (pmessage))
                {
					if ((strcmp(pobject->Name, "Content") == 0))
					{
						fromzero = 0;
						XFStringWrite(pxf, "\r\n", &fromzero, tostream, 1);
						written += 2;
						tostream += 2;
					}
					pcontext->StillToRead -= sizeof(DWORD) + sizeof(CHAR);
                }

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


				fromzero = 0;
				XFStringWrite(pxf, "\r\n", &fromzero, tostream, 1);
				written += 2;
				tostream += 2;
				
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


MXMessage* HTTPRecv (MX* pmx, MXCom* pcom)
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
			return NULL;
        }
        if (ret == 0) return NULL;

        stream = pcom->Client->RequestBuffer;

        pmessage = HTTPCheckAndReturnMessage (pmx, pcom, stream);
        if (!pmessage)
        {
            TPSetBlockingMode (pcom->Client, FALSE);
            return NULL;
        }
        pmessage->Com = pcom;
        if (pcom->TraceProcedure) pcom->TraceProcedure (pmessage, MXONRECV, stream, ret);
        ret = HTTPReadMessage (pmx, pmessage, stream, ret, (BYTE)((pcom->Protocol == IOPROTO_STORE) ? 0 : 1));
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




LONG HTTPSend (MX* pmx, MXCom* pcom, MXMessage* pmessage)
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
        ret = HTTPWriteMessage (pmx, pmessage, pcom->Client->TransBuffer, pcom->Client->TransSize - 2, 1);

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



LONG HTTPSendMessage (TP* ptp, TPClient* pclient, char* buf, int size)
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

    ret = HTTPWriteMessage (pmx, pmessage, buf, size, 1); /*client->TransSize - tp header;*/

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


MXMessage* HTTPCheckAndReturnMessage (MX* pmx, MXCom* pcom, char* stream)
{
    MXMessage* pmessage = NULL;

    if (pcom->InputMessages)
        pmessage = (MXMessage*)pcom->InputMessages->car;
    else
    {
        if (memcmp (stream, "HTTP", 4) == 0)
            pmessage = MXCreateMessage (pmx, HTTP_SYS, "Response");
        else
            pmessage = MXCreateMessage (pmx, HTTP_SYS, "Request");
        ListNewr (&pcom->InputMessages, pmessage);

    }
    return pmessage;
}

int HTTPCloseConnection (MXCom* pcom, void* app)
{
    if (pcom->InputMessages)
    {
        MXMessage* pmessage = (MXMessage*)pcom->InputMessages->car;
        /*     MXObject* pobject = MXGetObjectFromName (pmessage->MessageClass, "Content");
             char* Stream = pmessage->Stream + pmessage->Offset;
             int size;
             LONG from = 0;
             XF* pxf;
             
             if ((pmessage->Com) && (pmessage->Com->ComClass) && (pmessage->Com->XF))
              pxf = pmessage->Com->XF;
             else pxf = pmx->TP->XF;
             
             int size = XFDWordRead (pxf, Stream, &from, (char*)&pmessage->Size, 1);
        */   

        ListRemove(&pcom->InputMessages, pmessage);
        HTTPEndOtherMessages (pcom->MX, pcom, pmessage);
    }
    return 1;
}



LONG HTTPReadMessage (MX* pmx, MXMessage* pmessage, char* stream, int streamsize, BYTE WithFileContent)
{
    XF* pxf;
    BOOL FirstTime = FALSE;
    int isread = 0;
    char* c;
    char* v;
    char linevalues[50][3500];
    int EndHeader = 0;
    char* ContentLength;
	char* Encoding;
    char* fromstream = stream;
    int size;
    MXObject* pobject;
    LONG from = 0;
    char* Stream;
    char Type = 't';
    char* bufferattributes;



    MXMessageClass* pclassmess = pmessage->MessageClass;
    MXMessageContext* pcontext = pmessage->Context;
    int State = 0;
    int i = 0;
    int j = 0;
	int length;

    if (!pcontext)
    {
        FirstTime = TRUE;
        pcontext = MXAllocateContext ();
        if (!pcontext)
        {
            return -1;
        }
        pmessage->Context = pcontext;
        pcontext->StillToRead = 0;
        /*      MXInitValues (pmessage);*/
    }

    if ((pmessage->Com) && (pmessage->Com->ComClass) && (pmessage->Com->XF))
        pxf = pmessage->Com->XF;
    else pxf = pmx->TP->XF;

    if (FirstTime)
    {
        while (isread <= streamsize)
        {
            char ch = stream[isread];
            switch( ch )
            {
            case '\r': // ignore
                if (j == 0) EndHeader = 1;
                break;
            case '\n': // end-of-line
                linevalues[i][j]= 0;
                if (i == 0)
                {
                    if (memcmp (stream, "HTTP", 4) == 0)
                        MXSetValue (pmessage, "Status-Line", 1, linevalues[0]);
                    else
                        MXSetValue (pmessage, "Request-Line", 1, linevalues[0]);
                }
                else
                {
                    c = strtok (linevalues[i], ":");
                    v = strtok (NULL, "\r\n");
                    if (MXSetValue (pmessage, c, 1, v+1) < 0)
                        printf ("Unrecognized Tag : %s value : %s \n", c, v);
					else
					{
						//printf("Tag : %s value : %s \n", c, v);
					}
                }
                i++;
                j = 0;
                break;
            default:   // other....
                linevalues[i][j]= ch;
                j++;
                break;
            }
            isread++;
            if (EndHeader == 1)
            {
                isread++;
                break;
            }
        }


        if (EndHeader == 1)
        {
			Encoding = MXGetValue (pmessage,"Transfer-Encoding", 1);
			if (strcmp (Encoding, "chunked") == 0)
			{
			   EndHeader = 0;
               while (isread <= streamsize)
			   {
                   char ch = stream[isread];
		            switch( ch )
				    {
						case '\r': // ignore

						break;
						case '\n': // end-of-line
							EndHeader = 1;
							linevalues[i][j]= 0;
						break;
			            default:   // other....
						    linevalues[i][j]= ch;
							j++;
		                break;
				    }
					isread++;
					if (EndHeader == 1)
					{
		               	length = (int)strtol(linevalues[i], NULL,16);
						pcontext->StillToRead = length;
				        break;
					}
			   }
			}
			else
			{
	            ContentLength = MXGetValue (pmessage, "Content-Length", 1);    
                pcontext->StillToRead = atoi(ContentLength);
			}
			fromstream += isread;

			if (memcmp (stream, "HTTP", 4) == 0)
                if (pcontext->StillToRead == 0)
                    pcontext->StillToRead = -1;
                else
                    pmessage->Size = pcontext->StillToRead;
            else
            {
                pmessage->Size = pcontext->StillToRead;
                if ((pcontext->StillToRead == 0) && (streamsize != isread))
                {
                    printf ("Header Having Content With No Content Length !!!\n");
                    pcontext->StillToRead = streamsize - isread;
                    pmessage->Size = pcontext->StillToRead;

                }
            }
        }
    }
	if (isread > streamsize) isread = streamsize;

    if (pcontext->StillToRead == -1)
        pmessage->Size +=  streamsize - isread;

    pobject = MXGetObjectFromName (pclassmess, "Content");
    if (!MXBufferProcessing (pmessage) || (pcontext->StillToRead == -1))       /* = -1 means unknow size */
    {
        DWORD BufSize;
        if (pmessage->StreamSize < pmessage->Offset + from + sizeof (DWORD) + sizeof (CHAR))
            if (MXExtendValueBuffer (pmessage, MXBUFVALUESIZE) == 0) return -1;
        Stream = pmessage->Stream + pmessage->Offset;

        pmessage->Values[(WORD)pobject->Offset] = pmessage->Offset + from;
        size = XFDWordRead (pxf, Stream, &from, (char*)&pmessage->Size, 1);
        size = XFCharRead (pxf, Stream, &from, (char*)&Type, 1);
        OSCopy4 (&BufSize, Stream + from - 5);
        size = BufSize;
        if (pmessage->StreamSize < pmessage->Offset + from + size)
            if (MXExtendValueBuffer (pmessage, size) == 0) return -1;
    }

    bufferattributes = (char*) (pmessage->Stream + pmessage->Values[(WORD)pobject->Offset]);
    size = MXReadBufferContent (pmx, pcontext, fromstream,  bufferattributes, streamsize - isread);
    if (size < 0) /* something strange */
    {
        return -1;
    }
    fromstream += size;
    isread += size;
    from += size;

    if (pcontext->StillToRead == -1)
        return (LONG)(fromstream - stream);

    pcontext->StillToRead -= size;
    if (pcontext->StillToRead == 0)
    {
        OSDateHour (&pmessage->ADate, &pmessage->AHour);
        pmessage->Complete = 1;
        pmessage->Offset += pmessage->Size;
    }
    MXInitMessage (pmessage);
    return (LONG)(fromstream - stream);
}





LONG HTTPReceiveMessage (TP* ptp, TPClient* pclient, char* buf, int count)
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

    pmessage = HTTPCheckAndReturnMessage (pmx, pcom, stream);
    if (!pmessage)
        return -1;

    pmessage->Com = pcom;


    if (pcom->TraceProcedure)  pcom->TraceProcedure (pmessage, MXONRECV, buf, count);


    ret = HTTPReadMessage (pmx, pmessage, stream, count, 1);

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


