/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "mail.h"
#include "pop.h"
#include "smtp.h"




char m_ErrorMessage[500];
List* RetreiveMessages = NULL;
int m_NumberMail = 0;
int m_TotalSize = 0;
int m_ListSize[100];


#define Maximum(a,b) (((a)>= (b)) ? (a) : (b))




void EMailAddToHeader (EMailMessage* Message, char* str)
{
    if (Message->Header == NULL)
    {
        Message->Header = (char*)malloc (EMAILBUFSIZE);
        Message->HeaderSize = EMAILBUFSIZE;
        strcpy (Message->Header, str);
        return;
    }

    if ((strlen (Message->Header) + strlen (str)) > (size_t)Message->HeaderSize)
    {
        int size = Maximum (EMAILBUFSIZE, strlen (str));
        Message->Header = (char*)realloc (Message->Header, Message->HeaderSize + size);
        Message->HeaderSize += size;
    }
    strcat (Message->Header, str);
}


void EMailAddToBody (EMailMessage* Message, char* str)
{
    if (Message->Body == NULL)
    {
        Message->Body = (char*)malloc (EMAILBUFSIZE);
        Message->BodySize = EMAILBUFSIZE;
        strcpy (Message->Body, str);
        return;
    }
    if ((strlen (Message->Body) + strlen (str)) > (size_t)Message->BodySize)
    {
        int size = Maximum (EMAILBUFSIZE, strlen (str));
        Message->Body = (char*)realloc (Message->Body, Message->BodySize + size);
        Message->BodySize += size;
    }
    strcat (Message->Body, str);
}



EMailRecipient* EMailAddRecipient (EMailMessage* Message, char* EmailAddress, char* FriendlyName, BYTE Type)
{
    EMailRecipient* recipient;

    if ((EmailAddress == NULL) || (FriendlyName == NULL)) return NULL;

    if (strlen (EmailAddress) > 249) return NULL;
    if (strlen (FriendlyName) > 249) return NULL;

    recipient = (EMailRecipient *)malloc (sizeof (EMailRecipient));

    strcpy (recipient->Address , EmailAddress);
    strcpy (recipient->Name,     FriendlyName);

    switch (Type)
    {
    case EMAIL_FROM :
        ListNewr (&Message->FromList, recipient);
        break;

	case EMAIL_TO :
        ListNewr (&Message->ToList, recipient);
        break;
    case EMAIL_CC :
        ListNewr (&Message->CCList, recipient);
        break;
    case EMAIL_BCC :
        ListNewr (&Message->BCCList, recipient);
        break;
    }
    return recipient;
}


int EMailAddMultipleRecipients(EMailMessage* Message, char* szRecipients, BYTE type )
{
    char* buf;
    int pos;
    int start;
    char* sTemp;
    char sEmail[250];
    char sFriendly[250];
    int length;
    int nMark;
    int nMark2;
    char* pdest;
    char* pdest2;

    if (szRecipients == NULL ) return -1;

    length = (int)strlen( szRecipients );
    buf = (char *)malloc (length + 1);

    strcpy(buf, szRecipients);

    for( pos = 0, start = 0; pos <= length; pos++ )
    {
        if (buf[pos] == ';' || buf[pos] == ',' || buf[pos] == 0)
        {
            buf[pos] = 0;
            sTemp = &buf[start];
            pdest = strchr (sTemp , '<');

            nMark = pdest - sTemp;

            if( pdest != NULL )
            {
                strncpy (sFriendly, sTemp, nMark);
                sFriendly [nMark] = 0;

                pdest2 = strchr (sTemp, '>');
                nMark2 = pdest2 - pdest;

                if ((pdest2 == NULL) || ( pdest2 < pdest ))
                {
                    free (buf);
                    return -1;
                }
                strncpy (sEmail, pdest + 1, nMark2 - 1);
                sEmail[nMark2 - 1] = 0;
            }
            else
            {
                strcpy (sEmail, sTemp);
                strcpy (sFriendly , "");
            }

            EMailAddRecipient( Message, sEmail, sFriendly, type);
            start = pos + 1;
        }
    }
    free (buf);
    return 1;
}



EMailMessage* EMailCreateMessage (char* from, char* to, char* cc, char* bcc, char* subject, char* body)
{
    EMailMessage* Message;
    int size;

    if ((!from) || (strcmp (from, "") == 0)) return NULL;
    if ((!to) || (strcmp (to, "") == 0))     return NULL;

    Message = (EMailMessage *)malloc (sizeof (EMailMessage));
    Message->FromList  = NULL;
	Message->ToList  = NULL;
    Message->CCList  = NULL;
    Message->BCCList = NULL;
    Message->Header  = NULL;
    Message->MIMEList = NULL;           /* Parts       */
    Message->HeaderSize = 0;
    Message->BodySize = 0;
    Message->Header = NULL;
    Message->Body = NULL;

    EMailAddToHeader (Message, "");
    EMailAddToBody (Message, "");


    strcpy (Message->MailerName , "WC Mail" );

    /*MIME */
    strcpy (Message->MIMEContentType , "multipart/mixed" );
    strcpy (Message->PartBoundary , "WC_MAIL_PaRt_BoUnDaRy_05151998" );
    strcpy (Message->MIMEText , "This is a multi-part message in MIME format." );
    /*MIME*/


    Message->From = (char*)malloc (strlen (from) + 1);
    strcpy (Message->From, from);

    if (subject)
    {   size = (int)strlen (subject);
        Message->Subject = (char*)malloc (size + 1);
        strcpy (Message->Subject, subject);
    }
    else
    {
        Message->Subject = (char*)malloc (1);
        strcpy (Message->Subject, "");
    }
    if (body)
    {
        EMailAddToBody (Message, body);
    }
	
    if (from)
    {
        EMailAddMultipleRecipients (Message, from, EMAIL_FROM);
    }
	
    if (to)
    {
        EMailAddMultipleRecipients (Message, to, EMAIL_TO);
    }

    if (cc &&  (strcmp (cc, "") != 0))
    {
        EMailAddMultipleRecipients (Message, cc, EMAIL_CC);
    }

    if (bcc && (strcmp (bcc, "") != 0))
    {
        EMailAddMultipleRecipients (Message, bcc, EMAIL_BCC);
    }

    return Message;
}



void EMailFreeMessage (EMailMessage* Message)
{
    if (Message->From)    free (Message->From);
    if (Message->Subject) free (Message->Subject);
    if (Message->Body)    free (Message->Body);
    if (Message->Header)  free (Message->Header);

    while (Message->ToList)
    {
        EMailRecipient* recipient = (EMailRecipient *)Message->ToList->car;
        ListRemove (&Message->ToList, recipient);
        free (recipient);
    }
    while (Message->CCList)
    {
        EMailRecipient* recipient = (EMailRecipient *)Message->CCList->car;
        ListRemove (&Message->CCList, recipient);
        free (recipient);
    }
    while (Message->BCCList)
    {
        EMailRecipient* recipient = (EMailRecipient *)Message->BCCList->car;
        ListRemove (&Message->BCCList, recipient);
        free (recipient);
    }
    while (Message->FromList)
    {
        EMailRecipient* recipient = (EMailRecipient *)Message->FromList->car;
        ListRemove (&Message->FromList, recipient);
        free (recipient);
    }

	while (Message->MIMEList)
    {
        MimePart* part = (MimePart *)Message->MIMEList->car;
        ListRemove (&Message->MIMEList, part);
        free (part->Content);
        free (part);
    }

    free (Message);
}


int EMailAddMimePart(EMailMessage* Message, char* nContent, int nContentType, char* szParameters, int nEncoding,  BOOL bPath)
{
    MimePart* part = (MimePart *)malloc (sizeof (MimePart));
    part->ContentType = nContentType;
    strcpy (part->Parameters, szParameters);
    part->Encoding = nEncoding;
    part->Path = bPath;

    part->Content = (char*) malloc (strlen (nContent) + 1);
    strcpy (part->Content, nContent);

    /* part->Content.TrimLeft();
      part->Content.TrimRight();*/

    if (nContentType == TEXT_PLAIN)
        ListInsert (&Message->MIMEList, part);
    else
        ListNewr (&Message->MIMEList, part);
    return 1;
}



void EMailAppendMimeParts (EMailMessage* Message)
{

    List* list = Message->MIMEList;

    while (list)
    {
        MimePart* Part = (MimePart *)list->car;
        EMailAppendMimePart (Message, Part);
        list = list->cdr;
    }
}



int EMailAppendMimePart (EMailMessage* Message, MimePart* Part)
{
    char Destination[1500];
    char Buf[250];
    int l;
    if (Part->Content == NULL)
        return -1;

    /* boundary part */
    l = (int)strlen (Message->Body);
    if ((l < 3) ||  (strcmp (Message->Body + l - 2, "\r\n") != 0))
        EMailAddToBody(Message, "\r\n");
    sprintf (Buf, "--%s\r\n", Message->PartBoundary);
    EMailAddToBody(Message, Buf);

    EMailBuildMimeHeader (Message, Part, Destination);
    EMailAddToBody(Message, Destination);


    switch (Part->ContentType)
    {
    case APPLICATION_OCTETSTREAM :
        {
            FILE* file;
            int nBytesRead = 1;
            char szBuffer[BYTES_TO_READ + 1];
            char OutputBuffer[BYTES_TO_READ + 90];

            file = fopen(Part->Content, "rb");
            if (!file) return -1;

            switch( Part->Encoding )
            {
            default:
            case BASE64:
                while (nBytesRead > 0)
                {
                    nBytesRead = (int)fread(szBuffer, 1, BYTES_TO_READ, file);
                    szBuffer[nBytesRead] = 0;
                    EMailAddToBody(Message, Encode (szBuffer, nBytesRead, OutputBuffer));
                    EMailAddToBody(Message, "\r\n" );
                }
            }
            EMailAddToBody(Message, "\r\n" );

            fclose (file);
            return TRUE;
        }
        break;
    case TEXT_PLAIN :
        {
            /* sWrapped = wrap_text(Part->Content);*/
            EMailAddToBody(Message, Part->Content);
        }
        break;
    default :
        return -1;
        break;
    }
    return 1;
}


char* EMailBuildMimeHeader (EMailMessage* Message, MimePart* Part, char* sSubHeader)
{
    sSubHeader[0] = 0;

    switch (Part->ContentType)
    {
    case APPLICATION_OCTETSTREAM :
        {
            char sTemp[400];
            char Filename[400];
            char Repertory[100];

            sSubHeader[0] = 0;

            OSGetOnlyName (Part->Content, Filename, Repertory, 'D');

            if (Part->Path )
                sprintf (sTemp, "; file=%s", Filename);
            else
                sprintf (sTemp , "");

            sprintf (sSubHeader, "Content-Type: application/octet-stream%s\r\n", sTemp);
            strcat (sSubHeader, "Content-Transfer-Encoding: ");

            switch( Part->Encoding )
            {
            default:
            case BASE64:
                strcat (sSubHeader, "base64");
            }
            strcat (sSubHeader, "\r\n" );

            sprintf (sTemp, "Content-Disposition: attachment; filename=%s", Filename);
            strcat (sSubHeader, sTemp);

            strcat (sSubHeader, "\r\n\r\n" );
        }
        break;
    case TEXT_PLAIN :
        {
            sprintf (sSubHeader, "Content-Type: text/plain%s\r\n",  Part->Parameters );
            strcat (sSubHeader, "Content-Transfer-Encoding: ");

            switch (Part->Encoding)
            {
                /* This class handles only 7bit encoding, but others
                   may be added here.*/
            default:
            case _7BIT:
                strcat (sSubHeader, "7Bit");
            }
            strcat (sSubHeader, "\r\n\r\n" );
        }
        break;
    }
    return sSubHeader;
}

void EMailPrepareHeader (EMailMessage* Message)
{
    char buf [512];
    List* ToList;
    int i = 0;
    int j = 0;
    time_t ltime;
    struct tm *today;
    char timebuf[128];

    EMailRecipient * recipient = (EMailRecipient *)Message->FromList->car;
	sprintf (buf, "From: %s<%s>\r\n", recipient->Name, recipient->Address);

    EMailAddToHeader(Message, buf);
    for (j = 0; j < 2; j++)
    {
        i = 0;
        if (j == 0)
        {
            EMailAddToHeader(Message, "To: " );
            ToList = Message->ToList;
        }
        else
        {
            EMailAddToHeader(Message, "Cc: " );
            ToList = Message->CCList;
        }
        while (ToList)
        {
            EMailRecipient * recipient = (EMailRecipient *)ToList->car;
            if (i > 0) sprintf (buf, ",%s<%s>", recipient->Name, recipient->Address);
            else       sprintf (buf, "%s<%s>",  recipient->Name,  recipient->Address);
            EMailAddToHeader(Message, buf );
            ToList = ToList->cdr;
            i++;
        }
        EMailAddToHeader(Message, "\r\n" );
    }

/* subject */
    sprintf (buf, "Subject: %s\r\n" ,Message->Subject);
    EMailAddToHeader(Message, buf);
   
	
/* Date:                                               */
    time( &ltime );
    today = localtime( &ltime );
    strftime (timebuf, 128, "%a, %d %b %y %H:%M:%S %Z", today);
    strcpy (Message->Date, timebuf);

    sprintf (buf, "Date :%s\r\n", timebuf);
    EMailAddToHeader(Message, buf);



    sprintf (buf, "X-Mailer: %s\r\n" ,Message->MailerName);
    EMailAddToHeader(Message, buf);
    sprintf (buf,"MIME-Version: 1.0\r\nContent-Type: %s; boundary=%s\r\n",   Message->MIMEContentType, Message->PartBoundary);

//	sprintf (buf,"X-Mailer: Outlook Express 1.00\r\nMIME-Version: 1.0\r\nContent-Type: text/plain;\r\n\tcharset=\"iso-8859-1\"\r\nContent-Transfer-Encoding: 7bit\r\n");
    EMailAddToHeader(Message, buf);

 }


void EMailPrepareBody (EMailMessage* Message)
{
    int l;
    char Buf[250];

    if(strcmp (Message->Body, "") != 0)
    {
        EMailAddMimePart(Message, Message->Body, TEXT_PLAIN, "", _7BIT, FALSE);
        free (Message->Body);
        Message->Body = NULL;
        Message->BodySize = 0;
    }
    EMailAddToBody (Message, Message->MIMEText);
    EMailAddToBody (Message, "\r\n\r\n" );
    EMailAppendMimeParts (Message);

    l = (int)strlen (Message->Body);
    if ((l < 3) ||  (strcmp (Message->Body + l - 2, "\r\n") != 0))
        EMailAddToBody (Message, "\r\n");

    sprintf (Buf, "--%s--\r\n" , Message->PartBoundary);
    EMailAddToBody (Message, Buf);

    l = (int)strlen (Message->Body);
    if ((l < 3) ||
            (strcmp (Message->Body + l - 2, "\r\n") != 0))
        EMailAddToBody (Message, "\r\n");


//     EMailAddToBody (Message, "\r\n");

}




BOOL EMailDecodeHeader(EMailMessage* Message)
{
    char* startpos;
    char* endpos;
    char* sSearchFor;

    FILE* f = fopen ("c:\\header.txt", "wb");

    fwrite (Message->Header, strlen (Message->Header), 1, f);
    fclose (f);

    /* We can assume that there's a CR/LF before each of the tags, as the servers insert
       Received: lines on top of the mail while transporting the mail                    */

    sSearchFor ="\r\nFrom: ";
    startpos = strstr (Message->Header, sSearchFor);
    if (!startpos) return FALSE;
    sSearchFor ="\r\n";
    startpos += 8;
    endpos = strstr (startpos, sSearchFor);

    Message->From = (char*)malloc (endpos - startpos +1);
    strncpy (Message->From , startpos, (int)(endpos - startpos));
    Message->From[(int)(endpos - startpos)] = 0;

    sSearchFor="\r\nTo: ";
    startpos = strstr (Message->Header, sSearchFor);
    if (!startpos) return FALSE;
    sSearchFor ="\r\n";
    startpos += 6;
    endpos = strstr (startpos, sSearchFor);
    endpos[0] = 0;

    EMailAddMultipleRecipients(Message, startpos, EMAIL_TO);
    endpos[0] = '\r';

    sSearchFor="\r\nDate: ";
    startpos = strstr (Message->Header, sSearchFor);
    if (!startpos) return FALSE;
    sSearchFor ="\r\n";
    startpos += 8;
    endpos = strstr (startpos, sSearchFor);
    strncpy (Message->Date , startpos, (int)(endpos - startpos));
    Message->Date[(int)(endpos - startpos)] = 0;



    /*DATE=m_sHeader.Mid(startpos+sSearchFor.GetLength(),endpos));
     This is incorrect ! We have to parse the Date: line !!!
     Anyone likes to write a parser for the different formats a date string may have ?
     */
    sSearchFor ="\r\nSubject: ";
    startpos = strstr (Message->Header, sSearchFor);
    if (!startpos)
    {
        Message->Subject = (char*)malloc (1);
        strcpy (Message->Subject, "");
    }
    else
    {
        sSearchFor ="\r\n";
        startpos += 11;
        endpos = strstr (startpos, sSearchFor);

        Message->Subject = (char*)malloc (endpos - startpos +1);
        strncpy (Message->Subject , startpos, (int)(endpos - startpos));
        Message->Subject[(int)(endpos - startpos)] = 0;
    }

    sSearchFor="\r\nCc: ";
    startpos = strstr (Message->Header, sSearchFor);
    if (startpos)
    {
        sSearchFor ="\r\n";
        startpos += 6;
        endpos = strstr (startpos, sSearchFor);
        endpos[0] = 0;

        EMailAddMultipleRecipients(Message, startpos, EMAIL_CC);
        endpos[0] = '\r';
    }


    /*
      
          //ATTENTION: Cc parsing won't work, if Cc is split up in multiple lines
          // Cc: recipient1 <rec1@ab.cd>,
          //       recipient2 <rec2@ab.cd>,
          //       recipient3 <rec3@ab.cd>
          // won't work !!!
          sSearchFor="\r\nCc: ";
          startpos = m_sHeader.Find(sSearchFor);
          if (startpos >= 0) //no error if there's no Cc
          {
               endpos=m_sHeader.Mid(startpos+sSearchFor.GetLength()).Find("\r\n");
               AddMultipleRecipients(m_sHeader.Mid(startpos+sSearchFor.GetLength(),endpos));
          }
     */ 
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

void EMailDecodeBody (EMailMessage* Message)
{

    FILE* f = fopen ("c:\\body.txt", "wb");
    fwrite (Message->Body, strlen (Message->Body), 1, f);
    fclose (f);
    /*
         CString sCooked = "";
         char* szBad = "\r\n..\r\n";
         char* szGood = "\r\n.\r\n";
         int nPos;
         int nBadLength = strlen( szBad );
         if( m_sBody.Left(4) == "..\r\n" )
             m_sBody = m_sBody.Mid(1);
         while( (nPos = m_sBody.Find( szBad )) > -1 )
         {
             sCooked = m_sBody.Mid( 0, nPos );
             sCooked += szGood;
             m_sBody = sCooked + m_sBody.Right( m_sBody.GetLength() - (nPos + nBadLength) );
         }
      */
}



EMailMessage* EMailGetMessage (TPClient* pclient, int Nbr)
{
    char* sMsg;
    EMailMessage* Message;
    char* pos;
    char* body;
    int size;

    if (!POPRetrieve(pclient, Nbr)) return NULL;

    Message = (EMailMessage *)malloc (sizeof (EMailMessage));
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
    EMailAddToHeader (Message, "");
    EMailAddToBody (Message, "");

    sMsg= (char*)malloc (pclient->RequestSize + 1);
    size = pclient->RequestSize;
    memcpy (sMsg, pclient->RequestBuffer, pclient->RequestSize);
    sMsg[pclient->RequestSize] = 0;

    while ((pos = strstr (sMsg, "\r\n\r\n")) == NULL) /* while header not found */
    {
        if (TPRead (pclient->TP, pclient) < 0 )
        {
            free (sMsg);
            return NULL;
        }
        sMsg= (char*)realloc (sMsg, pclient->RequestSize + size + 1);
        memcpy (sMsg + size, pclient->RequestBuffer, pclient->RequestSize);
        size += pclient->RequestSize;
        sMsg[size] = 0;
    }
    pos[0] = 0;
    EMailAddToHeader (Message, sMsg);
    pos[0] = '\r';
    if (!EMailDecodeHeader(Message))
    {
        free (sMsg);
        return NULL;
    }
    body = (char *)malloc (size - (pos - sMsg + 4) + 1);
    memcpy (body, pos + 4, size - (pos - sMsg + 4));
    size = size - (pos - sMsg + 4);
    body[size] = 0;
    free (sMsg);
    sMsg = body;


    while ((pos = strstr (sMsg, "\r\n.\r\n")) == NULL)
    {
        if (TPRead (pclient->TP, pclient) < 0 )
        {
            free (sMsg);
            return NULL;
        }
        sMsg= (char*)realloc (sMsg, pclient->RequestSize + size + 1);
        memcpy (sMsg + size, pclient->RequestBuffer, pclient->RequestSize);
        size += pclient->RequestSize;
        sMsg[size] = 0;
    }


    /*
       sMsg=sMsg.Mid(sMsg.Find("\r\n")+2); //first line of output is +OK
       sMsg=sMsg.Left(sMsg.GetLength()-3); //last line is always .\r\n

       int br = sMsg.Find("\r\n\r\n");     //breakpoint between header and body 

       msg->m_sHeader=sMsg.Left(br);
       msg->m_sBody=sMsg.Mid(br+4);
    */
    EMailAddToBody (Message, sMsg);
    free (sMsg);
    EMailDecodeBody(Message);
    return Message;
}


int EMailSendMessage (TPClient* pclient, EMailMessage* Message)
{
    List* ToList;

    EMailPrepareHeader (Message);
    EMailPrepareBody (Message);

    if (!SMTPHelo  (pclient)) return -1;          /* Helo*/

    if (!SMTPMail  (pclient, Message->From)) return -1;          /* From*/
    ToList = Message->ToList;
    while (ToList)
    {
        EMailRecipient * recipient = (EMailRecipient *)ToList->car;
        if (!SMTPSetTo (pclient, recipient->Address)) return -1;    /* To*/
        ToList = ToList->cdr;
    }
    ToList = Message->CCList;
    while (ToList)
    {
        EMailRecipient * recipient = (EMailRecipient *)ToList->car;
        if (!SMTPSetTo (pclient, recipient->Address)) return -1;    /* CC*/
        ToList = ToList->cdr;
    }

    ToList = Message->BCCList;
    while (ToList)
    {
        EMailRecipient * recipient = (EMailRecipient *)ToList->car;
        if (!SMTPSetTo (pclient, recipient->Address)) return -1;    /* BCC*/
        ToList = ToList->cdr;
    }

    if (!SMTPData(pclient, Message->Header, Message->Body)) return -1;  /* Header, Body*/

    return 0;
}




int SendEmail (TP* tp, char* servername, EMailMessage* Message)
{
    TPClient* pclient;
    int error = 1;


    pclient = SMTPConnect(tp, servername);
    if (pclient == NULL) return -1;

    error = EMailSendMessage (pclient, Message);

    if (error != -1)
    {
        if (!SMTPQuit(pclient)) return -1;                           /* Quit*/
        TPCloseDownClient (tp, pclient);
    }
    return error;
}



int RetreiveEmail (TP* tp, char* servername, char* name, char* passwd)
{
    TPClient* pclient;
    int i;

    pclient = POPConnect(tp, servername , name , passwd);
    if (pclient != NULL)
    {
        POPStatistics (pclient);
        POPList (pclient);
        if (m_NumberMail > 0)
            for (i = 0; i < m_NumberMail; i++)
            {
                EMailMessage* Message = EMailGetMessage (pclient, i + 1);
                Message->Size = m_ListSize[i];
                ListNewr (&RetreiveMessages, Message);
            }
        /* POPDelete (pclient, 1);*/
        POPDisconnect(pclient);
    }

    return m_NumberMail;
}



