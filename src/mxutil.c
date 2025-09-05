/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "mx.h"

MXMessage* MXAllocMessage (MXDialogClass* pclass, MXMessageClass* pclassmessage)
{
    MXMessage* pmessage;
    char* deststream;
    char* values;

    pmessage = (MXMessage  *)malloc (sizeof (MXMessage));
    if (!pmessage )
    {
        OSError ("TEDAG019", NULL);
        return NULL;
    }
    memset (pmessage, 0, sizeof (MXMessage));

    pmessage->MessageClass = pclassmessage;
    pmessage->Class = pclass;
    pmessage->Values   = NULL;
    pmessage->Context  = NULL;
    pmessage->Size     = 0;
    pmessage->Offset   = 0;
    pmessage->Complete = 0;
    pmessage->Identity = 0;
    pmessage->Part     = 0;
    pmessage->Stream   = NULL;
    pmessage->StreamSize = 0;
    pmessage->Com      = NULL;
    pmessage->NbrRows  = 1;
    pmessage->InternalField = NULL;

    pmessage->Statut   = 0;
    pmessage->Type     = MXNORMAL;
    pmessage->TDate    = 0;
    pmessage->THour    = 0;
    pmessage->PDate    = 0;
    pmessage->PHour    = 0;
    pmessage->DDate    = 0;
    pmessage->DHour    = 0;
    pmessage->ADate    = 0;
    pmessage->AHour    = 0;
    OSDateHour (&pmessage->PDate, &pmessage->PHour);


    if (pclassmessage->Size != 0)
    {
        values = (char*)malloc (pclassmessage->Size * sizeof (int));
        if ((pclassmessage->Size) && (!values))
        {
            OSError ("TEDAG019", NULL);
            MXFreeMessage (pclass->MX, pmessage);
            return NULL;
        }
        else pmessage->Values = (DWORD *)values;
        deststream = (char*)malloc (MXBUFVALUESIZE);

        if (!deststream)
        {
            OSError ("TEDAG019", NULL);
            MXFreeMessage (pclass->MX, pmessage);
            return NULL;
        }
        else
        {
            pmessage->Stream = deststream;
            pmessage->StreamSize = MXBUFVALUESIZE;
        }
        memset (pmessage->Values, 0, pclassmessage->Size);

        memset (deststream , (int)0, MXBUFVALUESIZE);
    }
    MXInitValues (pmessage);
    return pmessage;
}

void MXTraceMessage (MXMessage* pmessage)
{   
    
	char Buf[250];
	List*  Objects;
	int i;
    sprintf(Buf, "Dialog : %s\n", pmessage->Class->Name);
    printf(Buf);


	sprintf (Buf, "Message : %s\n", pmessage->MessageClass->Name);
	printf (Buf);



	Objects = pmessage->MessageClass->Objects;

	while (Objects)
	{
		MXObject*  pobject = (MXObject*)Objects->car;
		switch (pobject->Type) {
		case MXSHORT :
			for (i = 0; i < pobject->Size; i++)
			{
				if (pobject->Size == 1)
					sprintf (Buf, "MXSHORT %s : %hd\n", pobject->Name, *(SHORT*)MXGetValue (pmessage,  pobject->Name, 1));
				else
					sprintf (Buf, "MXSHORT %s[%d] : %hd\n", pobject->Name, i + 1, *(SHORT*)MXGetValue (pmessage,  pobject->Name, i + 1));

				printf (Buf);
			}
			break;
		case MXWORD :
			for (i = 0; i < pobject->Size; i++)
			{
				if (pobject->Size == 1)
					sprintf (Buf, "MXWORD %s : %hu\n", pobject->Name, *(WORD*)MXGetValue (pmessage,  pobject->Name, 1));
				else
					sprintf (Buf, "MXWORD %s[%d] : %hu\n", pobject->Name, i + 1, *(WORD*)MXGetValue (pmessage,  pobject->Name, i + 1));
				printf (Buf);
			}
			break;
		case MXDWORD :
            for (i = 0; i < pobject->Size; i++)
            {
                if (pobject->Size == 1)
                    sprintf(Buf, "MXDWORD %s : %lu\n", pobject->Name, *(DWORD *)MXGetValue(pmessage, pobject->Name, 1));
                else
                    sprintf(Buf, "MXDWORD %s[%d] : %lu\n", pobject->Name, i + 1, *(DWORD *)MXGetValue(pmessage, pobject->Name, i + 1));
                printf(Buf);
            }
            break;
		case MXLONG :
			for (i = 0; i < pobject->Size; i++)
			{
				if (pobject->Size == 1)
					sprintf (Buf, "MXLONG %s : %ld\n", pobject->Name, *(LONG*)MXGetValue (pmessage,  pobject->Name, 1));
				else
					sprintf (Buf, "MXLONG %s[%d] : %ld\n", pobject->Name,  i + 1, *(LONG*)MXGetValue (pmessage,  pobject->Name, i + 1));
				printf (Buf);
			}
			break;
		case MXDOUBLE :
			for (i = 0; i < pobject->Size; i++)
			{
				if (pobject->Size == 1)
					sprintf (Buf, "MXDOUBLE %s : %f\n", pobject->Name, *(DOUBLE*)MXGetValue (pmessage,  pobject->Name, 1));
				else
					sprintf (Buf, "MXDOUBLE %s[%d] : %f\n", pobject->Name, i + 1, *(DOUBLE*)MXGetValue (pmessage,  pobject->Name, i + 1));
				printf (Buf);
			}
			break;
		case MXBYTE :
        case MXCHAR:
			for (i = 0; i < pobject->Size; i++)
			{
				if (pobject->Size == 1)
					sprintf (Buf, "MXBYTE %s : %c\n", pobject->Name, *(CHAR*)MXGetValue (pmessage,  pobject->Name, 1));
				else
					sprintf (Buf, "MXBYTE %s[%d] : %c\n", pobject->Name, i + 1, *(CHAR*)MXGetValue (pmessage,  pobject->Name, i + 1));
				printf (Buf);
			}
			break;
		case MXFILE :
			for (i = 0; i < pobject->Size; i++)
			{
				if (pobject->Size == 1)
			sprintf (Buf, "MXFILE %s : %p\n", pobject->Name, MXGetValue (pmessage,  pobject->Name, 1));
				else
			sprintf (Buf, "MXFILE %s[%d] : %p\n", pobject->Name, i + 1, MXGetValue (pmessage,  pobject->Name, i + 1));

			printf (Buf);
			}
			break;
		case MXBUFFER :
			for (i = 0; i < pobject->Size; i++)
			{
				if (pobject->Size == 1)
					sprintf (Buf, "MXBUFFER %s : %p\n", pobject->Name, MXGetValue (pmessage,  pobject->Name, 1));
				else
					sprintf (Buf, "MXBUFFER %s[%d] : %p\n", pobject->Name, i + 1, MXGetValue (pmessage,  pobject->Name, i + 1));
				printf (Buf);
			}
			break;
		case MXSTRING :
			for (i = 0; i < pobject->Size; i++)
			{
				if (pobject->Size == 1)
					sprintf (Buf, "MXSTRING %s : %s\n", pobject->Name, (STRING)MXGetValue (pmessage,  pobject->Name, 1));
				else
					sprintf (Buf, "MXSTRING %s[%d] : %s\n", pobject->Name, i + 1, (STRING)MXGetValue (pmessage,  pobject->Name, i + 1));
				printf (Buf);
			}

			break;
		default:
			break;
		}
		Objects = Objects->cdr;
	}


	sprintf (Buf, "Transfer Date : %d\n", pmessage->TDate);
	printf (Buf);
	sprintf (Buf, "Transfer Hour : %d\n", pmessage->THour);
	printf (Buf);

	sprintf (Buf, "Put Date : %d\n", pmessage->PDate);
	printf (Buf);
	sprintf (Buf, "Put Hour : %d\n", pmessage->PHour);
	printf (Buf);

	sprintf (Buf, "Departure Date : %d\n", pmessage->DDate);
	printf (Buf);
	sprintf (Buf, "Departure Hour : %d\n", pmessage->DHour);
	printf (Buf);
	
	sprintf (Buf, "Arrival Date : %d\n", pmessage->ADate);
	printf (Buf);
	sprintf (Buf, "Transfer Hour : %d\n\n", pmessage->AHour);
	printf (Buf);
	
}

void MXFreeMessage (MX* pmx, MXMessage* pmessage)
{
    if (!pmessage) return;
//	MXTraceMessage (pmessage);
    if (pmessage->Context) MXFreeContext (pmessage->Context);

    if (pmessage->Stream)  
        free ((BYTE*)pmessage->Stream);
    if (pmessage->Values)  
        free ((BYTE*)pmessage->Values);
    free ((BYTE*)pmessage);
}

void MXInitMessage (MXMessage* pmessage)
{
//    pmessage->Offset = 0;
    if (pmessage->Complete && pmessage->Context)
    {
	    pmessage->Offset = 0;
        pmessage->Part = 0;
  //      MXFreeContext (pmessage->Context);
  //      pmessage->Context = NULL;
    }

}

int MXInitValues (MXMessage* pmessage)
{
    MXObject*  pobject;
    MXMessageClass* pclassmess = pmessage->MessageClass;
    List* Objects = pclassmess->Objects;
    int Range;
    
    while (Objects)
    {
        pobject = (MXObject*)Objects->car;
        switch (pobject->Type)
        {
        case MXSTRING :
            for (Range = 1; Range <= pobject->Size; Range++)
                MXSetValue (pmessage, pobject->Name, Range, "");
            break;
        case MXSHORT :
            {
                SHORT value = 0;
                for (Range = 1; Range <= pobject->Size; Range++)
                    MXSetValue (pmessage, pobject->Name, Range, &value);
            }
            break;
        case MXWORD :
            {
                WORD value = 0;
                for (Range = 1; Range <= pobject->Size; Range++)
                    MXSetValue (pmessage, pobject->Name, Range, &value);
            }
            break;
        case MXDWORD :
            {
                DWORD value = 0;
                for (Range = 1; Range <= pobject->Size; Range++)
                    MXSetValue (pmessage, pobject->Name, Range, &value);
            }
            break;
        case MXLONG :
            {
                LONG value = 0;
                for (Range = 1; Range <= pobject->Size; Range++)
                    MXSetValue (pmessage, pobject->Name, Range, &value);
            }
            break;
        case MXDOUBLE :
            {
                DOUBLE value = 0;
                for (Range = 1; Range <= pobject->Size; Range++)
                    MXSetValue (pmessage, pobject->Name, Range, &value);
            }
            break;
        case MXBYTE :
        case MXCHAR :
            {
                char value[3000];
                memset (value, ' ', pobject->Size );
                memset (pmessage->Stream + pmessage->Offset, ' ', pobject->Size);
            }
            break;
        case MXFILE :
            {
                FILEPARM value;
                memset (&value, 0, sizeof(FILEPARM));
                for (Range = 1; Range <= pobject->Size; Range++)
                    MXSetValue (pmessage, pobject->Name, Range, &value);
            }

            break;
        case MXBUFFER :
            {
                BUFFERPARM value;
                memset (&value, 0, sizeof(BUFFERPARM));
                for (Range = 1; Range <= pobject->Size; Range++)
                    MXSetValue (pmessage, pobject->Name, Range, &value);
            }

            break;
        }
        Objects = Objects->cdr;
    }
    return 1;
}
/*==================================================================================*/
/* functions related to files and buffers*/
/*==================================================================================*/

DWORD MXExtendValueBuffer (MXMessage* pmessage, DWORD value)
{
    if (value == 0) return 0;
    pmessage->Stream = (char*)realloc (pmessage->Stream, pmessage->StreamSize + value);
    if (!pmessage->Stream)
    {
        OSError ("TEDAG003", NULL);
        return 0;
    }
    memset (pmessage->Stream + pmessage->StreamSize , (int)0, value);
    pmessage->StreamSize += value;
    return pmessage->StreamSize;
}

int MXSetFileValue (MXMessage* pmessage , void* Value)
{

    FILEPARM* filestream = (FILEPARM*)Value;
    char* stream = pmessage->Stream + pmessage->Offset;
    int size = 0;
    LONG error = 0;
    DWORD filesize = 0;

    strncpy (stream , filestream->FileOri, FILENAMESIZE);
    size = FILENAMESIZE;
    stream += size;

    strncpy (stream  , filestream->FileDest, FILENAMESIZE);
    stream += size;

    memcpy (stream , (CHAR *)(&filestream->FileOriLocal) , sizeof (CHAR));
    stream += sizeof (CHAR);
    memcpy (stream , (CHAR *)(&filestream->FileDestLocal) , sizeof (CHAR));
    stream += sizeof (CHAR);
    memcpy (stream , (CHAR *)(&filestream->FileType) , sizeof (CHAR));
    stream += sizeof (CHAR);
    memcpy (stream , (CHAR *)(&filesize) , sizeof (DWORD));
    stream += sizeof (DWORD);
    memcpy (stream , (CHAR *)(&error) , sizeof (LONG));
    stream += sizeof (LONG);
    return (int)(stream - (pmessage->Stream + pmessage->Offset));
}

int MXSetStringValue (MXMessage* pmessage,  void* Value)
{

    char* stream;
    int gotnow = pmessage->Offset;
    DWORD needed = gotnow +  (int)strlen (Value) + 1;

    /*check if there is enough space */
    if (needed > pmessage->StreamSize)
        if (needed > MAXBUFSIZE)
        {
            OSError ("TEDAG004", NULL);
            return -1;
        }
        else
        {
            /*extra MXBUFVALUESIZE*/
            if (MXExtendValueBuffer (pmessage, needed - pmessage->StreamSize + MXBUFVALUESIZE) == 0)
                return -1;

        }
    stream = pmessage->Stream + gotnow;
    memcpy (stream , Value , strlen (Value) + 1);
    return ((int)strlen (Value) + 1);
}

int MXSetBufferValue (MXMessage* pmessage,  void* Value)
{

    BUFFERPARM* bufferstream = (BUFFERPARM*)Value;
    char* stream;
    int gotnow = pmessage->Offset;
    DWORD needed = gotnow +  bufferstream->BufferSize + sizeof (DWORD) + sizeof (CHAR);

    if (needed > pmessage->StreamSize)
        if (needed > MAXBUFSIZE)
        {
            OSError ("TEDAG004", NULL);
            return -1;
        }
        else
            if (MXExtendValueBuffer (pmessage, needed - pmessage->StreamSize + MXBUFVALUESIZE) == 0)
                return -1;

    /*copy now */
    stream = pmessage->Stream + gotnow;
    memcpy (stream , (CHAR *)(&bufferstream->BufferSize) , sizeof (DWORD));
    stream += 4;
    memcpy (stream , (CHAR *)(&bufferstream->BufferType) , sizeof (CHAR));
    stream += 1;
    memcpy (stream , bufferstream->BufferContent , bufferstream->BufferSize);
    return (bufferstream->BufferSize + sizeof (DWORD) + sizeof (CHAR));
}

/* return the whole file size with its content if Content != 0*/
/* else it returns the file structure size that's all */

LONG MXGetBufferSize (char* fromstream, BYTE Content)
{
    DWORD bufsize = sizeof (DWORD) + sizeof (CHAR);
    if (Content)
    {
        DWORD contentsize;
        OSCopy4 (&contentsize, fromstream);
        bufsize += contentsize;
    }
    return (LONG)bufsize;
}

LONG MXGetFileSize (char* fromstream, BYTE Content)
{
    DWORD size = 0;
    DWORD minsize;
    LONG filesize = 0;
    CHAR filetype;
    CHAR filelocal;
    int error = 0;

    /* open file for having its size */
    if (Content)
    {
        filetype  = *(fromstream + (2 * FILENAMESIZE) + (2 * sizeof (CHAR)));
        filelocal = *(fromstream + (2 * FILENAMESIZE));
        filesize = OSGetFileSize (fromstream, filetype, filelocal, &error);
        if ( (filesize < 0) ||(error != 0) )
        {
            OSCopy4 (fromstream + (2 * FILENAMESIZE) + (3 * sizeof (CHAR)), (char*)&filesize);
            OSCopy4 (fromstream + (2 * FILENAMESIZE) + sizeof (DWORD) + (3 * sizeof (CHAR)), (char*)&error);
            filesize = 0;
        }
        OSCopy4 (fromstream + (2 * FILENAMESIZE) + (3 * sizeof (CHAR)), (char*)&filesize);
    }
    /* minimum structure size without file content */
    minsize =  (2 * FILENAMESIZE) + (3 * sizeof (CHAR)) + sizeof (DWORD) + sizeof (LONG);

    if (Content)
        size += filesize;
    size += minsize;
    return (LONG)size;
}

LONG MXGetMessageSize (MXMessage* pmessage, BYTE WithFiles)
{
    DWORD size = 0;
    int j;
    List* list;
    MXObject* pobject;
    MXMessageClass* pclassmess;
    BYTE type;
    BYTE WithContent = 1;
    char* stream;


    pclassmess = pmessage->MessageClass;

    list= pclassmess->Objects;

    while (list)
    {
        pobject = (MXObject *)list->car;

        type = pobject->Type;
        switch (type)
        {
        case MXSHORT :
            size += pobject->Size * sizeof (SHORT);
            break;
        case MXWORD :
            size += pobject->Size * sizeof (WORD);
            break;
        case MXDWORD :
        case MXLONG :
            size += pobject->Size * sizeof (DWORD);
            break;
        case MXDOUBLE :
            size += pobject->Size * sizeof (DOUBLE);
            break;
        case MXBYTE :
        case MXCHAR :
            size += pobject->Size * sizeof (BYTE);
            break;
        case MXFILE :
            for (j = 0; j < pobject->Size; j++)
            {
                stream = (char*)(pmessage->Stream + pmessage->Values[(int)pobject->Offset + j]);
                size += MXGetFileSize (stream, WithFiles);
            }
            break;
        case MXBUFFER :
            for (j = 0; j < pobject->Size; j++)
            {
                stream = (char*)(pmessage->Stream + pmessage->Values[(int)pobject->Offset + j]);
                size += MXGetBufferSize (stream, WithContent);
            }
            break;
        case MXSTRING :
            for (j = 0; j < pobject->Size; j++)
            {
                stream = (char*) (pmessage->Stream + pmessage->Values[(int)pobject->Offset + j]);
                if (stream)   size += (int)strlen (stream) + 1;
                else size++;   /* empty string  goes in in SendMessage*/
            }
            break;
        default:
            OSError ("TEDAG006", NULL);
            return -1;

        }
        list = list->cdr;
    }
    return (LONG)size;
}

LONG MXGetMessageClassSize (MXMessageClass* pclassmess)
{
    DWORD size = 0;
    List* list;
    MXObject* pobject;
    BYTE type;

    list= pclassmess->Objects;

    while (list)
    {
        pobject = (MXObject *)list->car;

        type = pobject->Type;
        switch (type)
        {
        case MXSHORT :
            size += pobject->Size * sizeof (SHORT);
            break;
        case MXWORD :
            size += pobject->Size * sizeof (WORD);
            break;
        case MXDWORD :
        case MXLONG :
            size += pobject->Size * sizeof (DWORD);
            break;
        case MXDOUBLE :
            size += pobject->Size * sizeof (DOUBLE);
            break;
        case MXBYTE :
        case MXCHAR :
            size += pobject->Size * sizeof (BYTE);
            break;
        case MXFILE :
            break;
        case MXBUFFER :
            break;
        case MXSTRING :
            break;
        default:
            OSError ("TEDAG006", NULL);
            return -1;

        }
        list = list->cdr;
    }
    return (LONG)size;
}

/*-----FIND PROCEDURES -------*/

MXApplicationProcedure* MXFindApplicationProcedure (List* ExecuteList, int (*funct)(MX*, void*), void* appfield)
{
    int found = 0;
    List* plist = ExecuteList;
    MXApplicationProcedure* pexecuteprocedure = NULL;


    while ((found == 0) && (plist != NULL))
    {
        pexecuteprocedure = (MXApplicationProcedure*)plist->car;

        if ((pexecuteprocedure->ExecuteProcedure == funct) &&
                (pexecuteprocedure->ApplicationField == appfield))
            found = 1;
        else plist = plist->cdr;
    }

	if (found == 1) return pexecuteprocedure;
	else return NULL;
}

MXComCallBack* MXFindComCallBack (List* ExecuteList, int (*funct)(MXCom*, void*), void* appfield)
{
    int found = 0;
    List* plist = ExecuteList;
    MXComCallBack* pexecuteprocedure = NULL;


    while ((found == 0) && (plist != NULL))
    {
        pexecuteprocedure = (MXComCallBack*)plist->car;

        if ((pexecuteprocedure->ExecuteProcedure == funct) &&
                (pexecuteprocedure->ApplicationField == appfield))
            found = 1;
        else plist = plist->cdr;
    }

	if (found == 1) return pexecuteprocedure;
	else return NULL;

}

MXCallBack* MXFindGeneralCallBack (List* ExecuteList, int (*funct)(MXMessage*, MXCom* pcom,void*), void* appfield)
{
    int found = 0;
    List* plist = ExecuteList;
    MXCallBack* pexecuteprocedure = NULL;


    while ((found == 0) && (plist != NULL))
    {
        pexecuteprocedure = (MXCallBack*)plist->car;

        if ((pexecuteprocedure->ExecuteProcedure == funct) &&
                (pexecuteprocedure->ApplicationField == appfield))
            found = 1;
        else plist = plist->cdr;
    }
	if (found == 1) return pexecuteprocedure;
	else return NULL;
}

MXCallBack* MXFindCallBack (MXMessageClass* pmessclass, List* ExecuteList, int (*funct)(MXMessage*,MXCom* pcom, void*), void* appfield)
{
    int found = 0;
    List* plist = ExecuteList;
    MXCallBack* pexecuteprocedure = NULL;


    while ((found == 0) && (plist != NULL))
    {
        pexecuteprocedure = (MXCallBack*)plist->car;

        if ((pexecuteprocedure->ExecuteProcedure == funct) &&
                (pexecuteprocedure->ApplicationField == appfield) &&
                (pexecuteprocedure->MessageClass == pmessclass))
            found = 1;
        else plist = plist->cdr;
    }
	if (found == 1) return pexecuteprocedure;
	else return NULL;

}

int MXGetProtocolFromName (char* Name)
{
	if (OSStrcmp (Name, "DG") == 0) 	return IPPROTO_DG;
	if (OSStrcmp (Name, "CB2A") == 0)	return IPPROTO_CB2A;  
	if (OSStrcmp (Name, "TP") == 0)		return IPPROTO_BC;       
	if (OSStrcmp (Name, "UDP") == 0)	return IPPROTO_UDP ;           
	if (OSStrcmp (Name, "TCP") == 0)	return IPPROTO_TCP;              
	if (OSStrcmp (Name, "HTTP") == 0)	return IPPROTO_HTTP;                  
	if (OSStrcmp (Name, "DNS") == 0)	return IPPROTO_DNS;                       
	if (OSStrcmp (Name, "ICMP") == 0)	return IPPROTO_ICMP;                           
	if (OSStrcmp (Name, "FTP") == 0)	return IPPROTO_FTP;                              
	if (OSStrcmp (Name, "SMTP") == 0)	return IPPROTO_SMTP;                                 
	if (OSStrcmp (Name, "POP") == 0)	return IPPROTO_POP;                                    
	if (OSStrcmp (Name, "ODBC") == 0)	return DBPROTO_ODBC;                                          
	if (OSStrcmp (Name, "SQLDS") == 0)	return DBPROTO_SQLDS;                                             
	if (OSStrcmp (Name, "ORACLE") == 0)	return DBPROTO_ORACLE;                                                  
	if (OSStrcmp (Name, "FIXED") == 0)	return IOPROTO_FIOP;                                                      
	if (OSStrcmp (Name, "STORE") == 0)	return IOPROTO_STORE;														
	if (OSStrcmp (Name, "MT") == 0)		return IOPROTO_MTT;
    return 0;
}

MXMessage* MXFindMessage (List* meslist, WORD Identity)
{
    int found = 0;
    List* plist = meslist;
    MXMessage* pmessage = NULL;

    while ((found == 0) && (plist != NULL))
    {
        pmessage = (MXMessage*)plist->car;

        if (pmessage->Identity == Identity)

            found = 1;
        else plist = plist->cdr;
    };
    return pmessage;
}

MXMessage* MXReadyToSend (MXMessage* pmessage)
{
    DWORD Date, Time;

    OSDateHour (&Date, &Time);
    if (pmessage->TDate <= Date && pmessage->THour <= Time)
        return pmessage;
    else return NULL;
}

MXMessage* MXFindIncompleteMessage (List* meslist)
{
    int found = 0;
    List* plist = meslist;
    MXMessage* pmessage = NULL;
    MXMessage* pwaitmessage = NULL;

    while ((found == 0) && (plist != NULL))
    {
        pmessage = (MXMessage*)plist->car;

        if (!pmessage->Complete)
            if (MXReadyToSend (pmessage))
                found = 1;
            else if (!pwaitmessage) pwaitmessage = pmessage;
        plist = plist->cdr;
    };
    if (found) return pmessage;
    if (pwaitmessage) return pwaitmessage;
    return NULL;
}

MXMessage* MXGetReadyInputMessage (MXCom* pcom)
{
    List* l = pcom->InputMessages;
    MXMessage* pmessage;

    while (l)
    {
        pmessage = (MXMessage*)l->car;
        if (pmessage->Complete) return pmessage;
        l = l->cdr;
    }
    return NULL;
}

MXMessage* MXGetReadyOutputMessage (MXCom* pcom)
{

    MXMessage* pmessage;

    /* no output messages */
    if (!pcom->OutputMessages) return NULL;

    pmessage = MXFindIncompleteMessage (pcom->OutputMessages);

    /* all messages are complete */

    if (!pmessage) return NULL;
    OSDateHour (&pmessage->DDate, &pmessage->DHour);

    return pmessage;
}

/* CREATE message */

void MXFreeAndMoveMessageIn (List* plist,  MXMessage* pmessage)
{

    if (pmessage->Context)
        MXFreeContext (pmessage->Context);

    pmessage->Context = NULL;

    if (pmessage->Stream)
        free ((BYTE*)pmessage->Stream);

    pmessage->Stream = NULL;

    if (pmessage->Values)
        free ((BYTE*)pmessage->Values);

    pmessage->Values = NULL;
    ListNewr (&plist, pmessage);     /* put it here fro trace */
}

/* this is called by basecom whe client is killed*/

int MXFreeAndMoveComIn (TPClient* pclient, void* par)
{
    MXCom* pcom = (MXCom*)par;
    MXComCallBack*  pendingprocedure;
    MXCallBack*  pexecutecomprocedure;
    MXMessage* pmessage;
    List* pt;

    /* on execute les Endingprocedures  */
    pt = pcom->EndingProcedures;
    while (pt)
    {
        pendingprocedure = (MXComCallBack *)pt->car;
        pt = pt->cdr;
        pendingprocedure->ExecuteProcedure (pcom, pendingprocedure->ApplicationField);
    }

    /*Class Ending CallBack */
    if (pcom->ComClass != NULL)
    {
        pt = pcom->ComClass->EndingProcedures;
        while (pt)
        {
            pendingprocedure = (MXComCallBack *)pt->car;
            pt = pt->cdr;
            pendingprocedure->ExecuteProcedure (pcom, pendingprocedure->ApplicationField);
        }
    }

    /*General Ending CallBack */
    pt = pcom->MX->EndingProcedures;
    while (pt)
    {
        pendingprocedure = (MXComCallBack *)pt->car;
        pt = pt->cdr;
        pendingprocedure->ExecuteProcedure (pcom, pendingprocedure->ApplicationField);
    }

    /* on lib�re les EndingProcedures */
    while (pcom->EndingProcedures)
    {
        pendingprocedure = (MXComCallBack *)pcom->EndingProcedures->car;
        ListRemove (&pcom->EndingProcedures, pendingprocedure);
        free (pendingprocedure);
    }
    while (pcom->ExecuteOutputs)
    {
        pexecutecomprocedure = (MXCallBack *)pcom->ExecuteOutputs->car;
        ListRemove (&pcom->ExecuteOutputs, pexecutecomprocedure);
        free (pexecutecomprocedure);
    }
    while (pcom->ExecuteInputs)
    {
        pexecutecomprocedure = (MXCallBack *)pcom->ExecuteInputs->car;
        ListRemove (&pcom->ExecuteInputs, pexecutecomprocedure);
        free (pexecutecomprocedure);
    }
    while (pcom->ExecuteOutputStream)
    {
        pexecutecomprocedure = (MXCallBack *)pcom->ExecuteOutputStream->car;
        ListRemove (&pcom->ExecuteOutputStream, pexecutecomprocedure);
        free (pexecutecomprocedure);
    }
    while (pcom->ExecuteInputStream)
    {
        pexecutecomprocedure = (MXCallBack *)pcom->ExecuteInputStream->car;
        ListRemove (&pcom->ExecuteInputStream, pexecutecomprocedure);
        free (pexecutecomprocedure);
    }
    while (pcom->ExecuteAck)
    {
        pexecutecomprocedure = (MXCallBack *)pcom->ExecuteAck->car;
        ListRemove (&pcom->ExecuteAck, pexecutecomprocedure);
        free (pexecutecomprocedure);
    }
    pcom->Client = NULL;

    /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!LIBERER LES MESSAGES DANS OUTPUT ET INPUT et ACK*/

    while (pcom->OutputMessages)
    {
        pmessage = (MXMessage*)pcom->OutputMessages->car;
        ListRemove (&pcom->OutputMessages, pmessage);
        MXFreeMessage (pcom->MX, pmessage);
    }

    while (pcom->InputMessages)
    {
        pmessage = (MXMessage*)pcom->InputMessages->car;
        ListRemove (&pcom->InputMessages, pmessage);
        MXFreeMessage (pcom->MX, pmessage);
    }
    while (pcom->AckMessages)
    {
        pmessage = (MXMessage*)pcom->AckMessages->car;
        ListRemove (&pcom->AckMessages, pmessage);
        MXFreeMessage (pcom->MX, pmessage);
    }
    while (pcom->ActiveOutputMessages)
    {
        pmessage = (MXMessage*)pcom->ActiveOutputMessages->car;
        ListRemove (&pcom->ActiveOutputMessages, pmessage);
        MXFreeMessage (pcom->MX, pmessage);
    }
    while (pcom->ActiveInputMessages)
    {
        pmessage = (MXMessage*)pcom->ActiveInputMessages->car;
        ListRemove (&pcom->ActiveInputMessages, pmessage);
        MXFreeMessage (pcom->MX, pmessage);
    }
    


    pcom->CloseTime = time (NULL);

    ListRemove (&(pcom->MX->Communications), pcom);

    ListNewr (&(pcom->MX->ComHistory), pcom);
    (pcom->MX->ComHistoryNumber)++;
    pcom->Closed = 1;
    if (pcom->MX->ComHistoryNumber > MXNumberComInHistory)
    {
        MXCom* RemoveCom = (MXCom *)pcom->MX->ComHistory->car;
        ListRemove (&(pcom->MX->ComHistory), RemoveCom);
        MXFreeCom (pcom->MX, RemoveCom);
        (pcom->MX->ComHistoryNumber)--;
    }
    return 1;
}

int MXExecuteApplicationProcedures (TP* ptp, void* app)
{

    List* plist;
    MX* pmx = (MX *)app;
    BOOL DontBlock;
    MXApplicationProcedure* AppProc;

    DontBlock = FALSE;
    plist = pmx->ApplicationProcedures;
    while (plist)
    {
        AppProc = (MXApplicationProcedure *)plist->car;
        plist = plist->cdr;
        if (AppProc->ExecuteProcedure(pmx, AppProc->ApplicationField) != 0)
            DontBlock = TRUE;
    }
    if (DontBlock) return 1;
    else return 0;
}

int MXEstablishConnectionOn (TPClient* pclient, void* app)
{
    MXCom* pcom;
    List* plist;
    MXComCallBack* ConnectionProc;
    MX* pmx = (MX *)app;

    if (!pclient) return -1;

    pcom = MXGetComFromClient (pmx, pclient);
    if (!pcom)  pcom =  MXSetCom (pmx, NULL, pclient, pclient->Protocol);

    plist = pmx->ConnectionProcedures;
    while (plist)
    {
        ConnectionProc = (MXComCallBack *)plist->car;
        plist = plist->cdr;
        ConnectionProc->ExecuteProcedure(pcom, ConnectionProc->ApplicationField);
    }
    return 1;
}

MXCom* MXGetComFromClient (MX* pmx, TPClient* pclient)
{
    List* list = pmx->Communications;
    MXCom* pcom;

    if (!pclient) return NULL;
    while (list)
    {
        pcom = (MXCom *) list->car;
        if (pcom->Client == pclient)  return pcom;
        list = list->cdr;
    }
    return NULL;
}

MXCom* MXSetCom (MX* pmx, MXComClass* pcomclass, TPClient* pclient, int protocol)
{
    MXCom* pcom;

    pcom = (MXCom*)malloc (sizeof (MXCom));
    if (!pcom)
    {
        OSError ("TEDAG021", NULL);
        return NULL;
    }
    pcom->ComClass = pcomclass;
    pcom->XF = NULL;
    pcom->OutputMessages = pcom->InputMessages = pcom->AckMessages = NULL;
    pcom->ActiveOutputMessages = pcom->ActiveInputMessages = NULL;
    pcom->InputHistory   = pcom->OutputHistory = NULL;
    pcom->MX = pmx;
    pcom->MessagesId = 0;
    pcom->EndingProcedures = NULL;
    pcom->ExecuteOutputs = NULL;
    pcom->ExecuteInputs = NULL;
    pcom->ExecuteOutputStream = NULL;
    pcom->ExecuteInputStream = NULL;
    pcom->ExecuteAck = NULL;
    pcom->TraceProcedure   = NULL;
    pcom->ApplicationField = NULL;
    pcom->InternalField    = NULL;
    pcom->IOMessageClass   = NULL;

    pcom->Closed = 0;
    pcom->OpenTime = pclient->CommunicationTime;
    pcom->Client   = pclient;
    TPSetApplicationField (pclient, pcom);
    TPSetEndingProcedure (pclient, MXFreeAndMoveComIn);

    ListNewr (&pmx->Communications, pcom);

    if (pcomclass)
    {
        MXSetComProtocol (pmx, pcom, pcomclass->Protocol);
    }
    else
        MXSetComProtocol (pmx, pcom, protocol);
    return pcom;
}

int MXCheckType (char* buf)
{
    if (!strcmp (buf, "SHORT")) return MXSHORT;
    else  if (!strcmp (buf, "WORD")) return MXWORD;
    else  if (!strcmp (buf, "DWORD")) return MXDWORD;
    else  if (!strcmp (buf, "BYTE")) return MXBYTE;
    else  if (!strcmp (buf, "FILE")) return MXFILE;
    else  if (!strcmp (buf, "STRING")) return MXSTRING;
    else  if (!strcmp (buf, "CHAR")) return MXCHAR;
    else  if (!strcmp (buf, "BUFFER")) return MXBUFFER;
    else  if (!strcmp (buf, "LONG")) return MXLONG;
    else  return -1;
}

int MXCheckType2 (char* buf)
{
    if (!strcmp (buf, "SHORT")) return MXSHORT;
    else  if (!strcmp (buf, "WORD")) return MXWORD;
    else  if (!strcmp (buf, "DWORD")) return MXDWORD;
    else  if (!strcmp (buf, "DOUBLE")) return MXDOUBLE;
    else  if (!strcmp (buf, "BYTE")) return MXBYTE;
    else  if (!strcmp (buf, "FILE")) return MXFILE;
    else  if (!strcmp (buf, "STRING")) return MXSTRING;
    else  if (!strcmp (buf, "CHAR")) return MXCHAR;
    else  if (!strcmp (buf, "BUFFER")) return MXBUFFER;
    else  if (!strcmp (buf, "LONG")) return MXLONG;

    if (!strcmp (buf, "SHORT_T")) return MXSHORT+10;
    else  if (!strcmp (buf, "WORD_T")) return MXWORD+10;
    else  if (!strcmp (buf, "DWORD_T")) return MXDWORD+10;
    else  if (!strcmp (buf, "BYTE_T")) return MXBYTE+10;
    else  if (!strcmp (buf, "FILE_T")) return MXFILE+10;
    else  if (!strcmp (buf, "STRING_T")) return MXSTRING+10;
    else  if (!strcmp (buf, "CHAR_T")) return MXCHAR+10;
    else  if (!strcmp (buf, "BUFFER_T")) return MXBUFFER+10;
    else  if (!strcmp (buf, "LONG_T")) return MXLONG+10;
    else  return -1;
}

MXMessageContext* MXAllocateContext (void)
{
    MXMessageContext* pcontext = NULL;
    pcontext = (MXMessageContext*)malloc (sizeof (MXMessageContext));
    if (pcontext)
    {

        pcontext->File = NULL;
        pcontext->Stream = NULL;
        pcontext->Buffer = NULL;
        pcontext->Pos = 0;
        pcontext->StreamPos = 0;
        pcontext->StreamSize = 0;
        pcontext->ObjectOffset = 0;
        pcontext->SizeOffset = 0;
        pcontext->StillToRead = 0;
    }
    else
        OSError ("TEDAG026", NULL);
    return pcontext;
}

void MXFreeContext (MXMessageContext* pcontext)
{
    if (pcontext->Stream)
        free ((BYTE*)pcontext->Stream);
    free ((BYTE*)pcontext);
}

int MXReadMessageStatut (MX* pmx, char* stream, MXMessageStatut* pstatut)
{

    LONG from = 0;
    XF* pxf = pmx->TP->XF;
    char* fromstream = stream + MXMessageHeaderSize + sizeof (WORD);
    /*Dates & hours */

    from = 0;
    fromstream += XFByteRead (pxf, (char*)(&pstatut->Type), &from, fromstream, 1);
    from = 0;
    fromstream += XFDWordRead (pxf, (char*)(&pstatut->TDate), &from, fromstream, 1);
    from = 0;
    fromstream += XFDWordRead (pxf, (char*)(&pstatut->THour), &from, fromstream, 1);
    from = 0;
    fromstream += XFDWordRead (pxf, (char*)(&pstatut->PDate), &from, fromstream, 1);
    from = 0;
    fromstream += XFDWordRead (pxf, (char*)(&pstatut->PHour), &from, fromstream, 1);
    from = 0;
    fromstream += XFDWordRead (pxf, (char*)(&pstatut->DDate), &from, fromstream, 1);
    from = 0;
    fromstream += XFDWordRead (pxf, (char*)(&pstatut->DHour), &from, fromstream, 1);
    return MXMessageStatutSize;
}

/*====================================================================================*/
/* Functions related to buffer */

int MXWriteBufferContent (MX* pmx, MXMessageContext* pcontext, char* stream, char* attributes, int maxcount)
{
    DWORD buffersize;
    int tocopy;
    CHAR buffertype;
    LONG from = 0;

    if (!pcontext->Buffer)
    {
        pcontext->Pos = 0;
    }

    pcontext->Buffer = attributes + sizeof (DWORD) + sizeof (CHAR);
    OSCopy4(&buffersize, attributes);
    buffertype = *(CHAR *)(attributes + 4);


    if (buffersize == 0)       /*nothing to read */
    {
        pcontext->Buffer = NULL;
        return 0;
    }

    if (maxcount == 0)        /* I can't read , read next time */
        return 0;


    if (buffersize - pcontext->Pos > (DWORD)maxcount)
        tocopy = maxcount;
    else tocopy = (int)(buffersize - pcontext->Pos);

    /* now copy in buffer */

    if ((buffertype == 't') || (buffertype == 'T'))
        XFCharWrite (pmx->TP->XF, (char*) (pcontext->Buffer + pcontext->Pos) , &from, stream, tocopy);
    else
        memcpy (stream , (char*) (pcontext->Buffer + pcontext->Pos) , tocopy);

    pcontext->Pos += tocopy;


    if (pcontext->StillToRead != -1)   /* don't end the file */
        if ((DWORD)pcontext->Pos ==  buffersize)           /* means end of buffer */
        {
            pcontext->Buffer = NULL;
            pcontext->Pos = 0;
        }
    return tocopy;
}

int MXReadBufferContent (MX* pmx, MXMessageContext* pcontext, char* fromstream, char* attributes, int maxcount)
{

    DWORD buffersize;
    int towrite = 0;
    DWORD StillToRead;
    CHAR buffertype;
    LONG from = 0;

    OSCopy4(&buffersize, attributes);
    buffertype = *(CHAR *) (attributes + 4);


    if (!pcontext->Buffer)
    {
        pcontext->Pos = 0;
    }

    pcontext->Buffer = attributes + sizeof (DWORD) + sizeof (CHAR);
    if (buffersize == 0)       /*nothing to read */
    {
        pcontext->Buffer = NULL;
        return 0;
    }

    if (maxcount == 0)        /* I can't read , read next time */
        return 0;

    StillToRead = buffersize - pcontext->Pos;

    if (StillToRead <  (DWORD)maxcount)
        towrite = (int)StillToRead; /* no worry it is smaller */
    else towrite = maxcount;

    if ((buffertype == 't') || (buffertype == 'T'))
        XFCharRead (pmx->TP->XF, (char*) (pcontext->Buffer + pcontext->Pos) , &from, fromstream,  towrite );
    else
        memcpy ((char *)(pcontext->Buffer + pcontext->Pos), fromstream , towrite);

    pcontext->Pos += towrite;


    if (pcontext->StillToRead != -1)   /* don't end the file */
        if ((DWORD)pcontext->Pos ==  buffersize)           /* means end of file */
        {
            pcontext->Pos = 0;
            pcontext->Buffer = NULL;
        }
    return towrite;
}

/*====================================================================================*/
/* Functions related to file */

LONG MXReadFileContent (MX* pmx, MXMessageContext* pcontext, char* stream,  char* attributes, WORD maxcount)
{
    char* fromfilename;
    char* tofilename;
    LONG* toerror;
    FILEID* File;
    WORD size;
    int filesize;
    CHAR filetype;
    CHAR filelocal;
    WORD towrite = 0;
    LONG written, writefile, StillToWrite;
    DWORD StillToRead;
    LONG from = 0;
    int error = 0;
    LONG fileerror;
    BOOL transcod = FALSE;
    fromfilename = attributes;
    size = FILENAMESIZE;
    tofilename = fromfilename + size;

    OSCopy4 (&filesize, tofilename + size + (3 * sizeof (CHAR)));
    filetype = *(fromfilename + (2 * FILENAMESIZE) + (2 * sizeof (CHAR)));
    filelocal = *(fromfilename + (2 * FILENAMESIZE) + sizeof (CHAR));
    toerror = (LONG*)(fromfilename + (2 * FILENAMESIZE) + (3 * sizeof (CHAR)) + sizeof (DWORD));
    OSCopy4(&fileerror, (char*)toerror);


    /* insert date */

    if ((filetype == 'T') || (filetype == 't'))
    {
        filetype = 't';
        transcod = TRUE;
    }
    else if ((filetype == 'G') || (filetype == 'g'))
    {
        filetype = 'b';
        transcod = TRUE;
    }
    else filetype = 'b';


    if ((fileerror != 0) ||(filesize == 0))       /*nothing to read */
    {

        File = OSOpenFile (tofilename, "w", filetype, filelocal, &error);
        if (File )OSCloseFile (File);
        return 0;
    }

    if (!pcontext->File)      /* file is not opened  so open it*/
    {

        /**** ouverture suivant le type du fichier */
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


    StillToRead = filesize - pcontext->Pos;

    if (StillToRead < maxcount)
        towrite = (WORD)StillToRead;      /* no worry it is smaller */
    else towrite = maxcount;

    /* we use xf in type */
    if (pcontext->StreamSize + towrite > MAXBUFSIZE)
    {
        OSError ("TEDAG054", NULL);    /* disk is full i can't write */
        error = 10005;
        OSCopy4 ((void*)    toerror, (char*)&error);
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

    if (pcontext->Pos == filesize)           /* means end of file */
    {
        if (pcontext->StreamSize != 0)
        {
            pcontext->Stream[pcontext->StreamSize] = '\n';
            OSWriteFile (pcontext->File, pcontext->Stream , pcontext->StreamSize + 1 , 0, &error);
        }
        OSCloseFile (pcontext->File);
        pcontext->File = NULL;
    }
    return towrite;
}

LONG MXWriteFileContent (MX* pmx, MXMessageContext* pcontext, char* stream, char* attributes, WORD maxcount)
{
    char* fromfilename;
    char* tofilename;
    char* filebuf;
    WORD size;
    DWORD filesize;
    CHAR filetype;
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
    filetype = *(fromfilename + (2 * FILENAMESIZE) + (2 * sizeof (CHAR)));
    filelocal = *(fromfilename + (2 * FILENAMESIZE));

    if ((filetype == 'T') || (filetype == 't'))
    {
        filetype = 't';
        transcod = TRUE;
    }
    else if ((filetype == 'G') || (filetype == 'g'))
    {
        filetype = 'b';
        transcod = TRUE;
    }
    else filetype = 'b';



    if (filesize == 0)       /*nothing to read */
        return 0;

    if (!pcontext->File)      /* file is not opened  so open it*/
    {
        /**** ouverture suivant le type du fichier */

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
        if (transcod)
            XFCharWrite (pmx->TP->XF, filebuf , &from, stream, isread);
        else
            memcpy (stream , filebuf, isread);
    }

    while (isread < maxcount)  /* still place to fill buffer */
    {
        from = 0;
        readfile = OSReadFile (pcontext->File, pcontext->Stream, MXBUFFILESIZE,  &error);

        /*      printf ("read from file %d\n", readfile);*/
        if (readfile == 0)
        {
            OSCloseFile (pcontext->File);
            pcontext->File = NULL;
            break;
        }
        if ((DWORD) (isread + readfile) > maxcount)
        {
            if (transcod)
                XFCharWrite (pmx->TP->XF, pcontext->Stream , &from, stream + isread, maxcount - isread);
            else
                memcpy (stream + isread , pcontext->Stream, maxcount - isread);
            pcontext->StreamSize = readfile;
            pcontext->StreamPos = maxcount - isread;
            isread = maxcount;
        }
        else
        {
            if (transcod)
                XFCharWrite (pmx->TP->XF, pcontext->Stream , &from, stream + isread, (int)readfile);
            else
                memcpy (stream + isread , pcontext->Stream, (WORD)readfile);
            isread += readfile;
        }
    }
    return isread;
}

LONG MXSetIOMessageClass(MXCom* pcom,MXMessageClass* pmessageclass)
{

    if ((pcom) &&
            (pcom->Client) &&
            (pcom->Client->Pipe) &&
            (pcom->Client->Pipe->fileid))
        pcom->IOMessageClass = pmessageclass;
    pcom->Client->Pipe->fileid->LineSize = MXGetMessageClassSize (pmessageclass);
    return pcom->Client->Pipe->fileid->LineSize;
}

LONG MXSetIOSize(MXCom* pcom,LONG LineSize)
{

    if ((pcom) &&
            (pcom->Client) &&
            (pcom->Client->Pipe) &&
            (pcom->Client->Pipe->fileid))
        pcom->Client->Pipe->fileid->LineSize = LineSize;
    return LineSize;
}

char* MXGetName (MXMessage* pmessage, int Order)
{
    MXObject*  pobject;

    if (Order <= 0) return NULL;

    pobject = (MXObject*)ListGetElt (pmessage->MessageClass->Objects,  Order - 1);
    if (!pobject)
    {
        OSError ("TEDAG016", " (%d)", Order);
        return NULL;
    }
    return pobject->Name;
}

void MXGetType (MXMessage* pmessage, int Order, char* cType)
{
    MXObject*  pobject;

    if (Order <= 0) return;

    pobject = (MXObject*)ListGetElt (pmessage->MessageClass->Objects,  Order - 1);
    if (!pobject)
    {
        OSError ("TEDAG016", " (%d)", Order);
        return ;
    }

    switch (pobject->InternalType)
    {
    case OSCHAR :
        sprintf (cType, "%s[%d]", "CHAR" ,pobject->InternalLength);
        break;
    case INTEGER :
        sprintf (cType, "%s[%d]", "INTEGER" ,pobject->InternalLength);
        break;
    case OSDECIMAL :
        sprintf (cType, "%s[%d]", "DECIMAL" ,pobject->InternalLength);
        break;
    case OSDOUBLE :
        sprintf (cType, "%s[%d]","DOUBLE"  ,pobject->InternalLength);
        break;
    case OSBIT :
        sprintf (cType, "%s[%d]", "BIT" ,pobject->InternalLength);
        break;
    case OSFLOAT :
        sprintf (cType, "%s[%d]","FLOAT" ,pobject->InternalLength);
        break;
    case OSLONGVARBINARY :
        sprintf (cType, "%s[%d]","LONGVARBINARY" ,pobject->InternalLength);
        break;
    case OSNUMERIC :
        sprintf (cType, "%s[%d]", "NUMERIC" ,pobject->InternalLength);
        break;
    case OSTIMESTAMP :
        sprintf (cType, "%s[%d]","TIMESTAMP" ,pobject->InternalLength);
        break;
    case OSTIME :
        sprintf (cType, "%s[%d]", "TIME" ,pobject->InternalLength);
        break;
    case OSDATE :
        sprintf (cType, "%s[%d]", "DATE" ,pobject->InternalLength);
        break;
    case OSVARCHAR :
        sprintf (cType, "%s[%d]", "VARCHAR",pobject->InternalLength);
        break;
    case OSINTEGER :
        sprintf (cType, "%s[%d]","INTEGER" ,pobject->InternalLength);
        break;
    case OSLONGVARCHAR :
        sprintf (cType, "%s[%d]", "LONGVARCHAR",pobject->InternalLength);
        break;
    case OSREAL :
        sprintf (cType, "%s[%d]","REAL" ,pobject->InternalLength);
        break;
    case OSBIGINT :
        sprintf (cType, "%s[%d]", "BIGINT",pobject->InternalLength);
        break;
    case OSBINARY  :
        sprintf (cType, "%s[%d]", "BINARY",pobject->InternalLength);
        break;
    case SMALLINT :
        sprintf (cType, "%s[%d]", "SMALLINT",pobject->InternalLength);
        break;
    case OSTINYINT :
        sprintf (cType, "%s[%d]", "TINYINT",pobject->InternalLength);
        break;
    case OSVARBINARY :
        sprintf (cType, "%s[%d]", "VARBINARY",pobject->InternalLength);
        break;
    case OSSHORT :
        sprintf (cType, "%s[%d]", "SHORT",pobject->InternalLength);
        break;
    case OSWORD :
        sprintf (cType, "%s[%d]", "WORD",pobject->InternalLength);
        break;
    case OSBYTE :
        sprintf (cType, "%s[%d]", "BYTE",pobject->InternalLength);
        break;
    case OSSTRING :
        sprintf (cType, "%s[%d]", "STRING",pobject->InternalLength);
        break;
    case OSBUFFER :
        sprintf (cType, "%s[%d]", "BUFFER",pobject->InternalLength);
        break;
    case OSFILE :
        sprintf (cType, "%s[%d]", "FILE",pobject->InternalLength);
        break;
    case OSLONG :
        sprintf (cType, "%s[%d]", "LONG" ,pobject->InternalLength);
        break;
    }
    return ;
}

SHORT MXGetInternalType (MXMessage* pmessage,  int Order)
{
    MXObject*  pobject;

    if (Order <= 0) return -1;

    pobject = (MXObject*)ListGetElt (pmessage->MessageClass->Objects,  Order - 1);
    if (!pobject)
    {
        OSError ("TEDAG016", " (%d)", Order);
        return -1;
    }

    return pobject->InternalType;
}

LONG MXGetInternalLength (MXMessage* pmessage,  int Order)
{
    MXObject*  pobject;

    if (Order <= 0) return -1;

    pobject = (MXObject*)ListGetElt (pmessage->MessageClass->Objects,  Order - 1);
    if (!pobject)
    {
        OSError ("TEDAG016", " (%d)", Order);
        return -1;
    }

    return pobject->InternalLength;
}

LONG MXGetInternalScale (MXMessage* pmessage,  int Order)
{
    MXObject*  pobject;

    if (Order <= 0) return -1;

    pobject = (MXObject*)ListGetElt (pmessage->MessageClass->Objects,  Order - 1);
    if (!pobject)
    {
        OSError ("TEDAG016", " (%d)", Order);
        return -1;
    }

    return pobject->InternalScale;
}

void* MXGetRangeValue (MXMessage* pmessage, int Order, int Range)
{
    int size;
    MXObject*  pobject;
    char* stream;
    FILEPARM* file;
    BUFFERPARM* buffer;

    if (Order <= 0) return NULL;

    pobject = (MXObject*)ListGetElt (pmessage->MessageClass->Objects,  Order - 1);
    if (!pobject)
    {
        OSError ("TEDAG016", " (%d)", Order);
        return NULL;
    }
    if ((Range > pobject->Size) || (Range < 1))
    {
        OSError ("TEDAG017", " (%d)", Range);
        return NULL;
    }

    /* no range is possible for char */
    if (pobject->Type == MXCHAR) Range = 1;

    /* return value can be null */

    stream = (pmessage->Stream + pmessage->Values[(int)pobject->Offset + (Range - 1)]);

    if (pobject->Type == MXFILE)
    {
        /* special treatment for FILEPARM Structure to resolve alignment */
        file = (FILEPARM *)malloc (sizeof (FILEPARM));
        if (!file) return NULL;

        size = FILENAMESIZE;
        strncpy (file->FileOri, stream, size);
        stream += size;
        strncpy (file->FileDest, stream, size);
        stream += size;
        file->FileOriLocal = *(CHAR*)stream;
        stream +=  sizeof (CHAR);
        file->FileDestLocal = *(CHAR*)stream;
        stream +=  sizeof (CHAR);
        file->FileType = *(CHAR*)stream;
        stream +=  sizeof (CHAR);

        OSCopy4(&file->FileSize, stream);
        stream +=  sizeof (DWORD);
        OSCopy4(&file->FileError, stream);
        stream +=  sizeof (LONG);
        return (void*)file;
    }

    if (pobject->Type == MXBUFFER)
    {
        /* special treatment for BUFFERPARM Structure to resolve alignment */
        buffer = (BUFFERPARM *)malloc (sizeof (BUFFERPARM));
        if (!buffer) return NULL;

        OSCopy4(&buffer->BufferSize, stream);
        stream += sizeof (DWORD);
        buffer->BufferType = *(CHAR*)stream;
        stream += sizeof (CHAR);

        buffer->BufferContent = (void *)malloc (buffer->BufferSize);
        if (!buffer->BufferContent)
            return (void*)buffer;
        memcpy (buffer->BufferContent, stream, buffer->BufferSize);
        return (void*)buffer;
    }
    if ((pobject->Type == MXWORD)  ||
        (pobject->Type == MXSHORT))
    {
        WORD word;
        OSCopy2 (&word, stream);
        return (void*)stream;
    }
    else
        if ((pobject->Type == MXDWORD)  ||
            (pobject->Type == MXLONG))
        {
            DWORD dword;
            OSCopy4 (&dword, stream);
            return (void*)stream;
        }
        else
            if (pobject->Type == MXDOUBLE)
            {

                DOUBLE wdouble;
                OSCopy8 (&wdouble, stream);

                return (void*)stream;
            }
            else
                return (void*)(pmessage->Stream + pmessage->Values[(int)pobject->Offset + (Range - 1)]);
}

int MXSetRangeValue (MXMessage* pmessage, int Order, int Range, void* Value)
{
    int  size = 0;
    MXObject*  pobject;
    MXMessageClass* pclassmess = pmessage->MessageClass;
    char* tostream = pmessage->Stream + pmessage->Offset;

    if (Order <= 0) return -1;

    pobject = (MXObject*)ListGetElt (pmessage->MessageClass->Objects,  Order - 1);

    if (!pobject)
    {
        OSError ("TEDAG013", " (%d)", Range);
        return -1;
    }

    if ((Range > pobject->Size) || (Range < 1))
    {
        OSError ("TEDAG014", " (%d)", Range);
        return -1;
    }

    switch (pobject->Type) {
    case MXSHORT :
        size = sizeof (SHORT);
        pmessage->Values[(WORD)pobject->Offset + (Range - 1)] = pmessage->Offset;
        if (Value) memcpy (tostream, (char*)Value, size);
        break;
    case MXWORD :
        size = sizeof (WORD);
        pmessage->Values[(WORD)pobject->Offset + (Range - 1)] = pmessage->Offset;
        if (Value) memcpy (tostream, (char*)Value, size);
        break;
    case MXDWORD :
    case MXLONG :
        size = sizeof (DWORD);
        pmessage->Values[(WORD)pobject->Offset + (Range - 1)] = pmessage->Offset;
        if (Value) memcpy (tostream, (char*)Value, size);
        break;
    case MXDOUBLE :
        size = sizeof (DOUBLE);
        pmessage->Values[(WORD)pobject->Offset + (Range - 1)] = pmessage->Offset;
        if (Value) memcpy (tostream, (char*)Value, size);
        break;
    case MXBYTE :
        size = sizeof (BYTE);
        pmessage->Values[(WORD)pobject->Offset + (Range - 1)] = pmessage->Offset;
        if (Value) memcpy (tostream, (char*)Value, size);
        break;
    case MXCHAR :
        size = pobject->Size;
        pmessage->Values[(WORD)pobject->Offset] = pmessage->Offset;
        if (Value) memcpy (tostream, (char*)Value, size);
        break;
    case MXFILE :
        pmessage->Values[(WORD)pobject->Offset + (Range - 1)] = pmessage->Offset;
        size = MXSetFileValue(pmessage,  (char*)Value);
        break;
    case MXBUFFER :
        pmessage->Values[(WORD)pobject->Offset + (Range - 1)] = pmessage->Offset;
        size = MXSetBufferValue(pmessage, (char*)Value);
        if (size < 0) return -1;
        break;
    case MXSTRING :
        pmessage->Values[(WORD)pobject->Offset + (Range - 1)] = pmessage->Offset;
        size = MXSetStringValue(pmessage, (char*)Value);
        if (size < 0) return -1;
        /*if (Value)
               {
                 strcpy (tostream , (char*)Value);
                 size = strlen ((char*)Value) + 1;
               }*/
        break;
    default:
        OSError ("TEDAG0015", NULL);
        return -1;

    }
    pmessage->Offset += size;
    return size;
}

int MXTranferOutputQueue (MXCom* FromCom, MXCom* ToCom)
{
    MXMessage* pmessage;
    int Nbr = 0;

    if ((!FromCom) || (!ToCom)) return 0;
    while (FromCom->OutputMessages)
    {
        Nbr++;
        pmessage = (MXMessage *)FromCom->OutputMessages->car;
        ListRemove (&FromCom->OutputMessages, pmessage);
        ListNewr (&ToCom->OutputMessages, pmessage);
    }
    return Nbr;
}

MXMessage* MXCopyMessage (MXMessage* frommessage)
{
    MXMessage* pmessage = (MXMessage  *)malloc (sizeof (MXMessage));
    if (!pmessage )
    {
        OSError ("TEDAG019", NULL);
        return NULL;
    }

    pmessage->MessageClass = frommessage->MessageClass;
    pmessage->Class = frommessage->Class;
    pmessage->Values = NULL;
    pmessage->Context = NULL;
    pmessage->Size = 0;
    pmessage->Offset = 0;
    pmessage->Complete = 0;
    pmessage->Identity = 0;
    pmessage->Part = 0;
    pmessage->Stream = NULL;
    pmessage->StreamSize = 0;
    pmessage->Com = NULL;

    pmessage->Statut  = 0;
    pmessage->Type  = MXNORMAL;
    pmessage->TDate = 0;
    pmessage->THour = 0;
    pmessage->PDate = 0;
    pmessage->PHour = 0;
    pmessage->DDate = 0;
    pmessage->DHour = 0;
    pmessage->ADate = 0;
    pmessage->AHour = 0;
    OSDateHour (&pmessage->PDate, &pmessage->PHour);

    if (pmessage->MessageClass->Size != 0)
    {
        pmessage->Values = (DWORD *)malloc (pmessage->MessageClass->Size * sizeof (int));
        if ((pmessage->MessageClass->Size) && (!pmessage->Values))
        {
            OSError ("TEDAG019", NULL);
            MXFreeMessage (pmessage->MessageClass->Class->MX, pmessage);
            return NULL;
        }
        else memcpy (pmessage->Values, frommessage->Values, pmessage->MessageClass->Size * sizeof (int));

        pmessage->Stream = (char*)malloc (frommessage->StreamSize);
        if (!pmessage->Stream)
        {
            OSError ("TEDAG019", NULL);
            MXFreeMessage (pmessage->MessageClass->Class->MX, pmessage);
            return NULL;
        }
        else
        {
            memcpy (pmessage->Stream, frommessage->Stream, frommessage->StreamSize);
            pmessage->StreamSize = frommessage->StreamSize;
        }
    }
    pmessage->Context = NULL;
    return pmessage;
}

int MXSetMode (MX* pmx, char* classname, char* messname, BYTE mode, BOOL Blocked)
{
    MXDialogClass* pclass;
    MXMessageClass* pmessclass;


    pclass = MXGetDialogClassFromName (pmx, classname);
    if (!pclass)
    {
        OSError ("TEDAG010", " (%s)", classname);
        return -1;
    }
    pmessclass = MXGetMessageClassFromName (pclass, messname);
    if (!pmessclass)
    {
        OSError ("TEDAG011", " (%s)", messname);
        return -1;
    }
    if (mode == MXONSEND)
    {
        pmessclass->BlockedOnOutput = Blocked;
        return 1;
    }
    if (mode == MXONRECV)
    {
        pmessclass->BlockedOnInput = Blocked;
        return 1;
    }
    OSError ("TEDAG0012", NULL);
    return -1;
}

void MXSetMessageStatut (MXMessage* pmessage, BYTE Statut)
{

    pmessage->Type = Statut;
}

void MXSetMessageDateHour (MXMessage* pmessage, char* Date, char* Hour)
{
    DWORD FDate, FHour;
    char timechar[7]; /* pour conversion HH:MM:SS -> HHMMSS */
    char datechar[7]; /* pour conversion YY:MM:DD -> YYMMDD */


    if (Hour == NULL) FHour = 0;
    else
    {
        if (strlen (Hour) != 8)
        {
            OSError ("TEDAG060", " <%s> : <%s>", Hour, "HH:MM:SS");
            return;
        }

        timechar[0] = Hour[0];
        timechar[1] = Hour[1];
        timechar[2] = Hour[3];
        timechar[3] = Hour[4];
        timechar[4] = Hour[6];
        timechar[5] = Hour[7];
        timechar[6] = 0;
        FHour = (DWORD) atol(timechar);
    }

    if (Date == NULL) FDate = 0;
    else
    {
        struct tm gmt;
        char buf1[7];
        char buf2[4];
        int year;
        time_t time;

        if (strlen (Date) != 10)
        {
            OSError ("TEDAG060", " <%s> <%s>", Date, "YYYY:MM:DD");
            return;
        }



        memset (&gmt, 0, sizeof (struct tm));

        datechar[0] = Date[0];
        datechar[1] = Date[1];
        datechar[2] = Date[2];
        datechar[3] = Date[3];
        datechar[4] = 0;
        gmt.tm_year= atoi (datechar) - 1900;

        datechar[0] = Date[5];
        datechar[1] = Date[6];
        datechar[2] = 0;
        gmt.tm_mon = atoi(datechar) - 1;

        datechar[0] = Date[8];
        datechar[1] = Date[9];
        datechar[2] = 0;
        gmt.tm_mday = atoi (datechar);

        time = mktime (&gmt);

        strftime(buf1,5,"%Y",&gmt);
        year = atoi(buf1);
        year -= 1990;
        strftime(buf2, 4, "%j", &gmt);
        sprintf(buf1,"%02d%s", year, buf2);
        FDate = (DWORD)atol(buf1);
    }

    pmessage->TDate = FDate;
    pmessage->THour = FHour;
}

int MXRecvResponse (MXMessage* pmessage, MXCom* pcom, void* AppField)
{
    CGI* pcgi = (CGI*)AppField;
    BUFFERPARM* Buffer;
    STRING Response;
    int i = 0;
    int j = 0;
    char* CgiBuf;
    Response = (STRING)MXGetValue (pmessage, "Status-Line", 1);
    Buffer = (BUFFERPARM*)MXGetValue (pmessage, "Content", 1);

    CgiBuf = (char*)malloc( Buffer->BufferSize + 1);
    while (j <= (int)Buffer->BufferSize)
    {
        if ((Buffer->BufferContent[j] != 0x0D) && (Buffer->BufferContent[j] != 0x0A))
        {
            CgiBuf[i] = Buffer->BufferContent[j];
            i++;
        }
        j++;
    }
    CgiBuf[i] = 0;
    CGICompute (pcgi, CgiBuf, Buffer->BufferSize + 1);
    free (Buffer->BufferContent);
    free (Buffer);
    return 1;
}

int MXProcess (MX* pmx)
{
    MXMessage* ServerMessage;
    MXCom* ServerCom = MXOpenTCP (pmx, pmx->CGI->Host,  80, IPPROTO_HTTP, NULL, NULL, TRUE);
    if (!ServerCom) return -1;

    MXAddComCallBack (pmx, ServerCom, HTTP_SYS, "Response",MXONRECV, MXRecvResponse, pmx->CGI);
    ServerMessage = MXPutMessage (ServerCom, HTTP_SYS, "Request");
    MXSetValue (ServerMessage, "Request-Line", 1, pmx->CGI->Request);
    MXSetValue (ServerMessage, "Host", 1, pmx->CGI->Host);
    return 1;
}

MXMessage* MXPutMessage (MXCom* pcom, char* classname, char* messclassname)
{
    MXMessage* pmessage;
    pmessage = MXCreateMessage (pcom->MX, classname, messclassname);

    if (pmessage)
    {
        pmessage->Identity = pcom->MessagesId;
        /*time put */
        OSDateHour (&pmessage->PDate, &pmessage->PHour);
        pcom->MessagesId++;
        ListNewr (&pcom->OutputMessages, pmessage);
        return pmessage;
    }
    else return NULL;
}

int MXPutThisMessage (MXCom* pcom, MXMessage* pmessage)
{
    if (pmessage)
    {
        pmessage->Identity = pcom->MessagesId;
        /*time put */
        OSDateHour (&pmessage->PDate, &pmessage->PHour);
        pcom->MessagesId++;
        ListNewr (&pcom->OutputMessages, pmessage);
        return pcom->MessagesId;
    }
    else return -1;
}

/*-----ADD PROCEDURES -------*/
/*-----------------------------------------------------------------------------------------------*/
/* CallBack related to application procedures                                                    */
/*-----------------------------------------------------------------------------------------------*/

int MXAddApplicationProcedure (MX* pmx, int (*funct)(MX*, void*), void* appfield)
{
    MXApplicationProcedure* executeprocedure;

    if ((executeprocedure = MXFindApplicationProcedure (pmx->ApplicationProcedures, funct, appfield)) != NULL)
        return 1;


    executeprocedure = (MXApplicationProcedure *) malloc (sizeof (MXApplicationProcedure));
    if (!executeprocedure)   return -1;
    executeprocedure->ApplicationField = appfield;
    executeprocedure->ExecuteProcedure = funct;
    ListInsert (&pmx->ApplicationProcedures, executeprocedure);
    return 0;
}

/*-----------------------------------------------------------------------------------------------*/
/* CallBack related to ONCLOSE pcom variable procedures                                                    */
/*-----------------------------------------------------------------------------------------------*/

int MXAddEndingProcedure (MXCom* pcom, int (*funct)(MXCom* , void*), void* appfield)
{
    MXComCallBack* endingprocedure;

    if (!pcom) return -1;

    if ((endingprocedure = MXFindComCallBack (pcom->EndingProcedures, funct, appfield)) != NULL)
        return 1;

    endingprocedure = (MXComCallBack *) malloc (sizeof (MXComCallBack));
    if (!endingprocedure)  return -1;

    endingprocedure->ApplicationField = appfield;
    endingprocedure->ExecuteProcedure = funct;

    ListNewr (&pcom->EndingProcedures, endingprocedure);
    return 1;
}

/*-----------------------------------------------------------------------------------------------*/
/* CallBack related to all connections MXONCLOSE and MXONCONNECT                                 */
/*-----------------------------------------------------------------------------------------------*/

int MXAddGeneralConnectCallBack (MX* pmx, BYTE mode, int (*funct)(MXCom* pcom, void*), void* appfield)
{

    MXComCallBack* executeprocedure;

    if (mode == MXONCONNECT)
    {
        if (MXFindComCallBack (pmx->ConnectionProcedures, funct, appfield) != NULL)
            return 1;
    }
    else
        if (mode == MXONCLOSE)
        {
            if (MXFindComCallBack (pmx->EndingProcedures, funct, appfield) != NULL)
                return 1;
        }
        else return -1;

    executeprocedure = (MXComCallBack *) malloc (sizeof (MXComCallBack));
    if (!executeprocedure)   return -1;
    executeprocedure->ApplicationField = appfield;
    executeprocedure->ExecuteProcedure = funct;

    if (mode == MXONCONNECT)
    {
        ListNewr (&pmx->ConnectionProcedures, executeprocedure);
        return 1;
    }

    if (mode == MXONCLOSE)
    {
        ListNewr (&pmx->EndingProcedures, executeprocedure);
        return 1;
    }

    OSError ("TEDAG009", NULL);
    return -1;
}

/*-----------------------------------------------------------------------------------------------*/
/* CallBack related to all connections belonging to class CLASSNAME on MXONCLOSE and MXONCONNECT */
/*-----------------------------------------------------------------------------------------------*/

int MXAddConnectCallBack (MX* pmx, char* pcomclassname, BYTE mode, int (*funct)(MXCom* pcom, void*), void* appfield)
{
    MXComClass* pcomclass = MXGetComClassFromName (pmx, pcomclassname);
    MXComCallBack* executeprocedure;

    if (!pcomclass)
    {
        OSError ("TEDAG007", " (%s)", pcomclassname);
        return -1;
    }

    if (mode == MXONCONNECT)
    {
        if (MXFindComCallBack (pcomclass->ConnectionProcedures, funct, appfield) != NULL)
            return 1;
    }
    else
        if (mode == MXONCLOSE)
        {
            if (MXFindComCallBack (pcomclass->EndingProcedures, funct, appfield) != NULL)
                return 1;
        }
        else return -1;

    executeprocedure = (MXComCallBack *) malloc (sizeof (MXComCallBack));
    if (!executeprocedure)   return -1;
    executeprocedure->ApplicationField = appfield;
    executeprocedure->ExecuteProcedure = funct;
    if (mode == MXONCONNECT)
    {
        ListNewr (&pcomclass->ConnectionProcedures, executeprocedure);
        return 1;
    }

    if (mode == MXONCLOSE)
    {
        ListNewr (&pcomclass->EndingProcedures, executeprocedure);
        return 1;
    }

    OSError ("TEDAG009", NULL);
    return -1;


    return 12;
}

/*-----------------------------------------------------------------------------------------------*/
/* CallBack related to pcom variable procedures MXONRECV, MXONSEND, MXONACK                      */
/*-----------------------------------------------------------------------------------------------*/

int MXAddComCallBack (MX* pmx, MXCom* pcom, char* classname, char* messname, BYTE mode, int (*funct)(MXMessage*, MXCom* pcom, void*), void* appfield)
{
    MXCallBack* executeprocedure;

    MXDialogClass* pclass;
    MXMessageClass* pmessclass;

    /* verify existence */

    pclass = MXGetDialogClassFromName (pmx, classname);

    if (!pclass)
    {
        OSError ("TEDAG007", " (%s)", classname);
        return -1;
    }
    pmessclass = MXGetMessageClassFromName (pclass, messname);
    if (!pmessclass)
    {
        /* accept dynamic messageclasses for DB and FIOP */
        if ((OSStrcmp (pclass->Name, DB_SYS) != 0) &&
                (OSStrcmp (pclass->Name, FIOP_SYS) != 0))
        {
            OSError ("TEDAG008", " (%s)", messname);
            return -1;
        }
        else
            pmessclass = MXCreateMessageClass(pmx, pclass, messname, 1, 0);
    }

    if (mode == MXONSEND)
    {
        if (MXFindCallBack (pmessclass, pcom->ExecuteOutputs, funct, appfield) != NULL)
            return 1;
    }
    else
    if (mode == MXONSENDING)
    {
        if (MXFindCallBack (pmessclass, pcom->ExecuteOutputStream, funct, appfield) != NULL)
        return 1;
    }
    else
    if (mode == MXONRECV)
    {
        if (MXFindCallBack (pmessclass, pcom->ExecuteInputs, funct, appfield) != NULL)
            return 1;
    }
    else
    if (mode == MXONRECEIVING)
    {
        if (MXFindCallBack (pmessclass, pcom->ExecuteInputStream, funct, appfield) != NULL)
            return 1;
    }

    else
    if (mode == MXONACK)
    {
        if (MXFindCallBack (pmessclass, pcom->ExecuteAck, funct, appfield) != NULL)
            return 1;
    }
    else return -1;

    executeprocedure = (MXCallBack *) malloc (sizeof (MXCallBack));
    if (!executeprocedure)   return -1;

    executeprocedure->ApplicationField = appfield;
    executeprocedure->MessageClass = pmessclass;
    executeprocedure->ExecuteProcedure = funct;

    if (mode == MXONSEND)
    {
        ListNewr (&pcom->ExecuteOutputs, executeprocedure);
        return 1;
    }
    if (mode == MXONRECV)
    {
        ListNewr (&pcom->ExecuteInputs, executeprocedure);
        return 1;
    }
    if (mode == MXONSENDING)
    {
        ListNewr (&pcom->ExecuteOutputStream, executeprocedure);
        return 1;
    }
    if (mode == MXONRECEIVING)
    {
        ListNewr (&pcom->ExecuteInputStream, executeprocedure);
        return 1;
    }
    if (mode == MXONACK)
    {
        ListNewr (&pcom->ExecuteAck, executeprocedure);
        return 1;
    }
    OSError ("TEDAG009", NULL);
    return -1;
}

/*-----------------------------------------------------------------------------------------------*/
/* CallBack related to pcom Class Name and Message Name MXONRECV, MXONSEND, MXONACK              */
/*-----------------------------------------------------------------------------------------------*/

int MXAddCallBack (MX* pmx, char* pcomclassname, char* classname, char* messname, BYTE mode, int (*funct)(MXMessage*, MXCom* pcom, void*), void* appfield)
{
    MXCallBack* executeprocedure;

    MXDialogClass* pclass;
    MXMessageClass* pmessclass;
    MXComClass* pcomclass;

    /* verify com class existence */

    pcomclass = MXGetComClassFromName (pmx, pcomclassname);

    if (!pcomclass)
    {
        OSError ("TEDAG007", " (%s)", pcomclassname);
        return -1;
    }


    /* verify class existence */

    pclass = MXGetDialogClassFromName (pmx, classname);

    if (!pclass)
    {
        OSError ("TEDAG007", " (%s)", classname);
        return -1;
    }
    pmessclass = MXGetMessageClassFromName (pclass, messname);
    if (!pmessclass)
    {
        /* accept dynamic messageclasses for DB and FIOP */
        if ((OSStrcmp (pclass->Name, DB_SYS) != 0) &&
                (OSStrcmp (pclass->Name, FIOP_SYS) != 0))
        {
            OSError ("TEDAG008", " (%s)", messname);
            return -1;
        }
        else
            pmessclass = MXCreateMessageClass(pmx, pclass, messname, 1, 0);
    }

    if (mode == MXONSEND)
    {
        if (MXFindCallBack (pmessclass, pcomclass->ExecuteOutputs, funct, appfield) != NULL)
            return 1;
    }
    else
        if (mode == MXONRECV)
        {
            if (MXFindCallBack (pmessclass, pcomclass->ExecuteInputs, funct, appfield) != NULL)
                return 1;
        }
        else
            if (mode == MXONACK)
            {
                if (MXFindCallBack (pmessclass, pcomclass->ExecuteAck, funct, appfield) != NULL)
                    return 1;
            }
            else return -1;

    executeprocedure = (MXCallBack *) malloc (sizeof (MXCallBack));
    if (!executeprocedure)   return -1;

    executeprocedure->ApplicationField = appfield;
    executeprocedure->MessageClass = pmessclass;
    executeprocedure->ExecuteProcedure = funct;

    if (mode == MXONSEND)
    {
        ListNewr (&pcomclass->ExecuteOutputs, executeprocedure);
        return 1;
    }
    if (mode == MXONRECV)
    {
        ListNewr (&pcomclass->ExecuteInputs, executeprocedure);
        return 1;
    }
    if (mode == MXONACK)
    {
        ListNewr (&pcomclass->ExecuteAck, executeprocedure);
        return 1;
    }
    OSError ("TEDAG009", NULL);
    return -1;
}

/*-----------------------------------------------------------------------------------------------*/
/* CallBack related to all messages independantly from com   MXONRECV, MXONSEND, MXONACK         */
/*-----------------------------------------------------------------------------------------------*/

int MXAddGeneralCallBack (MX* pmx, char* classname, char* messname, BYTE mode, int (*funct)(MXMessage*, MXCom* pcom,void*), void* appfield)
{

    MXCallBack* executeprocedure;

    MXDialogClass* pclass = NULL;
    MXMessageClass* pmessclass = NULL;

    if ((!classname && messname) || (classname && !messname))
    {
        OSError ("TEDAG007", " (%s)", classname);
        return -1;
    }

    if (classname && messname)
    {
        pclass = MXGetDialogClassFromName (pmx, classname);

        if (!pclass)
        {
            OSError ("TEDAG007", " (%s)", classname);
            return -1;
        }
        pmessclass = MXGetMessageClassFromName (pclass, messname);
        if (!pmessclass)
        {
            /* accept dynamic messageclasses for DB and FIOP */
            if ((OSStrcmp (pclass->Name, DB_SYS) != 0) &&
                    (OSStrcmp (pclass->Name, FIOP_SYS) != 0))
            {
                OSError ("TEDAG008", " (%s)", messname);
                return -1;
            }
            else
                pmessclass = MXCreateMessageClass(pmx, pclass, messname, 100, 0);
        }
    }
    if (mode == MXONSEND)
    {
        if (classname && messname)
        {
            if ((executeprocedure = MXFindGeneralCallBack (pmessclass->ExecuteOutputs, funct, appfield)) != NULL)
                return 1;
        }
        else
        {
            if ((executeprocedure = MXFindGeneralCallBack (pmx->ExecuteOutputs, funct, appfield)) != NULL)
                return 1;
        }
    }
    else
        if (mode == MXONRECV)
        {
            if (classname && messname)
            {
                if ((executeprocedure = MXFindGeneralCallBack (pmessclass->ExecuteInputs, funct, appfield)) != NULL)
                    return 1;
            }
            else
            {
                if ((executeprocedure = MXFindGeneralCallBack (pmx->ExecuteInputs, funct, appfield)) != NULL)
                    return 1;
            }
        }
        else
            if (mode == MXONACK)
            {
                if (classname && messname)
                {
                    if ((executeprocedure = MXFindGeneralCallBack (pmessclass->ExecuteAck, funct, appfield)) != NULL)
                        return 1;
                }
                else
                {
                    if ((executeprocedure = MXFindGeneralCallBack (pmx->ExecuteAck, funct, appfield)) != NULL)
                        return 1;
                }

            }
            else return -1;

    executeprocedure = (MXCallBack *) malloc (sizeof (MXCallBack));
    if (!executeprocedure)   return -1;

    executeprocedure->ApplicationField = appfield;
    executeprocedure->ExecuteProcedure = funct;

    if (mode == MXONSEND)
    {
        if (classname && messname)
            ListNewr (&pmessclass->ExecuteOutputs, executeprocedure);
        else
            ListNewr (&pmx->ExecuteOutputs, executeprocedure);
        return 0;
    }
    if (mode == MXONRECV)
    {
        if (classname && messname)
            ListNewr (&pmessclass->ExecuteInputs, executeprocedure);
        else
            ListNewr (&pmx->ExecuteInputs, executeprocedure);
        return 0;
    }
    if (mode == MXONACK)
    {
        if (classname && messname)
            ListNewr (&pmessclass->ExecuteAck, executeprocedure);
        else
            ListNewr (&pmx->ExecuteAck, executeprocedure);

        return 0;
    }
    OSError ("TEDAG009", NULL);
    return -1;
}

/*-----REMOVE PROCEDURES -------*/

int MXRemoveApplicationProcedure (MX* pmx, int (*funct)(MX*, void*), void* applicationfield)
{
    MXApplicationProcedure* executeprocedure;

    if ((executeprocedure = MXFindApplicationProcedure (pmx->ApplicationProcedures, funct, applicationfield)) == NULL)
        return 1;
    ListRemove (&pmx->ApplicationProcedures, executeprocedure);
    free (executeprocedure);
    return 1;
}

int MXRemoveEndingProcedure (MXCom* pcom, int (*funct)(MXCom* ,void*), void* appfield)
{
    MXComCallBack* endingprocedure;
    if ((endingprocedure = MXFindComCallBack (pcom->EndingProcedures, funct, appfield)) == NULL)
        return 1;

    ListRemove (&pcom->EndingProcedures, endingprocedure);
    free (endingprocedure);
    return 1;
}

int MXRemoveGeneralConnectCallBack (MX* pmx, BYTE mode, int (*funct)(MXCom* pcom, void*), void* appfield)
{
    MXComCallBack* executeprocedure;

    if (mode == MXONCONNECT)
    {
        if ((executeprocedure = MXFindComCallBack (pmx->ConnectionProcedures, funct, appfield)) != NULL)
        {
            ListRemove (&pmx->ConnectionProcedures, executeprocedure);
            free (executeprocedure);
            return 1;
        }
    }
    if (mode == MXONCLOSE)
    {
        if ((executeprocedure = MXFindComCallBack (pmx->EndingProcedures, funct, appfield)) != NULL)
        {
            ListRemove (&pmx->EndingProcedures, executeprocedure);
            free (executeprocedure);
            return 1;
        }
    }

    OSError ("TEDAG009", NULL);
    return -1;

}

int MXRemoveConnectCallBack (MX* pmx, char* pcomclassname, BYTE mode, int (*funct)(MXCom* pcom, void*), void* appfield)
{
    MXComCallBack* executeprocedure;

    MXComClass* pcomclass = MXGetComClassFromName (pmx, pcomclassname);
    if (!pcomclass)
    {
        OSError ("TEDAG007", " (%s)", pcomclassname);
        return -1;
    }


    if (mode == MXONCONNECT)
    {
        if ((executeprocedure = MXFindComCallBack (pcomclass->ConnectionProcedures, funct, appfield)) != NULL)
        {
            ListRemove (&pcomclass->ConnectionProcedures, executeprocedure);
            free (executeprocedure);
            return 1;
        }
    }
    if (mode == MXONCLOSE)
    {
        if ((executeprocedure = MXFindComCallBack (pcomclass->EndingProcedures, funct, appfield)) != NULL)
        {
            ListRemove (&pcomclass->EndingProcedures, executeprocedure);
            free (executeprocedure);
            return 1;
        }
    }

    OSError ("TEDAG009", NULL);
    return -1;

}

int MXRemoveComCallBack (MX* pmx, MXCom* pcom, char* classname, char* messname, BYTE mode, int (*funct)(MXMessage*, MXCom* pcom, void*), void* appfield)
{
    MXDialogClass* pclass;
    MXMessageClass* pmessclass;
    MXCallBack* executeprocedure;

    pclass = MXGetDialogClassFromName (pmx, classname);

    if (!pclass)
    {
        OSError ("TEDAG007", " (%s)", classname);
        return -1;
    }
    pmessclass = MXGetMessageClassFromName (pclass, messname);
    if (!pmessclass)
    {
        OSError ("TEDAG008", " (%s)", messname);
        return -1;
    }

    if (mode == MXONSEND)
    {
        if ((executeprocedure = MXFindCallBack (pmessclass, pcom->ExecuteOutputs, funct, appfield)) == NULL)
            return 1;
        ListRemove (&pcom->ExecuteOutputs, executeprocedure);
        free (executeprocedure);
        return 1;
    }
    if (mode == MXONSENDING)
    {
        if ((executeprocedure = MXFindCallBack (pmessclass, pcom->ExecuteOutputStream, funct, appfield)) == NULL)
            return 1;
        ListRemove (&pcom->ExecuteOutputStream, executeprocedure);
        free (executeprocedure);
        return 1;
    }
    if (mode == MXONRECV)
    {
        if ((executeprocedure = MXFindCallBack (pmessclass, pcom->ExecuteInputs, funct, appfield)) == NULL)
            return 1;
        ListRemove (&pcom->ExecuteInputs, executeprocedure);
        free (executeprocedure);
        return 1;
    }
    if (mode == MXONRECEIVING)
    {
        if ((executeprocedure = MXFindCallBack (pmessclass, pcom->ExecuteInputStream, funct, appfield)) == NULL)
            return 1;
        ListRemove (&pcom->ExecuteInputStream, executeprocedure);
        free (executeprocedure);
        return 1;
    }

    if (mode == MXONACK)
    {
        if ((executeprocedure = MXFindCallBack (pmessclass, pcom->ExecuteAck, funct, appfield)) == NULL)
            return 1;
        ListRemove (&pcom->ExecuteAck, executeprocedure);
        free (executeprocedure);
        return 1;
    }
    OSError ("TEDAG009", NULL);
    return -1;
}

int MXRemoveCallBack (MX* pmx, char* pcomclassname, char* classname, char* messname, BYTE mode, int (*funct)(MXMessage*, MXCom* pcom, void*), void* appfield)
{
    MXDialogClass* pclass;
    MXMessageClass* pmessclass;
    MXCallBack* executeprocedure;
    MXComClass* pcomclass;

    /* verify com class existence */

    pcomclass = MXGetComClassFromName (pmx, pcomclassname);

    if (!pcomclass)
    {
        OSError ("TEDAG007", " (%s)", pcomclassname);
        return -1;
    }


    pclass = MXGetDialogClassFromName (pmx, classname);

    if (!pclass)
    {
        OSError ("TEDAG007", " (%s)", classname);
        return -1;
    }
    pmessclass = MXGetMessageClassFromName (pclass, messname);
    if (!pmessclass)
    {
        OSError ("TEDAG008", " (%s)", messname);
        return -1;
    }

    if (mode == MXONSEND)
    {
        if ((executeprocedure = MXFindCallBack (pmessclass, pcomclass->ExecuteOutputs, funct, appfield)) == NULL)
            return 1;
        ListRemove (&pcomclass->ExecuteOutputs, executeprocedure);
        free (executeprocedure);
        return 1;
    }
    if (mode == MXONRECV)
    {
        if ((executeprocedure = MXFindCallBack (pmessclass, pcomclass->ExecuteInputs, funct, appfield)) == NULL)
            return 1;
        ListRemove (&pcomclass->ExecuteInputs, executeprocedure);
        free (executeprocedure);
        return 1;
    }
    if (mode == MXONACK)
    {
        if ((executeprocedure = MXFindCallBack (pmessclass, pcomclass->ExecuteAck, funct, appfield)) == NULL)
            return 1;
        ListRemove (&pcomclass->ExecuteAck, executeprocedure);
        free (executeprocedure);
        return 1;
    }
    OSError ("TEDAG009", NULL);
    return -1;
}

int MXRemoveGeneralCallBack (MX* pmx, char* classname, char* messname,BYTE mode, int (*funct)(MXMessage*, MXCom* pcom,void*), void* appfield)
{
    MXDialogClass* pclass;
    MXMessageClass* pmessclass;
    MXCallBack* executeprocedure;

    pclass = MXGetDialogClassFromName (pmx, classname);

    if (!pclass)
    {
        OSError ("TEDAG007", " (%s)", classname);
        return -1;
    }
    pmessclass = MXGetMessageClassFromName (pclass, messname);
    if (!pmessclass)
    {
        OSError ("TEDAG008", " (%s)", messname);
        return -1;
    }

    if (mode == MXONSEND)
    {
        if ((executeprocedure = MXFindGeneralCallBack (pmessclass->ExecuteOutputs, funct, appfield)) == NULL)
            return 1;
        ListRemove (&pmessclass->ExecuteOutputs, executeprocedure);
        free (executeprocedure);
        return 1;
    }
    if (mode == MXONRECV)
    {
        if ((executeprocedure = MXFindGeneralCallBack (pmessclass->ExecuteInputs, funct, appfield)) == NULL)
            return 1;

        ListRemove (&pmessclass->ExecuteInputs, executeprocedure);
        free (executeprocedure);
        return 1;
    }
    if (mode == MXONACK)
    {
        if ((executeprocedure = MXFindGeneralCallBack (pmessclass->ExecuteInputs, funct, appfield)) == NULL)
            return 1;

        ListRemove (&pmessclass->ExecuteAck, executeprocedure);
        free (executeprocedure);
        return 1;
    }
    OSError ("TEDAG009", NULL);
    return -1;
}

MXDialogClass* MXGetDialogClassFromName (MX* pmx, char* name)
{
    List* list = pmx->Classes;
    MXDialogClass* pclass;

    while (list)
    {
        pclass = (MXDialogClass *) list->car;
        if (OSStrcmp (pclass->Name, name) == 0) return pclass;
        list = list->cdr;
    }
    return NULL;
}

MXDialogClass* MXGetDialogClassFromCode (MX* pmx, WORD ClassCode)
{
    List* list = pmx->Classes;
    MXDialogClass* pclass;

    while (list)
    {
        pclass = (MXDialogClass *) list->car;
        if (pclass->Code == ClassCode) return pclass;
        list = list->cdr;
    }
    return NULL;
}

MXMessageClass* MXGetMessageClassFromName (MXDialogClass* pclass, char* name)
{
    List* list = pclass->MessageClasses;
    MXMessageClass* pmessclass;

    while (list)
    {
        pmessclass = (MXMessageClass *) list->car;
        if (OSStrcmp (pmessclass->Name, name) == 0) return pmessclass;
        list = list->cdr;
    }
    return NULL;
}

MXMessageClass* MXGetMessageClassFromNames (MX* pmx, char* classname, char* messname)
{
    MXDialogClass* pclass;
    MXMessageClass* pmessageclass = NULL;

    pclass = MXGetDialogClassFromName (pmx, classname);
    if (!pclass) return NULL;

    pmessageclass = MXGetMessageClassFromName (pclass, messname);

    return pmessageclass;
}

MXMessageClass* MXGetMessageClassFromCode (MX* pmx, MXDialogClass* pclass, WORD MessageClassCode)
{
    MXMessageClass* pmessclass;

    List* list = pclass->MessageClasses;

    while (list)
    {
        pmessclass = (MXMessageClass *) list->car;
        if (pmessclass->Code == MessageClassCode) return pmessclass;
        list = list->cdr;
    }
    return NULL;
}

MXMessageClass* MXGetMessageClassFromCodes (MX* pmx, WORD ClassCode, WORD MessageClassCode)
{
    MXDialogClass* pclass;
    MXMessageClass* pmessageclass = NULL;

    pclass = MXGetDialogClassFromCode (pmx, ClassCode);
    if (!pclass) return NULL;

    pmessageclass = MXGetMessageClassFromCode (pmx, pclass, MessageClassCode);

    return pmessageclass;
}

MXObject* MXGetObjectFromName (MXMessageClass* pclassmess, char* name)
{
    List* list = pclassmess->Objects;
    MXObject* pclass;

    while (list)
    {
        pclass = (MXObject *) list->car;
        if (OSStrcmp (pclass->Name, name) == 0) return pclass;
        list = list->cdr;
    }
    return NULL;
}

MXComClass* MXGetComClassFromName (MX* pmx, char* Name)
{
    List* list = pmx->ComClasses;
    MXComClass* pcomclass;

    while (list)
    {
        pcomclass = (MXComClass *) list->car;
        if (OSStrcmp (pcomclass->Name, Name) == 0) return pcomclass;
        list = list->cdr;
    }
    return NULL;
}

MXComClass* MXGetComClassFromCode (MX* pmx, WORD Code)
{
    List* list = pmx->ComClasses;
    MXComClass* pcomclass;

    while (list)
    {
        pcomclass = (MXComClass *) list->car;
        if (pcomclass->Code == Code) return pcomclass;
        list = list->cdr;
    }
    return NULL;
}