/*-------------------------------------------------------------------------*/
/*                    NETPROG (Network Programming)                        */
/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */  
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*                                                                         */
/* This program is free software; you can redistribute it and/or modify it */
/* under the terms of the GNU General Public License as published by the   */
/* Free Software Foundation; either version 2 of the License, or (at your  */ 
/* option) any later  version.                                             */
/*                                                                         */
/* This program is distributed in the hope that it will be useful, but     */
/* WITHOUT ANY WARRANTY; without even the implied warranty of              */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU General */
/* Public License for more details.                                        */
/*                                                                         */
/* You should have received a copy of the GNU General Public License along */
/* with this program; if not, write to the Free Software Foundation, Inc., */
/* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA                   */
/* Copy of GNU General Public License at: http://www.gnu.org/              */
/*                                                                         */
/* Source code home page: http://sourceforge.net/projects/netprog/         */
/* Contact author at: Gabriel_Jureidini@yahoo.com                          */
/*-------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS
#include "cgi.h" 



cgiEnvironmentResultType CGIReadEnvironment(CGI* pcgi, char *filename)
{
    FILE *in;
    CGIFormEntry *e, *p;
    /* Free any exist ing data first */
    CGIFreeResources(pcgi);
    in = fopen(filename, "rb");
    if (!in)         /* Can't access file */
        return cgiEnvironmentIO;

    if (!CGIReadString(pcgi, in, &pcgi->ServerSoftware))   goto error;
    if (!CGIReadString(pcgi, in, &pcgi->ServerName))       goto error;

    if (!CGIReadString(pcgi, in, &pcgi->GatewayInterface)) goto error;
    if (!CGIReadString(pcgi, in, &pcgi->ServerProtocol))   goto error;
    if (!CGIReadString(pcgi, in, &pcgi->ServerPort))       goto error;
    if (!CGIReadString(pcgi, in, &pcgi->RequestMethod))    goto error;
    if (!CGIReadString(pcgi, in, &pcgi->PathInfo))         goto error;
    if (!CGIReadString(pcgi, in, &pcgi->PathTranslated))   goto error;
    if (!CGIReadString(pcgi, in, &pcgi->ScriptName))       goto error;
    if (!CGIReadString(pcgi, in, &pcgi->QueryString))      goto error;
    if (!CGIReadString(pcgi, in, &pcgi->RemoteHost))       goto error;
    if (!CGIReadString(pcgi, in, &pcgi->RemoteAddr))       goto error;
    if (!CGIReadString(pcgi, in, &pcgi->AuthType))         goto error;
    if (!CGIReadString(pcgi, in, &pcgi->RemoteUser))       goto error;
    if (!CGIReadString(pcgi, in, &pcgi->RemoteIdent))      goto error;
    if (!CGIReadString(pcgi, in, &pcgi->ContentType))      goto error;
    if (!CGIReadString(pcgi, in, &pcgi->Accept))           goto error;
    if (!CGIReadString(pcgi, in, &pcgi->UserAgent))        goto error;
    if (!CGIReadString(pcgi, in, &pcgi->Referrer))         goto error;
    if (!CGIReadInt(pcgi, in,    &pcgi->ContentLength))    goto error;

    p = 0;
    while (1)
    {
        e = (CGIFormEntry *) malloc(sizeof(CGIFormEntry));
        if (!e)
        {
            CGIFreeResources(pcgi);
            fclose(in);
            return cgiEnvironmentMemory;
        }
        if (!CGIReadString(pcgi, in, &e->Attribute))
        {
            /* This means we've reached the end of the list. */
            free(e);
            break;
        }
        if (!CGIReadString(pcgi, in, &e->Value))
        {
            free(e);
            goto error;
        }
        e->next = 0;
        if (p) p->next = e;
        else   pcgi->FormEntryFirst = e;
        p = e;
    }
    fclose(in);
    pcgi->Restored = 1;
    return cgiEnvironmentSuccess;
error:
    CGIFreeResources(pcgi);
    fclose(in);
    return cgiEnvironmentIO;
}


cgiEnvironmentResultType CGIWriteEnvironment(CGI* pcgi, char *filename)
{
    FILE *out;
    CGIFormEntry *e;
    out = fopen(filename, "wt");
    if (!out)
        return cgiEnvironmentIO;
    if (!CGIWriteString(pcgi, out, pcgi->ServerSoftware))      goto error;
    if (!CGIWriteString(pcgi, out, pcgi->ServerName))          goto error;
    if (!CGIWriteString(pcgi, out, pcgi->GatewayInterface))    goto error;
    if (!CGIWriteString(pcgi, out, pcgi->ServerProtocol))      goto error;
    if (!CGIWriteString(pcgi, out, pcgi->ServerPort))          goto error;
    if (!CGIWriteString(pcgi, out, pcgi->RequestMethod))       goto error;
    if (!CGIWriteString(pcgi, out, pcgi->PathInfo))            goto error;
    if (!CGIWriteString(pcgi, out, pcgi->PathTranslated))      goto error;
    if (!CGIWriteString(pcgi, out, pcgi->ScriptName))          goto error;
    if (!CGIWriteString(pcgi, out, pcgi->QueryString))         goto error;
    if (!CGIWriteString(pcgi, out, pcgi->RemoteHost))          goto error;
    if (!CGIWriteString(pcgi, out, pcgi->RemoteAddr))          goto error;
    if (!CGIWriteString(pcgi, out, pcgi->AuthType))            goto error;
    if (!CGIWriteString(pcgi, out, pcgi->RemoteUser))          goto error;
    if (!CGIWriteString(pcgi, out, pcgi->RemoteIdent))         goto error;
    if (!CGIWriteString(pcgi, out, pcgi->ContentType))         goto error;
    if (!CGIWriteString(pcgi, out, pcgi->Accept))              goto error;
    if (!CGIWriteString(pcgi, out, pcgi->UserAgent))           goto error;
    if (!CGIWriteString(pcgi, out, pcgi->Referrer))            goto error;
    if (!CGIWriteInt(pcgi, out,    pcgi->ContentLength))       goto error;

    e = pcgi->FormEntryFirst;
    while (e)
    {
        if (!CGIWriteString(pcgi, out, e->Attribute))          goto error;
        if (!CGIWriteString(pcgi, out, e->Value))         goto error;

        e = e->next;
    }
    fclose(out);
    return cgiEnvironmentSuccess;
error:
    fclose(out);
    return cgiEnvironmentIO;
}





int CGIInitEnv(CGI* pcgi)
{
    int result = 0;
    CGISetupConstants(pcgi);
    CGIGetEnv(&pcgi->ServerSoftware,      "SERVER_SOFTWARE");
    CGIGetEnv(&pcgi->ServerName,          "SERVER_NAME");
    CGIGetEnv(&pcgi->ServerProtocol,      "SERVER_PROTOCOL");
    CGIGetEnv(&pcgi->ServerPort,          "SERVER_PORT");
    CGIGetEnv(&pcgi->RequestMethod,       "REQUEST_METHOD");
    CGIGetEnv(&pcgi->QueryString,         "QUERY_STRING");
    CGIGetEnv(&pcgi->PathInfo,            "PATH_INFO");
    CGIGetEnv(&pcgi->PathTranslated,      "PATH_TRANSLATED");
    CGIGetEnv(&pcgi->ScriptName,          "SCRIPT_NAME");
    CGIGetEnv(&pcgi->GatewayInterface,    "GATEWAY_INTERFACE");
    CGIGetEnv(&pcgi->AuthType,            "AUTH_TYPE");
    CGIGetEnv(&pcgi->RemoteHost,          "REMOTE_HOST");
    CGIGetEnv(&pcgi->RemoteAddr,          "REMOTE_ADDR");
    CGIGetEnv(&pcgi->RemoteUser,          "REMOTE_USER");
    CGIGetEnv(&pcgi->RemoteIdent,         "REMOTE_IDENT");
    CGIGetEnv(&pcgi->ContentType,         "CONTENT_TYPE");
    CGIGetEnv(&pcgi->ContentLengthString, "CONTENT_LENGTH");
    CGIGetEnv(&pcgi->Accept,              "HTTP_ACCEPT");
    CGIGetEnv(&pcgi->UserAgent,           "HTTP_USER_AGENT");
    CGIGetEnv(&pcgi->Referrer,            "HTTP_REFERER");
    CGIGetEnv(&pcgi->Cookie,              "HTTP_COOKIE");

    pcgi->ContentLength    = atoi(pcgi->ContentLengthString);
    pcgi->CookieLength     = strlen(pcgi->Cookie);


    pcgi->FormEntryFirst   = NULL;
    pcgi->CookieEntryFirst = NULL;
    pcgi->Restored         = 0;
    pcgi->FindTarget       = 0;
    pcgi->FindPos          = NULL;
    pcgi->In               = stdin;
    pcgi->Out              = stdout;


    if (CGIParseCookieInput(pcgi, pcgi->Cookie, pcgi->CookieLength) != cgiParseSuccess)
    {
        CGIFreeResources(pcgi);
        return -1;
    }

    if (OSStrEqNc(pcgi->RequestMethod, "post"))
    {
        if (OSStrEqNc(pcgi->ContentType, "application/x-www-form-urlencoded"))
        {
            if (CGIParsePostFormInput(pcgi) != cgiParseSuccess)
            {
                CGIFreeResources(pcgi);
                return -1;
            }
        }
        else
            if (OSStrncmp (pcgi->ContentType, "multipart/form-data", 19) == 0)
            {
                if (CGIParsePostMultiPartFormInput(pcgi) != cgiParseSuccess)
                {
                    CGIFreeResources(pcgi);
                    return -1;
                }
            }
    }
    else
        if (OSStrEqNc(pcgi->RequestMethod, "get"))
        {
            pcgi->ContentLength = strlen(pcgi->QueryString);
            if (CGIParseFormInput(pcgi, pcgi->QueryString, pcgi->ContentLength) != cgiParseSuccess)
            {
                CGIFreeResources(pcgi);
                return -1;
            }
        }
    return result;
}



void CGIFreeResources(CGI* pcgi)
{
    CGIFormEntry* c = pcgi->FormEntryFirst;
    CGIFormEntry* n;

    while (c)
    {
        n = c->next;
        free (c->Attribute);
        free (c->Value);
        free (c);
        c = n;
    }
    c= pcgi->CookieEntryFirst;
    while (c)
    {
        n = c->next;
        free (c->Attribute);
        free (c->Value);
        free (c);
        c = n;
    }

    /* If the cgi environment was restored from a saved environment,
        then these are in allocated space and must also be freed */
    if (pcgi->Restored)
    {
        free (pcgi->ServerSoftware);
        free (pcgi->ServerName);
        free (pcgi->GatewayInterface);
        free (pcgi->ServerProtocol);
        free (pcgi->ServerPort);
        free (pcgi->RequestMethod);
        free (pcgi->PathInfo);
        free (pcgi->PathTranslated);
        free (pcgi->ScriptName);
        free (pcgi->QueryString);
        free (pcgi->RemoteHost);
        free (pcgi->RemoteAddr);
        free (pcgi->AuthType);
        free (pcgi->RemoteUser);
        free (pcgi->RemoteIdent);
        free (pcgi->ContentType);
        free (pcgi->Accept);
        free (pcgi->UserAgent);
        free (pcgi->Referrer);
    }
}



int CGIInit (CGI* pcgi, char* Host, char* Request)
{
    int i, j;

    memset (pcgi, 0, sizeof (CGI));
    pcgi->RootNode      = NULL;
    pcgi->ColumnsNumber = 0;
    pcgi->Buffer        = NULL;

    if (!Request)
        strcpy (pcgi->Request, "");
    else
        strcpy (pcgi->Request, Request);
    if (!Host)
        strcpy (pcgi->Host, "");
    else
        strcpy (pcgi->Host, Host);

    CGIInitEnv(pcgi);

    OSGetSDateHour (&ActualDate, &ActualHour);

    for (i = 0; i < CGIMaxLevels; i++)
    {
        pcgi->NodeLevels[i] = NULL;
        for (j = 0; j < CGIMaxColumn; j++)
        {
            pcgi->ColumnsSequence[i][j] = -1;
        }

        pcgi->ColumnsVisited[i] = 0;
        pcgi->ColumnsLastRange[i] = 0;
    }
    pcgi->RootNode = CGIMakeNode ("ROOT", CGITEXT,0);
    for (i = 0; i < CGIMaxColumn; i++)
        pcgi->ColumnsValues[i].Values = NULL;

    pcgi->Out = stdout;
    pcgi->In  = stdin;
    return 1;
}



void CGIEnd (CGI* pcgi)
{

    CGIFreeColumns (pcgi);
    CGIFreeTags (pcgi);
    if (pcgi->RootNode) CGIFreeNode (pcgi->RootNode);
    free (pcgi->Buffer);
    CGIFreeResources(pcgi);
}


void CGIFreeTags (CGI* pcgi)
{
    CGITag* elt;
    while (pcgi->Tags)
    {
        elt = (CGITag *)pcgi->Tags->car;
        ListRemove (&pcgi->Tags, elt);
        CGIFreeTag (elt);
    }
}



void CGIFreeTag (CGITag* ptag)
{
    while (ptag->Elements)
    {
        CGIElement* elt;
        ListRemove (&ptag->Elements, elt = ptag->Elements->car);
        free (elt);
    }
    while (ptag->Columns)
    {
        CGITagColumn* pcolumn;
        ListRemove (&ptag->Columns, pcolumn = ptag->Columns->car);
        free (pcolumn);
    }
    free (ptag);
}



void CGIFreeValue (CGIValue* pvalue)
{
    while (pvalue->ExecuteValue)
    {
        CGICallBack* pexecute;
        ListRemove (&pvalue->ExecuteValue, pexecute = pvalue->ExecuteValue->car);
        free (pexecute);
    }
    free (pvalue->Value);
    free (pvalue);
}



void CGIFreeValues (CGI* pcgi)
{
    int i;
    CGIValue* elt;

    for (i = 0; i < pcgi->ColumnsNumber; i++)
    {
        while (pcgi->ColumnsValues[i].Values)
        {
            elt = (CGIValue *)pcgi->ColumnsValues[i].Values->car;
            ListRemove (&pcgi->ColumnsValues[i].Values, elt);
            CGIFreeValue (elt);
        }
    }
}


void CGIFreeColumns (CGI* pcgi)
{
    int i;
    CGIValue* elt;

    for (i = 0; i < pcgi->ColumnsNumber; i++)
    {
        free (pcgi->ColumnsValues[i].Name);
        while (pcgi->ColumnsValues[i].Values)
        {
            elt = (CGIValue *)pcgi->ColumnsValues[i].Values->car;
            ListRemove (&pcgi->ColumnsValues[i].Values, elt);
            CGIFreeValue (elt);
        }
    }
}


int CGIAddColumn (CGI* pcgi, char* name, int namesize, int valuesize, void* values)
{
    int index;

    if ((index = CGIGetColumnIndex (pcgi, name)) >= 0)
    {
        return index;
    }
    pcgi->ColumnsValues[pcgi->ColumnsNumber].Name = (char *) malloc (namesize + 1);
    sprintf (pcgi->ColumnsValues[pcgi->ColumnsNumber].Name, "%.*s", namesize, name);
    pcgi->ColumnsValues[pcgi->ColumnsNumber].Size = valuesize;
    pcgi->ColumnsValues[pcgi->ColumnsNumber].Values = values;
    return pcgi->ColumnsNumber++;
}



CGINode* CGIMakeNode (char* name, BYTE type, DWORD pointer)
{
    CGINode* Node = (CGINode *)malloc (sizeof (CGINode));
    Node->Name = (char *)malloc (strlen(name) + 1);
    Node->Type = type;
    Node->Pointer = pointer;
    Node->Nodes = NULL;
    strcpy (Node->Name, name);
    return Node;
}


void CGIFreeNode (CGINode* node)
{
    List* Nodes = node->Nodes;
    while (Nodes)
    {
        CGINode* Node = (CGINode *)Nodes->car;
        ListRemove (&Nodes, Node);
        CGIFreeNode (Node);
    }
    free (node->Name);
    free (node);
}


BYTE CGILookKeyWord (char* name)
{
    if (strcmp (name, "begindetail") == 0)
        return CGIBEGINDETAIL;
    else if(strcmp (name, "enddetail") == 0)
        return CGIENDDETAIL;
    else if (strcmp (name, "constant") == 0)
        return CGICONSTANT;
    else return CGICOLUMN;
}



CGIValue* CGIAddValue    (CGI* pcgi, char* ColumName, char* Value)
{
    int index;
    CGIValue* SValue;
    index = CGIGetColumnIndex(pcgi, ColumName);
    if (index < 0) return NULL;
    SValue = (CGIValue*)malloc (sizeof (CGIValue));
    if (!SValue) return NULL;

    SValue->UserField = NULL;
    SValue->ExecuteValue = NULL;
    SValue->Value = (char *) malloc (pcgi->ColumnsValues[index].Size + 1);
    if (Value)
        sprintf (SValue->Value, "%.*s", pcgi->ColumnsValues[index].Size, Value);
    else
        sprintf (SValue->Value, "%.*s", pcgi->ColumnsValues[index].Size, "");
    ListNewr (&pcgi->ColumnsValues[index].Values, SValue);
    return SValue;
}


int CGISetValue (CGI* pcgi, char* ColumName, int Range, char* Value)
{
    int index;
    CGIValue* RValue;
    index = CGIGetColumnIndex(pcgi, ColumName);
    if (index < 0) return -1;
    RValue = (CGIValue*)ListGetElt (pcgi->ColumnsValues[index].Values, Range - 1);
    if (!RValue) return -1;
    sprintf (RValue->Value, "%.*s", pcgi->ColumnsValues[index].Size, Value);
    return 1;
}

char* CGIGetValue (CGI* pcgi, char* ColumName, int Range)
{
    int index;
    CGIValue* RValue;
    index =CGIGetColumnIndex(pcgi, ColumName);
    if (index < 0) return NULL;
    RValue = (CGIValue*)ListGetElt (pcgi->ColumnsValues[index].Values, Range - 1);
    return RValue->Value;
}



int CGIGetColumnIndex (CGI* pcgi, char* name)
{
    int i;
    for (i = 0; i < pcgi->ColumnsNumber; i++)
        if (OSStrcmp (pcgi->ColumnsValues[i].Name, name) == 0)
            return i;
    return -1;
}


void CGISetColumnsSequence (CGI* pcgi, int Level, int Nbr)
{
    int i;
    for (i = 0; i < CGIMaxColumn; i++)
        pcgi->ColumnsSequence[Level][i] = Nbr;
}

int CGIColumnsInList (CGI* pcgi, List* list)
{
    CGINode* Node;
    int index;
    List* nodelist = list;
    while (nodelist)
    {
        Node = (CGINode *)nodelist->car;
        if ((Node->Type == CGICOLUMN) &&
                ((index = CGIGetColumnIndex(pcgi, Node->Name)) >= 0))
            return ListNbElt (pcgi->ColumnsValues[index].Values);
        nodelist = nodelist->cdr;
    }
    return 0;
}



void CGIPrintOut (CGI* pcgi, FILE* fileout, CGINode* node, int* Level, int* Range)
{
    int columnindex;
    List* Values;
    List* list;
    CGIValue* value;
    int number;
    int i;
    int fromrange;
    int Visited;


    switch (node->Type)
    {
    case CGITEXT :
        fprintf(fileout,	"%s", (char *)(pcgi->Buffer + node->Pointer));
        break;
    case CGIBEGINDETAIL :
        (*Level)++;
        number = CGIColumnsInList (pcgi, node->Nodes);
        if (number == 0) number = 1;
        fromrange = 0;
        Visited = pcgi->ColumnsVisited[*Level];

        if (pcgi->ColumnsSequence[*Level][Visited] != -1)
        {
            fromrange = pcgi->ColumnsLastRange[*Level];
            number = fromrange + pcgi->ColumnsSequence[*Level][Visited];
        }

        for (i = fromrange; i < number; i++)
        {
            list = node->Nodes;
            while (list)
            {
                CGIPrintOut (pcgi, fileout, (CGINode *)list->car, Level, &i);
                list = list->cdr;
            }
        }
        if (pcgi->ColumnsSequence[*Level][Visited] != -1)
            pcgi->ColumnsLastRange[*Level] += pcgi->ColumnsSequence[*Level][Visited];
        pcgi->ColumnsVisited[*Level] +=1;
        break;
    case CGIENDDETAIL :
        (*Level)--;
        break;
    case CGICOLUMN :
        columnindex = CGIGetColumnIndex(pcgi, node->Name);
        if (columnindex < 0)
            fprintf(fileout,	"%s", "  ");
        else
        {
            Values = pcgi->ColumnsValues[columnindex].Values;
            if ((*Level) == 0)  *Range = 0;
            value = ListGetElt (pcgi->ColumnsValues[columnindex].Values, *Range);
            if (!value)
                value = ListGetLastElt (pcgi->ColumnsValues[columnindex].Values);
            if (!value)
                fprintf(fileout,	"%s", "  ");
            else
                fprintf(fileout,	"%s", value->Value);
        }
        break;
    }
}



void CGIReturn (CGI* pcgi, FILE* fileout)
{
    List* list;
    CGINode* node;
    int Level = 0;
    int Range = 0;

    /*  CGIHeaderContentType(pcgi, "text/html\n");*/

    list = pcgi->RootNode->Nodes;
    while (list)
    {
        node = (CGINode *)list->car;
        CGIPrintOut (pcgi, fileout, node, &Level, &Range);
        list = list->cdr;
    }
    fclose (fileout);
}



int CGIParseFile (CGI* pcgi, char* FileName)
{
    int l;
    int i,j,t;
    char* pStart;
    char* pPtr;
    char* keyword;
    FILE* f;
    char ch;
    int State = 0;
    int inter = 0;
    int sinter = 0;
    int level = 0;

    CGINode* node;
    CGINode* DetailedNode = NULL;
    BYTE     type;

    f = fopen (FileName, "r");
    if (!f) return - 1;

    strcpy (pcgi->FileName, FileName);
    j = 0;
    i = 0;
    t = 0;

    pcgi->NodeLevels[level] = pcgi->RootNode;
    DetailedNode = pcgi->RootNode;

    pcgi->Buffer = (char *)malloc (4097);

    pStart = pcgi->Buffer;
    pPtr = pStart;
    l = 0;

    while (i  = fread (pcgi->Buffer + t, 1, 4096, f))
    {
        t+=i;
        pcgi->Buffer[t] = 0;

        while (*pPtr != 0)
        {
            switch (State)
            {
            case 0 :
                ch = *(pPtr);
                while (ch != 0 && ch != '<')
                {
                    pPtr++; l++; ch = *(pPtr);
                }

                if ((ch == '<') && (*(pPtr + 1) == '%'))   /* find node */
                {State = 1; ++pPtr;}
                else
                    if ((ch == '<') && (*(pPtr + 1) == 0))    /* maybe find node but on the next buffer */
                    {State = 6; ++pPtr;}
                    else if (ch != 0) {++pPtr; l++;}          /* continue */
                break;
            case 1 :
                pStart[l] = 0;
                node = CGIMakeNode ("text", CGITEXT, (DWORD)(pStart - pcgi->Buffer));
                ListNewr (&DetailedNode->Nodes, node);
                l = 0;
                pPtr += 1;
                if (*pPtr == 0)
                {State =  5; break;}      /* find <% continue on next buffer */
                keyword = pPtr;
                pStart = pPtr;
                State = 2;
                break;
            case 2 :
                ch = *(pPtr);
                while (ch != 0 && ch != '>')
                {
                    pPtr++; l++; ch = *(pPtr);
                }

                if ((ch == '>') && (*(pPtr -1) == '%'))  State = 3;    /* end node */
            else if (ch != 0) {++pPtr; l++;}
                break;
            case 3 :
                keyword = pPtr -l;
                keyword[l-1] = 0;
                type = CGILookKeyWord (keyword);
                node = CGIMakeNode (keyword, type, (DWORD)(keyword - pcgi->Buffer));

                if (type == CGIBEGINDETAIL)
                {
                    ListNewr (&DetailedNode->Nodes, node);
                    level++;
                    pcgi->NodeLevels[level] = node;
                    DetailedNode = node;
                }
                else
                    if (type == CGIENDDETAIL)
                    {
                        level--;
                        DetailedNode = pcgi->NodeLevels[level];
                        ListNewr (&DetailedNode->Nodes, node);
                    }
                    else
                        ListNewr (&DetailedNode->Nodes, node);
                ++pPtr;
                pStart = pPtr;
                State = 0;
                l = 0;
                break;
            case 5 :
                keyword = pPtr;
                pStart = pPtr;
                State = 2;
                break;
            case 6 :
                if (*pPtr == '%') State = 1;
            else {l++; State = 0;};
                break;
            }
        }
        sinter =  pStart - pcgi->Buffer;
        inter =   pPtr   - pcgi->Buffer;
        j++;
        pcgi->Buffer  = (char *)realloc (pcgi->Buffer, t + 4097);
        pPtr   = pcgi->Buffer + inter;
        pStart = pcgi->Buffer + sinter;
    }

    if (pStart != pPtr)
    {
        pStart[l-1] = 0;
        node = CGIMakeNode ("text", CGITEXT, (DWORD)(pStart - pcgi->Buffer));
        ListNewr (&DetailedNode->Nodes, node);
    }
    fclose (f);
    return 1;
}



BOOL CGIIsDigit (char* value)
{
    BOOL IsDigit = TRUE;
    int i = 0;
    int l;
    IsDigit = TRUE;
    l = (int)strlen (value);
    while (i < l)
    {
        if (isalpha (value[i]))
        { IsDigit = FALSE;
            break;
        }
        else i++;
    }
    return IsDigit;
}



void OSGetSDateHour(long* dat, long* hour)
{
    time_t dh;
    char buf1[7];
    char buf2[4];
    char buf3[20];
    struct tm *gmt;
    int ann;
    int i;

    BeginDay = 1;
    time(&dh);
    gmt = localtime(&dh);

    gmt->tm_mday = 1;
    gmt->tm_mon = 8;
    gmt->tm_year = 99;
    gmt->tm_wday	= 3;
    gmt->tm_yday = 243;



    strftime(buf1,5,"%Y",gmt);
    ann = atoi(buf1);
    ann -= 1990;

    strftime(buf2, 4, "%j", gmt);
    ActualYearDay = atoi (buf2);

    sprintf(buf1,"%02d%s", ann, buf2);

    *dat = (long)atol(buf1);

    strftime(buf1,7,"%H%M%S",gmt);
    *hour = (long)atol(buf1);

    strftime(buf3, 2, "%w", gmt);
    ActualWeekDay = atoi (buf3);

    strftime(buf3, 5, "%Y", gmt);
    ActualYear = atoi (buf3);
    OSFormatDate(*dat, ActualStrDate, 1);
    OSFormatHour (ActualHour, ActualStrHour, 1);

    InDay = OSGetDayFromDate (ActualYear - 1900, ActualMonth + 1, ActualMonthDay);
    sprintf (ActualStrDay, "%s", Day[ActualWeekDay]);
    InMonth = OSGetMonthFromDate (ActualYear - 1900, ActualMonth + 1);

    if (ActualMonth >= 8)
        for (i = 8; i < ActualMonth; i++)
            BeginDay +=  NbDays[i];
    else
    {
        for (i = 8; i <= 11; i++)
            BeginDay +=  NbDays[i];
        for (i = 0; i <= ActualMonth; i++)
            BeginDay +=  NbDays[i];
    }
    return;
}


int CGIAddCallBack (CGI* pcgi, CGIValue* pvalue, int (*funct)(CGI* pcgi, CGIValue* pvalue, void* appfield), void* appfield)
{
    CGICallBack* executeprocedure;

    executeprocedure = (CGICallBack *) malloc (sizeof (CGICallBack));
    if (!executeprocedure)   return -1;

    executeprocedure->ApplicationField = appfield;
    executeprocedure->ExecuteProcedure = funct;
    ListNewr (&pvalue->ExecuteValue, executeprocedure);
    return 1;
}


CGIElement* CGIAddElement (CGI* pcgi, CGITag* ptag, BYTE Type, char* Tag, int Occurrence)
{
    CGIElement* Element  = (CGIElement*)malloc (sizeof (CGIElement));
    if (!Element) return NULL;
    Element->Type = Type;
    strcpy (Element->Tag, Tag);
    Element->Occurrence = Occurrence;
    ListNewr (&ptag->Elements, Element);
    return Element;
}



CGITag*  CGIAddTag (CGI* pcgi, char* Name, BYTE Type, int nb_param, ...)
{
    va_list p_liste;
    CGITag* ptag;
    CGITagColumn* pcolumn;
    int i = 0;


    ptag = (CGITag *)malloc (sizeof (CGITag));
    memset (ptag, 0, sizeof (CGITag));
    strcpy (ptag->Name, Name);
    ptag->Type = Type;
    if (Type == CGITABLE)
    {
        va_start(p_liste,nb_param);
        for (i = 1; i <= nb_param; i++)
        {
            pcolumn = (CGITagColumn*)malloc (sizeof (CGITagColumn));
            if (!pcolumn) continue;

            strcpy(pcolumn->Name, va_arg(p_liste, char *));
            pcolumn->Range = va_arg(p_liste, int);
            ListNewr (&ptag->Columns, pcolumn);
        }
    }
    ListNewr (&pcgi->Tags, ptag);
    return ptag;
}


int CGICompute (CGI* pcgi, char* Content, int Size)
{
    int         Pos = 0;
    List*       Tags = pcgi->Tags;
    while (Tags)
    {
        CGITag*      Tag = (CGITag*)Tags->car;
        int          index = CGIGetColumnIndex(pcgi, Tag->Name);
        List*        Elements = Tag->Elements;

        while (Elements)
        {
            CGIElement*  Element = (CGIElement*)Elements->car;
            Pos = 0;
            switch (Element->Type)
            {
            case CGIPOS :
                Pos = atoi (Element->Tag);
                break;
            case CGIFIND :
                {
                    int pos =  OSFindString (Content + Pos , Size - Pos, Element->Tag, Element->Occurrence);
                    if (pos == -1)
                    {
                        Pos = -1;
                        break;
                    }
                    Pos += pos;
                }
                break;
            case CGIINCR :
                Pos += atoi (Element->Tag);
                break;
            case CGIDECR :
                Pos -= atoi (Element->Tag);
                break;
            case CGINEXNOTAG :
                {
                    char* line = Content + Pos;
                    int pos = 0;
                    while (pos < (Size - Pos))
                    {
                        if ((line[pos] == '>') || (line[pos] == '<')) pos++;

                        while ((line[pos] != '>') && (line[pos] != '<') && (pos < (Size - Pos)))
                            pos++;
                        if (pos == (Size - Pos))
                        {
                            Pos = -1;
                            break;
                        }
                        if (line[pos] == '<')
                            continue;
                        pos++;
                        while (line[pos] == ' ')
                            pos++;  /* skip blank*/
                        if ((line[pos] != '>') && (line[pos] != '<'))
                            break;
                    }
                    Pos += pos;
                }
                break;

            default:
                break;
            }
            if (Pos < 0) break;
            if ((Size - Pos) <= 0) break;
            Elements = Elements->cdr;
        }
        switch (Tag->Type)
        {
        case  CGITABLE : /* now find /</table */
            {
                char* TableBuffer;
                int TableSize;
                int colpos = 0;
                int rowpos = 0;
                int RowSize = 0;
                int pos;
                int First = 1;

                pos =  OSFindString (Content + Pos , Size - Pos, "</table>", 1);
                if (pos == -1)
                {
                    Pos = -1;
                    break;
                }

                pos += 8;
                TableSize =  pos;
                TableBuffer = (char*)malloc (TableSize);
                memcpy (TableBuffer, Content + Pos, TableSize);
                Pos = 0;

                while ((pos = OSFindString (TableBuffer + Pos,  TableSize - Pos, "</tr>", 1)) != -1)
                {
                    int colpos = 0;
                    int _colpos = 0;
                    int Range = 1;
                    if (First)
                    {
                        First = 0;
                        Pos += pos;
                        Pos += 5;
                        continue;
                    }
                    while ((_colpos = OSFindString (TableBuffer + Pos + colpos,  pos - colpos, "<td>", 1)) != -1)
                    {
                        char* line;
                        int colsize;
                        List* Columns = Tag->Columns;
                        int ipos = 0;
                        colpos +=_colpos;
                        colpos += 4;
                        line = TableBuffer + Pos + colpos ;
                        ipos = OSFindString (TableBuffer + Pos + colpos,  pos - colpos, "</td>", 1);
                        colsize = ipos;
                        while (Columns)
                        {
                            CGITagColumn* Column = (CGITagColumn*)Columns->car;
                            if (Column->Range == Range)
                            {
                                int i= 0;
                                int size = 0;
                                char* Value;
                                int index = CGIGetColumnIndex(pcgi, Column->Name);
                                if (index < 0) continue;
                                Value = (char*)malloc (colsize + 1);
                                sprintf (Value, "%.*s", colsize, line);
                                Value[colsize] = 0;
                                CGIAddValue (pcgi, Column->Name, Value);
                                free (Value);
                            }
                            Columns = Columns->cdr;
                        }
                        Range++;
                    }
                    Pos += pos;
                    Pos += 5;
                }
                free (TableBuffer);
            }
            break;
        case  CGITEXT :

            if (Pos >= 0)
            {
                int i = 0;
                int size = pcgi->ColumnsValues[index].Size;
                char* Value = (char*)malloc (size + 1);
                sprintf (Value, "%.*s", size, Content + Pos);
                while ((i < size) && (Value[i] != '<') && (Value[i] != '>'))
                    i++;
                Value[i] = 0;

                CGIAddValue (pcgi, Tag->Name, Value);
                free (Value);
            }
            break;
        }
        Tags = Tags->cdr;
    }
    return 1;
}


/*====================================================*/


cgiParseResultType CGIParseFormInput(CGI* pcgi, char *data, int length)
{
    int pos = 0;
    CGIFormEntry* n;
    CGIFormEntry* l = 0;

    while (pos != length)
    {
        int foundEq = 0;
        int foundAmp = 0;
        int start = pos;
        int len = 0;
        char *attr;
        char *value;

        while (pos != length)
        {
            if (data[pos] == '=')
            {
                foundEq = 1;
                pos++;
                break;
            }
            pos++;
            len++;
        }
        if (!foundEq)         break;
        if (CGIUnescapeChars(pcgi, &attr, data+start, len) != cgiUnescapeSuccess)
            return cgiParseMemory;
        start = pos;
        len = 0;
        while (pos != length)
        {
            if (data[pos] == '&')
            {
                foundAmp = 1;
                pos++;
                break;
            }
            pos++;
            len++;
        }
        /* The last pair probably won't be followed by a &, but
            that's fine, so check for that after accepting it */
        if (CGIUnescapeChars(pcgi, &value, data+start, len) != cgiUnescapeSuccess)
            return cgiParseMemory;

        /* OK, we have a new pair, add it to the list. */
        n = (CGIFormEntry *) malloc(sizeof(CGIFormEntry));

        if (!n)    return cgiParseMemory;
        n->Attribute = attr;
        n->Value = value;


        n->next = 0;
        if (!l)  pcgi->FormEntryFirst = n;
        else     l->next = n;
        l = n;
        if (!foundAmp)         break;
    }

    return cgiParseSuccess;
}

cgiParseResultType CGIParsePostFormInput(CGI* pcgi)
{
    char *input;
    cgiParseResultType result;
    int size;

    if (!pcgi->ContentLength)
        return cgiParseSuccess;

    input = (char *) malloc(pcgi->ContentLength);
    if (!input)         return cgiParseMemory;


    if ((size = fread(input, 1, pcgi->ContentLength, pcgi->In))!= (int)pcgi->ContentLength)
    {
        return cgiParseIO;
    }
    result = CGIParseFormInput(pcgi, input, pcgi->ContentLength);
    free(input);
    return result;
}


cgiParseResultType CGIParsePostMultiPartFormInput(CGI* pcgi)
{
    char *input;
    int size = 0;
    int sofar = 0;
    int pos = 0;
    int TotalSize;
    if (!pcgi->ContentLength)
        return cgiParseSuccess;

    size = (pcgi->ContentLength < MULTI_PART_BUF_SIZE) ? pcgi->ContentLength :MULTI_PART_BUF_SIZE;

    TotalSize = pcgi->ContentLength;

    input = (char *) malloc(size + 1);
    if (!input)
        return cgiParseMemory;

    while((!(feof(pcgi->In))) && (TotalSize != 0))
    {
        input[pos] = (char)fgetc(pcgi->In);
        if(pos == size)
        {
            input[pos+1] = '\0';
            size *= 2;
            input = (char *)realloc(input, (size + 1) );
        }
        TotalSize--;
        pos++;
    }
    input[pos+1] = '\0';

    /*
      cgiParseResultType result;
      int Writesize;

      while (size  != (int)pcgi->ContentLength)
      {
        writesize = fread(input, 1, pcgi->ContentLength - size, cgiIn);
        size += writesize;
        sprintf (buf, "%d--%d \r\n", pcgi->ContentLength, writesize);
        fwrite (input, 1, writesize, f);
        readsize = min (pcgi->ContentLength - size - 314, MULTI_PART_BUF_SIZE);
      }          
      result = CGIParseFormInput(pcgi, input, pcgi->ContentLength);
      free(input);
      */
    return cgiParseSuccess;
}


cgiParseResultType CGIParseCookieInput(CGI* pcgi, char *data, int length)
{
    int pos = 0;
    CGIFormEntry* n;
    CGIFormEntry* l = NULL;

    while (pos != length)
    {
        int foundEq = 0;
        int foundAmp = 0;
        int start = pos;
        int len = 0;
        char *attr;
        char *value;
        while (pos != length)
        {
            if (data[pos] == '=')
            {
                foundEq = 1;
                pos++;
                break;
            }
            pos++;
            len++;
        }
        if (!foundEq)    break;
        if (CGIUnescapeChars(pcgi, &attr, data+start, len) != cgiUnescapeSuccess)
            return cgiParseMemory;

        start = pos;
        len = 0;
        while (pos != length)
        {
            if (data[pos] == ';')
            {
                foundAmp = 1;
                pos++;
                break;
            }
            pos++;
            len++;
        }
        if (CGIUnescapeChars(pcgi, &value, data+start, len) != cgiUnescapeSuccess)
            return cgiParseMemory;


        n = (CGIFormEntry *) malloc(sizeof(CGIFormEntry));

        n->Attribute  = attr;
        n->Value = value;
        n->next  = 0;

        if (!l) pcgi->CookieEntryFirst = n;
        else    l->next = n;
        l = n;
        if (!foundAmp)         break;
    }
    return cgiParseSuccess;
}


char* CGIGetEnv(char **s, char *var)
{
    *s = getenv(var);
    if (!(*s))     *s = "";
    return *s;
}



cgiUnescapeResultType CGIUnescapeChars(CGI* pcgi, char **sp, char *cp, int len)
{
    char *s;
    cgiEscapeState escapeState = cgiEscapeRest;
    int escapedValue = 0;
    int srcPos = 0;
    int dstPos = 0;

    s = (char *) malloc(len + 1);
    if (!s)
        return cgiUnescapeMemory;

    while (srcPos < len)
    {
        int ch = cp[srcPos];
        if (escapeState == cgiEscapeRest)
        {
            if (ch == '%')     escapeState = cgiEscapeFirst;
            else if (ch == '+')     s[dstPos++] = ' ';
            else s[dstPos++] = ch;
        } else
            if (escapeState == cgiEscapeFirst)
            {
                escapedValue = pcgi->HexValue[ch] << 4;
                escapeState = cgiEscapeSecond;
            } else
                if (escapeState == cgiEscapeSecond)
                {
                    escapedValue += pcgi->HexValue[ch];
                    s[dstPos++] = escapedValue;
                    escapeState = cgiEscapeRest;
                }
        srcPos++;
    }
    s[dstPos] = '\0';
    *sp = s;
    return cgiUnescapeSuccess;
}

void CGISetupConstants(CGI* pcgi)
{
    int i;
    for (i=0; (i < 256); i++)   pcgi->HexValue[i] = 0;

    pcgi->HexValue['0'] = 0;
    pcgi->HexValue['1'] = 1;
    pcgi->HexValue['2'] = 2;
    pcgi->HexValue['3'] = 3;
    pcgi->HexValue['4'] = 4;
    pcgi->HexValue['5'] = 5;
    pcgi->HexValue['6'] = 6;
    pcgi->HexValue['7'] = 7;
    pcgi->HexValue['8'] = 8;
    pcgi->HexValue['9'] = 9;
    pcgi->HexValue['A'] = 10;
    pcgi->HexValue['B'] = 11;
    pcgi->HexValue['C'] = 12;
    pcgi->HexValue['D'] = 13;
    pcgi->HexValue['E'] = 14;
    pcgi->HexValue['F'] = 15;
    pcgi->HexValue['a'] = 10;
    pcgi->HexValue['b'] = 11;
    pcgi->HexValue['c'] = 12;
    pcgi->HexValue['d'] = 13;
    pcgi->HexValue['e'] = 14;
    pcgi->HexValue['f'] = 15;
}

cgiFormResultType CGIFormString(CGI* pcgi, char *name, char *result, int max)
{
    CGIFormEntry* e = CGIFormEntryFindFirst(pcgi, name);
    if (!e)
    {
        strcpy(result, "");
        return cgiFormNotFound;
    }
    return CGIFormEntryString(e, result, max, 1);
}


cgiFormResultType CGIFormStringNoNewlines(CGI* pcgi, char *name, char *result, int max)
{
    CGIFormEntry* e = CGIFormEntryFindFirst(pcgi, name);
    if (!e)
    {
        strcpy(result, "");
        return cgiFormNotFound;
    }
    return CGIFormEntryString(e, result, max, 0);
}

CGIFormEntry* CGIFormEntryFindFirst(CGI* pcgi, char *name)
{
    pcgi->FindTarget = name;
    pcgi->FindPos = pcgi->FormEntryFirst;
    return CGIFormEntryFindNext(pcgi);
}

CGIFormEntry* CGIFormEntryFindNext(CGI* pcgi)
{
    while (pcgi->FindPos)
    {
        CGIFormEntry *c = pcgi->FindPos;
        pcgi->FindPos = c->next;
        if (!strcmp (c->Attribute, pcgi->FindTarget))
            return c;
    }
    return 0;
}

cgiFormResultType CGIFormEntryString(CGIFormEntry *e, char *result, int max, int newlines)
{
    char *dp, *sp;
    int truncated = 0;
    int len = 0;
    int avail = max-1;
    int crCount = 0;
    int lfCount = 0;
    dp = result;
    sp = e->Value;
    while (1)
    {
        int ch;
        ch = *sp;
        if (len >= avail)
        {
            truncated = 1;
            break;
        }
        /* Fix the CR/LF, LF, CR nightmare: watch for
            consecutive bursts of CRs and LFs in whatever
            pattern, then actually output the larger number 
            of LFs. Consistently sane, yet it still allows
            consecutive blank lines when the user
            actually intends them. */
        if ((ch == 13) || (ch == 10))
        {
            if (ch == 13)     crCount++;
            else lfCount++;
        }
        else
        {
            if (crCount || lfCount)
            {
                int lfsAdd = crCount;
                if (lfCount > crCount)     lfsAdd = lfCount;
                /* Stomp all newlines if desired */
                if (!newlines) lfsAdd = 0;
                while (lfsAdd)
                {
                    if (len >= avail)
                    {
                        truncated = 1;
                        break;
                    }
                    *dp = 10;
                    dp++;
                    lfsAdd--;
                    len++;
                }
                crCount = 0;
                lfCount = 0;
            }
            if (ch == '\0')
                /* The end of the source string */
                break;
            /* 1.06: check available space before adding
                the character, because a previously added
                LF may have brought us to the limit */
            if (len >= avail)
            {
                truncated = 1;
                break;
            }
            *dp = ch;
            dp++;
            len++;
        }
        sp++;
    }
    *dp = '\0';
    if (truncated)         return cgiFormTruncated;
    else
        if (!len) return cgiFormEmpty;
        else         return cgiFormSuccess;
}


int CGIFirstNonspaceChar(char *s)
{
    int len = strspn(s, " \n\r\t");
    return s[len];
}

void CGIStringArrayFree(char **stringArray)
{
    char *p;
    p = *stringArray;
    while (p)
    {
        free(p);
        stringArray++;
        p = *stringArray;
    }
}

cgiFormResultType CGIFormStringMultiple(CGI* pcgi, char *name, char ***result)
{
    char **stringArray;
    CGIFormEntry* e;
    int i;
    int total = 0;
    /* Make two passes. One would be more efficient, but this
       function is not commonly used. The select menu and
       radio box functions are faster. */
    e = CGIFormEntryFindFirst(pcgi, name);
    if (e != 0)
    {
        do
        {
            total++;
        }
        while ((e = CGIFormEntryFindNext(pcgi)) != 0);
    }
    stringArray = (char **) malloc(sizeof(char *) * (total + 1));
    if (!stringArray)
    {
        *result = 0;
        return cgiFormMemory;
    }
    /* initialize all entries to null; the last will stay that way */
    for (i=0; (i <= total); i++)
        stringArray[i] = 0;

    /* Now go get the entries */
    e = CGIFormEntryFindFirst(pcgi, name);
    if (e)
    {
        i = 0;
        do
        {
            int max = strlen(e->Value) + 1;
            stringArray[i] = (char *) malloc(max);
            if (stringArray[i] == 0)
            {
                /* Memory problems */
                CGIStringArrayFree(stringArray);
                *result = 0;
                return cgiFormMemory;
            }
            strcpy(stringArray[i], e->Value);
            CGIFormEntryString(e, stringArray[i], max, 1);
            i++;
        } while ((e = CGIFormEntryFindNext(pcgi)) != 0);
        *result = stringArray;
        return cgiFormSuccess;
    }
    else
    {
        *result = stringArray;
        return cgiFormNotFound;
    }
}



cgiFormResultType CGIFormStringSpaceNeeded(CGI* pcgi, char *name, int *result)
{
    CGIFormEntry* e = CGIFormEntryFindFirst(pcgi, name);
    if (!e)
    {
        *result = 1;
        return cgiFormNotFound;
    }
    *result = strlen(e->Value) + 1;
    return cgiFormSuccess;
}

cgiFormResultType CGIFormInteger(CGI* pcgi, char *name, int *result, int defaultV)
{
    int ch;
    CGIFormEntry* e = CGIFormEntryFindFirst(pcgi, name);
    if (!e)
    {
        *result = defaultV;
        return cgiFormNotFound;
    }
    if (!strlen(e->Value))
    {
        *result = defaultV;
        return cgiFormEmpty;
    }
    ch = CGIFirstNonspaceChar(e->Value);
    if (!(isdigit(ch)) && (ch != '-') && (ch != '+'))
    {
        *result = defaultV;
        return cgiFormBadType;
    }
    else
    {
        *result = atoi(e->Value);
        return cgiFormSuccess;
    }
}



cgiFormResultType CGIFormIntegerBounded(CGI* pcgi, char *name, int *result, int min, int max, int defaultV)
{
    cgiFormResultType error = CGIFormInteger(pcgi, name, result, defaultV);
    if (error != cgiFormSuccess)
        return error;

    if (*result < min)
    {
        *result = min;
        return cgiFormConstrained;
    }
    if (*result > max)
    {
        *result = max;
        return cgiFormConstrained;
    }
    return cgiFormSuccess;
}



cgiFormResultType CGIFormDouble(CGI* pcgi, char *name, double *result, double defaultV)
{
    int ch;
    CGIFormEntry* e = CGIFormEntryFindFirst(pcgi, name);
    if (!e)
    {
        *result = defaultV;
        return cgiFormNotFound;
    }
    if (!strlen(e->Value))
    {
        *result = defaultV;
        return cgiFormEmpty;
    }
    ch = CGIFirstNonspaceChar(e->Value);
    if (!(isdigit(ch)) && (ch != '.') && (ch != '-') && (ch != '+'))
    {
        *result = defaultV;
        return cgiFormBadType;
    }
    else
    {
        *result = atof(e->Value);
        return cgiFormSuccess;
    }
}

cgiFormResultType CGIFormDoubleBounded(CGI* pcgi, char *name, double *result, double min, double max, double defaultV)
{

    cgiFormResultType error = CGIFormDouble(pcgi, name, result, defaultV);

    if (error != cgiFormSuccess)
        return error;

    if (*result < min)
    {
        *result = min;
        return cgiFormConstrained;
    }
    if (*result > max)
    {
        *result = max;
        return cgiFormConstrained;
    }
    return cgiFormSuccess;
}



cgiFormResultType CGIFormSelectSingle(CGI* pcgi, char *name, char **choicesText, int choicesTotal, int *result, int defaultV)
{
    int i;
    CGIFormEntry* e = CGIFormEntryFindFirst(pcgi, name);
    if (!e)
    {
        *result = defaultV;
        return cgiFormNotFound;
    }
    for (i=0; (i < choicesTotal); i++)
    {
        if (CGIStrEq(choicesText[i], e->Value))
        {
            *result = i;
            return cgiFormSuccess;
        }
    }
    *result = defaultV;
    return cgiFormNoSuchChoice;
}

cgiFormResultType CGIFormSelectMultiple(CGI* pcgi, char *name, char **choicesText, int choicesTotal,int *result, int *invalid)
{
    CGIFormEntry* e;
    int i;
    int hits = 0;
    int invalidE = 0;
    for (i=0; (i < choicesTotal); i++)
        result[i] = 0;

    e = CGIFormEntryFindFirst(pcgi, name);
    if (!e)
    {
        *invalid = invalidE;
        return cgiFormNotFound;
    }
    do
    {
        int hit = 0;
        for (i=0; (i < choicesTotal); i++)
        {
            if (CGIStrEq(choicesText[i], e->Value))
            {
                result[i] = 1;
                hits++;
                hit = 1;
                break;
            }
        }
        if (!(hit))     invalidE++;
    } while ((e = CGIFormEntryFindNext(pcgi)) != 0);

    *invalid = invalidE;

    if (hits)     return cgiFormSuccess;
    else          return cgiFormNotFound;

}

cgiFormResultType CGIFormCheckboxSingle(CGI* pcgi, char *name)
{
    CGIFormEntry* e = CGIFormEntryFindFirst(pcgi, name);
    if (!e)
        return cgiFormNotFound;

    return cgiFormSuccess;
}

cgiFormResultType CGIFormCheckboxMultiple(CGI* pcgi, char *name, char **valuesText, int valuesTotal,int *result, int *invalid)
{
    return CGIFormSelectMultiple(pcgi, name, valuesText, valuesTotal, result, invalid);
}

cgiFormResultType CGIFormRadio(CGI* pcgi, char *name,    char **valuesText, int valuesTotal, int *result, int defaultV)
{
    return CGIFormSelectSingle(pcgi, name, valuesText, valuesTotal,        result, defaultV);
}

void CGIHeaderLocation(CGI* pcgi, char *redirectUrl)
{
    fprintf(pcgi->Out, "Location: %s%c%c", redirectUrl, 10, 10);
}

void CGIHeaderStatus(CGI* pcgi, int status, char *statusMessage)
{
    fprintf(pcgi->Out, "Status: %d %s%c%c", status, statusMessage,    10, 10);
}

void CGIHeaderContentType(CGI* pcgi, char *mimeType)
{
    fprintf(pcgi->Out, "Content-type: %s%c%c", mimeType, 11 , 11);
}


int CGIWriteString(CGI* pcgi, FILE *out, char *s)
{
    int len = strlen(s);
    CGIWriteInt(pcgi, out, len);
    fprintf (out, "%.*s\n", len, s);
    return 1;
}

int CGIWriteInt(CGI* pcgi, FILE *out, int i)
{
    fprintf (out, "Int*%d*\n", i);
    return 1;
}


int CGIReadString(CGI* pcgi, FILE *in, char **s)
{
    int len;
    CGIReadInt(pcgi, in, &len);
    *s = (char *) malloc(len + 1);
    if (!(*s))     return 0;

    if ((int)fread(*s, 1, len, in) != (int)len)
        return 0;
    (*s)[len] = '\0';
    return 1;
}



int CGIReadInt(CGI* pcgi, FILE *out, int *i)
{
    if (!fread(i, sizeof(int), 1, out))      return 0;
    return 1;
}


char* CGIGetValueFormEntry (CGI* pcgi, char* Attribute)
{

    CGIFormEntry *c = pcgi->FormEntryFirst;
    while (c)
    {
        if (OSStrcmp (c->Attribute, Attribute) == 0)
            return c->Value;
        c = c->next;
    }
    return NULL;
}
