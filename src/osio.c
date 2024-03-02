/* NetProg is an API in C language allowing an homogeneous programming of  */
/* communicating applications                                              */
/* Copyright (C) 2002,2003,2004,2005                                       */
/* Gabriel Jureidini                                                       */
/* Version 2.0 - 17 July 2005                                              */
/*-------------------------------------------------------------------------*/


#define _CRT_SECURE_NO_WARNINGS
#include "osio.h"
#include "osdb.h"



#ifdef _VM_

#pragma linkage(QBYTFIC,OS)
#pragma linkage(DMSCSL,OS)
extern int DMSCSL() ;

char OSLastAddr[5] = "";
char OSLastMach[9] = "";
char OSLastDisk    = '0';
char OSCurrentSFSDirectory[250] = "/";



void* OpenSFS (char* filename, char* mode, int* reasc)
{
    int cret ;


    void* file = malloc (8);
    char filemode[10];

    if (mode[0] == 'r')
    {
        strcpy (filemode , "READ");
        if (mode[1] == '+')
            strcpy (filemode, "WRITE");
    }
    if (mode[0] == 'w')
        if (mode[1] == '+')
            strcpy (filemode, "REPLACE F");
        else
            strcpy (filemode, "REPLACE V");

    if (mode[0] == 'a')
        if (mode[1] == '+')
            strcpy (filemode , "WRITE F");
        else
            strcpy (filemode , "WRITE V");

    DMSCSL("DMSOPEN ", &cret, reasc, filename, strlen (filename), filemode, strlen(filemode), (char*)file) ;

    if ((cret == 8) || (cret == 12))
    {
        free (file);
        return NULL;
    }
    *reasc = 0;
    return file;
}



int CopySFS (char* filename, char* newfilename, int* reasc)
{
    int cret;

    DMSCSL("DMSFILEC", &cret, reasc ,filename, strlen(filename), newfilename, strlen (newfilename),
           "COMMIT REPLACE", 14) ;

    if ((cret == 8) || (cret == 12))
        return -1;
    *reasc = 0;
    return 0;
}

int ReadSFS (char* buffer, int size, FILEID* fileid, int* reasc)
{
    int cret;
    int isread;

    if ((fileid->Type == 't') || (fileid->Type == 'T'))
    {
        DMSCSL("DMSREAD ", &cret, reasc, (char*)fileid->File, 1, size -1 , buffer, size -1, &isread) ;
        buffer[isread] = '\n';
        if (isread != 0) isread++;
    }
    else
        DMSCSL("DMSREAD ", &cret, reasc, (char*)fileid->File, 1, size , buffer, size, &isread) ;


    if ((cret == 8) || (cret == 12))
        return -1;
    *reasc = 0;
    return isread;
}


int WriteSFS (char* buffer, int size, FILEID* fileid, int pos, int* reasc)
{
    int cret ;
    int i = 0;
    int written = size;

    if (size == 0) return 0;

    if ((fileid->Type == 't') || (fileid->Type == 'T'))
    {
        while (i < size)
        {
            if (buffer[i] == '\n') break;
            else i++;
        }

        if (i == size) return 0;
        if (i == 0)               /* buffer[0] == '\n'*/
            DMSCSL("DMSWRITE", &cret, reasc, (char*)fileid->File, 1, 1, " ", size, pos);
        else
            DMSCSL("DMSWRITE", &cret, reasc, (char*)fileid->File, 1, i, buffer, size, pos);
        if ((cret == 8) || (cret == 12))
            return  -1;
        written = i + 1;
    }
    else
    {
        DMSCSL("DMSWRITE", &cret, reasc, (char*)fileid->File, 1, size,  buffer, size, pos);

    }
    if ((cret == 8) || (cret == 12))
        return  -1;
    *reasc = 0;
    return written;
}


int CloseSFS (FILEID* fileid, int* reasc)
{
    int cret ;


    DMSCSL("DMSCLOSE", &cret, reasc, (char*)fileid->File, "COMMIT", 6) ;
    free (fileid->File);
    if ((cret == 8) || (cret == 12))
    {
        return -1;
    }
    *reasc = 0;
    return 0;
}




int EraseSFS (char* filename, int* reasc)
{
    int cret;

    DMSCSL("DMSERASE", &cret, reasc, filename, strlen (filename), "COMMIT", 6)  ;

    if ((cret == 8) || (cret == 12))
    {
        return -1;
    }
    *reasc = 0;
    return 0;

}

int RenameSFS (char* filename, char* filename1, int* reasc)
{
    int cret;

    DMSCSL("DMSRENAM", &cret, reasc, filename, strlen (filename), filename1, strlen (filename1), "COMMIT", 6)  ;

    if ((cret == 8) || (cret == 12))
    {
        return -1;
    }
    *reasc = 0;
    return 0;

}


int ExistDirSFS (char* dirname, int* reasc)
{
    int cret;

    DMSCSL("DMSEXIDI", &cret, reasc, dirname, strlen (dirname), "COMMIT", 6)  ;

    if ((cret == 8) || (cret == 12))
    {
        return -1;
    }
    *reasc = 0;
    return 0;

}


/*-------------------------------------------------------------------*/
/*  Open d'un Directory SFS                                          */
/*  Respectivement, les variables sont :                             */
/*    Dep  -> Fn, Ft, Fm  de depart (eventuellement * * ...)         */
/*    Val  -> Par exemple, FILEEXT                                   */
/*-------------------------------------------------------------------*/



int OpenDirSFS (char * Dep, char *Token, char* Val, int* reasc)
{
    long cret = 0;      /* Code Retour commande DMSCSL                */

    DMSCSL("DMSOPDIR ", &cret, reasc, Dep, (long) strlen(Dep), Val, (long)strlen(Val), Token);
    if ((cret == 8) || (cret == 12))
        return -1;

    return 0;
}

/*-------------------------------------------------------------------*/
/*  Acces aux informations systemes d'un directory                   */
/*  Respectivement, les variables sont :                             */
/*   Token -> Adresse Directory                                      */
/*    Buff -> Buffer de stockage des informations                    */
/*    Nbre -> Nbre de lignes lues                                    */
/*-------------------------------------------------------------------*/


int GetDirSFS (char * Token, char * Buff, int LngBuff, int NBre, int* reasc)
{
    long cret = 0;      /* Code Retour commande DMSCSL                */

    DMSCSL("DMSGETDI", &cret, reasc, Token, Buff, LngBuff, NBre);
    if ((cret == 8) || (cret == 12))
        return -1;
    if (*reasc != 0 && *reasc != 44040)
        return -1;
    *reasc = 0;
    return 0;
}


/*-------------------------------------------------------------------*/
/*  Close d'un Directory SFS                                         */
/*  Respectivement, les variables sont :                             */
/*   Token -> Adresse Directory                                      */
/*    Val  -> Par exemple, COMMIT                                    */
/*-------------------------------------------------------------------*/
int CloseDirSFS (char *Token, int* reasc)
{
    long cret = 0;      /* Code Retour commande DMSCSL                */

    DMSCSL("DMSCLDIR", &cret, reasc, Token, "COMMIT", 6);
    if ((cret == 8) || (cret == 12))
        return -1;

    return 0;
}


#endif


/*=============================================================================================*/




int OSRenameFile (char* filename, char* tofilename, char loc, int* error)
{

#ifdef _VM_
    if ((loc == 'r') || (loc == 'R'))  /* SFS */
        return RenameSFS (filename, tofilename, error);
#endif
    *error =  rename (filename, tofilename);
    if (*error == 0) return 1;
    else return -1;
}




int OSRemoveFile (char* filename, char loc, int* error)
{
#ifdef _VM_
    char Command[100];
    if ((loc == 'r') || (loc == 'R'))  /* SFS */
        return EraseSFS (filename, error);
    else
    {
        strcpy (Command , "ERASE ");
        strcat (Command, filename);
        system (Command);
        return 1;
    }
#else
    *error = remove (filename);
    if (*error == 0) return 1;
    else return -1;
#endif
}




int OSAccess (char* Filename, char* disk, int* error)
{
#ifdef _VM_
    char diskmode;
    char command[100];
    char addr[5];
    char mach[9];
    char* p;
    char filename[400];
    int cret;


    *error = 10003;
    strcpy (filename, Filename);
    p = strtok (filename, " ");
    if (!p) return -1;

    p = strtok (NULL, " ");
    if (!p) return -1;
    p = strtok (NULL, " ");
    if (!p) return -1;
    else strcpy(addr, p);

    p = strtok (NULL, " ");
    if (!p) return -1;
    else strcpy(mach, p);

    *error = 0;
    if ((strcmp (OSLastAddr, addr) == 0) &&
            (strcmp (OSLastMach, mach) == 0))
    {
        *disk = OSLastDisk;
        return 1;
    }
    else OSRelease (OSLastDisk);

    DMSCSL("DMSGETFM",&cret, error, &diskmode);
    if (cret !=0)
        return -1;

    *error = 0;

    *disk = diskmode;

    OSLastDisk = diskmode;
    strcpy (OSLastMach, mach);
    strcpy (OSLastAddr, addr);

    sprintf(command,"EXEC LINKEX %s %s 600 RR READ",mach ,addr);
    /*printf ("%s\n", command);*/
    cret = system(command);
    if (cret)
    {
        *error = 10002;
        return -1;
    }
    sprintf(command,"ACCESS 600 %c", diskmode);
    cret = system(command);
    if (cret)
    {
        *error = 10003;
        return -1;
    }
#else
    return 1;
#endif
}


int OSRelease (char disk)
{
#ifdef _VM_
    int cret = 0;
    char command[100];
    if (disk == '0') return 1;
    sprintf(command,"RELEASE %c (DET", disk);
    cret = system(command);
    OSLastDisk = '0';
    if (cret)
        return -1;
#endif
    return 1;
}



int OSGetNbElt (char* Filename)
{
    char filename[400];
    char* p;

    strcpy (filename, Filename);
    p = strtok (filename, " ");
    if (!p) return 1;
    p = strtok (NULL, " ");
    if (!p) return 1;
    p = strtok (NULL, " ");
    if (!p) return 2;
    p = strtok (NULL, " ");
    if (!p)  return 3;
    else return 4;
}

int OSGetAccessedFileName (char* filename, char* diskmode, int* error)
{
    char Filename[400];
    char FileName[10];
    char FileType[10];
    char* p;
    int nbelt;
    char disk;

    *error = 0;
    *diskmode = '0';
    strcpy (Filename, filename);

    nbelt = OSGetNbElt (Filename);
    if (nbelt < 0)
    {

        *error = 10004;
        return -1;
    }
    if (nbelt >= 3)
    {
        p = strtok (Filename, " ");
        strcpy (FileName, p);
        p = strtok (NULL, " ");
        strcpy (FileType, p);
        if (nbelt > 3)
        {
            strcpy (Filename, filename);
            /*   printf ("***Filename is : %s\n" , Filename);*/
            if (OSAccess (Filename, diskmode, error) < 0)
                return -1;
            disk = *diskmode;
        }
        else
        {
            p = strtok (NULL, " ");
            disk = p[0];
        }

        sprintf(Filename, "%*s %*s %c" ,
                strlen (FileName), FileName, (int)strlen (FileType), FileType, disk);
        strcpy (filename, Filename);
    }
    return 1;
}


FILEID* OSOpenFile (char* filename, char* mode , char type, char loc, int* error)
{
    char osmode[11];
    int modlength;
    FILEID* fileid;
    char Filename[400];
    char* buffer;
#ifdef _VM_
    int nbelt = 0;
#endif

    *error = 0;
    fileid = (FILEID*)malloc (sizeof (FILEID));
    fileid->File = NULL;
    fileid->Disc = '0';
    fileid->Loc = loc;

    modlength = (int)strlen (mode);
    strcpy (osmode, mode);

#if defined(__VOS__) || defined(AS400)
    if (type != 't')         /* ignore text type */
    {
        osmode[modlength] = type;
        osmode[modlength + 1] = 0;
    }
#else
    osmode[modlength] = type;
    osmode[modlength + 1] = 0;
#endif

    fileid->Type = type;
    strcpy (Filename, filename);

#ifdef _VM_
    if ((loc == 'r') || (loc == 'R'))  /* SFS */
    {
        nbelt = OSGetNbElt (Filename);
        if (nbelt == 2) sprintf (Filename, "%s %s", filename, OSCurrentSFSDirectory);
        fileid->File = OpenSFS(Filename, osmode, error);
        if ((!fileid->File) || ((buffer = (char*)malloc (BUFSIZE)) == NULL))
        {
            free (fileid);
            fileid = NULL;
        }
    }
    else
    {
        if (type == 't')
            osmode[modlength] = 0;   /* par defaut c'est texte */

        if (OSGetAccessedFileName(Filename, &fileid->Disc, error) < 0)
            return NULL;
    }
#endif
    fileid->File = (void *)fopen (Filename, osmode);
    if ((!fileid->File) || ((buffer = (char*)malloc (BUFSIZE)) == NULL))
    {
        /*       OSRelease (fileid->Disc);*/
        free (fileid);
        fileid = NULL;
        *error = 10001;
    }
    return fileid;
}



int OSCloseFile (FILEID* fileid)
{
    int error = 0;

#ifdef _VM_
    int cret;
    char command[100];
    if ((fileid->Loc == 'r') || (fileid->Loc == 'R'))  /* SFS */
    {
        CloseSFS(fileid, &error);
        OSFreePipe (fileid->Pipe);
        if (fileid->TransBuffer)
            free ((BYTE*)fileid->TransBuffer);
        free (fileid);
        return error;
    }
#endif
    if (fclose ((FILE*)(fileid->File)))
        error = 1;

#ifdef _VM_
    if (fileid->Disc != '0')
    {
        /*     OSRelease (fileid->Disc);*/
    }
#endif
    free (fileid);
    return error;
}

LONG OSReadFile (FILEID* fileid, char* buffer, LONG size, int* error)
{
    *error = 0;
#ifdef _VM_
    if ((fileid->Loc == 'r') || (fileid->Loc == 'R'))  /* SFS */
        return ReadSFS(buffer, size, fileid, error);
#endif
    return (int)fread (buffer, 1, (size_t)size, (FILE*)fileid->File);

}

LONG OSWriteFile (FILEID* fileid, char* buffer, LONG size, int pos, int* error)
{
    *error = 0;
#ifdef _VM_
    if ((fileid->Loc == 'r') || (fileid->Loc == 'R'))  /* SFS */
        return WriteSFS(buffer, size, fileid, pos, error);
#endif
    return (int)fwrite (buffer, 1, (size_t)size, (FILE*)fileid->File);

}


void OSSeekFile (FILEID* fileid, long offset, int whence)
{

#ifdef _VM_
    if ((fileid->Loc == 'r') || (fileid->Loc == 'R'))  /* SFS */
        return;
#endif
    fseek ((FILE*)fileid->File, offset, whence);
}

/* l'erreur d'ouverture de fichier n'itant pas liie ` l'existence du
  fichier seulement les erreurs 10001 et 44000 peuvent nous dire
  que le fichier n'existe pas riellement... et encore
  retour > 0    existe
  retour < 0    erreur autre
  retour 0     n'existe pas*/

int OSVerifExistence (char* filename, char loc, int* error)
{
    FILEID* fileid;

    if ((fileid = OSOpenFile (filename, "r" , 'b', loc,  error)) != NULL)
    {
        *error = 0;
        OSCloseFile (fileid);
        return 1;
    }
    else
        if ((*error == 44000) || (*error == 10001) ||
                (*error == 10004) || (*error == 90530))
        {
            *error = 0;
            return 0;
        }
        else return -1;
}





LONG OSGetFileSize (char* filename, char type, char loc, int* error)
{
    FILEID* file;
    LONG filesize = 0;
    LONG isread = 0;
#ifdef _VM_
    char diskmode;
    char Filename[400];
    typedef _Packed struct {
        char FNAME[8];
        char FTYPE[8];
        char FMODE[2];
        char Gap[2];
        long int NBYTE;
        long int RETC;
        char Trash[52];
    } PARAM;
    PARAM FileTest;
    char* p;
    int i;
#endif
    *error = 0;

    if ((type != 't') && (type != 'T')) type = 'b';
    else type = 't';
#ifdef _VM_
    if ((type == 'b') && (loc != 'r') && (loc != 'R'))
    {
        strcpy (Filename, filename);

        if (OSGetAccessedFileName(Filename, &diskmode, error) < 0)
        {
            /*   printf ("I Cant access\n");*/
            return -1;
        }
        /*   printf ("FILE NAME IS : %s\n", Filename);
           printf ("diskmode is : %c\n",  diskmode);*/

        strncpy (FileTest.FMODE, "  ", 2);
        FileTest.NBYTE = 0;
        FileTest.RETC = 0 ;

        p = strtok (Filename, " ");
        if (!p) return -1;
        for (i = 0; i < strlen(p); i++) FileTest.FNAME[i] = toupper(p[i]);
        for (i = strlen(p); i < 8; i++) FileTest.FNAME[i] = ' ';
        p = strtok (NULL, " ");
        if (!p) return -1;
        for (i = 0; i < strlen(p); i++) FileTest.FTYPE[i] = toupper(p[i]);
        for (i = strlen(p); i < 8; i++) FileTest.FTYPE[i] = ' ';
        p = strtok (NULL, " ");
        if (p)
        {
            for (i = 0; i <strlen(p); i++) FileTest.FMODE[i] = toupper(p[i]);
            for (i = strlen(p); i < 2; i++) FileTest.FMODE[i] = ' ';
        }
        QBYTFIC(&FileTest);
        /* OSRelease (diskmode);  */
        if (FileTest.RETC)
        {
            *error = (int)FileTest.RETC;
            return -1;
        }
        else return FileTest.NBYTE;
    }
#endif

    file = OSOpenFile (filename, "r", type, loc, error);
    if (!file)
        return -1;
    else
#ifdef WINDOWS
        if (type == 'b')
        {
            OSSeekFile (file, 0, SEEK_END);
            filesize = ftell ((FILE*)(file->File));
        }
        else
#endif
        {


            char* buffer = (char*)malloc (MAXBUFSIZE);
            if (!buffer)
            {

                OSCloseFile (file);
                return -1;
            }
            else
                while ((isread = OSReadFile (file, buffer, MAXBUFSIZE, error)) > 0)
                    filesize += isread;
            free ((BYTE*)buffer);

        }
    OSCloseFile (file);
    if (isread < 0) return -1;
    else return filesize;
}




int OSCopyFile (char* filename, char* newfilename, char loc, int* error)
{
    FILEID* file;
    FILEID* newfile;
    char* buffer;

    LONG isread = 0;
    *error = 0;

#ifdef _VM_
    if ((loc == 'r') || (loc == 'R'))  /* SFS */
        return CopySFS(filename, newfilename, error);
#endif
    file = OSOpenFile (filename, "r", 'b', loc, error);
    if (!file)
    {
        return -1;
    }

    newfile = OSOpenFile (newfilename, "w", 'b', loc, error);
    if (!newfile)
    {
        OSCloseFile (file);
        return -1;
    }
    buffer  = (char*)malloc (MAXBUFSIZE);
    if (!buffer)
    {
        OSCloseFile (file);
        OSCloseFile (newfile);
        return -1;
    }
    while ((isread = OSReadFile (file, buffer, MAXBUFSIZE, error)) > 0)
        OSWriteFile (newfile, buffer, isread, 0, error);
    free ((BYTE*)buffer);
    OSCloseFile (file);
    OSCloseFile (newfile);
    if (*error != 0) return -1;
    return 0;
}


/*************************************************************************/



int OSChangeDir (char* NewDir, char FileLoc, int* error)
{
#if defined(AS400)
    *error = 1;
    return -1;
#else


#if defined(_VM_)
    if ((FileLoc == 'r') || (FileLoc == 'R'))  /* SFS */
    {
        char sfsdir[250];
        char realdir[250];
        char sfsformat[250];
        char* start;

        /* .. see backwards */


        if (NewDir[0] != '/')
        {
            if (OSCurrentSFSDirectory [strlen (OSCurrentSFSDirectory) - 1] == '/')
                sprintf (sfsdir, "%s%s", OSCurrentSFSDirectory, NewDir);
            else
                sprintf (sfsdir, "%s/%s", OSCurrentSFSDirectory, NewDir);
        }
        else
            strcpy (sfsdir, NewDir);

        OSTakeOffBackpoints (sfsdir, realdir);
        /*    printf ("sfsdir : %s\n", sfsdir);
        printf ("realdir : %s\n", realdir);*/

        if (OSGetSFSFormatFromReper (realdir, sfsformat) < 0)
        {
            *error = 19;
            return -1;
        }
        /*  printf ("sfsformat : %s\n", sfsformat);*/

        if (ExistDirSFS(sfsformat, error) < 0)
        {
            /*   printf ("error exist dir : %d\n", *error);*/
            return -1;
        }
        else
        {
            strcpy (OSCurrentSFSDirectory, realdir);
            start = strchr (OSCurrentSFSDirectory, ' ');
            if (start) *start = 0;
        }
        return 1;
    }
#else
    *error = _chdir(NewDir);
    if (*error != 0)
    {
        *error = errno;
        return -1;
    }
    else return 1;
#endif
#endif
}



int OSMakeDir (char* Dir, char FileLoc, int* error)
{
#if defined(_VM_) || defined(AS400)
    *error = 1;
    if ((FileLoc == 'r') || (FileLoc == 'R'))  /* SFS */
        return -1;
    else
        return -1;
#else
#ifdef __VOS__
    char_varying (256)  path_name;
    short int error_code;
    char Pwd[256];
    OSGetDir (Pwd, FileLoc, error);
    if (*error == 0)
    {
        strcat (Pwd, ">");
        strcat (Pwd, Dir);
        strcpy (Dir, Pwd);
        strcpy_vstr_nstr(&path_name, Dir);
    }
    else
        return -1;
    s$create_dir (&path_name, &error_code);
    if (error_code != 0)
    {
        char_varying (32) caller = "";
        char_varying (256) message_text = "";
        *error = error_code;
        s$error (&error_code, &caller, &message_text);
        return -1;
    }
    else return 1;
#else

    *error = _mkdir(Dir);
    if (*error != 0)
    {
        *error = errno;
        return -1;
    }
    else return 1;
#endif
#endif
}


int OSRemoveDir (char* Dir, char FileLoc, int* error)
{
#if defined(_VM_) || defined(AS400)
    if ((FileLoc == 'r') || (FileLoc == 'R'))  /* SFS */
        return -1;
    else
        return -1;
#else
#ifdef __VOS__
    char_varying(256)  path_name;
    char_varying (32)  caller = "";
    short int error_code;
    short int switchpar = 1;
    char Pwd[256];
    OSGetDir (Pwd, FileLoc, error);
    if (*error == 0)
    {
        strcat (Pwd, ">");
        strcat (Pwd, Dir);
        strcpy (Dir, Pwd);
        strcpy_vstr_nstr(&path_name, Dir);
    }
    else
        return -1;
    s$delete_dir (&path_name, &caller, &switchpar, &error_code);

    if (error_code != 0)
    {
        *error = error_code;
        return -1;
    }
    else return 1;
#else
    *error = _rmdir(Dir);
    if (*error != 0)
    {
        *error = errno;
        return -1;
    }
    else return 1;
#endif
#endif
}


/* make sure it ends with '/' */


char* OSGetDir (char* dir, char FileLoc, int* error)
{
#if defined(AS400) 
    *error = 1;
    return NULL;
#endif

#if defined(__VOS__)

    char_varying(256)  path_name;
    s$get_current_dir (&path_name);

    strcpy_nstr_vstr(dir, &path_name);
    return dir;
#else
#if defined(_VM_) 
    if ((FileLoc == 'r') || (FileLoc == 'R'))  /* SFS */
    {
        strcpy (dir, OSCurrentSFSDirectory);
        if (dir[strlen (dir) -1] != '/')   strcat (dir, "/");
        return dir;
    }
    else
    {
        strcpy (dir, "");
    }
#else
    if (_getcwd (dir, 250) == NULL)
        *error = errno;

    if ((dir[strlen (dir) -1] != '\\') &&
            (dir[strlen (dir) -1] != '/'))
#ifdef WINDOWS
        strcat (dir, "\\");
#else
    strcat (dir, "/");
#endif
    return dir;
#endif
#endif
}


void OSGetFileName (IDENTFICH* identfile, char* filename)
{
    int ltype, lname, lrep, i;
    char format[60];
    ltype = lname = lrep = 0;
    i = 0;


    while ((i < LNGNOMFI) &&
            (identfile->ficfname[i] != ' ') &&
            (identfile->ficfname[i++] != '\0'))
        lname++;
    i = 0;
    while ((i < LNGNOMFI) &&
            (identfile->ficftype[i] != ' ') &&
            (identfile->ficftype[i++] != '\0'))
        ltype++;
    i = 0;
    while ((i < LNGLOCAL) &&
            (identfile->zloc.localis[i] != ' ') &&
            (identfile->zloc.localis[i++] != '\0'))
        lrep++;

    switch (identfile->zloc.typloc) {
    case LOCREPER :
        sprintf (format,"%s%d%s%d%s%d%s",
                 "%.", lname,"s %.", ltype, "s %.", lrep, "s");
        sprintf(filename, format ,identfile->ficfname,
                identfile->ficftype, identfile->zloc.localis) ;
        break;
    case LOCDISQU :
#if defined(_VM_) 
        sprintf (format,"%s%d%s%d%s%d%s",
                 "%.", LNGNOMFI, "s %.", LNGNOMFI, "s %.", LNGLOCAL, "s");
        sprintf(filename, format ,identfile->ficfname,
                identfile->ficftype, identfile->zloc.localis);
#else
        sprintf (format,"%s%d%s%d%s%d%s",
                 "%.", lrep, "s%.", lname, "s.%.", ltype, "s");
        sprintf(filename, format ,identfile->zloc.localis, identfile->ficfname,
                identfile->ficftype);
        /*   sprintf(filename, "%.*s%.*s.%.*s",lrep,identfile->zloc.localis, lname,identfile->ficfname,ltype,
        identfile->ficftype);*/
#endif
        break;
    }
}




char* OSGetFileNameExtension (char* extension, char* fromname)
{

    char name[200];
    char filename[250];
    char repertory[250];
    extension[0] = 0;
    OSGetOnlyName (fromname, filename, repertory, 'D');
    OSGetSFSFormatFromFileName (filename, name, extension);
    return extension;
}


char* OSGetOnlyName (char* fromname, char* filename, char* Repertory, char FileLoc)
{
    int i = 0;
    int l;
    char name[500];
    char sep;
#ifdef _VM_
    char Repertory1[500];
#endif

    Repertory[0] = 0;

#ifdef _VM_
    if ((FileLoc != 'r') && (FileLoc != 'R'))  /* SFS */
    {
        strcpy (filename, fromname);
        return fromname;
    }
#endif

    while (fromname[i] != 0)   i++;
    if (i == 0)
    {
        filename[0] = '\0';
        Repertory[0] = '\0';
        return filename;
    }
    strncpy (name, fromname, i);
    name[i] = '\0';

    l = (int)strlen (name) -1;
    sep = '/';

    for (i = l; ((i != 0) && ((name[i] != sep) && (name[i] != '\\') && (name[i] != '>'))) ; i--);
    if ((name[i] != sep)  &&
            (name[i] != '\\') &&
            (name[i] != '>'))
        --i;

    strcpy (filename , name + i + 1);
    strncpy (Repertory, name , i + 1);
    Repertory[i + 1] = '\0';
#ifdef _VM_
    if ((FileLoc == 'r') || (FileLoc == 'R'))  /* SFS */
    {
        if (strstr (Repertory, ".."))
        {
            if (OSCurrentSFSDirectory [strlen (OSCurrentSFSDirectory) - 1] == '/')
                sprintf (Repertory1, "%s%s", OSCurrentSFSDirectory, Repertory);
            else
                sprintf (Repertory1, "%s/%s", OSCurrentSFSDirectory, Repertory);
            OSTakeOffBackpoints (Repertory1, Repertory);
        }
    }
#endif
    return filename;
}





int OSSetFileName (char* destfilename, char* repertory, char* filename, char FileLoc)
{
    char sfsname[100];
    char sfstype[100];
    char destlocal[250];
    int size;

    if ((FileLoc == 'r') || (FileLoc == 'R'))  /* SFS */
    {
        if ((repertory) && (strcmp (repertory, "") != 0))
        {
            OSGetSFSFormatFromReper (repertory, destlocal);
            OSGetSFSFormatFromFileName (filename, sfsname, sfstype);
            sprintf (destfilename, "%s %s %s", sfsname, sfstype, destlocal);
        }
        else
        {
            OSGetSFSFormatFromFileName (filename, sfsname, sfstype);
            sprintf (destfilename, "%s %s", sfsname, sfstype);
        }
    }
    else
    {
        if ((repertory) && (strcmp (repertory, "") != 0))
        {
            size = (int)strlen (repertory);
            if ((repertory[size -1] != '\\') &&
                    (repertory[size -1] != '>') &&
                    (repertory[size -1] != '/'))
#ifdef __VOS__
                sprintf (destfilename,  "%s>%s", repertory, filename);
#else               
                sprintf (destfilename,  "%s/%s", repertory, filename);
#endif
            else
                sprintf (destfilename,  "%s%s",  repertory, filename);
        }
        else sprintf (destfilename, "%s", filename);
    }
    return 1;
}

/*
     OSGetCMSFormatFromFileName (filename, sfsname, sfstype, sfsloc);
     sprintf (destfilename, "%s %s %s", sfsname, sfstype, sfsloc);
   }
  printf ("setfilename %s\n", destfilename);
 
#else
*/ 
int OSGetSFSFormatFromReper (char* directory, char* sfsdir)
{
    char* start;
    char* second;

    if (directory[0] != '/') return -1;

    strcpy (sfsdir, directory + 1);

    start = strchr (sfsdir, ' ');
    if (start) *start = 0;

    if (strcmp (sfsdir, "") == 0) return 1;

    if (sfsdir [strlen (sfsdir) - 1] == '/')
        sfsdir [strlen(sfsdir) - 1] = 0;


    start = strchr (sfsdir, '/');
    if (start) *start = ':';
    else
    {

        strcat (sfsdir, ":");
        return 1;
    }
    second = strchr (sfsdir, '/');
    if (second) *second = '.';
    else
    {

        strcat (sfsdir, ".");
        return 1;
    }

    while (start = strchr (sfsdir, '/'))
    {
        *start = '.';
    }

    return 1;
}


int OSGetSFSFormatFromFileName (char* filename, char* sfsname, char* sfstype)
{
    char string[250];
    char seps[]   = ".";
    char* token;


    strcpy (string, filename);
    sfsname[0]  = sfstype[0] = 0;
    token = strtok( string, seps );
    if (token != NULL)
    {
        strcpy (sfsname, token);
        token = strtok( NULL, seps );
        if (token != NULL)
            strcpy (sfstype, token);
    }
    /*   printf ("filename is now : %s %s \n", sfsname, sfstype);*/
    return 1;
}


int OSGetCMSFormatFromFileName (char* filename, char* sfsname, char* sfstype, char* sfsloc)
{
    char string[250];
    char seps[]   = " .";
    char* token;

    strcpy (string, filename);
    sfsname[0]  = sfstype[0]  = sfsloc[0] = 0;;
    token = strtok( string, seps );
    if (token != NULL)
    {
        strcpy (sfsname, token);
        token = strtok( NULL, seps );
        if (token != NULL)
        {
            strcpy (sfstype, token);
            token = strtok( NULL, seps );
            if (token != NULL)
                strcpy (sfsloc, token);
            else strcpy (sfsloc, "A");
        }
    }
    /*   printf ("filename is now : %s %s %s\n", sfsname, sfstype, sfsloc);*/
    return 1;
}



int OSTakeOffBackpoints (char* directory, char* realdir)
{

    char string[250];
    char seps[]   = "/";
    char t[10][50];
    int i = 0;
    int j;

    char* token;
    strcpy (string, directory);

    token = strtok( string, seps );

    while( token != NULL )
    {
        printf( " %s\n", token );
        if (strcmp (token , "..") == 0)
        {
            if (i != 0)  strcpy (t[--i], "");
        }
        else  strcpy (t[i++], token);
        token = strtok( NULL, seps );
    }
    realdir[0] = '/';
    realdir[1] = 0;

    for (j= 0; j < i; j++)
    {
        strcat (realdir, t[j]);
        strcat (realdir, "/");
    }
    printf("%s\n", realdir);

    return 1;
}



char* OSFormateDate(long date, char* datcha, BOOL WithSep)
{
    int v2, v3, i, flagstop;
    int Qan[12] ;
    int AA, QQ;
    div_t dres ;
    char annee[3];
    char jour[4];
    char format[6];

    flagstop = 0 ;

    Qan[0] = 31 ;  Qan[1] = 59 ;  Qan[2] = 90 ;
    Qan[3] = 120;  Qan[4] = 151;  Qan[5] = 181;
    Qan[6] = 212;  Qan[7] = 243;  Qan[8] = 273;
    Qan[9] = 304;  Qan[10] = 334; Qan[11] = 365;

    sprintf (format, "%.5ld", date);

    strncpy(annee, format, 2);
    annee[2] = '\0';


    strncpy(jour, format + 2, 3);
    jour[3] = '\0';

    AA = atoi (annee) + 1990;
    QQ = atoi (jour);
    dres = div (AA, 4) ;
    if (dres.rem == 0)
        for (i = 1; i < 12; i++)
            Qan[i]++ ;
    v2 = QQ ;
    if (v2 != 0)
    {
        for (i = 0; flagstop == 0; i++)
        {
            if (QQ <= Qan[i])
                flagstop = 1 ;
            else
                v2 = QQ - Qan[i] ;
        }
    }
    else i = 0 ;


    if (WithSep)
    {
        if (AA >= 2000) v3 = AA - 2000;
        else v3 = AA - 1900 ;
        sprintf(datcha, "%02d/%02d/%02d", v2, i, v3) ;
    }
    else
        sprintf(datcha, "%04d%02d%02d", AA, i, v2);

    datcha[8] = '\0' ;
    return(datcha) ;
}

char* OSFormateHour(long hour, char* hourcha, BOOL WithSep)
{
    char chour[20];
    char heure[3];
    char minute[3];
    char sec[3];

    sprintf (chour, "%06ld", hour);
    strncpy (heure, chour, 2);
    strncpy (minute, chour + 2, 2);
    strncpy (sec, chour + 4, 2);

    heure[2] = minute[2] = sec[2] = '\0';
    if (WithSep)
        sprintf (hourcha, "%.2s:%.2s:%.2s", heure, minute, sec);
    else
        sprintf (hourcha, "%.2s%.2s%.2s", heure, minute, sec);
    return hourcha;

}



void OSDateHour(DWORD* dat, DWORD* hour)
{
    time_t dh;
    char buf1[7];
    char buf2[4];
    struct tm *gmt;
    int ann;

    time(&dh);
    gmt = localtime(&dh);

    strftime(buf1,5,"%Y",gmt);
    ann = atoi(buf1);
    ann -= 1990;

    strftime(buf2, 4, "%j", gmt);
    sprintf(buf1,"%02d%s", ann, buf2);
    *dat = (DWORD)atol(buf1);

    strftime(buf1,7,"%H%M%S",gmt);
    *hour = (DWORD)atol(buf1);
    return;
}

DWORD OSIncrementHour (DWORD hour)
{
    char heure[7];
    char cminutes[3];
    char cseconds[3];
    char chour[3];
    int iminutes, iseconds, ihours;


    sprintf (heure, "%06ld", hour);
    sprintf (chour, "%.2s", heure);
    sprintf (cminutes, "%.2s", heure + 2);
    sprintf (cseconds, "%.2s", heure + 4);
    iminutes = atoi (cminutes);
    iseconds = atoi (cseconds);
    ihours = atoi (chour);

    if (iseconds == 59)
    {
        if (iminutes == 59)
        {
            if (ihours == 23) ihours = 0;
            else ihours++ ;
            iminutes = 0 ;
        }
        else iminutes ++ ;
        iseconds = 0 ;
    }
    else iseconds ++ ;

    sprintf (heure, "%02d%02d%02d", ihours, iminutes, iseconds);

    return (DWORD)atol (heure);
}


int OSStrcmp(char* string1, char* string2)
{
    char *p1;
    char *p2;
    char car1, car2;
    int length = 0;

    if ((!string1) || (!string2))
        return -1;

    length = (int)strlen(string1);

    if (length != (int)strlen(string2))
        return -1;


    for (p1 = string1, p2 = string2; p1 < string1 + length; p1++, p2++)
    {

        if (islower(*p1))
            car1 = toupper(*p1);
        else  car1 = *p1;

        if (islower(*p2))
            car2 = toupper(*p2);
        else
            car2 = *p2;

        if (car1 != car2)    return -1;
    }

    return 0;
}




int OSStrncmp(char* string1, char* string2, int nbcar)
{
    char *p1;
    char *p2;
    char car1, car2;

    if ((!string1) || (!string2))
        return -1;

    if ((nbcar > (int)strlen(string1)) || (nbcar > (int)strlen(string2)))
        return -1;


    for (p1 = string1, p2 = string2; p1 < string1 + nbcar; p1++, p2++)
    {

        if (islower(*p1))
            car1 = toupper(*p1);
        else
            car1 = *p1;

        if (islower(*p2))
            car2 = toupper(*p2);
        else
            car2 = *p2;

        if (car1 != car2)  return -1;
    }

    return 0;
}


int OSStrEqNc(char* s1, char* s2)
{
    while(1)
    {
        if (!(*s1))
        {
            if (!(*s2))
            {
                return 1;
            }
            else return 0;
        }
        else
            if (!(*s2)) return 0;
        if (isalpha(*s1))
        {
            if (tolower(*s1) != tolower(*s2)) return 0;
        }
        else
            if ((*s1) != (*s2))
                return 0;
        s1++;
        s2++;
    }
}



PIPE* OSAllocatePipe (void)
{
    PIPE* pcom;
    pcom = (PIPE*)malloc (sizeof (PIPE));
    if (!pcom)
    {
        return  NULL;
    }
    pcom->Input      = NULL;
    pcom->Output     = NULL;
    pcom->fileid     = NULL;
    pcom->connection = NULL;
    return pcom;
}

void OSFreePipe (PIPE* pcom)
{
    if (pcom)
    {
        if (pcom->connection) /*database*/
        {
            List* l = (List*)pcom->Input;
            OSStatement* pstatement;
             while (l)
            {
                pstatement = (OSStatement *)l->car;
                ListRemove (&l, pstatement);
                OSFreeStatement (pstatement);
            }
            l = (List*)pcom->Output;
            while (l)
            {
                pstatement = (OSStatement *)l->car;
                ListRemove (&l, pstatement);
                OSFreeStatement (pstatement);
            }
        }
        else         /* file*/
        {
            if (pcom->Input)
                OSFreeChannel (pcom->Input);
            if (pcom->Output)
                OSFreeChannel (pcom->Output);
        }
        free ((BYTE*)pcom);
    }
}




CHANNEL* OSAllocateChannel (void)
{
    CHANNEL* pchannel;
    pchannel = (CHANNEL*)malloc (sizeof (CHANNEL));
    if (pchannel)
    {
        pchannel->Buffer = (char*)malloc(BUFSIZE);
        if (!pchannel->Buffer)
        {
            free ((BYTE*)pchannel);
            pchannel = NULL;
        }
    }
    if (pchannel)
    {
        pchannel->Size = BUFSIZE;
        pchannel->BufPtr = pchannel->Buffer;
        pchannel->BufCnt = 0;
        pchannel->LenLastReq = 0;
    }
    return pchannel;
}


void OSFreeChannel (CHANNEL* pchannel)
{
    if (pchannel->Buffer) free ((BYTE *)pchannel->Buffer);
    free (pchannel);
}


int OSDir (char* Name, char FileLoc, BUFFERPARM* Buffer)
{
    char OnlyFileName[120];
    char LocalRepertory[50];
    List* ListFiles = NULL;
    char* Buf = 0;
    char FileLine[300];
    DWORD BufSize = 0;
    int NbrFiles = 0;


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
        Buffer->BufferContent = (char*)malloc (MAXBUFSIZE);
        Buffer->BufferContent[0] = 0;
    }

    while (ListFiles)
    {
        FILEDATA* filedata = (FILEDATA *)ListFiles->car;
        sprintf (FileLine, "%s %s %d\r\n",  filedata->Name,   ((filedata->Type == TYPEDIRECTORY) ? "<Dir>" : ((filedata->Type == TYPELINK) ? "<Link>" : "")), filedata->Size);
        strcat (Buffer->BufferContent, FileLine);
        BufSize += (int)strlen (FileLine);
        free (ListFiles->car);
        ListRemove (&ListFiles, ListFiles->car);
    }
    Buffer->BufferType    = 'T';
    Buffer->BufferSize    = BufSize;
    return NbrFiles;
}



List* OSList (char* Name, char FileLoc)
{
    char OnlyFileName[120];
    char LocalRepertory[50];
    List* ListFiles = NULL;
    int NbrFiles = 0;


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
    return ListFiles;
}


