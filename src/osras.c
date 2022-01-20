/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "osras.h"

#ifdef _WIN321
HRASCONN   hRasConn;
#endif




#define        MAX_CONN  8    


DWORD OSRasError(DWORD rc, char* Buf, WORD size)
{
#ifdef _WIN321 
    return RasGetErrorString (rc, Buf, size);
#endif  
    return 0;
}

DWORD OSRasDial(char *lpszEntryName, char *lpszUserName, char *lpszPassword)
{

    DWORD result = 0;
#ifdef _WIN321
    RASDIALPARAMS    rdparams;

    memset(&rdparams, 0, sizeof(rdparams));
    rdparams.dwSize = sizeof(rdparams);

    strcpy((rdparams.szEntryName), lpszEntryName);

    rdparams.szPhoneNumber[0] = '\0';
    rdparams.szCallbackNumber[0] = '\0';

    strcpy((rdparams.szUserName), lpszUserName);
    strcpy((rdparams.szPassword), lpszPassword);

    rdparams.szDomain[0] = '*';
    rdparams.szDomain[1] = '\0';

    result =  RasDial( NULL, NULL,&rdparams, 0xFFFFFFFF, NULL, &hRasConn);
#endif
    return result;
}


DWORD  OSRasHangUp (void)
{
#ifdef _WIN321
    return RasHangUp(hRasConn);
#else
    return 1;
#endif
}



DWORD  OSRasGetConnectStatus(void)
{
#ifdef _WIN321

    DWORD nStatRet;
    RASCONNSTATUS RasConnStatus;

    RasConnStatus.dwSize = sizeof(RasConnStatus);
    nStatRet = RasGetConnectStatus(hRasConn, &RasConnStatus);
    return nStatRet;
    /*
      if  (nStatRet != 0)
        return RasConnStatus.dwError; 
      else
             return RasConnStatus.rasconnstate;
    */
#else
    return 1;
#endif
}




DWORD  OSRasGetConnectHandle (char* lpszEntryName)
{

#ifdef _WIN321
    RASCONN RasConn[MAX_CONN];

    DWORD nEnumRet;
    DWORD cbsize, nConnections, i;

    RasConn[0].dwSize = sizeof (RasConn[0]);
    cbsize = sizeof(RasConn[0]) * MAX_CONN;
    nEnumRet = RasEnumConnections(&RasConn[0], &cbsize, &nConnections);

    if (nEnumRet != 0 )          return nEnumRet;
    if (nConnections == 0) return (0xFFFFFFFF);

    for (i = 0; i < nConnections; i++)
        if (OSStrcmp (lpszEntryName, RasConn[i].szEntryName) == 0)
            hRasConn = RasConn[i].hrasconn;
#endif
    return 0;
}

