/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "sc.h"




SCPROCRESULT SCRmdirProc (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    int Error = 0;
    WORD NbParam;
    char FileLoc = LOCDISQU;


    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTMINPARAM(thisaction, 1);
    SCASSERTMAXPARAM(thisaction, 2);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);

    NbParam = thisaction->NbParam;
    if(NbParam > 1)
    {
        Val2 = SCGetParamValue (psc, thisaction, 2);
        SCASSERTDEFINEDVALUE (Val2);
        SCASSERTVALUE (Val2, SCCHAR);
        SCASSERTVALUESIZE(Val2, 2);
        FileLoc = *(char*)Val2->Val;
    }

    OSRemoveDir ((char*)Val1->Val, FileLoc, &Error);

    if (!Error) SCCopyValue (psc, thisaction->Value, Val1);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/



SCPROCRESULT SCRrmdirProc(SC* psc,  SCAction* thisaction)
{

    SCValue* Val = NULL;
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    MXCom* pcom = psc->Com;
    char DistantFileLoc;
    char* Reper = NULL;
    WORD NbParam;
    WORD range = 1;

    DistantFileLoc = LOCDISQU;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTMINPARAM (thisaction, 1);
    SCASSERTMAXPARAM (thisaction, 3);


    Val = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val);
    SCASSERTORVALUES (Val, SCPOINTER, SCLONG);

    pcom = (MXCom *)Val->Val;
    SCASSERTMINPARAM (thisaction, 2);
    Val1 = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTVALUE (Val1, SCSTRING);

    SCASSERTNOTNULL (pcom);

    Reper = (char*)Val1->Val;

    NbParam = thisaction->NbParam;
    if(NbParam == range)
    {
        Val2 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val2);
        SCASSERTVALUE (Val2, SCCHAR);
        SCASSERTVALUESIZE(Val2, 2);
        DistantFileLoc = *(char*)Val2->Val;
    }

    DGRemoveDir (pcom, thisaction->Identity, Reper, DistantFileLoc);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/



SCPROCRESULT SCCdProc (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    int Error = 0;
    WORD NbParam;
    char FileLoc = LOCDISQU;


    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);


    SCASSERTMINPARAM(thisaction, 1);
    SCASSERTMAXPARAM(thisaction, 2);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);


    NbParam = thisaction->NbParam;
    if(NbParam > 1)
    {
        Val2 = SCGetParamValue (psc, thisaction, 2);
        SCASSERTDEFINEDVALUE (Val2);
        SCASSERTVALUE (Val2, SCCHAR);
        SCASSERTVALUESIZE(Val2, 2);
        FileLoc = *(char*)Val2->Val;
    }


    OSChangeDir ((char*)Val1->Val, FileLoc, &Error);

    if (!Error) SCCopyValue (psc, thisaction->Value, Val1);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*je suis la */
/*------------------------------------------------------------------------*/



SCPROCRESULT SCRcdProc (SC* psc,  SCAction* thisaction)
{
    SCValue* Val = NULL;
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    MXCom* pcom = psc->Com;
    char DistantFileLoc;
    char* Reper = NULL;
    WORD NbParam;
    WORD range = 1;

    DistantFileLoc = LOCDISQU;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTMINPARAM (thisaction, 1);
    SCASSERTMAXPARAM (thisaction, 3);


    Val = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val);
    SCASSERT3ORVALUES (Val, SCPOINTER, SCLONG, SCCHAR);

    pcom = (MXCom *)Val->Val;
    SCASSERTMINPARAM (thisaction, 2);
    Val1 = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);

    SCASSERTNOTNULL (pcom);

    Reper = (char*)Val1->Val;

    NbParam = thisaction->NbParam;
    if(NbParam == range)
    {
        Val2 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val2);
        SCASSERTVALUE (Val2, SCCHAR);
        SCASSERTVALUESIZE(Val2, 2);
        DistantFileLoc = *(char*)Val2->Val;
    }


    DGChangeDir (pcom, thisaction->Identity, Reper, DistantFileLoc);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/



SCPROCRESULT SCMkdirProc (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    int Error = 0;
    WORD NbParam;
    char FileLoc = LOCDISQU;


    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTMINPARAM(thisaction, 1);
    SCASSERTMAXPARAM(thisaction, 2);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);

    NbParam = thisaction->NbParam;
    if(NbParam > 1)
    {
        Val2 = SCGetParamValue (psc, thisaction, 2);
        SCASSERTDEFINEDVALUE (Val2);
        SCASSERTVALUE (Val2, SCCHAR);
        SCASSERTVALUESIZE(Val2, 2);
        FileLoc = *(char*)Val2->Val;
    }

    OSMakeDir ((char*)Val1->Val, FileLoc, &Error);

    thisaction->Error= Error;

    if (!Error) SCCopyValue (psc, thisaction->Value, Val1);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/



SCPROCRESULT SCRmkdirProc(SC* psc,  SCAction* thisaction)
{
    SCValue* Val = NULL;
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    MXCom* pcom = psc->Com;
    char DistantFileLoc;
    char* Reper = NULL;
    WORD NbParam;
    WORD range = 1;

    DistantFileLoc = LOCDISQU;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTMINPARAM (thisaction, 1);
    SCASSERTMAXPARAM (thisaction, 3);


    Val = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val);
    SCASSERTORVALUES (Val, SCPOINTER, SCLONG);
    pcom = (MXCom *)Val->Val;
    SCASSERTMINPARAM (thisaction, 2);
    Val1 = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTVALUE (Val1, SCSTRING);

    SCASSERTNOTNULL (pcom);

    Reper = (char*)Val1->Val;

    NbParam = thisaction->NbParam;
    if(NbParam == range)
    {
        Val2 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val2);
        SCASSERTVALUE (Val2, SCCHAR);
        SCASSERTVALUESIZE(Val2, 2);
        DistantFileLoc = *(char*)Val2->Val;
    }


    DGMakeDir (pcom, thisaction->Identity, Reper, DistantFileLoc);

    return thisaction->Value;
}


/*------------------------------------------------------------------------*/


SCPROCRESULT SCDeleteProc(SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    int Error = 0;
    int FinalError = 0;
    List* l;
    List* ListFiles = NULL;
    WORD NbParam;
    char FileLoc = LOCDISQU;


    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTMINPARAM(thisaction, 1);
    SCASSERTMAXPARAM(thisaction, 2);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);


    NbParam = thisaction->NbParam;
    if(NbParam > 1)
    {
        Val2 = SCGetParamValue (psc, thisaction, 2);
        SCASSERTDEFINEDVALUE (Val2);
        SCASSERTVALUE (Val2, SCCHAR);
        SCASSERTVALUESIZE(Val2, 2);
        FileLoc = *(char*)Val2->Val;
    }


    OSGetFilesFromWildCards ((char *)Val1->Val, &ListFiles, FileLoc, TRUE);

    l = ListFiles;
    while (l)
    {
        OSRemoveFile (((FILEDATA *)l->car)->Name, FileLoc, &Error);
        if (Error) FinalError = Error;
        l = l->cdr;
    }

    while (ListFiles)
    {
        free (ListFiles->car);
        ListRemove (&ListFiles, ListFiles->car);
    }


    if (!FinalError) SCCopyValue (psc, thisaction->Value, Val1);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, FinalError);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/



SCPROCRESULT SCRdeleteProc(SC* psc, SCAction* thisaction)
{

    SCValue* Val = NULL;
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    MXCom* pcom = psc->Com;
    char DistantFileLoc;
    char* Reper = NULL;
    WORD NbParam;
    WORD range = 1;

    DistantFileLoc = LOCDISQU;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTMINPARAM (thisaction, 1);
    SCASSERTMAXPARAM (thisaction, 3);


    Val = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val);
    SCASSERTORVALUES (Val, SCPOINTER, SCLONG);

    pcom = (MXCom *)Val->Val;
    SCASSERTMINPARAM (thisaction, 2);
    Val1 = SCGetParamValue (psc, thisaction, range++);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTVALUE (Val1, SCSTRING);

    SCASSERTNOTNULL (pcom);

    Reper = (char*)Val1->Val;

    NbParam = thisaction->NbParam;
    if(NbParam == range)
    {
        Val2 = SCGetParamValue (psc, thisaction, range++);
        SCASSERTDEFINEDVALUE (Val2);
        SCASSERTVALUE (Val2, SCCHAR);
        SCASSERTVALUESIZE(Val2, 2);
        DistantFileLoc = *(char*)Val2->Val;
    }


    DGRemoveFiles (pcom, thisaction->Identity, Reper, DistantFileLoc);

    return thisaction->Value;
}



/*------------------------------------------------------------------------*/


SCPROCRESULT SCPwdProc   (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    int Error = 0;
    char* Repertory = NULL;
    char Reper[250];
    WORD NbParam;
    char FileLoc = LOCDISQU;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTMAXPARAM(thisaction, 1);


    NbParam = thisaction->NbParam;
    if(NbParam == 1)
    {
        Val1 = SCGetParamValue (psc, thisaction, 1);
        SCASSERTDEFINEDVALUE (Val1);
        SCASSERTVALUE (Val1, SCCHAR);
        SCASSERTVALUESIZE(Val1, 2);
        FileLoc = *(char*)Val1->Val;
    }


    OSGetDir (Reper, FileLoc, &Error);

    SCPutValue (thisaction->Value, Reper, SCSTRING);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}


/*------------------------------------------------------------------------*/






SCPROCRESULT SCExistProc (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    int Error = 0;
    BOOL exist = TRUE;
    char FileLoc = LOCDISQU;
    WORD NbParam;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTMINPARAM(thisaction, 1);
    SCASSERTMAXPARAM(thisaction, 2);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCSTRING, SCCHAR);

    NbParam = thisaction->NbParam;
    if(NbParam > 1)
    {
        Val2 = SCGetParamValue (psc, thisaction, 2);
        SCASSERTDEFINEDVALUE (Val2);
        SCASSERTVALUE (Val2, SCCHAR);
        FileLoc = *(char*)Val2->Val;
    }

    if (OSVerifExistence ((char*)Val1->Val, FileLoc, &Error) == 0)
        exist = FALSE;

    if (exist) SCCopyValue (psc, thisaction->Value, Val1);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}


/*------------------------------------------------------------------------*/

SCPROCRESULT SCOpenProc (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    SCValue* Val4 = NULL;
    int Error = 0;
    char FileLoc  = LOCDISQU;
    char FileType = 't';
    FILEID* fileid;
    WORD NbParam;

    SCSETRETURNVALUETYPE(thisaction->Value, SCPOINTER);

    SCASSERTMINPARAM(thisaction, 3);
    SCASSERTMAXPARAM(thisaction, 4);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTVALUE (Val1, SCSTRING);

    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTVALUE (Val2, SCSTRING);

    Val3 = SCGetParamValue (psc, thisaction, 3);
    SCASSERTDEFINEDVALUE (Val3);
    SCASSERTVALUE (Val3, SCCHAR);
    FileType = *(char*)Val3->Val;


    NbParam = thisaction->NbParam;
    if(NbParam > 3)
    {
        Val4 = SCGetParamValue (psc, thisaction, 4);
        SCASSERTDEFINEDVALUE (Val4);
        SCASSERTVALUE (Val4, SCCHAR);
        FileLoc = *(char*)Val4->Val;
    }


    fileid = OSOpenFile ((char *)Val1->Val , (char *)Val2->Val, FileType, FileLoc, &Error);

    SCAffectValue (thisaction->Value, fileid, SCPOINTER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/


SCPROCRESULT SCCloseProc (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    int Error = 0;
    char FileLoc = LOCDISQU;
    WORD NbParam;


    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);

    SCASSERTMINPARAM(thisaction, 1);
    SCASSERTMAXPARAM(thisaction, 2);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCPOINTER, SCLONG);

    SCASSERTNOTNULL (Val1->Val);

    NbParam = thisaction->NbParam;
    if (NbParam > 1)
    {
        Val2 = SCGetParamValue (psc, thisaction, 2);
        SCASSERTDEFINEDVALUE (Val2);
        SCASSERTVALUE (Val2, SCCHAR);
        FileLoc = *(char*)Val2->Val;
    }


    Error = OSCloseFile ((FILEID *)Val1->Val);

    SCPutValue (Val1, NULL, SCPOINTER);

    SCAffectValue (thisaction->Value, &Error, SCINTEGER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}

/*------------------------------------------------------------------------*/


SCPROCRESULT SCReadProc (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    SCValue* Val4 = NULL;
    int Error = 0;
    LONG result;
    char FileLoc = LOCDISQU;
    WORD NbParam;

    SCSETRETURNVALUETYPE(thisaction->Value, SCLONG);

    SCASSERTMINPARAM(thisaction, 3);
    SCASSERTMAXPARAM(thisaction, 4);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTORVALUES (Val1, SCLONG, SCPOINTER);
    SCASSERTNOTNULL (Val1->Val);



    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTORVALUES (Val2, SCINTEGER, SCLONG);

    Val3 = SCGetParamValue (psc, thisaction, 3);
    SCASSERTDEFINEDVALUE (Val3);
    SCASSERTORVALUES (Val3, SCPOINTER, SCLONG);


    NbParam = thisaction->NbParam;
    if(NbParam > 3)
    {
        Val4 = SCGetParamValue (psc, thisaction, 4);
        SCASSERTDEFINEDVALUE (Val4);
        SCASSERTVALUE (Val4, SCCHAR);
        FileLoc = *(char*)Val4->Val;
    }


    result = OSReadFile ((FILEID *)Val3->Val, (char *)Val1->Val, (LONG)Val2->Val, &Error);

    SCAffectValue (thisaction->Value, &result, SCLONG);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}



SCPROCRESULT SCStrLengthProc (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    int result = 0;

    int Error = 0;


    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);

    SCASSERTPARAMNUMBER(thisaction, 1);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTVALUE (Val1, SCSTRING);

    if (Val1->Val)
        result = strlen ((char*)Val1->Val);

    SCAffectValue (thisaction->Value, (void*)result, SCINTEGER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}


SCPROCRESULT SCStrTrimProc (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    int length = 0;
    char* fromstring;
    char* tostring;
    int Error = 0;

    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTPARAMNUMBER(thisaction, 1);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTVALUE (Val1, SCSTRING);

    if (Val1->Val)
    {
        int i;
        fromstring = (char*)Val1->Val;
        length = strlen (fromstring);
        tostring = (char*)malloc (length + 1);
        memset (tostring, 0, length + 1);

        while ((*fromstring) && (*fromstring == ' ')) fromstring++;
        if (*fromstring)
            memcpy (tostring , fromstring, strlen (fromstring));
        i = strlen (tostring) - 1;
        if (i > 0)
        {
            while (*(tostring + i) == ' ') i--;
            *(tostring + i + 1) = 0;
        }

        SCPutValue (thisaction->Value, tostring, SCSTRING);
        free (tostring);
    }

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}





SCPROCRESULT SCGetlineProc (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    char* result;
    char Buffer[4096];
    FILEID* fileid = NULL;
    WORD NbParam;
    int Error = 0;


    SCSETRETURNVALUETYPE(thisaction->Value, SCSTRING);

    SCASSERTMAXPARAM(thisaction, 1);

    NbParam = thisaction->NbParam;
    if (NbParam == 1)
    {
        Val1 = SCGetParamValue (psc, thisaction, 1);
        SCASSERTDEFINEDVALUE (Val1);
        SCASSERTORVALUES (Val1, SCPOINTER,SCLONG);
        fileid = (FILEID *)Val1->Val;
        SCASSERTNOTNULL (fileid);

    }


    if (fileid) result = fgets (Buffer, 4096, ((FILEID *)Val1->Val)->File);
    else result = fgets (Buffer, 4096, NULL);

    SCPutValue (thisaction->Value, result, SCSTRING);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}


SCPROCRESULT SCPutlineProc (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    int result;
    FILEID* fileid = NULL;
    WORD NbParam;
    int Error = 0;


    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);

    SCASSERTMAXPARAM(thisaction, 2);

    NbParam = thisaction->NbParam;
    if (NbParam == 2)
    {
        Val1 = SCGetParamValue (psc, thisaction, 1);
        SCASSERTDEFINEDVALUE (Val1);
        SCASSERTVALUE (Val1, SCSTRING);

        Val2 = SCGetParamValue (psc, thisaction, 2);
        SCASSERTORVALUES (Val2, SCPOINTER, SCLONG);
        fileid = (FILEID *)Val2->Val;
        SCASSERTNOTNULL (fileid);

    }

    if (fileid)
    {
        result = fputs ((char*)Val1->Val, ((FILEID *)Val2->Val)->File);
        fputs ("\n", ((FILEID *)Val2->Val)->File);
    }
    else result = fputs ((char*)Val1->Val, NULL);

    SCAffectValue (thisaction->Value, (void*)result, SCINTEGER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);

    return thisaction->Value;
}



/*------------------------------------------------------------------------*/


SCPROCRESULT SCWriteProc (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    SCValue* Val4 = NULL;
    int Error = 0;
    char FileLoc = LOCDISQU;
    WORD NbParam;
    LONG result;



    SCSETRETURNVALUETYPE(thisaction->Value, SCLONG);

    SCASSERTMINPARAM(thisaction, 3);
    SCASSERTMAXPARAM(thisaction, 4);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERT3ORVALUES (Val1, SCPOINTER, SCLONG, SCSTRING);
    SCASSERTNOTNULL (Val1->Val);


    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTORVALUES(Val2, SCINTEGER, SCLONG);

    Val3 = SCGetParamValue (psc, thisaction, 3);
    SCASSERTDEFINEDVALUE (Val3);
    SCASSERTORVALUES (Val3, SCPOINTER, SCLONG);


    NbParam = thisaction->NbParam;
    if(NbParam > 3)
    {
        Val4 = SCGetParamValue (psc, thisaction, 4);
        SCASSERTDEFINEDVALUE (Val4);
        SCASSERTVALUE (Val4, SCCHAR);
        FileLoc = *(char*)Val4->Val;
    }

    result = OSWriteFile ((FILEID *)Val3->Val,(char *)Val1->Val, (LONG)Val2->Val, 0, &Error);

    SCAffectValue (thisaction->Value, &result, SCLONG);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);


    return thisaction->Value;
}

/*------------------------------------------------------------------------*/


SCPROCRESULT SCRenameProc (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    int Error = 0;
    char FileLoc = LOCDISQU;
    int result;
    WORD NbParam;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);

    SCASSERTMINPARAM(thisaction, 2);
    SCASSERTMAXPARAM(thisaction, 3);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTVALUE (Val1, SCSTRING);

    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTVALUE (Val2, SCSTRING);

    NbParam = thisaction->NbParam;
    if(NbParam > 2)
    {
        Val3 = SCGetParamValue (psc, thisaction, 3);
        SCASSERTDEFINEDVALUE (Val3);
        SCASSERTVALUE (Val3, SCCHAR);
        FileLoc = *(char*)Val3->Val;
    }

    result = OSRenameFile ((char *)Val1->Val , (char *)Val2->Val, FileLoc, &Error);

    SCAffectValue (thisaction->Value, &result, SCINTEGER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);
    return thisaction->Value;
}


/*------------------------------------------------------------------------*/

SCPROCRESULT SCCopyProc (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    int Error = 0;
    char FileLoc = LOCDISQU;
    int result;
    WORD NbParam;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);

    SCASSERTMINPARAM(thisaction, 2);
    SCASSERTMAXPARAM(thisaction, 3);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTVALUE (Val1, SCSTRING);

    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTVALUE (Val2, SCSTRING);

    NbParam = thisaction->NbParam;
    if(NbParam > 2)
    {
        Val3 = SCGetParamValue (psc, thisaction, 3);
        SCASSERTDEFINEDVALUE (Val3);
        SCASSERTVALUE (Val3, SCCHAR);
        FileLoc = *(char*)Val3->Val;
    }

    result = OSCopyFile ((char *)Val1->Val , (char *)Val2->Val, FileLoc, &Error);

    SCAffectValue (thisaction->Value, &result, SCINTEGER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);


    return thisaction->Value;
}


SCPROCRESULT SCReplaceProc (SC* psc,  SCAction* thisaction)
{
    SCValue* Val1 = NULL;
    SCValue* Val2 = NULL;
    SCValue* Val3 = NULL;
    SCValue* Val4 = NULL;
    int Error = 0;
    char FileLoc = LOCDISQU;
    int result;
    WORD NbParam;

    SCSETRETURNVALUETYPE(thisaction->Value, SCINTEGER);

    SCASSERTMINPARAM(thisaction, 3);
    SCASSERTMAXPARAM(thisaction, 4);

    Val1 = SCGetParamValue (psc, thisaction, 1);
    SCASSERTDEFINEDVALUE (Val1);
    SCASSERTVALUE (Val1, SCSTRING);

    Val2 = SCGetParamValue (psc, thisaction, 2);
    SCASSERTDEFINEDVALUE (Val2);
    SCASSERTVALUE (Val2, SCSTRING);


    Val3 = SCGetParamValue (psc, thisaction, 3);
    SCASSERTDEFINEDVALUE (Val3);
    SCASSERTVALUE (Val3, SCSTRING);


    NbParam = thisaction->NbParam;
    if(NbParam > 3)
    {
        Val4 = SCGetParamValue (psc, thisaction, 4);
        SCASSERTDEFINEDVALUE (Val4);
        SCASSERTVALUE (Val4, SCCHAR);
        FileLoc = *(char*)Val4->Val;
    }

    result = OSReplace ((char *)Val1->Val, (char *)Val2->Val, (char *)Val3->Val, FileLoc, 'G' ,0, &Error);


    SCAffectValue (thisaction->Value, &result, SCINTEGER);

    SCSetActionState (psc, thisaction, SCDONE, SCDEFINED, Error);


    return thisaction->Value;
}

