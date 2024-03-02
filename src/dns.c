/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "dns.h"

/* Names of RR types and qTypes.  Types and qTypes are the same, except
 * that T_ANY is a qType but not a type.  (You can ask for records of type
 * T_ANY, but you can't have any records of that type in the database.)
 */


DNSResourceSymbols  DNSClassSyms[] = {
                                         {C_IN,       "IN"},
                                         {C_CHAOS,    "CHAOS"},
                                         {C_HS,       "HS"},
                                         {C_HS,       "HESIOD"},
                                         {C_ANY,      "ANY"},
                                         {C_IN,       (char *)0}
                                     };

DNSResourceSymbols DNSTypeSyms[] = {
                                       {T_A,        "A",        "address"},
                                       {T_NS,       "NS",       "name server"},
                                       {T_MD,       "MD",       "mail destination (deprecated)"},
                                       {T_MF,       "MF",       "mail forwarder (deprecated)"},
                                       {T_CNAME,    "CNAME",    "canonical name"},
                                       {T_SOA,      "SOA",      "start of authority"},
                                       {T_MB,       "MB",       "mailbox"},
                                       {T_MG,       "MG",       "mail group member"},
                                       {T_MR,       "MR",       "mail rename"},
                                       {T_NULL,     "NULL",     "null"},
                                       {T_WKS,      "WKS",      "well-known service (deprecated)"},
                                       {T_PTR,      "PTR",      "domain name pointer"},
                                       {T_HINFO,    "HINFO",    "host information"},
                                       {T_MINFO,    "MINFO",    "mailbox information"},
                                       {T_MX,       "MX",       "mail exchanger"},
                                       {T_TXT,      "TXT",      "text"},
                                       {T_RP,       "RP",       "responsible person"},
                                       {T_AFSDB,    "AFSDB",    "DCE or AFS server"},
                                       {T_X25,      "X25",      "X25 address"},
                                       {T_ISDN,     "ISDN",     "ISDN address"},
                                       {T_RT,       "RT",       "router"},
                                       {T_NSAP,     "NSAP",      "nsap address"},
                                       {T_NSAP_PTR, "NSAP_PTR",  "domain name pointer"},
                                       {T_SIG,      "SIG",       "signature"},
                                       {T_KEY,      "KEY",       "key"},
                                       {T_PX,       "PX",        "mapping information"},
                                       {T_GPOS,     "GPOS",      "geographical position (withdrawn)"},
                                       {T_AAAA,     "AAAA",      "IPv6 address"},
                                       {T_LOC,      "LOC",       "location"},
                                       {T_NXT,      "NXT",       "next valid name (unimplemented)"},
                                       {T_EID,      "EID",       "endpoint identifier (unimplemented)"},
                                       {T_NIMLOC,   "NIMLOC",    "NIMROD locator (unimplemented)"},
                                       {T_SRV,      "SRV",       "server selection"},
                                       {T_ATMA,     "ATMA",      "ATM address (unimplemented)"},
                                       {T_IXFR,     "IXFR",      "incremental zone transfer"},
                                       {T_AXFR,     "AXFR",      "zone transfer"},
                                       {T_MAILB,    "MAILB",     "mailbox-related data (deprecated)"},
                                       {T_MAILA,    "MAILA",     "mail agent (deprecated)"},
                                       {T_UINFO,    "UINFO",     "user information (nonstandard)"},
                                       {T_UID,      "UID",       "user ID (nonstandard)"},
                                       {T_GID,      "GID",       "group ID (nonstandard)"},
                                       {T_NAPTR,    "NAPTR",     "URN Naming Authority"},
                                       {T_UNSPEC,   "UNSPEC",    "unspecified data (nonstandard)"},
                                       {T_ANY,      "ANY",       "\"any\""},
                                       {0,           NULL,       NULL}
                                   };

char* Print_cdname_sub (char* cp, char* msg, char* eom, char* value, int format)
{
    int n;
    char name[256];

    n = DNSExpand(msg, eom, cp, name, sizeof name);

    if (n < 0)
        return (NULL);
    if (name[0] == '\0')   (void) strcpy(name, "(root)");
    if (format)
        sprintf(value, "%-30s", name);
    else
        sprintf(value, "%s", name);
    return (cp + n);
}

char* Print_cdname (char* cp, char* msg, char* eom, char* value)
{
    return (Print_cdname_sub(cp, msg, eom, value, 0));
}


char* Print_cdname2 (char* cp, char* msg, char* eom, char* value)
{
    return (Print_cdname_sub(cp, msg, eom, value, 1));
}

int OSStripDomain (char* string, char* domain)
{
    register char* dot;

    if (*domain != '\0')
    {
        dot = string;
        while ((dot = strchr(dot, '.')) != NULL && OSStrcmp(domain, ++dot));
        if (dot != NULL)
        {
            dot[-1] = '\0';
            return 1;
        }
    }
    return 0;
}

int OSGetDomainName (char* string, char* name)
{
    register char* dot;

    if (*name != '\0')
    {
        dot = name;
        dot = strchr(dot, '.');
        if (dot != NULL)
        {
            strcpy (string , ++dot);
            return 1;
        }
    }
    return 0;
}

/*
 *  We are looking for info from answer resource records.
 *  If there aren't any, return with an error. We assume
 *  there aren't any question records.
  */

char* DNSCodeToName (DNSResourceSymbols *syms, int code, int* success)
{
    static char unname[20];

    while (syms->Name != 0)
    {
        if (code == syms->Code)
        {
            if (success)
                *success = 1;
            return (syms->Name);
        }
        syms++;
    }
    sprintf (unname, "%d", code);
    if (success)
        *success = 0;
    return (unname);
}


char* DNSCodeToHumanName (DNSResourceSymbols *syms, int code, int* success)
{
    static char unname[20];

    while (syms->Name != 0)
    {
        if (code == syms->Code)
        {
            if (success)
                *success = 1;
            return (syms->HumanName);
        }
        syms++;
    }
    sprintf(unname, "%d", code);
    if (success)
        *success = 0;
    return (unname);
}

char* DNSGetNameFromType(int type)
{
    return (DNSCodeToName (DNSTypeSyms, type, (int *)0));
}

char* DNSGetNameFromClass(int class)
{
    return (DNSCodeToName (DNSClassSyms, class, (int *)0));
}




MXMessage* DNSCheckAndReturnMessage (MX* pmx, MXCom* pcom, char* stream)
{
    DNSMessageHeader*   HeaderPtr;
    MXMessage* pmessage = NULL;
    LONG RIdentity, Identity;

    HeaderPtr = (DNSMessageHeader *)(stream + 2);
    RIdentity = ntohs((WORD)HeaderPtr->id);

    while (pcom->InputMessages)
    {
        pmessage =  (MXMessage*)pcom->InputMessages->car;
        Identity = *(LONG *)MXGetValue (pmessage, "Identity", 1);
        if (Identity != RIdentity)
        {
            ListRemove (&pcom->InputMessages, pmessage);
            MXFreeMessage (pcom->MX, pmessage);
        }
        else break;
    }
    return pmessage;
}

LONG DNSReceiveMessage (TP* ptp, TPClient* pclient, char* buf, int count)
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
    pmessage = DNSCheckAndReturnMessage (pmx, pcom, stream);
    if (!pmessage)
        return -1;

    pmessage->Com = pcom;

    if (pcom->TraceProcedure)  pcom->TraceProcedure (pmessage, MXONRECV, buf, count);


    ret = DNSReadMessage (pmx, pmessage, stream, count, 1);

    if (ret < 0)
    {
        pmessage->Complete = 1;     /*finish with this message */
    }

    TPSetBlockingMode (pcom->Client, pmessage->MessageClass->BlockedOnInput);

    if (pmessage->Complete)
    {

        TPSetBlockingMode (pcom->Client, MXNoBlock);


        /*   ListRemove(&pcom->InputMessages, pmessage);*/

        plist = pmx->ExecuteInputs;
        while (plist)
        {
            pexecuteprocedure = (MXCallBack *)plist->car;
            plist = plist->cdr;
            pexecuteprocedure->ExecuteProcedure (pmessage, pcom, pexecuteprocedure->ApplicationField);
        }

        /* Commentaire */
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
        /*     MXFreeMessage (pcom->MX, pmessage);*/
    }
    return ret;
}


LONG DNSReadMessage (MX* pmx, MXMessage* pmessage, char* stream, int streamsize, BYTE WithFileContent)
{
    DNSMessageHeader*   HeaderPtr;
    WORD    type, class;
    int dlen, nameLen;
    DWORD  ttl;
    int    n, pref, count;
    struct in_addr    inaddr;
    char   name[256];
    char   name2[256];
    BOOL   stripped;
    char*  cpstream;
    WORD   RequestSize;
    char*  eom;
    char*  Stream = stream + 2;
    char   value[256];
    char   svalue[256];
    WORD    qdcount, ancount, arcount, nscount,currentcount;
    WORD identity;
    int rcode;
    int j;
    int i = 1;

    STRING Domain = (STRING)MXGetValue (pmessage, "Domain", 1);
    WORD qType     = *(WORD *)MXGetValue (pmessage, "QType", 1);
    WORD qClass    = *(WORD *)MXGetValue (pmessage, "QClass", 1);

    RequestSize = TPGetDNSLength (stream);      /* TP dependent */


    eom = Stream + RequestSize;

    HeaderPtr = (DNSMessageHeader *)Stream;
    cpstream = (char*)HeaderPtr + DNSMessageHeaderSize;

    rcode = HeaderPtr->rcode;
    MXSetValue (pmessage, "Code", 1, &rcode);

    if (HeaderPtr->rcode != 0)
    {
        pmessage->Complete = 1;
        return 1;
    }

    rcode   = ntohs((WORD)HeaderPtr->qdcount);
    qdcount = ntohs((WORD)HeaderPtr->qdcount);
    ancount = ntohs((WORD)HeaderPtr->ancount);
    arcount = ntohs((WORD)HeaderPtr->arcount);
    nscount = ntohs((WORD)HeaderPtr->nscount);

    /*
     *  We are looking for info from answer resource records.
     *  If there aren't any, return with an error. We assume
     *  there aren't any question records.
     */



    for (n = qdcount; n > 0; n--)  /* usually one */
    {
        int  qtype;
        int  qclass;
        char domain[256];
        identity = htons((WORD)HeaderPtr->id);
        nameLen = DNSExpand(Stream, eom, cpstream, domain, sizeof domain);
        if (nameLen < 0)   return (ERROR);
        cpstream += nameLen;/* + DNSMessageQuerySize;*/
        OSGetWord(qtype, cpstream);
        OSGetWord(qclass, cpstream);
    }

    MXSetValue (pmessage, "AnswerCount",     1, &ancount);
    MXSetValue (pmessage, "AuthorityCount",  1, &arcount);
    MXSetValue (pmessage, "AdditionalCount", 1, &nscount);


    for (j = 0; j < 3; j++)
    {
        if (j == 0) currentcount = ancount;
        else
            if (j == 1) currentcount = arcount;
            else
                currentcount = nscount;


        for (count = currentcount; count > 0; count--)
        {

            *value = *svalue = 0;
            nameLen = DNSExpand(Stream, eom, cpstream, name, sizeof name);
            if (nameLen < 0)
                return (ERROR);
            cpstream += nameLen;

            OSGetWord(type, cpstream);


            OSGetWord(class, cpstream);
            OSGetDword(ttl,  cpstream);
            OSGetWord(dlen,  cpstream);

            if (name[0] == 0)
                strcpy(name, "(root)");

            stripped = FALSE;
            if (type != T_SOA)
                stripped = OSStripDomain (name, Domain);

            if (!stripped && nameLen < sizeof(name)-1)
                strcat(name, ".");

            MXSetValue (pmessage, "Section", i, &j);
            MXSetValue (pmessage, "Name",       i, name);
            MXSetValue (pmessage, "Type",       i, &type);

            switch (type)
            {
            case T_A:
                if (class == C_IN)
                {
                    memcpy((char *)&inaddr, cpstream, INADDRSZ);
                    if (dlen == 4)
                        sprintf(value," %s", inet_ntoa(inaddr));
                    else
                        if (dlen == 7)
                            sprintf(value," %s (%d, %d)", inet_ntoa(inaddr), cpstream[4],(cpstream[5] << 8) + cpstream[6]);
                        else
                            sprintf(value, " (dlen = %d?)", dlen);
                }
                cpstream += dlen;
                break;
            case T_CNAME:
            case T_MB:
            case T_MG:
            case T_MR:
            case T_NS:
                nameLen = DNSExpand(Stream, eom, cpstream, name2, sizeof name2);
                if (nameLen < 0)
                {
                    sprintf(value, " ***\n");
                    return (ERROR);
                }
                sprintf (value, " %s", name2);
                cpstream += nameLen;
                break;
            case T_PTR:
            case T_NSAP_PTR:
                if (qType != T_ANY)
                    sprintf(value," %s = ", type == T_PTR ? "host" : "server");
                cpstream = Print_cdname2(cpstream, Stream, eom, svalue);
                strcat (value, svalue);
                if (!cpstream)
                {
                    strcat (value, " ***\n");
                    return (ERROR);
                }
                break;
            case T_HINFO:
            case T_ISDN:
                {
                    char *cpstream2 = cpstream + dlen;
                    if (n = *cpstream++)
                    {
                        (void)sprintf(name,"%.*s", n, cpstream);
                        sprintf(value," %-10s", name);
                        cpstream += n;
                    }
                    else
                        sprintf(value," %-10s", " ");
                    if (cpstream == cpstream2)
                        break;

                    if (n = *cpstream++)
                    {
                        sprintf(svalue,"  %.*s", n, cpstream);
                        strcat (value, svalue);
                        cpstream += n;
                    }
                }
                break;
            case T_SOA:
                nameLen = DNSExpand(Stream, eom, cpstream, name2, sizeof name2);
                if (nameLen < 0)
                {
                    sprintf(value, " ***\n");
                    return (ERROR);
                }
                cpstream += nameLen;
                sprintf(value, " %s", name2);
                nameLen = DNSExpand(Stream, eom, cpstream, name2, sizeof name2);
                if (nameLen < 0)
                {
                    strcat(value, " ***\n");
                    return (ERROR);
                }
                cpstream += nameLen;
                sprintf(svalue, " %s. (", name2);
                strcat (value, svalue);
                for (n = 0; n < 5; n++)
                {
                    DWORD u;
                    OSGetDword(u, cpstream);
                    sprintf(svalue,"%s%lu", n? " " : "", u);
                    strcat (value, svalue);
                }
                strcat (value, ")");
                break;
            case T_MX:
            case T_AFSDB:
            case T_RT:
                OSGetWord(pref, cpstream);
                sprintf(svalue, " %-3d ", pref);
                strcat(value, svalue);
                nameLen = DNSExpand(Stream, eom, cpstream, name2, sizeof name2);
                if (nameLen < 0)
                {
                    strcat(value, " ***\n");
                    return (ERROR);
                }
                sprintf(svalue, " %s", name2);
                strcat (value, svalue);
                cpstream += nameLen;
                break;
            case T_PX:
                OSGetWord(pref, cpstream);
                sprintf(value," %-3d ",pref);
                nameLen = DNSExpand(Stream, eom, cpstream, name2, sizeof name2);
                if (nameLen < 0)
                {
                    strcat(value, " ***\n");
                    return (ERROR);
                }
                sprintf(svalue, " %s", name2);
                strcat (value, svalue);
                cpstream += nameLen;
                nameLen = DNSExpand(Stream, eom, cpstream, name2, sizeof name2);
                if (nameLen < 0)
                {
                    strcat(value, " ***\n");
                    return (ERROR);
                }
                sprintf(svalue, " %s", name2);
                strcat (value, svalue);
                cpstream += nameLen;
                break;

            case T_X25:
                if (n = *cpstream++)
                {
                    sprintf(value,"  %.*s", n, cpstream);
                    cpstream += n;
                }
                break;
            case T_TXT:
                {
                    char *cpstream2 = cpstream + dlen;
                    int c;
                    while (cpstream < cpstream2)
                    {
                        strcat (value,  "\"");
                        if (n = *cpstream++)
                            for (c = n; c > 0 && cpstream < cpstream2; c--)
                            {
                                if (strchr("\n\"\\", *cpstream))
                                    (void) strcat(value, "\\");
                                sprintf (svalue, "%c", *cpstream++);
                                strcat (value, svalue);
                            }
                        strcat (value, "\"");
                        if (cpstream < cpstream2)
                            strcat (value, " ");
                    }
                }
                break;

            case T_NSAP:
                /*         sprintf(value, " %s", inet_nsap_ntoa(dlen, cpstream, NULL));*/
                cpstream += dlen;
                break;

            case T_AAAA:
                {
                    /*        char t[sizeof "ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255"];    */
                    /*        sprintf(value, " %s", inet_ntop(AF_INET6, cpstream, t, sizeof t)); */
                    cpstream += dlen;
                    break;
                }

            case T_LOC:
                {
                    /*         char t[255];*/
                    /*         sprintf(value, "\t%s", loc_ntoa(cpstream, t));*/
                    cpstream += dlen;
                    break;
                }

            case T_SRV:
                {
                    u_int priority, weight, port;

                    OSGetWord(priority, cpstream);
                    OSGetWord(weight  , cpstream);
                    OSGetWord(port    ,cpstream);
                    sprintf(value, "\t%u %u %u ", priority, weight, port);
                    if ((cpstream = Print_cdname(cpstream, Stream, eom, svalue)) == NULL)
                        return (ERROR);
                    strcat (value, svalue);
                    break;
                }

            case T_NAPTR:
                {
                    u_int order, preference;

                    OSGetWord(order, cpstream);
                    sprintf(value, "\t%u", order);

                    OSGetWord(preference, cpstream);
                    sprintf(svalue, " %u", preference);
                    strcat (value, svalue);

                    if (n = *cpstream++)
                    {
                        sprintf(svalue, " \"%.*s\"", (int)n, cpstream);
                        strcat (value, svalue);
                        cpstream += n;
                    }
                    if (n = *cpstream++)
                    {
                        sprintf(svalue, " \"%.*s\"", (int)n, cpstream);
                        strcat (value, svalue);
                        cpstream += n;
                    }
                    if (n = *cpstream++)
                    {
                        sprintf(svalue, " \"%.*s\"", (int)n, cpstream);
                        strcat (value, svalue);
                        cpstream += n;
                    }
                    if ((cpstream = Print_cdname(cpstream, Stream, eom, svalue)) == NULL)
                        return (ERROR);
                    strcat (value, svalue);
                    break;
                }

            case T_MINFO:
            case T_RP:
                cpstream = Print_cdname(cpstream, Stream, eom, value);

                if (!cpstream)
                {
                    strcat(value, " ***\n");
                    return (ERROR);
                }
                strcat (value, "  ");
                cpstream = Print_cdname(cpstream, Stream, eom, svalue);
                if (!cpstream)
                {
                    strcat(value, " ***\n");
                    return (ERROR);
                }
                strcat (value, svalue);
                break;
            case T_UINFO:
                sprintf(value, " %s", cpstream);
                cpstream += dlen;
                break;
            case T_UID:
            case T_GID:
                {
                    DWORD l_uid;
                    OSGetDword(l_uid, cpstream);
                    sprintf(value, " %lu", l_uid);
                    cpstream += dlen - 4;
                }
                break;
            case T_WKS:
                if (class == C_IN)
                {
                    struct protoent *pp;
                    struct servent *ss;
                    WORD port;

                    cpstream += 4;
                    dlen -= 4;
                    /*             setprotoent(1);
                                   setservent(1);
                    */
                    n = *cpstream & 0377;
                    pp = getprotobynumber(n);
                    if (pp == 0)
                        sprintf(value," %-3d ", n);
                    else
                        sprintf(value," %-3s ", pp->p_name);
                    cpstream++; dlen--;

                    port = 0;
                    while (dlen-- > 0)
                    {
                        n = *cpstream++;
                        do
                        {
                            if (n & 0200)
                            {
                                ss = getservbyport((int)htons(port),  pp->p_name);
                                if (ss == 0)
                                    sprintf(svalue," %u", port);
                                else
                                    sprintf(svalue," %s", ss->s_name);
                                strcat (value, svalue);
                            }
                            n <<= 1;
                        } while (++port & 07);
                    }
                    /*           endprotoent();
                                 endservent();
                    */
                }
                break;
            }
            MXSetValue (pmessage, "Value", i, value);
            i++;
        }
    }
    pmessage->Complete = 1;
    MXInitMessage (pmessage);
    return(SUCCESS);
}



#define RRFIXEDSZ    10        /* #/bytes of fixed data in r record */

LONG DNSWriteMessage (MX* pmx, MXMessage* pmessage, char* stream, int maxcount, BYTE WithFileContent)
{
    DNSMessageHeader* hp;
    char* tostream;
    int n;
    char* dnptrs[20];
    char** dpp;
    char** lastdnptr;
    int   Type = T_AXFR;

    LONG   Identity        = *(LONG*)MXGetValue (pmessage,   "Identity",  1);
    WORD   Operation       = *(WORD*)MXGetValue (pmessage,   "Operation", 1);
    STRING Domain          = (STRING)MXGetValue (pmessage, "Domain",    1);
	WORD   Class           = *(WORD *)MXGetValue (pmessage,    "QClass",     1);
	WORD   QType           = *(WORD *)MXGetValue (pmessage,    "QType",     1);
    BUFFERPARM* Buffer     = (BUFFERPARM*)MXGetValue (pmessage, "RecordData", 1);

    char* data  = Buffer->BufferContent;
    int datalen = Buffer->BufferSize;

    if ((stream == NULL) || (maxcount < DNSMessageHeaderSize ))
        return -1;

    memset (stream, 0, DNSMessageHeaderSize );

    hp = (DNSMessageHeader *)stream;

    hp->id     = htons((WORD)Identity);
    hp->opcode = Operation;
    hp->rd     = 1; /* (_res.options & RES_RECURSE) != 0;*/
    hp->rcode  = 0;

    tostream   = stream + DNSMessageHeaderSize ;
    maxcount    -= DNSMessageHeaderSize ;

    dpp        = dnptrs;
    *dpp++     = stream;
    *dpp++     = NULL;
    lastdnptr  = dnptrs + sizeof dnptrs / sizeof dnptrs[0];

    switch (Operation)
    {
    case QUERY:
    case NS_NOTIFY_OP:

        if (((maxcount -= DNSMessageQuerySize ) < 0) ||
                ((n = DNSComp(Domain, tostream, maxcount, dnptrs, lastdnptr)) < 0))
        {
            if (Buffer->BufferSize)
            {
                free (Buffer->BufferContent);
                free (Buffer);
            }
            return -1;
        }
        tostream += n;
        maxcount -= n;

        OSPutWord(QType,  tostream);
        OSPutWord(Class, tostream);

        hp->qdcount = htons(1);

        if (Operation == QUERY || data == NULL)
            break;

        maxcount -= RRFIXEDSZ ;
        n = DNSComp((char *)data, tostream, maxcount, dnptrs, lastdnptr);

        if (n < 0)
        {
            if (Buffer->BufferSize)
            {
                free (Buffer->BufferContent);
                free (Buffer);
            }
            return -1;
        }
        tostream += n;
        maxcount -= n;
        OSPutWord(0,     tostream);
        OSPutWord(Class, tostream);
        OSPutDword(0,    tostream);
        OSPutWord(0,     tostream);
        hp->arcount = htons(1);
        break;

    case IQUERY:                /* Initialize answer section  */

        if (maxcount < 1 + DNSMessageRecordSize  + datalen)
            return -1;
        *tostream++ = '\0';    /* no domain name */

        OSPutWord(QType,  tostream);
        OSPutWord(Class, tostream);
        OSPutDword(0,    tostream);
        OSPutWord(0,     tostream);
        if (datalen)
        {
            memcpy (tostream, data, datalen);
            tostream += datalen;
        }
        hp->ancount = htons(1);
        break;

    default:
        if (Buffer->BufferSize)  free (Buffer->BufferContent);
        free (Buffer);
        return -1;
        break;
    }
    if (Buffer->BufferSize)
    {
        free (Buffer->BufferContent);
        free (Buffer);
    }
    pmessage->Complete = 1;
    MXInitMessage (pmessage);
    return (LONG)(tostream - stream);
}



LONG DNSSendMessage (TP* ptp, TPClient* pclient, char* buf, int size)
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

    ret = DNSWriteMessage (pmx, pmessage, buf, size - 2, 1); /*client->TransSize - tp header;*/

    if (ret < 0)
    {
        ListRemove(&pcom->OutputMessages, pmessage);
        MXFreeMessage (pcom->MX, pmessage);
        return -1;
    }
    if (pcom->TraceProcedure) pcom->TraceProcedure (pmessage, MXONSEND, buf, ret);

    if (pmessage->Complete)
    {
        /* dns */
        MXMessage* pmessage1  = MXCreateMessage (pmx, DNS_SYS , "Reply");
        LONG Identity    = *(LONG*)MXGetValue (pmessage, "Identity", 1);
        WORD   Operation = *(WORD*)MXGetValue (pmessage, "Operation",1);
        WORD   Class     = *(WORD*)MXGetValue (pmessage, "QClass",    1);
        WORD   Type      = *(WORD*)MXGetValue (pmessage, "QType",    1);

        MXSetValue (pmessage1, "Identity",    1, &Identity);
        MXSetValue (pmessage1, "QClass",       1, &Class);
        MXSetValue (pmessage1, "QType",       1, &Type);
        MXSetValue (pmessage1, "Operation",   1, &Operation);
        MXSetValue (pmessage1, "Domain",      1, MXGetValue (pmessage, "Domain", 1));

        ListNewr (&pcom->InputMessages, pmessage1);
        /* dns */
        TPSetBlockingMode (pcom->Client, MXNoBlock);
        ListRemove (&pcom->OutputMessages, pmessage);
        if (!(pmessage->Type & MXACK))
            MXFreeMessage (pcom->MX, pmessage);
        else
            ListNewr (&pcom->AckMessages, pmessage);
    }

    return ret;
}

